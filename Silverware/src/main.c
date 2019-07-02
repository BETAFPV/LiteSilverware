/*
The MIT License (MIT)

Copyright (c) 2016 silverx

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


// STM32 acro firmware
// files of this project should be assumed MIT licence unless otherwise noted


#include "project.h"

#include "led.h"
#include "util.h"
#include "sixaxis.h"
#include "drv_adc.h"
#include "drv_time.h"
#include "drv_softi2c.h"
#include "config.h"
#include "drv_pwm.h"
#include "drv_adc.h"
#include "drv_gpio.h"
#include "drv_serial.h"
#include "rx.h"
#include "drv_spi.h"
#include "control.h"
#include "pid.h"
#include "defines.h"
#include "drv_i2c.h"
#include "drv_softi2c.h"
#include "drv_serial.h"
#include "buzzer.h"
#include "drv_fmc2.h"
#include "gestures.h"
#include "binary.h"
#include "drv_osd.h"
#include "menu.h"
#include "drv_dshot.h" 
#include "drv_fmc.h"

#include <stdio.h>
#include <math.h>
#include <inttypes.h>

#ifdef USE_SERIAL_4WAY_BLHELI_INTERFACE
#include "drv_softserial.h"
#include "serial_4way.h"
#endif									   
						   
						   
#if defined (__GNUC__)&& !( defined (SOFT_LPF_NONE) || defined (SOFT_LPF_1ST_HZ) || defined (SOFT_LPF_2ST_HZ) )
#warning the soft lpf may not work correctly with gcc due to longer loop time
#endif



#ifdef DEBUG
#include "debug.h"
debug_type debug;
#endif


#define Roll     0   
#define Pitch    1   
#define Yaw      2   
#define Throttle 3   
#define menu_max_item 3
 
 
char down_flag = 0;
char up_flag = 0;
char right_flag = 0;
char left_flag = 0;
char menu_flag = 0;
char PID_parameter_tuning_flag = 0;

Menu_List main_menu,main_menu_head;
Menu_List PID_menu,PID_menu_head;
Menu_List Motor_menu,Motor_menu_head;
Menu_List Menu_pointer;

unsigned char OSD_DATA[4
] = {0x01,0x02,0x03,0x04};

unsigned char show_pidkp[3];
unsigned char show_pidki[3];
unsigned char show_pidkd[3];

// hal
void clk_init(void);
void imu_init(void);
extern void flash_load( void);
extern void flash_save( void);
extern void flash_hard_coded_pid_identifier(void);
extern void motor_dir(uint8_t number, uint16_t value);
extern int save_motor_dir[4];

// looptime in seconds
float looptime;
// filtered battery in volts
float vbattfilt = 0.0;
float vbatt_comp = 4.2;
// voltage reference for vcc compensation
float vreffilt = 1.0;
// average of all motors
float thrfilt = 0;

unsigned int lastlooptime;
// signal for lowbattery
int lowbatt = 1;	

//int minindex = 0;

// holds the main four channels, roll, pitch , yaw , throttle
float rx[4];

// holds auxilliary channels
// the last 2 are always on and off respectively
char aux[AUXNUMBER] = { 0 ,0 ,0 , 0 , 0 , 0};
char lastaux[AUXNUMBER];
// if an aux channel has just changed
char auxchange[AUXNUMBER];


extern float pidkp[PIDNUMBER];  
extern float pidki[PIDNUMBER];	
extern float pidkd[PIDNUMBER];


// bind / normal rx mode
extern int rxmode;
// failsafe on / off
extern int failsafe;
extern float hardcoded_pid_identifier;
extern int onground;
int in_air;
int armed_state;
int arming_release;
int binding_while_armed = 1;

//Experimental Flash Memory Feature
int flash_feature_1 = 0;
int flash_feature_2 = 0;
int flash_feature_3 = 0;

// for led flash on gestures
int ledcommand = 0;
int ledblink = 0;
unsigned long ledcommandtime = 0;

void failloop( int val);
#ifdef USE_SERIAL_4WAY_BLHELI_INTERFACE
volatile int switch_to_4way = 0;
static void setup_4way_external_interrupt(void);
#endif									   
int random_seed = 0;

int main(void)
{
	
	delay(1000);


#ifdef ENABLE_OVERCLOCK
clk_init();
#endif
	
	
#if defined(RX_DSMX_2048) || defined(RX_DSM2_1024)    
	
	lite_2S_BINDKEY_init();            // Initialize the binding key

	delay(1000);
	if(GPIO_ReadInputDataBit(LED1PORT,LED1PIN) == 0)   //Read key value
	{
		lite_2S_rx_spektrum_bind();	   // Send Spektrum bind pulses
	}
	delay(50000);
#endif
	
	
	main_menu = CreateDbCcLinkList(3,0);     //长度为3  0：主菜单
	main_menu_head = main_menu;   					 //记录主菜单的头
	
	PID_menu = CreateDbCcLinkList(9,1);
	PID_menu_head = PID_menu;                //记录PID调试菜单链表的头
	
	Motor_menu = CreateDbCcLinkList(4,2);
	Motor_menu_head = Motor_menu;
	
	Menu_pointer = main_menu;
  gpio_init();	
  ledon(255);	//Turn on LED during boot so that if a delay is used as part of using programming pins for other functions, the FC does not appear inactive while programming times out
	spi_init();
	
  time_init();
	osd_spi_init();
	
	delay(100000);
		
	i2c_init();	
	
	pwm_init();

	pwm_set( MOTOR_BL , 0);
	pwm_set( MOTOR_FL , 0);	 
	pwm_set( MOTOR_FR , 0); 
	pwm_set( MOTOR_BR , 0); 
    delay(1000);
   
	sixaxis_init();

	if ( sixaxis_check() ) 
	{
		
	}
	else 
	{
        //gyro not found   
		failloop(4);
	}
	
	adc_init();
//set always on channel to on
aux[CH_ON] = 1;	
	
#ifdef AUX1_START_ON
aux[CH_AUX1] = 1;
#endif
    
    
 #ifdef FLASH_SAVE1
// read pid identifier for values in file pid.c
    flash_hard_coded_pid_identifier();

// load flash saved variables
    flash_load( );
	

#endif


	
	rx_init();

	
int count = 0;
	
while ( count < 64 )
{
	vbattfilt += adc_read(0);
	delay(1000);
	count++;
}
#ifdef RX_BAYANG_BLE_APP
   // for randomising MAC adddress of ble app - this will make the int = raw float value        
    random_seed =  *(int *)&vbattfilt ; 
    random_seed = random_seed&0xff;
#endif
 vbattfilt = vbattfilt/64;	
// startvref = startvref/64;

	
#ifdef STOP_LOWBATTERY
// infinite loop
if ( vbattfilt < (float) 3.3f) failloop(2);
#endif



	gyro_cal();

extern void rgb_init( void);
rgb_init();

#ifdef SERIAL_ENABLE
serial_init();
#endif



imu_init();

#ifdef FLASH_SAVE2
// read accelerometer calibration values from option bytes ( 2* 8bit)
extern float accelcal[3];
 accelcal[0] = flash2_readdata( OB->DATA0 ) - 127;
 accelcal[1] = flash2_readdata( OB->DATA1 ) - 127;
#endif
				   

extern int liberror;
if ( liberror ) 
{
    failloop(7);
}



 lastlooptime = gettime();


//
//
// 		MAIN LOOP
//
//

#ifdef USE_SERIAL_4WAY_BLHELI_INTERFACE
	setup_4way_external_interrupt();
#endif  
//将要显示的PID值扩大10倍
{
		int i=0;
		for(i=0;i<3;i++)
		{
				show_pidkp[i] = pidkp[i]*100;
		}
		for(i=0;i<3;i++)
		{
				show_pidki[i] = pidki[i]*100;
		}
		for(i=0;i<3;i++)
		{
				show_pidkd[i] = pidkd[i]*100;
		}
}	
	while(1)
	{ 
		// gettime() needs to be called at least once per second 
		unsigned long time = gettime(); 
		looptime = ((uint32_t)( time - lastlooptime));
		if ( looptime <= 0 ) looptime = 1;
		looptime = looptime * 1e-6f;
		if ( looptime > 0.03f ) // max loop 20ms
		{
			failloop( 6);	
			//endless loop			
		}
	
		#ifdef DEBUG				
		debug.totaltime += looptime;
		lpf ( &debug.timefilt , looptime, 0.998 );
		#endif
		lastlooptime = time;
		
		if ( liberror > 20) 
		{
			failloop(8);
			// endless loop
		}

        // read gyro and accelerometer data	
		sixaxis_read();
		
        // all flight calculations and motors
		control();

        // attitude calculations for level mode 		
 		extern void imu_calc(void);		
		imu_calc(); 
       
      
// battery low logic

        // read acd and scale based on processor voltage
		float battadc = adc_read(0)*vreffilt; 
        // read and filter internal reference
        lpf ( &vreffilt , adc_read(1)  , 0.9968f);	
  
		

		// average of all 4 motor thrusts
		// should be proportional with battery current			
		extern float thrsum; // from control.c
	
		// filter motorpwm so it has the same delay as the filtered voltage
		// ( or they can use a single filter)		
		lpf ( &thrfilt , thrsum , 0.9968f);	// 0.5 sec at 1.6ms loop time	

        static float vbattfilt_corr = 4.2;
        // li-ion battery model compensation time decay ( 18 seconds )
        lpf ( &vbattfilt_corr , vbattfilt , FILTERCALC( 1000 , 18000e3) );
	
        lpf ( &vbattfilt , battadc , 0.9968f);


// compensation factor for li-ion internal model
// zero to bypass
#define CF1 0.25f

        float tempvolt = vbattfilt*( 1.00f + CF1 )  - vbattfilt_corr* ( CF1 );

#ifdef AUTO_VDROP_FACTOR

static float lastout[12];
static float lastin[12];
static float vcomp[12];
static float score[12];
static int z = 0;
static int minindex = 0;
static int firstrun = 1;


if( thrfilt > 0.1f )
{
	vcomp[z] = tempvolt + (float) z *0.1f * thrfilt;
		
	if ( firstrun ) 
    {
        for (int y = 0 ; y < 12; y++) lastin[y] = vcomp[z];
        firstrun = 0;
    }
	float ans;
	//	y(n) = x(n) - x(n-1) + R * y(n-1) 
	//  out = in - lastin + coeff*lastout
		// hpf
	ans = vcomp[z] - lastin[z] + FILTERCALC( 1000*12 , 6000e3) *lastout[z];
	lastin[z] = vcomp[z];
	lastout[z] = ans;
	lpf ( &score[z] , ans*ans , FILTERCALC( 1000*12 , 60e6 ) );	
	z++;
       
    if ( z >= 12 )
    {
        z = 0;
        float min = score[0]; 
        for ( int i = 0 ; i < 12; i++ )
        {
         if ( (score[i]) < min )  
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

    float hyst;
    if ( lowbatt ) hyst = HYST;
    else hyst = 0.0f;

    if (( tempvolt + (float) VDROP_FACTOR * thrfilt <(float) VBATTLOW + hyst )
        || ( vbattfilt < ( float ) 2.7f ) )
        lowbatt = 1;
    else lowbatt = 0;

    vbatt_comp = tempvolt + (float) VDROP_FACTOR * thrfilt; 	

           
#ifdef DEBUG
	debug.vbatt_comp = vbatt_comp ;
#endif		
// check gestures
    if ( onground )
	{
	 gestures( );
	}

//        if(aux[0]>0) 
//     ledoff(255); 
//        else
//            ledon(255);


if ( LED_NUMBER > 0)
{
// led flash logic	
    if ( lowbatt )
        ledflash ( 500000 , 8);
    else
    {
        if ( rxmode == RXMODE_BIND)
        {// bind mode
	//				DSHOT_CMD_ROTATE_NORMAL = 20
//       DSHOT_CMD_ROTATE_REVERSE = 21
						motor_dir(0,DSHOT_CMD_ROTATE_REVERSE);
						motor_dir(1,DSHOT_CMD_ROTATE_NORMAL);
					  motor_dir(2,DSHOT_CMD_ROTATE_NORMAL);
						motor_dir(3,DSHOT_CMD_ROTATE_REVERSE);
            ledflash ( 100000, 12);
        }else
        {// non bind
            if ( failsafe) 
                {
                    ledflash ( 500000, 15);			
                }
            else 
            {  
                int leds_on = !aux[LEDS_ON];
                if (ledcommand)
                {
                    if (!ledcommandtime)
                      ledcommandtime = gettime();
                    if (gettime() - ledcommandtime > 500000)
                    {
                        ledcommand = 0;
                        ledcommandtime = 0;
                    }
                    ledflash(100000, 8);
                }
                else if (ledblink)
                {
                    unsigned long time = gettime();
                    if (!ledcommandtime)
                    {
                        ledcommandtime = time;
                        if ( leds_on) ledoff(255);
                        else ledon(255); 
                    }
                    if ( time - ledcommandtime > 500000)
                    {
                        ledblink--;
                        ledcommandtime = 0;
                    }
                     if ( time - ledcommandtime > 300000)
                    {
                        if ( leds_on) ledon(255);
                        else  ledoff(255);
                    }
                }
                else if ( leds_on )
                {
                    if ( LED_BRIGHTNESS != 15)	
                    led_pwm(LED_BRIGHTNESS);
                    else ledon(255);
                }
                else ledoff(255);
            }
        } 		       
    }
}



#if ( RGB_LED_NUMBER > 0)
// RGB led control
extern	void rgb_led_lvc( void);
rgb_led_lvc( );
#ifdef RGB_LED_DMA
extern void rgb_dma_start();
rgb_dma_start();
#endif
#endif


#ifdef BUZZER_ENABLE	
	buzzer();
#endif

            
#ifdef FPV_ON
// fpv switch
    static int fpv_init = 0;
    if ( !fpv_init && rxmode == RXMODE_NORMAL ) {
        fpv_init = gpio_init_fpv();
        }
    if ( fpv_init ) {
        if ( failsafe ) {
            GPIO_WriteBit( FPV_PORT, FPV_PIN, Bit_RESET );
        } else {
            GPIO_WriteBit( FPV_PORT, FPV_PIN, aux[ FPV_ON ] ? Bit_SET : Bit_RESET );
        }
    }
#endif
#ifdef USE_SERIAL_4WAY_BLHELI_INTERFACE
		extern int onground;
		if (onground)
		{
			NVIC_EnableIRQ(EXTI4_15_IRQn);

			if (switch_to_4way)
			{
				switch_to_4way = 0;

				NVIC_DisableIRQ(EXTI4_15_IRQn);
				ledon(2);
				esc4wayInit();
				esc4wayProcess();
				NVIC_EnableIRQ(EXTI4_15_IRQn);
				ledoff(2);

				lastlooptime = gettime();
			}
		}
		else
		{
			NVIC_DisableIRQ(EXTI4_15_IRQn);
		}
#endif

// receiver function
checkrx();

/*osd data transmit*/
while ( (gettime() - time) < LOOPTIME );	

