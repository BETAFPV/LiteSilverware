#include <stdio.h>
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
#include "defines.h"
#include "rx.h"



#define YEAR ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 + (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))

#define MONTH (__DATE__ [2] == 'n' ? 0 \
    : __DATE__ [2] == 'b' ? 1 \
    : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 2 : 3) \
    : __DATE__ [2] == 'y' ? 4 \
    : __DATE__ [2] == 'n' ? 5 \
    : __DATE__ [2] == 'l' ? 6 \
    : __DATE__ [2] == 'g' ? 7 \
    : __DATE__ [2] == 'p' ? 8 \
    : __DATE__ [2] == 't' ? 9 \
    : __DATE__ [2] == 'v' ? 10 : 11)

#define DAY ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 + (__DATE__ [5] - '0'))

#define MDK_HOUR  ((__TIME__[0]-'0')*10 + __TIME__[1]-'0')
#define MDK_MIN   ((__TIME__[3]-'0')*10 + __TIME__[4]-'0')
#define MDK_SEC   ((__TIME__[6]-'0')*10 + __TIME__[7]-'0')


float looptime;
unsigned int lastlooptime;

uint16_t year,month,day;
uint8_t mdk_hour,mdk_min,mdk_sec;




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

    year = YEAR;
    month = MONTH + 1;
    day = DAY;
    mdk_hour = MDK_HOUR;
    mdk_min = MDK_MIN;
    mdk_sec = MDK_SEC;


    while(1)
    {
        unsigned long time = gettime();
        looptime = ((uint32_t)(time - lastlooptime));
        looptime = looptime * 1e-6f;

        lastlooptime = time;

        mpu6050_read();

        control();

        imu_calc();

        rxCheck();

        batteryUpdate(10);

        usbUpdate(50);

        osd_process(50);

        ledUpdate(30);
#ifdef USE_RX_BAYANG
        keyUpdate(100);
#endif
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

