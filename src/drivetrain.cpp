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

auto chassis = std::dynamic_pointer_cast<ChassisControllerPID>(ChassisControllerBuilder()
	.withMotors(left, right)
	.withDimensions({AbstractMotor::gearset::blue, (72.0/60.0)}, {{4_in, 15.75_in}, imev5BlueTPR})
	.withGains(
		{0.0015, 0.0, 0.0000005}, 
		{3.15, 0, 1.5}, 
		{0, 0, 0})
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

/// @brief Custom Turnangle Function
/// @param angle angle in degrees
/// @param timeout timeout before the robot gives up in seconds
void turnAngle(float angle, int timeout = 10) {
    auto gains = get<1>(chassis->getGains());

    float target = angle + gyro.get_rotation();
    float error = angle;
	float previousError = 0;
	float integral = 0;
	float errorCounter = 0;
	float precision = 1;
	
	auto exitTime = std::chrono::high_resolution_clock::now() + std::chrono::seconds(timeout);
	while (errorCounter < 50 && std::chrono::high_resolution_clock::now() < exitTime) {
		integral += error;
		float velocity = setMinAbs((gains.kP * error + (error - previousError) * gains.kD + gains.kI * integral), 1);
		right.moveVelocity(-velocity);
		left.moveVelocity(velocity);
		pros::delay(10);
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
}
