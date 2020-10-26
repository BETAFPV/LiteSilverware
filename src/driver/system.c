#include "system.h"
#include "time.h"
#include "config.h"
#include "defines.h"
#include "hardware.h"
#include "misc_gpio.h"
#include "bus_softspi.h"
#include "serial_uart.h"
#include "bus_i2c.h"
#include "pwm.h"
#include "mpu6050.h"
#include "adc.h"





uint8_t systemInit(void)
{
    delay(1000);
    
    //overclock 64M
    setclock();
    
    //sysTick 1ms
    sysTick_init();
    
    delay_ms(10);
    
    //gpio
    gpio_init();
    
    //softspi
    spi_init();
    
    //serial uart
    uart_init(4800);
    
    delay_ms(10);
    
    //i2c
    i2c_init();
    
    //pwm
    pwm_init();
    for(int i=0;i<4;i++)
    {
       pwm_set(i,0);
    }
    
    //mpu6050
    mpu6050_init();
    
    if (mpu6050_check())
    {
    }
    else
    {
       return 1;
    }
    
    //adc
    adc_init();
    
    
    
    
    
    return 0;
}





