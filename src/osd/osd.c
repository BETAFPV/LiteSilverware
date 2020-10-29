#include "osd.h"
#include "item.h"
#include "defines.h"
#include "serial_uart.h"
#include "math.h"
#include <stdlib.h>
#include "mpu6050.h"
#include "flash.h"
#include "time.h"
#include "util.h"


extern volatile uint32_t sysTickUptime;
extern float rx[4];

#define AETR  ((-0.6f > rx[Yaw]) && (0.3f < rx[Throttle]) && (0.7f > rx[Throttle]) && (0.6f < rx[Pitch]) && (-0.3f < rx[Roll]) && (0.3f > rx[Roll]))



menu_list setMenu, setMenuHead;
menu_list configMenu, configMenuHead;
menu_list layoutMenu, layoutMenuHead;
menu_list vtxMenu, vtxMenuHead;
menu_list currentMenu;



unsigned char vtx_index = 0;
unsigned char band_index = 0;
uint32_t vtx_freq = 0;
uint32_t old_freq = 0;


unsigned char showcase = 0;
extern char aux[16];
extern uint8_t openLogBuff[20];
unsigned char osd_mode = 0;
extern int failsafe;
unsigned char flymode;
unsigned char low_vol;
unsigned int vol;
extern float vbattfilt;
extern uint8_t rgb_led;
uint8_t acc_checked = 0;
char down_flag = 0;
char up_flag = 0;
char right_flag = 0;
char left_flag = 0;
unsigned char rx_select = 0;
unsigned char vol_show = 1;
unsigned char rx_show = 1;
unsigned char mode_show = 1;
extern unsigned char aux6;
extern unsigned char aux7;
extern unsigned char aux8;
unsigned int ratesValue=860;
unsigned int ratesValueYaw = 500;
unsigned char osd_mode_cnt=0;

void getIndex()
{
    if ((rx[Pitch] < -0.6f) && (down_flag == 1))
    {
        currentMenu = currentMenu->next;
        down_flag = 0;
    }

    if ((rx[Pitch] > 0.6f) && (up_flag == 1))
    {
        currentMenu = currentMenu->prior;
        up_flag = 0;
    }

    if ((rx[Pitch]) < 0.6f && (rx[Pitch] > -0.6f))
    {
        up_flag = 1;
        down_flag = 1;
    }
    if ((rx[Roll]) < 0.6f && (rx[Roll] > -0.6f))
    {
        right_flag = 1;
        left_flag = 1;
    }
}


