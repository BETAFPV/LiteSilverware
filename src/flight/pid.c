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

#include <stdbool.h>
#include <stdlib.h>
#include "pid.h"
#include "util.h"
#include "led.h"
#include "defines.h"
#include "math.h"
#include "config.h"


//**************************ADVANCED PID CONTROLLER - WITH PROFILE SWITCHING ON AUX SWITCH PIDPROFILE*******************************
// GENERAL SUMMARY OF THIS FEATURE:
// stickAccelerator and stickTransition are a more detailed version of the traditional D term setpoint weight and transition variables that you may be familiar with in other firmwares.
// The difference here is that we name the D term setpoint weight "Stick Accelerator" because it's actual function is to accelerate the response of the pid controller to stick inputs.
// Another difference is that negative stick transitions are possible meaning that you can have a higher stick acceleration near center stick which fades to a lower stick acceleration at
// full stick throws should you desire to see what that feels like.  Traditionally we are only used to being able to transition from a low setpoint to a higher one.
// The final differences are that you can adjust each axis independently and also set up two seperate profiles so that you can switch "feels" in flight with the PIDPROFILE aux
// channel selection set up in the receiver section of config.h
//
//HOW TO USE THIS FEATURE:
// Safe values for stickAccelerator are from 0 to about 2.5 where 0 represents a "MEASUREMENT" based D term calculation and is the traditional Silverware PID controller, and a
// a value of 1 represents an "ERROR" based D term calculation.  Values above 1 add even more acceleration but be reasonable and keep this below about 2.5.

// Range of acceptable values for stickTransition are from -1 to 1.  Do not input a value outside of this range.  When stick transition is 0 - no stick transition will take place
// and stick acceleration will remain constant regardless of stick position.  Positive values up to 1 will represent a transition where stick acceleration at it's maximum at full
// stick deflection and is reduced by whatever percentage you enter here at stick center.  For example accelerator at 1 and transition at .3 means that there will be 30% reduction
// of acceleration at stick center, and acceleration strength of 1 at full stick.


//pid profile A                      Roll  PITCH  YAW
float stickAcceleratorProfileA[3] = { 0.0, 0.0, 0.0};             //keep values between 0 and 2.5
float stickTransitionProfileA[3]  = { 0.0, 0.0, 0.0};             //keep values between -1 and 1

//pid profile B                      Roll  PITCH  YAW
float stickAcceleratorProfileB[3] = { 1.5, 1.5, 1.0};             //keep values between 0 and 2.5
float stickTransitionProfileB[3]  = { 0.3, 0.3, 0.0};             //keep values between -1 and 1



//************************************PIDS****************************************

float pidkp[PIDNUMBER] = {0.1438, 0.1438, 0.2436 };
float pidki[PIDNUMBER] = { 1.4, 1.4, 1.4    };
float pidkd[PIDNUMBER] = { 0.5666, 0.5666, 0.05   };

//*********************************Saved Initial PIDs****************************************


//************************************Setpoint Weight & Limits********************************
#ifdef BRUSHLESS_TARGET

/// output limit
const float outlimit[PIDNUMBER] = { 0.8, 0.8, 0.4 };

// limit of integral term (abs)
const float integrallimit[PIDNUMBER] = { 0.8, 0.8, 0.4 };

#else  //BRUSHED TARGET

// "p term setpoint weighting" 0.0 - 1.0 where 1.0 = normal pid
#define ENABLE_SETPOINT_WEIGHTING
//            Roll   Pitch   Yaw
//float b[3] = { 0.97 , 0.98 , 0.95};   //BRUSHED RACE
float b[3] = { 0.93, 0.93, 0.9};        //BRUSHED FREESTYLE

/// output limit
const float outlimit[PIDNUMBER] = { 1.7, 1.7, 0.5 };

// limit of integral term (abs)
const float integrallimit[PIDNUMBER] = { 1.7, 1.7, 0.5 };

#endif



