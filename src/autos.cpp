#include "main.h";

void skillsAuto() {
	intake.move(127);
	chassis->moveDistance(36_in);
	turnAngle(-90);
	chassis->moveDistance(-24_in);
	mogoClamp.extend();
	lift.move(127);
}

void goalRushAuto() {

}
