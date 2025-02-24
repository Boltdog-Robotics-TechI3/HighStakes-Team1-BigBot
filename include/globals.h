#pragma once
#include "main.h"

extern int MAX_VELOCITY;


extern int teleOPCurrentLimit;

#define setMinAbs(num, bound) ((num > -(bound) && num < bound) ? ((num >= 0) ? bound : -(bound)) : num)

// Comp Specifications
extern bool skills;
extern bool elim;
extern bool qual;
extern bool redAlliance;
extern bool blueAlliance;
extern bool match;
extern int autoSelection;

extern bool isSorting;

extern pros::Controller master;
extern pros::Motor lift;
extern pros::Motor intake;

extern pros::Optical optical;

extern okapi::Motor ladyBrownLeft;
extern okapi::Motor ladyBrownRight;

extern okapi::MotorGroup ladyBrownGroup;

extern pros::adi::Pneumatics mogoClamp;
extern pros::adi::Pneumatics rushMech;

void colorSorting();

void intakeUntilColor();

void colorSortingAuto();

void ladyBrownScore();

void ladyBrownDown();

void ladyBrownPrime();
