#include "main.h"

extern okapi::Motor frontRight;
extern okapi::Motor backRight;
extern okapi::Motor topRight;
 
extern okapi::Motor frontLeft;
extern okapi::Motor backLeft;
extern okapi::Motor topLeft;

extern pros::IMU gyro(12);

extern std::shared_ptr<okapi::ChassisControllerPID> chassis;

extern std::shared_ptr<ChassisModel> drivetrain;

void setDriveCurrentLimt(int limit);

void turnAngle(float angle, int timeout = 10);

void drivetrainInit();