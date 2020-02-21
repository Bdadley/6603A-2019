#include "main.h"
#include "math.h"
#include "globals.h"

//Autononmous Variable Initialization
double DPI = 28.64788977; //Degrees that the wheels need to turn to move the robot forwards one inch
bool finishedLifting = false; //Set to true whenever the arms have been lifted in order to prevent anything else from moving
int startingDegrees = 0; //Rotation that the robot starts at whenever it makes a turn


//PID Coefficients
double kP = 0.7; //Proportional Coefficient
double kI = 0.000002; //Integral Coefficient
double kD = 50; //Derivative Coefficient

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



//Linear movement
void move(int distance, int mySpeed=64) //+ = forwards, - = backwards.
{
  //add slow move function, so the jounce is not as high
  int tempSpeed = 0; //NEW

  finishedLifting = false;
  if(distance < 0) //If moving backwards, set speed to reverse
  { mySpeed = mySpeed * -1; }

  backLeft.tare_position(); backRight.tare_position(); frontLeft.tare_position(); frontRight.tare_position(); //Reset motor encoders
  if(distance > 0)
  {
    distance -= backLeft.get_position()/DPI;
    while(abs(mySpeed - backLeft.get_actual_velocity()) > 5) //NEW -- SHOULD THEORETICALLY ACCELERATE THE BOT
    {
      backLeft.move(tempSpeed);
      backRight.move(tempSpeed);
      frontLeft.move(tempSpeed);
      frontRight.move(tempSpeed);
      tempSpeed+=4;
      pros::delay(50);
    }
  }

  if((abs(mySpeed - backLeft.get_actual_velocity()) <= 5)) //NEW
  {
    frontLeft.move_absolute(distance * DPI, mySpeed); backLeft.move_absolute(distance * DPI, mySpeed); //Move left side the desired distance at the desired speed
    frontRight.move_absolute(distance * DPI, mySpeed); backRight.move_absolute(distance * DPI, mySpeed); //Move right side the desired distance at the desired speed
    pros::delay(500);
    while(abs(backLeft.get_actual_velocity()) > 2 && abs(backRight.get_actual_velocity()) > 2 && abs(backLeft.get_position()) < abs(distance) * DPI && abs(backRight.get_position()) < abs(distance) * DPI) //While the robot has not reached its destination, keep moving
    { pros::delay(2); }

    frontLeft.move(0); frontRight.move(0); backLeft.move(0); backRight.move(0); //Stop all motors
    pros::delay(200);
  }
}

//PID Controlled Turning
void turn(int degrees)
{
  finishedLifting = false;

  degrees = myTeam * (degrees + imu.get_rotation());

  double error = degrees - imu.get_rotation();
  double integral = 1;
  double derivative = 0;
  double prevError = 0;
  double speed = 0;
  bool pidRunning = true;
  double countdown = 2222.222222 * abs(degrees);

  //Control loop
  while(pidRunning)
  {
    //pros::lcd::set_text(3, std::to_string(error));
    //pros::lcd::set_text(5, std::to_string(integral));
    //pros::lcd::set_text(6, std::to_string(derivative));
    //pros::lcd::set_text(7, std::to_string(countdown));

    error = degrees - imu.get_rotation(); //Calculate the error (P)
    integral = integral + error; //Calculate the integral

    //Anti-windup
    if(error == 0) //If the error is zero, dont change the speed
    { integral = 0; }
    if(abs(error) > 8) //Prevents adding the integral value while it is in the "unusuable" zone
    { integral = 0; }

    derivative = error - prevError; prevError = error; //Calculate the deriative

    speed = (kP * error) + (kI * integral) + (kD * derivative); //Combine the P + I + D into one movement variable

    //Move
    backLeft.move_velocity(speed);
    frontLeft.move_velocity(speed);
    backRight.move_velocity(-speed);
    frontRight.move_velocity(-speed);
    if(error < 2)
    {
      countdown--;
      if(countdown < 0)
      {
        pidRunning = false;
      }
    }
    else
    {
      countdown = 2222.222222 * abs(degrees);
    }
  }

  frontLeft.move(0); frontRight.move(0); backLeft.move(0); backRight.move(0); //Stop the robot from turning any further
  pros::delay(200);
}

//Imprecise, but faster, turn
void impTurn(int time, int direction)
{
  if(direction == 1 || direction == -1)
  {
    finishedLifting = false;
    backLeft.move(32* direction);
    frontLeft.move(32 * direction);
    backRight.move(32 * -direction);
    frontRight.move(32 * -direction);
    pros::delay(time);
    backLeft.move(0);
    frontLeft.move(0);
    backRight.move(0);
    frontRight.move(0);
  }


}
//Intake Cubes
void startIntake()
{
  intake1.move(127);
  intake2.move(127);
}

