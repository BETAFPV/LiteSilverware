#include "drv_osd.h"
#include "drv_time.h"
/*********************************************
* Function name: osd_spi_cson
* Effect: OSD cs set low
* Import: void 
* Output: void
**********************************************/
void osd_spi_cson( )
{
	OSD_SPI_CSN_PORT->BRR = OSD_SPI_CSN_PIN;
}

/*********************************************
* Function name: osd_spi_csoff
* Effect: OSD cs set High
* Import: void 
* Output: void
**********************************************/
void osd_spi_csoff( )
{
	OSD_SPI_CSN_PORT->BSRR = OSD_SPI_CSN_PIN;
}

/*********************************************
* Function name: Tx_Data
* Effect: OSD SPI Initialization 
* Import: void 
* Output: void
**********************************************/
void osd_spi_init(void)
{    
	GPIO_InitTypeDef  GPIO_InitStructure;
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	osd_spi_csoff();

}

/*********************************************
* Function name: Tx_Data
* Effect: SPI Write string
* Import: void 
* Output: void
**********************************************/
void OSD_Tx_Data(uint8_t *OSD_Data ,uint8_t length)
{
	uint8_t index = 0;
	osd_spi_cson();
	while(index < length)
	{
		spi_sendbyte(OSD_Data[index]);
		index++;
	}	
	osd_spi_csoff();
}

/*************************
* Function name: OSD_checksum
* Effect: packet Check and compute
* Import: string address 
* Output: void
**************************/
uint8_t OSD_checksum(uint8_t OSD_DATA[])
{
    unsigned char sum = OSD_DATA[0];
    for (uint8_t i = 1; i < 3; i++)
    sum += OSD_DATA[i];
    return sum;
}
/*********************************************
* Function name: OSD_Data_Send
* Effect: FC MCU sends data to OSD MCU through SPI 
* Import: Package_Type : ARMING state is 0;Vbattfilt is 1
*         data : Package Type Corresponding data
* Output: void
**********************************************/
void OSD_Data_Send(uint8_t Package_Type,uint16_t data)
{
    uint8_t osd_data[4];
    osd_data[0] = Package_Type;           // Package Type
    osd_data[1] = data >> 8;
    osd_data[2] = data & 0xFF;
    osd_data[3] = OSD_checksum(osd_data);
    OSD_Tx_Data(osd_data,4);
}


