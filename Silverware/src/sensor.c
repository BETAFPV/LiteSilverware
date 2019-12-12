#ifndef __SENSOR_H
#define	__SENSOR_H

#include "sensor.h"
#include <stdint.h>
#include <stdlib.h>
#include "stdbool.h"
#include <math.h>
#include <stdio.h>
#include "LevenbergMarquardt.h"
#include "sixaxis.h"

#define RAD_TO_DEG           57.29577951f	//弧度转角度
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : -(x))


bool should_cali = true;
bool accsuccess = false;
//bool levelCalisuccess = false;
uint8_t accstep = 0; 
float acccal[3];
static Vector3f_t new_offset;
static Vector3f_t new_scale;
static Vector3f_t samples[6];
extern float accelcal[3];

extern float accelraw[3];
enum ORIENTATION_STATUS orientationStatus;
//uint8_t accLevelstep = 0; 
//bool Levelshould_cali = true;
//float Levelacccal[3] = {0,0,0};



//float Pythagorous3(float a, float b, float c)
//{
//    return sqrtf(a*a+b*b+c*c);
//}


//void Vector3f_Normalize(Vector3f_t* vector)
//{
//    float mag = Pythagorous3(vector->x, vector->y, vector->z);

//    vector->x /= mag;
//    vector->y /= mag;
//    vector->z /= mag;
//}
//float SafeArcsin(float v)
//{
//    if (isnan(v)) {
//        return 0.0;
//    }
//    if (v >= 1.0f) {
//        return M_PI/2;
//    }
//    if (v <= -1.0f) {
//        return -M_PI/2;
//    }
//    return asinf(v);
//}

//void AccVectorToRollPitchAngle(Vector3f_t* angle, Vector3f_t vector)
//{
//    //加速度向量归一化
//    Vector3f_Normalize(&vector);

//    angle->x = -SafeArcsin(vector.y);       //横滚角
//    angle->y = atan2f(vector.x, vector.z);  //俯仰角
//}

//float Degrees(float rad)
//{
//    return rad * RAD_TO_DEG;
//}

//void ImuLevelCalibration(void)
//{
//    const int16_t CALIBRATING_ACC_LEVEL_CYCLES = 3000;
//    static float acc_sum[3] = {0, 0, 0};
//    Vector3f_t accAverage;
//    Vector3f_t caliTemp;
//    static int16_t count = 0;

//    if(!Levelshould_cali)
//        return;

//    while(count < 3000)
//    {
//        acc_sum[0] += accelraw[0];
//        acc_sum[1] += accelraw[1];
//        acc_sum[2] += accelraw[2];
//        count++;
//    }
//   

//    if(count == CALIBRATING_ACC_LEVEL_CYCLES)
//    {
//        accAverage.x = acc_sum[0] / (CALIBRATING_ACC_LEVEL_CYCLES-1);
//        accAverage.y = acc_sum[1] / (CALIBRATING_ACC_LEVEL_CYCLES-1);
//        accAverage.z = acc_sum[2] / (CALIBRATING_ACC_LEVEL_CYCLES-1);
//        acc_sum[0] = 0;
//        acc_sum[1] = 0;
//        acc_sum[2] = 0;
//        count = 0;
//        Levelshould_cali = false;

//        //加速度向量转化为姿态角
//        AccVectorToRollPitchAngle(&caliTemp, accAverage);

//        if(abs(Degrees(caliTemp.x)) < 10 && abs(Degrees(caliTemp.y)) < 10)
//        {
//            levelCalisuccess = 1;
//            printf("%s\r\n","level_cal finished");
//            Levelacccal[0] = -caliTemp.x;
//            Levelacccal[1] = -caliTemp.y;
//            Levelacccal[2] = 0;

//        }
//        else
//        {
//            levelCalisuccess = 0;

//        }
//    }
//}


void ImuOrientationDetect(void)
{
    const float CONSTANTS_ONE_G = 1;
    const float accel_err_thr = 0.5;

    // [ g, 0, 0 ]
    if (fabsf(accelraw[0] - CONSTANTS_ONE_G) < accel_err_thr &&
            fabsf(accelraw[1]) < accel_err_thr &&
            fabsf(accelraw[2]) < accel_err_thr)
    {
        orientationStatus = ORIENTATION_FRONT;
        
    }
    // [ -g, 0, 0 ]
    if (fabsf(accelraw[0] + CONSTANTS_ONE_G) < accel_err_thr &&
            fabsf(accelraw[1]) < accel_err_thr &&
            fabsf(accelraw[2]) < accel_err_thr)
    {
        orientationStatus = ORIENTATION_BACK;

    }
    // [ 0, g, 0 ]
    if (fabsf(accelraw[0]) < accel_err_thr &&
            fabsf(accelraw[1] - CONSTANTS_ONE_G) < accel_err_thr &&
            fabsf(accelraw[2]) < accel_err_thr)
    {
        orientationStatus = ORIENTATION_LEFT;

    }
    // [ 0, -g, 0 ]
    if (fabsf(accelraw[0]) < accel_err_thr &&
            fabsf(accelraw[1] + CONSTANTS_ONE_G) < accel_err_thr &&
            fabsf(accelraw[2]) < accel_err_thr)
    {
        orientationStatus = ORIENTATION_RIGHT;

    }
    // [ 0, 0, g ]
    if (fabsf(accelraw[0]) < accel_err_thr &&
            fabsf(accelraw[1]) < accel_err_thr &&
            fabsf(accelraw[2] - CONSTANTS_ONE_G) < accel_err_thr)
    {
        orientationStatus = ORIENTATION_UP;

    }
    // [ 0, 0, -g ]
    if (fabsf(accelraw[0]) < accel_err_thr &&
            fabsf(accelraw[1]) < accel_err_thr &&
            fabsf(accelraw[2] + CONSTANTS_ONE_G) < accel_err_thr)
    {
        orientationStatus = ORIENTATION_DOWN;

    }
}

