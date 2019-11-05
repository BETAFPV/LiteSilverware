
// library headers
#include <stdbool.h>
#include <inttypes.h>

//#define _USE_MATH_DEFINES
#include <math.h>
#include "drv_time.h"

#include "util.h"
#include "sixaxis.h"
#include "defines.h"
#include "IIR_filter.h"
#include <stdlib.h>

#include <stdio.h>
#ifdef DEBUG
#include "debug.h"
extern debug_type debug;
#endif

#define ACC_1G 1.0f

// disable drift correction ( for testing)
#define DISABLE_ACC 0

#ifdef ACCELEROMETER_DRIFT_FIX
// filter time in seconds
// time to correct gyro readings using the accelerometer
// 1-4 are generally good
#define FILTERTIME 5.0

// accel magnitude limits for drift correction
#define ACC_MIN 0.9f
#define ACC_MAX 1.1f
#else
// filter time in seconds
// time to correct gyro readings using the accelerometer
// 1-4 are generally good
#define FILTERTIME 2.0

// accel magnitude limits for drift correction
#define ACC_MIN 0.7f
#define ACC_MAX 1.3f
#endif

#define _sinf(val) sinf(val)
#define _cosf(val) cosf(val)

float GEstG[3] = { 0, 0, ACC_1G };

float attitude[3];

extern float gyro[3];
extern float accel[3];

extern float accelcal[3];
extern float accelraw[3];
float accelscal[3] = {1,1,1};

void imu_init(void)
{
	// init the gravity vector with accel values
	for (int xx = 0; xx < 100; xx++)
	  {
		  acc_read();

		  for (int x = 0; x < 3; x++)
		    {
			    lpf(&GEstG[x], (accelraw[x] - accelcal[x]) * accelscal[x], 0.85);
		    }
		  delay(1000);
	  }
}

// from http://en.wikipedia.org/wiki/Fast_inverse_square_root
// originally from quake3 code

float Q_rsqrt( float number )
{

	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       
	i  = 0x5f3759df - ( i >> 1 );               
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 3nd iteration, this can be removed
	
	return y;
}


void vectorcopy(float *vector1, float *vector2);


float atan2approx(float y, float x);

float calcmagnitude(float vector[3])
{
	float accmag = 0;
	for (uint8_t axis = 0; axis < 3; axis++)
	  {
		  accmag += vector[axis] * vector[axis];
	  }
	accmag = 1.0f/Q_rsqrt(accmag);
	return accmag;
}


void vectorcopy(float *vector1, float *vector2)
{
	for (int axis = 0; axis < 3; axis++)
	  {
		  vector1[axis] = vector2[axis];
	  }
}

extern float looptime;

extern char aux[16];
extern int onground;
unsigned k = 0,l=0;

