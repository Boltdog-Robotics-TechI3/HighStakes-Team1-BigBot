#include "main.h"
#include "globals.h"
#include "drivetrain.hpp"




//Init functions

void ladyBrownInit(){
	ladyBrownLeft.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	ladyBrownRight.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	ladyBrownLeft.set_reversed(true);
	ladyBrownRight.set_reversed(false);

	ladyBrownGroup.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	ladyBrownGroup.set_encoder_units(pros::motor_encoder_units_e_t::E_MOTOR_ENCODER_DEGREES);
}



/**
 * The autonomous path used for a skill run.
 * 
 * Should do these in order:
 * - Score 1 ring on a alliance stake
 * - 3 rings on one mobile goal and place it in a corner
 * - 1 ring on the wall stake
 * - 4 ring on another mobile goal and place it in the corner
 * - Buddy climb
 */

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {}

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
	lift.set_gearing(pros::MotorGear::green);
	lift.set_encoder_units(pros::MotorEncoderUnits::degrees);
	lift.set_brake_mode(pros::MotorBrake::brake);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {

}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
}

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
			newSkillsAuto();
			break;
		case 1:
			//Match Plus Side Drop Goal Auto
			goalRushAutoBlue();
			break;
		case 2:
			//Match Climb Goal Keep Goal Autowatch climb keep
			goalRushAutoRed();
			break;
		case 3:
			//Match Climb Goal Drop Goal Selected
			break;
		case 4:
			//Skills
			skillsAuto();
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
	int count = 0;
	// chassis -> setMaxVelocity(10000);

	master.rumble(".");
	while (true) {
		double leftY = master.get_analog(ANALOG_LEFT_Y) / 127.0;
		double rightX = master.get_analog(ANALOG_RIGHT_X) / 127.0;

		double dir = pow(leftY, 3);    // Gets amount forward/backward from left joystick
		double turn = pow(rightX, 3);  // Gets the turn left/right from right joystick

		// if(intakeFront){
		// 	dir = pow(leftY, 3);    // Gets amount forward/backward from left joystick
		// 	turn = pow(rightX, 3);  // Gets the turn left/right from right joystick
		// } else {
		// 	dir = pow(leftY, 3);    // Gets amount forward/backward from left joystick
		// 	turn = pow(rightX, 3);  // Gets the turn left/right from right joystick
		// }


		
		//Move the ring conveyor up or down depending on what right shoulder button is pressed.
		if(!isSorting){
			if (master.get_digital(DIGITAL_R1)) { 
				colorSorting();
				intake.move(127);
			} else if (master.get_digital(DIGITAL_R2)) {
				lift.move(-127);
				intake.move(-127);
			} else if (master.get_digital(DIGITAL_L1)){
				intake.move(127);
			} else {
				lift.move(0);
				intake.move(0);
			}
		} else {
			if(lift.get_actual_velocity() == 0){
				if(count > 5){
					isSorting = false;
					count = 0;
				}
				count++;
			}
		}
		
		//Toggle the goal clamp on the back (pneumatic clamp)
		if (master.get_digital_new_press(DIGITAL_B)) { 
			mogoClamp.toggle();
		}
		if (master.get_digital_new_press(DIGITAL_A)) { 
			rushMech.toggle();
		}

		if (master.get_digital_new_press(DIGITAL_X) && master.get_digital_new_press(DIGITAL_Y)) { 
			newSkillsAuto();
		}
		// if ( master.get_digital_new_press(DIGITAL_LEFT) && master.get_digital_new_press(DIGITAL_Y)){
		// 	bruhAuto();
		// }

		// if (master.get_digital_new_press(DIGITAL_DOWN)) {
		// 	ladyBrownGroup.moveAbsolute(0, 30); //home ladybrown arm
		// } else if (master.get_digital_new_press(DIGITAL_RIGHT)){
		// 	ladyBrownGroup.moveAbsolute(55, 60); //for grabbing rings
		// } else if (master.get_digital_new_press(DIGITAL_UP)){
		// 	ladyBrownGroup.moveAbsolute(255, 90); //score
		// }

		// count++;
		// if (master.get_digital(DIGITAL_LEFT)) {
		// 	goalRushAutoBlue();
		// 	// turnAngle(90, 10);
		// }
		// // if (master.get_digital(DIGITAL_UP)){
		// // 	safePath();
		// // }
		// if (master.get_digital(DIGITAL_RIGHT)) {
		// 	skillsAuto();	
		// }
		// if (master.get_digital_new_press(DIGITAL_DOWN)) {
		// 	bruhAuto();	
		// }

		if(master.get_digital_new_press(DIGITAL_UP)){
			lift.move(-127);
    pros::delay(150);
			pros::Task ladyUp(ladyBrownScore);
		} else if(master.get_digital_new_press(DIGITAL_L2) || master.get_digital_new_press(DIGITAL_DOWN)){
			pros::Task ladyDown(ladyBrownDown);
		} else if(master.get_digital(DIGITAL_RIGHT)){
			ladyBrownPrime();
		} else if(ladyBrownGroup.get_voltage() == 0){
		//testing for driveDistance
		}else if(master.get_digital(DIGITAL_LEFT)){
		driveDistance(18, 7);
		}else if(master.get_digital(DIGITAL_L1)){
		driveDistancePID(18, 7, 10000, false);
		}
		

		// if(!master.get_digital(DIGITAL_L2)){
		// } else {
		// 	drivetrain->arcade(dir * .6, turn * .6); // Takes in the inputs from the analog sticks and moves the robot accordingly using arcade controls.
		// }

		pros::delay(20);                               // Run for 20 ms then update
	}
}