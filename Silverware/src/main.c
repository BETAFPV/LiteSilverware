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
#include "defines.h"
#include "led.h"
#include "util.h"
#include "sixaxis.h"
#include "drv_adc.h"
#include "drv_time.h"
#include "drv_softi2c.h"
#include "drv_pwm.h"
#include "drv_adc.h"
#include "drv_gpio.h"
#include "drv_serial.h"
#include "rx.h"
#include "drv_spi.h"
#include "control.h"
#include "pid.h"
#include "drv_i2c.h"
#include "drv_softi2c.h"
#include "drv_serial.h"
#include "drv_dshot.h"
#include "buzzer.h"
#include "drv_fmc2.h"
#include "binary.h"
#include "osd.h"
#include "IIR_filter.h"


#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include "rx_sbus_dsmx_bayang_switch.h"

#ifdef USE_SERIAL_4WAY_BLHELI_INTERFACE
#include "drv_softserial.h"
#include "serial_4way.h"
#endif									   
						   
						   
#if defined (__GNUC__)&& !( defined (SOFT_LPF_NONE) || defined (GYRO_FILTER_PASS1) || defined (GYRO_FILTER_PASS2) )
#warning the soft lpf may not work correctly with gcc due to longer loop time
#endif



#ifdef DEBUG
#include "debug.h"
debug_type debug;
#endif

// hal
void clk_init(void);
void imu_init(void);
extern void flash_load( void);
void (*pFun)(void);
extern void flash_save( void);
extern void flash_hard_coded_pid_identifier(void);
extern unsigned char showcase;
extern unsigned char rx_switch;
//lite osd
unsigned char cal=0;
unsigned char osd_data[12] = {0};
unsigned int osd_count = 0;
unsigned int pwm_count = 0;
unsigned int vol = 0;
float electricCur = 0;
unsigned int cur = 0;
unsigned char flip_start = 0;
unsigned char flip_finish = 0;
extern int sbus_dsmx_flag;
extern unsigned char motorDir[4];

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
// analog version of each aux channel
float aux_analog[AUXNUMBER];
float lastaux_analog[AUXNUMBER];
// if an analog aux channel has just changed
char aux_analogchange[AUXNUMBER];

// bind / normal rx mode
extern int rxmode;

unsigned char aux7=0;
extern unsigned char tx_config;
// failsafe on / off
extern int failsafe;
extern float hardcoded_pid_identifier;
extern int onground;
int in_air;
int armed_state;
int arming_release;
int binding_while_armed = 1;
float lipo_cell_count = 1;

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

#define ApplicationAddress 0x1FFFC400
  
typedef void (*pFunction)(void);
float tempvolt;
float hyst;
//copy from AN2606.pdf
/*
Due to empty check mechanism present on this product, it is not possible to jump from user 
code to system bootloader. 
Such jump will result in a jump back to user flash space.
But if the first 4 bytes of User Flash (at 0x0800 0000) are empty at the moment of jump (ie. 
erase first sector before jump or execute code from SRAM while Flash is empty), then 
system bootloader will be executed when jumped to.
*/
void flashErase( void) {

    FLASH_Unlock();
    
	int test = FLASH_ErasePage( 0X08000000 );
    test = FLASH_ErasePage( 0X08007C00 );
	if ( test != FLASH_COMPLETE ) FLASH_Lock();
    else return ;

    FLASH_ProgramWord(0x8000000, 0);
    FLASH_ProgramWord(0x8000000 +2, 0);
    FLASH_ProgramWord(0x8000000 + 4, 0);
    FLASH_ProgramWord(0x8000000 + 6, 0);  
    FLASH_ProgramWord(0x8000000 + 8, 0);  

    FLASH_Lock();
}


int main(void)
{
	
	delay(1000);
    
#ifdef f042_1s_bayang
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;	
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_PA11_PA12_RMP;
    delay(1000);
    gpiopa_init();
    
    delay(100000);
    
    //judge PA11 OR PA12 whther pull down by pluging a usb
    if( KEY11 ==0 || KEY12 == 0)
    {
        flashErase();

        delay(1000);
        uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);

        pFunction Jump_To_Boot = (pFunction) JumpAddress;

        __set_MSP(*(__IO uint32_t*) ApplicationAddress);

        Jump_To_Boot();
    }
    
