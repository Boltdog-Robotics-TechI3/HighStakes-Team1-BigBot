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

void skillsAuto() {
    chassis.setPose(-60,-24, 90);
    intake.move_voltage(8000);
    chassis.moveToPoint(-24,-24,5000, {}, false);
    //driverController.print(0,0,"%f, %f", chassis.getPose().x, chassis.getPose().y);
    chassis.turnToHeading(0, 5000, {}, false);
    chassis.turnToHeading(90, 5000, {}, false);
    driverController.print(0,0,"%f, %f", chassis.getPose().x, chassis.getPose().y);
    return;

    chassis.setPose(-24, -24, 0);
    //driverController.print(0,0,"%f, %f", chassis.getPose().x, chassis.getPose().y);
    chassis.moveToPoint(-24, -40, 5000, {.forwards = false, .maxSpeed = 60}, false);
    //driverController.print(0,0, "%f", chassis.getPose().theta);
    mogoClamp.extend();
    lift.move(127);
    chassis.moveToPoint(-24, -48, 5000, {}, false);
    chassis.turnToHeading(-90, 5000, {}, false);
    driverController.print(0,0,"%f, %f", chassis.getPose().x, chassis.getPose().y);
    chassis.setPose(-24, -48, -90);
    chassis.moveToPoint(-48, -48, 5000, {}, false);
}

