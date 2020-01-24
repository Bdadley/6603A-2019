#include "main.h"


pros::Motor backLeft (12, pros::E_MOTOR_GEARSET_18);
pros::Motor backRight (19,pros::E_MOTOR_GEARSET_18, true);
pros::Motor frontLeft(1, pros::E_MOTOR_GEARSET_18);
pros::Motor frontRight(10, pros::E_MOTOR_GEARSET_18, true);

//Initialize Intake Motors
pros::Motor intake1 (11, pros::E_MOTOR_GEARSET_18);
pros::Motor intake2 (20, pros::E_MOTOR_GEARSET_18, true);
pros::Motor angleAdjuster(18, pros::E_MOTOR_GEARSET_36);


//Initialize Arms
pros::Motor arm(14, pros::E_MOTOR_GEARSET_36);


//Auton
int selectedAuton = NULL;
int myTeam = -1;
