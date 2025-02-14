#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

const int MAX_VELOCITY = 600;

bool intakeFront = true;

int teleOPCurrentLimit = 2500;

pros::Controller driverController(pros::E_CONTROLLER_MASTER);

pros::MotorGroup leftDriveMotors({-8, 9, -10}, pros::MotorGearset::blue);
pros::MotorGroup rightDriveMotors({1, 2, -3}, pros::MotorGearset::blue); 

lemlib::Drivetrain drivetrain(&leftDriveMotors, // left motor group
                              &rightDriveMotors, // right motor group
                              11.625, // 12 inch track width
                              lemlib::Omniwheel::NEW_275, // using new 2.75" omnis
                              600 * (36.0 / 48.0), // drivetrain rpm is 450
                              2 // horizontal drift is 2 (for now)
);

pros::Imu imu(12);

pros::Rotation rotationSensor(-7);

// horizontal tracking wheel
lemlib::TrackingWheel horizontalTrackingWheel(&rotationSensor, lemlib::Omniwheel::NEW_275, -4);

lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &horizontalTrackingWheel, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// anuglar PID controller
lemlib::ControllerSettings angularController( 10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                        	  100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// lateral PID controller
lemlib::ControllerSettings lateralController( 10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// lemlib::ExpoDriveCurve driveCurve(3, 10, .9);

// create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateralController, // lateral PID settings
                        angularController, // angular PID settings
                        sensors // odometry sensors
						// &driveCurve
);

pros::Motor lift(-11);
pros::Motor intake(-20);

pros::MotorGroup ladyBrownGroup({5, -6}, pros::MotorGearset::green);

pros::adi::Pneumatics mogoClamp = pros::adi::Pneumatics('H', false);
pros::adi::Pneumatics rushMech = pros::adi::Pneumatics('G', false);

//Init functions

void drivetrainInit(){
	pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
}

void ladyBrownInit(){
	// ladyBrownGroup.set_current_limit_all(2500.0 / 2.0);

	ladyBrownGroup.set_brake_mode_all(pros::MotorBrake::hold);

	ladyBrownGroup.set_encoder_units_all(pros::MotorUnits::degrees);
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
	switch (autoSelection) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
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
 * 
 *
 */

 ASSET(testPath1_txt);
 ASSET(example_txt);


void opcontrol() {
	// int count = 0;
	// setDriveCurrentLimt(teleOPCurrentLimit);

	// chassis.setPose(0, 0, 0);

	// chassis.follow(example_txt, 5, 30000);

	while (true) {


		if(driverController.get_digital_new_press(DIGITAL_A)){
			rushMech.toggle();
		}

		// if(count > 10){
		// 	driverController.clear();
		// 	driverController.print(0, 0, "%d", chassis.getPose().x);
		// 	driverController.print(1, 0, "%d", chassis.getPose().y);
		// 	count = 0;
		// } else {
		// 	count++;
		// }


        int leftY = driverController.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = driverController.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        //move the robot

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

		if (driverController.get_digital(DIGITAL_UP)){
			ladyBrownGroup.move(40);
		} else if (driverController.get_digital(DIGITAL_DOWN)){
			ladyBrownGroup.move(-40);
		} else {
			ladyBrownGroup.brake();
		}

		// if (driverController.get_digital_new_press(DIGITAL_L2)) { 
		// 	intakeFront = !intakeFront;
		// }

        chassis.arcade(leftY, rightX);

		pros::delay(20);                               // Run for 20 ms then update
	}
}