void osd_process(uint16_t period)
{
    static uint32_t LastRunTime=0;
    if((sysTickUptime-LastRunTime)<period)return;
    LastRunTime=sysTickUptime;

    vol = (int)(vbattfilt * 100);

    if( (vbattfilt) < 3.3)
    {
        low_vol = 1;
    }
    else {
        low_vol = 0;
    }

    switch (showcase)
    {
    case 0:
        if (!aux[ARMING] && AETR)
        {
            showcase = 1;
        }

        openLogBuff[0] = 0x0f;
        openLogBuff[0] |= 0 << 4;
        openLogBuff[1] = (aux[CHAN_5] << 0) | (aux[CHAN_6] << 2) | (aux[CHAN_7] << 4) | (aux[CHAN_8] << 6) ;
        openLogBuff[2] = (osd_mode<<0) | (failsafe << 1)  | flymode <<4 ;
        openLogBuff[3] = vol >> 8;
        openLogBuff[4] = vol & 0xFF;
        openLogBuff[5] = low_vol | rx_select << 4;

        openLogBuff[6] = 0;
        openLogBuff[6] = (vol_show << 0) | (rx_show << 1) | (mode_show << 2) | (rx[0] < 0 ? 1 : 0) << 4 | (rx[1] < 0 ? 1 : 0) << 5 | (rx[2] < 0 ? 1 : 0) << 6;

        openLogBuff[7]  = round(fabs(rx[0]) * 100);
        openLogBuff[8] =  round(fabs(rx[1]) * 100);
        openLogBuff[9] =  round(fabs(rx[2]) * 100);
        openLogBuff[10] =  round(fabs(rx[3]) * 100);

        openLogBuff[11] = 0;
        openLogBuff[12] = 0;
        for (uint8_t i = 0; i < 12; i++)
            openLogBuff[12] += openLogBuff[i];

        UART2_DMA_Send();

        break;

    case 1:
        
        if (osd_mode)
        {
            getIndex();
            
            if ((rx[Roll] > 0.6f) && right_flag == 1)
            {
                switch (currentMenu->index)
                {
                case 0:
                    currentMenu = configMenuHead;
                    showcase = 2;
                    break;
                case 1:
                    currentMenu = layoutMenuHead;
                    showcase = 3;
                    break;
                case 2:
                    currentMenu = vtxMenuHead;
                    showcase = 4;
                    break;
                case 3:
                    flash_save();

                    showcase = 0;
                    currentMenu = setMenuHead;
                    down_flag = 0;
                    up_flag = 0;

                    NVIC_SystemReset();
                    break;
                case 4:
                    showcase = 0;
                    currentMenu = setMenuHead;
                    down_flag = 0;
                    up_flag = 0;
                    break;
                }
                right_flag = 0;
            }
        }
        else{
             if ((rx[Pitch] < -0.6f) && (down_flag == 1))
            {
                switch (currentMenu->index)
                {
                case 0:
                    currentMenu = currentMenu->next;
                    break;
                case 1:
                    currentMenu = currentMenu->next;
                    currentMenu = currentMenu->next;
                    break;

                case 3:
                    currentMenu = currentMenu->next;
                    break;

                case 4:
                    currentMenu = currentMenu->next;
                    break;
                }
                down_flag = 0;
            }

            if ((rx[Pitch] > 0.6f) && (up_flag == 1))
            {
                switch (currentMenu->index)
                {
                case 0:
                    currentMenu = currentMenu->prior;
                    break;
                case 1:
                    currentMenu = currentMenu->prior;
                    break;

                case 3:
                    currentMenu = currentMenu->prior;
                    currentMenu = currentMenu->prior;
                    break;

                case 4:
                    currentMenu = currentMenu->prior;
                    break;
                }
                up_flag = 0;
            }

            if ((rx[Pitch]) < 0.6f && (rx[Pitch] > -0.6f))
            {
                up_flag = 1;
                down_flag = 1;
            }
            if ((rx[Roll]) < 0.6f && (rx[Roll] > -0.6f))
            {
                right_flag = 1;
                left_flag = 1;
            }

            if ((rx[Roll] > 0.6f) && right_flag == 1)
            {
                switch (currentMenu->index)
                {
                case 0:
                    currentMenu = configMenuHead;
                    showcase = 2;
                    break;
                case 1:
                    currentMenu = layoutMenuHead;
                    showcase = 3;
                    break;

                case 3:
                    flash_save();
                    showcase = 0;
                    currentMenu = setMenuHead;
                    down_flag = 0;
                    up_flag = 0;

                    NVIC_SystemReset();
                    break;
                case 4:
                    showcase = 0;
                    currentMenu = setMenuHead;
                    down_flag = 0;
                    up_flag = 0;
                    break;
                }
                right_flag = 0;
            }    
        }
        openLogBuff[0] = 0x0f;
        openLogBuff[0] |= showcase << 4;
        openLogBuff[1] = currentMenu->index;
        openLogBuff[2] = osd_mode;
        openLogBuff[3] = 0;
        openLogBuff[4] = 0;
        openLogBuff[5] = 0;
        openLogBuff[6] = 0;
        openLogBuff[7] = 0;
        openLogBuff[8] = 0;
        openLogBuff[9] = 0;
        openLogBuff[10] = 0;
        openLogBuff[11] = 0;
        openLogBuff[12] = 0;
        for (uint8_t i = 0; i < 12; i++)
            openLogBuff[12] += openLogBuff[i];

        UART2_DMA_Send();
        
        break;

    case 2:
        if(osd_mode)
        {
            getIndex();
            
            if ((rx[Roll] > 0.6f) && right_flag == 1)
            {
                switch (currentMenu->index)
                {
                case 0:
                    osd_mode = !osd_mode;
                    if(!osd_mode)
                    {
                        rx_show = 0;
                    }
                    else
                    {
                        rx_show = 1;
                    }
                    break;

                case 1:
                    rgb_led = !rgb_led;
                    break;

                case 2:
                    gyro_cal();
                    acc_cal();

                    flash_save();
                    acc_checked = 1;
                    break;

                case 3:
                    ratesValue += 10;
                    break;

                case 4:
                    ratesValueYaw += 10;
                    break;

                case 5:
                    currentMenu = setMenuHead;
                    showcase = 1;
                    break;
                }
                right_flag = 0;
            }
            if ((rx[Roll] < -0.6f) && left_flag == 1)
            {
                switch (currentMenu->index)
                {
                case 3:
                    ratesValue -= 10;
                    break;

                case 4:
                    ratesValueYaw -= 10;
                    break;
                }
                left_flag = 0;
            }
        }
        else
        {
            if ((rx[Pitch] < -0.6f) && (down_flag == 1))
            {
                switch (currentMenu->index)
                {
                case 0:
                    currentMenu = currentMenu->next;
                    break;
                case 1:
                    currentMenu = currentMenu->next;
                    break;
                
                case 2:
                    currentMenu = currentMenu->next;
                    currentMenu = currentMenu->next;
                    currentMenu = currentMenu->next;
                    break;
                
                case 5:
                    currentMenu = currentMenu->next;
                    break;
                }
                down_flag = 0;
            }

            if ((rx[Pitch] > 0.6f) && (up_flag == 1))
            {
                switch (currentMenu->index)
                {
                case 0:
                    currentMenu = currentMenu->prior;
                    break;
                case 1:
                    currentMenu = currentMenu->prior;
                    break;

                case 2:
                    currentMenu = currentMenu->prior;
                    break;

                case 5:
                    currentMenu = currentMenu->prior;
                    currentMenu = currentMenu->prior;
                    currentMenu = currentMenu->prior;
                    break;
                }
                up_flag = 0;
            }

            if ((rx[Pitch]) < 0.6f && (rx[Pitch] > -0.6f))
            {
                up_flag = 1;
                down_flag = 1;
            }
            if ((rx[Roll]) < 0.6f && (rx[Roll] > -0.6f))
            {
                right_flag = 1;
                left_flag = 1;
            }
            
            if ((rx[Roll] > 0.6f) && right_flag == 1)
            {
                switch (currentMenu->index)
                {
                case 0:
                    if(osd_mode_cnt ==3)
                    {
                        osd_mode_cnt = 0;
                        osd_mode = !osd_mode;
                    }

                    if(osd_mode_cnt ==2)
                        osd_mode_cnt++;

                    if(!osd_mode)
                    {
                        rx_show = 0;
                    }
                    else
                    {
                        rx_show = 1;
                    }
                    aux6 = aux[LEVELMODE];
                    aux7 = aux[RACEMODE];
                    aux8 = aux[HORIZON];
                    break;

                case 1:
                    rgb_led = !rgb_led;
                    break;

                case 2:
                    gyro_cal();
                    acc_cal();

                    flash_save();
                    acc_checked = 1;
                    break;

                case 5:
                    currentMenu = setMenuHead;
                    showcase = 1;
                    break;
                }
                right_flag = 0;
            }
            if ((rx[Roll] < -0.6f) && left_flag == 1)
            {
                switch (currentMenu->index)
                {
                case 0:
                    osd_mode_cnt++;
                    if(osd_mode_cnt>=2)
                        osd_mode_cnt = 2;
                    break;

                }
                left_flag = 0;
            }
            
        }
        
       
        openLogBuff[0] = 0x0f;
        openLogBuff[0] |= showcase << 4;
        openLogBuff[1] = currentMenu->index;
        openLogBuff[2] = osd_mode;
        openLogBuff[3] = ratesValue >> 8;
        openLogBuff[4] = ratesValue & 0xff;
        openLogBuff[5] = ratesValueYaw >> 8;
        openLogBuff[6] = ratesValueYaw & 0xff;
        openLogBuff[7] = acc_checked;
        openLogBuff[8] = rx_select;
        openLogBuff[9] = rgb_led;
        openLogBuff[10] = 0;
        openLogBuff[11] = 0;
        openLogBuff[12] = 0;
        for (uint8_t i = 0; i < 12; i++)
            openLogBuff[12] += openLogBuff[i];

        UART2_DMA_Send();
        break;

    case 3:
        getIndex();

        if ((rx[Roll] > 0.6f) && right_flag == 1)
        {
            switch (currentMenu->index)
            {
            case 0:
                rx_show = !rx_show;
                break;

            case 1:
                mode_show = !mode_show;
                break;

            case 2:
                vol_show = !vol_show;
                break;

            case 3:
                showcase = 1;
                currentMenu = setMenuHead;
                break;
            }
            right_flag = 0;
        }

        openLogBuff[0] = 0x0f;
        openLogBuff[0] |= showcase << 4;
        openLogBuff[1] = currentMenu->index;
        openLogBuff[2]  = rx_show;
        openLogBuff[3] =  mode_show;
        openLogBuff[4] =  vol_show;
        openLogBuff[5] =  0;
        openLogBuff[6] =  0;
        openLogBuff[7] =  0;
        openLogBuff[8] =  0;
        openLogBuff[9] =  0;
        openLogBuff[10] = 0;
        openLogBuff[11] = 0;
        openLogBuff[12] = 0;
        for (uint8_t i = 0; i < 12; i++)
            openLogBuff[12] += openLogBuff[i];

        UART2_DMA_Send();

        break;

    case 4:
        getIndex();

        if ((rx[Roll] > 0.6f) && right_flag == 1)
        {
            switch (currentMenu->index)
            {
            case 0:
                vtx_index ++;
                if (vtx_index > 39)
                    vtx_index = 0;

                break;

            case 1:
                if(band_index<4)
                {
                    band_index++;
                    vtx_index +=8;
                }
                else {
                    band_index = 0;
                    vtx_index -=32;
                }
                break;

            case 2:
                openLogBuff[0] = 0xAA;
                openLogBuff[1] = 0x55;
                openLogBuff[2] = 0x07;
                openLogBuff[3] = 0x01;
                openLogBuff[4] = vtx_index;
                openLogBuff[5] = CRC8(openLogBuff,5);
                openLogBuff[6] = 0;
                openLogBuff[7] = 0;
                openLogBuff[8] = 0;
                openLogBuff[9] = 0;
                openLogBuff[10] = 0;
                openLogBuff[11] = 0;
                openLogBuff[12] = 0;

                UART2_DMA_Send();
                
                delay_ms(100);
                flash_save();
                break;

            case 3:
                currentMenu = setMenuHead;
                showcase = 1;
                break;
            }
            right_flag = 0;
        }
        if ((rx[Roll] < -0.6f) && left_flag == 1)
        {
            switch (currentMenu->index)
            {
            case 0:
                if (vtx_index == 0)
                {
                    vtx_index = 39;
                }
                else
                {
                    vtx_index --;
                }
                break;

            case 1:
                if(band_index>0)
                {
                    band_index--;
                    vtx_index -=8;
                }
                else
                {
                    band_index = 4;
                    vtx_index +=32;
                }
                break;

            default:
                break;
            }
            left_flag = 0;
        }
        if(vtx_index<8)
        {
            band_index = 0;
        }
        else if(vtx_index<16)
        {
            band_index = 1;
        }
        else if(vtx_index<24)
        {
            band_index = 2;
        }
        else if(vtx_index<32)
        {
            band_index = 3;
        }
        else {
            band_index = 4;
        }
        
        
        openLogBuff[0] = 0x0f;
        openLogBuff[0] |= showcase << 4;
        openLogBuff[1] = currentMenu->index;
        openLogBuff[2] = band_index;
        openLogBuff[3] = vtx_index;
        openLogBuff[4] = 0;
        openLogBuff[5] = 0;
        openLogBuff[6] = 0;
        openLogBuff[7] = 0;
        openLogBuff[8] = 0;
        openLogBuff[9] = 0;
        openLogBuff[10] = 0;
        openLogBuff[11] = 0;
        openLogBuff[12] = 0;
        for (uint8_t i = 0; i < 12; i++)
            openLogBuff[12] += openLogBuff[i];

        UART2_DMA_Send();
        break;
    }

}

void osd_init(void)
{
    setMenu = createMenu(4, 0);
    setMenuHead = setMenu;

    configMenu = createMenu(5, 1);
    configMenuHead = configMenu;

    layoutMenu = createMenu(3, 2);
    layoutMenuHead =  layoutMenu;

    vtxMenu = createMenu(3, 3);
    vtxMenuHead = vtxMenu;

    currentMenu = setMenu;
}



