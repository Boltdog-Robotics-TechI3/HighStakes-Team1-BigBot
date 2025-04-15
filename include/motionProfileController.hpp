#include "PIDcontroller.hpp"
class MotionProfileController{
    private:
        double setpoint;
        double maxAcceleration;
        double maxVelocity;
        double startTime;
        double timeAcceleration;
        double distanceTraveledAcceleration;
        double distanceTraveledMaxVelocity; 
        double timeAtMaxVelocity;
        double totalTime;
        PIDController PIDcontroller;
         
    
    public:
        MotionProfileController(double maxAcceleration, double maxVelocity);
        void reset(double setpoint);
        double calculate(int currentTime, double velocity);
};