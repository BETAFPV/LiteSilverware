

#ifndef __DRV_SBUS_H__
#define __DRV_SBUS_H__

#include "stm32f0xx.h"
#include "stdbool.h"



#define SERIAL_RX_PIN GPIO_Pin_7
#define SERIAL_RX_PORT GPIOB
#define SERIAL_RX_SOURCE GPIO_PinSource7
#define SERIAL_RX_CHANNEL GPIO_AF_0

typedef struct
{
    int16_t roll;
    int16_t pitch;
    int16_t yaw;
    int16_t throttle;
    int16_t aux1;
    int16_t aux2;
    int16_t aux3;
    int16_t aux4;
    int16_t aux5;
    int16_t aux6;
    int16_t aux7;
    int16_t aux8;
    int16_t aux9;
    int16_t aux10;
    int16_t aux11;
    int16_t aux12;
} RCDATA_t;

typedef void (*RcDataCallback)(RCDATA_t data);

void sbus_init(void);
void sbus_check(void);

#endif


