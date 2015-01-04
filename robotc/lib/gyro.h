/**********************************************************

Gyroscopic Drive
gyro.h

Implements the HiTechnic Gyroscope.

**********************************************************/
#ifndef _GYRO_H
#define _GYRO_H

#include "../drivers/hitechnic-gyro.h" //gyroscope
#include "../lib/logging.h" //logging

/***** LOCAL VARIABLES *****/
float vel_curr = 0.0;
float vel_prev = 0.0;
float dt = 0.0;
static float heading = 0;
static tSensors gyro = 0;
static bool go = true;
static int gyro_logid = -1;
string gyro_filename = "gyrolog.txt";
/***** QUICK METHODS *****/

//Get current rotational speed (in deg/s)
float gyro_getrotspeed() { return vel_curr; }
//Get current heading
float gyro_getheading() { return (heading % 360); }
//Reset gyro heading to zero
void gyro_reset() { heading = 0; }
//Stop gyro calibration task
void gyro_kill() { go = false; }

/***** METHODS *****/
/**
* Gyro logging setup
*/
void gyro_setupLogging()
{

	log_init(gyro_filename, false, gyro_logid);
	log_write("Gyro Log", gyro_logid);
}
/**
* Gyro initialization function
* @param The gyro sensor.
*/
float gyro_init_internal(tSensors link)
{
    //Read gyro offset
    gyro = link;
    if (log_enabled) {
        gyro_setupLogging();
    }
    return HTGYROstartCal(link);
}
/**
* Gyro calibration task
* Call before your while(true) loop with priority 8
*/
void updateGyro()
{
    vel_prev = vel_curr;
    dt = (float)time1[T1]/(float)1000.0;
    time1[T1]=0;
    // Read the current rotation speed
    vel_curr = (float)HTGYROreadRot(gyro);
    // Average of last two velocities multiplied by the time passed = change in heading
    heading += ((float)vel_prev+(float)vel_curr)*(float)0.5*(float)dt;

    if(log_enabled){
	    string s;
	    StringFormat(s, "GYRO : %.4f, %.4f, %.4f", vel_prev, vel_curr, dt);
	    log_write(s, gyro_logid);
    }
}
#endif
