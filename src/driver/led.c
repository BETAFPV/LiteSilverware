#include "led.h"
#include "defines.h"
#include "time.h"
#include "util.h"

#define RGB_BIT_TIME 		((SYS_CLOCK_FREQ_HZ/1000/800)-1)
#define RGB_T0H_TIME 		(RGB_BIT_TIME*0.30 + 0.05 )
#define RGB_T1H_TIME 		(RGB_BIT_TIME*0.60 + 0.05 )


int rgb_led_value[RGB_LED_NUMBER];
int led_num=0;
volatile int	rgb_dma_phase = 0;
extern int failsafe;

const int offset = RGB_PIN > GPIO_Pin_7;

volatile uint32_t	rgb_data_portA[ RGB_LED_NUMBER*24/4 ] = { 0 };	// DMA buffer: reset output when bit data=0 at TOH timing
volatile uint16_t	rgb_portX[1] = { RGB_PIN };						// sum of all rgb pins at port
volatile uint32_t RGB_DATA16[16];


// normal flight rgb colour - LED switch ON
#define RGB_VALUE_INFLIGHT_ON RGB( 255 , 255 , 255 )

// normal flight rgb colour - LED switch OFF
#define RGB_VALUE_INFLIGHT_OFF RGB( 0 , 0 , 0 )

//  colour before bind
#define RGB_VALUE_BEFORE_BIND RGB( 0 , 128 , 128 )

// fade from one color to another when changed
#define RGB_FILTER_ENABLE
#define RGB_FILTER_TIME_MICROSECONDS 50e3

// runs the update once every 16 loop times ( 16 mS )
#define DOWNSAMPLE 16

#define RGB_FILTER_TIME FILTERCALC( 1000*DOWNSAMPLE , RGB_FILTER_TIME_MICROSECONDS)
#define RGB( r , g , b ) ( ( ((int)g&0xff)<<16)|( ((int)r&0xff)<<8)|( (int)b&0xff ))


float r_filt, g_filt, b_filt;
extern int ledcommand;
extern unsigned char low_vol;
uint8_t rgb_led=0;


// sets all leds to a brightness
void rgb_led_set_all( int rgb )
{
// deconstruct the colour into components
    int g = rgb>>16;
    int r = (rgb&0x0000FF00)>>8;
    int b = rgb & 0xff;

// filter individual colors
    lpf( &r_filt, r, RGB_FILTER_TIME);
    lpf( &g_filt, g, RGB_FILTER_TIME);
    lpf( &b_filt, b, RGB_FILTER_TIME);

    int temp = RGB( r_filt, g_filt, b_filt );

    for ( int i = 0 ; i < RGB_LED_NUMBER ; i++)
        rgb_led_value[i] = temp;
}


void rgb_ledflash( int color1, int color2, uint32_t period, int duty )
{
    if ( gettime() % period > (period*duty)>>4 )
    {
        rgb_led_set_all( color1 );
    }
    else
    {
        rgb_led_set_all( color2 );
    }

}

void rgb_dma_buffer_making()
{
    // generate rgb dma packet
    int j=0;
    for( int n=0; n<RGB_LED_NUMBER; n++ ) {
        rgb_data_portA[ j++ ] = RGB_DATA16[ (rgb_led_value[ n ] >> 20) & 0x0f ];
        rgb_data_portA[ j++ ] = RGB_DATA16[ (rgb_led_value[ n ] >> 16) & 0x0f ];
        rgb_data_portA[ j++ ] = RGB_DATA16[ (rgb_led_value[ n ] >> 12) & 0x0f ];
        rgb_data_portA[ j++ ] = RGB_DATA16[ (rgb_led_value[ n ] >>  8) & 0x0f ];
        rgb_data_portA[ j++ ] = RGB_DATA16[ (rgb_led_value[ n ] >>  4) & 0x0f ];
        rgb_data_portA[ j++ ] = RGB_DATA16[ (rgb_led_value[ n ]      ) & 0x0f ];
    }
}