#endif    


#ifdef ENABLE_OVERCLOCK
clk_init();
#endif
    

	
#if defined(RX_SBUS_DSMX_BAYANG_SWITCH) 
    switch_key(); 
    if(KEY ==0)
    {	
       lite_2S_rx_spektrum_bind();
        
    #ifdef f042_1s_bayang
        rx_switch = 0;
    #else
        rx_switch = 1;
    #endif
        
        while(KEY == 0); 
        unsigned long time=0;
        while(time < 4000000)       
        {
            if(KEY == 0)
            {
                rx_switch++;
                while(KEY == 0);
            }
            if(rx_switch >= 3)
            {
                rx_switch = 3;
            }
            time++;
        }
        flash_save();
    }
#else    
#ifdef RX_BAYANG_PROTOCOL_TELEMETRY_AUTOBIND
    switch_key(); 
    if(KEY ==0)
    {	        
        extern int rx_bind_enable;
        rx_bind_enable=0;
        flash_save();
    }
#endif
#ifdef RX_SBUS
rx_switch = 2 ;
#endif

#ifdef RX_DSMX_2048
rx_switch = 3;
#endif
#ifdef RX_DSMX_1024
rx_switch = 4;
#endif
#ifdef RX_IBUS
rx_switch = 5;
#endif 
#endif
    
    
  gpio_init();	
    
  ledon(255);		//Turn on LED during boot so that if a delay is used as part of using programming pins for other functions, the FC does not appear inactive while programming times out
	spi_init();
	
  time_init();

 #ifdef Lite_OSD
    
    osdMenuInit();
  
    #ifndef debug_uart
      UART2_init(4800);
    #endif

 #endif  
 
 #ifdef debug_uart
    
  UART2_init(115200);
    
 #endif   
   
#if defined(RX_DSMX_2048) || defined(RX_DSM2_1024)    
		rx_spektrum_bind(); 
#endif
	
	
	delay(100000);
		
	i2c_init();	
	
	pwm_init();

	pwm_set( MOTOR_BL , 0);
	pwm_set( MOTOR_FL , 0);	 
	pwm_set( MOTOR_FR , 0); 
	pwm_set( MOTOR_BR , 0); 


	sixaxis_init();
	
	if ( sixaxis_check() ) 
	{
		
	}
	else 
	{
        //gyro not found   
		failloop(4);
	}

    
 #ifdef ENABLE_BARO
    barometer_init();
    if (barometer_check())
    {
    }
    else
    {
        //barometer not found
        failloop(9);
    }
#endif 
    
    
    
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

#ifdef RX_SBUS_DSMX_BAYANG_SWITCH
    if(rx_switch == 0)
    {
        rx_init();
        pFun =checkrx;
    }
    else if(rx_switch == 1)
    {
        SET_RX_PSW(1);
        sbus_rx_init();
        sbus_dsmx_flag = 1;
        pFun = sbus_checkrx;
    }
    else if(rx_switch == 2)
    {
        SET_RX_PSW(0);
        sbus_rx_init();
        sbus_dsmx_flag = 1;
        pFun = sbus_checkrx;
    }
    else if(rx_switch == 3)    //DSMX 2048
    {
        SET_RX_PSW(0);
        dsmx_rx_init();
        sbus_dsmx_flag = 0;
        pFun = dsmx_checkrx;
    }
#else
	rx_init();
    pFun =checkrx;
#endif
    
#ifdef USE_ANALOG_AUX
  // saves initial pid values - after flash loading
  pid_init();
#endif
int sa_cnt=0;
extern  unsigned char channel;
    
