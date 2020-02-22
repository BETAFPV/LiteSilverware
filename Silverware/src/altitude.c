/*
The MIT License (MIT)

Copyright (c) 2016 silverx

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "config.h"
#include "control.h"
#include "barometer.h"
#include "altitude.h"
#include "util.h"
#include "drv_time.h"
#include <math.h>
#include "defines.h"
#include "stdio.h"

#define AH_REFRESH_FREQ      1000.0f

#define HOVER_THROTTLE_MIN      0.1f
#define HOVER_THROTTLE_MAX      1.0f

// Define maximum target distance for full throttle in m
#define FULL_THROTTLE_ALT_TARGET  0.2f

// Define minimum low_throttle_time before motors off
#define LOW_THROTTLE_TIMEOUT    1.0f

#define KP 20.0f
#define KI 40.0f
#define KD 15.0f

#define ALT_P 0.1f
#define ALT_I 0.1f
#define ALT_D 0.06f

#define THROTTLE_SMOOTH_FACTOR 0.01 // feedback amount
//#define THROTTLE_SMOOTH_CENTER_ONLY  // active around center only

extern float looptime;
extern float press_fl;

extern float rx[4];
extern unsigned char initdone;
float altitude = 0;
float alt_target = 0;
  

#define KALMAN_Q	0.03f   //0.03
#define KALMAN_R	15.0f
float KalmanFilter(const float ResrcData)
{
	
	float R0 = KALMAN_R;
	float Q0 = KALMAN_Q;

	static	float x_last0;

	float x_mid0 = x_last0;
	float x_now0;

	static	float p_last0;

	float p_mid0 ;
	float p_now0;
	float kg0;	
	x_mid0=x_last0; 
	p_mid0=p_last0+Q0; 
	kg0=p_mid0/(p_mid0+R0);
	x_now0=x_mid0+kg0*(ResrcData-x_mid0);
	p_now0=(1-kg0)*p_mid0;

	p_last0 = p_now0;
	x_last0 = x_now0;

	return x_now0;		
}


void altitude_read(void)
{
    float alt_temp;
    read_pressure();
    alt_temp = (1.0f - pow(press_fl/101325.0f, 0.190295f)) * 44330.0f; // pressure to altitude @sea-level
    altitude = (KalmanFilter(alt_temp));
}

extern float vbattfilt;

void throttle_smooth(float *throttle) {
    // THROTTLE_SMOOTH (deprecated in acro/level)
    static float accelz_lpf;
    static float accel_integral;

    static float accel_integral_bias;
    static float accel_integral_filt;
    static float g2_filt = 0.0;

    extern float GEstG[3];
    extern float accel[3];
    extern int onground;

    if (*throttle > 0) {
        // calculate integral of z axis accel
        // some filters added to prevent runaway

        //excess acceleration in z axis
        float g2 = GEstG[2];

        // remove bias from accelerometer imperfections
        float accelz_adj = g2_filt - g2;

        // an lpf to remove more biases
        lpf( &accelz_lpf , accelz_adj , 0.99998);

        // bias calibration if on ground
        if (onground)  lpf( &g2_filt , g2 , 0.998);

        if (g2_filt < -0.10f ) g2_filt = -0.10f;
        if (g2_filt > 0.10f ) g2_filt = 0.10f;

        // remove the lpf component to make a hpf
        accelz_adj -= accelz_lpf;

        // actual integration of filtered accel
        accel_integral -= accelz_adj*looptime;

        // why not filter the integral too?
        lpf(&accel_integral_bias,accel_integral , 0.998);
        accel_integral_filt = accel_integral - accel_integral_bias;

        // a limit just in case something goes really wrong, so we still have some throttle
        limitf( &accel_integral_filt , 0.3f/(float) THROTTLE_SMOOTH_FACTOR );

        #ifdef THROTTLE_SMOOTH_CENTER_ONLY
        //100% at center - 0% at max/min
        float thr_gain = (1.0f - 2.0f*fabs(*throttle - 0.5f));
        #else
        //100% full range
        const float thr_gain = 1.0;
        #endif

        // add accel integral ( which is vertical speed) to throttle
        *throttle += (float) THROTTLE_SMOOTH_FACTOR * thr_gain * accel_integral_filt;
    }
    else
    {
        *throttle = 0;
    }
}




float altitude_hold(void)
{
    static float low_throttle_time = 0;
    static int grounded = 1;

/*
    static float last_alt_e, alt_i;     // PID loop memory
    static float new_ah_throttle;
    static float last_dt;
    float new_alt_e, alt_e, new_alt_d, alt_d, alt_corr, new_alt_corr = 0;
*/


    static float ah_throttle = HOVER_THROTTLE_MIN;
    static float last_ah_time;
