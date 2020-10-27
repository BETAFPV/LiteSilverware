#include "bus_i2c.h"



#define HW_I2C_TIMINGREG 0x00700818
#define HW_I2C_DIGITAL_FILTER 15

int liberror;


void i2c_init(void)
{
    GPIO_InitTypeDef gpioinitI2C1;

    gpioinitI2C1.GPIO_Mode = GPIO_Mode_AF;
    gpioinitI2C1.GPIO_OType = GPIO_OType_OD;
    gpioinitI2C1.GPIO_PuPd = GPIO_PuPd_UP;

    gpioinitI2C1.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOF, &gpioinitI2C1);

    GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_1);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);

    I2C_InitTypeDef initI2C1;

    initI2C1.I2C_Timing = HW_I2C_TIMINGREG;
    initI2C1.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
    initI2C1.I2C_DigitalFilter = HW_I2C_DIGITAL_FILTER ;
    initI2C1.I2C_Mode = I2C_Mode_I2C;
    initI2C1.I2C_OwnAddress1 = 0xAB;
    initI2C1.I2C_Ack = I2C_Ack_Enable;
    initI2C1.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    I2C_Init(I2C1, &initI2C1);
    I2C_Cmd(I2C1, ENABLE);
}


#define I2C_CONDITION ((i2c_timeout>>13))

int i2c_sendheader(int address, int reg, int bytes)
{

    unsigned int i2c_timeout = 0;
//check i2c ready
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) == SET)
    {
        i2c_timeout++;
        if (I2C_CONDITION)
        {
            liberror++;
            return 0;
        }
    }

// start transfer
    I2C_TransferHandling(I2C1, address << 1, bytes, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

//i2c_timeout = 0;
// wait for address to be sent
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
    {
        i2c_timeout++;
        if (I2C_CONDITION)
        {
            liberror++;
            return 0;
        }
    }

// send next byte (register location)
    I2C_SendData(I2C1, (uint8_t)reg);

//i2c_timeout = 0;
// wait until last data sent
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) == RESET)
    {
        i2c_timeout++;
        if (I2C_CONDITION)
        {
            liberror++;
            return 0;
        }
    }

    return 1;
}



void i2c_writereg(int address, int reg, int data)
{

    unsigned int i2c_timeout = 0;

// send start + writeaddress + register location, common send+receive
    i2c_sendheader(address, reg, 2);
// send register value
    I2C_SendData(I2C1, (uint8_t) data);
// wait for finish
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_TC) == RESET)
    {
        i2c_timeout++;
        if (I2C_CONDITION)
        {
            liberror++;
            return;
        }
    }

// send stop - end transaction
    I2C_GenerateSTOP(I2C1, ENABLE);


    return;
}



int i2c_readdata(int address, int reg, int *data, int size)
{

    static uint8_t i = 0;
    unsigned int i2c_timeout = 0;

    // send start + writeaddress + register location, common send+receive
    i2c_sendheader(address, reg, 1);

    //send restart + readaddress
    I2C_TransferHandling(I2C1, address << 1, size, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

//wait for data
    for (i = 0; i < size; i++)
    {
        i2c_timeout = 0;
        while (I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
        {
            i2c_timeout++;
            if (I2C_CONDITION)
            {
                liberror++;
                return 0;
            }
        }
        data[i] = I2C_ReceiveData(I2C1);
    }

//data received
    return 1;
}



int i2c_readreg(int address, int reg)
{
    int data;
    i2c_readdata(address, reg, &data, 1);
    return data;
}