void rgb_dma_trigger()
{
    TIM1->ARR 	= RGB_BIT_TIME;
    TIM1->CCR1 	= RGB_T0H_TIME;
    TIM1->CCR3 	= RGB_T1H_TIME;

    DMA1_Channel5->CPAR = (uint32_t)&RGB_PORT->BSRR;
    DMA1_Channel5->CMAR = (uint32_t)rgb_portX;
    DMA1_Channel2->CPAR = (uint32_t)&RGB_PORT->BRR+offset;
    DMA1_Channel2->CMAR = (uint32_t)rgb_data_portA;

    DMA1_Channel3->CPAR = (uint32_t)&RGB_PORT->BRR;
    DMA1_Channel3->CMAR = (uint32_t)rgb_portX;

    DMA1_Channel2->CCR &= ~(DMA_CCR_MSIZE_0 | DMA_CCR_MSIZE_1
                            | DMA_CCR_PSIZE_0 | DMA_CCR_PSIZE_1);
    DMA_ClearFlag( DMA1_FLAG_GL2 | DMA1_FLAG_GL3 | DMA1_FLAG_GL5 );
    DMA1_Channel3->CNDTR = RGB_LED_NUMBER*24;

    DMA1_Channel5->CNDTR = RGB_LED_NUMBER*24;
    DMA1_Channel2->CNDTR = RGB_LED_NUMBER*24;


    TIM1->SR = 0;

    DMA_Cmd(DMA1_Channel2, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);

    DMA_Cmd(DMA1_Channel3, ENABLE);
    TIM_DMACmd(TIM1, TIM_DMA_Update | TIM_DMA_CC2 | TIM_DMA_CC1, ENABLE);

    TIM_SetCounter( TIM1, RGB_BIT_TIME );
    TIM_Cmd( TIM1, ENABLE );


}

void rgb_send( int data )
{
    if( !rgb_dma_phase )	rgb_dma_phase = 3;
}


void ledUpdate(uint16_t period)
{

    /*
    上电时电压过低( < 3.3v)，初始不通过：红色 red 闪两次  RGB( 255 , 0 , 0 )

    1初始话完成，飞行过程中电压低(上电时 >3.3v, 飞行过程中 < 3.3v) :红色 red 慢闪 RGB( 255 , 0 , 0 )

    2初始化完成，还没有遥控信息，或者重新进入对频：绿色 green 慢闪烁 RGB( 0,160,0 )
        D16 FCC:
        D16 LBT:
        D8:
        SBUS:
        BAYANG:

    3初始化完成，有遥控信息，但没有信号， 或者飞行过程中信号丢失： 橙色 orange 慢闪  RGB(255,165,0)

    4初始化完成，可以正常飞行状态：设置0：蓝色 blue 常亮 RGB(0,127,255)；设置1：慢闪

    5解锁时，油门不在最低处 ：白色 white RGB(255,255,255) 慢闪

    优先级：

    */


    static uint32_t LastRunTime=0;
    if((sysTickUptime-LastRunTime)<period)return;
    LastRunTime=sysTickUptime;


    if(low_vol)
    {
        rgb_ledflash(RGB( 0, 255, 0 ), RGB( 0, 0, 0 ),500000, 8); //ok
    }
    else
    {
        if(failsafe)
        {
            switch(ledcommand)
            {
            case 2:
                rgb_ledflash ( RGB( 255, 0, 0 ), RGB( 0, 0, 0 ),500000, 8); //ok
                break;


            case 3:
                rgb_ledflash(RGB(255,165,0), RGB( 0, 0, 0 ),500000, 8); //ok
                break;

            }
        }
        else
        {
            switch(ledcommand)
            {
            case 4:
                if(rgb_led)
                {
                    led_num ++;

                    if(led_num <10)
                    {
                        rgb_led_set_all(RGB( 255, 0, 0 ));
                    }
                    else if(led_num <20)
                    {
                        rgb_led_set_all(RGB( 0,160,0 ));
                    }
                    else if(led_num < 30)
                    {
                        rgb_led_set_all(RGB(255,165,0));
                    }
                    else if(led_num<40)
                    {
                        rgb_led_set_all(RGB(0,127,255));
                    }
                    else if(led_num<50)
                    {
                        led_num =0;
                    }
                }
                else {
                    rgb_led_set_all(RGB(0,0,255));
                }
                break;

            case 5:
                rgb_ledflash(RGB( 255, 255, 255 ), RGB( 0, 0, 0 ),500000, 8);
                break;
            }
        }
    }

    rgb_send(0);

    if( rgb_dma_phase <= 1 )
        return;

    if( rgb_dma_phase ==3 ) {
        rgb_dma_buffer_making();
        rgb_dma_phase = 2;
        return;
    }


    rgb_dma_phase = 1;
    rgb_dma_trigger();
}


