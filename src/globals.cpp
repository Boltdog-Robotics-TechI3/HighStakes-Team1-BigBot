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



pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor lift(-11);
pros::Motor intake(-20);

pros::Optical optical(7);

okapi::Motor ladyBrownLeft(-5);
okapi::Motor ladyBrownRight(6);

okapi::MotorGroup ladyBrownGroup({ladyBrownLeft, ladyBrownRight});

pros::adi::Pneumatics mogoClamp = pros::adi::Pneumatics('H', false);
pros::adi::Pneumatics rushMech = pros::adi::Pneumatics('G', false);

void colorSorting(){
    if(blueAlliance){
        if(optical.get_hue() > 0 && optical.get_hue() < 25){
            lift.move_relative(645.0, 10000000);
            isSorting = true;
        } else {
            lift.move(127);
        }
    } else {
        if(optical.get_hue() > 130 && optical.get_hue() < 230){
            lift.move_relative(645.0, 10000000);
            isSorting = true;
        } else {
            lift.move(127);
        }
    }
}

void intakeUntilColor(){
    lift.move(127);
    if(!blueAlliance){
        while (!(optical.get_hue() > 0 && optical.get_hue() < 25)) pros::delay(20);
    } else {
        while (!(optical.get_hue() > 130 && optical.get_hue() < 230)) pros::delay(20);
    }
    lift.move(0);
    pros::delay(100000);
}