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

void setDriveCurrentLimt(int limit){
	frontLeft.setCurrentLimit(limit);
	frontRight.setCurrentLimit(limit);
	topLeft.setCurrentLimit(limit);
	topRight.setCurrentLimit(limit);
	backLeft.setCurrentLimit(limit);
	backRight.setCurrentLimit(limit);
}

/// @brief Turns the robot to an absolute heading using PID control
/// @param heading The heading to turn to
/// @param timeout Timeout before the robot gives up in seconds, default to 10
void turnToHeading(float heading, int timeout) {
	float currentHeading = gyro.get_heading();
	float error = heading - currentHeading;
	
	if (error > 180) {
		error = error - 360;
	} else if (error < -180) {
		error = 360 + error;
	}
	//master.print(0, 0, "%f", error);
	turnAngle(error, timeout);
}

/// @brief Turns the robot a specified distance using PID control
/// @param angle Amount to turn in degrees
/// @param timeout Timeout before the robot gives up in seconds, default to 10
void turnAngle(float angle, int timeout) {
    auto gains = get<1>(chassis->getGains());

    float target = angle + gyro.get_rotation();
    float error = angle;
	float previousError = 0;
	float integral = 0;
	float errorCounter = 0;
	float precision = 1;

	float windUp = 5;
	
	auto exitTime = std::chrono::high_resolution_clock::now() + std::chrono::seconds(timeout);
	while (errorCounter < 100 && std::chrono::high_resolution_clock::now() < exitTime) {
		pros::delay(10);
		if (abs(error) < windUp){
			integral += error;	
		}
		float velocity = setMinAbs((gains.kP * error + (error - previousError) * gains.kD + gains.kI * integral), 2);
		right.moveVelocity(-velocity);
		left.moveVelocity(velocity);
		//driverController.print(0,0,"%f", velocity);
		previousError = error;
		error = target - gyro.get_rotation();
		if (abs(error) < precision) {
			errorCounter++;
		}
		else {
			errorCounter = 0;
		}
	}
	right.moveVelocity(0);
	left.moveVelocity(0);
}

void drivetrainInit(){
	drivetrain->setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
	gyro.reset();
	while(gyro.is_calibrating());
}
