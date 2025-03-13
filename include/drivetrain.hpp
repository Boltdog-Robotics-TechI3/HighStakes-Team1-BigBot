#pragma once
#include "main.h"

extern okapi::Motor frontRight;
extern okapi::Motor backRight;
extern okapi::Motor topRight;
 
extern okapi::Motor frontLeft;
extern okapi::Motor backLeft;
extern okapi::Motor topLeft;

extern pros::IMU gyro;

extern std::shared_ptr<okapi::ChassisControllerPID> chassis;

extern std::shared_ptr<ChassisModel> drivetrain;

struct Drivetrain {
    okapi::MotorGroup *leftMotors;
    okapi::MotorGroup *rightMotors;
    double wheelDiameter;
    double wheelTrack;
    double gearRatio;
    okapi::AbstractMotor::gearset gearSet;
};

struct PID {
    double kP;
    double kI;
    double kD;
    double smallErrorRange;
    double smallErrorTimeout;
    double largeErrorRange;
    double largeErrorTimeout;
    double minVelocity;
};

enum class TurnBehavior {
    DEFAULT,
    LEFT,
    RIGHT
};

void setDriveCurrentLimit(int limit);

void turnToHeading(double heading, double maxVelocity=1.0, int timeout=5000, enum TurnBehavior behavior=TurnBehavior::DEFAULT);

void turnAngle(double angle, double maxVelocity=1.0, int timeout=5000);

void drivetrainInit();