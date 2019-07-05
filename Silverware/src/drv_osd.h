#ifndef __DRV_OSD_
#define __DRV_OSD_

#include "drv_spi.h"
#include "project.h"

#define OSD_SPI_CSN_PIN GPIO_Pin_2
#define OSD_SPI_CSN_PORT GPIOA
#define pack_len 17
void OSD_Tx_Data(uint8_t *OSD_Data ,uint8_t length);
unsigned char OSD_checksum(unsigned char OSD_DATA[]);
void osd_spi_init(void);
void OSD_Data_Send(uint8_t Package_Type,uint16_t data);
void OSD_Data_Send_buf(unsigned char data[]);
void make_vol_pack(unsigned char data[],unsigned int VOL,float kp[],float ki[],float kd[],unsigned char menu_flag,unsigned char menu_class,unsigned char menu_index);
#endif