//    static float new_alt_target = 0;

    float ah_time = gettime();
    float dt = (ah_time - last_ah_time) * 1e-6f; // dt in seconds

    // Silverxxx AH controller
    float out = 0;
    static float alt_lpf;
    static float last_altitude;
    static float ierror = 0;
    static float lastspeed;
    static unsigned char moveing;
    if (dt > 1.0f/AH_REFRESH_FREQ) {

        last_ah_time = ah_time;
        //last_dt = dt;

        float newrx = mapf(rx[3], 0, 1.0f, -1.0f, 1.0f);  // Zero center throttle

        if (fabs(newrx) > 0.8f)
        {
            if (newrx > 0) newrx = 0.6f;
            else newrx += 0.6f;
            
            moveing = 1;
//            new_alt_target = altitude + newrx * FULL_THROTTLE_ALT_TARGET;     // Add +/- FULL_THROTTLE_ALT_TARGET meter to altitude for full throttle travel
//            lpf(&alt_target, new_alt_target, lpfcalc(dt, 5.0f));             // Easy climbing and descending
//            alt_target = new_alt_target;
        }
        else{
            newrx = 0.02;
            moveing = 0;
        }


        // Onground
        if (grounded)
        {
            if (newrx > 0.1f)
            {
//                alt_target = altitude;
                grounded = 0;
            }
        } else
        {
            if (newrx < -0.35f)
            {
                if ((ah_time - low_throttle_time) * 1e-6f > LOW_THROTTLE_TIMEOUT) grounded = 1;

            } else
            {
                low_throttle_time = ah_time;
            }
        }

        lpf(&alt_lpf, altitude, FILTERCALC(dt, 0.02f));

        if (!grounded)
        {
/*
            // Altitude PID by MrVanes
            alt_e = alt_target - altitude;
            constrain(&alt_e, -1.0 * FULL_THROTTLE_ALT_TARGET, FULL_THROTTLE_ALT_TARGET);  // Apply FULL_THROTTLE_ALT_TARGET leash
            last_alt_e = alt_e;

            alt_i += alt_e * dt;
            constrain(&alt_i, -0.5, 0.5);

            alt_d = (alt_e - last_alt_e) / dt;

            alt_corr = ALT_P * alt_e + ALT_I * alt_i + ALT_D * alt_d;

            new_ah_throttle = ah_throttle + alt_corr;
            constrain(&new_ah_throttle, HOVER_THROTTLE_MIN, HOVER_THROTTLE_MAX);

            ah_throttle = new_ah_throttle;
*/

            // Velocity PI by Silverxxx
            float desired_speed = newrx * 0.002f;
            float speed;
            if(moveing){
                speed = 0;
            }
            else{        
               speed = (alt_lpf - last_altitude);
            }
            float error = desired_speed - speed;

            ierror += KI * dt * error;
            limitf (&ierror, 0.5);

            out = error * KP;
            out += ierror*0.95f;
            out += KD * (lastspeed - speed );
            out += 0.4f;

            lastspeed = speed;

            constrain(&out, 0, 1);
            lpf ( &ah_throttle, out, FILTERCALC(dt, 0.2f));

        } else
        {
            ah_throttle = 0;
        }

        last_altitude = alt_lpf;
    }

    return ah_throttle;
}

