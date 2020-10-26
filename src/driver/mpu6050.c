#include "mpu6050.h"
#include "time.h"
#include "bus_i2c.h"
#include "binary.h"
#include "util.h"
#include "math.h"


#define SOFTI2C_GYRO_ADDRESS 0x68
#define GYRO_LOW_PASS_FILTER 0
#define GYRO_ID_1 0x68
#define GYRO_ID_2 0x98 // common h8 gyro
#define GYRO_ID_3 0x7D
#define GYRO_ID_4 0x72

void mpu6050_init(void)
{
    i2c_writereg(SOFTI2C_GYRO_ADDRESS, 107 , 128);

    delay(40000);

    i2c_writereg(SOFTI2C_GYRO_ADDRESS, 25 , 0);

    i2c_writereg(SOFTI2C_GYRO_ADDRESS, 107 , 0x03);
    i2c_writereg(SOFTI2C_GYRO_ADDRESS, 55 , 0 << 7 | 0 << 6 | 0 << 5 | 0 << 4 | 0 << 3 | 0 << 2 | 1 << 1 | 0 << 0);

    delay(100);

    // Gyro DLPF low pass filter
    i2c_writereg(SOFTI2C_GYRO_ADDRESS, 26 , GYRO_LOW_PASS_FILTER);

    i2c_writereg(SOFTI2C_GYRO_ADDRESS, 28, B00010000);   // 8G scale

    i2c_writereg(SOFTI2C_GYRO_ADDRESS, 27 , B00010000);   
}




int mpu6050_check(void)
{
    int id = i2c_readreg(SOFTI2C_GYRO_ADDRESS, 117);

    return (GYRO_ID_1 == id || GYRO_ID_2 == id || GYRO_ID_3 == id || GYRO_ID_4 == id);
}


float accel[3];
float gyro[3];
float accelraw[3];
float accelcal[3] = {0, 0, 0};
float gyrocal[3];
float gyronew[3];

float lpffilter(float in, int num);
float lpffilter2(float in, int num);


void mpu6050_read(void)
{
    int data[16];

    i2c_readdata(SOFTI2C_GYRO_ADDRESS, 59 , data , 14);

    accelraw[0] = -(int16_t)((data[0] << 8) + data[1]);
    accelraw[1] = -(int16_t)((data[2] << 8) + data[3]);
    accelraw[2] = (int16_t)((data[4] << 8) + data[5]);

    float temp = accelraw[1];
    accelraw[1] = -accelraw[0];
    accelraw[0] = temp;

    accelraw[2] = -accelraw[2];
    accelraw[0] = -accelraw[0];

    // reduce to accel in G
    for (int i = 0; i < 3; i++)
    {
        //  accelraw[i] = accel[i]*( 1/ 2048.0f);
        accelraw[i] *= (1 / 4096.0f);
    }

    gyronew[1] = (int16_t)((data[8] << 8) + data[9]);
    gyronew[0] = (int16_t)((data[10] << 8) + data[11]);
    gyronew[2] = (int16_t)((data[12] << 8) + data[13]);


    gyronew[0] = gyronew[0] - gyrocal[0];
    gyronew[1] = gyronew[1] - gyrocal[1];
    gyronew[2] = gyronew[2] - gyrocal[2];

    temp = gyronew[1];
    gyronew[1] = gyronew[0];
    gyronew[0] = -temp;

    gyronew[1] = -gyronew[1];
    gyronew[2] = -gyronew[2];

    gyronew[1] = - gyronew[1];
    gyronew[2] = - gyronew[2];

    for (int i = 0; i < 3; i++)
    {
        gyronew[i] = gyronew[i] * 0.030517578f * 0.017453292f;
        gyro[i] = lpffilter(gyronew[i], i);
        gyro[i] = lpffilter2(gyro[i], i);
    }
}


void gyro_read(void)
{
    int data[6];

    i2c_readdata(SOFTI2C_GYRO_ADDRESS, 67 , data , 6);

    float gyronew[3];

    gyronew[1] = (int16_t)((data[0] << 8) + data[1]);
    gyronew[0] = (int16_t)((data[2] << 8) + data[3]);
    gyronew[2] = (int16_t)((data[4] << 8) + data[5]);

    gyronew[0] = gyronew[0] - gyrocal[0];
    gyronew[1] = gyronew[1] - gyrocal[1];
    gyronew[2] = gyronew[2] - gyrocal[2];

    float temp = gyronew[1];
    gyronew[1] = gyronew[0];
    gyronew[0] = -temp;

    gyronew[1] = -gyronew[1];
    gyronew[2] = -gyronew[2];

    gyronew[1] = - gyronew[1];
    gyronew[2] = - gyronew[2];

    for (int i = 0; i < 3; i++)
    {
        gyronew[i] = gyronew[i] * 0.030517578f * 0.017453292f;
        gyro[i] = lpffilter(gyronew[i], i);
        gyro[i] = lpffilter2(gyro[i], i);
    }
}


#define CAL_TIME 2e6

void gyro_cal(void)
{
    int data[6];
    float limit[3];
    unsigned long time = gettime();
    unsigned long timestart = time;
    unsigned long timemax = time;
    unsigned long lastlooptime = time;

    float gyro[3];

    for (int i = 0 ; i < 3 ; i++)
    {
        limit[i] = gyrocal[i];
    }


    while (time - timestart < CAL_TIME  &&  time - timemax < 15e6)
    {
        unsigned long looptime;
        looptime = time - lastlooptime;
        lastlooptime = time;
        if (looptime == 0) looptime = 1;

        i2c_readdata(SOFTI2C_GYRO_ADDRESS, 67 , data , 6);


        gyro[1] = (int16_t)((data[0] << 8) + data[1]);
        gyro[0] = (int16_t)((data[2] << 8) + data[3]);
        gyro[2] = (int16_t)((data[4] << 8) + data[5]);


#define GLOW_TIME 62500

        for (int i = 0 ; i < 3 ; i++)
        {

            if (gyro[i] > limit[i])  limit[i] += 0.1f;   // 100 gyro bias / second change
            if (gyro[i] < limit[i])  limit[i] -= 0.1f;

            limitf(&limit[i] , 800);

            if (fabsf(gyro[i]) > 100 + fabsf(limit[i]))
            {
                timestart = gettime();
            }
            else
            {
                lpf(&gyrocal[i] , gyro[i], lpfcalc((float) looptime , 0.5 * 1e6));
            }
        }
        while ((gettime() - time) < 1000) delay(10);
        time = gettime();
    }

    if (time - timestart < CAL_TIME)
    {
        for (int i = 0 ; i < 3; i++)
        {
            gyrocal[i] = 0;

        }
    }
}


void acc_cal(void)
{
    accelcal[2] = 1;
    for (int y = 0; y < 500; y++)
    {
        mpu6050_read();
        for (int x = 0; x < 3; x++)
        {
            lpf(&accelcal[x], accelraw[x], 0.92);
        }
        gettime();    // if it takes too long time will overflow so we call it here

    }
    accelcal[2] -= 1;

    for (int x = 0; x < 3; x++)
    {
        limitf(&accelcal[x], 500);
    }
}









