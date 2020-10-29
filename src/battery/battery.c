#include "battery.h"
#include "config.h"
#include "adc.h"
#include "util.h"
#include "defines.h"

extern volatile uint32_t sysTickUptime;


// filtered battery in volts
float vbattfilt = 0.0;
float vbatt_comp = 4.2;
// voltage reference for vcc compensation
float vreffilt = 1.0;
// average of all motors
float thrfilt = 0;

float tempvolt;
float hyst;
// signal for lowbattery
int lowbatt = 1;
float lipo_cell_count = 1;


void batteryUpdate(uint16_t period)
{
    static uint32_t LastRunTime=0;
    if((sysTickUptime-LastRunTime)<period)return;
    LastRunTime=sysTickUptime;

    // read acd and scale based on processor voltage
    float battadc = adc_read(0) * vreffilt;


    // read and filter internal reference
    lpf(&vreffilt, adc_read(1), 0.9968f);


    // average of all 4 motor thrusts
    // should be proportional with battery current
    extern float thrsum; // from control.c

    // filter motorpwm so it has the same delay as the filtered voltage
    // ( or they can use a single filter)
    lpf(&thrfilt, thrsum, 0.9968f);      // 0.5 sec at 1.6ms loop time

    float vbattfilt_corr = 4.2f * lipo_cell_count;
    // li-ion battery model compensation time decay ( 18 seconds )
    lpf(&vbattfilt_corr, vbattfilt, FILTERCALC(1000, 18000e3));

    lpf(&vbattfilt, battadc, 0.9968f);


    // compensation factor for li-ion internal model
    // zero to bypass
#define CF1 0.25f

    tempvolt = vbattfilt * (1.00f + CF1)  - vbattfilt_corr * (CF1);

#ifdef AUTO_VDROP_FACTOR

    static float lastout[12];
    static float lastin[12];
    static float vcomp[12];
    static float score[12];
    static int z = 0;
    static int minindex = 0;
    static int firstrun = 1;


    if (thrfilt > 0.1f)
    {
        vcomp[z] = tempvolt + (float) z * 0.1f * thrfilt;

        if (firstrun)
        {
            for (int y = 0 ; y < 12; y++) lastin[y] = vcomp[z];
            firstrun = 0;
        }
        float ans;
        //  y(n) = x(n) - x(n-1) + R * y(n-1)
        //  out = in - lastin + coeff*lastout
        // hpf
        ans = vcomp[z] - lastin[z] + FILTERCALC(1000 * 12, 6000e3) * lastout[z];
        lastin[z] = vcomp[z];
        lastout[z] = ans;
        lpf(&score[z], ans * ans, FILTERCALC(1000 * 12, 60e6));
        z++;

        if (z >= 12)
        {
            z = 0;
            float min = score[0];
            for (int i = 0 ; i < 12; i++)
            {
                if ((score[i]) < min)
                {
                    min = (score[i]);
                    minindex = i;
                    // add an offset because it seems to be usually early
                    minindex++;
                }
            }
        }

    }

#undef VDROP_FACTOR
#define VDROP_FACTOR  minindex * 0.1f
#endif


    if (lowbatt) hyst = HYST;
    else hyst = 0.0f;

    if ((tempvolt + (float) VDROP_FACTOR * thrfilt < (float) VBATTLOW + hyst)
            || (vbattfilt < (float) 2.7f))
        lowbatt = 1;
    else lowbatt = 0;

    vbatt_comp = tempvolt + (float) VDROP_FACTOR * thrfilt;

}

