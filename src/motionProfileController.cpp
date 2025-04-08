#include "main.h"
#include "motionProfileController.hpp"

MotionProfileController::MotionProfileController(double maxAcceleration, double maxVelocity){
this->maxAcceleration = maxAcceleration;
this->maxVelocity = maxVelocity;
} 
void MotionProfileController::reset(double setpoint){
this->setpoint = setpoint;
this->startTime = pros::millis();
}
double MotionProfileController::calculate(int currentTime){
    
}
