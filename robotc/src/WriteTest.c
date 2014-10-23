#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S2,     HTGYRO,         sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     IR,             sensorI2CCustom)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     Rf,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     Rb,            tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     Lf,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lb,            tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/***** DEFINES *****/
#define _FORCE_DEBUG //Uncomment to force using debug (non-optimized) mode
//#define _DISABLE_JOYDISPLAY //Uncomment to disable joystick display
#define _ENABLE_LCDDISPLAY //Uncomment to enable live NXT LCD display

/***** INCLUDES *****/
#include "../lib/naturalization.h" //naturalize RobotC
#include "../lib/drive.h" //drive trains
#include "../lib/gyro.h" //gyroscope and FOD
#include "../lib/i2c.h" //I2C error checking
#include "../lib/display.h" //splash screens
#include "../drivers/hitechnic-irseeker-v2.h" //IR diver
/***** STATICS *****/
static float k_deadband = 15;

/***** VARIABLES *****/
//TJoystick controller; //--declared in JoystickDriver.c, imported by drive.h--
float rotatlast = 0.0; //last rotation values where
bool contsearch = true;
task getIR()
{
	rotatlast = 0.0;
	while (contsearch) {
		int irnum = HTIRS2readDCDir(IR);
		nxtDisplayString(3, "%i", irnum);
		if (irnum != 5)
		{
			rotatlast = gyro_getheading();
			return;
		}
	}
}
void writeIRToFile(float first,float second, TFileHandle hFileHandle,TFileIOResult nIOResult){
	string s = "";
	StringFormat(s,"%f,%f\r\n",first,second);
	WriteText(hFileHandle, nIOResult, s);         // write 's' to the file
	wait1Msec(10);
}
void init()
{
    bSmartDiagnostics = false; //true to enable smart diagnostic screen
    bCompetitionMode = false; //true to enable competition mode

    displaySplash("NoStep", "Auto IR Test", true);

    bool ok = true;
    while(!ok)
    {
        const int testcount = 2;
	    bool test[testcount] = {
	        errorcheck(1,0,1,MOTORCON),
	        errorcheck(1,0,2,MOTORCON)};
	    string desc[testcount] = {"MC1-1","MC1-2"};
	    ok = error_display(test,desc,testcount);
	    if (!ok) {
	        PlayTone(440, 50);
	        if (test[0] == false && test[1] == false){
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
  	TFileHandle   hFileHandle;              // will keep track of our file
  	TFileIOResult nIOResult;                // will store our IO results
  	string        sFileName = "test.txt";   // the name of our file
  	int           nFileSize = 3000;          // will store our file size
  	byte CR = 0x13;   // define CR (carriage return)
  	byte LF = 0x10;   // define LF (line feed)
    /***** BEGIN Mecanum Field Oriented Drive Test *****/
    init();
    StartTask(gyro_calibrate, 8);
//    StartTask(displaySmartDiags, 255);
		//Write to file
    OpenWrite(hFileHandle, nIOResult, sFileName, nFileSize);    // open the file for writing (creates the file if it does not exist)
    WriteText(hFileHandle, nIOResult, "Time,IR\r\n");
		float array[50][3] ;
    for (int i = 0; i < 50; i++){
    		array[i][0] = i;
    		array[i][1] = i*2;
   	}
   	for (int j = 0; j < 50; j++){
   		writeIRToFile(array[j][0],array[j][1],hFileHandle,nIoResult);
  	}
  	Close(hFileHandle, nIOResult);
}
