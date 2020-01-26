#include "main.h"
#include "math.h"
#include "globals.h"


int totalTicks = 1800;
double DPI = 28.64788977;
double degCoef = 2.570;
bool finishedLifting = false;
//DO NOT TOUCH THESE FUNCTIONS
 void moveForwards(int distance, int mySpeed=64)
 {
   finishedLifting = false;
   backLeft.tare_position();
   backRight.tare_position();
   frontLeft.tare_position();
   frontRight.tare_position();
   frontLeft.move_absolute(distance * DPI, mySpeed);
   frontRight.move_absolute(distance * DPI, mySpeed);
   backLeft.move_absolute(distance * DPI, 64);
   backRight.move_absolute(distance * DPI, mySpeed);
   while(backLeft.get_position() < distance * DPI && backRight.get_position() < distance * DPI)
   {
     pros::delay(2);
   }
   pros::delay(200);
 }

 void moveBackwards(int distance, int mySpeed=-64)
 {
   finishedLifting = false;
   backLeft.tare_position();
   backRight.tare_position();
   frontLeft.tare_position();
   frontRight.tare_position();

   frontLeft.move_absolute(-distance * DPI, -mySpeed);
   frontRight.move_absolute(-distance * DPI, -mySpeed);
   backLeft.move_absolute(-distance * DPI, -mySpeed);
   backRight.move_absolute(-distance * DPI, -mySpeed);
   while(abs(backLeft.get_position()) < distance * DPI && abs(backRight.get_position()) < distance * DPI)
   {
     pros::delay(2);
   }
   pros::delay(200);
 }

void turnLeft(int degrees)
{
  finishedLifting = false;
  backLeft.tare_position();
  backRight.tare_position();
  frontLeft.tare_position();
  frontRight.tare_position();
  frontLeft.move_absolute(-degrees * degCoef * myTeam, -48);
  frontRight.move_absolute(degrees * degCoef * myTeam, 48);
  backLeft.move_absolute(-degrees * degCoef * myTeam, -48);
  backRight.move_absolute(degrees * degCoef * myTeam, 48);
  while(abs(backLeft.get_position()) < degrees * degCoef && abs(backRight.get_position()) < degrees * degCoef)
  {
    pros::delay(2);
  }
  pros::delay(200);
}

void turnRight(int degrees)
{
  finishedLifting = false;
  backLeft.tare_position();
  backRight.tare_position();
  frontLeft.tare_position();
  frontRight.tare_position();

  frontLeft.move_absolute(degrees * degCoef * myTeam, 48);
  frontRight.move_absolute(-degrees * degCoef * myTeam, -48);
  backLeft.move_absolute(degrees * degCoef * myTeam, 48);
  backRight.move_absolute(-degrees * degCoef * myTeam, -48);
  while(abs(backLeft.get_position()) < degrees * degCoef && abs(backRight.get_position()) < degrees * degCoef)
  {
    pros::delay(2);
  }
  pros::delay(200);
}
 void startIntake()
 {
   intake1.move(127);
   intake2.move(127);
 }

 void reverseIntake()
 {
   intake1.move(-76);
   intake2.move(-76);
 }

 void stopIntake()
 {
   intake1.move(0);
   intake2.move(0);
 }

 void raiseArms()
 {
   finishedLifting = false;
   arm.move(127);
   pros::delay(800);
   arm.move(0);
 }

 void lowerArms()
 {
   finishedLifting = false;
   arm.move(-127);
   pros::delay(800);
   arm.move(0);
 }


 void raiseTray(double percentage)
 {
   finishedLifting = false;
   intake1.move(0);
   intake2.move(0);
   angleAdjuster.tare_position();
   angleAdjuster.move(-105);
   while(angleAdjuster.get_position() > -2300 * (percentage/100.))
   {
     pros::delay(2);
     if(angleAdjuster.get_position() < -1650)
     {
       angleAdjuster.move(-65);
     }
   }
   angleAdjuster.move(0);
   pros::delay(200);
 }

 void lowerTray(double percentage)
 {
   finishedLifting = false;
   intake1.move(0);
   intake2.move(0);
   angleAdjuster.move(95);
   pros::delay(1480 * (percentage/100.));
   angleAdjuster.move(0);
   pros::delay(200);
 }

 void deploy()
 {
   finishedLifting = false;
   intake1.move(0);
   intake2.move(0);
   angleAdjuster.move(-30);
   pros::delay(300);
   angleAdjuster.move(127);
   arm.move(-127);
   angleAdjuster.move(0);
   pros::delay(600);
   arm.move(127);
   pros::delay(600);
   arm.move(0);
   pros::delay(200);
 }

