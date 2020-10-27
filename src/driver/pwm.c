#include "pwm.h"
#include "defines.h"
#include "hardware.h"
#include  <math.h>



TIM_OCInitTypeDef  TIM_OCInitStructure;


void init_timer(TIM_TypeDef *TIMx, int period)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    TIM_TimeBaseStructure.TIM_Prescaler = PWM_DIVIDER - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = period;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
}


void pwm_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);        // PA0 , PA1

    init_timer(TIM2, PWMTOP);


    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;

    TIM_OCInitStructure.TIM_Pulse = 0;

    GPIO_InitStructure.GPIO_Pin = PWM_PA7_PIN;
    GPIO_Init(PWM_PA7_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(PWM_PA7_PORT, PWM_PA7_PINSOURCE, PWM_PA7_AF);
    TIM_OC2Init(PWM_PA7_TIMER, &TIM_OCInitStructure);


    GPIO_InitStructure.GPIO_Pin = PWM_PA15_PIN;
    GPIO_Init(PWM_PA15_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(PWM_PA15_PORT, PWM_PA15_PINSOURCE, PWM_PA15_AF);
    TIM_OC1Init(PWM_PA15_TIMER, &TIM_OCInitStructure);


    GPIO_InitStructure.GPIO_Pin = PWM_PB1_PIN;
    GPIO_Init(PWM_PB1_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(PWM_PB1_PORT, PWM_PB1_PINSOURCE, PWM_PB1_AF);
    TIM_OC4Init(PWM_PB1_TIMER, &TIM_OCInitStructure);


    GPIO_InitStructure.GPIO_Pin = PWM_PB3_PIN;
    GPIO_Init(PWM_PB3_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(PWM_PB3_PORT, PWM_PB3_PINSOURCE, PWM_PB3_AF);
    TIM_OC2Init(PWM_PB3_TIMER, &TIM_OCInitStructure);

    TIM_Cmd(TIM2, ENABLE);
    TIM_CtrlPWMOutputs(TIM2, ENABLE);

    TIM_Cmd(TIM3, ENABLE);
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
}



void pwm_set(uint8_t number, float pwmf)
{
    int pwm = pwmf * PWMTOP ;

    if (pwm < 0) pwm = 0;
    if (pwm > PWMTOP) pwm = PWMTOP;

    switch (number)
    {
    case 0:
        TIM3->CCR4 = (uint16_t) pwm;
        break;

    case 1:
        TIM3->CCR2 = (uint16_t) pwm;
        break;

    case 2:
        TIM2->CCR1 = (uint16_t) pwm;
        break;

    case 3:
        TIM2->CCR2 = (uint16_t) pwm;
        break;
    }

}