void imu_calc(void)
{

// remove bias
    accel[0] = (accelraw[0] - accelcal[0]) * accelscal[0];
    accel[1] = (accelraw[1] - accelcal[1]) * accelscal[1];
	accel[2] = (accelraw[2] - accelcal[2]) * accelscal[2];
     
    accel[0] = LPF2pApply_1(accel[0]);
    accel[1] = LPF2pApply_2(accel[1]);
    accel[2] = LPF2pApply_3(accel[2]);

   // gyro[0] = LPF2pApply_4(gyro[0]);
    //gyro[1] = LPF2pApply_5(gyro[1]);
    //gyro[2] = LPF2pApply_6(gyro[2]);  
//    gyro[0] = 0;
//    gyro[1] = 0;
//    gyro[2] = 0;
      
//	float deltaGyroAngle[3];

//	for ( int i = 0 ; i < 3 ; i++)
//    {
//        deltaGyroAngle[i] = (gyro[i]) * looptime;
//    }
//	
//	
//	GEstG[2] = GEstG[2] - (deltaGyroAngle[0]) * GEstG[0];
//	GEstG[0] = (deltaGyroAngle[0]) * GEstG[2] +  GEstG[0];


//	GEstG[1] =  GEstG[1] + (deltaGyroAngle[1]) * GEstG[2];
//	GEstG[2] = -(deltaGyroAngle[1]) * GEstG[1] +  GEstG[2];


//	GEstG[0] = GEstG[0] - (deltaGyroAngle[2]) * GEstG[1];
//	GEstG[1] = (deltaGyroAngle[2]) * GEstG[0] +  GEstG[1];

    float EstG[3];

	vectorcopy(&EstG[0], &GEstG[0]);

    float gyros[3];
	for (int i = 0; i < 3; i++)
    {
        gyros[i] = gyro[i] * looptime;
    }
    float mat[3][3];
	float cosx, sinx, cosy, siny, cosz, sinz;
	float coszcosx, coszcosy, sinzcosx, coszsinx, sinzsinx;

	cosx = _cosf(gyros[1]);
	sinx = _sinf(gyros[1]);
	cosy = _cosf(gyros[0]);
	siny = _sinf(-gyros[0]);
	cosz = _cosf(gyros[2]);
	sinz = _sinf(-gyros[2]);

	coszcosx = cosz * cosx;
	coszcosy = cosz * cosy;
	sinzcosx = sinz * cosx;
	coszsinx = sinx * cosz;
	sinzsinx = sinx * sinz;

	mat[0][0] = coszcosy;
	mat[0][1] = -cosy * sinz;
	mat[0][2] = siny;
	mat[1][0] = sinzcosx + (coszsinx * siny);
	mat[1][1] = coszcosx - (sinzsinx * siny);
	mat[1][2] = -sinx * cosy;
	mat[2][0] = (sinzsinx) - (coszcosx * siny);
	mat[2][1] = (coszsinx) + (sinzcosx * siny);
	mat[2][2] = cosy * cosx;

	EstG[0] = GEstG[0] * mat[0][0] + GEstG[1] * mat[1][0] + GEstG[2] * mat[2][0];
	EstG[1] = GEstG[0] * mat[0][1] + GEstG[1] * mat[1][1] + GEstG[2] * mat[2][1];
	EstG[2] = GEstG[0] * mat[0][2] + GEstG[1] * mat[1][2] + GEstG[2] * mat[2][2];


    vectorcopy(&GEstG[0], &EstG[0]);
// calc acc mag
	float accmag;

	accmag = calcmagnitude(&accel[0]);

    if(onground)
    {
        if ((accmag > ACC_MIN * ACC_1G) && (accmag < ACC_MAX * ACC_1G) && !DISABLE_ACC)
          {			 
            // normalize acc
            for (int axis = 0; axis < 3; axis++)
            {
                accel[axis] = accel[axis] * ( ACC_1G / accmag);
            }       
            float filtcoeff = lpfcalc_hz( looptime, 1.0f/(float)FILTERTIME);
            for (int x = 0; x < 3; x++)
            {
               lpf(&GEstG[x], accel[x], filtcoeff);
            }
          }
    }
    else
    {
        for (int axis = 0; axis < 3; axis++)
            {
                accel[axis] = accel[axis] * ( ACC_1G / accmag);
            }       
            float filtcoeff = lpfcalc_hz( looptime, 1.0f/(float)FILTERTIME);
            for (int x = 0; x < 3; x++)
            {
               lpf(&GEstG[x], accel[x], filtcoeff);
            }
    }
    

    
#ifdef debug_uart
  // if(aux[CHAN_6])
   {
        k++;
        if(k == 100)
        {
            k=0;
            l++;
            printf("%d,%f,%f,%f,%f,%f,%f\r\n",l,GEstG[0],GEstG[1],GEstG[2], gyro[0], gyro[1], gyro[2]);
        }
    }

#endif
    
	attitude[0] = atan2approx(GEstG[0], GEstG[2]) ;

	attitude[1] = atan2approx(GEstG[1], GEstG[2])  ;

}




#define OCTANTIFY(_x, _y, _o)   do {                            \
    float _t;                                                   \
    _o= 0;                                                \
    if(_y<  0)  {            _x= -_x;   _y= -_y; _o += 4; }     \
    if(_x<= 0)  { _t= _x;    _x=  _y;   _y= -_t; _o += 2; }     \
    if(_x<=_y)  { _t= _y-_x; _x= _x+_y; _y=  _t; _o += 1; }     \
} while(0);

// +-0.09 deg error
float atan2approx(float y, float x)
{

	if (x == 0)
		x = 123e-15f;
	float phi = 0;
	float dphi;
	float t;

	OCTANTIFY(x, y, phi);

	t = (y / x);
	// atan function for 0 - 1 interval
	dphi = t*( ( M_PI/4 + 0.2447f ) + t *( ( -0.2447f + 0.0663f ) + t*( - 0.0663f)) );
	phi *= M_PI / 4;
	dphi = phi + dphi;
	if (dphi > (float) M_PI)
		dphi -= 2 * M_PI;
	return RADTODEG * dphi;
}

