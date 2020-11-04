/**
******************************************************************************
* @file    configure.c
* @author
* @version V0.0.1
* @date    9/05/2020
* @brief   configure文件，configure相关函数.
******************************************************************************

上位机与飞控的通信相关函数。协议是mavlink.

上位机发送命令过来，飞控才应答，不发不答，发一答一

*/

#include "configure.h"
#include "config.h"
#include "defines.h"
#include "time.h"
#include "flash.h"




//mavlink send defines
mavlink_message_t mavlink_msg;
uint8_t mavlink_buf[64];
unsigned int mavlink_len;

Mavlink_Messages currentMsg;

//mavlink receive defines
int num = 0;
uint8_t receBuf[64] = {0};
uint8_t tab_index=10;
mavlink_message_t receivemsg;
mavlink_status_t status;

uint16_t chan[4]= {0};

extern float rx[];
extern float aux[];
extern unsigned char rx_select;
extern float lowvol;
//uint8_t max_angle = 65;


void serialProcess(uint16_t period)
{
    static uint32_t LastRunTime=0;
    if((sysTickUptime-LastRunTime)<period)return;
    LastRunTime=sysTickUptime;
    
    num = CDC_Receive_BytesAvailable();
    if (num)
    {
        CDC_Receive_DATA(receBuf, num);
       
        for (int i = 0; i < num; i++)
        {
            if (mavlink_parse_char(0, *(receBuf + i), &receivemsg, &status))
            {
                switch (receivemsg.msgid)
                {
                case MAVLINK_MSG_ID_config_tab:
                    mavlink_msg_config_tab_decode(&receivemsg,&currentMsg.tab_index);
                    tab_index = currentMsg.tab_index.tab_index;
                    break;

                case MAVLINK_MSG_ID_reboot:
//                    mavlink_msg_reboot_decode(&receivemsg,&currentMsg.reboot);
//                    usb_for_msp = currentMsg.reboot.flashfirmware;
//                    tab_index = 10;
                    break;
                
                default:
                    break;

                }
            }
        }
    }

    switch(tab_index)
    {
    case 0:
        break;

    case 1:
        break;

    case 2:
        break;

    case 3:
        break;

    case 4:
        mavlink_len = mavlink_msg_tx_pack(1,5,&mavlink_msg,rx[0],rx[1],rx[2],rx[3],chan[0],chan[1],chan[2],chan[3]);
        mavlink_len = mavlink_msg_to_send_buffer(mavlink_buf,&mavlink_msg);
        CDC_Send_DATA(mavlink_buf,mavlink_len);

        break;

    case 5:
        break;

    default:
        break;

    }

}




