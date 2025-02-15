#include "main.h"
// Comp Specifications
bool skills = false;
bool match = true;
bool elim = false;
bool qual = true;
bool redAlliance = false;
bool blueAlliance = true;
int autoSelection = 0; 

// other things
const int MAX_VELOCITY = 600;

bool intakeFront = true;

int teleOPCurrentLimit = 2500;

pros::Controller driverController(pros::E_CONTROLLER_MASTER);

pros::Motor lift(-11);
pros::Motor intake(-20);

pros::MotorGroup ladyBrownGroup({5, -6}, pros::MotorGearset::green);

pros::adi::Pneumatics mogoClamp = pros::adi::Pneumatics('H', false);
pros::adi::Pneumatics rushMech = pros::adi::Pneumatics('G', false);

//Init functions