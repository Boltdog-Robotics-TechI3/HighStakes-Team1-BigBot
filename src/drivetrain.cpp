#include "main.h"

okapi::Motor frontRight(1);
okapi::Motor backRight(2);
okapi::Motor topRight(-3);

okapi::Motor frontLeft(-8);
okapi::Motor backLeft(9);
okapi::Motor topLeft(-10);

okapi::MotorGroup right({frontRight, topRight, backRight});
okapi::MotorGroup left({frontLeft, topLeft, backLeft});

pros::IMU gyro(12);

std::shared_ptr<okapi::ChassisControllerPID> chassis = std::dynamic_pointer_cast<ChassisControllerPID>(ChassisControllerBuilder()
	.withMotors(left, right)
	.withDimensions({AbstractMotor::gearset::blue, (48.0/36.0)}, {{2.75_in, 11.75_in}, imev5BlueTPR})
	.withGains(
		{0.0017, 0.0, 0.0000005}, 
		{2.80, 0.0000, 1}, 
		{0, 0, 0})
	.withClosedLoopControllerTimeUtil()
	.build());

std::shared_ptr<ChassisModel> drivetrain = chassis->getModel();

// PID constants for turning
PID turnPID = {
    .kP = 2.5,
    .kI = 0.0,
    .kD = 0.0,
    .smallErrorRange = 0.4, // degrees
    .smallErrorTimeout = 100.0, // milliseconds
    .largeErrorRange = 0.75, // degrees
    .largeErrorTimeout = 500.0, // milliseconds
    .minVelocity = 3.0
};

void setDriveCurrentLimt(int limit){
	frontLeft.setCurrentLimit(limit);
	frontRight.setCurrentLimit(limit);
	topLeft.setCurrentLimit(limit);
	topRight.setCurrentLimit(limit);
	backLeft.setCurrentLimit(limit);
	backRight.setCurrentLimit(limit);
}

/**
* @brief Turn the robot to an absolute heading using PID control
* @param heading The heading to turn to in degrees: [0,360)
* @param maxVelocity The maximum velocity of the robot in the range [0,1], default to 1
* @param timeout Timeout before the robot gives up in milliseconds, default to 5000
* @param behavior The behavior of the turn, LEFT, RIGHT, or DEFAULT (closest direction), default to DEFAULT
*/
void turnToHeading(double heading, double maxVelocity, int timeout, enum TurnBehavior behavior) {
    // Get the current heading
	double currentHeading = gyro.get_heading();
	double error = heading - currentHeading;
	
    if (behavior == TurnBehavior::DEFAULT) {
        // Normalize the error
        if (error > 180) {
            error = error - 360;
        } else if (error < -180) {
            error = 360 + error;
        }
    } else if (behavior == TurnBehavior::LEFT) {
        if (error > 0) {
            error = -360 + error;
        }
    } else if (behavior == TurnBehavior::RIGHT) {
        if (error < 0) {
            error = -360 + error;
        }
    }

	turnAngle(error, maxVelocity, timeout);
}

/** 
* @brief Turn the robot by the specified angle using PID control
* @param angle angle in degrees
* @param maxVelocity maximum velocity of the robot in the range [0,1], default to 1
* @param timeout timeout before the robot gives up in milliseconds, default to 5000
*/
void turnAngle(double angle, double maxVelocity, int timeout) {
    // Calculate the target angle
    double target = angle + gyro.get_rotation();
    double error = angle;
	double previousError = 0;
	double integral = 0;

    // Set the start time and exit time
	int startTime = pros::millis();
	int exitTime = pros::millis() + timeout;

    int smallErrorEntryTime = -1; // Time when the robot entered the small error range
    int largeErrorEntryTime = -1; // Time when the robot entered the large error range

    // Loop until the target is reached or the timeout is reached
    bool targetReached = false;
	while (!targetReached && pros::millis() < exitTime) {
        // Calculate the velocity
		double velocity = turnPID.kP * error + (error - previousError) * turnPID.kD + turnPID.kI * integral;
        if (velocity > 0) {
            velocity = std::clamp(velocity+turnPID.minVelocity, 0.0, 600.0*maxVelocity);
        } else if (velocity < 0) {
            velocity = std::clamp(velocity-turnPID.minVelocity, -600.0*maxVelocity, 0.0);
        }

        // Set the motor velocities
		rightMotorGroup.moveVelocity(-velocity);
		leftMotorGroup.moveVelocity(velocity);
		pros::delay(5);

        // Determine if within small error range
        if (abs(error) < turnPID.smallErrorRange) {
            // Set the entry time if not already set
            if (smallErrorEntryTime == -1) {
                smallErrorEntryTime = pros::millis();
            }
            // Check if the timeout has been reached
            if (smallErrorEntryTime + turnPID.smallErrorTimeout < pros::millis()) {
                targetReached = true;
                break;
            }
        } else {
            // Reset the entry time
            smallErrorEntryTime = -1;
        }
        // Determine if within large error range
        if (abs(error) < turnPID.largeErrorRange) {
            // Set the entry time if not already set
            if (largeErrorEntryTime == -1) {
                largeErrorEntryTime = pros::millis();
            }
            // Check if the timeout has been reached
            if (largeErrorEntryTime + turnPID.largeErrorTimeout < pros::millis()) {
                targetReached = true;
                break;
            }
        } else {
            // Reset the entry time
            largeErrorEntryTime = -1;
        }

        // Update the error values
		previousError = error;
		error = target - gyro.get_rotation();
        integral = integral * 0.8 + error;
	}
    drivetrain->stop();
    pros::delay(50);
}

void drivetrainInit(){
	drivetrain->setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
	gyro.reset();
	while(gyro.is_calibrating());
}
