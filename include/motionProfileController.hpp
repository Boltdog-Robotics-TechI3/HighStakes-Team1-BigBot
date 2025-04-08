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
    
    public:
        MotionProfileController(double maxAcceleration, double maxVelocity);
        void reset(double setpoint);
        double calculate(int currentTime);
};