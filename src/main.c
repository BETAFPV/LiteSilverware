#include "stm32f0xx.h"
#include "system.h"
#include "time.h"
#include "pwm.h"
#include "mpu6050.h"
#include "imu.h"
#include "control.h"
#include "rx_bayang.h"
#include "battery.h"
#include "osd.h"
#include "led.h"
#include "misc_gpio.h"
#include "configure.h"


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
        looptime = looptime * 1e-6f;

        lastlooptime = time;

        mpu6050_read();

        control();

        imu_calc();

        checkrx(10);

        batteryUpdate(20);

        serialProcess(50);
        
        osd_process(20);

        ledUpdate(50);

        keyUpdate(100);
        
        flymodeUpdate(50);
        
        while(gettime() - time < 1000);
    }

}

void HardFault_Handler(void)
{
	failloop(5);
}
void MemManage_Handler(void) 
{
	failloop(5);
}
void BusFault_Handler(void) 
{
	failloop(5);
}
void UsageFault_Handler(void) 
{
	failloop(5);
}

