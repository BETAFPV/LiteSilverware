#include "rx_sbus.h"
#include "time.h"
#include "defines.h"
#include "config.h"


#ifdef USE_RX_SBUS

extern int rxmode;

int failsafe=1;

struct sbus_dat {
    uint32_t start : 8;
    uint32_t chan1 : 11;
    uint32_t chan2 : 11;
    uint32_t chan3 : 11;
    uint32_t chan4 : 11;
    uint32_t chan5 : 11;
    uint32_t chan6 : 11;
    uint32_t chan7 : 11;
    uint32_t chan8 : 11;
    uint32_t chan9 : 11;
    uint32_t chan10 : 11;
    uint32_t chan11 : 11;
    uint32_t chan12 : 11;
    uint32_t chan13 : 11;
    uint32_t chan14 : 11;
    uint32_t chan15 : 11;
    uint32_t chan16 : 11;
} __attribute__ ((__packed__));

union 
{
	uint8_t  raw[25];
    struct sbus_dat msg;
}sbus;

RCDATA_t sbusData;

extern int ledcommand;
extern float rx[];
extern char aux[];
extern uint16_t chan[4];
extern int rx_ready;

uint8_t flag=12;

void sbus_init(void)
{
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

    USART_InitStructure.USART_BaudRate = 100000;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_2;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    USART_InitStructure.USART_Mode = USART_Mode_Rx ;//USART_Mode_Rx ;

    USART_Init(USART1, &USART_InitStructure);


    USART_InvPinCmd(USART1, USART_InvPin_Rx, ENABLE ); //USART_InvPin_Tx


    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART1, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    rxmode = !RXMODE_BIND;
}




//void sbus_check(void)
//{
//    bool signalReceived = false;

//    if (!sbusFrameData.done)
//    {
//        return ;
//    }
//    sbusFrameData.done = false;


//    const uint8_t frameStatus = sbusChannelsDecode(&sbusFrameData.frame.frame.channels);


////    if (!(frameStatus & (RX_FRAME_FAILSAFE | RX_FRAME_DROPPED))) {
////        lastRcFrameTimeUs = sbusTimeLast;
////    }

//    rxIsInFailsafeMode = (frameStatus & RX_FRAME_FAILSAFE) != 0;
//    bool rxFrameDropped = (frameStatus & RX_FRAME_DROPPED) != 0;
//    signalReceived = !(rxIsInFailsafeMode || rxFrameDropped);

//    if (signalReceived)
//    {

//        rx[0] = (val[0] - 993) * 0.00122026f;
//        rx[1] = (val[1] - 993) * 0.00122026f;
//        rx[2] = (val[3] - 993) * 0.00122026f;

//        rx[3] = (val[2] - 173) * 0.000610128f;

//        if (rx[3] > 1) rx[3] = 1;
//        if (rx[3] < 0) rx[3] = 0;

//        chan[0] = val[4];
//        chan[1] = val[5];
//        chan[2] = val[6];
//        chan[3] = val[7];

//        aux[CHAN_5] = (val[4] > 500) ? ((val[4] < 1500) ? 1 : 2) : 0;
//        aux[CHAN_6] = (val[5] > 500) ? ((val[5] < 1500) ? 1 : 2) : 0;
//        aux[CHAN_7] = (val[6] > 500) ? ((val[6] < 1500) ? 1 : 2) : 0;
//        aux[CHAN_8] = (val[7] > 500) ? ((val[7] < 1500) ? 1 : 2) : 0;


//        bind_safety++;
//        if (bind_safety > 100)
//        {
//            rx_ready = 1;
//            failsafe = 0;
//            rxmode = !RXMODE_BIND;
//            bind_safety = 101;
//            if(ledcommand != 5)
//                ledcommand = 4;
//        }
//        time_siglost = gettime();
//        failsafe_siglost = 0;
//    }
//    else
//    {
//        if (gettime() - time_siglost > 1000000)
//        {
//            failsafe_siglost = 1;

//            rx[0] = 0;
//            rx[1] = 0;
//            rx[2] = 0;
//            rx[3] = 0;

//            if(ledcommand !=2)
//                ledcommand = 3;
//        }

//        failsafe = failsafe_siglost;

//    }
//}