void led_init(void)
{
    if ( (RGB_PIN == GPIO_Pin_13 || RGB_PIN == GPIO_Pin_14) && RGB_PORT == GPIOA ) {
        // programming port used
        // wait until 2 seconds from powerup passed
        while ( gettime() < 2e6 );
    }

    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = RGB_PIN;
    GPIO_Init( RGB_PORT, &GPIO_InitStructure );


    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_OCStructInit(&TIM_OCInitStructure);
    // TIM1 Periph clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 						RGB_BIT_TIME;
    TIM_TimeBaseStructure.TIM_Prescaler = 				0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 		0;
    TIM_TimeBaseStructure.TIM_CounterMode = 			TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(TIM1, DISABLE);

    /* Timing Mode configuration: Channel 1 */
    TIM_OCInitStructure.TIM_OCMode = 							TIM_OCMode_Timing;
    TIM_OCInitStructure.TIM_OutputState = 				TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = 							RGB_T0H_TIME;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);

    /* Timing Mode configuration: Channel 4 */
    TIM_OCInitStructure.TIM_OCMode = 							TIM_OCMode_Timing;
    TIM_OCInitStructure.TIM_OutputState = 				TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = 							RGB_T1H_TIME;

    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);


    DMA_InitTypeDef DMA_InitStructure;

    DMA_StructInit(&DMA_InitStructure);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* DMA1 Channe5 configuration ----------------------------------------------*/
    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = 		(uint32_t)&RGB_PORT->BSRR;
    DMA_InitStructure.DMA_MemoryBaseAddr = 				(uint32_t)rgb_portX;
    DMA_InitStructure.DMA_DIR = 									DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 						RGB_LED_NUMBER*24;
    DMA_InitStructure.DMA_PeripheralInc = 				DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = 						DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = 		DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = 				DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = 									DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = 							DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = 									DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    /* DMA1 Channel2 configuration ----------------------------------------------*/
    DMA_DeInit(DMA1_Channel2);
    DMA_InitStructure.DMA_PeripheralBaseAddr = 		(uint32_t)&RGB_PORT->BRR+ offset;
    DMA_InitStructure.DMA_MemoryBaseAddr = 				(uint32_t)rgb_data_portA;
    DMA_InitStructure.DMA_DIR = 									DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 						RGB_LED_NUMBER*24;
    DMA_InitStructure.DMA_PeripheralInc = 				DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = 						DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = 		DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = 				DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = 									DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = 							DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = 									DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);

    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = 		(uint32_t)&GPIOA->BRR;
    DMA_InitStructure.DMA_MemoryBaseAddr = 				(uint32_t)rgb_portX;
    DMA_InitStructure.DMA_DIR = 									DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 						16;
    DMA_InitStructure.DMA_PeripheralInc = 				DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = 						DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = 		DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = 				DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = 									DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = 							DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = 									DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    TIM_DMACmd(TIM1, TIM_DMA_Update | TIM_DMA_CC2 | TIM_DMA_CC1, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = 					DMA1_Channel2_3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 	(uint8_t)DMA_Priority_High;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 			ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* enable DMA1 Channel3 transfer complete interrupt */
    DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);

    for (int i=0; i<RGB_LED_NUMBER; i++) {
        rgb_led_value[i]=0;
    }
    if( !rgb_dma_phase )	rgb_dma_phase = 3;

    int pin = rgb_portX[0];

    if ( offset )
    {
        pin >>=8;
    }

    for( int i=0; i<16; i++ ) {
        RGB_DATA16[ i ]  = (i & 0x01) ? 0:pin;
        RGB_DATA16[ i ]<<= 8;
        RGB_DATA16[ i ] |= (i & 0x02) ? 0:pin;
        RGB_DATA16[ i ]<<= 8;
        RGB_DATA16[ i ] |= (i & 0x04) ? 0:pin;
        RGB_DATA16[ i ]<<= 8;
        RGB_DATA16[ i ] |= (i & 0x08) ? 0:pin;
    }


}


void DMA1_Channel2_3_IRQHandler(void)
{
    DMA_Cmd(DMA1_Channel5, DISABLE);
    DMA_Cmd(DMA1_Channel2, DISABLE);
    DMA_Cmd(DMA1_Channel3, DISABLE);

    TIM_DMACmd(TIM1, TIM_DMA_Update | TIM_DMA_CC2 | TIM_DMA_CC1, DISABLE);
    DMA_ClearITPendingBit(DMA1_IT_TC3);

    TIM_Cmd( TIM1, DISABLE );

    if( rgb_dma_phase == 2 )
    {
        rgb_dma_phase = 1;
        rgb_dma_trigger();
    }

    rgb_dma_phase = 0;
}



