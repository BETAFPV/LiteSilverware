#ifndef __DRV_OSD_
#define __DRV_OSD_

#include "drv_spi.h"
#include "project.h"

#define OSD_SPI_CSN_PIN GPIO_Pin_2
#define OSD_SPI_CSN_PORT GPIOA

void OSD_Tx_Data(uint8_t *OSD_Data ,uint8_t length);
unsigned char OSD_checksum(unsigned char OSD_DATA[]);
void osd_spi_init(void);
void OSD_Data_Send(uint8_t Package_Type,uint16_t data);
#endif