void sbus_check()
{
    if(flag ==12)
    {
        rxmode = 0;
        failsafe = 1;
        if(ledcommand !=2)
            ledcommand = 3;
    }
    else
    {
        rxmode = 1;
        failsafe = 0;
        rx_ready =1;
        if(ledcommand != 5)
            ledcommand = 4;
    }
    //一帧数据解析完成
    rx[0] = (sbusData.roll - 1500) * 0.002f;
    rx[1] = (sbusData.pitch - 1500) * 0.002f;
    rx[2] = (sbusData.yaw - 1500) * 0.002f;

    rx[3] = (sbusData.throttle - 1000) * 0.001f;

    if (rx[3] > 1) rx[3] = 1;
    if (rx[3] < 0) rx[3] = 0;

    chan[0] = sbusData.aux1;
    chan[1] = sbusData.aux2;
    chan[2] = sbusData.aux3;
    chan[3] = sbusData.aux4;

    aux[CHAN_5] = (sbusData.aux1 > 1000) ? ((sbusData.aux1 < 1600) ? 1 : 2) : 0;
    aux[CHAN_6] = (sbusData.aux2 > 1000) ? ((sbusData.aux2 < 1600) ? 1 : 2) : 0;
    aux[CHAN_7] = (sbusData.aux3 > 1000) ? ((sbusData.aux3 < 1600) ? 1 : 2) : 0;
    aux[CHAN_8] = (sbusData.aux4 > 1000) ? ((sbusData.aux4 < 1600) ? 1 : 2) : 0;
}


/**********************************************************************************************************
*函 数 名: Sbus_Decode
*功能说明: sbus协议解析
*形    参: 输入数据
*返 回 值: 无
**********************************************************************************************************/
static void Sbus_Decode(uint8_t data)
{  
    static uint32_t lastTime;
    static uint32_t dataCnt = 0;
    
    if(millis() < 2000)
        return;
    
    uint32_t deltaT = millis() - lastTime;
    lastTime = millis();
    
    //数据间隔大于3ms则可以认为新的一帧开始了
    if(deltaT > 3)
    {
        dataCnt = 0;
    }
    
    //接收数据
    sbus.raw[dataCnt++] = data;    
    
    //每帧数据长度为25
    if(dataCnt == 25)
    {
        //判断帧头帧尾是否正确
        if(sbus.raw[0] != 0x0F || sbus.raw[24] != 0)
            return;
        
        //每个通道数据占据11个字节，这里使用了字节对齐的方式来进行解析
        //转换摇杆数据量程为[1000:2000]
        sbusData.roll     = sbus.msg.chan1 * 0.625f + 880;  
        sbusData.pitch    = sbus.msg.chan2 * 0.625f + 880;
        sbusData.throttle = sbus.msg.chan3 * 0.625f + 880;  
        sbusData.yaw      = sbus.msg.chan4 * 0.625f + 880;  
        sbusData.aux1     = sbus.msg.chan5 * 0.625f + 880;  
        sbusData.aux2     = sbus.msg.chan6 * 0.625f + 880;  
        sbusData.aux3     = sbus.msg.chan7 * 0.625f + 880;  
        sbusData.aux4     = sbus.msg.chan8 * 0.625f + 880;  
        sbusData.aux5     = sbus.msg.chan9 * 0.625f + 880;  
        sbusData.aux6     = sbus.msg.chan10 * 0.625f + 880;  
        sbusData.aux7     = sbus.msg.chan11 * 0.625f + 880;  
        sbusData.aux8     = sbus.msg.chan12 * 0.625f + 880;  
        sbusData.aux8     = sbus.msg.chan13 * 0.625f + 880;  
        sbusData.aux10    = sbus.msg.chan14 * 0.625f + 880;  
        sbusData.aux11    = sbus.msg.chan15 * 0.625f + 880;  
        sbusData.aux12    = sbus.msg.chan16 * 0.625f + 880;        
        
        flag = sbus.raw[23];
        

    }
}

//Receive ISR callback
void USART1_IRQHandler(void)
{
    uint8_t com_data;
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET) 
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE); 
        
        com_data = USART_ReceiveData(USART1);
        Sbus_Decode(com_data);
	}

}


#endif