//#define RECTANGULAR_RULE_INTEGRAL
//#define MIDPOINT_RULE_INTEGRAL
#define SIMPSON_RULE_INTEGRAL

//#define ANTI_WINDUP_DISABLE

// non changable things below
float *pids_array[3] = {pidkp, pidki, pidkd};
int number_of_increments[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
int current_pid_axis = 0;
int current_pid_term = 0;
float *current_pid_term_pointer = pidkp;

float ierror[PIDNUMBER] = { 0, 0, 0};
float pidoutput[PIDNUMBER];
float setpoint[PIDNUMBER];
static float lasterror[PIDNUMBER];
float v_compensation = 1.00;

#ifdef ANALOG_AUX_PIDS
int analog_aux_pids_adjusted = 0;
#endif

extern float error[PIDNUMBER];
extern float setpoint[PIDNUMBER];
extern float looptime;
extern float gyro[3];
extern int onground;
extern float looptime;
extern int in_air;
extern char aux[AUXNUMBER];
extern float aux_analog[AUXNUMBER];
extern char aux_analogchange[AUXNUMBER];
extern float vbattfilt;

// multiplier for pids at 3V - for PID_VOLTAGE_COMPENSATION - default 1.33f from H101 code
#define PID_VC_FACTOR 1.33f

#ifdef SIMPSON_RULE_INTEGRAL
static float lasterror2[PIDNUMBER];
#endif

float timefactor;


float pid(int x)
{
    if ((aux[LEVELMODE]) && (!aux[RACEMODE]))
    {
        if ((onground) || (in_air == 0))
        {
            ierror[x] *= 0.98f;
        }
    }
    else
    {
        if (onground) ierror[x] *= 0.98f;
    }


#ifdef TRANSIENT_WINDUP_PROTECTION
    static float avgSetpoint[3];
    static int count[3];
    extern float splpf(float in, int num);

    if (x < 2 && (count[x]++ % 2) == 0)
    {
        avgSetpoint[x] = splpf(setpoint[x], x);
    }
#endif

    int iwindup = 0;
    if ((pidoutput[x] == outlimit[x]) && (error[x] > 0))
    {
        iwindup = 1;
    }

    if ((pidoutput[x] == -outlimit[x]) && (error[x] < 0))
    {
        iwindup = 1;
    }

#ifdef ANTI_WINDUP_DISABLE
    iwindup = 0;
#endif

#ifdef TRANSIENT_WINDUP_PROTECTION
    if (x < 2 && fabsf(setpoint[x] - avgSetpoint[x]) > 0.1f)
    {
        iwindup = 1;
    }
#endif

    if (!iwindup)
    {
#ifdef MIDPOINT_RULE_INTEGRAL
        // trapezoidal rule instead of rectangular
        ierror[x] = ierror[x] + (error[x] + lasterror[x]) * 0.5f *  pidki[x] * looptime;
        lasterror[x] = error[x];
#endif

#ifdef RECTANGULAR_RULE_INTEGRAL
        ierror[x] = ierror[x] + error[x] *  pidki[x] * looptime;
        lasterror[x] = error[x];
#endif

#ifdef SIMPSON_RULE_INTEGRAL
        // assuming similar time intervals
        ierror[x] = ierror[x] + 0.166666f * (lasterror2[x] + 4 * lasterror[x] + error[x]) *  pidki[x] * looptime;
        lasterror2[x] = lasterror[x];
        lasterror[x] = error[x];
#endif
    }

    limitf(&ierror[x], integrallimit[x]);


#ifdef ENABLE_SETPOINT_WEIGHTING
    // P term
    pidoutput[x] = error[x] * (b[x]) * pidkp[x];
    // b
    pidoutput[x] +=  - (1.0f - b[x]) * pidkp[x] * gyro[x];
#else
    // P term with b disabled
    pidoutput[x] = error[x] * pidkp[x];
#endif

    // I term
    pidoutput[x] += ierror[x];

    // D term
    // skip yaw D term if not set
    if (pidkd[x] > 0)
    {

#if (defined DTERM_LPF_2ND_HZ && defined ADVANCED_PID_CONTROLLER)
        extern float rxcopy[4];
        float dterm;
        float transitionSetpointWeight[3];
        float stickAccelerator[3];
        float stickTransition[3];

        stickAccelerator[x] = stickAcceleratorProfileA[x];
        stickTransition[x] = stickTransitionProfileA[x];

        if (stickAccelerator[x] < 1)
        {
            transitionSetpointWeight[x] = (fabs(rxcopy[x]) * stickTransition[x]) + (1 - stickTransition[x]);
        }
        else
        {
            transitionSetpointWeight[x] = (fabs(rxcopy[x]) * (stickTransition[x] / stickAccelerator[x])) + (1 - stickTransition[x]);
        }
        static float lastrate[3];
        static float lastsetpoint[3];
        float lpf2(float in, int num);

        dterm = ((setpoint[x] - lastsetpoint[x]) * pidkd[x] * stickAccelerator[x] * transitionSetpointWeight[x] * timefactor) - ((gyro[x] - lastrate[x]) * pidkd[x] * timefactor);
        lastsetpoint[x] = setpoint [x];
        lastrate[x] = gyro[x];
        dterm = lpf2(dterm, x);
        pidoutput[x] += dterm;
#endif

    }

#ifdef PID_VOLTAGE_COMPENSATION
    pidoutput[x] *= v_compensation;
#endif
    limitf(&pidoutput[x], outlimit[x]);

    return pidoutput[x];
}

// calculate change from ideal loop time
// 0.0032f is there for legacy purposes, should be 0.001f = looptime
// this is called in advance as an optimization because it has division
void pid_precalc()
{
    timefactor = 0.0032f / looptime;

#ifdef PID_VOLTAGE_COMPENSATION
    extern float lipo_cell_count;
    v_compensation = mapf((vbattfilt / lipo_cell_count), 2.5, 3.85, PID_VC_FACTOR, 1.00);
    if (v_compensation > PID_VC_FACTOR) v_compensation = PID_VC_FACTOR;
    if (v_compensation < 1.00f) v_compensation = 1.00;
#ifdef LEVELMODE_PID_ATTENUATION
    if (aux[LEVELMODE]) v_compensation *= LEVELMODE_PID_ATTENUATION;
#endif
#endif
}



#ifndef DTERM_LPF_2ND_HZ
#define DTERM_LPF_2ND_HZ 99
#endif

//the compiler calculates these
static float two_one_minus_alpha = 2 * FILTERCALC(0.001, (1.0f / DTERM_LPF_2ND_HZ));
static float one_minus_alpha_sqr = (FILTERCALC(0.001, (1.0f / DTERM_LPF_2ND_HZ))) * (FILTERCALC(0.001, (1.0f / DTERM_LPF_2ND_HZ)));
static float alpha_sqr = (1 - FILTERCALC(0.001, (1.0f / DTERM_LPF_2ND_HZ))) * (1 - FILTERCALC(0.001, (1.0f / DTERM_LPF_2ND_HZ)));

static float last_out[3], last_out2[3];

float lpf2(float in, int num)
{

    float ans = in * alpha_sqr + two_one_minus_alpha * last_out[num]
                - one_minus_alpha_sqr * last_out2[num];

    last_out2[num] = last_out[num];
    last_out[num] = ans;

    return ans;
}

void rotateErrors()
{
#ifdef YAW_FIX
    // rotation around x axis:
    ierror[1] -= ierror[2] * gyro[0] * looptime;
    ierror[2] += ierror[1] * gyro[0] * looptime;

    // rotation around y axis:
    ierror[2] -= ierror[0] * gyro[1] * looptime;
    ierror[0] += ierror[2] * gyro[1] * looptime;

    // rotation around z axis:
    ierror[0] -= ierror[1] * gyro[2] * looptime;
    ierror[1] += ierror[0] * gyro[2] * looptime;
#endif
}
