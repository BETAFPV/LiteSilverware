#include "rx_sbus.h"
#include "time.h"
#include "defines.h"
#include "config.h"


#ifdef USE_RX_SBUS

extern int rxmode;

#define RX_BUFF_SIZE 26                         //SPEK_FRAME_SIZE 16  
uint8_t rx_buffer[RX_BUFF_SIZE];    //spekFrame[SPEK_FRAME_SIZE]
uint8_t rx_start = 0;
uint8_t rx_end = 0;
uint16_t rx_time[RX_BUFF_SIZE];

int framestarted = -1;
uint8_t framestart = 0;

int failsafe=1;
extern int rxmode;
extern int rx_ready;
extern int ledcommand;
unsigned long time_lastrx;
unsigned long time_siglost = 0;
uint8_t last_rx_end = 0;
int last_byte = 0;
unsigned long time_lastframe;
int frame_received = 0;
int rx_state = 0;
extern int bind_safety;
uint8_t data[25];
int channels[9];

sbusFrameData_t sbusFrameData;
static bool rxIsInFailsafeMode = true;
extern char aux[16];
extern float rx[4];
extern uint16_t chan[4];

#define SBUS_TIME_NEEDED_PER_FRAME    3000
#define SBUS_FRAME_BEGIN_BYTE 0x0F

uint16_t val[18];  // Õ®µ¿÷µ
uint8_t  rc_flag = 0;
int failsafe_siglost = 0;




void sbus_init(void)
{
    // make sure there is some time to program the board
    if ( gettime() < 2000000 ) return;

    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = SERIAL_RX_PIN ;
    GPIO_Init(SERIAL_RX_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(SERIAL_RX_PORT, SERIAL_RX_SOURCE, SERIAL_RX_CHANNEL);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = SERIAL_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_2;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    USART_InitStructure.USART_Mode = USART_Mode_Rx ;//USART_Mode_Rx ;

    USART_Init(USART1, &USART_InitStructure);


    if (SBUS_INVERT) USART_InvPinCmd(USART1, USART_InvPin_Rx, ENABLE ); //USART_InvPin_Tx


    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART1, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    framestarted = 0;
}

uint8_t sbusChannelsDecode(const sbusChannels_t *channels)
{
    val[0] = channels->chan0;
    val[1] = channels->chan1;
    val[2] = channels->chan2;
    val[3] = channels->chan3;
    val[4] = channels->chan4;
    val[5] = channels->chan5;
    val[6] = channels->chan6;
    val[7] = channels->chan7;
    val[8] = channels->chan8;
    val[9] = channels->chan9;
    val[10] = channels->chan10;
    val[11] = channels->chan11;
    val[12] = channels->chan12;
    val[13] = channels->chan13;
    val[14] = channels->chan14;
    val[15] = channels->chan15;

    if (channels->flags & SBUS_FLAG_CHANNEL_17)
    {
        val[16] = SBUS_DIGITAL_CHANNEL_MAX;
    }
    else
    {
        val[16] = SBUS_DIGITAL_CHANNEL_MIN;
    }

    if (channels->flags & SBUS_FLAG_CHANNEL_18)
    {
        val[17] = SBUS_DIGITAL_CHANNEL_MAX;
    }
    else
    {
        val[17] = SBUS_DIGITAL_CHANNEL_MIN;
    }

    if (channels->flags & SBUS_FLAG_FAILSAFE_ACTIVE)
    {
        // internal failsafe enabled and rx failsafe flag set
        // RX *should* still be sending valid channel data (repeated), so use it.
        return RX_FRAME_COMPLETE | RX_FRAME_FAILSAFE;
    }

    if (channels->flags & SBUS_FLAG_SIGNAL_LOSS)
    {
        // The received data is a repeat of the last valid data so can be considered complete.
        return RX_FRAME_COMPLETE | RX_FRAME_DROPPED;
    }

    return RX_FRAME_COMPLETE;
}



void sbus_check(void)
{
    bool signalReceived = false;

    if (!sbusFrameData.done)
    {
        return ;
    }
    sbusFrameData.done = false;


    const uint8_t frameStatus = sbusChannelsDecode(&sbusFrameData.frame.frame.channels);


//    if (!(frameStatus & (RX_FRAME_FAILSAFE | RX_FRAME_DROPPED))) {
//        lastRcFrameTimeUs = sbusTimeLast;
//    }

    rxIsInFailsafeMode = (frameStatus & RX_FRAME_FAILSAFE) != 0;
    bool rxFrameDropped = (frameStatus & RX_FRAME_DROPPED) != 0;
    signalReceived = !(rxIsInFailsafeMode || rxFrameDropped);

    if (signalReceived)
    {

        rx[0] = (val[0] - 993) * 0.00122026f;
        rx[1] = (val[1] - 993) * 0.00122026f;
        rx[2] = (val[3] - 993) * 0.00122026f;

        rx[3] = (val[2] - 173) * 0.000610128f;

        if (rx[3] > 1) rx[3] = 1;
        if (rx[3] < 0) rx[3] = 0;

        chan[0] = val[4];
        chan[1] = val[5];
        chan[2] = val[6];
        chan[3] = val[7];

        aux[CHAN_5] = (val[4] > 500) ? ((val[4] < 1500) ? 1 : 2) : 0;
        aux[CHAN_6] = (val[5] > 500) ? ((val[5] < 1500) ? 1 : 2) : 0;
        aux[CHAN_7] = (val[6] > 500) ? ((val[6] < 1500) ? 1 : 2) : 0;
        aux[CHAN_8] = (val[7] > 500) ? ((val[7] < 1500) ? 1 : 2) : 0;


        bind_safety++;
        if (bind_safety > 100)
        {
            rx_ready = 1;
            failsafe = 0;
            rxmode = !RXMODE_BIND;
            bind_safety = 101;
            if(ledcommand != 5)
                ledcommand = 4;
        }
        time_siglost = gettime();
        failsafe_siglost = 0;
    }
    else
    {
        if (gettime() - time_siglost > 1000000)
        {
            failsafe_siglost = 1;

            rx[0] = 0;
            rx[1] = 0;
            rx[2] = 0;
            rx[3] = 0;

            if(ledcommand !=2)
                ledcommand = 3;
        }

        failsafe = failsafe_siglost;

    }
}


void UART1_IRQHandler(void)
{
    if (USART_GetFlagStatus(USART1, USART_IT_RXNE)  != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);


        uint8_t c =  USART_ReceiveData(USART1);;
        const uint32_t nowUs = gettime();

        const int32_t sbusFrameTime = cmpTimeUs(nowUs, sbusFrameData.startAtUs);


        if (sbusFrameTime > (long)(SBUS_TIME_NEEDED_PER_FRAME + 500))
        {
            sbusFrameData.position = 0;
        }
        if (sbusFrameData.position == 0)
        {
            if (c != SBUS_FRAME_BEGIN_BYTE)
            {
                return;
            }
            sbusFrameData.startAtUs = nowUs;
        }

        if (sbusFrameData.position < SBUS_FRAME_SIZE)
        {
            sbusFrameData.frame.bytes[sbusFrameData.position++] = (uint8_t)c;
            if (sbusFrameData.position < SBUS_FRAME_SIZE)
            {
                sbusFrameData.done = false;
            }
            else
            {
                sbusFrameData.done = true;
            }
        }
    }
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)
    {
        USART_ClearFlag(USART2,USART_FLAG_ORE);
    }

}


#endif









