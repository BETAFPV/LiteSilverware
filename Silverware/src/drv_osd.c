#include "drv_osd.h"
#include "drv_time.h"
#include "math.h"
#include "defines.h"
#include "targets.h"
#include "config.h"

#ifdef  Lite_OSD
extern char motor_sta;
extern char aux[AUXNUMBER];
extern int rx_switch;
unsigned char showcase = 0;
extern float rx[];
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
	if(1==rx_switch)
	{
		osd_spi_cson();
		while(index < length)
		{
			spi_sendbyte(OSD_Data[index]);
			index++;
		}
		osd_spi_csoff();
	}
	else
	{
		osd_spi_cson();
		while(index < length)
		{
			spi_sendbyte_sd(OSD_Data[index]);
			index++;
		}
		osd_spi_csoff();
	}
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

void make_vol_pack(unsigned char data[],unsigned int VOL,unsigned char aetr_or_taer,float rx[],char aux[],float kp[],float ki[],float kd[],unsigned char menu_flag,unsigned char menu_class,unsigned char menu_index,unsigned char showcase)
{
			data[0] =0x0f;

			if(aux[CHAN_5] == 1)
			{
					data[0] |= (1<<4);
			}
			else
			{
					data[0] &= ~(1<<4);
			}
			
			data[0] &= ~(0x3 << 6);
			data[0] |= rx_switch << 6;
			
			data[1] = VOL >> 8;
	    data[2] = VOL & 0xFF;
			
			data[3]=0x0;
			data[3] |= showcase << 0;
			data[3] |= motor_sta <<4;
			
			data[4] = menu_index;
			data[4] |= aetr_or_taer << 4;
			if(!aux[LEVELMODE])
			{
				if(aux[RACEMODE])
				{
					data[4] |= 1<<6;
				}
				else
				{
					data[4] &= ~(1<<6);
				}
			}
			data[5]=0x00;
			data[6]=0x00;
			if(showcase !=2)
			{
				if(rx[0]> -0.2f && rx[0] <0.2f)
				{
					data[5] |= 0x1<<0;
				}
				else if(rx[0] > 0.4f)
				{
					data[5] |= 0x2<<0;
				}
				else if(rx[0] < -0.4f)
				{
					data[5] |= 0x0<<0;
				}
				
				if(rx[1]> -0.2f && rx[1] <0.2f)
				{
					data[5] |= 0x1<<2;
				}
				else if(rx[1]>= 0.2f)
				{
					data[5] |= 0x2<<2;
				}
				else 
				{
					data[5] |= 0x0<<2;
				}
				
				if(rx[2]> -0.2f && rx[2] <0.2f)
				{
					data[5] |= 0x1<<4;
				}
				else if(rx[2]>= 0.2f)
				{
					data[5] |= 0x2<<4;
				}
				else 
				{
					data[5] |= 0x0<<4;
				}
				
				if(rx[3]> 0.4f && rx[3] <0.6f)
				{
					data[5] |= 0x1<<6;
				}
				else if(rx[3]>= 0.6f)
				{
					data[5] |= 0x2<<6;
				}
				else 
				{
					data[5] |= 0x0<<6;
				}
				
				data[6] |= aux[CHAN_5] << 0;
				data[6] |= aux[CHAN_6] << 1;
				data[6] |= aux[CHAN_7] << 2;
				data[6] |= aux[CHAN_8] << 3;
			}
			else
			{
					data[5] = round(kp[0]*100);
					data[6] = round(kp[1]*100);
					data[7] = round(kp[2]*100);
					data[8] = round(ki[0]*100);
					data[9] = round(ki[1]*100);
					data[10] = round(ki[2]*100);
					data[11] = round(kd[0]*100);
					data[12] = round(kd[1]*100);
					data[13] = round(kd[2]*100);
			}

	    data[14] = OSD_checksum(data);
}

#endif