/*******************************************************************************************************************/
if(((float)-0.7 > rx[Yaw]) && ((float)0.3 < rx[Throttle]) && ((float)0.7 > rx[Throttle]) && ((float)0.7 < rx[Pitch]) && ((float)-0.1 < rx[Roll]) && ((float)0.1 > rx[Roll]) && (float)0 == aux[0])    //组合打杆，进入调试界面，前提条件在未解锁情况下
{
		int a;
		menu_flag = 1;
		for(a=0;a<3;a++)
		{
				PID_menu->PID_value = (pidkp[a]*100.0f);
				PID_menu = PID_menu->next;
		}
		
		for(a=0;a<3;a++)
		{
				PID_menu->PID_value = (pidki[a]*100.0f);
				PID_menu = PID_menu->next;
		}
		
		for(a=0;a<3;a++)
		{
				PID_menu->PID_value = (pidkd[a]*100.0f);
				PID_menu = PID_menu->next;
		}
		PID_menu = PID_menu_head;
}

if(1 == menu_flag)
{
		if((rx[Pitch] < (float)-0.6) && (down_flag == 1))
		{
			  Menu_pointer = Menu_pointer->next;
				down_flag = 0;
		}		
		
		if((rx[Pitch] > (float)0.6) && (up_flag == 1))
		{
			  Menu_pointer = Menu_pointer->prior;
				up_flag = 0;
		}
		
		if((rx[Pitch]) < (float)0.6 && (rx[Pitch] > (float)-0.6))
		{
				up_flag = 1;
				down_flag = 1;
		}
		
		/******************************************************************/
		if((rx[Roll] > (float)0.6) && right_flag == 1)     //右打杆操作
		{
				if(0 == Menu_pointer->menu_class && 0 == Menu_pointer->menu_index)    //PID parameter tuning
				{
						Menu_pointer = PID_menu_head;
				}		
				else if(1 == Menu_pointer->menu_class)    //PID值 操作
				{
						int a;
						Menu_pointer->PID_value ++;
						if(Menu_pointer->PID_value >= 100.0f)
						{
								Menu_pointer->PID_value = 100.0f;
						}
						PID_menu = PID_menu_head;
					  //更新PID值
						for(a=0;a<3;a++)
						{
								pidkp[a] = ((float)PID_menu->PID_value/100.0f);
								PID_menu = PID_menu->next;
						}
						
						for(a=0;a<3;a++)
						{
								pidki[a] = ((float)PID_menu->PID_value/100.0f);
								PID_menu = PID_menu->next;
						}
						
						for(a=0;a<3;a++)
						{
								pidkd[a] = ((float)PID_menu->PID_value/100.0f);
								PID_menu = PID_menu->next;
						}
						PID_menu = PID_menu_head;
				}
				
				if(1 == Menu_pointer->menu_class && 9 == Menu_pointer->menu_index) //返回上一级菜单
				{
						int a;
						Menu_pointer = main_menu_head;
					  PID_menu = PID_menu_head;
					  //退出PID调试  更新PID值
						for(a=0;a<3;a++)
						{
								pidkp[a] = ((float)PID_menu->PID_value/100.0f);
								PID_menu = PID_menu->next;
						}
						
						for(a=0;a<3;a++)
						{
								pidki[a] = ((float)PID_menu->PID_value/100.0f);
								PID_menu = PID_menu->next;
						}
						
						for(a=0;a<3;a++)
						{
								pidkd[a] = ((float)PID_menu->PID_value/100.0f);
								PID_menu = PID_menu->next;
						}
						PID_menu = PID_menu_head;
				}
				//进入电机转向调试
				if(0 == Menu_pointer->menu_class  && 1 == Menu_pointer->menu_index)    //Motor direction
				{
					  Menu_pointer = Motor_menu_head;
				}
				else if(2 == Menu_pointer->menu_class)
				{
						Menu_pointer->dir ++;
						if(Menu_pointer->dir > 1)
						{
								Menu_pointer->dir = 1;
						}
				}
				
				if(2 == Menu_pointer->menu_class && 4 == Menu_pointer->menu_index)     //退出电机转向菜单
				{
						int i;
						Menu_pointer = main_menu_head;
						for(i=0;i<4;i++)
						{
								save_motor_dir[i] = Motor_menu_head->dir;
								Motor_menu_head = Motor_menu_head->next;
						}
				}
				
				if(2 == Menu_pointer->menu_class && 4 == Menu_pointer->menu_index) //返回上一级菜单
				{
						Menu_pointer = main_menu_head;
				}
				
				if(0 == Menu_pointer->menu_class && 2 == Menu_pointer->menu_index)    //save parameter
				{
						 #ifdef FLASH_SAVE1
								flash_hard_coded_pid_identifier();
								flash_save( );
                //flash_load( );
                // reset flash numbers
                extern int number_of_increments[3][3];
                for( int i = 0 ; i < 3 ; i++)
                    for( int j = 0 ; j < 3 ; j++)
                        number_of_increments[i][j] = 0; 
              #endif
				}
				
				if(0 == Menu_pointer->menu_class && 3 == Menu_pointer->menu_index)   //menu exit
				{
						//init main menu index paramenter
						menu_flag = 0;
						down_flag = 0;
						up_flag = 0;
						menu_flag = 0;
				}
				right_flag = 0;
		}
		
		if((rx[Roll] < (float)-0.6) && left_flag == 1)     //左打杆操作
		{
				if(1 == Menu_pointer->menu_class)       //PID值 操作
				{
						int a;
						Menu_pointer->PID_value --;
						if(Menu_pointer->PID_value == 0xFF)
						{
								Menu_pointer->PID_value = 0;
						}
						PID_menu = PID_menu_head;
					  //退出PID调试  更新PID值
						for(a=0;a<3;a++)
						{
								pidkp[a] = ((float)PID_menu->PID_value/100.0f);
								PID_menu = PID_menu->next;
						}
						
						for(a=0;a<3;a++)
						{
								pidki[a] = ((float)PID_menu->PID_value/100.0f);
								PID_menu = PID_menu->next;
						}
						
						for(a=0;a<3;a++)
						{
								pidkd[a] = ((float)PID_menu->PID_value/100.0f);
								PID_menu = PID_menu->next;
						}
						PID_menu = PID_menu_head;
				}
				
				if(2 == Menu_pointer->menu_class)
				{
						Menu_pointer->dir --;
						if(Menu_pointer->dir > 0xF0)
						{
								Menu_pointer->dir = 0;
						}
				}
				left_flag = 0;
		}
		
		if((rx[Roll]) < (float)0.6 && (rx[Roll] > (float)-0.6))
		{
				right_flag = 1;
			  left_flag = 1;
		}
}

