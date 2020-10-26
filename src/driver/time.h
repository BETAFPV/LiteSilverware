#include "stm32f0xx.h"



#define TICK_PER_SECOND 1000
#define TICK_US         1000

extern volatile uint32_t sysTickUptime;

static inline int32_t cmpTimeUs(uint32_t a, uint32_t b)
{
  return (int32_t)(a - b);
}

uint32_t gettime(void);

void sysTick_init(void);

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

uint32_t millis(void);

void delay(uint32_t data);

void setclock(void);




