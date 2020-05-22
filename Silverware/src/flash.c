
#include "project.h"
#include "drv_fmc.h"
#include "defines.h"

extern int fmc_erase( void );
extern void fmc_unlock(void);
extern void fmc_lock(void);

extern float accelcal[];
extern unsigned char powerlevel;
extern unsigned char channel;
extern float * pids_array[3];
extern  unsigned char tx_config;
extern  unsigned char mode_config;
extern unsigned char led_config;
extern unsigned char T8SG_config;
extern float hardcoded_pid_identifier;
extern unsigned char rx_switch;
extern  char motorDir[4];
extern unsigned char low_bat_l;
extern unsigned char mode_l;
extern unsigned char vol_l;
extern unsigned char curr_l;
extern unsigned char turtle_l;
extern unsigned char low_battery;

extern unsigned char profileAB;
extern unsigned int ratesValue;
extern unsigned int ratesValueYaw;

#define FMC_HEADER 0x12AA0001

int save_motor_dir_identifier;
float initial_pid_identifier = -10;
float saved_pid_identifier;
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
  
    fmc_write_float(addresscount++, ratesValue);
    fmc_write_float(addresscount++, ratesValueYaw);

    
    writeword(addresscount++, powerlevel);
    writeword(addresscount++, channel);
    
#ifdef f042_1s_bayang
    writeword(38,led_config);
    writeword(39,T8SG_config);
    writeword(40,tx_config);
    writeword(41,mode_config);
#else 
    writeword(40,(motorDir[0]|motorDir[1]|motorDir[2]|motorDir[3]));
    writeword(41, motorDir[0] | (motorDir[1]<<8) |(motorDir[2]<<16) |(motorDir[3]<<24));
#endif
    writeword(42,rx_switch);
    writeword(43,low_bat_l);
    writeword(44,mode_l);
    writeword(45,vol_l);
    
#ifdef f042_2s_bl
    writeword(46,curr_l);
#endif

    writeword(47,turtle_l);
    writeword(48,low_battery);
    writeword(49,profileAB);
    
    
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
// load pids from flash if pid.c values are still the same       
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

     accelcal[0] = fmc_read_float(addresscount++ );
     accelcal[1] = fmc_read_float(addresscount++ );
     accelcal[2] = fmc_read_float(addresscount++ );  
    
     ratesValue = fmc_read_float(addresscount++ );
     ratesValueYaw = fmc_read_float(addresscount++ ); 
    
     powerlevel = fmc_read(addresscount++ );
     channel = fmc_read(addresscount++ ); 
     
     if(channel >39)
         channel = 0;
     
#ifdef f042_1s_bayang
     led_config = fmc_read(38);
     if(led_config>1)
         led_config=0;
     T8SG_config = fmc_read(39);  
     if(T8SG_config>1)
         T8SG_config=0;
     tx_config = fmc_read(40);
     if(tx_config>1)
         tx_config=0;
     mode_config = fmc_read(41);
#else
     save_motor_dir_identifier =  fmc_read(40);
     
     int temp = fmc_read(41);
     for ( int i = 0 ; i < 4; i++)
     {
        save_motor_dir_temp[i] =  temp>>(i*8);        
     }
     if(save_motor_dir_temp[0]|save_motor_dir_temp[1]|save_motor_dir_temp[2]|save_motor_dir_temp[3] == save_motor_dir_identifier)
     {
        for(int i = 0 ; i < 4; i++)
        {
            motorDir[i] = save_motor_dir_temp[i];
        }
     }
#endif
     rx_switch = fmc_read(42);	
     
     low_bat_l = fmc_read(43);
     mode_l = fmc_read(44);
     vol_l = fmc_read(45);
     
#ifdef f042_2s_bl
     curr_l = fmc_read(46);
#endif
     
     turtle_l = fmc_read(47);
     low_battery = fmc_read(48);
     profileAB = fmc_read(49);
     
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