static uint8 i,j;
    switch(i)
    {
        case 0  :OSD_Data_Send(1,(int)(vbattfilt*100));//transmit vbattfil 
        break; /* 可选的 */
        case 1  :OSD_Data_Send(2,aux[0]);  //transmit ARMING state	
        break; /* 可选的 */
        case 2  :OSD_Data_Send(3,aux[7]);//hori mode
        break; /* 可选的 */
		case 3  :OSD_Data_Send(4,aux[8]);//race mode
        break; /* 可选的 */
		case 4  :
				for(j=0;j<3;j++){
					delay(100);
					OSD_Data_Send(5+j,show_pidkp[j]);//pid
					delay(100);
					}
        break; /* 可选的 */
		case 5  :for(j=0;j<3;j++){
					delay(100);
					OSD_Data_Send(8+j,show_pidki[j]);//pid
					delay(100);
					}
        break; /* 可选的 */

		case 6  :for(j=0;j<3;j++){
					delay(100);
					OSD_Data_Send(11+j,show_pidkd[j]);//pid
					delay(100);
					}
        break; /* 可选的 */


        default : break;
    }
i++;
if(i>6)
{
	i=0;
}


	}// end loop
	

}

// 2 - low battery at powerup - if enabled by config
// 3 - radio chip not detected
// 4 - Gyro not found
// 5 - clock , intterrupts , systick
// 6 - loop time issue
// 7 - i2c error 
// 8 - i2c error main loop


