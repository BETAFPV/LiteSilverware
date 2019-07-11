
#include "project.h"
#include "drv_fmc.h"
#include "config.h"
#include "math.h"
int moto_temp = 0;
float re;
extern int fmc_erase( void );
extern void fmc_unlock(void);
extern void fmc_lock(void);
extern char save_motor_dir[4];
extern float accelcal[];
extern float * pids_array[3];
float save_pid[3][3] = {0};
extern float hardcoded_pid_identifier;
extern int rx_switch;

#define FMC_HEADER 0x12AA0001

float initial_pid_identifier = -10;
float saved_pid_identifier;

int save_motor_dir_identifier;
int initial_motor_dir_identifier;
char save_motor_dir_temp[4] = {0};

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




void flash_save( void) {

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
 

    fmc_write_float(addresscount++, accelcal[0]);
    fmc_write_float(addresscount++, accelcal[1]);
    fmc_write_float(addresscount++, accelcal[2]);
	
		writeword(20,save_motor_dir[0] | (save_motor_dir[1]<<8) | (save_motor_dir[2]<<16) | (save_motor_dir[3]<<24));
	  //motor dir identifier 
		writeword(21,(save_motor_dir[0]|save_motor_dir[1]|save_motor_dir[2]|save_motor_dir[3]));
		writeword(22,rx_switch);
#ifdef RX_BAYANG_PROTOCOL_TELEMETRY_AUTOBIND
// autobind info     
extern char rfchannel[4];
extern char rxaddress[5];
extern int telemetry_enabled;
extern int rx_bind_enable;
    
 // save radio bind info  
    if ( rx_bind_enable )
    {
    writeword(50, rxaddress[4]|telemetry_enabled<<8);
    writeword(51, rxaddress[0]|(rxaddress[1]<<8)|(rxaddress[2]<<16)|(rxaddress[3]<<24));
    writeword(52, rfchannel[0]|(rfchannel[1]<<8)|(rfchannel[2]<<16)|(rfchannel[3]<<24));
    }
    else
    {
      // this will leave 255's so it will be picked up as disabled  
    }
#endif  


#ifdef SWITCHABLE_FEATURE_1
extern int flash_feature_1;

//save filter cut info

if (flash_feature_1)
{
	fmc_write_float (53,1);	
}else{
	fmc_write_float (53,0);	
}
#endif

#ifdef SWITCHABLE_FEATURE_2
extern int flash_feature_2;

//save LVC info

if (flash_feature_2)
{
	fmc_write_float (54,1);	
}else{
	fmc_write_float (54,0);	
}
#endif

#ifdef SWITCHABLE_FEATURE_3
extern int flash_feature_3;

//save LVC info

if (flash_feature_3)
{
	fmc_write_float (55,1);	
}else{
	fmc_write_float (55,0);	
}
#endif



#if defined(RX_DSMX_2048) || defined(RX_DSM2_1024)
extern int rx_bind_enable;
if ( rx_bind_enable ){
		fmc_write_float (56,1);
	}else{
		fmc_write_float (56,0);	
	}
#endif

    writeword(255, FMC_HEADER);
    
	fmc_lock();
}



void flash_load( void) {

	unsigned long addresscount = 0;
// check if saved data is present
    if (FMC_HEADER == fmc_read(addresscount++)&& FMC_HEADER == fmc_read(255))
    {
     saved_pid_identifier = fmc_read_float(addresscount++);
		 /************************2019 7 9************************/
			for (int i=0;  i<3 ; i++) 
			{
					for(int j=0; j<3 ; j++)
					{
                save_pid[i][j] = fmc_read_float(addresscount++);
          }
      }
			
			for (int i=0;  i<3 ; i++)
			{
					for (int j=0; j<3 ; j++) 
					{
							re += save_pid[i][j] * (i+1) * (j+1) * 0.932f;
					}
	    }
			if(fabs(re - saved_pid_identifier) < 1.0)
			{
					for (int i=0;  i<3 ; i++) 
					{
							for(int j=0; j<3 ; j++)
							{
									pids_array[i][j] = save_pid[i][j];
							}
					}
			}
			else
			{
					addresscount+=9; 
			}
		 /********************************************************/
// load pids from flash if pid.c values are still the same       
//     if (saved_pid_identifier == initial_pid_identifier )
//     {
//         for (int i=0;  i<3 ; i++) {
//            for (int j=0; j<3 ; j++) {
//                pids_array[i][j] = fmc_read_float(addresscount++);
//            }
//        }
//     }
//     else
//		 {
//        addresscount+=9; 
//     }    

    accelcal[0] = fmc_read_float(addresscount++ );
    accelcal[1] = fmc_read_float(addresscount++ );
    accelcal[2] = fmc_read_float(addresscount++ );  
	  save_motor_dir_identifier =  fmc_read(21);
		moto_temp = fmc_read(20);
		for ( int i = 0 ; i < 4; i++)
		
		{
				save_motor_dir_temp[i] =  moto_temp>>(i*8);        
		}
		if(save_motor_dir_temp[0]|save_motor_dir_temp[1]|save_motor_dir_temp[2]|save_motor_dir_temp[3] == save_motor_dir_identifier)
		{
				for(int i = 0 ; i < 4; i++)
				{
						save_motor_dir[i] = save_motor_dir_temp[i];
				}
		}
		else
		{
				//*****
		}
	  rx_switch = fmc_read(22);
 #ifdef RX_BAYANG_PROTOCOL_TELEMETRY_AUTOBIND  
extern char rfchannel[4];
extern char rxaddress[5];
extern int telemetry_enabled;
extern int rx_bind_load;
extern int rx_bind_enable;
     
 // save radio bind info   

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

        telemetry_enabled = fmc_read(50)>>8;
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
#endif

#ifdef SWITCHABLE_FEATURE_1
	extern int flash_feature_1;
	flash_feature_1 = fmc_read_float(53);
#endif

#ifdef SWITCHABLE_FEATURE_2
	extern int flash_feature_2;
	flash_feature_2 = fmc_read_float(54);
#endif

#ifdef SWITCHABLE_FEATURE_3
	extern int flash_feature_3;
	flash_feature_3 = fmc_read_float(55);
#endif
#if defined(RX_DSMX_2048) || defined(RX_DSM2_1024)
	extern int rx_bind_enable;
	rx_bind_enable = fmc_read_float(56);
#endif

    }
    else
    {
        
    }
    
}