while(sa_cnt <5000)
{
    osd_data[0] =0xAA;
    osd_data[1] = 0x55;
    osd_data[2] = 0x07;
    osd_data[3] = 0x01;
    osd_data[4] = channel;
    osd_data[5] = CRC8(osd_data,5);
    osd_data[6] = 0;
    osd_data[7] = 0;
    osd_data[8] = 0;
    osd_data[9] = 0;
    osd_data[10] = 0;
    osd_data[11] = 0;
    
    UART2_DMA_Send();
    
    sa_cnt ++;
}

int count = 0;
	
while ( count < 5000 )
{
	float bootadc = adc_read(0)*vreffilt;
	lpf ( &vreffilt , adc_read(1)  , 0.9968f);
	lpf ( &vbattfilt , bootadc , 0.9968f);
	count++;
}

#ifndef LIPO_CELL_COUNT
for ( int i = 6 ; i > 0 ; i--)
{
		float cells = i;
		if (vbattfilt/cells > 3.7f)
		{	
			lipo_cell_count = cells;
			break;
		}
}
#else
		lipo_cell_count = (float)LIPO_CELL_COUNT;
#endif
	
#ifdef RX_BAYANG_BLE_APP
   // for randomising MAC adddress of ble app - this will make the int = raw float value        
    random_seed =  *(int *)&vbattfilt ; 
    random_seed = random_seed&0xff;
#endif
	
