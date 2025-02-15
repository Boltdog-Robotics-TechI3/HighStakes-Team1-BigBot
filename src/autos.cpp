#include "main.h"

void goalRush(){
    chassis.setPose(-49.0, -28.0, 100.0);
    chassis.moveToPose(-12.0, -36.0, 100.0, 10000, {.forwards = true}, false);
    rushMech.extend();
}

void safeAuto(){
    
}


