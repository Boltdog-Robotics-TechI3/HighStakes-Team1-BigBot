#include "PIDcontroller.hpp"
#include "api.h"
PIDController::PIDController(double kP, double kI, double kD) {
    this->kP = kP;
    this->kI = kI;
    this->kD = kD;

    currentTime = pros::millis();
    previousTime = pros::millis();
}

/**
 * Default constructor for the PID controller.
 * Sets the PID gains to 0.
 */
PIDController::PIDController() {
    PIDController(0, 0, 0);
}

/**
 * Sets the large error timeout for the PID controller.
 * Typically used for stopping the loop when the error is small enough for a certain period of time. This period of time is longer than the small error timeout. 
 * Thus, the large error range should be larger than the small error range.
 * This functionality is not built into the PIDController class, and instead has to be implemented in the loop in which the calculate method is used.
 * 
 * @param range the large error range in the same units as the measurement
 */
void PIDController::setLargeErrorRange(double range) {
    largeErrorRange = range;
}

/**
 * Sets the small error range for the PID controller.
 * Typically used for stopping the loop when the error is small enough for a certain period of time. This period of time is shorter than the large error timeout.
 * Thus, the small error range should be smaller than the large error range
 * This functionality is not built into the PIDController class, and instead has to be implemented in the loop in which the calculate method is used.
 * 
 * @param range the small error range in the same units as the measurement
 */
void PIDController::setSmallErrorRange(double range) {
    smallErrorRange = range;
}

/**
 * Sets the output limits for the PID controller.
 * If these values are not 0, the calculate() method will automatically clamp the output to be within these limits.
 * Set to 0 to disable this functionality.
 * 
 * @param min the minimum output value
 * @param max the maximum output value
 */
void PIDController::setOutputLimits(double min, double max) {
    minOutput = min;
    maxOutput = max;
}

/**
 * Sets the PID gains for the PID controller.
 * 
 * @param kP the proportional gain
 * @param kI the integral gain
 * @param kD the derivative gain
 */
void PIDController::setGains(double kP, double kI, double kD) {
    this->kP = kP;
    this->kI = kI;
    this->kD = kD;
}

/**
 * Sets the proportional gain for the PID controller.
 * 
 * @param kP the proportional gain
 */
void PIDController::setP(double kP) {
    this->kP = kP;
} 

/**
 * Sets the integral gain for the PID controller.
 * 
 * @param kP the integral gain
 */
void PIDController::setI(double kI) {
    this->kI = kI;
}

/**
 * Sets the derivative gain for the PID controller.
 * 
 * @param kP the derivative gain
 */
void PIDController::setD(double kD) {
    this->kD = kD;
}

/**
 * Gets the proportional gain for the PID controller.
 * 
 * @return the proportional gain
 */
double PIDController::getP() {
    return kP;
}

/**
 * Gets the integral gain for the PID controller.
 * 
 * @return the integral gain
 */   
double PIDController::getI() {
    return kI;
}

/**
 * Gets the derivative gain for the PID controller.
 * 
 * @return the derivative gain
 */
double PIDController::getD() {
    return kD;
}

/**
 * Sets the IZone for the PID controller.
 * The IZone is the maximum error within which the integral term will be able to accumulate.
 * If the error is outside of this range, the integral term will do nothing.
 * Set this to 0 to disable this functionality.
 * 
 * @param IZone the IZone value
 */
void PIDController::setIZone(double IZone) {
    this->IZone = IZone;
}

/**
 * Gets the IZone for the PID controller.
 * The IZone is the maximum error within which the integral term will be able to accumulate.
 * If the error is outside of this range, the integral term will do nothing.
 * Set this to 0 to disable this functionality.
 * 
 * @return the IZone value
 */
double PIDController::getIZone() {
    return IZone;
}

/**
 * Gets the current error of the PID controller.
 * 
 * @return the current error
 */
double PIDController::getError() {
    return error;
}

/**
 * Gets the current setpoint of the PID controller.
 * 
 * @return the current setpoint
 */
double PIDController::getSetpoint() {
    return setpoint;
}

/**
 * Resets the PID controller.
 * This sets the accumulated error, error, and previous error to 0.
 * It is highly recommended to call this method before starting a new loop to ensure accurate results.
 */
double PIDController::reset() {
    accumulatedError = 0.0;
    error = 0.0;
    previousError = 0.0;
}

/**
 * Calculates the output of the PID controller based on the current measurement and setpoint.
 * This method should be called in a loop to continuously update the output.
 * 
 * @param measurement the current measurement of the system
 * @param setpoint the desired setpoint of the system
 * @return the output of the PID controller
 */
double PIDController::calculate(double measurement, double setpoint) {
    // Update the error
    error = setpoint - measurement;

    // Calculate the elapsed time of this current feedback loop
    int elapsedTime = currentTime - previousTime;

    // Calculate the output of the PID controller
    double output = kP * error +                                  // P term
                    (kI * accumulatedError * elapsedTime) +       // I term
                    ((error - previousError) * kD / elapsedTime); // D term

    // Clamp the output
    if (minOutput != 0.0) {
        if (output > 0) {
            output = std::max(output, minOutput);
        } else {
            output = std::min(output, -minOutput);
        }
    }
    if (maxOutput != 0.0 ) {
        if (output > 0) {
            output = std::min(output, maxOutput);
        } else {
            output = std::max(output, -maxOutput);
        }
    }

    //Slew Rate (Max rate of change)
    auto lastDifference = output - previousOutput;
    auto maxDifference = maxSlewRate * (elapsedTime / 1000);
    if (maxSlewRate != 0 && std::abs(lastDifference) > maxDifference) {
        // If output is decreasing
        if (lastDifference < 0) {
            output = previousOutput - maxDifference;
        }
        // If output is increasing
        else {
            output = previousOutput + maxDifference;
        }
    }

    // Update values
    previousError = error;
    previousTime = currentTime;
    currentTime = pros::millis();
    previousOutput = output;

    // Only accumulate the error if it is within the IZone (or if IZone is disabled)
    accumulatedError += (IZone != 0 && std::abs(error) > IZone) ? 0 : error;


    return output;
}

/**
 * Checks if the error is within the small error range.
 * 
 * @return true if the error is within the small error range, false otherwise
 */
bool PIDController::isInSmallErrorRange() {
    return std::abs(error) < smallErrorRange;
}

/**
 * Checks if the error is within the large error range.
 * 
 * @return true if the error is within the large error range, false otherwise
 */
bool PIDController::isInLargeErrorRange() {
    return std::abs(error) < largeErrorRange;
}