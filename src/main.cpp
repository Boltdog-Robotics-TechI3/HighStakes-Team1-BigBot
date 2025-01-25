#include "main.h"

const int MAX_VELOCITY = 600;

bool intakeFront = true;

int teleOPCurrentLimit = 2200;

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor lift(5);

okapi::Motor frontRight(1);
okapi::Motor backRight(2);
okapi::Motor topRight(-3);

okapi::Motor frontLeft(-8);
okapi::Motor backLeft(-9);
okapi::Motor topLeft(10);

okapi::MotorGroup right({frontRight, topRight, backRight});
okapi::MotorGroup left({frontLeft, topLeft, backLeft});

okapi::Motor ladyBrownLeft(-7);
okapi::Motor ladyBrownRight(4);

okapi::MotorGroup ladyBrownGroup({ladyBrownLeft, ladyBrownRight});

auto chassis = std::dynamic_pointer_cast<ChassisControllerPID>(ChassisControllerBuilder()
	.withMotors(left, right)
	.withDimensions({AbstractMotor::gearset::blue, (72.0/60.0)}, {{4_in, 15.75_in}, imev5BlueTPR})
	.withGains(
		{0.00285, 0.0, 0.0}, 
		{0.00095, 0, 0}, 
		{0, 0, 0})
	.build());

std::shared_ptr<ChassisModel> drivetrain = chassis->getModel();

pros::adi::Pneumatics mogoClamp = pros::adi::Pneumatics('A', false);

//Init functions

