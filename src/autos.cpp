#include "main.h";

//😱
// TODO: Set headings at the beginning of autos
void skillsAuto() {
	intake.move(127);
	chassis->moveDistance(36_in);
	intake.move(0);
	turnAngle(-90);
	chassis->setMaxVelocity(MAX_VELOCITY * 0.4);
	chassis->moveDistance(-21_in);
	mogoClamp.extend();
	intake.move(127);
	lift.move(127);
	chassis->setMaxVelocity(MAX_VELOCITY * 0.8);
	turnAngle(-90);
	chassis->moveDistance(24_in);
	turnAngle(-45);
	chassis->setMaxVelocity(MAX_VELOCITY * 0.4);
	//Poor man's delay
	chassis->moveDistanceAsync(28_in);
	pros::delay(1500);
	chassis->stop();

	chassis->moveDistance(-18_in);
	turnAngle(180);

	chassis->moveDistanceAsync(-18_in);
	pros::delay(1500);
	chassis->stop();

	chassis->setMaxVelocity(MAX_VELOCITY * 0.8);
	mogoClamp.retract();
	chassis->moveDistance(20_in);
	turnAngle(45);
	lift.move(0);
	chassis->moveDistance(48_in);
	turnAngle(125);
	chassis->setMaxVelocity(MAX_VELOCITY * 0.5);
	chassis->moveDistance(20_in);
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
	turnAngle(190);
	chassis -> setMaxVelocity(MAX_VELOCITY * .6);
	chassis -> moveDistance(-16_in);
	mogoClamp.extend();
	turnAngle(-55);
	chassis -> setMaxVelocity(MAX_VELOCITY * .2);
	lift.move(127);
	intake.move(127);
	chassis -> moveDistance(16_in);
	turnAngle(40);
	chassis -> setMaxVelocity(MAX_VELOCITY);
	chassis -> moveDistance(24_in);
	pros::delay(2000);
	turnAngle(135);
	chassis -> setMaxVelocity(MAX_VELOCITY * .2);
	chassis -> moveDistanceAsync(-16_in);
	pros::delay(2500);
	mogoClamp.retract();
	chassis -> moveDistanceAsync(54_in);

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