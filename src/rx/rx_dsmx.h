#include "stm32f0xx.h"
#include "stdbool.h"



#define SERIAL_RX_PIN GPIO_Pin_7
#define SERIAL_RX_PORT GPIOB
#define SERIAL_RX_SOURCE GPIO_PinSource7
#define SERIAL_RX_CHANNEL GPIO_AF_0



#define DSMX_SERIAL_BAUDRATE 115200


#define DSM_SCALE_PERCENT 150                                               //adjust this line to match the stick scaling % set in your transmitter
#define SPEK_FRAME_SIZE 16
#define SPEKTRUM_NEEDED_FRAME_INTERVAL  5000
#define SPEKTRUM_MAX_FADE_PER_SEC       40
#define SPEKTRUM_FADE_REPORTS_PER_SEC   2
#define SPEKTRUM_MAX_SUPPORTED_CHANNEL_COUNT 12
#define SPEKTRUM_2048_CHANNEL_COUNT     12
#define SPEKTRUM_1024_CHANNEL_COUNT     7


#define PIN_OFF( port , pin ) GPIO_ResetBits( port , pin)
#define PIN_ON( port , pin ) GPIO_SetBits( port , pin)

void dsm_init(void);

void dsm_bind(void);

void dsm_check(void);


