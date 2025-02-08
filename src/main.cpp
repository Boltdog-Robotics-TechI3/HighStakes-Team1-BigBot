#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

const int MAX_VELOCITY = 600;

bool intakeFront = true;

int teleOPCurrentLimit = 2200;

pros::Controller driverController(pros::E_CONTROLLER_MASTER);

pros::MotorGroup leftDriveMotors({-18, 19, -20}, pros::MotorGearset::blue);
pros::MotorGroup rightDriveMotors({11, -12, 13}, pros::MotorGearset::blue); 

lemlib::Drivetrain drivetrain(&leftDriveMotors, // left motor group
                              &rightDriveMotors, // right motor group
                              12, // 12 inch track width
                              lemlib::Omniwheel::NEW_4, // using new 4" omnis
                              360, // drivetrain rpm is 360
                              2 // horizontal drift is 2 (for now)
);

// pros::Imu imu(10);

pros::Rotation rotationSensor(9);

// horizontal tracking wheel
lemlib::TrackingWheel horizontalTrackingWheel(&rotationSensor, lemlib::Omniwheel::NEW_275, -4);

lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &horizontalTrackingWheel, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            nullptr // inertial sensor
);

// lateral PID controller
lemlib::ControllerSettings lateralController(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angularController(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateralController, // lateral PID settings
                        angularController, // angular PID settings
                        sensors // odometry sensors
);

pros::Motor lift(-2);

pros::Motor intake(-10);

okapi::Motor ladyBrownLeft(-7);
okapi::Motor ladyBrownRight(4);

okapi::MotorGroup ladyBrownGroup({ladyBrownLeft, ladyBrownRight});

pros::adi::Pneumatics mogoClamp = pros::adi::Pneumatics('A', false);

//Init functions

void drivetrainInit(){
	pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
}

void ladyBrownInit(){
	ladyBrownLeft.setVoltageLimit(7200);
	ladyBrownRight.setVoltageLimit(7200);

	ladyBrownLeft.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	ladyBrownRight.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);

	ladyBrownLeft.setReversed(false);
	ladyBrownRight.setReversed(true);

	ladyBrownGroup.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
	ladyBrownGroup.setVoltageLimit(7200);

	ladyBrownGroup.setEncoderUnits(okapi::AbstractMotor::encoderUnits::degrees);
}

void setDriveCurrentLimt(int limit){
	leftDriveMotors.set_current_limit_all(limit);
	rightDriveMotors.set_current_limit_all(limit);
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	initializeScreen();

	drivetrainInit();
	ladyBrownInit();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	setDriveCurrentLimt(2200);
	switch (autoSelection) {
		case 0:
			//Match Plus Side Keep Goal Auto
			break;
		case 1:
			//Match Plus Side Drop Goal Auto
			// keepGoalAuto();
			break;
		case 2:
			//Match Climb Goal Keep Goal Autowatch climb keep
			// dropGoalAuto();
			break;
		case 3:
			//Match Climb Goal Drop Goal Selected
			break;
		case 4:
			//Skills
			// skills_autonomous();
			break;
		case 5:
			//Do nothing
			break;
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	//Controls:
	//B button toggles clamp
	//A button controls intake and lift
	//Left joystick controls forward/backwards movement
	//Right joystick controls turning

	setDriveCurrentLimt(teleOPCurrentLimit);

	driverController.rumble(".");

	while (true) {
		if (driverController.get_digital_new_press(DIGITAL_Y)) {

		}

        int leftY = driverController.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = driverController.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        // move the robot
        chassis.arcade(leftY, rightX);

		//Move the ring conveyor up or down depending on what right shoulder button is pressed.
		if (driverController.get_digital(DIGITAL_R1)) { 
			lift.move(127);
			intake.move(127);
			
		} else if (driverController.get_digital(DIGITAL_R2)) {
			lift.move(-127);
			intake.move(-127);
		} else {
			lift.move(0);
			intake.move(0);
		}
		
		//Toggle the goal clamp on the back (pneumatic clamp)
		if (driverController.get_digital_new_press(DIGITAL_B)) { 
			mogoClamp.toggle();
		}

		if (driverController.get_digital_new_press(DIGITAL_L2)) { 
			intakeFront = !intakeFront;
		}


		// if (driverController.get_digital(DIGITAL_UP)) { 
		// 	ladyBrownGroup.moveVoltage(12000);
		// } else if (driverController.get_digital(DIGITAL_DOWN)) {
		// 	ladyBrownGroup.moveVoltage(-12000);
		// } else {
		// 	ladyBrownGroup.moveVoltage(0);
		// }

		if (driverController.get_digital_new_press(DIGITAL_LEFT)) {
			ladyBrownGroup.moveAbsolute(200, 30);
		}
		// if(count == 10){
		// 	avgCurr = std::fmax(avgCurr, ((frontLeft.getCurrentDraw() + frontRight.getCurrentDraw() + 
		// 			backLeft.getCurrentDraw() + backRight.getCurrentDraw() + 
		// 			topLeft.getCurrentDraw() + topRight.getCurrentDraw()) / 6));
		// 	driverController.set_text(1, 1, std::to_string(avgCurr));
			
		// 	count = 0;
		// }
		// maxCurr = std::fmax(maxCurr, frontLeft.getCurrentDraw());

		// count++;

		// drivetrain->arcade(dir, turn); // Takes in the inputs from the analog sticks and moves the robot accordingly using arcade controls.
		pros::delay(20);                               // Run for 20 ms then update
	}
}