//Outtake cubes -- slower than intaking for precision purposes
void reverseIntake(int time)
{
  intake1.move(-76);
  intake2.move(-76);
  pros::delay(time);
 }

//Stop the intake rollers
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

//Raise the tray up percentage% (0% is no change, 100% is perpendicular to the ground)
void raiseTray(double percentage)
{
  finishedLifting = false;
  intake1.move(0); intake2.move(0); //Stop the intake so as not to break the robot
  angleAdjuster.tare_position(); //Reset motor encoder

  angleAdjuster.move(-90);
  while(angleAdjuster.get_position() > -4400 * (percentage/100.)) //Equation allows the user to lift the angle to a percentage of 2300 ticks (the upright value)
  {
    pros::delay(2);

    if(angleAdjuster.get_position() < -3225) //Prevents the tray from moving too fast when it reaches near the top of its rotation
    { angleAdjuster.move(-40); }
    else
    { angleAdjuster.move(-90); }
  }
  angleAdjuster.move(0); //Stop the tray from moving

  pros::delay(200);
}

//Raise the tray up percentage% (0% is no change, 100% is back in resting position)
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

//Deploy the tray
void deploy() //MAYBE FIX THIS??? IT SHOULDNT REALLY WORK
{
  finishedLifting = false;

  intake1.move(0); intake2.move(0);

  angleAdjuster.move(-30); //Move the tray forwards a small amount
  pros::delay(400);

  angleAdjuster.move(127); //WHY IS THIS HERE?
  arm.move(-127);
  angleAdjuster.move(0);
  pros::delay(750);

  arm.move(127);
  pros::delay(750);

  arm.move(0);
  pros::delay(200);
}

//Hold the cubes so that they dont fall out of the tray onto the ground
void maintain()
{
  finishedLifting = false;
  intake1.move(30); intake2.move(30);
}


//Stack whatever cubes are in the tray and move backwards finDistance inches after doing so
void dispose(int finDistance=15)
{
  finishedLifting = false; //Used for compatibility with tower-stacking functions
  intake1.move(0); intake2.move(0); //Stop the intake (in case it is moving already)
  reverseIntake(275); //Lowers the cube into the robot's "hand," allowing for easier stacking
  stopIntake(); //Stop last command
  raiseTray(100); //Raise the tray to the vertical position in order to stack

  backLeft.move(31); backRight.move(32); frontLeft.move(32); frontRight.move(32); //Move forward in order to stabilize the stack
  pros::delay(450);

  backLeft.move(0); backRight.move(0); frontLeft.move(0); frontRight.move(0); //Stop Movement
  pros::delay(1000);

  backLeft.move(-19); backRight.move(-20); frontLeft.move(-20); frontRight.move(-20);  //Move Backwards
  pros::delay(200);

  move(-finDistance); //Move backwards finDistance inches. Used for precise control over movement
  lowerTray(120); //Lower the tray back to starting position

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
        reverseIntake(250);
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
  stopIntake();
  bool liftingArm = true;
  while(liftingArm == true)
  {
    liftingArm = true;
    if(angleAdjuster.get_position() > -800)
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
          angleAdjuster.move(0);
          arm.move(0);
          liftingArm = false;
          reverseIntake(1000);
        }
        else
        {
          angleAdjuster.move(0);
          arm.move(0);
          liftingArm = false;
        }
      }
    }
  }
}

//Drop a cube from the tray into the intake
void putCubeInHand()
{
  finishedLifting = false;
  reverseIntake(250);
  pros::delay(200);
  stopIntake();
}


//team: -1 = Red, 1 = Blue
void autonomous()
{
  if(selectedAuton == 2) //Small Zone - 5 Point --- CHANGE TO "if", not "else if"!!!
  {
    deploy();
    startIntake();
    move(40, 48); //Move forwards and collect line of four cubes
    stopIntake();
    maintain(); //Hold the cubes in place
    move(-23, 48); //Reverse so that you can turn and stack
    turn(-135); //Turn towards small zone
    move(14, 32); //Move into small zone
    dispose(); //Stack all five cubes
  }
  else if(selectedAuton == 3) //Big Zone - 3 Point
  {
    deploy();
    startIntake();
    move(19); //Move forwards and grab the first cube
    turn(90); //Turn towards the second cube
    move(21); //Move forwards and grab the second cube
    turn(45); //Turn towards the big zone
    move(5); //Move into the big zone
    dispose(); //Stack the three stack
  }
  else if(selectedAuton == 4) //Skills
  {
    startIntake();
    move(112, 48);
    turn(90);
    pros::delay(500);
    stopIntake();
    move(26);
    pros::delay(500);
    impTurn(1000,-1);
    move(14);
    stopIntake();
    dispose();
    move(-5);
    turn(-145);
    startIntake();
    move(40);
    stopIntake();
    reverseIntake(250);
    move(-10);
    mediumTowerStack();

  }
  else
  {

  }
}
