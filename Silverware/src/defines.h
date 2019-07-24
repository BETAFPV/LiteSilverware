
#include "hardware.h"

// gcc warnings in main.c

// defines for things that do not normally need changing

#define MOTOR_BL 0
#define MOTOR_FL 1
#define MOTOR_FR 3
#define MOTOR_BR 2

#define PIDNUMBER 3

#define DEGTORAD 0.017453292f
#define RADTODEG 57.29577951f

#define AUXNUMBER 16

#define ROLL 0
#define PITCH 1
#define YAW 2

// this should be precalculated by the compiler as it's a constant
#define FILTERCALC( sampleperiod, filtertime) (1.0f - ( 6.0f*(float)sampleperiod) / ( 3.0f *(float)sampleperiod + (float)filtertime))


#define RXMODE_BIND 0
#define RXMODE_NORMAL (!RXMODE_BIND)

/*
#define CH_ON 4
#define CH_OFF 5
#define CH_FLIP 0
#define CH_EXPERT 1
#define CH_HEADFREE 2
#define CH_RTH 3
*/

// defines for bayang protocol radio
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

// defines for cg023 protocol
#define CH_CG023_LED 3
#define CH_CG023_FLIP 0
#define CH_CG023_STILL 2
#define CH_CG023_VIDEO 1

#define CH_H7_FLIP 0
#define CH_H7_VIDEO 1
#define CH_H7_FS 2

#define CH_CX10_CH0 0
#define CH_CX10_CH2 2

#define CH_AUX3 CH_OFF
#define CH_AUX4 CH_OFF

#ifdef USE_DEVO
// devo tx channel mapping
// also for nr24multipro
#define CHAN_5 CH_INV
#define CHAN_6 CH_FLIP
#define CHAN_7 CH_PIC
#define CHAN_8 CH_VID
#define CHAN_9 CH_HEADFREE
#define CHAN_10 CH_RTH
#define CHAN_11 CH_TO
#define CHAN_12 CH_EMG
#define CHAN_13 CH_ANA_AUX1
#define CHAN_14 CH_ANA_AUX2
#define CHAN_ON CH_ON
#define CHAN_OFF CH_OFF
#endif

#ifdef USE_MULTI
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
#endif

#ifdef USE_STOCK_TX
#define CHAN_5 CH_EXPERT
#define CHAN_6 CH_AUX1
#define CHAN_7 CH_HEADFREE
#define CHAN_8 CH_RLL_TRIM
#define CHAN_9 CH_PIT_TRIM
#define CHAN_10 CH_OFF
#define CHAN_ON CH_ON
#define CHAN_OFF CH_OFF
#endif


#ifdef BETAFLIGHT_RATES
#define ACRO_EXPO_ROLL BF_EXPO_ROLL
#define ACRO_EXPO_PITCH BF_EXPO_PITCH
#define ACRO_EXPO_YAW BF_EXPO_YAW
#define ANGLE_EXPO_ROLL BF_EXPO_ROLL
#define ANGLE_EXPO_PITCH BF_EXPO_PITCH
#define ANGLE_EXPO_YAW BF_EXPO_YAW
#define MAX_RATE 200*(float)BF_RC_RATE_ROLL*(1/1-(float)BF_SUPER_RATE_ROLL)     // roll max rate used for flip sequencer when bf rates selected
#endif


// used for the pwm driver
#define CH1 0
#define CH2 1
#define CH3 2
#define CH4 3


#define int32 int_fast32_t
#define int16 int_fast16_t
#define int8 int_fast8_t

#define uint32 uint_fast32_t
#define uint16 uint_fast16_t
#define uint8 uint_fast8_t

// for h-bridge states
#define FREE 2
#define BRAKE 3
#define DIR1 1
#define DIR2 0

// for inverted flight motor direction
#define FORWARD DIR2
#define REVERSE DIR1


//things moved from old config.h below

#ifdef LVC_LOWER_THROTTLE
#define SWITCHABLE_FEATURE_2
#endif

#ifdef INVERT_YAW_PID
#define SWITCHABLE_FEATURE_3
#endif

