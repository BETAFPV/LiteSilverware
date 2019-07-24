
#include "config.h"


#ifdef Lite_Frsky
//LEDS
#define LED_NUMBER 1
#define LED1PIN GPIO_Pin_0
#define LED1PORT GPIOF
#define LED2PIN GPIO_Pin_0
#define LED2PORT GPIOA

//SOFT I2C & GYRO
#define USE_HARDWARE_I2C
#define GYRO_ID_1 0x68
#define GYRO_ID_2 0x98 // new id
#define GYRO_ID_3 0x78
#define GYRO_ID_4 0x72 
#define SENSOR_ROTATE_90_CCW

// SPI PINS DEFINITONS & RADIO
#if defined(RX_SBUS) || defined(RX_DSMX_2048) || defined(RX_DSM2_1024) || defined(RX_CRSF) 
#define SERIAL_RX_SPEKBIND_BINDTOOL_PIN GPIO_Pin_2
#define SERIAL_RX_SPEKBIND_RX_PIN GPIO_Pin_3
#define SERIAL_RX_PIN GPIO_Pin_3
#define SERIAL_RX_PORT GPIOA
#define SERIAL_RX_SOURCE GPIO_PinSource3
#define SERIAL_RX_CHANNEL GPIO_AF_1
#define SOFTSPI_NONE
//dummy spi placeholders
#define SPI_MOSI_PIN GPIO_Pin_x
#define SPI_MOSI_PORT GPIOA
#define SPI_CLK_PIN GPIO_Pin_y
#define SPI_CLK_PORT GPIOA
#define SPI_SS_PIN GPIO_Pin_z
#define SPI_SS_PORT GPIOA
#define RADIO_CHECK
#else
#define SOFTSPI_3WIRE
#define SPI_MOSI_PIN GPIO_Pin_3
#define SPI_MOSI_PORT GPIOA
#define SPI_CLK_PIN GPIO_Pin_2
#define SPI_CLK_PORT GPIOA
#define SPI_SS_PIN GPIO_Pin_1
#define SPI_SS_PORT GPIOA
#define RADIO_CHECK
#define RADIO_XN297L
#endif

//VOLTAGE DIVIDER
#define BATTERYPIN GPIO_Pin_5
#define BATTERYPORT GPIOA
#define BATTERY_ADC_CHANNEL ADC_Channel_5
//#define ADC_SCALEFACTOR 0.002423  //old value
#ifndef VOLTAGE_DIVIDER_R1
#define VOLTAGE_DIVIDER_R1 2000
#endif
#ifndef VOLTAGE_DIVIDER_R2
#define VOLTAGE_DIVIDER_R2 1000
#endif
#ifndef ADC_REF_VOLTAGE
#define ADC_REF_VOLTAGE 3.3
#endif

// MOTOR PINS
#define MOTOR0_PIN_PA7
//#define MOTOR1_PIN_PA4  //2nd Draft prototype patch
//#define MOTOR2_PIN_PB1  //2nd Draft prototype patch
#define MOTOR1_PIN_PB1
#define MOTOR2_PIN_PA4
#define MOTOR3_PIN_PA6
#endif



#ifdef Lite_Brushless
#define BRUSHLESS_TARGET
#define Lite_OSD
//LEDS
#define LED_NUMBER 1
#define LED1PIN GPIO_Pin_1
#define LED1PORT GPIOA
#define LED2PIN GPIO_Pin_0
#define LED2PORT GPIOA

//SOFT I2C & GYRO
#define USE_HARDWARE_I2C
#define GYRO_ID_1 0x68
#define GYRO_ID_2 0x98 // common h8 gyro
#define GYRO_ID_3 0x7D
#define GYRO_ID_4 0x72
#define SENSOR_FLIP_180

// SPI PINS DEFINITONS & RADIO

#if defined(RX_SBUS) || defined(RX_DSMX_2048) || defined(RX_DSM2_1024) || defined(RX_SBUS_DSMX_BAYANG_SWITCH)
#define SERIAL_RX_SPEKBIND_BINDTOOL_PIN GPIO_Pin_2
#define SERIAL_RX_PIN GPIO_Pin_3
#define SERIAL_RX_PORT GPIOA
#define SERIAL_RX_SOURCE GPIO_PinSource3
#define SERIAL_RX_CHANNEL GPIO_AF_1
//#define SOFTSPI_NONE
//dummy spi placeholders
#define SPI_MOSI_PIN GPIO_Pin_0
#define SPI_MOSI_PORT GPIOA
#define SPI_CLK_PIN GPIO_Pin_1
#define SPI_CLK_PORT GPIOF
#define SPI_SS_PIN GPIO_Pin_0
#define SPI_SS_PORT GPIOF
#define RADIO_CHECK
#define SOFTSPI_3WIRE
#define RADIO_XN297L
#else
#define SOFTSPI_3WIRE
#define SPI_MOSI_PIN GPIO_Pin_0
#define SPI_MOSI_PORT GPIOA
#define SPI_CLK_PIN GPIO_Pin_1
#define SPI_CLK_PORT GPIOF
#define SPI_SS_PIN GPIO_Pin_0
#define SPI_SS_PORT GPIOF
#define RADIO_CHECK
#define RADIO_XN297L
#endif

//VOLTAGE DIVIDER
#define BATTERYPIN GPIO_Pin_5
#define BATTERYPORT GPIOA
#define BATTERY_ADC_CHANNEL ADC_Channel_5

#ifndef VOLTAGE_DIVIDER_R1
#define VOLTAGE_DIVIDER_R1 1630
#endif
#ifndef VOLTAGE_DIVIDER_R2
#define VOLTAGE_DIVIDER_R2 1000
#endif
#ifndef ADC_REF_VOLTAGE
#define ADC_REF_VOLTAGE 3.3
#endif

// Assingment of pin to motor
#define MOTOR0_PIN_PA6 // motor 0 back-left
#define MOTOR1_PIN_PA4 // motor 1 front-left
#define MOTOR2_PIN_PA7 // motor 2 back-right
#define MOTOR3_PIN_PB1 // motor 3 front-right
#endif