void maintain()
{
  finishedLifting = false;
  intake1.move(30);
  intake2.move(30);
}

void dispose(int finDistance = 15)
{
  finishedLifting = false;
  intake1.move(0);
  intake2.move(0);
  reverseIntake();
  pros::delay(250);
  stopIntake();
  raiseTray(100);
  moveForwards(1, 127);
  pros::delay(1000);
  backLeft.move(-20);
  frontRight.move(-20);
  frontLeft.move(-20);
  backRight.move(-20);
  pros::delay(200);
  lowerTray(20);
  //backLeft.move(-30);
  //frontRight.move(-30);
  //frontLeft.move(-30);
  //backRight.move(-30);
  moveBackwards(finDistance + 1);
  pros::delay(1000);
  lowerTray(60);
  backLeft.move(0);
  frontRight.move(0);
  frontLeft.move(0);
  backRight.move(0);
}

void smallTowerStack()
{
  while(finishedLifting == false)
  {
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
        reverseIntake();
        pros::delay(1000);
        stopIntake();
        arm.move(127);
        pros::delay(500);
        if(angleAdjuster.get_position() < 0)
        {
          angleAdjuster.move(95);
        }
        else
        {
          angleAdjuster.move(0);
          finishedLifting = true;
        }
      }
    }
  }
}

void mediumTowerStack()
{
  while(finishedLifting == false)
  {
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
        reverseIntake();
        pros::delay(1000);
        stopIntake();
        arm.move(127);
        pros::delay(500);
        if(angleAdjuster.get_position() < 0)
        {
          angleAdjuster.move(95);
        }
        else
        {
          angleAdjuster.move(0);
          finishedLifting = true;
        }
      }
    }
  }
}

void putCubeInHand()
{
  finishedLifting = false;
  reverseIntake();
  pros::delay(200);
  stopIntake();
}

//team: 1 = red, -1 = blue

void autonomous() {

  if(selectedAuton == 1) //Back Auton
  {
    deploy();
    startIntake();
    //moveForwards(44);
    moveForwards(42); //Move forwards and collect line of four cubes
    pros::delay(200); //Stabilize
    //turnRight(20);
    //moveForwards(5);
    //pros::delay(500);
    //moveBackwards(10);
    //moveBackwards(5); //-
    stopIntake();
    maintain(); //Hold the cubes in place
    //turnLeft(20);
    //pros::delay(200);
    moveBackwards(18, 75); //Reverse so that you can turn and stack
    turnLeft(135); //Turn towards small zone
    moveForwards(16); //Move into small zone
    dispose(); //Stack all five cubes
  }
  else if(selectedAuton == 2) //Front Auton
  {
    //Put Cube on back of robot.
    moveBackwards(12); //Move cube into zone
    pros::delay(200);
    moveForwards(12); //Move out of zone
    deploy();
  }
  else if(selectedAuton == 3) //Skills
  {
    deploy();
    startIntake();
    moveForwards(42); //Collect line of four cubes
    pros::delay(200);
    stopIntake();
    maintain();
    moveBackwards(18, 75); //Move backwards so you can turn and stack the cubes
    turnLeft(135); //Turn towards small zone
    moveForwards(16); //Move into small zone
    dispose(16); //Stack five cubes and move back XX inches in order to align robot for medium tower
    turnLeft(135); //Turn towards Medium tower
    moveForwards(35); //Move towards Medium tower
    startIntake();
    moveForwards(9); //Collect cube next to Medium Tower
    pros::delay(500);
    stopIntake();
    moveBackwards(4); //Align robot for easier tower stacking
    putCubeInHand(); //Move cube from tray to rollers
    mediumTowerStack(); //Stack cube in Medium tower
    moveBackwards(40); //Move backwards and align for phase 3
    turnLeft(90); //Turn towards the line of 4 cubes on other half of the field
    startIntake();
    moveForwards(96); //Collect line of 4 cubes on the other half of the field
    stopIntake();
    maintain();
    turnLeft(45); //Turn towards the small zone
    moveForwards(16); //Move into the small zone
    dispose(); //Stack the stack of 4-5 cubes.

  }
  else
  {

  }
}
