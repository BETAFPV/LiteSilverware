#include "flash.h"
#include "drv_fmc.h"
#include "defines.h"


extern int fmc_erase( void );
extern void fmc_unlock(void);
extern void fmc_lock(void);
extern int flash_feature_2;

#define FMC_HEADER 0x12AA0001

extern char rfchannel[4];
extern char rxaddress[5];
extern int rx_bind_load;
extern int rx_bind_enable;
extern uint8_t rgb_led;
extern unsigned char vol_show;
extern unsigned char rx_show;
extern unsigned char mode_show ;
extern float accelcal[3];
extern unsigned char vtx_index;
extern float * pids_array[3];

extern float rcRate[3];
extern float superExpo[3];
extern float Expo[3];


float initial_pid_identifier = -10;
float saved_pid_identifier;


float flash_get_hard_coded_pid_identifier( void) {
	float result = 0;

	for (int i=0;  i<3 ; i++) {
		for (int j=0; j<3 ; j++) {
			result += pids_array[i][j] * (i+1) * (j+1) * 0.932f;
		}
	}
	return result;
}


void flash_hard_coded_pid_identifier( void)
{
 initial_pid_identifier = flash_get_hard_coded_pid_identifier();
}


void flash_save( void) 
{
    fmc_unlock();
    fmc_erase();

    unsigned long addresscount = 0;

    writeword(addresscount++, FMC_HEADER);

    fmc_write_float(addresscount++, initial_pid_identifier );
	
	for (int i=0;  i<3 ; i++) {
		for (int j=0; j<3 ; j++) {
            fmc_write_float(addresscount++, pids_array[i][j]);
		}
	}
    
    
    writeword(addresscount++, rx_show);
    writeword(addresscount++, mode_show);
    writeword(addresscount++, vol_show);
    writeword(addresscount++, rgb_led);
    writeword(addresscount++, vtx_index);
    
    
    fmc_write_float(addresscount++, accelcal[0]);
    fmc_write_float(addresscount++, accelcal[1]);
    fmc_write_float(addresscount++, accelcal[2]);
    
    fmc_write_float(addresscount++, rcRate[0]);
    fmc_write_float(addresscount++, rcRate[1]);
    fmc_write_float(addresscount++, rcRate[2]);
    
    fmc_write_float(addresscount++, superExpo[0]);
    fmc_write_float(addresscount++, superExpo[1]);
    fmc_write_float(addresscount++, superExpo[2]);
    
    fmc_write_float(addresscount++, Expo[0]);
    fmc_write_float(addresscount++, Expo[1]);
    fmc_write_float(addresscount++, Expo[2]);
    
    
    if ( rx_bind_enable )
    {
        writeword(50, rxaddress[4]);
        writeword(51, rxaddress[0]|(rxaddress[1]<<8)|(rxaddress[2]<<16)|(rxaddress[3]<<24));
        writeword(52, rfchannel[0]|(rfchannel[1]<<8)|(rfchannel[2]<<16)|(rfchannel[3]<<24));
    }
    else
    {

    }

    if (flash_feature_2)
    {
        fmc_write_float (54,1);
    } else {
        fmc_write_float (54,0);
    }
    writeword(255, FMC_HEADER);

    fmc_lock();
}


void flash_load( void)
{
    unsigned long addresscount = 0;
    unsigned long temp;

    if (FMC_HEADER == fmc_read(addresscount++)&& FMC_HEADER == fmc_read(255))
    {
         saved_pid_identifier = fmc_read_float(addresscount++);     
         if (  saved_pid_identifier == initial_pid_identifier )
         {
             for (int i=0;  i<3 ; i++) {
                for (int j=0; j<3 ; j++) {
                    pids_array[i][j] = fmc_read_float(addresscount++);
                }
            }
         }
         else{
             addresscount+=9; 
         }
     
        temp = fmc_read(addresscount++);
        rx_show = temp & 0x0F;
        if (rx_show > 1)
            rx_show = 1;
        
        temp = fmc_read(addresscount++);
        mode_show = temp & 0x0F;
        if (mode_show > 1)
            mode_show = 1;
        
        temp = fmc_read(addresscount++);
        vol_show = temp & 0x0F;
        if (vol_show > 1)
            vol_show = 1;
        
        temp = fmc_read(addresscount++);
        rgb_led = temp & 0x0F;
        if (rgb_led > 1)
            rgb_led = 0;
        
        temp = fmc_read(addresscount++);
        vtx_index = temp & 0x0F;
        if (vtx_index > 39)
            vtx_index = 8;
        
        
        accelcal[0] = fmc_read_float(addresscount++);
        accelcal[1] = fmc_read_float(addresscount++);
        accelcal[2] = fmc_read_float(addresscount++);
        
        rcRate[0] = fmc_read_float(addresscount++);
        rcRate[1] = fmc_read_float(addresscount++);
        rcRate[2] = fmc_read_float(addresscount++);
        
        
        superExpo[0] = fmc_read_float(addresscount++);
        superExpo[1] = fmc_read_float(addresscount++);
        superExpo[2] = fmc_read_float(addresscount++);
        
        Expo[0] = fmc_read_float(addresscount++);
        Expo[1] = fmc_read_float(addresscount++);
        Expo[2] = fmc_read_float(addresscount++);
        
        for(int i=0;i<3;i++)
        {
            if(rcRate[i] > 2.55f)
            {
                rcRate[i] = 1.0f;
            }
            
            if(superExpo[i] > 1.0f)
            {
                superExpo[i] = 0.0f;
            }
            
            if(Expo[i] > 1.0f)
            {
                Expo[i] = 0.0f;
            }
            
        }
        
        int temp = fmc_read(52);
        int error = 0;
        for ( int i = 0 ; i < 4; i++)
        {
            if ( ((temp>>(i*8))&0xff  ) > 127)
            {
                error = 1;
            }
        }

        if( !error )
        {
            rx_bind_load = rx_bind_enable = 1;

            rxaddress[4] = fmc_read(50);

            int temp = fmc_read(51);
            for ( int i = 0 ; i < 4; i++)
            {
                rxaddress[i] =  temp>>(i*8);
            }

            temp = fmc_read(52);
            for ( int i = 0 ; i < 4; i++)
            {
                rfchannel[i] =  temp>>(i*8);
            }
        }


        flash_feature_2 = fmc_read_float(54);
    }
    else
    {

    }

}








