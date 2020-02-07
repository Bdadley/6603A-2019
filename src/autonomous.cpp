#include "main.h"
#include "math.h"
#include "globals.h"

//Autononmous Variable Initialization
double DPI = 28.64788977;
double degCoef = 2.570;
bool finishedLifting = false;
int startingDegrees = 0;

//PID Coefficients
double kP = 1; //Proportional Coefficient
double kI = 0.0; //Integral Coefficient
double kD = 0.02; //Derivative Coefficient

// Effects of Increasing Coefficients
//-------------------------------------
//|Coeff|  A  |  B  |  C  |  D  |  E  |
//|------------------------------------           A = Time required to get from starting point to desired point
//|  kP |  -  |  +  |  =  |  -  |  -  |           B = Overshoot
//|------------------------------------           C = The time it takes to settle down after encountering a change
//|  kI |  -  |  +  |  +  |  -  |  -  |           D = The error at the equilibrium
//|------------------------------------           E = The "smoothness" of the speed
//|  kD |  =  |  -  |  -  |  =  |  +  |
//-------------------------------------




void move(int distance, int mySpeed=64)
{
  if(distance < 0)
  {
    mySpeed = mySpeed * -1;
  }

  finishedLifting = false;
  backLeft.tare_position();
  backRight.tare_position();
  frontLeft.tare_position();
  frontRight.tare_position();

  frontLeft.move_absolute(distance * DPI, mySpeed);
  frontRight.move_absolute(distance * DPI, mySpeed);
  backLeft.move_absolute(distance * DPI, 64);
  backRight.move_absolute(distance * DPI, mySpeed);
  while(abs(backLeft.get_position()) < distance * DPI && abs(backRight.get_position()) < distance * DPI)
  {
    pros::delay(2);
  }
  pros::delay(200);
}


//PID Controlled Turning
void turn(int degrees)
{
  finishedLifting = false;
  degrees = degrees + imu.get_rotation();

  double error = degrees - imu.get_rotation();
  double integral = 1;
  double derivative = 0;
  double prevError = 0;
  double speed = 0;

  while(true)
  {
    error = degrees - imu.get_rotation();
    integral = integral + error;

    if(error == 0) //If the error is zero, dont change the speed
    {
      integral = 0;
    }
    if(abs(error) > 30) //Prevents adding the integral value while it is in the "unusuable" zone
    {
      integral = 0;
    }

    derivative = error - prevError;
    prevError = error;

    speed = (kP * error) + (kI * integral) + (kD * derivative);

    backLeft.move_velocity(myTeam * speed);
    backRight.move_velocity(myTeam * -speed);
    frontLeft.move_velocity(myTeam * speed);
    frontRight.move_velocity(myTeam * -speed);
  }

  backLeft.move(0);
  backRight.move(0);
  frontLeft.move(0);
  frontRight.move(0);
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
  pros::delay(200);
  angleAdjuster.move(127);
  arm.move(-127);
  angleAdjuster.move(0);
  pros::delay(750);
  arm.move(127);
  pros::delay(750);
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
  finishedLifting = false; //Used for compatibility with tower-stacking functions
  intake1.move(0); intake2.move(0); //Stop the intake (in case it is moving already)
  reverseIntake(); //Lowers the cube into the robot's "hand," allowing for easier stacking
  pros::delay(250);
  stopIntake(); //Stop last command
  raiseTray(100); //Raise the tray to the vertical position in order to stack

  backLeft.move(32); backRight.move(32); frontLeft.move(32); frontRight.move(32); //Move forward in order to stabilize the stack
  pros::delay(450);

  backLeft.move(0); backRight.move(0); frontLeft.move(0); frontRight.move(0); //Stop Movement
  pros::delay(1000);

  backLeft.move(-20); backRight.move(-20); frontLeft.move(-20); frontRight.move(-20);  //Move Backwards
  pros::delay(200);

  move(-finDistance); //Move backwards finDistance inches. Used for precise control over movement
  lowerTray(82); //Lower the tray back to starting position

  backLeft.move(0); backRight.move(0); frontLeft.move(0); frontRight.move(0); //Stop robot. (In case it is still moving for whatever reason)
}

void smallTowerStack() //Probably have to fix this
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

void mediumTowerStack() //Probably have to fix this too
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


//team: -1 = Red, 1 = Blue
void autonomous() {
  if(true)
  {
    turn(-90);
  }
  else if(selectedAuton == 2) //Small Zone - 5 Point --- CHANGE TO "if", not "else if"!!!
  {
    deploy();
    startIntake();
    move(42); //Move forwards and collect line of four cubes
    pros::delay(200); //Stabilize
    stopIntake();
    maintain(); //Hold the cubes in place
    move(-18, 75); //Reverse so that you can turn and stack
    turn(-139); //Turn towards small zone
    move(16); //Move into small zone
    dispose(); //Stack all five cubes
  }
  else if(selectedAuton == 3) //Big Zone - 3 Point
  {
    deploy();
    startIntake();
    move(19);
    turn(90);
    move(21);
    turn(45);
    move(5);
    dispose();
  }
  else if(selectedAuton == 4) //Skills
  {

  }
}
