#include "main.h";



// void intakeUntilColor(void* param) {
//     while (true) {
//         lift.move(127);
//         if (redAlliance && optical.get_hue() >= 1 && optical.get_hue() <= 25) {
//             lift.move(0);
//             break;
//         }
//         else if (blueAlliance && optical.get_hue() >= 160 && optical.get_hue() <= 230) {
//             lift.move(0);
//             break;
//         }
//         pros::delay(20);
//     }
// }

//😱
// TODO: Set headings at the beginning of autos
void skillsAuto() {
	// grab first ring
	chassis -> setGains(
		{0.0017, 0.0, 0.0000005}, 
		{3.0, 0.001, 1}, 
		{0, 0, 0}
	);
	intake.move(127);
	chassis->moveDistanceAsync(36_in);
	pros::delay(1000);
	chassis -> stop();
	intake.move(0);
	// turn to and grab mogo
	turnToHeading(270);
	chassis->setMaxVelocity(MAX_VELOCITY * 0.4);
	chassis->moveDistance(-21_in);
	mogoClamp.extend();
	intake.move(127);
	lift.move(127);
	chassis->setMaxVelocity(MAX_VELOCITY * 0.8);
	// grab ring ring 2 
	turnToHeading(180);
	chassis->moveDistance(24_in);
	//turn to face cornor
	turnToHeading(140);
	chassis->setMaxVelocity(MAX_VELOCITY * 0.4);
	// grab the ring in  the cornor
	chassis->moveDistanceAsync(28_in);
	pros::delay(1500);
	chassis->stop();
	//back out of cornor
	chassis->moveDistance(-18_in);
	//turn so the mogo is lined up with cornor
	turnToHeading(315);
	// back into cornor
	chassis->moveDistanceAsync(-18_in);
	pros::delay(1500);
	chassis->stop();

	chassis->setMaxVelocity(MAX_VELOCITY * 0.8);
	//drop mogo in cornor
	mogoClamp.retract();
	pros::delay(100);
	// move out of a cornor
	chassis->moveDistance(14_in);
	//line up with ring 4
	turnToHeading(0);
	intake.move(127);
	pros::Task task(intakeUntilColor);
	chassis->moveDistanceAsync(50_in);
	pros::delay(2000);
	chassis -> stop();
	drivetrain -> stop();
	chassis -> moveDistanceAsync(18_in);
	pros::delay(500);
	chassis -> stop();
	drivetrain -> stop();
	lift.move(0);
	turnToHeading(90);
	chassis -> setMaxVelocity(.4 * MAX_VELOCITY);
	chassis -> moveDistance(-20_in);
	mogoClamp.extend();
	chassis -> setMaxVelocity(MAX_VELOCITY);

	turnToHeading(45);

	lift.move(127);
	chassis -> moveDistanceAsync(32_in);
	pros::delay(1000);
	chassis -> stop();
	drivetrain -> stop();
	turnToHeading(225);

	chassis -> setMaxVelocity(.4 * MAX_VELOCITY);
	lift.move(0);

	chassis -> moveDistanceAsync(-18_in);
	pros::delay(3000);

	mogoClamp.retract();
	chassis -> moveDistance(6_in);

	turnToHeading(45);

	chassis -> moveDistanceAsync(-90_in);
	
	ladyBrownGroup.moveVoltage(-12000);
	pros::delay(500);
	ladyBrownGroup.moveVoltage(0);
	pros::delay(2000);
	chassis -> moveDistance(4_in);
	task.remove();
	pros::delay(10000);
}


void grabGoal(void* param) {
    chassis->moveDistance(34_in);
}

void goalRushAutoRed() {
	gyro.set_heading(15);
	chassis -> moveDistanceAsync(34_in);
	pros::delay(1000);
	rushMech.extend();
	chassis -> moveDistance(-10_in);
	rushMech.retract();
	chassis -> moveDistance(-5_in);
	turnToHeading(205);
	chassis -> setMaxVelocity(MAX_VELOCITY * .4);
	chassis -> moveDistance(-16_in);
	mogoClamp.extend();
	lift.move(127);
	intake.move(127);
	turnToHeading(175);
	chassis -> moveDistance(36_in);
	pros::delay(2000);
	lift.move(0);
	intake.move(0);
	turnToHeading(320);
	chassis -> setMaxVelocity(MAX_VELOCITY * .2);
	chassis -> moveDistanceAsync(-16_in);
	pros::delay(2500);
	chassis -> stop();
	drivetrain -> stop();
	mogoClamp.retract();
	pros::delay(100);
	chassis -> setMaxVelocity(MAX_VELOCITY);
	chassis -> moveDistanceAsync(50_in);
	ladyBrownGroup.moveVoltage(-12000);
	pros::delay(500);
	ladyBrownGroup.moveVoltage(0);
	pros::delay(5000);
	chassis -> stop();
	drivetrain -> stop();
	chassis -> setMaxVelocity(MAX_VELOCITY * .2);
	chassis -> moveDistanceAsync(3_in);
	pros::delay(10000);

}

void goalRushAutoBlue() {
	gyro.set_heading(15);
	chassis -> moveDistanceAsync(34_in);
	pros::delay(1000);
	rushMech.extend();
	chassis -> moveDistance(-10_in);
	rushMech.retract();
	chassis -> moveDistance(-5_in);
	turnToHeading(205);
	chassis -> setMaxVelocity(MAX_VELOCITY * .4);
	chassis -> moveDistance(-16_in);
	mogoClamp.extend();
	lift.move(127);
	intake.move(127);
	turnToHeading(175);
	chassis -> moveDistance(36_in);
	pros::delay(2000);
	lift.move(0);
	intake.move(0);
	turnToHeading(320);
	
	chassis -> moveDistanceAsync(34_in);
	ladyBrownGroup.moveVoltage(-12000);
	pros::delay(500);
	ladyBrownGroup.moveVoltage(0);
	pros::delay(5000);
	chassis -> stop();
	drivetrain -> stop();
	chassis -> setMaxVelocity(MAX_VELOCITY * .2);
	chassis -> moveDistanceAsync(7_in);
	pros::delay(10000);

}


void safePath() {
	chassis-> moveDistance(-2_in);
	mogoClamp.extend();
	turnAngle(-45);
	intake.move(90);
	lift.move(127);
	chassis->setMaxVelocity(MAX_VELOCITY *.6);
	chassis->moveDistance(56_in);
	turnAngle(90);
	chassis->moveDistance(12_in);
}

void bruhAuto(){
	intake.move(127);
	pros::Task task(intakeUntilColor);

	pros::delay(100000);
}