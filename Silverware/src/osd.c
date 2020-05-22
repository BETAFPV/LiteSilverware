#include "osd.h"
#include "drv_serial.h"
#include "drv_time.h"
#include "math.h"
#include <stdlib.h>

#define AETR  ((-0.6f > rx[Yaw]) && (0.3f < rx[Throttle]) && (0.7f > rx[Throttle]) && (0.6f < rx[Pitch]) && (-0.3f < rx[Roll]) && (0.3f > rx[Roll]))
#define toy_AETR  ((-0.6f > rx[Yaw]) && (0.3f > rx[Throttle]) && (0.6f < rx[Pitch]) && (-0.3f < rx[Roll]) && (0.3f > rx[Roll]))

#define POLYGEN 0xd5


extern void flash_load( void);
extern void flash_save( void);
extern void flash_hard_coded_pid_identifier(void);

extern unsigned char osd_data[12];
extern char aux[16];
extern unsigned int osd_count;
extern unsigned int vol;
extern unsigned int cur;
extern float electricCur;
extern float rx[4];
extern float pidkp[PIDNUMBER];  
extern float pidki[PIDNUMBER];	
extern float pidkd[PIDNUMBER];
extern int number_of_increments[3][3];
extern unsigned long lastlooptime;

unsigned char profileAB =0;

unsigned char powerlevel = 0;
unsigned char channel = 8;
unsigned char powerleveltmp = 0;
unsigned char channeltmp = 0;

unsigned char mode = 0;
unsigned char sa_flag = 0;
unsigned char showcase = 0;
int showcase_cnt = 0;
unsigned char showcase_init=0;
unsigned char low_bat_l=16;
unsigned char mode_l=21;
unsigned char vol_l=23;
unsigned char curr_l = 23;
unsigned char turtle_l=18;
unsigned char tx_config=0;
unsigned char mode_config=0;
unsigned char led_config=0;
unsigned char T8SG_config=0;
char motorDir[4] = {0,0,0,0};

#if defined(f042_1s_bl) || defined(f042_1s_bayang) 
unsigned char low_battery=33;
#endif

#ifdef f042_2s_bl 
unsigned char low_battery=68;
#endif

#ifdef f042_1s_bayang
    unsigned char rx_switch = 0;
#else
    unsigned char rx_switch = 1;
#endif

unsigned int ratesValue=860;
unsigned int ratesValueYaw = 500;


unsigned char main_version = 1;
unsigned char modify_version = 0;
char down_flag = 0;
char up_flag = 0;
char right_flag = 0;
char left_flag = 0;

menu_list setMenu,setMenuHead;
menu_list pidMenu,pidMenuHead;
menu_list motorMenu,motorMenuHead;
menu_list receiverMenu,receiverMenuHead;
menu_list smartaudioMenu,smartaudioMenuHead;
menu_list displayMenu,displayMenuHead;
menu_list ratesMenu,ratesMenuHead;
menu_list currentMenu;


