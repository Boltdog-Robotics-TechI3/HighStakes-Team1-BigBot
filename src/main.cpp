#include "main.h"


const int MAX_VELOCITY = 600;

bool intakeFront = true;

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor lift(11);

okapi::Motor frontRight(3);
okapi::Motor backRight(1);
okapi::Motor topRight(-2);

okapi::Motor frontLeft(-8);
okapi::Motor backLeft(9);
okapi::Motor topLeft(-10);

okapi::MotorGroup right({frontRight, topRight, backRight});
okapi::MotorGroup left({frontLeft, topLeft, backLeft});


std::shared_ptr<ChassisController> chassis = ChassisControllerBuilder()
	.withMotors(left, right)
	.withDimensions({AbstractMotor::gearset::blue, (84.0/60.0)}, {{4_in, 15.25_in}, imev5BlueTPR})
	.withGains({0.00285, 0.0, 0.0/*0.000075*/}, {0.00095, 0, 0}, {0, 0, 0})
	.build();

std::shared_ptr<ChassisModel> drivetrain = chassis->getModel();

pros::adi::Pneumatics mogoClamp = pros::adi::Pneumatics('A', false);


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
void skills_autonomous() {

	//Score 1 ring on alliance stake
	chassis -> setMaxVelocity(MAX_VELOCITY * 0.4); 
	lift.move(127); //start intake 
	chassis -> moveDistance(6_in); //grab 1st ring
	pros::delay(150);
	lift.move(0);
	chassis -> moveDistance(-6_in); //go back to alliance stake 
	lift.move(127); //start intake 
	pros::delay(1250); //score ring

	//Put 3 rings on mobile goal and place in corner
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
	chassis -> moveDistance(-15_in); 
	mogoClamp.retract(); //drop goal in corner
	/*
	chassis -> moveDistance(4_ft);
	chassis -> turnAngle(-45_deg);
	chassis -> moveDistance(28_in); // going to the corner
	
	//Drop off ring
	chassis -> turnAngle(180_deg); 
	mogoClamp.retract();
	pros::delay(300);
	
	chassis -> turnAngle(45_deg); // going for the stack of rings (we will discard them)
	chassis -> moveDistance(52_in); 
	// chassis -> turnAngle(180_deg);
	// chassis -> moveDistance(36_in); // going for mobile goal
	// chassis -> turnAngle(90_deg); 
	// chassis -> moveDistance(18_in); // going for the 5th ring
	// chassis -> turnAngle(-90_deg);
	// chassis -> moveDistance(); // going for next 
	// chassis -> turnAngle();
	// chassis ->moveDistance(); // going to the corner
	*/

}
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
	drivetrain->setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
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
void autonomous() {}

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

int currentLimit = 2200;


void opcontrol() {
	//Controls:
	//B button toggles clamp
	//A button controls intake and lift
	//Left joystick controls forward/backwards movement
	//Right joystick controls turning
	drivetrain -> setBrakeMode(okapi::AbstractMotor::brakeMode::coast);

	frontLeft.setCurrentLimit(currentLimit);
	frontRight.setCurrentLimit(currentLimit);
	topLeft.setCurrentLimit(currentLimit);
	topRight.setCurrentLimit(currentLimit);
	backLeft.setCurrentLimit(currentLimit);
	backRight.setCurrentLimit(currentLimit);

	master.rumble(".");

	int maxCurr = 0;
	int count = 0;
	double eff = 0;
	int avgCurr = 0;

	while (true) {
		int dir;
		int turn;

		if(intakeFront){
			dir = (pow((master.get_analog(ANALOG_LEFT_Y) / 127.0), 3) * 127.0);    // Gets amount forward/backward from left joystick
			turn = (pow((master.get_analog(ANALOG_RIGHT_X) / 127.0), 3) * 127.0);  // Gets the turn left/right from right joystick
		} else {
			dir = (pow((-master.get_analog(ANALOG_LEFT_Y) / 127.0), 3) * 127.0);
			turn = (pow((master.get_analog(ANALOG_RIGHT_X) / 127.0), 3) * 127.0);
		}

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

		if (master.get_digital(DIGITAL_RIGHT)) {
			skills_autonomous();
		}

		if (master.get_digital_new_press(DIGITAL_L2)) { 
			intakeFront = !intakeFront;
		}

		if(count == 10){
			avgCurr = std::fmax(avgCurr, ((frontLeft.getCurrentDraw() + frontRight.getCurrentDraw() + 
					backLeft.getCurrentDraw() + backRight.getCurrentDraw() + 
					topLeft.getCurrentDraw() + topRight.getCurrentDraw()) / 6));
			master.set_text(1, 1, std::to_string(avgCurr));
			
			count = 0;
		}
		// maxCurr = std::fmax(maxCurr, frontLeft.getCurrentDraw());

		count++;

		drivetrain->arcade(dir, turn); // Takes in the inputs from the analog sticks and moves the robot accordingly using arcade controls.
		pros::delay(20);                               // Run for 20 ms then update
	}
}