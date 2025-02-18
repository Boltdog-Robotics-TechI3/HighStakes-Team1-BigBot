#include "main.h"

int MAX_VELOCITY = 600;

int teleOPCurrentLimit = 2200;
// Comp Specifications
bool skills = false;
bool match = true;
bool elim = false;
bool qual = true;
bool redAlliance = false;
bool blueAlliance = true;
int autoSelection = 0; 


pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor lift(-11);
pros::Motor intake(-20);

okapi::Motor ladyBrownLeft(-5);
okapi::Motor ladyBrownRight(6);

okapi::MotorGroup ladyBrownGroup({ladyBrownLeft, ladyBrownRight});

pros::adi::Pneumatics mogoClamp = pros::adi::Pneumatics('H', false);
pros::adi::Pneumatics rushMech = pros::adi::Pneumatics('G', false);
