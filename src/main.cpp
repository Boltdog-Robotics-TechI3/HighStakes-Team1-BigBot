#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep


void ladyBrownInit(){
	// ladyBrownGroup.set_current_limit_all(2500.0 / 2.0);

	ladyBrownGroup.set_brake_mode_all(pros::MotorBrake::hold);

	ladyBrownGroup.set_encoder_units_all(pros::MotorUnits::degrees);
}



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize(); // initialize brain screen

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
	switch (1) {
		case 0:
			break;
		case 1:
			goalRush();
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

		if(driverController.get_digital_new_press(DIGITAL_LEFT)){
			autonomous();
		}

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