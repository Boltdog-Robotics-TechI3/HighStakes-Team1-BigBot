#include "main.h"

int MAX_VELOCITY = 600;

int teleOPCurrentLimit = 2200;
// Comp Specifications
bool skills = false;
bool match = true;
bool elim = false;
bool qual = true;
bool redAlliance = true;
bool blueAlliance = false;
int autoSelection = 0; 

bool isSorting = false;

int ladyBrownPrimed = -150;
bool ladyBrownUp = false;

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor lift(-11);
pros::Motor intake(-20);

pros::Optical optical(7);

okapi::Motor ladyBrownLeft(-5);
okapi::Motor ladyBrownRight(6);

okapi::MotorGroup ladyBrownGroup({ladyBrownLeft, ladyBrownRight});

pros::adi::Pneumatics mogoClamp = pros::adi::Pneumatics('H', false);
pros::adi::Pneumatics rushMech = pros::adi::Pneumatics('G', false);

bool seesRed(){
    return optical.get_hue() > 1 && optical.get_hue() < 25;
}

bool seesBlue(){
    return optical.get_hue() > 175 && optical.get_hue() < 230;
}

bool hasBadRing = false;

void colorSorting(){                                                                                                                                                                                                                                                                
    optical.set_led_pwm(100);
    if(blueAlliance){
        if(seesRed()){
            lift.move(127);
            hasBadRing = true;
        } else if (!seesRed() && hasBadRing) {
            lift.move_relative(150, 10000);
            isSorting = true;
            hasBadRing =false;
        }  else {
            lift.move(127);
        }
    } else {
        if(seesBlue()){
            lift.move(127);
            hasBadRing = true;
        } else if (!seesBlue() && hasBadRing) {
            lift.move(127);
            pros::delay(125);
            lift.move(-127);
            pros::delay(150);
            lift.move(0);
            pros::delay(150);
            isSorting = true;
            hasBadRing =false;
        }  else {
            lift.move(127);
        }
    }
}

void intakeUntilColor(){
    lift.move(127);
    optical.set_led_pwm(100);

    if(!blueAlliance){
        while (!(optical.get_hue() > 0 && optical.get_hue() < 25)) pros::delay(20);
    } else {
        while (!(optical.get_hue() > 180 && optical.get_hue() < 230)) pros::delay(20);
    }
    lift.move(0);
    // intake.move(0);
}

void colorSortingAuto(){
    int count = 0;
    while(1){
        if(!isSorting){
            colorSorting();
        } else {
            if(lift.get_actual_velocity() == 0){
				if(count > 5){
					isSorting = false;
					count = 0;
				}
				count++;
			}
        }

        pros::delay(20);
    }
}

void ladyBrownScore(){
    if(!ladyBrownUp){
        ladyBrownGroup.moveVoltage(-10000);
        pros::delay(600);
        ladyBrownGroup.moveVoltage(0);
    } else {
        ladyBrownGroup.moveVoltage(-4000);
        pros::delay(250);
        ladyBrownGroup.moveVoltage(0);
    }
    ladyBrownUp = true;
}

void ladyBrownDown(){
    if(ladyBrownUp){
        ladyBrownGroup.moveVoltage(10000);
        pros::delay(600);
        ladyBrownGroup.moveVoltage(0);
    } else {
        ladyBrownGroup.moveVoltage(4000);
        pros::delay(250);
        ladyBrownGroup.moveVoltage(0);
    }
    ladyBrownUp = false;
    ladyBrownGroup.tarePosition();
}

void ladyBrownPrime(){
    ladyBrownGroup.moveAbsolute(ladyBrownPrimed, 100);
    ladyBrownUp = false;
}