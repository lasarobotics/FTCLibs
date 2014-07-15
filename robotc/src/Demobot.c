#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Sensor, S3,     HTGYRO,         sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     PSPNXV4,        sensorI2CCustomFastSkipStates)
#pragma config(Motor,  mtr_S1_C1_1,     drive_right,   tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     drive_left,    tmotorTetrix, openLoop, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/***** DEFINES *****/
#define _FORCE_DEBUG //Uncomment to force using debug (non-optimized) mode
#define _DISABLE_JOYDISPLAY //Uncomment to disable joystick display
#define _ENABLE_LCDDISPLAY //Uncomment to enable live NXT LCD display

/***** INCLUDES *****/
#include "../lib/naturalization.h" //naturalize RobotC
#include "../lib/drive.h" //drive trains
#include "../lib/gyro.h" //gyroscope and FOD
#include "../lib/i2c.h" //I2C error checking
#include "../lib/display.h" //splash screens

#include "../drivers/mindsensors-ps2ctrl-v4.h" //mindsensors stuffs

/***** STATICS *****/
static float k_deadband = 15;

void init()
{
    bSmartDiagnostics = true; //true to enable smart diagnostic screen
    bCompetitionMode = false; //true to enable competition mode

    displaySplash("Demo Robot", "", true);

    bool ok = false;
    while(!ok)
    {
        const int testcount = 2;
	    bool test[testcount] = {
	        errorcheck(1,1,MOTORCON),
	        errorcheck(4,1,PSPV4)};
	    string desc[testcount] = {"MC1","PSPV4"};
	    ok = error_display(test,desc,testcount);
	    if (!ok) {
	        PlayTone(440, 50);
	        if (test[0] == false && test[1] == true){
	            nxtDisplayCenteredTextLine(7, "Reboot MC!");
	        }
	    }
	    else { ClearSounds(); }
    }

    eraseDisplay();
    gyro_init(HTGYRO);
    wait1Msec(50);
    nxtbarOn();
    return;
}

task main()
{
    init();

    // This is the struct that holds all the info on all buttons and joypads/sticks
    tPSP controller;
    float left, right;

    StartTask(gyro_calibrate, 8);
    StartTask(displaySmartDiags, 255);
    if (bCompetitionMode) {waitForStart();}

    while (true)
    {
        // Read the state of the buttons
        PSPV4readButtons(PSPNXV4, controller);

        //y-axis is inverted
        drive_tank(deadband(k_deadband,-controller.joystickLeft_y), deadband(k_deadband,-controller.joystickRight_y), left, right);
        motor[drive_left] = left;
        motor[drive_right] = right;

        nxtDisplayCenteredBigTextLine(6, "%.2f", gyro_getheading());
        nxtDisplayCenteredTextLine(5, "%.2f", gyro_getrotspeed());
        nxtDisplayCenteredTextLine(4, "%.2f", gyro_getfirstoffset());

        while(nNxtButtonPressed == kEnterButton)
        {
            //Reset heading to zero
            gyro_reset();
        }

    }
}
