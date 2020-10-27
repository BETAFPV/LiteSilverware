#include "time.h"
#include "binary.h"
#include "defines.h"


void setclock(void)
{
    // turn on HSI
    RCC->CR |= 1;
    // wait for HSI ready
    while ((RCC->CR & RCC_CR_HSIRDY) == 0)
    {
    }
    // set clock source HSI
    RCC->CFGR &= (0xFFFFFFFC);
    // wait for switch
    while ((RCC->CFGR & B00001100))
    {

    }
    // turn pll off
    RCC->CR &= (uint32_t)0xFEFFFFFF;

    while ((RCC->CR & RCC_CR_PLLRDY) != 0)
    {
    }

    //reset pll settings
    RCC->CFGR &= (uint32_t)0xFFC0FFFF;

#ifdef ENABLE_OVERCLOCK
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL);
    RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLMULL16;
#else
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL);
    RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLMULL12;
#endif

    //PLL on
    RCC->CR |= RCC_CR_PLLON;

    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
    //set PLL as source
//  RCC->CFGR&=(0xFFFFFF|B11111110);
    RCC->CFGR |= (B00000010);

//  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
//  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    //wait until PLL is the clock
#ifdef ENABLE_OVERCLOCK
    // only if overclocked as it is redundant
    while ((RCC->CFGR & B00001100) != B00001000)
    {
    }
#endif
}


volatile uint32_t sysTickUptime = 0;
volatile uint32_t sysTickValStamp = 0;



void sysTick_init()
{
    if (SysTick_Config(SYS_CLOCK_FREQ_HZ / 1000))
    {
        /* Capture error */
        while (1);
    }
    NVIC_SetPriority(SysTick_IRQn, 0x0);//SysTick中断优先级设置
}

void SysTick_Handler(void)
{
    sysTickUptime++;
    sysTickValStamp = SysTick->VAL;
}


uint32_t millis(void)
{
    return sysTickUptime;
}


uint32_t gettime(void)
{
    register uint32_t ms, cycle_cnt;

    do
    {
        ms = sysTickUptime;
        cycle_cnt = SysTick->VAL;
    }
    while (ms != sysTickUptime || cycle_cnt > sysTickValStamp);

    return (ms * TICK_US + (SysTick->LOAD - SysTick->VAL) * TICK_US / SysTick->LOAD);

}


void delay_us(uint32_t us)
{
    uint64_t now = gettime();
    while ((gettime() - now) < us);
}


void delay_ms(uint32_t ms)
{
    delay_us(ms * 1000);
}

void delay(uint32_t data)
{
    volatile uint32_t count;
    count = data * 7;
    while (count--);
}

















