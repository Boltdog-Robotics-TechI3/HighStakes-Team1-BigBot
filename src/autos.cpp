#include "drivetrain.hpp"
#include "globals.h"
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

void skillsAuto() {
	master.set_text(0, 0, "Alliance: " + std::to_string(blueAlliance));
	gyro.set_heading(0);
	// grab first ring
	intake.move(127);
	pros::delay(1000);
	intake.move(0);
	// turn to and grab mogo
	turnToHeading(270);
	mogoClamp.extend();
	intake.move(127);
	lift.move(127);

	// grab ring ring 2 
	turnToHeading(180);

	//turn to face cornor
	turnToHeading(140);

	// grab the ring in  the cornor
	pros::delay(1500);

	//back out of cornor

	//turn so the mogo is lined up with cornor
	turnToHeading(315);
	// back into cornor

	pros::delay(1500);
	
	//drop mogo in cornor
	mogoClamp.retract();
	pros::delay(100);
	// move out of a cornor
	//line up with ring 4
	turnToHeading(0);
	intake.move(127);
	pros::Task task(intakeUntilColor);
	pros::delay(2000);
	pros::delay(500);
	lift.move(0);
	turnToHeading(90);
	mogoClamp.extend();

	turnToHeading(45);

	lift.move(127);
	pros::delay(1000);
	turnToHeading(225);

	lift.move(0);

	pros::delay(3000);

	mogoClamp.retract();

	turnToHeading(45);
	
	ladyBrownGroup.move_voltage(-12000);
	pros::delay(500);
	ladyBrownGroup.move_voltage(0);
	pros::delay(2000);
	task.remove();
	pros::delay(10000);
}


void grabGoal(void* param) {

}

void goalRushAutoRed() {
	gyro.set_heading(15);
	pros::delay(800);
	rushMech.extend();
	pros::delay(500);
	rushMech.retract();
	turnAngle(195);
	mogoClamp.extend();
	lift.move(127);
	pros::delay(1000);
	intake.move(127);
	pros::Task liftMove(intakeUntilColor);
	
	turnToHeading(175);
	// pros::delay(2000);
	// lift.move(0);
	// intake.move(0);
	// turnToHeading(320);

	pros::delay(2000);
	lift.move(0);
	intake.move(0);
	turnToHeading(320);
	lift.move(127);

	pros::delay(2500);
	mogoClamp.retract();
	pros::delay(100);
	// chassis -> moveDistanceAsync(50_in);
	// ladyBrownGroup.moveVoltage(-12000);
	pros::delay(500);
	// ladyBrownGroup.moveVoltage(0);
	pros::delay(5000);
	// chassis -> setMaxVelocity(MAX_VELOCITY * .2);
	// chassis -> moveDistanceAsync(8_in);

	pros::delay(10000);

}

void goalRushAutoBlue() {
	gyro.set_heading(15);
	pros::delay(800);
	rushMech.extend();
	pros::delay(500);
	rushMech.retract();
	turnAngle(195);
	mogoClamp.extend();
	lift.move(127);
	pros::delay(1000);
	intake.move(127);
	pros::Task liftMove(intakeUntilColor);
	
	turnToHeading(175);
	

	pros::delay(2000);
	lift.move(0);
	intake.move(0);
	turnToHeading(320);
	lift.move(127);
	// chassis -> moveDistanceAsync(34_in);
	// ladyBrownGroup.moveVoltage(-12000);
	// pros::delay(500);
	// ladyBrownGroup.moveVoltage(0);
	// pros::delay(5000);
	// chassis -> setMaxVelocity(MAX_VELOCITY * .2);
	// chassis -> moveDistanceAsync(7_in);
	pros::delay(10000);

}


void safePath() {
	mogoClamp.extend();
	turnAngle(-45);
	intake.move(90);
	lift.move(127);
	turnAngle(90);
}

void bruhAuto(){
	intake.move(127);
	pros::Task task(intakeUntilColor);

	pros::delay(100000);
}

void newSkillsAuto(){
	gyro.set_heading(0);

//stage 2 rings
	intake.move(127);
	pros::Task task(intakeUntilColor);
	pros::delay(1000); //stop async move
	turnToHeading(315, 0.9); //face center ring

//grab mogo
	turnToHeading(270, 0.9); //face mogo with back
	mogoClamp.extend(); //grab mogo
	pros::Task lady(ladyBrownScore);

// score 2 staged ring and grab 2 more
	intake.move(127);
	lift.move(127); //scoring 2 staged rings

	turnToHeading(180, 0.9); //face 3rd ring
	
	turnToHeading(145, 0.9); //face the corner and 4th ring
	//move into corner async so there is a timeout
	pros::delay(1500);
	//back out of corner

	turnToHeading(315); //face mogo to corner
	mogoClamp.retract(); //drop goal
	//back into corner
	pros::delay(1400);
	
//pick up 2 more rings on the other side of the field
	//back away from corner
	turnToHeading(0, 0.9); //turn back towards more rings
	pros::Task getColor(intakeUntilColor);
	//Collect 2 rings ahead
	pros::delay(2000);
	turnToHeading(90);

//Grab mogo 2
	mogoClamp.retract();
	mogoClamp.extend();
	// lift.move(127);
	pros::Task colorsort(colorSortingAuto);

	turnToHeading(0, 0.9);
	//Take in ring
	turnToHeading(90, 0.9);
	//Take in another ring
	turnToHeading(225, 0.9);
	mogoClamp.retract();

	

//Drop mogo2 in the corner slowly (rings are still there)
	pros::delay(1000);
	colorsort.suspend();
	intake.move(0);
	lift.move(0);

//Grab third mogo
	turnToHeading(90, 0.9);
	mogoClamp.extend();
	pros::delay(100);

	// Score ring by blue stake on mogo
	// turnToHeading(45, 0.9);
	// intake.move(127);
	pros::Task colorsort3(colorSortingAuto);
	// lift.move(127);
	// pros::delay(1000);
	turnToHeading(140);
	// ladyBrownScore();

	// intake.move(0);
	// lift.move(0);
	// mogoClamp.retract();

	// turnToHeading(45, 0.9);

	// chassis->moveDistanceAsync(-2.5_ft);
	// pros::delay(2000);
	// ladyBrownPrime();
 
	pros::delay(10000); //delay so the auto doesnt end too early
}