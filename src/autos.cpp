#include "main.h";

//😱

void skillsAuto() {
	intake.move(127);
	chassis->moveDistance(36_in);
	turnAngle(-90);
	chassis->moveDistance(-24_in);
	mogoClamp.extend();
	lift.move(127);
}


void grabGoal(void* param) {
    chassis->moveDistance(34_in);
}

void goalRushAuto() {
	gyro.set_heading(5);
	chassis -> moveDistanceAsync(34_in);
	pros::delay(800);
	rushMech.extend();
	chassis -> moveDistance(-10_in);
	rushMech.retract();
	chassis -> moveDistance(-5_in);
	turnAngle(35);
	rushMech.extend();
	turnAngle(55);
	rushMech.retract();
	turnAngle(90);

}


void safePath() {
	chassis-> moveDistance(-3_in);
	mogoClamp.extend();
	turnAngle(-45);
	intake.move(90);
	lift.move(127);
	chassis->setMaxVelocity(MAX_VELOCITY *.6);
	chassis->moveDistance(56_in);
	turnAngle(90);
	chassis->moveDistance(12_in);
}