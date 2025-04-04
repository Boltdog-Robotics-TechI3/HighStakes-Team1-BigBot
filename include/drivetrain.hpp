#pragma once
#include "main.h"

extern pros::Motor frontRight;
extern pros::Motor backRight;
extern pros::Motor topRight;
 
extern pros::Motor frontLeft;
extern pros::Motor backLeft;
extern pros::Motor topLeft;

extern pros::IMU gyro;



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

void driveDistance(double distance, double voltage);
void driveDistancePID(double distance, double maxVoltage, double timeout, bool async);