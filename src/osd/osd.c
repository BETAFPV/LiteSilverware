#include "osd.h"
#include "item.h"
#include "defines.h"
#include "serial_uart.h"
#include "math.h"
#include <stdlib.h>


extern volatile uint32_t sysTickUptime;
extern float rx[4];

#define AETR  ((-0.6f > rx[Yaw]) && (0.3f < rx[Throttle]) && (0.7f > rx[Throttle]) && (0.6f < rx[Pitch]) && (-0.3f < rx[Roll]) && (0.3f > rx[Roll]))



menu_list setMenu, setMenuHead;
menu_list configMenu, configMenuHead;
menu_list layoutMenu, layoutMenuHead;
menu_list vtxMenu, vtxMenuHead;
menu_list currentMenu;

unsigned char showcase = 0;
extern char aux[16];
extern uint8_t openLogBuff[20];
unsigned char osd_mode = 0;
extern int failsafe;
 unsigned char turtlemode;
 unsigned char flymode;
 unsigned char low_vol;
 unsigned int vol;

char down_flag = 0;
char up_flag = 0;
char right_flag = 0;
char left_flag = 0;
unsigned char rx_select = 0;
unsigned char vol_show = 1;
unsigned char rx_show = 1;
unsigned char mode_show = 1;

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
        openLogBuff[2] = (osd_mode<<0) | (failsafe << 1) | (turtlemode<<2) | flymode <<4 ;
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
        
        break;
    
    case 2:
        
        break;
    
    case 3:
        
        break;
    
    case 4:
        
        break;
    }

}

void osd_init(void)
{
    setMenu = createMenu(3, 0);
    setMenuHead = setMenu;

    configMenu = createMenu(5, 1);
    configMenuHead = configMenu;

    layoutMenu = createMenu(3, 2);
    layoutMenuHead =  layoutMenu;

    vtxMenu = createMenu(3, 3);
    vtxMenuHead = vtxMenu;

    currentMenu = setMenu;
}



