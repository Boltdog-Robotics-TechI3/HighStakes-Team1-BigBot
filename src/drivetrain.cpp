#include "main.h"

pros::MotorGroup leftDriveMotors({-8, 9, -10}, pros::MotorGearset::blue);
pros::MotorGroup rightDriveMotors({1, 2, -3}, pros::MotorGearset::blue); 

lemlib::Drivetrain drivetrain(&leftDriveMotors, // left motor group
                              &rightDriveMotors, // right motor group
                              11.625, // 12 inch track width
                              lemlib::Omniwheel::NEW_275, // using new 2.75" omnis
                              600 * (36.0 / 48.0), // drivetrain rpm is 450
                              2 // horizontal drift is 2 (for now)
);

pros::Imu imu(12);

pros::Rotation rotationSensor(-7);

// horizontal tracking wheel
lemlib::TrackingWheel horizontalTrackingWheel(&rotationSensor, lemlib::Omniwheel::NEW_275, -4);

lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &horizontalTrackingWheel, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// anuglar PID controller
lemlib::ControllerSettings angularController( 4, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              2, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in degrees
                                        	  0, // small error range timeout, in milliseconds
                                              0, // large error range, in degrees
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// lateral PID controller
lemlib::ControllerSettings lateralController( 100, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              0, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// lemlib::ExpoDriveCurve driveCurve(3, 10, .9);

// create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateralController, // lateral PID settings
                        angularController, // angular PID settings
                        sensors // odometry sensors
						// &driveCurve
);

void setDriveCurrentLimt(int limit){
	leftDriveMotors.set_current_limit_all(limit);
	rightDriveMotors.set_current_limit_all(limit);
}

void drivetrainInit(){
    chassis.calibrate(); // calibrate sensors
}

