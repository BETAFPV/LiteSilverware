#include "stm32f0xx.h"



void i2c_init(void);


int i2c_readreg(int address, int reg);


int i2c_readdata(int address, int reg, int *data, int size);


void i2c_writereg(int address, int reg , int data);


int i2c_sendheader(int address, int reg, int bytes);














