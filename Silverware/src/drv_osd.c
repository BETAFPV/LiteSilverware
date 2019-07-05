#include "drv_osd.h"
#include "drv_time.h"
#include "math.h"
extern char motor_sta;
/*********************************************
* Function name: osd_spi_cson
* Effect: OSD cs set low
* Import: void 
* Output: void
**********************************************/
void osd_spi_cson( )
{
	OSD_SPI_CSN_PORT->BRR = OSD_SPI_CSN_PIN;    //jiang yinjiao la di 
}

/*********************************************
* Function name: osd_spi_csoff
* Effect: OSD cs set High
* Import: void 
* Output: void
**********************************************/
void osd_spi_csoff( )
{
	OSD_SPI_CSN_PORT->BSRR = OSD_SPI_CSN_PIN;    // jiang yinjiao la gao 
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
    for (uint8_t i = 1; i < pack_len-1; i++)
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
    uint8_t osd_data[15];
    osd_data[0] = Package_Type;           // Package Type
    osd_data[1] = data >> 8;
    osd_data[2] = data & 0xFF;
    osd_data[14] = OSD_checksum(osd_data);
    OSD_Tx_Data(osd_data,14);
}



void make_vol_pack(unsigned char data[],unsigned int VOL,float kp[],float ki[],float kd[],unsigned char menu_flag,unsigned char menu_class,unsigned char menu_index)
{
			data[0] = 0x88;
			data[1] = VOL >> 8;
	    data[2] = VOL & 0xFF;
			data[3] = round(kp[0]*100);
			data[4] = round(kp[1]*100);
			data[5] = round(kp[2]*100);
			data[6] = round(ki[0]*100);
			data[7] = round(ki[1]*100);
			data[8] = round(ki[2]*100);
			data[9] = round(kd[0]*100);
			data[10] = round(kd[1]*100);
			data[11] = round(kd[2]*100);
			if(1 == menu_flag)
			{
					data[12] |= 0x10;
			}
			else
			{
					data[12] &= ~0x10;
			}
			data[12] &= 0xF0;
			data[12] |= motor_sta;
			
			data[13] = menu_class;
			data[14] = menu_index;
			data[15] = 0;
	    data[16] = OSD_checksum(data);
}
