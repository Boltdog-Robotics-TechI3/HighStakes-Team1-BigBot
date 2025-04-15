class PIDController {
    private:
        // PID Controller Settings
        // These values' respective functions will be disabled if they are 0
        double kP;
        double kI;
        double kD;
        double smallErrorRange = 0;
        double largeErrorRange = 0;
        double minOutput = 0;
        double maxOutput = 0;
        double IZone = 0;
        double maxSlewRate = 0;

        // Other variables
        double setpoint;
        double measurement;
        int currentTime;
        int previousTime;
        double error;
        double previousError;
        double accumulatedError;
        double previousOutput;

    public:
        /**
         * Constructor for the PID controller with inputs for the PID gains.
         * 
         * @param kP the proportional gain
         * @param kI the integral gain
         * @param kD the derivative gain
         */
        PIDController(double kP, double kI, double kD);

        /**
         * Default constructor for the PID controller.
         * Sets the PID gains to 0.
         */
        PIDController();

        /**
         * Sets the large error timeout for the PID controller.
         * Typically used for stopping the loop when the error is small enough for a certain period of time. This period of time is longer than the small error timeout. 
         * Thus, the large error range should be larger than the small error range.
         * This functionality is not built into the PIDController class, and instead has to be implemented in the loop in which the calculate method is used.
         * 
         * @param range the large error range in the same units as the measurement
         */
        void setLargeErrorRange(double range);

        /**
         * Sets the small error range for the PID controller.
         * Typically used for stopping the loop when the error is small enough for a certain period of time. This period of time is shorter than the large error timeout.
         * Thus, the small error range should be smaller than the large error range
         * This functionality is not built into the PIDController class, and instead has to be implemented in the loop in which the calculate method is used.
         * 
         * @param range the small error range in the same units as the measurement
         */
        void setSmallErrorRange(double range);

        /**
         * Sets the output limits for the PID controller.
         * If these values are not 0, the calculate() method will automatically clamp the output to be within these limits.
         * Set to 0 to disable this functionality.
         * 
         * @param min the minimum output value
         * @param max the maximum output value
         */
        void setOutputLimits(double min, double max);

        /**
         * Sets the PID gains for the PID controller.
         * 
         * @param kP the proportional gain
         * @param kI the integral gain
         * @param kD the derivative gain
         */
        void setGains(double kP, double kI, double kD);

        /**
         * Sets the proportional gain for the PID controller.
         * 
         * @param kP the proportional gain
         */
        void setP(double kP);

        /**
         * Sets the integral gain for the PID controller.
         * 
         * @param kP the integral gain
         */
        void setI(double kI);

        /**
         * Sets the derivative gain for the PID controller.
         * 
         * @param kP the derivative gain
         */
        void setD(double kD);

        /**
         * Gets the proportional gain for the PID controller.
         * 
         * @return the proportional gain
         */
        double getP();

        /**
         * Gets the integral gain for the PID controller.
         * 
         * @return the integral gain
         */
        double getI();

        /**
         * Gets the derivative gain for the PID controller.
         * 
         * @return the derivative gain
         */
        double getD();

        /**
         * Sets the IZone for the PID controller.
         * The IZone is the maximum error within which the integral term will be able to accumulate.
         * If the error is outside of this range, the integral term will do nothing.
         * Set this to 0 to disable this functionality.
         * 
         * @param IZone the IZone value
         */
        void setIZone(double IZone);

        /**
         * Gets the IZone for the PID controller.
         * The IZone is the maximum error within which the integral term will be able to accumulate.
         * If the error is outside of this range, the integral term will do nothing.
         * Set this to 0 to disable this functionality.
         * 
         * @return the IZone value
         */
        double getIZone();

        /**
         * Gets the current error of the PID controller.
         * 
         * @return the current error
         */
        double getError();

        /**
         * Gets the current setpoint of the PID controller.
         * 
         * @return the current setpoint
         */
        double getSetpoint();

        /**
         * Resets the PID controller.
         * This sets the accumulated error, error, and previous error to 0.
         * It is highly recommended to call this method before starting a new loop to ensure accurate results.
         */
        double reset();

        /**
         * Calculates the output of the PID controller based on the current measurement and setpoint.
         * This method should be called in a loop to continuously update the output.
         * 
         * @param measurement the current measurement of the system
         * @param setpoint the desired setpoint of the system
         * @return the output of the PID controller
         */
        double calculate(double measurement, double setpoint);

        /**
         * Determines if the error is within the small error range.
         * This is typically used for stopping the loop when the error is small enough for a certain period of time.
         * This period of time is shorter than the large error timeout.
         * Thus, the small error range should be smaller than the large error range.
         * This functionality is not built into the PIDController class, and instead has to be implemented in the loop in which the calculate method is used.
         * 
         * @return whether or not the error is within the small error range
         */
        bool isInSmallErrorRange();

        /**
         * Determines if the error is within the large error range.
         * This is typically used for stopping the loop when the error is small enough for a certain period of time.
         * This period of time is longer than the small error timeout. 
         * Thus, the large error range should be larger than the small error range.
         * This functionality is not built into the PIDController class, and instead has to be implemented in the loop in which the calculate method is used.
         * 
         * @return whether or not the error is within the large error range
         */
        bool isInLargeErrorRange();
};