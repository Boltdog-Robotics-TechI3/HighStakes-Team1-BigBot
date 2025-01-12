#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor lift(16);

std::shared_ptr<ChassisController> chassis = ChassisControllerBuilder()
	.withMotors({-18, 19, -20}, {11, -12, 13})
	.withDimensions({AbstractMotor::gearset::blue, (84.0/60.0)}, {{4_in, 15.25_in}, imev5BlueTPR})
	.build();

std::shared_ptr<ChassisModel> drivetrain = chassis->getModel();

pros::adi::Pneumatics mogoClamp = pros::adi::Pneumatics('H', false);

std::shared_ptr<AsyncMotionProfileController> profileController = 
  AsyncMotionProfileControllerBuilder()
    .withLimits({
      1.0, // Maximum linear velocity of the Chassis in m/s
      2.0, // Maximum linear acceleration of the Chassis in m/s/s
      10.0 // Maximum linear jerk of the Chassis in m/s/s/s
    })
    .withOutput(chassis)
    .buildMotionProfileController();


/* move 4 in
	get ring
	back 4 in
	score ring
   turn 45
   move forward 2 ft
*/
void skills_autonomous() {
	chassis->setMaxVelocity(300);
	lift.move(127);
	chassis -> moveDistance(4_in);
	pros::delay(500);
	chassis -> moveDistance(-2.25_in);
	pros::delay(500);
	chassis -> moveDistance(16_in);
	chassis -> turnAngle(45_deg);
	chassis -> moveDistance(28_in);
	pros::delay(2000);
	lift.move(0);
	chassis -> turnAngle(-135_deg);
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
void opcontrol() {
	//Controls:
	//B button toggles clamp
	//A button controls intake and lift
	//Left joystick controls forward/backwards movement
	//Right joystick controls turning

	while (true) {
		int dir = (pow((master.get_analog(ANALOG_LEFT_Y) / 127.0), 3) * 127.0);    // Gets amount forward/backward from left joystick
		int turn = master.get_analog(ANALOG_RIGHT_X);  // Gets the turn left/right from right joystick

		if (master.get_digital(DIGITAL_R1)) { 
			lift.move(127);
		} else if (master.get_digital(DIGITAL_R2)) {
			lift.move(-127);
		} else {
			lift.move(0);
		}

		if (master.get_digital_new_press(DIGITAL_B)) { 
			mogoClamp.toggle();
		}

		if (master.get_digital(DIGITAL_RIGHT)) {
			skills_autonomous();
		}
		
		drivetrain->arcade(dir, turn);
		pros::delay(20);                               // Run for 20 ms then update
	}
}