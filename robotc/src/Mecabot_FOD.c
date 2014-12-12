#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S2,     HTGYRO,         sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     PSPNXV4,        sensorI2CCustomFastSkipStates)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     Rf,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     Rb,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     Lf,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lb,            tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/***** DEFINES *****/
//#define _FORCE_DEBUG //Uncomment to force using debug (non-optimized) mode
//#define _DISABLE_JOYDISPLAY //Uncomment to disable joystick display
#define _ENABLE_LCDDISPLAY //Uncomment to enable live NXT LCD display

/***** INCLUDES *****/
#include "../lib/naturalization.h" //naturalize RobotC
#include "../lib/logging.h" //logging

#include "../lib/gyro.h" //gyroscope
#include "../lib/drive.h" //drive trains
#include "../lib/i2c.h" //I2C error checking
#include "../lib/options.h" //splash screens
#include "../lib/ir.h" //IR reading
#include "../lib/sensor.h" //sensor IO

#include "../drivers/mindsensors-ps2ctrl-v4.h" //mindsensors stuffs

/***** STATICS *****/
static float k_deadband = 15;

/***** VARIABLES *****/
//TJoystick controller; //--declared in JoystickDriver.c, imported by drive.h--

void init()
{
    bSmartDiagnostics = true; //true to enable smart diagnostic screen
    bCompetitionMode = false; //true to enable competition mode

    displaySplash("Mecanum Bot", "FOD Test", true);
    eraseDisplay();
    gyro_init(HTGYRO);
    wait1Msec(50);
    nxtbarOn();
    return;
}

task main()
{
    float leftFront, leftBack, rightFront, rightBack; // motors
    float y, x, c;
    tPSP controller;

    /***** BEGIN Mecanum Field Oriented Drive Test *****/
    init();
    wait10Msec(100);
    StartTask(readSensors, 8);
    StartTask(displaySmartDiags, 255);
    //if (bCompetitionMode) {waitForStart();}

    while (true)
    {
				nxtDisplayCenteredTextLine(3, "%i", gyro_getheading());
        /***** Proportional Motor Control *****/
        PSPV4readButtons(PSPNXV4, controller);

			  //scale to -1 to 1
        y = (deadband(k_deadband,-controller.joystickLeft_y))/100; //strafe
        x = (deadband(k_deadband,controller.joystickLeft_x))/100; //forward/rev
        c = (deadband(k_deadband,controller.joystickRight_x))/100; //spin

        nxtDisplayTextLine(4, "%i", controller.joypadRight);
        nxtDisplayTextLine(4, "%i", controller.joypadLeft);

        if ((y == 0) && (x == 0))
        {
        	x += controller.joypadRight; //strafe
        	x -= controller.joypadLeft; //strafe
	        y += controller.joypadUp; //forward/rev
	        y -= controller.joypadDown; //forward/rev
        }

        nxtDisplayTextLine(5, "%i", y);

        mecanum_arcadeFOD(y, x, c, gyro_getheading(),
        leftFront, rightFront, leftBack, rightBack);

        motor[Lf] = leftFront*100;
        motor[Rf] = rightFront*100;
        motor[Lb] = leftBack*100;
        motor[Rb] = rightBack*100;

        if(controller.circleBtn == 1) { gyro_reset(); }
        while(nNxtButtonPressed == kEnterButton) { gyro_reset(); }
        wait1Msec(5);
    }
}