uint8_t CRC8(unsigned char *data, const int8_t len)
{
    uint8_t crc = 0; /* start with 0 so first byte can be 'xored' in */
    uint8_t currByte;

    for (int i = 0 ; i < len ; i++) {
        currByte = data[i];

        crc ^= currByte; /* XOR-in the next input byte */

        for (int i = 0; i < 8; i++) {
            if ((crc & 0x80) != 0) {
                crc = (uint8_t)((crc << 1) ^ POLYGEN);
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}
#ifdef Lite_OSD

void getIndex()
{
    if((rx[Pitch] < -0.6f) && (down_flag == 1))
    {
        currentMenu = currentMenu->next;
        down_flag = 0;
    }		
    
    if((rx[Pitch] > 0.6f) && (up_flag == 1))
    {
        currentMenu = currentMenu->prior;
        up_flag = 0;
    }
    
    if((rx[Pitch]) < 0.6f && (rx[Pitch] > -0.6f))
    {
        up_flag = 1;
        down_flag = 1;
    }
    if((rx[Roll]) < 0.6f && (rx[Roll] > -0.6f))
    {
        right_flag = 1;
        left_flag = 1;
    }
}

void osd_setting()
{
    if(showcase_cnt < 1610)
    {
        showcase_cnt++;
        showcase =6;
    }
    else if(!showcase_init){
        showcase_init = 1;
        showcase =0;
    }
    
    switch(showcase)
    {
        #ifdef f042_1s_bayang
        case 0:
            if(tx_config){
                if(toy_AETR)
                {
                    showcase = 1;
                    unsigned char i = 0;
                    for(i=0; i<3; i++)
                    {
                        pidMenu->fvalue = pidkp[i];
                        pidMenu = pidMenu->next;
                    }
                    
                    for(i=0; i<3; i++)
                    {
                        pidMenu->fvalue = pidki[i];
                        pidMenu = pidMenu->next;
                    }
                    
                    for(i=0; i<3; i++)
                    {
                        pidMenu->fvalue = pidkd[i];
                        pidMenu = pidMenu->next;
                    }
                    
                    pidMenu = pidMenuHead;
                    motorMenu = motorMenuHead;
                    channeltmp = channel;
                    powerleveltmp = powerlevel;
                }
                if(osd_count >= 200)
                {
                    osd_data[0] = 0x0f;
                    osd_data[0] |=showcase << 4;
                    osd_data[1] = aux[CHAN_5];
                    osd_data[2] = 0;
                    osd_data[3] = vol >> 8;
                    osd_data[4] = vol & 0xFF;
                    osd_data[5] = rx_switch;
                    
                    osd_data[6] = 0;
                    osd_data[6] = (aux[CHAN_6] << 0);
       
                    osd_data[7] = 0;
                    osd_data[8] = 0;
                    osd_data[9] = 0;
                #ifdef CURR_ADC
                    osd_data[8] = cur >> 8;
                    osd_data[9] = cur & 0xFF;
                #endif
                    osd_data[10] = 0;
                    osd_data[11] = 0;
                    for (uint8_t i = 0; i < 11; i++)
                        osd_data[11] += osd_data[i];  
                    
                    UART2_DMA_Send();
                    osd_count = 0;
                }   
            }
            else{
                if(!aux[ARMING] && AETR)
                {
                    showcase = 1;
                    unsigned char i = 0;
                    for(i=0; i<3; i++)
                    {
                        pidMenu->fvalue = pidkp[i];
                        pidMenu = pidMenu->next;
                    }
                    
                    for(i=0; i<3; i++)
                    {
                        pidMenu->fvalue = pidki[i];
                        pidMenu = pidMenu->next;
                    }
                    
                    for(i=0; i<3; i++)
                    {
                        pidMenu->fvalue = pidkd[i];
                        pidMenu = pidMenu->next;
                    }
                    
                for(i=0; i<4; i++)
                {
                    motorMenu->uvalue = motorDir[i];
                    motorMenu = motorMenu->next;
                }
                    pidMenu = pidMenuHead;
                    motorMenu = motorMenuHead;
                    channeltmp = channel;
                    powerleveltmp = powerlevel;
                }
                if(osd_count >= 200)
                {
                    osd_data[0] = 0x0f;
                    osd_data[0] |=showcase << 4;
                    osd_data[1] = aux[CHAN_5];
                    osd_data[2] = 0;
                    osd_data[3] = vol >> 8;
                    osd_data[4] = vol & 0xFF;
                    osd_data[5] = rx_switch;
                    
                    osd_data[6] = 0;
                    osd_data[6] = (aux[CHAN_6] << 0) | (aux[CHAN_7] << 1) | (aux[CHAN_8] << 2);
       
                    osd_data[7] = 0;
                    osd_data[8] = 0;
                    osd_data[9] = 0;
                #ifdef CURR_ADC
                    osd_data[8] = cur >> 8;
                    osd_data[9] = cur & 0xFF;
                #endif
                    osd_data[10] = 0;
                    osd_data[11] = 0;
                    for (uint8_t i = 0; i < 11; i++)
                        osd_data[11] += osd_data[i];  
                    
                    UART2_DMA_Send();
                    osd_count = 0;
                }   
            }
            
            break;
        #else
            case 0:
            if(AETR)
            {
                showcase = 1;
                unsigned char i = 0;
                for(i=0; i<3; i++)
                {
                    pidMenu->fvalue = pidkp[i];
                    pidMenu = pidMenu->next;
                }
                
                for(i=0; i<3; i++)
                {
                    pidMenu->fvalue = pidki[i];
                    pidMenu = pidMenu->next;
                }
                
                for(i=0; i<3; i++)
                {
                    pidMenu->fvalue = pidkd[i];
                    pidMenu = pidMenu->next;
                }
                
                for(i=0; i<4; i++)
                {
                    motorMenu->uvalue = motorDir[i];
                    motorMenu = motorMenu->next;
                }
                pidMenu = pidMenuHead;
                motorMenu = motorMenuHead;
                channeltmp = channel;
                powerleveltmp = powerlevel;
            }
            if(osd_count >= 200)
            {
                osd_data[0] = 0x0f;
                osd_data[0] |=showcase << 4;
                osd_data[1] = aux[CHAN_5];
                osd_data[2] = 0;
                osd_data[3] = vol >> 8;
                osd_data[4] = vol & 0xFF;
                osd_data[5] = rx_switch;
                
                osd_data[6] = 0;
                osd_data[6] = (aux[CHAN_6] << 0) | (aux[CHAN_7] << 1) | (aux[CHAN_8] << 2);
   
                osd_data[7] = (!aux[LEVELMODE] && aux[RACEMODE]);
                osd_data[8] = 0;
                osd_data[9] = 0;
                osd_data[10] = 0;
                osd_data[11] = 0;
                for (uint8_t i = 0; i < 11; i++)
                    osd_data[11] += osd_data[i];  
                
                UART2_DMA_Send();
                osd_count = 0;
            }   
            break;
        #endif
        case 1:
            getIndex();
            if((rx[Roll] > 0.6f) && right_flag == 1)
            {
                switch(currentMenu->index)
                {
                    case 0:
                        currentMenu = pidMenuHead;
                        showcase = 2;
                        break;
                    case 1:
                        currentMenu = motorMenuHead;
                        showcase = 3;
                        break; 
                    case 2:
                        currentMenu = receiverMenuHead;
                        showcase = 4;
                        break; 
                    case 3:
                        currentMenu = smartaudioMenuHead;
                        showcase = 5;
                        break; 
                    case 4:
                        currentMenu = displayMenuHead;
                        showcase = 6;
                        break;
                    case 5:
                        currentMenu = ratesMenuHead;
                        showcase = 7;
                        break;
                    case 6:
                        showcase =0;
                        currentMenu = setMenuHead;
                        down_flag = 0;
                        up_flag = 0;
                        
                        extern void flash_save( void);
                        extern void flash_load( void);
						flash_save( );
                        flash_load( );
                    
                        extern int number_of_increments[3][3];
                        for( int i = 0 ; i < 3 ; i++)
                            for( int j = 0 ; j < 3 ; j++)
                                number_of_increments[i][j] = 0;
                        
                        extern unsigned long lastlooptime;
                        lastlooptime = gettime();
                        break;
                    case 7:
                        showcase =0;
                        currentMenu = setMenuHead;
                        down_flag = 0;
                        up_flag = 0;
                        break;
                }
                right_flag = 0;
            }
            if(osd_count >= 200)
            {
                osd_data[0] =0x0f;
                osd_data[0] |=showcase << 4;
                osd_data[1] = currentMenu->index;
                osd_data[2] = main_version;
                osd_data[3] = modify_version;
                osd_data[4] = 0;
                osd_data[5] = 0;
                osd_data[6] = 0;
                osd_data[7] = 0;
                osd_data[8] = 0;
                osd_data[9] = 0;
                osd_data[10] = 0;
                osd_data[11] = 0;
                for (uint8_t i = 0; i < 11; i++)
                    osd_data[11] += osd_data[i];  
                
                UART2_DMA_Send();
                osd_count = 0;
            }   
            break;
        
        case 2:
            getIndex();
            
            if((rx[Roll] > 0.6f) && right_flag == 1)
            {
                if(currentMenu->index <9)
                {
                    int a;
                    currentMenu->fvalue += 0.01f;

                    pidMenu = pidMenuHead;
                    for(a=0;a<3;a++)
                    {
                        pidkp[a] = pidMenu->fvalue;
                        pidMenu = pidMenu->next;
                    }
                    
                    for(a=0;a<3;a++)
                    {
                        pidki[a] = pidMenu->fvalue;
                        pidMenu = pidMenu->next;
                    }
                    
                    for(a=0;a<3;a++)
                    {
                        pidkd[a] = pidMenu->fvalue;
                        pidMenu = pidMenu->next;
                    }
                }
                else{
                    showcase = 1;
                    pidMenu = pidMenuHead;
                    currentMenu = setMenuHead;
                }
                right_flag = 0;
            }
            if((rx[Roll] < -0.6f) && left_flag == 1)
            {
                if(currentMenu->index <9)
                {
                    int a;
                    currentMenu->fvalue -= 0.01f;
                    if(currentMenu->fvalue <=0)
                    {
                        currentMenu->fvalue = 0;
                    }
                    
                    pidMenu = pidMenuHead;
                    for(a=0;a<3;a++)
                    {
                        pidkp[a] = pidMenu->fvalue;
                        pidMenu = pidMenu->next;
                    }
                    
                    for(a=0;a<3;a++)
                    {
                        pidki[a] = pidMenu->fvalue;
                        pidMenu = pidMenu->next;
                    }
                    
                    for(a=0;a<3;a++)
                    {
                        pidkd[a] = pidMenu->fvalue;
                        pidMenu = pidMenu->next;
                    }
                }
                left_flag = 0;
            }
            if(osd_count >= 200)
            {
                osd_data[0] =0x0f;
                osd_data[0] |=showcase << 4;
                osd_data[1] = currentMenu->index;
                osd_data[2] = round(pidkp[0]*100);
                osd_data[3] = round(pidkp[1]*100);
                osd_data[4] = round(pidkp[2]*100);
                osd_data[5] = round(pidki[0]*100);
                osd_data[6] = round(pidki[1]*100);
                osd_data[7] = round(pidki[2]*100);
                osd_data[8] = round(pidkd[0]*100);
                osd_data[9] = round(pidkd[1]*100);
                osd_data[10] = round(pidkd[2]*100);
                osd_data[11] = 0;
                for (uint8_t i = 0; i < 11; i++)
                    osd_data[11] += osd_data[i];  
                
                UART2_DMA_Send();
                osd_count = 0;
            } 
            break;
     #ifdef f042_1s_bayang
        case 3:            
            getIndex();
        
            if((rx[Roll] > 0.6f) && right_flag == 1)
            {
                if(currentMenu->index ==0)
                {
                    tx_config = !tx_config;
                }
                else if(currentMenu->index ==1)
                {
                    mode_config++;
                    if(mode_config>1)
                        mode_config=0;
                }
                else if(currentMenu->index ==2)
                {
                    led_config = !led_config;
                }
                else if(currentMenu->index ==3){
                    T8SG_config =!T8SG_config;
                }
                else{
                    showcase = 1;
                    motorMenu = motorMenuHead;
                    currentMenu = setMenuHead;
                } 
                right_flag = 0;
            }
        
            if(osd_count >= 200)
            {
                osd_data[0] =0x0f;
                osd_data[0] |=showcase << 4;
                osd_data[1] = currentMenu->index;
                osd_data[2] = tx_config;
                osd_data[3] = mode_config;
                osd_data[4] = led_config;
                osd_data[5] = T8SG_config;
                osd_data[6] = 0;
                osd_data[7] = 0;
                osd_data[8] = 0;
                osd_data[9] = 0;
                osd_data[10] = 0;
                osd_data[11] = 0;
                for (uint8_t i = 0; i < 11; i++)
                    osd_data[11] += osd_data[i];  
                
                UART2_DMA_Send();
                osd_count = 0;
            }
            break;
        #else
        case 3:            
            getIndex();
        
            if((rx[Roll] > 0.6f) && right_flag == 1)
            {
                if(currentMenu->index <4)
                {
                    currentMenu->uvalue = !currentMenu->uvalue;
                    motorDir[currentMenu->index] = currentMenu->uvalue;
                }
                else{
                    showcase = 1;
                    motorMenu = motorMenuHead;
                    currentMenu = setMenuHead;
                } 
                right_flag = 0;
            }
        
            if(osd_count >= 200)
            {
                osd_data[0] =0x0f;
                osd_data[0] |=showcase << 4;
                osd_data[1] = currentMenu->index;
                osd_data[2] = motorDir[0] | (motorDir[1] <<1) | (motorDir[2] << 2) | (motorDir[3] <<3);
                osd_data[3] = 0;
                osd_data[4] = 0;
                osd_data[5] = 0;
                osd_data[6] = 0;
                osd_data[7] = 0;
                osd_data[8] = 0;
                osd_data[9] = 0;
                osd_data[10] = 0;
                osd_data[11] = 0;
                for (uint8_t i = 0; i < 11; i++)
                    osd_data[11] += osd_data[i];  
                
                UART2_DMA_Send();
                osd_count = 0;
            }
            break;
        #endif
        case 4:
            getIndex();
            
            if((rx[Roll] > 0.6f) && right_flag == 1)
            {
                if(aux[LEVELMODE])
                {
                    showcase = 1;
                    currentMenu = setMenuHead;
                }
                right_flag = 0;
            }
            if(osd_count >= 200)
            {
                osd_data[0] =0x0f;
                osd_data[0] |=showcase << 4;
                osd_data[1] = currentMenu->index;
                
                osd_data[2]  = round(rx[0]*100);
                osd_data[3] =  round(rx[1]*100);
                osd_data[4] =  round(rx[2]*100);
                osd_data[5] =  round(rx[3]*100);
                osd_data[6] =  aux[CHAN_5];
                osd_data[7] =  aux[CHAN_6];
                osd_data[8] =  aux[CHAN_7];
                osd_data[9] =  aux[CHAN_8];
                osd_data[10] = 0;
                osd_data[11] = 0;
                for (uint8_t i = 0; i < 11; i++)
                    osd_data[11] += osd_data[i];  
                
                UART2_DMA_Send();
                osd_count = 0;
            }
            break;
        
        case 5:
            getIndex();
            
            if((rx[Roll] > 0.6f) && right_flag == 1)
            {
                switch(currentMenu->index)
                {
                    case 0:   
                        channeltmp++;
                        if(channeltmp > 39)
                            channeltmp = 0;
                        sa_flag = 1;
                        break;
                    
                    case 1:
                        powerleveltmp++;
                        if(powerleveltmp >1)
                            powerleveltmp = 0;
                        sa_flag = 2;
                        break;
                    
                    case 2:
                        if(sa_flag == 2)
                        {   
                            powerlevel = powerleveltmp;
                            sa_flag = 0;
                            osd_data[0] =0xAA;
                            osd_data[1] = 0x55;
                            osd_data[2] = 0x05;
                            osd_data[3] = 0x01;
                            osd_data[4] = powerleveltmp;
                            osd_data[5] = CRC8(osd_data,5);
                            osd_data[6] = 0;
                            osd_data[7] = 0;
                            osd_data[8] = 0;
                            osd_data[9] = 0;
                            osd_data[10] = 0;
                            osd_data[11] = 0;
                        }
                        if(sa_flag == 1)
                        {
                            channel = channeltmp;
                            sa_flag = 0;
                            osd_data[0] =0xAA;
                            osd_data[1] = 0x55;
                            osd_data[2] = 0x07;
                            osd_data[3] = 0x01;
                            osd_data[4] = channeltmp;
                            osd_data[5] = CRC8(osd_data,5);
                            osd_data[6] = 0;
                            osd_data[7] = 0;
                            osd_data[8] = 0;
                            osd_data[9] = 0;
                            osd_data[10] = 0;
                            osd_data[11] = 0;
                        }
                        flash_save();
                        extern unsigned long lastlooptime;
                        lastlooptime = gettime();
                        UART2_DMA_Send();
                        break;
                    
                    case 3:
                        showcase = 1;
                        currentMenu = setMenuHead;
                        break;
                    
                    default:
                        break;
                }
                right_flag = 0;
            }
            if((rx[Roll] < -0.6f) && left_flag == 1)
            {
                switch(currentMenu->index)
                {
                    case 0:
                        if(channeltmp == 0)
                        {
                            channeltmp = 40;
                        }
                        channeltmp --;
                        sa_flag = 1;
                        break;
  
                    default:
                        break;
                }
                left_flag = 0;
            }
            
            if(osd_count >= 200)
            {
                osd_data[0] =0x0f;
                osd_data[0] |=showcase << 4;
                osd_data[1] = currentMenu->index;
                osd_data[2] = channel;
                osd_data[3] = powerlevel;
                osd_data[4] = 0;
                osd_data[5] = channeltmp;
                osd_data[6] = powerleveltmp;
                osd_data[7] = 0;
                osd_data[8] = 0;
                osd_data[9] = 0;
                osd_data[10] = 0;
                osd_data[11] = 0;
                for (uint8_t i = 0; i < 11; i++)
                    osd_data[11] += osd_data[i];  
                
                UART2_DMA_Send();
                osd_count = 0;
            }
            break;
           
        case 6:
            getIndex();
        
            if((rx[Roll] > 0.6f) && right_flag == 1)
            {
                switch(currentMenu->index)
                {
                    case 0:
                        low_bat_l++;
                        if(low_bat_l>32)
                            low_bat_l=0;
                        break;
                    
                    case 1:
                        mode_l++;
                        if(mode_l>32)
                            mode_l=0;
                        break;
                    
                    case 2:
                        vol_l++;
                        if(vol_l>32)
                            vol_l=0;
                        break;
                #ifdef f042_1s_bayang
                    case 3:
                        low_battery++;
                        if(low_battery>40)
                            low_battery=28;
                        break;
                        
                    case 4:
                        showcase = 1;
                        displayMenu = displayMenuHead;
                        currentMenu = setMenuHead;
                        break;  
                        
                #endif
                #ifdef f042_1s_bl
                    case 3:
                        turtle_l++;
                        if(turtle_l>32)
                            turtle_l=0;
                        break;
                    
                    case 4:
                        low_battery++;
                        if(low_battery>40)
                            low_battery=28;
                        break;
                        
                    case 5:
                        showcase = 1;
                        displayMenu = displayMenuHead;
                        currentMenu = setMenuHead;
                        break;  
                 #endif
                    
                 #ifdef f042_2s_bl
                    case 3:
                        curr_l++;
                        if(curr_l>32)
                            curr_l=0;
                        break;
                    
                    case 4:
                        turtle_l++;
                        if(turtle_l>32)
                            turtle_l=0;
                        break;
                    
                    case 5:
                        low_battery++;
                        if(low_battery>80)
                            low_battery=55;
                        break;
                        
                    case 6:
                        showcase = 1;
                        displayMenu = displayMenuHead;
                        currentMenu = setMenuHead;
                        break; 
                #endif
                    
                }
                right_flag = 0;
            }
            if((rx[Roll] < -0.6f) && left_flag == 1)
            {
                switch(currentMenu->index)
                {
                    case 0:
                        if(low_bat_l==0)
                            low_bat_l=32;
                        else
                            low_bat_l--;
                        break;
                    
                    case 1:
                        if(mode_l==0)
                            mode_l=32;
                        else
                            mode_l--;
                        break;
                    
                    case 2:
                        if(vol_l==0)
                            vol_l=32;
                        else
                            vol_l--;
                        break;
                #ifdef f042_1s_bayang
                    case 3:
                        low_battery--;
                        if(low_battery<28)
                            low_battery=40;
                        break;
                #endif
                #ifdef f042_1s_bl
                    case 3:
                        if(turtle_l==0)
                            turtle_l=32;
                        else
                            turtle_l--;
                        break;
                    
                    case 4:
                        low_battery--;
                        if(low_battery<28)
                            low_battery=40;
                        break;
                #endif
                        
                #ifdef f042_2s_bl
                    case 3:
                        if(curr_l==0)
                            curr_l=32;
                        else
                            curr_l--;
                        break;
                    
                    case 4:
                        if(turtle_l==0)
                            turtle_l=32;
                        else
                            turtle_l--;
                        break;
                    
                    case 5:
                        low_battery--;
                        if(low_battery<55)
                            low_battery=80;
                        break;
                #endif
                }
                
                left_flag = 0;
            }
            if(osd_count >= 200)
            {
                osd_data[0] =0x0f;
                osd_data[0] |=showcase << 4;
                osd_data[1] = currentMenu->index;
                osd_data[2] = low_bat_l;
                osd_data[3] = mode_l;
                osd_data[4] = vol_l;
                osd_data[5] = 0;
            #ifdef f042_2s_bl
                osd_data[5] = curr_l;
            #endif
                osd_data[6] = turtle_l;
                osd_data[7] = low_battery;
                osd_data[8] = 0;
                osd_data[9] = 0;
                osd_data[10] = 0;
                osd_data[11] = 0;
                for (uint8_t i = 0; i < 11; i++)
                    osd_data[11] += osd_data[i];  
                
                UART2_DMA_Send();
                osd_count = 0;
            }
            break;
        
        case 7:
            getIndex();
        
            if((rx[Roll] > 0.6f) && right_flag == 1)
            {
                switch(currentMenu->index)
                {
                    case 0:
                        ratesValue +=10;
                        break;
                    
                    case 1:
                        ratesValueYaw +=10;
                        break;
                    
                    case 2:
                        profileAB = !profileAB;
                        break;
                    
                    case 3:
                        showcase = 1;
                        ratesMenu = ratesMenuHead;
                        currentMenu = setMenuHead;
                        break;
                }
                right_flag = 0;
            }
            if((rx[Roll] < -0.6f) && left_flag == 1)
            {
                switch(currentMenu->index)
                {
                    case 0:
                        ratesValue -=10;
                        break;
                    
                    case 1:
                        ratesValueYaw -=10;
                        break;
                    
                    case 2:
                        profileAB = !profileAB;
                        break;
                }
                left_flag = 0;
            }
            
            if(osd_count >= 200)
            {
                osd_data[0] =0x0f;
                osd_data[0] |=showcase << 4;
                osd_data[1] = currentMenu->index;
                osd_data[2] = ratesValue >> 8;
                osd_data[3] = ratesValue & 0xff;
                osd_data[4] = ratesValueYaw >> 8;
                osd_data[5] = ratesValueYaw & 0xff;
                osd_data[6] = profileAB;
                osd_data[7] = 0;
                osd_data[8] = 0;
                osd_data[9] = 0;
                osd_data[10] = 0;
                osd_data[11] = 0;
                for (uint8_t i = 0; i < 11; i++)
                    osd_data[11] += osd_data[i];  
                
                UART2_DMA_Send();
                osd_count = 0;
            }
            break;    
        default:
            break;
    }
}

menu_list createMenu(char len,char item)
{
    char i = 0;
    menu_list pTail = NULL,p_new = NULL;
    menu_list pHead = (menu_list)malloc(sizeof(menu_node));
    if(NULL == pHead)
    {
        return 0;
    }
    
    pHead->index = 0;
    pHead->item = item;
    pHead->prior = pHead;
    pHead->next = pHead;
    pTail = pHead;
    
    for(i=1; i<len+1; i++)
    {
        p_new = (menu_list)malloc(sizeof(menu_node));
        if(NULL == p_new)
        {
            return 0;
        }
        p_new->index = i;
        p_new->item = item;
        p_new->prior = pTail;
        p_new->next = pHead;
        pTail->next = p_new;
        pHead->prior = p_new;
        pTail = p_new;
    }
    return pHead;  
}

void osdMenuInit(void)
{
    setMenu = createMenu(7,0);
    setMenuHead = setMenu;
    
    pidMenu = createMenu(9,1);
    pidMenuHead = pidMenu;
    
    motorMenu = createMenu(4,2);
    motorMenuHead = motorMenu;
    
    receiverMenu = createMenu(0,3);
    receiverMenuHead = receiverMenu;
    
    smartaudioMenu = createMenu(3,4);
    smartaudioMenuHead = smartaudioMenu;
    
#ifdef f042_1s_bl
    displayMenu = createMenu(5,5);
#endif
    
#ifdef f042_2s_bl
    displayMenu = createMenu(6,5);
#endif

#ifdef f042_1s_bayang
    displayMenu = createMenu(4,5);
#endif

    displayMenuHead = displayMenu;
    
    ratesMenu = createMenu(3,6);
    ratesMenuHead = ratesMenu;
    
    currentMenu = setMenu;    
}


#endif


