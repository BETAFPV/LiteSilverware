
#include "targets.h"


// HARDWARE PINS SETTING
//


// do not change hardware pins below
// make sure you don't set SWDIO or SWDCLK pins (programming pins)
// if you do, you lose board programmability without a reset pin
//
// example: pin "PB2" ( port b , pin 2 )
// pin: GPIO_Pin_2
// port: GPIOB


// setting procedure:
// set led number zero, led aux number zero
// uncomment  DISABLE_SPI_PINS and DISABLE_PWM_PINS
// this will prevent any pins to be used, so that there are no conflicts
// set pins starting with leds, in order to see working status
// do not set PA13 , PA14 (stm32f031) as this will break the programming interface
// to disable led pins set number to zero

// the error codes indicate a failure that prevents normal operation
// led flash codes - the quad will not fly / bind if flashing a code
// 2 - low battery at powerup - if enabled by config.h "#define STOP_LOWBATTERY" 
// 3 - radio chip not found
// 4 - Gyro not found - maybe i2c speed
// 5 - clock , intterrupts , systick , gcc bad code , bad memory access (code issues like bad pointers)- this should not come up
// 6 - loop time issue - if loop time exceeds 20mS
// 7 - i2c error  - triggered by hardware i2c driver only
// 8 - i2c error main loop  - triggered by hardware i2c driver only




// i2c driver to use ( dummy - disables i2c )
// hardware i2c used PB6 and 7 by default ( can also use PA9 and 10)
//#define USE_HARDWARE_I2C
//#define USE_SOFTWARE_I2C
//#define USE_DUMMY_I2C


// I2C speed: fast = no delays 
// slow1 = for i2c without pull-up resistors
// slow2 = i2c failsafe speed
#define SOFTI2C_SPEED_FAST
//#define SOFTI2C_SPEED_SLOW1
//#define SOFTI2C_SPEED_SLOW2


// hardware i2c speed ( 1000, 400 , 200 , 100Khz)
#define HW_I2C_SPEED_FAST2
//#define HW_I2C_SPEED_FAST
//#define HW_I2C_SPEED_SLOW1
//#define HW_I2C_SPEED_SLOW2


// pins for hw i2c , select one only
// select pins PB6 and PB7 OR select pins PA9 and PA10
//#define HW_I2C_PINS_PB67
//#define HW_I2C_PINS_PA910
#define HW_I2C_PINS_PF01



// pins for software i2c
#define SOFTI2C_SDAPIN GPIO_Pin_10
#define SOFTI2C_SDAPORT GPIOA
#define SOFTI2C_SCLPIN GPIO_Pin_9
#define SOFTI2C_SCLPORT GPIOA
#define SOFTI2C_GYRO_ADDRESS 0x68


// disable the check for known gyro that causes the 4 times flash
//#define DISABLE_GYRO_CHECK

// TODO: gyro ids for the gyro check
//move to sixaxis & defines.h


// disable lvc functions
//#define DISABLE_LVC


// pwm driver = brushed motors
// esc driver = servo type signal for brushless esc
// pins PA0 - PA11 , PB0 , PB1

#ifdef BRUSHLESS_CONVERSION
	#define BRUSHLESS_TARGET
#endif

//**DO NOT ENABLE ESC DRIVER WITH BRUSHED MOTORS ATTACHED**

#ifdef BRUSHLESS_TARGET
	#define USE_DSHOT_DMA_DRIVER
	//#define USE_DSHOT_DRIVER_BETA
	//#define USE_ESC_DRIVER
#else
	#define USE_PWM_DRIVER
#endif

// baro type
#define USE_BARO_DPS310

// baro I2C address
#define DPS310_I2C_ADDRESS 0x76


//FC must have MOSFETS and motor pulldown resistors removed. MAY NOT WORK WITH ALL ESCS
//#define USE_SERIAL_4WAY_BLHELI_INTERFACE
		
		
// pwm pins disable
// disable all pwm pins / function
//#define DISABLE_PWM_PINS

// pwm pin initialization
// enable the pwm pins to be used here ( multiple pins ok)
//#define PWM_PA0
//#define PWM_PA1
//#define PWM_PA2
//#define PWM_PA3
//#define PWM_PA4
//#define PWM_PA5
//#define PWM_PA6
//#define PWM_PA7
//#define PWM_PA8
//#define PWM_PA9
//#define PWM_PA10
//#define PWM_PA11
//#define PWM_PA15
//#define PWM_PB0
//#define PWM_PB1
//#define PWM_PB3
//#define PWM_PB4
//#define PWM_PB5
//#define PWM_PB6
//#define PWM_PB7

#ifdef Lite_BrushlessRX
#define PWM_PB0
#define PWM_PB1
#define PWM_PB4
#define PWM_PA15
#endif

#ifdef Lite_BrushedRX
#define PWM_PB1
#define PWM_PB3
#define PWM_PA7
#define PWM_PA15
#endif



// pin for fpv switch ( turns off at failsafe )
// GPIO_Pin_13 // SWDAT - GPIO_Pin_14 // SWCLK  
// if programming pin, will not flash after bind
#define FPV_PIN GPIO_Pin_13
#define FPV_PORT GPIOA 


// BUZZER pin settings - buzzer active "high"
// SWDAT and SWCLK pins OK here
// GPIO_Pin_13 // SWDAT - GPIO_Pin_14 // SWCLK 
#define BUZZER_PIN       GPIO_Pin_14
#define BUZZER_PIN_PORT  GPIOA
// x (micro)seconds after loss of tx or low bat before buzzer starts
#define BUZZER_DELAY     30e6 
