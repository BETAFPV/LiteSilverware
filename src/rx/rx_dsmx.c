#include "rx_dsmx.h"
#include "config.h"
#include "defines.h"
#include "time.h"

#ifdef USE_RX_DSMX


#define RX_DSMX_2048

#ifdef RX_DSMX_2048
#define CHANNEL_COUNT 10
#define BIND_PULSES 9
// 11 bit frames
static uint8_t spek_chan_shift = 3;
static uint8_t spek_chan_mask = 0x07;
#endif

#ifdef RX_DSM2_1024
#define CHANNEL_COUNT 7
#define BIND_PULSES 3
// 10 bit frames
static uint8_t spek_chan_shift = 2;
static uint8_t spek_chan_mask = 0x03;
#endif


extern float rx[4];
extern char aux[16];
extern char lastaux[16];
extern char auxchange[16];
extern int ledcommand;
int failsafe=1;
extern int rxmode;
extern int rx_ready;
extern int bind_safety;
extern int rx_bind_enable ;

uint32_t channels[CHANNEL_COUNT];
int rcFrameComplete = 0;
int framestarted=-1;
int rx_frame_pending;
int rx_frame_pending_last;
uint32_t flagged_time;
volatile uint8_t spekFrame[SPEK_FRAME_SIZE];
float dsm2_scalefactor = (0.29354210f / DSM_SCALE_PERCENT);
float dsmx_scalefactor = (0.14662756f / DSM_SCALE_PERCENT);

uint8_t spekFramePosition = 0;
uint32_t spekTimeLast = 0;
uint32_t spekTime = 0;
uint32_t spekTimeInterval = 0;

extern uint16_t chan[4];
unsigned int dsmtime;
unsigned int lastdsmtime;


