#include "rx.h"
#include "system.h"
#include "time.h"
#include "config.h"
#include "defines.h"
#include "serial_uart.h"
#include "util.h"
#include "flash.h"
#include "rx_bayang.h"
#include "rx_sbus.h"
#include "rx_dsmx.h"



extern unsigned char osd_mode;
extern unsigned char flymode;
extern unsigned char aux6;
extern unsigned char aux7;
extern unsigned char aux8;
extern char aux[AUXNUMBER];
extern unsigned char vtx_index ;
uint8_t crash = 0;
extern float attitude[3];
extern int init_rx;
extern uint8_t openLogBuff[20];

rxUpdate *rxCheck;

void rx_init(void)
{
#ifdef USE_RX_BAYANG
    bayang_init();
    rxCheck = bayang_check;
#endif


#ifdef USE_RX_SBUS
    sbus_init();
    rxCheck =  sbus_check;
#endif


#ifdef USE_RX_DSMX
    dsm_init();
    rxCheck = dsm_check;
#endif

}



void flymodeUpdate(uint16_t period)
{
    static uint32_t LastRunTime=0;
    if((sysTickUptime-LastRunTime)<period)return;
    LastRunTime=sysTickUptime;

    if (osd_mode)
    {
        if (aux[LEVELMODE])
        {
            if (aux[RACEMODE])
            {
                if (aux[HORIZON])
                {
                    flymode = 2;
                }
                else
                {
                    flymode = 3;
                }
            }
            else
            {
                if (aux[HORIZON])
                {
                    flymode = 4;
                }
                else
                {
                    flymode = 0;
                }
            }
        }
        else
        {
            flymode = 1;
        }

    }
    else
    {
        if (!aux[LEVELMODE])
        {
            flymode=0;
        }
        else if(aux[LEVELMODE] == 1)
        {
            flymode=4;
        }
        else
        {
            flymode=1;
        }
        if (aux7 != aux[RACEMODE] && (init_rx >=100))
        {
            aux7 = aux[RACEMODE];
            vtx_index++;
            if (vtx_index > 15 || vtx_index <8)
                vtx_index = 8;

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
        }


        if(flymode==0 && (attitude[0] > (65 + 10) || attitude[0] < -(65 + 10) || attitude[1] > (65 + 10) || attitude[1] <-(65 + 10)))
        {
            crash = 1;
        }
    }
}