void drivetrainInit(){
	drivetrain->setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
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
	frontLeft.setCurrentLimit(limit);
	frontRight.setCurrentLimit(limit);
	topLeft.setCurrentLimit(limit);
	topRight.setCurrentLimit(limit);
	backLeft.setCurrentLimit(limit);
	backRight.setCurrentLimit(limit);
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
void cornerMoveFunct(void* param){
	chassis -> moveDistanceAsync(-20_in);
}


void skills_autonomous() {


	//Score 1 ring on alliance stake
	chassis->setGains(
		{0.003, 0.0, 0.0}, 
		{0.00095, 0, 0}, 
		{0, 0, 0}
	);
	chassis -> setMaxVelocity(MAX_VELOCITY * 0.3);
	lift.move(127); //start intake 
	pros::delay(100);
	chassis -> moveDistance(12_in); //grab 1st ring
	pros::delay(300);
	lift.move(0);
	chassis -> moveDistance(-11_in); //go back to alliance stake 
	lift.move(127); //start intake 
	pros::delay(1250); //score ring

	//Put 3 rings on mobile goal and place in corner
	chassis->setGains(
		{0.003, 0.0, 0.0}, 
		{0.00095, 0, 0}, 
		{0, 0, 0}
	);	
	chassis -> setMaxVelocity(MAX_VELOCITY * 0.4); 
	chassis -> moveDistance(16_in);
	chassis -> setMaxVelocity(MAX_VELOCITY); 
	chassis -> turnAngle(45_deg);
	chassis -> setMaxVelocity(MAX_VELOCITY * 0.4); 
	chassis -> moveDistance(34_in); //go to 2nd next ring
	pros::delay(100);
	lift.move(0); //stop intake because we dont have a goal yet
	chassis -> setMaxVelocity(MAX_VELOCITY); 
	chassis -> turnAngle(-135_deg);
	chassis -> setMaxVelocity(MAX_VELOCITY * 0.25); //slow down more so we dont hit the goal away
	chassis -> moveDistance(-20_in);
	mogoClamp.extend(); // grab goal
	lift.move(127);
	chassis -> setMaxVelocity(MAX_VELOCITY); 
	chassis -> turnAngle(-90_deg); //turn to next ring
	chassis -> setMaxVelocity(MAX_VELOCITY * 0.4); 
	chassis -> moveDistance(24_in);
	chassis -> setMaxVelocity(MAX_VELOCITY); 
	chassis -> turnAngle(-45_deg); //turn to corner
	chassis -> setMaxVelocity(MAX_VELOCITY * 0.4); 
	chassis -> moveDistance(13.5_in); //go into corner
	pros::delay(200); //grab ring
	chassis -> moveDistance(-13.5_in); //back up and spin around
	lift.move(0); 
	chassis -> setMaxVelocity(MAX_VELOCITY); 
	chassis -> turnAngle(180_deg); //turn around so back is facing corner
	chassis -> setMaxVelocity(MAX_VELOCITY * 0.4); 
	// pros::Task* cornerMoveTask = new pros::Task(cornerMoveFunct);
	chassis -> moveDistanceAsync(-20_in);
	pros::delay(2300);
	mogoClamp.retract(); //drop goal in corner

	pros::delay(200);

	chassis -> moveDistance(24_in);
	chassis -> setMaxVelocity(MAX_VELOCITY); 
	chassis -> turnAngle(-135_deg);
	chassis -> setMaxVelocity(MAX_VELOCITY * 0.4); 
	chassis -> moveDistance(-48_in);
	chassis -> setMaxVelocity(MAX_VELOCITY); 
	chassis -> turnAngle(-45_deg);

	chassis -> setMaxVelocity(MAX_VELOCITY * .2);
	chassis -> moveDistance(-24_in);

	mogoClamp.extend();

	chassis -> setMaxVelocity(MAX_VELOCITY);

	chassis -> turnAngle(90_deg);

	chassis -> setMaxVelocity(MAX_VELOCITY * 0.4); 
	chassis -> moveDistance(-48_in);

	/**/

}

void dropGoalAuto(){
	ladyBrownGroup.moveAbsolute(200, 30);
	chassis -> setMaxVelocity(MAX_VELOCITY * .3);
	chassis -> setGains(
		{0.002, 0.0, 0.0}, 
		{0.00095, 0, 0}, 
		{0, 0, 0});
	chassis -> moveDistance(-4_in);
	mogoClamp.extend();
	chassis -> setMaxVelocity(MAX_VELOCITY * .5);
	chassis -> setGains(
		{0.001, 0.0, 0.0}, 
		{0.00095, 0, 0}, 
		{0, 0, 0});
	chassis -> moveDistance(12_in);
	chassis -> turnAngle(-20_deg);
	lift.move(127);
	pros::delay(500);
	lift.move(-127);
	pros::delay(500);
	lift.move(127);
	chassis -> moveDistance(30_in);
	chassis -> turnAngle(-135_deg);
	chassis -> moveDistance(-12_in);
	mogoClamp.retract();
	lift.move(0);
	ladyBrownGroup.moveAbsolute(245, 30);
	chassis -> moveDistance(54_in);
}

void keepGoalAuto(){
	ladyBrownGroup.moveAbsolute(200, 30);
	chassis -> setMaxVelocity(MAX_VELOCITY * .3);
	chassis -> setGains(
		{0.002, 0.0, 0.0}, 
		{0.00095, 0, 0}, 
		{0, 0, 0});
	chassis -> moveDistance(-4_in);
	mogoClamp.extend();
	chassis -> setMaxVelocity(MAX_VELOCITY * .5);
	chassis -> setGains(
		{0.001, 0.0, 0.0}, 
		{0.00095, 0, 0}, 
		{0, 0, 0});
	chassis -> moveDistance(12_in);
	chassis -> turnAngle(-20_deg);
	lift.move(127);
	pros::delay(500);
	lift.move(-127);
	pros::delay(500);
	lift.move(127);
	chassis -> moveDistance(30_in);
	chassis -> turnAngle(-145_deg);
	ladyBrownGroup.moveAbsolute(245, 30);
	lift.move(0);
	chassis -> moveDistance(54_in);
}


void keepGoalAutoElims(){
	ladyBrownGroup.moveAbsolute(200, 30);
	chassis -> setMaxVelocity(MAX_VELOCITY * .3);
	chassis -> setGains(
		{0.002, 0.0, 0.0}, 
		{0.00095, 0, 0}, 
		{0, 0, 0});
	chassis -> moveDistance(-4_in);
	mogoClamp.extend();
	chassis -> setMaxVelocity(MAX_VELOCITY * .5);
	chassis -> setGains(
		{0.001, 0.0, 0.0}, 
		{0.00095, 0, 0}, 
		{0, 0, 0});
	chassis -> moveDistance(12_in);
	chassis -> turnAngle(-20_deg);
	lift.move(127);
	pros::delay(500);
	lift.move(-127);
	pros::delay(500);
	lift.move(127);
	chassis -> moveDistance(30_in);
	chassis -> turnAngle(-145_deg);
	chassis -> moveDistance(-36_in);
	return;
}
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
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	chassis -> stop();
	drivetrain -> stop();


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
			keepGoalAuto();
			break;
		case 1:
			//Match Plus Side Drop Goal Auto
			keepGoalAutoElims();
			break;
		case 2:
			//Match Climb Goal Keep Goal Autowatch climb keep
			dropGoalAuto();
			break;
		case 3:
			//Match Climb Goal Drop Goal Selected
			break;
		case 4:
			//Skills
			skills_autonomous();
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
	chassis -> stop();
	drivetrain -> stop();


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
		if (master.get_digital(DIGITAL_R1)) { 
			lift.move(127);
		} else if (master.get_digital(DIGITAL_R2)) {
			lift.move(-127);
		} else {
			lift.move(0);
		}
		
		//Toggle the goal clamp on the back (pneumatic clamp)
		if (master.get_digital_new_press(DIGITAL_B)) { 
			mogoClamp.toggle();
		}

		// if (master.get_digital(DIGITAL_RIGHT)) {
		// 	skills_autonomous();
		// }

		if (master.get_digital_new_press(DIGITAL_L2)) { 
			intakeFront = !intakeFront;
		}


		// if (master.get_digital(DIGITAL_UP)) { 
		// 	ladyBrownGroup.moveVoltage(12000);
		// } else if (master.get_digital(DIGITAL_DOWN)) {
		// 	ladyBrownGroup.moveVoltage(-12000);
		// } else {
		// 	ladyBrownGroup.moveVoltage(0);
		// }

		if (master.get_digital_new_press(DIGITAL_DOWN)) {
			ladyBrownGroup.moveAbsolute(0, 30); //home ladybrown arm
		} else if (master.get_digital_new_press(DIGITAL_RIGHT)){
			ladyBrownGroup.moveAbsolute(55, 60); //for grabbing rings
		} else if (master.get_digital_new_press(DIGITAL_UP)){
			ladyBrownGroup.moveAbsolute(255, 90); //score
		}
		// } else if(ladyBrownGroup.isStopped()){
		// 	ladyBrownGroup.moveAbsolute(ladyBrownGroup.getPosition(), 30); //hold postion other wise
		// }
		// if(count == 10){
		// 	avgCurr = std::fmax(avgCurr, ((frontLeft.getCurrentDraw() + frontRight.getCurrentDraw() + 
		// 			backLeft.getCurrentDraw() + backRight.getCurrentDraw() + 
		// 			topLeft.getCurrentDraw() + topRight.getCurrentDraw()) / 6));
		// 	master.set_text(1, 1, std::to_string(avgCurr));
			
		// 	count = 0;
		// }
		// maxCurr = std::fmax(maxCurr, frontLeft.getCurrentDraw());

		// count++;
		if(master.get_digital(DIGITAL_L1)){
			drivetrain->arcade(dir, turn); 
		} else {
			drivetrain->arcade(dir * .6, turn * .6); // Takes in the inputs from the analog sticks and moves the robot accordingly using arcade controls.
		}

		pros::delay(20);                               // Run for 20 ms then update
	}
}