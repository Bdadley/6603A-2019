#include "main.h"
#include "math.h"
#include "globals.h"


int totalTicks = 1800;
double TPI = 840.0/(4*M_PI);
double TPD = 6.33;
//DO NOT TOUCH THESE FUNCTIONS
 void moveForwards(int distance, int mySpeed=64)
 {
   backLeft.tare_position();
   backRight.tare_position();
   frontLeft.tare_position();
   frontRight.tare_position();
   frontLeft.move_absolute(distance * TPI, mySpeed);
   frontRight.move_absolute(distance * TPI, mySpeed);
   backLeft.move_absolute(distance * TPI, 64);
   backRight.move_absolute(distance * TPI, mySpeed);
   while(backLeft.get_position() < distance * TPI && backRight.get_position() < distance * TPI)
   {
     pros::delay(2);
   }
   pros::delay(200);
 }

 void moveBackwards(int distance, int mySpeed=-64)
 {
   backLeft.tare_position();
   backRight.tare_position();
   frontLeft.tare_position();
   frontRight.tare_position();

   frontLeft.move_absolute(-distance * TPI, -mySpeed);
   frontRight.move_absolute(-distance * TPI, -mySpeed);
   backLeft.move_absolute(-distance * TPI, -mySpeed);
   backRight.move_absolute(-distance * TPI, -mySpeed);
   while(abs(backLeft.get_position()) < distance * TPI && abs(backRight.get_position()) < distance * TPI)
   {
     pros::delay(2);
   }
   pros::delay(200);
 }

void turnLeft(int degrees)
{
  backLeft.tare_position();
  backRight.tare_position();
  frontLeft.tare_position();
  frontRight.tare_position();
  frontLeft.move_absolute(-degrees * TPD * myTeam, -48);
  frontRight.move_absolute(degrees * TPD * myTeam, 48);
  backLeft.move_absolute(-degrees * TPD * myTeam, -48);
  backRight.move_absolute(degrees * TPD* myTeam, 48);
  while(abs(backLeft.get_position()) < degrees * TPD && abs(backRight.get_position()) < degrees * TPD)
  {
    pros::delay(2);
  }
  pros::delay(200);
}

void turnRight(int degrees)
{
  backLeft.tare_position();
  backRight.tare_position();
  frontLeft.tare_position();
  frontRight.tare_position();

  frontLeft.move_absolute(degrees * TPD * myTeam, 48);
  frontRight.move_absolute(-degrees * TPD * myTeam, -48);
  backLeft.move_absolute(degrees * TPD * myTeam, 48);
  backRight.move_absolute(-degrees * TPD * myTeam, -48);
  while(abs(backLeft.get_position()) < degrees * TPD)
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
   arm.move(127);
   pros::delay(800);
   arm.move(0);
 }

 void lowerArms()
 {
   arm.move(-127);
   pros::delay(800);
   arm.move(0);
 }


 void raiseTray(double percentage)
 {
   intake1.move(0);
   intake2.move(0);
   angleAdjuster.tare_position();
   angleAdjuster.move(-105);
   while(angleAdjuster.get_position() > -2350 * (percentage/100.))
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
   intake1.move(0);
   intake2.move(0);
   angleAdjuster.move(95);
   pros::delay(1480 * (percentage/100.));
   angleAdjuster.move(0);
   pros::delay(200);
 }

 void deploy()
 {
   intake1.move(0);
   intake2.move(0);
   angleAdjuster.move(-30);
   pros::delay(400);
   angleAdjuster.move(127);
   arm.move(-127);
   angleAdjuster.move(0);
   pros::delay(1000);
   arm.move(0);
   pros::delay(100);
   arm.move(127);
   pros::delay(800);
   arm.move(0);
   pros::delay(200);
 }

void maintain()
{
  intake1.move(30);
  intake2.move(30);
}

void dispose()
{
  intake1.move(0);
  intake2.move(0);
  reverseIntake();
  pros::delay(100);
  stopIntake();
  raiseTray(100);
  moveForwards(2, 20);
  pros::delay(1000);
  backLeft.move(-20);
  frontRight.move(-20);
  frontLeft.move(-20);
  backRight.move(-20);
  pros::delay(200);
  lowerTray(20);
  backLeft.move(-30);
  frontRight.move(-30);
  frontLeft.move(-30);
  backRight.move(-30);
  pros::delay(1000);
  lowerTray(60);
  backLeft.move(0);
  frontRight.move(0);
  frontLeft.move(0);
  backRight.move(0);
}


//team: 1 = red, -1 = blue

void autonomous() {
  //deploy();
  if(true)//selectedAuton == 1)
  {
    startIntake();
    moveForwards(44);
    pros::delay(1000);
    turnRight(20);
    moveForwards(5);
    pros::delay(500);
    moveBackwards(5);
    stopIntake();
    maintain();
    turnLeft(20);
    pros::delay(200);
    moveBackwards(28, 75);
    turnLeft(133);
    moveForwards(10);
    dispose();
  }
  else if(selectedAuton == 2)
  {
    //ALL AUTON CODE GOES HERE

  }
  else if(selectedAuton == 3)
  {
    startIntake();
    moveForwards(45);
    pros::delay(500);
    stopIntake();
    turnRight(150);
    moveForwards(37);
    turnLeft(149);
    startIntake();
    moveForwards(40);
    pros::delay(500);
    stopIntake();
    maintain();
    moveBackwards(10);
    turnLeft(135);
    moveForwards(42);
    dispose();
  }
  else
  {
    startIntake(); //Intake pre-load
    moveForwards(45); //Pick up second row
    pros::delay(500);
    stopIntake();
    turnRight(33); //Point torwards first row
    moveBackwards(41, 127); //Move to first row
    turnLeft(48);
    startIntake();
    moveForwards(40); //Pick up first row
    pros::delay(500);
    maintain();
    moveBackwards(29, 127); //Return to original x position
    turnLeft(148);
    moveForwards(16); //Move to small zone
    dispose();
  }
}
