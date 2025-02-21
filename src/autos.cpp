#include "main.h";

//😱
// TODO: Set headings at the beginning of autos
void skillsAuto() {
	// grab first ring
	intake.move(127);
	chassis->moveDistance(36_in);
	intake.move(0);
	master.print(0,0,"%f",gyro.get_rotation());
	// turn to and grab mogo
	turnToHeading(270);
	master.print(0,0,"%f",gyro.get_rotation());
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
	chassis->moveDistance(16_in);
	//line up with ring 4
	turnToHeading(0);
	master.print(0,0, "%f",gyro.get_heading());
	pros::Task task(intakeUntilColor);
	chassis->moveDistance(5_ft);


}


void grabGoal(void* param) {
    chassis->moveDistance(34_in);
}

void goalRushAuto() {
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
	turnToHeading(310);
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
	pros::delay(1000);
	ladyBrownGroup.moveVoltage(0);
	chassis -> moveDistanceAsync(5_in);
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
	gyro.set_heading(0);
	turnToHeading(90);
	intake.move(100);
	pros::delay(2000);
	intake.move(0);
	turnToHeading(180);
	intake.move(100);
	pros::delay(2000);
	intake.move(0);
	turnToHeading(135);
	intake.move(100);
	pros::delay(2000);
	intake.move(0);
	turnToHeading(45);
	intake.move(100);
	pros::delay(2000);
	intake.move(0);

}