void dsm_init(void)
{
    // make sure there is some time to program the board
    if ( gettime() < 2000000 ) return;
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = SERIAL_RX_PIN ;
    GPIO_Init(SERIAL_RX_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(SERIAL_RX_PORT, SERIAL_RX_SOURCE, SERIAL_RX_CHANNEL);

    GPIO_InitStructure.GPIO_Pin = SERIAL_RX_PIN;
    GPIO_Init(SERIAL_RX_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(SERIAL_RX_PORT, SERIAL_RX_SOURCE, SERIAL_RX_CHANNEL);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = DSMX_SERIAL_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;    //sbus is even parity
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    USART_InitStructure.USART_Mode = USART_Mode_Rx ;//USART_Mode_Rx ;

    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    framestarted = 0;
}

void dsm_bind(void)
{
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SERIAL_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(SERIAL_RX_PORT, &GPIO_InitStructure);

    // RX line, set high
    PIN_ON(SERIAL_RX_PORT, SERIAL_RX_PIN);
    // Bind window is around 20-140ms after powerup
    delay(60000);

    for (uint8_t i = 0; i < BIND_PULSES; i++) { // 9 pulses for internal dsmx 11ms, 3 pulses for internal dsm2 22ms
        // RX line, drive low for 120us
        PIN_OFF(SERIAL_RX_PORT, SERIAL_RX_PIN);
        delay(120);

        // RX line, drive high for 120us
        PIN_ON(SERIAL_RX_PORT, SERIAL_RX_PIN);
        delay(120);
    }
}



void spektrumFrameStatus(void)
{
    if (rcFrameComplete == 0)
    {
        rx_frame_pending = 1;
    }
    else
    {
        rcFrameComplete = 0;

        for (int b = 3; b < SPEK_FRAME_SIZE; b += 2)
        {
            const uint8_t spekChannel = 0x0F & (spekFrame[b - 1] >> spek_chan_shift);
            if (spekChannel < CHANNEL_COUNT && spekChannel < SPEKTRUM_MAX_SUPPORTED_CHANNEL_COUNT)
            {
                channels[spekChannel] = ((uint32_t)(spekFrame[b - 1] & spek_chan_mask) << 8) + spekFrame[b];
                framestarted = 1;
                rx_frame_pending = 0;
                bind_safety++;

            }
        }
    }
}


void dsm_check(void)
{
    unsigned long time = gettime();
    dsmtime = ((uint32_t)(time - lastdsmtime));

    lastdsmtime = time;

    if (framestarted < 0)
    {
        failsafe = 1;
        dsm_init();
        rxmode = !RXMODE_BIND;
        ledcommand = 2;
    }

    if (framestarted == 0)
    {
        failsafe = 1;
        if(ledcommand !=2)
            ledcommand = 3;
    }

    rx_frame_pending_last = rx_frame_pending;
    spektrumFrameStatus();

    if (rx_frame_pending != rx_frame_pending_last) flagged_time = gettime();
    if (gettime() - flagged_time > FAILSAFETIME) framestarted = 0;

    if ((bind_safety < 100) && (bind_safety > 0)) rxmode = RXMODE_BIND;

    // TAER channel order
#ifdef RX_DSMX_2048
    rx[0] = (channels[1] - 1024.0f) * dsmx_scalefactor;
    rx[1] = (channels[2] - 1024.0f) * dsmx_scalefactor;
    rx[2] = (channels[3] - 1024.0f) * dsmx_scalefactor;
    rx[3] = ((channels[0] - 1024.0f) * dsmx_scalefactor * 0.5f) + 0.5f;

    if (rx[3] > 1) rx[3] = 1;
    if (rx[3] < 0) rx[3] = 0;
#endif


#ifdef RX_DSM2_1024
    rx[0] = (channels[1] - 512.0f) * dsm2_scalefactor;
    rx[1] = (channels[2] - 512.0f) * dsm2_scalefactor;
    rx[2] = (channels[3] - 512.0f) * dsm2_scalefactor;
    rx[3] = ((channels[0] - 512.0f) * dsm2_scalefactor * 0.5f) + 0.5f;

    if (rx[3] > 1) rx[3] = 1;
    if (rx[3] < 0) rx[3] = 0;
#endif


#ifdef RX_DSMX_2048

    chan[0] = channels[4];
    chan[1] = channels[5];
    chan[2] = channels[6];
    chan[3] = channels[7];

    aux[CHAN_5] = (channels[4] > 1100) ? 1 : 0;
    aux[CHAN_6] = (channels[5] > 1100) ? 1 : 0;
    aux[CHAN_7] = (channels[6] > 1100) ? 1 : 0;
    aux[CHAN_8] = (channels[7] > 1100) ? 1 : 0;
    aux[CHAN_9] = (channels[8] > 1100) ? 1 : 0;
    aux[CHAN_10] = (channels[9] > 1100) ? 1 : 0;
#endif

#ifdef RX_DSM2_1024
    aux[CHAN_5] = (channels[4] > 550) ? 1 : 0;
    aux[CHAN_6] = (channels[5] > 550) ? 1 : 0;
    aux[CHAN_7] = (channels[6] > 550) ? 1 : 0;
#endif

    if (bind_safety > 100)
    {
        rx_ready = 1;
        failsafe = 0;
        rxmode = !RXMODE_BIND;
        bind_safety = 101;
        if(ledcommand != 5)
            ledcommand = 4;
    }
}


void UART1_IRQHandler(void)
{

    if (USART_GetFlagStatus(USART1, USART_IT_RXNE)  != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);

        spekTime = gettime();
        spekTimeInterval = cmpTimeUs(spekTime, spekTimeLast);
        spekTimeLast = spekTime;

        if (spekTimeInterval > SPEKTRUM_NEEDED_FRAME_INTERVAL)
        {
            spekFramePosition = 0;
        }

        if (spekFramePosition < SPEK_FRAME_SIZE)
        {
            spekFrame[spekFramePosition++] = USART_ReceiveData(USART1);;
            if (spekFramePosition < SPEK_FRAME_SIZE)
            {
                rcFrameComplete = 0;
            }
            else
            {
                rcFrameComplete = 1;
            }
        }

        spekFramePosition %= (SPEK_FRAME_SIZE);

    }
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)
    {
        USART_ClearFlag(USART2,USART_FLAG_ORE);
    }
}
#endif













