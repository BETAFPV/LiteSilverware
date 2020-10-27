#include "stm32f0xx.h"




#define UART2_TX_PIN        GPIO_Pin_2
#define UART2_RX_PIN        GPIO_Pin_3
#define UART2_PORT          GPIOA
#define UART2_CHANNEL       GPIO_AF_1
#define UART2_TX_SOURCE     GPIO_PinSource2
#define UART2_RX_SOURCE     GPIO_PinSource3
#define UART_TX_DMA





#define SPI_MOSI_PIN GPIO_Pin_0
#define SPI_MOSI_PORT GPIOA
#define SPI_CLK_PIN GPIO_Pin_4
#define SPI_CLK_PORT GPIOB
#define SPI_SS_PIN GPIO_Pin_5
#define SPI_SS_PORT GPIOB



#define PWM_PA7_PIN GPIO_Pin_7
#define PWM_PA7_PORT GPIOA
#define PWM_PA7_AF GPIO_AF_1
#define PWM_PA7_PINSOURCE GPIO_PinSource7
#define PWM_PA7_TIMER TIM3
#define PWM_PA7_CHANNEL CH2
#ifndef ENABLE_TIM3
#define ENABLE_TIM3
#endif

#define PWM_PA15_PIN GPIO_Pin_15
#define PWM_PA15_PORT GPIOA
#define PWM_PA15_AF GPIO_AF_2
#define PWM_PA15_PINSOURCE GPIO_PinSource15
#define PWM_PA15_TIMER TIM2
#define PWM_PA15_CHANNEL CH1
#ifndef ENABLE_TIM2
#define ENABLE_TIM2
#endif

#define PWM_PB3_PIN GPIO_Pin_3
#define PWM_PB3_PORT GPIOB
#define PWM_PB3_AF GPIO_AF_2
#define PWM_PB3_PINSOURCE GPIO_PinSource3
#define PWM_PB3_TIMER TIM2
#define PWM_PB3_CHANNEL CH2
#ifndef ENABLE_TIM2
#define ENABLE_TIM2
#endif

#define PWM_PB1_PIN GPIO_Pin_1
#define PWM_PB1_PORT GPIOB
#define PWM_PB1_AF GPIO_AF_1
#define PWM_PB1_PINSOURCE GPIO_PinSource1
#define PWM_PB1_TIMER TIM3
#define PWM_PB1_CHANNEL CH4
#ifndef ENABLE_TIM3
#define ENABLE_TIM3
#endif


#define BATTERYPIN GPIO_Pin_6
#define BATTERYPORT GPIOA
#define BATTERY_ADC_CHANNEL ADC_Channel_6

#define VOLTAGE_DIVIDER_R1 100000
#define VOLTAGE_DIVIDER_R2 10000
#define ADC_REF_VOLTAGE 3.3










