#include "stm32f0xx.h"





#define ENABLE_OVERCLOCK

#ifdef ENABLE_OVERCLOCK
#define SYS_CLOCK_FREQ_HZ 64000000
#else
#define SYS_CLOCK_FREQ_HZ 48000000
#endif




#define UART_TX_DMA




#define PWM_DIVIDER 1
#define PWMFREQ 32000
#define PWMTOP (( SYS_CLOCK_FREQ_HZ / PWMFREQ ) - 1)












