#include "main.h"
#include "globals.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);


void initialize() {
	frontLeft.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	frontRight.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	backLeft.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	backRight.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	angleAdjuster.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	intake1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	intake2.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);



}


/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {

}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize()
{

}
