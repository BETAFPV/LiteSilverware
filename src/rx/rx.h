#pragma once

#include "stm32f0xx.h"


void rx_init(void);


//void rxUpdate(uint16_t period);



void flymodeUpdate(uint16_t period);



typedef void rxUpdate(void);


extern rxUpdate *rxCheck;

