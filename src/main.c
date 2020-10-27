#include "stm32f0xx.h"
#include "system.h"
#include "time.h"
#include "pwm.h"
#include "mpu6050.h"
#include "imu.h"
#include "control.h"
#include "rx_bayang.h"




// looptime in seconds
float looptime;
unsigned int lastlooptime;




void failloop(int val)
{
    for (int i = 0 ; i <= 3 ; i++)
    {
        pwm_set(i, 0);
    }

    while (1)
    {
        for (int i = 0 ; i < val; i++)
        {

        }
        delay(800000);
    }
}



int main(void)
{
    while(systemInit());


    lastlooptime = gettime();

    while(1)
    {
        unsigned long time = gettime();
        looptime = ((uint32_t)(time - lastlooptime));
        if (looptime <= 0) looptime = 1;
        looptime = looptime * 1e-6f;
        if (looptime > 0.02f)   // max loop 20ms
        {
            failloop(6);
            //endless loop
        }

        lastlooptime = time;

        mpu6050_read();
        
        control();

        imu_calc();
        
        checkrx();
        
        while(gettime() - time < 1000);
    }

    return 0;
}


