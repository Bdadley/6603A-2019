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
	//Auton Selector
	int phase = 1;
	int team = 0;
	int teamIndex = 1;
	int position = 0;
	int positionIndex = 1;
	bool textUpdated = false;
	pros::delay(1000);
	master.set_text(0, 0, "[R]   B    S ");
		while(phase == 1) //-1 = Blue, 1 = Red, 2 = Skills
		{
			if(master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) == 1)
			{
					if(teamIndex == 1)
					{
						teamIndex = 2;
						master.set_text(0, 0, " R    B   [S]");
					}
					else if(teamIndex == -1)
					{
						teamIndex = 1;
						master.set_text(0, 0, "[R]   B    S ");
					}
					else if(teamIndex == 2)
					{
						teamIndex = -1;
						master.set_text(0, 0, " R   [B]   S ");
					}
					pros::delay(500);
			}
			else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT) == 1)
			{
				if(teamIndex == 1)
				{
					teamIndex = -1;
					master.set_text(0, 0, " R   [B]   S ");
				}
				else if(teamIndex == -1)
				{
					teamIndex = 2;
					master.set_text(0, 0, " R    B   [S]");
				}
				else if(teamIndex == 2)
				{
					teamIndex = 1;
					master.set_text(0, 0, "[R]   B    S ");
				}
				pros::delay(500);
			}
			else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 1)
			{
				team = teamIndex;
				master.set_text(0, 0, "None         ");
				pros::delay(500);
				if(teamIndex != 2)
				{
					phase = 2;
				}
				else
				{
					phase = 3;
				}
			}
		}

		while(phase == 2) //1 = None, 2 = Small Zone, 3 = Big Zone
		{
			if(master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) == 1)
			{
				if(positionIndex == 1)
				{
					master.set_text(0, 0, "Big Zone         ");
					positionIndex = 3;
				}
				else if(positionIndex == 2)
				{
					master.set_text(0, 0, "None          ");
					positionIndex = 1;
				}
				else if(positionIndex == 3)
				{
					master.set_text(0, 0, "Small Zone        ");
					positionIndex = 2;
				}
				pros::delay(500);
			}
			else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT) == 1)
			{
				if(positionIndex == 1)
				{
					master.set_text(0, 0, "Small Zone         ");
					positionIndex = 2;
				}
				else if(positionIndex == 2)
				{
					master.set_text(0, 0, "Big Zone          ");
					positionIndex = 3;
				}
				else if(positionIndex == 3)
				{
					master.set_text(0, 0, "None        ");
					positionIndex = 1;
				}
				pros::delay(500);
			}
			else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 1)
			{
				position = positionIndex;
				phase = 3;
				pros::delay(500);
			}
		}

		if(phase == 3)
		{
			master.set_text(0, 0, "Please Wait...");
			myTeam = team;
			if(team != 2)
			{
				selectedAuton = position;
			}
			phase = 4;
		}
}
