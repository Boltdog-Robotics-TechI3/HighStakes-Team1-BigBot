#include "main.h"
#include "motionProfileController.hpp"

MotionProfileController::MotionProfileController(double maxAcceleration, double maxVelocity){
this->maxAcceleration = maxAcceleration;
this->maxVelocity = maxVelocity;
} 
void MotionProfileController::reset(double setpoint){
this->setpoint = setpoint;
this->startTime = pros::millis();
this->timeAcceleration = maxVelocity / maxAcceleration;
this->distanceTraveledAcceleration = 1/2 * timeAcceleration * maxVelocity;
this->distanceTraveledMaxVelocity = setpoint - distanceTraveledAcceleration * 2;
this->timeAtMaxVelocity = distanceTraveledMaxVelocity / maxVelocity;
this->totalTime = 2 * timeAcceleration + timeAtMaxVelocity;
}
double MotionProfileController::calculate(int currentTime){
    double velocity;
    double localTime = currentTime - startTime;
    if(localTime < timeAcceleration){
        velocity = (localTime/timeAcceleration) * maxVelocity;
        return velocity;
    }
    if(localTime < timeAcceleration + timeAtMaxVelocity){
        velocity = maxVelocity;
        return velocity;
    }
    if(localTime < totalTime){
        velocity = maxVelocity * (1 - (localTime - (timeAcceleration + timeAtMaxVelocity)/timeAcceleration));
        return velocity;
    }
}
