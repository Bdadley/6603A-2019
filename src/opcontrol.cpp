#include "main.h"
#include "globals.h"


pros::ADIAnalogIn lightSensor('A');

//THIS IS THE DRIVER CONTROL FILE
void opcontrol() {
	lightSensor.calibrate();

	//Initialize Controller as "master"
	pros::Controller master(pros::E_CONTROLLER_MASTER);

  //Variables
	const int deadzone = 40; //The distance that you can move the joystick without making the bot move. Used to prevent accidental movement.
  bool liftingArm = false; //Checks to see if the bot is currently executing the tower task
	int upperLiftLimit = 2600; //The farthest that the tray can rotate before stopping.

	//Clear Encoders
	angleAdjuster.tare_position(); //Set tray encoder to 0
	arm.tare_position(); //Set arm encoder to 0

	master.set_text(0, 0, "Driver Control     ");
	while (true) {

		//Left Base Control
		if(abs(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)) > deadzone) {
			backLeft.move(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
			frontLeft.move(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) == 1)
		{
			backLeft.move(40);
			frontLeft.move(40);
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) == 1)
		{
			backLeft.move(-40);
			frontLeft.move(-40);
		}
		else {
			backLeft.move(0);
			frontLeft.move(0);
		}

		//Right Base Control
		if(abs(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)) > deadzone) {
			backRight.move(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));
			frontRight.move(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) == 1)
		{
			backRight.move(40);
			frontRight.move(40);
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) == 1)
		{
			backRight.move(-40);
			frontRight.move(-40);
		}
		else {
			backRight.move(0);
			frontRight.move(0);
		}




		//Arm Control
		if(liftingArm == false)
		{
			if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2) == 0 && master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) == 1)
			{
				arm.move(-127);
			}
			else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2) == 1 && master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) == 0)
			{
				arm.move(127);
			}
			else if(arm.get_position() < -600)
			{
				arm.move(-30);
			}
			else
			{
				arm.move(0);
			}
		}


		//Automatic Tower Lift -- DO NOT TOUCH
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 1)// && angleAdjuster.get_position() < 50)
		{
			liftingArm = true;
			if(angleAdjuster.get_position() > -850)
			{
				angleAdjuster.move(-95);
			}
			else
			{
				angleAdjuster.move(0);
				if(arm.get_position() > -2550)
				{
					arm.move(-127);
				}
				else
				{
					arm.move(0);
					if(angleAdjuster.get_position() < 0)
					{
						angleAdjuster.move(95);
					}
					else
					{
						angleAdjuster.move(0);
					}
				}
			}
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) == 1)
		{
			liftingArm = true;
			if(angleAdjuster.get_position() > -850)
			{
				angleAdjuster.move(-95);
			}
			else
			{
				angleAdjuster.move(0);
				if(arm.get_position() > -1750)
				{
					arm.move(-127);
				}
				else
				{
					arm.move(0);
					if(angleAdjuster.get_position() < 0)
					{
						angleAdjuster.move(95);
					}
					else
					{
						angleAdjuster.move(0);
					}
				}
			}
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 0)
		{
			liftingArm = false;
		}



		//Debugging
		//std::string anglePos = std::to_string(angleAdjuster.get_position()); //Writes the current encoder value of the tray to the screen
		//std::string armPos = std::to_string(arm.get_position());
		//pros::lcd::set_text(3, anglePos);
		//pros::lcd::set_text(4, armPos);
		//master.set_text(0, 0, "Hello");


		//Loading Dock Angle Adjustment
		if(liftingArm == false)
		{
			if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X) == 1 && master.get_digital(pros::E_CONTROLLER_DIGITAL_B) == 0)
			{
				if(angleAdjuster.get_position() * -1 < 1650)
				{
					angleAdjuster.move(-127); //If the tray has not reached a certain point, lift fast
				}
				else if(angleAdjuster.get_position() * -1 < upperLiftLimit)
				{
					angleAdjuster.move(-65); //Once it reaches that point, lift slowwer -- for more precise lifting
				}
				else if(angleAdjuster.get_position() * -1 > upperLiftLimit)
				{
					angleAdjuster.move(0); //Do not move the tray past a certain point
				}

				if(angleAdjuster.get_position() * -1 > 1450 && angleAdjuster.get_position() * -1 < upperLiftLimit)
				{
					arm.move(127);
				}
				else
				{
					arm.move(0);
				}
			}
			else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X) == 0 && master.get_digital(pros::E_CONTROLLER_DIGITAL_B) == 1)
			{
				angleAdjuster.move(95); //Lower the tray
			}
			else if(angleAdjuster.get_position() < -200 && angleAdjuster.get_position() > -2200)
			{
				angleAdjuster.move(0); //If no button is pressed, dont move the tray
			}
			else
			{
				angleAdjuster.move(0); //If no button is pressed, dont move the tray
			}
		}



		//Intake Control - DO NOT TOUCH
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) == 1 && master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) == 0) //If Pressing R1 (intake)
		{
			intake1.move(127); //Rotate intake inwards
			intake2.move(127);
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) == 0 && master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) == 1) //If Pressing R1 (outake)
		{
			intake1.move(-76); //Rotate intake outwards
			intake2.move(-76);
		}
		else //otherwise...
		{
			intake1.move(0); //Don't rotate
			intake2.move(0);
		}

		pros::delay(20); //DO NOT TOUCH!!! Should say "pros::delay(20);"
	}
}
