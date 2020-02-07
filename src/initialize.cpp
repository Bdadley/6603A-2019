#include "main.h"
#include "globals.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Imu imu (17);

int phase = 0;
int team = 0;
int teamIndex = -1;
int position = 0;
int positionIndex = 1;
bool textUpdated = false;

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
 void on_left_button()
 {
	 if(phase == 1)
	 {
		 if(teamIndex == 1)
		 {
			 teamIndex = -1;
			 pros::lcd::set_text(1, "[Red]   Blue    Skills ");
		 }
		 else if(teamIndex == -1)
		 {
			teamIndex = 2;
		 	pros::lcd::set_text(1, " Red    Blue   [Skills]");
		 }
		 else if(teamIndex == 2)
		 {
			teamIndex = 1;
 		 	pros::lcd::set_text(1, " Red   [Blue]   Skills ");
		 }
		 pros::delay(500);
	 }
	 else if (phase == 2)
	 {
		 if(positionIndex == 1)
		 {
		 	pros::lcd::set_text(1, "Small   [Big]    None ");
		 	positionIndex = 3;
		 }
		 else if(positionIndex == 2)
		 {
		 	pros::lcd::set_text(1, "Small   Big    [None]");
		 	positionIndex = 1;
		 }
		 else if(positionIndex == 3)
		 {
		 	pros::lcd::set_text(1, "[Small]   Big    None ");
		 	positionIndex = 2;
		 }
		 pros::delay(500);
	 }
 }

 void on_center_button()
 {
	 if(phase==1)
	 {
	 	team = teamIndex;
		pros::lcd::set_text(1, "Select A Position:");
	 	pros::lcd::set_text(1, "Small   Big    [None]");
	 	pros::delay(500);
	 	if(teamIndex != 2)
	 	{
	 		phase = 2;
	 	}
	 	else
	 	{
	 		phase = 3;
	 	}
		if(team == 1)
		{
			pros::lcd::set_text(3, "Team: Blue");
		}
		else if(team == -1)
		{
			pros::lcd::set_text(3, "Team: Red");
		}
		else if(team == 2)
		{
			pros::lcd::set_text(3, "Skills");
		}
	 }
	 else if (phase == 2)
	 {
		 position = positionIndex;
		 phase = 3;
		 pros::delay(500);
		 pros::lcd::set_text(0, "                         ");
		 pros::lcd::set_text(1, "                         ");
		 if(position == 1)
		 {
			 pros::lcd::set_text(4, "Position: None Selected");
		 }
		 else if(position == 2)
		 {
			 pros::lcd::set_text(4, "Position: Small Zone");
		 }
		 else if(position == 3)
		 {
			 pros::lcd::set_text(4, "Position: Big Zone");
		 }
		 pros::lcd::set_text(0,"Please Wait...");
		 myTeam = team;
		 if(team != 2)
		 {
			 selectedAuton = position;
		 }
		 phase = 4;
	 }
 }

 void on_right_button()
 {
	 if(phase == 1)
	 {
	 	if(teamIndex == 1)
	 	{
			teamIndex = 2;
	 		pros::lcd::set_text(1, " Red    Blue   [Skills]");
	 	}
	 	else if(teamIndex == -1)
	 	{
			teamIndex = 1;
	 		pros::lcd::set_text(1, " Red   [Blue]    Skills ");
	 	}
	 	else if(teamIndex == 2)
	 	{
			teamIndex = -1;
	 		pros::lcd::set_text(1, " [Red]   Blue   Skills ");
	 	}
	 	pros::delay(500);
	 }
	 else if (phase == 2)
	 {
		 if(positionIndex == 1)
		 {
		 	pros::lcd::set_text(1, "[Small]   Big    None ");
		 	positionIndex = 2;
		 }
		 else if(positionIndex == 2)
		 {
		 	pros::lcd::set_text(1, "Small   [Big]    None ");
		 	positionIndex = 3;
		 }
		 else if(positionIndex == 3)
		 {
		 	pros::lcd::set_text(1, "Small   Big    [None]");
		 	positionIndex = 1;
		 }
		 pros::delay(500);
	 }
 }

void competition_initialize()
{
	pros::lcd::initialize();
	imu.reset();
	while(imu.is_calibrating())
	{
			pros::lcd::set_text(0, "Calibrating...");
	}
	phase = 4;
	//pros::lcd::register_btn0_cb(on_left_button);
	//pros::lcd::register_btn1_cb(on_center_button);
	//pros::lcd::register_btn2_cb(on_right_button);

	//Auton Selector
	//pros::lcd::set_text(0, "Select A Team:");
	//pros::lcd::set_text(1, "[Red]   Blue    Skills ");

}