#ifdef STOP_LOWBATTERY
// infinite loop
if ( vbattfilt/lipo_cell_count < 3.3f) failloop(2);
#endif

    IIRFilter_Init();

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

	while(1)
	{ 
		// gettime() needs to be called at least once per second 
		unsigned long time = gettime(); 
		looptime = ((uint32_t)( time - lastlooptime));
		if ( looptime <= 0 ) looptime = 1;
		looptime = looptime * 1e-6f;
		if ( looptime > 0.02f ) // max loop 20ms
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
       
        
    #ifdef ENABLE_BARO 
        altitude_read(); 
    #endif 
        
// battery low logic

        // read acd and scale based on processor voltage
		float battadc = adc_read(0)*vreffilt; 
        
    #ifdef CURR_ADC
        electricCur = adc_read(2);
    #endif
        
        // read and filter internal reference
        lpf ( &vreffilt , adc_read(1)  , 0.9968f);	
  
		

		// average of all 4 motor thrusts
		// should be proportional with battery current			
		extern float thrsum; // from control.c
	
		// filter motorpwm so it has the same delay as the filtered voltage
		// ( or they can use a single filter)		
		lpf ( &thrfilt , thrsum , 0.9968f);	// 0.5 sec at 1.6ms loop time	

        float vbattfilt_corr = 4.2f * lipo_cell_count;
        // li-ion battery model compensation time decay ( 18 seconds )
        lpf ( &vbattfilt_corr , vbattfilt , FILTERCALC( 1000 , 18000e3) );
	
        lpf ( &vbattfilt , battadc , 0.9968f);


// compensation factor for li-ion internal model
// zero to bypass
#define CF1 0.25f

        tempvolt = vbattfilt*( 1.00f + CF1 )  - vbattfilt_corr* ( CF1 );

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

    
    if ( lowbatt ) hyst = HYST;
    else hyst = 0.0f;

    if (( tempvolt + (float) VDROP_FACTOR * thrfilt <(float) VBATTLOW + hyst )
        || ( vbattfilt < ( float ) 2.7f ) )
        lowbatt = 1;
    else lowbatt = 0;

    vbatt_comp = tempvolt + (float) VDROP_FACTOR * thrfilt; 	

           

if ( LED_NUMBER > 0)
{
// led flash logic	
    if ( lowbatt )
        ledflash ( 500000 , 8);
    else
    {
        if ( rxmode == RXMODE_BIND)
        {// bind mode
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
	pFun();
        
#ifdef Lite_OSD   

    osd_count ++;

    vol = (int)(vbattfilt*100);
    
#ifdef CURR_ADC
    cur = (int)(electricCur*10);
#endif

#ifndef f042_1s_bayang
    if(aux[ARMING] && showcase == 0)
    {
        if(osd_count == 400)
        {
            osd_data[0] = 0x0f;
            osd_data[0] |=0 << 4;
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
    else
    {
#endif
        osd_setting();
      
    #ifdef f042_1s_bayang
        if(tx_config && showcase ==0)
        {
            if(aux7 !=aux[RACEMODE]){
                aux7 = aux[RACEMODE];                
                channel++; 
                if(channel>15) channel=8;                   
                osd_data[0] = 0xAA;
                osd_data[1] = 0x55;
                osd_data[2] = 0x07;
                osd_data[3] = 0x01;
                osd_data[4] = channel;
                osd_data[5] = CRC8(osd_data,5);
                osd_data[6] = 0;                     
                UART2_DMA_Send();
            }
        }
        else{
            aux7 = aux[RACEMODE];
        }
    #endif
        
    #ifdef BRUSHLESS_TARGET 
         pwm_count ++;        
         if(pwm_count ==30)
         {
             if(showcase==0){                
                 if(((motorDir[0]==0)&&(motorDir[1]==0)&&(motorDir[2]==0)&&(motorDir[3]==0)) || ((motorDir[0]==1)&&(motorDir[1]==1)&&(motorDir[2]==1)&&(motorDir[3]==1)))
                 {     
                     
                 }
                 else{
                     motorDir[0]=0;
                     motorDir[1]=0;
                     motorDir[2]=0;
                     motorDir[3]=0;
                 }                
             }
            if (aux[LEVELMODE])
            {
                for(int i=20;i>0;i--)
                {
                  motor_dir(0,(motorDir[0] ? DSHOT_CMD_ROTATE_REVERSE : DSHOT_CMD_ROTATE_NORMAL));
                  motor_dir(1,(motorDir[1] ? DSHOT_CMD_ROTATE_REVERSE : DSHOT_CMD_ROTATE_NORMAL));
                  motor_dir(2,(motorDir[2] ? DSHOT_CMD_ROTATE_REVERSE : DSHOT_CMD_ROTATE_NORMAL));
                  motor_dir(3,(motorDir[3] ? DSHOT_CMD_ROTATE_REVERSE : DSHOT_CMD_ROTATE_NORMAL));
                }

            }
            else
            {
                if (!aux[RACEMODE])
                {
                    for(int i=20;i>0;i--)
                    {
                      motor_dir(0,(motorDir[0] ? DSHOT_CMD_ROTATE_REVERSE : DSHOT_CMD_ROTATE_NORMAL));
                      motor_dir(1,(motorDir[1] ? DSHOT_CMD_ROTATE_REVERSE : DSHOT_CMD_ROTATE_NORMAL));
                      motor_dir(2,(motorDir[2] ? DSHOT_CMD_ROTATE_REVERSE : DSHOT_CMD_ROTATE_NORMAL));
                      motor_dir(3,(motorDir[3] ? DSHOT_CMD_ROTATE_REVERSE : DSHOT_CMD_ROTATE_NORMAL));
                    }
                }
                else if(aux[RACEMODE])
                {
                    for(int i=20;i>0;i--)
                    {
                      motor_dir(0,(motorDir[0] ? DSHOT_CMD_ROTATE_NORMAL : DSHOT_CMD_ROTATE_REVERSE));
                      motor_dir(1,(motorDir[1] ? DSHOT_CMD_ROTATE_NORMAL : DSHOT_CMD_ROTATE_REVERSE));
                      motor_dir(2,(motorDir[2] ? DSHOT_CMD_ROTATE_NORMAL : DSHOT_CMD_ROTATE_REVERSE));
                      motor_dir(3,(motorDir[3] ? DSHOT_CMD_ROTATE_NORMAL : DSHOT_CMD_ROTATE_REVERSE));
                    }
                }
                        
            }
            pwm_count = 0;
        }
    #endif
#ifndef f042_1s_bayang    
    }
#endif
	
#endif 
    
    
#ifdef DEBUG
	debug.cpu_load = (gettime() - lastlooptime )*1e-3f;
#endif

while ( (gettime() - time) < LOOPTIME );	

		
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