#ifdef ALIENWHOOP_ZERO_FILTERING
#define KALMAN_GYRO
#define GYRO_FILTER_PASS1 HZ_90
#define  DTERM_LPF_2ND_HZ 100
#define MOTOR_FILTER2_ALPHA MFILT1_HZ_50
#define SWITCHABLE_MOTOR_FILTER2_ALPHA MFILT1_HZ_90
#define SWITCHABLE_FEATURE_1
#endif

#ifdef WEAK_FILTERING
#define KALMAN_GYRO
#define GYRO_FILTER_PASS1 HZ_90
#define  DTERM_LPF_2ND_HZ 100
#define MOTOR_FILTER2_ALPHA MFILT1_HZ_90
#endif

#ifdef STRONG_FILTERING
#define KALMAN_GYRO
#define GYRO_FILTER_PASS1 HZ_80
#define  DTERM_LPF_2ND_HZ 90
#define MOTOR_FILTER2_ALPHA MFILT1_HZ_80
#endif

#ifdef VERY_STRONG_FILTERING
#define KALMAN_GYRO
#define GYRO_FILTER_PASS1 HZ_70
#define  DTERM_LPF_2ND_HZ 80
#define MOTOR_FILTER2_ALPHA MFILT1_HZ_70
#endif

#ifdef BETA_FILTERING
	#if (!defined(KALMAN_GYRO) && !defined(PT1_GYRO)) || (!defined(GYRO_FILTER_PASS1) && !defined(GYRO_FILTER_PASS2))
		#define SOFT_LPF_NONE
	#endif
#endif

#define GYRO_LOW_PASS_FILTER 0

#define DISABLE_FLIP_SEQUENCER  //**************todo adapt into turtle mode
#define STARTFLIP CHAN_OFF     //**************todo adapt into turtle mode


// *************motor curve to use - select one
// *************the pwm frequency has to be set independently
#define MOTOR_CURVE_NONE

// loop time in uS
// this affects soft gyro lpf frequency if used
#define LOOPTIME 1000

// failsafe time in uS
#define FAILSAFETIME 1000000  // one second


// enable motors if pitch / roll controls off center (at zero throttle)
// possible values: 0 / 1
// use in acro build only
#define ENABLESTIX 0
#define ENABLESTIX_TRESHOLD 0.3
#define ENABLESTIX_TIMEOUT 1e6


// define logic - do not change
///////////////

#pragma diag_warning 1035 , 177 , 4017
#pragma diag_error 260

//--fpmode=fast

// overclock to 64Mhz

#define ENABLE_OVERCLOCK

// used for pwm calculations
#ifdef ENABLE_OVERCLOCK
#define SYS_CLOCK_FREQ_HZ 64000000
#else
#define SYS_CLOCK_FREQ_HZ 48000000
#endif

#ifdef MOTOR_BEEPS
#ifdef USE_ESC_DRIVER
#warning "MOTOR BEEPS_WORKS WITH BRUSHED MOTORS ONLY"
#endif
#endif

// for the ble beacon to work after in-flight reset
#ifdef RX_BAYANG_PROTOCOL_BLE_BEACON
#undef STOP_LOWBATTERY
#endif


// The following define can always be left uncommented. It just gathers all analog aux PID settings together into one define.
#if defined USE_ANALOG_AUX && (defined ANALOG_R_P || defined ANALOG_R_I || defined ANALOG_R_D || defined ANALOG_P_P || defined ANALOG_P_I || defined ANALOG_P_D || defined ANALOG_Y_P || defined ANALOG_Y_I || defined ANALOG_Y_D || defined ANALOG_RP_P || defined ANALOG_RP_I || defined ANALOG_RP_D || defined ANALOG_RP_PD)
    #define ANALOG_AUX_PIDS
#endif

// *************flash save method
// *************flash_save 1: pids + accel calibration
// *************flash_save 2: accel calibration to option bytes
#define FLASH_SAVE1
//#define FLASH_SAVE2


// *************compensation for battery voltage vs throttle drop
#define VDROP_FACTOR 0.7
// *************calculate above factor automatically
#define AUTO_VDROP_FACTOR

// *************voltage hysteresis in volts
#define HYST 0.10

// *************MIXER SETTINGS & LIMITS


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

//
// filter settings (must be defined after filter type selection - KALMAN_GYRO or PT1_GYRO)
//

