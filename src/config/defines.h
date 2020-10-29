#include "stm32f0xx.h"





#define ENABLE_OVERCLOCK

#ifdef ENABLE_OVERCLOCK
#define SYS_CLOCK_FREQ_HZ 64000000
#else
#define SYS_CLOCK_FREQ_HZ 48000000
#endif


#define LOOPTIME 1000

// failsafe time in uS
#define FAILSAFETIME 1000000  // one second

#define UART_TX_DMA

#define RADIO_XN297L
#define RADIO_CHECK

#define PWM_DIVIDER 1
#define PWMFREQ 32000
#define PWMTOP (( SYS_CLOCK_FREQ_HZ / PWMFREQ ) - 1)

#define DEGTORAD 0.017453292f
#define RADTODEG 57.29577951f

#define RXMODE_BIND 0
#define RXMODE_NORMAL (!RXMODE_BIND)

#define CH_ON (AUXNUMBER - 2)
#define CH_OFF (AUXNUMBER - 1)
#define CH_FLIP 0
#define CH_EXPERT 1
#define CH_HEADFREE 2
#define CH_RTH 3
#define CH_AUX1 4
#define CH_AUX2 5
#define CH_EMG 10
#define CH_TO 11
#define CH_ANA_AUX1 12
#define CH_ANA_AUX2 13
// trims numbers have to be sequential, start at CH_PIT_TRIM
#define CH_PIT_TRIM 6
#define CH_RLL_TRIM 7
#define CH_THR_TRIM 8
#define CH_YAW_TRIM 9
// next 3 channels only when *not* using USE_STOCK_TX
#define CH_INV 6
#define CH_VID 7
#define CH_PIC 8

// multimodule mapping ( taranis )
#define CHAN_5 CH_FLIP
#define CHAN_6 CH_RTH
#define CHAN_7 CH_PIC
#define CHAN_8 CH_VID
#define CHAN_9 CH_HEADFREE
#define CHAN_10 CH_INV
#define CHAN_12 CH_TO
#define CHAN_13 CH_EMG
#define CHAN_14 CH_ANA_AUX1
#define CHAN_15 CH_ANA_AUX2
#define CHAN_ON CH_ON
#define CHAN_OFF CH_OFF

#define ARMING CHAN_5
#define IDLE_UP CHAN_5
#define LEVELMODE CHAN_6
#define RACEMODE  CHAN_7
#define HORIZON   CHAN_8
#define PIDPROFILE CHAN_9                //For switching stickAccelerator & stickTransition profiles on pid.c page
#define RATES CHAN_ON
#define LEDS_ON CHAN_10

#define Roll     0
#define Pitch    1
#define Yaw      2
#define Throttle 3
#define ROLL 0
#define PITCH 1
#define YAW 2


#define MAX_RATE 860.0          //Roll & Pitch axis
#define MAX_RATEYAW 500.0       //Yaw axis (used in acro and leveling modes)

// *************EXPO from 0.00 to 1.00 , 0 = no exp
// *************positive = less sensitive near center
#define ACRO_EXPO_ROLL 0.80
#define ACRO_EXPO_PITCH 0.80
#define ACRO_EXPO_YAW 0.60

#define ANGLE_EXPO_ROLL 0.55
#define ANGLE_EXPO_PITCH 0.0
#define ANGLE_EXPO_YAW 0.55

// *************max angle for level mode
#define LEVEL_MAX_ANGLE 65.0f

// ************* low rates multiplier if rates are assigned to a channel
#define LOW_RATES_MULTI 0.5f

// *************transmitter stick adjustable deadband for roll/pitch/yaw
// *************.01f = 1% of stick range - comment out to disable
#define STICKS_DEADBAND .01f

//**************joelucid's yaw fix
#define YAW_FIX

//**************joelucid's transient windup protection.  Removes roll and pitch bounce back after flips
#define TRANSIENT_WINDUP_PROTECTION


//enables use of stick accelerator and stick transition for d term lpf 1 & 2
#define ADVANCED_PID_CONTROLLER

//Throttle must drop below this value if arming feature is enabled for arming to take place.  MIX_INCREASE_THROTTLE_3 if enabled
//will also not activate on the ground untill this threshold is passed during takeoff for safety and better staging behavior.
#define THROTTLE_SAFETY .10f

//Activating this setting makes the accelerometer less prone to drift or yaw slow down in angle mode but more likely to become confused in crashes or impacts.  Only use if necessary
//#define ACCELEROMETER_DRIFT_FIX

// level mode "manual" trims ( in degrees)
// pitch positive forward
// roll positive right
#define TRIM_PITCH 0.0
#define TRIM_ROLL 0.0


#define ENABLESTIX 0
#define ENABLESTIX_TRESHOLD 0.3
#define ENABLESTIX_TIMEOUT 1e6



#ifdef BRUSHLESS_TARGET
#define BRUSHLESS_MIX_SCALING
#else
#define MIX_LOWER_THROTTLE
#define MIX_THROTTLE_REDUCTION_PERCENT 10
//#define MIX_INCREASE_THROTTLE

//#define MIX_LOWER_THROTTLE_3
#define MIX_INCREASE_THROTTLE_3
#define MIX_THROTTLE_INCREASE_MAX 0.2f
#endif



#define PID_VOLTAGE_COMPENSATION
#define LEVELMODE_PID_ATTENUATION 0.90f  //used to prevent oscillations in angle modes with pid_voltage_compensation enabled due to high pids

// *************lower throttle when battery below threshold - forced landing low voltage cutoff
// *************THIS FEATURE WILL BE OFF BY DEFAULT EVEN WHEN DEFINED - USE STICK GESTURE LEFT-LEFT-LEFT TO ACTIVATE THEN DOWN-DOWN-DOWN TO SAVE AS ON
// *************Led light will blink once when LVC forced landing is turned on, blink twice when turned off, and will blink multiple times upon save command
// *************Enter values in volts per cell
#define LVC_LOWER_THROTTLE
#define LVC_LOWER_THROTTLE_VOLTAGE 3.30
#define LVC_LOWER_THROTTLE_VOLTAGE_RAW 2.70
#define LVC_LOWER_THROTTLE_KP 3.0

// *************do not start software if battery is too low (below 3.3v per cell) - only works on 1s lipos
// *************flashes 2 times repeatedly at startup
#define STOP_LOWBATTERY

// *************voltage per cell to start warning led blinking
#define VBATTLOW 3.1

#define FILTERCALC( sampleperiod, filtertime) (1.0f - ( 6.0f*(float)sampleperiod) / ( 3.0f *(float)sampleperiod + (float)filtertime))


#ifdef LVC_LOWER_THROTTLE
#define SWITCHABLE_FEATURE_2
#endif


#define MOTOR_BL 0
#define MOTOR_FL 1
#define MOTOR_FR 3
#define MOTOR_BR 2

#define MOTOR_MIN_COMMAND  10.0


#define PIDNUMBER 3

#define AUXNUMBER 16


#define FLASH_SAVE1
//#define FLASH_SAVE2


// *************compensation for battery voltage vs throttle drop
#define VDROP_FACTOR 0.7
// *************calculate above factor automatically
#define AUTO_VDROP_FACTOR

// *************voltage hysteresis in volts
#define HYST 0.10


#define RGB_LED_NUMBER 2
#define RGB_LED_DMA
// pin / port for the RGB led ( programming port ok )
#define RGB_PIN GPIO_Pin_0
#define RGB_PORT GPIOB