void failloop( int val)
{
	for ( int i = 0 ; i <= 3 ; i++)

	{
		pwm_set( i ,0 );
	}	
	while(1)
	{
		for ( int i = 0 ; i < val; i++)
		{
		 ledon( 255);		
		 delay(200000);
		 ledoff( 255);	
		 delay(200000);			
		}
		delay(800000);
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


#ifdef USE_SERIAL_4WAY_BLHELI_INTERFACE

// set up external interrupt to check 
// for 4way serial start byte
static void setup_4way_external_interrupt(void)
{
	SYSCFG->EXTICR[3] &= ~(0x000F) ; //clear bits 3:0 in the SYSCFG_EXTICR1 reg
	EXTI->FTSR |= EXTI_FTSR_TR14;
	EXTI->IMR |= EXTI_IMR_MR14;
	NVIC_SetPriority(EXTI4_15_IRQn,2);
}

// interrupt for detecting blheli serial 4way
// start byte (0x2F) on PA14 at 38400 baud
void EXTI4_15_IRQHandler(void)
{
	if( (EXTI->IMR & EXTI_IMR_MR14) && (EXTI->PR & EXTI_PR_PR14))
	{
#define IS_RX_HIGH (GPIOA->IDR & GPIO_Pin_14)
		uint32_t micros_per_bit = 26;
		uint32_t micros_per_bit_half = 13;

		uint32_t i = 0;
		// looking for 2F
		uint8_t start_byte = 0x2F;
		uint32_t time_next = gettime();
		time_next += micros_per_bit_half; // move away from edge to center of bit

		for (; i < 8; ++i)
		{
			time_next += micros_per_bit;
			delay_until(time_next);
			if ((0 == IS_RX_HIGH) != (0 == (start_byte & (1 << i))))
			{
				i = 0;
				break;
			}
		}

		if (i == 8)
		{
			time_next += micros_per_bit;
			delay_until(time_next); // move away from edge

			if (IS_RX_HIGH) // stop bit
			{
				// got the start byte
				switch_to_4way = 1;
			}
		}
			
		// clear pending request
		EXTI->PR |= EXTI_PR_PR14 ;
	}
}
#endif