enum ORIENTATION_STATUS GetImuOrientation(void)
{
    return orientationStatus;
}



void AccCalibration(void)
{
    static uint16_t samples_count = 0;
    static uint8_t orientationCaliFlag[6];
    static uint8_t caliFlag = 0;
    static uint32_t caliCnt = 0;

    if(!should_cali)
        return;

    /*********************************??IMU????************************************/
    if(GetImuOrientation() == ORIENTATION_UP && !orientationCaliFlag[ORIENTATION_UP])
    {
         while(caliCnt<1000)
             caliCnt++;
                
//        if(caliCnt > 1000)
//        {
            caliFlag = 1;
            orientationCaliFlag[ORIENTATION_UP] = 1;
            samples_count = 0;
            accstep++;
//        }
    }

    if(GetImuOrientation() == ORIENTATION_DOWN && !orientationCaliFlag[ORIENTATION_DOWN])
    {
         while(caliCnt<1000)
             caliCnt++;
         
//        if(caliCnt > 1000)
//        {
            caliFlag = 1;
            orientationCaliFlag[ORIENTATION_DOWN] = 1;
            samples_count = 0;
            accstep++;
//        }
    }

    if(GetImuOrientation() == ORIENTATION_FRONT && !orientationCaliFlag[ORIENTATION_FRONT])
    {
        while(caliCnt<1000)
             caliCnt++;

//        if(caliCnt > 1000)
//        {
            caliFlag = 1;
            orientationCaliFlag[ORIENTATION_FRONT] = 1;
            samples_count = 0;
            accstep++;
//        }
    }

    if(GetImuOrientation() == ORIENTATION_BACK && !orientationCaliFlag[ORIENTATION_BACK])
    {

        while(caliCnt<1000)
             caliCnt++;

//        if(caliCnt > 1000)
//        {
            caliFlag = 1;
            orientationCaliFlag[ORIENTATION_BACK] = 1;
            samples_count = 0;
            accstep++;
//        }
    }

    if(GetImuOrientation() == ORIENTATION_LEFT && !orientationCaliFlag[ORIENTATION_LEFT])
    {
        while(caliCnt<1000)
             caliCnt++;
        
//        if(caliCnt > 1000)
//        {
            caliFlag = 1;
            orientationCaliFlag[ORIENTATION_LEFT] = 1;
            samples_count = 0;
            accstep++;
//        }
    }

    if(GetImuOrientation() == ORIENTATION_RIGHT && !orientationCaliFlag[ORIENTATION_RIGHT])
    {

        while(caliCnt<1000)
             caliCnt++;

//        if(caliCnt > 1000)
//        {
            caliFlag = 1;
            orientationCaliFlag[ORIENTATION_RIGHT] = 1;
            samples_count = 0;
            accstep++;
//        }
    }

    while(caliFlag)
    {
        acc_read();
        if(samples_count < 500)
        {
            samples[accstep - 1].x += accelraw[0];
            samples[accstep - 1].y += accelraw[1];
            samples[accstep - 1].z += accelraw[2];
            samples_count++;
        }
        else if(samples_count == 500)
        {
            samples[accstep - 1].x /= 500;
            samples[accstep - 1].y /= 500;
            samples[accstep - 1].z /= 500;
            samples_count++;

            caliFlag = 0;
            caliCnt  = 0;
        }
        
    }

    if(accstep == 6 && samples_count == 501)
    {
        float initBeta[6];
        initBeta[0] = 0;
        initBeta[1] = 0;
        initBeta[2] = 0;
        initBeta[3] = 1;
        initBeta[4] = 1;
        initBeta[5] = 1;


        LevenbergMarquardt(samples, &new_offset, &new_scale, initBeta, 1);

        if(fabsf(new_scale.x-1.0f) > 0.1f || fabsf(new_scale.y-1.0f) > 0.1f || fabsf(new_scale.z-1.0f) > 0.1f)
        {
            accsuccess = false;
        }
        else if(fabsf(new_offset.x) > (1 * 0.35f) || fabsf(new_offset.y) > (1 * 0.35f) || fabsf(new_offset.z) > (1 * 0.6f))
        {
            accsuccess = false;
        }
        else
        {
            accsuccess = true;
        }

        for(uint8_t i=0; i<6; i++)
        {
            samples[i].x = 0;
            samples[i].y = 0;
            samples[i].z = 0;
        }
        
        accelcal[0] = new_offset.x;
        accelcal[1] = new_offset.y;
        accelcal[2] = new_offset.z;
        
        should_cali = false;
        accstep = 0;
        for(uint8_t i=0; i<6; i++)
            orientationCaliFlag[i] = 0;
    }
    
}


#endif



