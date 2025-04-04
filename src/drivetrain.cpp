#include "main.h"

double gearRatio = 3/4; //450 rpm
double wheelDiameter = 2.75; //in

pros::Motor frontRight(1);
pros::Motor backRight(2);
pros::Motor topRight(-3);
pros::Motor frontLeft(-8);
pros::Motor backLeft(9);
pros::Motor topLeft(-10);

pros::MotorGroup right({1, -3, 2});
pros::MotorGroup left({-8, -10, 9});

pros::IMU gyro(12);

//.withDimensions({AbstractMotor::gearset::blue, (48.0/36.0)}, {{2.75_in, 11.75_in}, imev5BlueTPR})

// PID constants for lateral motion
PID lateralPID = {
    .kP = 0.1,
    .kI = 0.0,
    .kD = 0.0,
    .smallErrorRange = 1.0, // inches
    .smallErrorTimeout = 1000.0, // milliseconds
    .largeErrorRange = 100.0, // inches
    .largeErrorTimeout = 5000.0, // milliseconds
};
// PID constants for turning
PID turnPID = {
    .kP = 2.75,
    .kI = 0.0,
    .kD = 0,
    .smallErrorRange = 0.4, // degrees
    .smallErrorTimeout = 100.0, // milliseconds
    .largeErrorRange = 1.0, // degrees
    .largeErrorTimeout = 400.0, // milliseconds
    .minVelocity = 2.5
};

void setDriveCurrentLimt(int limit){
	frontLeft.set_current_limit(limit);
	frontRight.set_current_limit(limit);
	topLeft.set_current_limit(limit);
	topRight.set_current_limit(limit);
	backLeft.set_current_limit(limit);
	backRight.set_current_limit(limit);
}

void arcadeDrive(double forward, double turn) {
    if (forward != 0 || turn != 0) {
        left.move_voltage(forward + turn);
        right.move_voltage(forward - turn);
    } else {
        left.move_voltage(0);
        right.move_voltage(0);
    }   
}

double getTargetIMEOffset(double distance) {
    double coefficient = 1.0;
    switch (left.get_encoder_units()) {
        case pros::MotorUnits::degrees:
            coefficient = 360;
            break;
        case pros::MotorUnits::rotations:
            break;
            case pros::MotorUnits::counts:
            switch (left.get_gearing()) {
                case pros::MotorGears::green:
                    coefficient = 900;
                    break;
                case pros::MotorGears::red:
                    coefficient = 1800;
                    break;
                case pros::MotorGears::blue:
                    coefficient = 300;
                    break;
                default:
                    throw std::invalid_argument("Invalid gear set");
            
            }
            break;
        default:
            throw std::invalid_argument("Invalid encoder units");
    }
    return distance * coefficient * gearRatio / (wheelDiameter * M_PI);
}

void driveDistance(double distance, double voltage) { //distance is in inches
    double target = getTargetIMEOffset(distance);
    left.move_relative(target, voltage);
    right.move_relative(target, voltage);
}

void driveDistancePID(double distance, double maxVoltage, double timeout, bool async) {
    pros::Task drivePIDTask([=] {
        double leftTarget = left.get_position() + getTargetIMEOffset(distance);
        double rightTarget = right.get_position() + getTargetIMEOffset(distance);
        double target = (leftTarget + rightTarget) / 2;

        double position = (left.get_position() + right.get_position()) / 2;
        double error = target - position;
        double tolerance = 10;

        double startTime = pros::millis();
        double endTime = startTime + timeout;


        double integral = 0;
        double derivative = 0;
        double lastError = 0;

        while (std::abs(error) > target + tolerance && pros::millis() < endTime) {
            double voltage = error * lateralPID.kP + integral * lateralPID.kI + derivative * lateralPID.kD;
            if (voltage > maxVoltage) {
                voltage = maxVoltage;
            } else if (voltage < -maxVoltage) {
                voltage = -maxVoltage;
            }

            left.move_voltage(voltage);
            right.move_voltage(voltage);

            position = (left.get_position() + right.get_position()) / 2;
            error = target - position;
            integral += error;
            derivative = error - lastError;
            lastError = error;

            pros::delay(10);
        }
        left.move_voltage(0);
        right.move_voltage(0);
    });
    if (!async) {
        drivePIDTask.join();
    }
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
            error = 360 + error;
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
		right.move_velocity(-velocity);
		left.move_velocity(velocity);
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
    left.brake();
    right.brake();
    pros::delay(50);
}

void drivetrainInit(){
	left.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	right.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    gyro.set_data_rate(5);
	gyro.reset(true);
	while(gyro.is_calibrating());
}
