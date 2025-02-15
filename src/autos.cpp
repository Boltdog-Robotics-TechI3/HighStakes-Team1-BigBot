#include "main.h"

void goalRush(){
    chassis.setPose(-49.0, -28.0, 100.0);
    chassis.moveToPose(-12.0, -36.0, 100.0, 10000, {.forwards = true}, false);
    rushMech.extend();
}

void safeAuto(){
    chassis.setPose(0,0,0);
    chassis.moveToPoint(0,24,10000);
    //chassis.setPose(-50.259, -14.009, 10);
    //chassis.moveToPose(-47.39, -0.237, 0,2000, {.forwards=false});
    //chassis.moveToPose(-47.772, -46.525, 0, 2000);
    //chassis.moveToPose(-23.481 ,-46.525, 90, 2000);
    //chassis.moveToPose(-10.092, -13.818, 0, 2000);
}