#ifdef KALMAN_GYRO
// kalman Q/R ratio for Q = 0.02
// loop time 1000Hz
#define	HZ_10	0.004078
#define	HZ_20	0.015952
#define	HZ_30	0.035546
#define	HZ_40	0.062984
#define	HZ_50	0.097857
#define	HZ_60	0.139957
#define	HZ_70	0.190992
#define	HZ_80	0.249072
#define	HZ_90	0.308894
#define	HZ_100	0.397188
#define	HZ_120	0.542488
#define	HZ_140	0.719026
#define	HZ_160	0.928746
#define	HZ_180	1.144837
#define	HZ_200	1.354386
#define	HZ_220	1.611742
#define	HZ_240	1.87532
#define	HZ_260	2.123421
#define	HZ_280	2.377006
#define	HZ_300	2.595641
#define	HZ_320	2.864404
#define	HZ_340	3.052077
#define	HZ_360	3.272997
#define	HZ_380	3.44942
#define	HZ_400	3.679173
#define	HZ_420	3.721861
#define	HZ_440	3.880844
#define	HZ_460	3.908564
#define	HZ_480	3.984022
#define	HZ_500	4.100000
#endif

#ifdef PT1_GYRO
#define	HZ_10	10
#define	HZ_20	20
#define	HZ_30	30
#define	HZ_40	40
#define	HZ_50	50
#define	HZ_60	60
#define	HZ_70	70
#define	HZ_80	80
#define	HZ_90	90
#define	HZ_100	100
#define	HZ_120	120
#define	HZ_140	140
#define	HZ_160	160
#define	HZ_180	180
#define	HZ_200	200
#define	HZ_220	220
#define	HZ_240	240
#define	HZ_260	260
#define	HZ_280	280
#define	HZ_300	300
#define	HZ_320	320
#define	HZ_340	340
#define	HZ_360	360
#define	HZ_380	380
#define	HZ_400	400
#define	HZ_420	420
#define	HZ_440	440
#define	HZ_460	460
#define	HZ_480	480
#define	HZ_500	500
#endif
// 1st order lpf alpha
// for 1000Hz loop frequency
#define	MFILT1_HZ_10	0.056677
#define	MFILT1_HZ_20	0.109243
#define	MFILT1_HZ_30	0.15976
#define	MFILT1_HZ_40	0.207311
#define	MFILT1_HZ_50	0.250878
#define	MFILT1_HZ_60	0.292612
#define	MFILT1_HZ_70	0.331242
#define	MFILT1_HZ_80	0.366444
#define	MFILT1_HZ_90	0.406108
#define	MFILT1_HZ_100	0.434536
#define	MFILT1_HZ_120	0.49997
#define	MFILT1_HZ_140	0.543307
#define	MFILT1_HZ_160	0.582436
#define	MFILT1_HZ_180	0.631047
#define	MFILT1_HZ_200	0.67169
#define	MFILT1_HZ_220	0.697849
#define	MFILT1_HZ_240	0.714375
#define	MFILT1_HZ_260	0.725199
#define	MFILT1_HZ_280	0.740312
#define	MFILT1_HZ_300	0.758612
#define	MFILT1_HZ_320	0.773861
#define	MFILT1_HZ_340	0.79364
#define	MFILT1_HZ_360	0.803003
#define	MFILT1_HZ_380	0.809752
#define	MFILT1_HZ_400	0.817944
#define	MFILT1_HZ_420	0.81943
#define	MFILT1_HZ_440	0.824737
#define	MFILT1_HZ_460	0.825618
#define	MFILT1_HZ_480	0.827956
#define	MFILT1_HZ_500	0.836544


#if defined MOTOR_MIN_COMMAND && defined (BRUSHLESS_TARGET)
	#define IDLE_OFFSET  (MOTOR_MIN_COMMAND*10)  //dshot has 2000 steps
#endif
#if defined MOTOR_MIN_COMMAND2 && defined (BRUSHLESS_TARGET)
	#define IDLE_OFFSET  (MOTOR_MIN_COMMAND2*10)  //dshot has 2000 steps
#endif
#if defined MOTOR_MIN_COMMAND3 && defined (BRUSHLESS_TARGET)
	#define IDLE_OFFSET  (MOTOR_MIN_COMMAND3*10)  //dshot has 2000 steps
#endif


