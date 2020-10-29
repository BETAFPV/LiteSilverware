#include "misc_gpio.h"
#include "time.h"
#include "xn297.h"
#include "rx_bayang.h"
#include "defines.h"


static int key_cnt = 0;
extern int rx_ready;
extern int ledcommand;
extern int rxmode;


void gpio_init(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOFEN, ENABLE);


    //blue led
    GPIO_InitTypeDef  GPIO_InitStructure;


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_1);

}



void keyUpdate(uint16_t period)
{
    static uint32_t LastRunTime=0;
    if((sysTickUptime-LastRunTime)<period)return;
    LastRunTime=sysTickUptime;


    if ((GPIOA->IDR & GPIO_Pin_1) == (uint32_t)Bit_RESET)
    {
        delay_ms(10);
        while ((GPIOA->IDR & GPIO_Pin_1) == (uint32_t)Bit_RESET)
        {
            key_cnt ++;
            delay_ms(1);
        }
        if (key_cnt > 100)
        {
            rx_ready = 0;
            rxmode = RXMODE_BIND;
            ledcommand = 2;
            key_cnt = 0;


            static uint8_t rxaddr[6] = { 0x2a, 0, 0, 0, 0, 0  };
            writeregs( rxaddr, sizeof(rxaddr) );
            xn_writereg(RF_CH, 0);

        }
    }
}







