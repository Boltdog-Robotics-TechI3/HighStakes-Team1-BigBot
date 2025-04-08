class MotionProfileController{
    private:
        double setpoint;
        double maxAcceleration;
        double maxVelocity;
        double startTime;
        double timeAcceleration = maxVelocity / maxAcceleration;
        double distanceTraveledAcceleration = 1/2 * timeAcceleration * maxVelocity;
        double distanceTraveledMaxVelocity = setpoint - distanceTraveledAcceleration * 2; 
        double timeAtMaxVelocity = distanceTraveledMaxVelocity / maxVelocity; 
    
    public:
        MotionProfileController(double maxAcceleration, double maxVelocity);
        void reset(double setpoint);
        double calculate(int currentTime);
};