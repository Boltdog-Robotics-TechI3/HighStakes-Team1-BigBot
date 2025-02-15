#include "main.h"

extern pros::MotorGroup leftDriveMotors;
extern pros::MotorGroup rightDriveMotors;

extern lemlib::Drivetrain drivetrain;

extern pros::Imu imu;

extern pros::Rotation rotationSensor;
extern lemlib::TrackingWheel horizontalTrackingWheel;
extern lemlib::OdomSensors sensors;
extern lemlib::ControllerSettings angularController;
extern lemlib::ControllerSettings lateralController;
extern lemlib::Chassis chassis;

void setDriveCurrentLimit();
void drivetrainInit();