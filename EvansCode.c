#pragma config(Motor,  port1,           ErightLiftMotor, tmotorVex393HighSpeed_HBridge, openLoop, driveLeft)
#pragma config(Motor,  port2,           backRight,     tmotorVex393HighSpeed_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port3,           backLeft,      tmotorVex393HighSpeed_MC29, openLoop, driveLeft)
#pragma config(Motor,  port4,           frontRight,    tmotorVex393HighSpeed_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port5,           frontLeft,     tmotorVex393HighSpeed_MC29, openLoop, driveLeft)
#pragma config(Motor,  port6,           leftLiftMotor, tmotorVex393HighSpeed_MC29, openLoop, driveLeft)
#pragma config(Motor,  port7,           rightLiftMotor, tmotorVex393HighSpeed_MC29, openLoop, driveRight)
#pragma config(Motor,  port8,           leftClaw,      tmotorNone, openLoop)
#pragma config(Motor,  port9,           rightClaw,     tmotorNone, openLoop)
#pragma config(Motor,  port10,          EleftLiftMotor, tmotorVex393HighSpeed_HBridge, openLoop, driveRight)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

/////////////////////////////////////////////////////////////
///   COMMENT THIS OUT TO DISABLE AUTONOMOUS TESTING      ///
/////////////////////////////////////////////////////////////
//#define AUTONOMOUS_TESTING

struct UserInput
{
	int forwardStick;
	int strafeStick;
	int rotateStick;
	bool raiseArm;
	bool lowerArm;
	bool openClaw;
	bool closeClaw;
};

/**
* Limits the raw input value to be in the range:
* if < -max then -max
* if >= -max and < -min then rawInput
* if >= -min and <= min then 0
* if > min and <= max then rawInput
* if > max then max
*
* @param rawInput the value you want to pass to the motor
* @param max the absolute maximum value you want to pass to the motor (both positive and negative)
* @param min the absolute minimum value you want to output before setting to zero (both positive and negative)
* @return the value that should be passed to the motor
*/
int limitValue(int rawInput, int max, int min)
{
	if (abs(rawInput) > max)
	{
		rawInput = max * (rawInput > 0 ? 1 : -1);
	}

	if (abs(rawInput) <= min)
	{
		rawInput = 0;
	}

	return rawInput;
}

/**
* Checks all of the assigned bindings once
*
* @return The current inputs of the controller
*/
void GetVexControllerInputs(struct UserInput& setMe)
{
	setMe.forwardStick = vexRT[Ch3];
	setMe.strafeStick = vexRT[Ch4];
	setMe.rotateStick = vexRT[Ch1];
	setMe.raiseArm = vexRT[Btn5U] == 1;
	setMe.lowerArm = vexRT[Btn5D] == 1;
	setMe.openClaw = vexRT[Btn6U] == 1;
	setMe.closeClaw = vexRT[Btn6D] == 1;
}

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/



void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks
	// running between Autonomous and Driver controlled modes. You will need to
	// manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	//bIfiAutonomousMode = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/**
 * Runs the drive motors according to "forward", "strafe", and "rotation"
 * values.
 *
 * @param forward The speed of the robot's forward movement (opposite the throw direction: +127, toward throw direction: -127)
 * @param strafe The speed of the robot's strafing movement (to the right of claw opening: +127, to the left: -127)
 * @param rotation The speed of the robot's rotation (clockwise: +127, counterclockwise: -127)
 */
void movement(int forward, int strafe, int rotation)
{
	int deadzoneOuter = 127;
	int deadzoneInner = 30;

	motor[frontLeft] = limitValue( forward + strafe + rotation, deadzoneOuter, deadzoneInner );
	motor[frontRight] = limitValue( forward - strafe - rotation, deadzoneOuter, deadzoneInner );
	motor[backLeft] = limitValue( forward - strafe + rotation, deadzoneOuter, deadzoneInner );
	motor[backRight] = limitValue( forward + strafe - rotation, deadzoneOuter, deadzoneInner );

}

void lift(int rise)
{
	motor[leftLiftMotor] = rise;
	motor[EleftLiftMotor] = rise * -1;
	motor[rightLiftMotor]= rise * -1;
	motor[ErightLiftMotor] = rise * -1;
}

void claw(int chomp)
{
	motor[rightClaw] = chomp;
	motor[leftClaw] = chomp * -1;
}

void throw()
{
	lift(127);
	wait1Msec(250);

	claw(-100);
	wait1Msec(1000);

	lift(-50);
	claw(0);
}

void autonomousMode()
{
	claw(60);//grabs initial object
	wait1Msec(300);
	movement(-127,0,0);//moves backwards towards wall
	wait1Msec(1900);
	movement(0,0,0);
	wait1Msec(500);
	throw();
	wait1Msec(1000);

	movement(0,127,0);//moves right to the right most wall
	wait1Msec(3400);
	movement(0,0,0);
	wait1Msec(500);

	movement(127,0,0);//moves forward to grab new object
	wait1Msec(1900);
	movement(0,0,0);
	wait1Msec(500);
	claw(60);//grabs second object
	wait1Msec(500);

	movement(-127,0,0);//moves backwards to the wall to throw object
	wait1Msec(1900);
	movement(0,0,0);
	wait1Msec(500);
	throw();//throws the second object
	wait1Msec(1000);

	movement(0,-127,0);//moves to the center of the field
	wait1Msec(2000);
	movement(0,0,0);
	wait1Msec(500);

	movement(127,0,0);//begins moving towards last object
	wait1Msec(1900);
	movement(0,0,0);
	wait1Msec(500);
	claw(60);//grabs the last object
	wait1Msec(300);

	movement(-127,0,0);//moves backwards to throw the last object
	wait1Msec(1900);
	movement(0,0,0);
	wait1Msec(500);
	throw();//throws the last object
	wait1Msec(1000);

	lift(0);//shutdown the claw and lift
	claw(0);

	wait1Msec(1000); // Wait till the end of autonomous
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
task autonomous()
{
	#ifndef AUTONOMOUS_TESTING

	// Run autonomous code if we're not testing it from user control
	autonomousMode();

	#else

	// Do nothing if we're testing from user control
	AutonomousCodePlaceholderForTesting();

	#endif
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task usercontrol()
{

	#ifdef AUTONOMOUS_TESTING

	// Only run autonomous code if we're in autonomous testing mode
	autonomousMode();

	#else

	// User control code runs if AUTONOMOUS_TESTING is not defined
	struct UserInput controllerInputs;

	while (true)
	{
		GetVexControllerInputs(controllerInputs);

		int rise = 0;
		int chomp = 0;

		if (controllerInputs.raiseArm)
		{
			rise = 127;
		}
		else if (controllerInputs.lowerArm)
		{
			rise = -40;
		}

		lift(rise);

		if (controllerInputs.openClaw)
		{
			chomp = 60;
		}
		else if (controllerInputs.closeClaw)
		{
			chomp = -30;
		}

		claw(chomp);

		movement(controllerInputs.forwardStick, controllerInputs.strafeStick, controllerInputs.rotateStick);
	}
	#endif

}
