#include "serial_uart.h"
#include "defines.h"
#include "hardware.h"

unsigned char osd_data[12];



void uart_init(int BAUDRATE)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = UART2_TX_PIN;


    GPIO_Init(UART2_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(UART2_PORT, UART2_TX_SOURCE, UART2_CHANNEL);

    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);
    USART_HalfDuplexCmd(USART2, ENABLE);

    USART_Cmd(USART2, ENABLE);
    USART_ClearFlag(USART2, USART_FLAG_TC);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


#ifdef UART_TX_DMA
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel4);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->TDR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)osd_data;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 12;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
    DMA_ClearFlag(DMA1_FLAG_TC4);
    DMA_Cmd(DMA1_Channel4, DISABLE);

    /* enable DMA1 Channel4 transfer complete interrupt */
    NVIC_InitStructure.NVIC_IRQChannel =            DMA1_Channel4_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = (uint8_t)DMA_Priority_VeryHigh;
    NVIC_InitStructure.NVIC_IRQChannelCmd =         ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
}


void UART2_DMA_Send(void)
{
    DMA_Cmd(DMA1_Channel4, DISABLE);

    DMA_SetCurrDataCounter(DMA1_Channel4, 12); //send DataNumber
    DMA_Cmd(DMA1_Channel4, ENABLE);

}

void DMA1_Channel4_5_IRQHandler(void)
{
    if (DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET)
    {
        DMA_ClearFlag(DMA1_FLAG_TC4);
    }
}


void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
    if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) == SET)
    {
        USART_ClearFlag(USART2, USART_FLAG_ORE);
        USART_ReceiveData(USART2);
    }

}



