// serial for stm32 not used yet
#include "project.h"
#include "stm32f0xx_usart.h"
#include <stdio.h>
#include "drv_serial.h"
#include "defines.h"
#include "drv_time.h"

// enable serial driver ( pin SWCLK after calibration) 
// WILL DISABLE PROGRAMMING AFTER GYRO CALIBRATION - 2 - 3 seconds after powerup)

// this has to be in config.h
//#define SERIAL_ENABLE

extern unsigned char osd_data[12];

#ifdef ENABLE_UART1_TX 
int fputc( int ch, FILE *f )
{
    USART_SendData(USART1,ch );
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
    return ch;
}
#else
#ifdef debug_uart
int fputc( int ch, FILE *f )
{
    USART_SendData(USART2,ch );
    while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
    return ch;
}
#endif
#endif
#define SERIAL_BUFFER_SIZE 64

#ifdef ENABLE_UART2
void UART2_init(unsigned int BAUDRATE)
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
    GPIO_PinAFConfig(UART2_PORT, UART2_TX_SOURCE , UART2_CHANNEL);
    
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
    DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
    USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE); 
    DMA_ClearFlag(DMA1_FLAG_TC4);    
    DMA_Cmd(DMA1_Channel4, DISABLE);
		
    /* enable DMA1 Channel4 transfer complete interrupt */ 
    NVIC_InitStructure.NVIC_IRQChannel = 			DMA1_Channel4_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 	(uint8_t)DMA_Priority_VeryHigh;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 		ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif


}

#ifdef UART_TX_DMA
void UART2_DMA_Send(void) 
{ 
	DMA_Cmd(DMA1_Channel4, DISABLE ); 	

    DMA_SetCurrDataCounter(DMA1_Channel4,12);//send DataNumber
    DMA_Cmd(DMA1_Channel4, ENABLE);

}

void DMA1_Channel4_5_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET)
	{ 
        DMA_ClearFlag(DMA1_FLAG_TC4);
	}
}
#endif


void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET) 
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE); 
	}
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET) 
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);
		USART_ReceiveData(USART2);
	}

}

#endif
#define SERIAL_BAUDRATE 115200
//#define SERIAL_ENABLE
#ifdef SERIAL_ENABLE

uint8_t buffer[SERIAL_BUFFER_SIZE];
char buffer_start = 0;
char buffer_end = 0;


void USART1_IRQHandler(void)
{
	if (buffer_end != buffer_start)
	  {
		  USART_SendData(USART1, buffer[buffer_start]);
		  buffer_start++;
		  buffer_start = buffer_start % (SERIAL_BUFFER_SIZE);
	  }
	else
	  {
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	  }
}

void serial_init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource14 , GPIO_AF_1);

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
  
  USART_InitStructure.USART_BaudRate = SERIAL_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx;//USART_Mode_Rx | USART_Mode_Tx;
	
  USART_Init(USART1, &USART_InitStructure);

//	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


}


int fputc(int ch, FILE * f)
{			
		buffer[buffer_end] = (char)ch;
		buffer_end++;
		buffer_end = buffer_end % (SERIAL_BUFFER_SIZE);

		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	return ch;
}

void buffer_add(int val )
{
	buffer[buffer_end] = (char)val;
	buffer_end++;
	buffer_end = buffer_end % (SERIAL_BUFFER_SIZE);

	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	return;
}

#else
// serial disabled - dummy functions
void serial_init(void)
{
	
}

void buffer_add(int val )
{
	
}

#endif



