/*
The MIT License (MIT)

Copyright (c) 2016 silverx

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "binary.h"
#include "drv_spi.h"

#include "project.h"
#include "xn297.h"
#include "drv_time.h"
#include <stdio.h>
#include "config.h"
#include "defines.h"

#include "rx_bayang.h"

#include "stm32f0xx_usart.h"
#include <stdio.h>
#include "drv_serial.h"
#include "defines.h"
#include "util.h"
#include "rx_sbus_dsmx_bayang_switch.h"

#include "stm32f0xx_usart.h"
#include <stdio.h>
#include "drv_serial.h"
#include "drv_fmc.h"
#include "targets.h"

#ifdef RX_SBUS_DSMX_BAYANG_SWITCH

// global use rx variables
extern float rx[4];
extern char aux[AUXNUMBER];
extern char lastaux[AUXNUMBER];
extern char auxchange[AUXNUMBER];
int failsafe = 0;
int rxmode = 0;
int rx_ready = 0;
int bind_safety = 0;
int rx_bind_enable = 0;
int sbus_dsmx_flag = 0; 

extern unsigned char tx_config;
extern unsigned char mode_config;
extern unsigned char showcase;
extern unsigned char led_config;
extern unsigned char T8SG_config;

#define DSM_SCALE_PERCENT 100	
float dsm2_scalefactor = (0.29354210f/DSM_SCALE_PERCENT);
float dsmx_scalefactor = (0.14662756f/DSM_SCALE_PERCENT);

// internal dsm variables

#define DSMX_SERIAL_BAUDRATE 115200
#define SPEK_FRAME_SIZE 16   
#define SPEKTRUM_NEEDED_FRAME_INTERVAL  5000
#define SPEKTRUM_MAX_FADE_PER_SEC       40
#define SPEKTRUM_FADE_REPORTS_PER_SEC   2
#define SPEKTRUM_MAX_SUPPORTED_CHANNEL_COUNT 12
#define SPEKTRUM_2048_CHANNEL_COUNT     12
#define SPEKTRUM_1024_CHANNEL_COUNT     7

#define RX_DSMX_2048

#ifdef RX_DSMX_2048
	#define CHANNEL_COUNT 10
	#define BIND_PULSES 9
	// 11 bit frames
	static uint8_t spek_chan_shift = 3;
	static uint8_t spek_chan_mask = 0x07;  	
#endif

#ifdef RX_DSM2_1024
	#define CHANNEL_COUNT 7
	#define BIND_PULSES 3
	// 10 bit frames
	static uint8_t spek_chan_shift = 2;
	static uint8_t spek_chan_mask = 0x03;
#endif


static uint32_t dsmx_channels[CHANNEL_COUNT];
static int rcFrameComplete = 0;
int framestarted = -1;
int rx_frame_pending;
int rx_frame_pending_last;
uint32_t flagged_time;
static volatile uint8_t spekFrame[SPEK_FRAME_SIZE];


#define SERIAL_BAUDRATE 100000

// sbus is normally inverted
#define SBUS_INVERT 1

// channel order,Enable is AETR, 
#define AETR 

// global use rx variables
extern float rx[4];
extern char aux[AUXNUMBER];
extern char lastaux[AUXNUMBER];
extern char auxchange[AUXNUMBER];
//int failsafe = 0;
//int rxmode = 0;
//int rx_ready = 0;


// internal sbus variables
#define RX_BUFF_SIZE 64							//SPEK_FRAME_SIZE 16  
uint8_t rx_buffer[RX_BUFF_SIZE];    //spekFrame[SPEK_FRAME_SIZE]
uint8_t rx_start = 0;
uint8_t rx_end = 0;
uint16_t rx_time[RX_BUFF_SIZE];			//????

//int framestarted = -1;
uint8_t framestart = 0;


unsigned long time_lastrx;
unsigned long time_siglost;
uint8_t last_rx_end = 0;
int last_byte = 0;
unsigned long time_lastframe;
int frame_received = 0;
int rx_state = 0;
//int bind_safety = 0;
uint8_t data[25];
int sbus_channels[9];

int failsafe_sbus_failsafe = 0;   
int failsafe_siglost = 0; 
int failsafe_noframes = 0;

// enable statistics
const int sbus_stats = 0;

// statistics
int stat_framestartcount;
int stat_timing_fail;
int stat_garbage;
//int stat_timing[25];
int stat_frames_accepted = 0;
int stat_frames_second;
int stat_overflow;




// compatibility with older version hardware.h
#if ( !defined RADIO_XN297 && !defined RADIO_XN297L)
#define RADIO_XN297
#endif

#define BAYANG_LOWRATE_MULTIPLIER 1.0


extern float rx[4];
extern char aux[AUXNUMBER];
extern char lastaux[AUXNUMBER];
extern char auxchange[AUXNUMBER];


  char rfchannel[4];
	int rxaddress[5];
//	int rxmode = 0;
	int rf_chan = 0;
//  int rx_ready = 0;
//	int bind_safety = 0;



void bleinit( void);

void writeregs ( uint8_t data[] , uint8_t size )
{
spi_cson();
for ( uint8_t i = 0 ; i < size ; i++)
{
	spi_sendbyte( data[i]);
}
spi_csoff();
delay(1000);
}

#define BLE_INTERVAL 30000

// this allows different quads to show up at the same time.
// 0 - 255 select a different number for each quad if you need several simultaneous
#define BLE_QUAD_NUMBER 17


void rx_init()
{

#ifdef RADIO_XN297L

#ifndef TX_POWER
#define TX_POWER 7
#endif
 	
// Gauss filter amplitude - lowest
static uint8_t demodcal[2] = { 0x39 , B00000001 };
writeregs( demodcal , sizeof(demodcal) );

// powerup defaults
//static uint8_t rfcal2[7] = { 0x3a , 0x45 , 0x21 , 0xef , 0xac , 0x3a , 0x50};
//writeregs( rfcal2 , sizeof(rfcal2) );

static uint8_t rfcal2[7] = { 0x3a , 0x45 , 0x21 , 0xef , 0x2c , 0x5a , 0x50};
writeregs( rfcal2 , sizeof(rfcal2) );

static uint8_t regs_1f[6] = { 0x3f , 0x0a, 0x6d , 0x67 , 0x9c , 0x46 };
writeregs( regs_1f , sizeof(regs_1f) );


static uint8_t regs_1e[4] = { 0x3e , 0xf6 , 0x37 , 0x5d };
writeregs( regs_1e , sizeof(regs_1e) );

//#define XN_TO_RX B10001111
//#define XN_TO_TX B10000010
//#define XN_POWER B00000001|((TX_POWER&7)<<3)


#define XN_TO_RX B10001111
#define XN_TO_TX B10000010
#define XN_POWER B00111111
	
#endif

	
#ifdef RADIO_XN297
static uint8_t bbcal[6] = { 0x3f , 0x4c , 0x84 , 0x6F , 0x9c , 0x20  };
writeregs( bbcal , sizeof(bbcal) );
// new values
static uint8_t rfcal[8] = { 0x3e , 0xc9 , 0x9a , 0xA0 , 0x61 , 0xbb , 0xab , 0x9c  };
writeregs( rfcal , sizeof(rfcal) );

static uint8_t demodcal[6] = { 0x39 , 0x0b , 0xdf , 0xc4 , 0xa7 , 0x03};
writeregs( demodcal , sizeof(demodcal) );


#define XN_TO_RX B00001111
#define XN_TO_TX B00000010
#define XN_POWER B00000111
#endif

// dummy write	
//xn_writereg( RF_CH , 1 );  // bind on channel 0

bleinit();

delay(100);


int rxaddress[5] = { 0 , 0 , 0 , 0 , 0  };
xn_writerxaddress( rxaddress);

	xn_writereg( EN_AA , 0 );	// aa disabled
	xn_writereg( EN_RXADDR , 1 ); // pipe 0 only
	xn_writereg( RF_SETUP , XN_POWER);  // lna high current on ( better performance )
	xn_writereg( RX_PW_P0 , 15 ); // payload size
	xn_writereg( SETUP_RETR , 0 ); // no retransmissions ( redundant?)
	xn_writereg( SETUP_AW , 3 ); // address size (5 bits)
	xn_command( FLUSH_RX);
  xn_writereg( RF_CH , 0 );  // bind on channel 0

// set above
//	xn_writereg( 29 , 32); // feture reg , CE mode (software controlled)

#ifdef RADIO_XN297L
xn_writereg( 0x1d, B00111000 ); // 64 bit payload , software ce
spi_cson();
spi_sendbyte( 0xFD); // internal CE high command
spi_sendbyte( 0); // required for above
spi_csoff();
#endif

#ifdef RADIO_XN297
  xn_writereg( 0x1d, B00011000 ); // 64 bit payload , software ce
#endif

  xn_writereg( 0 , XN_TO_RX ); // power up, crc enabled, rx mode


#ifdef RADIO_CHECK
void check_radio(void);
// check_radio();
#endif	
}


void check_radio()
{	
int	rxcheck = xn_readreg( 0x0f); // rx address pipe 5	
	// should be 0xc6
	extern void failloop( int);
	if ( rxcheck != 0xc6) failloop(3);
}

///////////////////
// BLE FUNCTIONS
//  https://github.com/lijunhw/nRF24_BLE/blob/master/Arduino/nRF24_BLE_advertizer_demo/nRF24_BLE_advertizer_demo.ino


void btLeCrc(const uint8_t* data, uint8_t len, uint8_t* dst){
// implementing CRC with LFSR
uint8_t v, t, d;
while(len--){
d = *data++;
for(v = 0; v < 8; v++, d >>= 1){
t = dst[0] >> 7;
dst[0] <<= 1;
if(dst[1] & 0x80) dst[0] |= 1;
dst[1] <<= 1;
if(dst[2] & 0x80) dst[1] |= 1;
dst[2] <<= 1;
if(t != (d & 1)){
dst[2] ^= 0x5B;
dst[1] ^= 0x06;
}
}
}
}



// scrambling sequence for xn297
const uint8_t xn297_scramble[] = {
    0xe3, 0xb1, 0x4b, 0xea, 0x85, 0xbc, 0xe5, 0x66,
    0x0d, 0xae, 0x8c, 0x88, 0x12, 0x69, 0xee, 0x1f,
    0xc7, 0x62, 0x97, 0xd5, 0x0b, 0x79, 0xca, 0xcc,
    0x1b, 0x5d, 0x19, 0x10, 0x24, 0xd3, 0xdc, 0x3f,
    0x8e, 0xc5, 0x2f};

// bit reversed of above
const uint8_t xn297_scramble_rev[] = { 
		0xc7 , 0x8d , 0xd2 , 0x57 , 0xa1 , 0x3d , 0xa7 , 0x66 ,
		0xb0 , 0x75 , 0x31 , 0x11 , 0x48 , 0x96 , 0x77 , 0xf8 , 
		0xe3 , 0x46 , 0xe9 , 0xab , 0xd0 , 0x9e , 0x53 , 0x33 , 
		0xd8 , 0xba , 0x98 , 0x8 , 0x24 , 0xcb , 0x3b , 0xfc , 
		0x71 , 0xa3 , 0xf4 , 85  , 104  , 207  , 169  , 25   ,
		108  ,  93  , 76   , 4   , 146  , 229  , 29  };

		// whitening sequence for adv channel 37 (rf chan 2402)
		// for speed
const uint8 ble_whiten_37[] = { 
	0x8D , 0xd2 , 0x57 , 0xa1 , 0x3d , 0xa7 , 0x66 , 0xb0 ,
	0x75 , 0x31 , 0x11 , 0x48 , 0x96 , 0x77 , 0xf8 , 0xe3 ,
	0x46 , 0xe9 , 0xab , 0xd0 , 0x9e , 0x53 , 0x33 , 0xd8 ,
	0xba , 0x98 , 0x08 , 0x24 , 0xcb , 0x3b , 0xfc , 0x71 ,
	0xa3 , 0xf4 , 0x55 , 0x68 , 0xCF , 0xA9 , 0x19 , 0x6C ,
	0x5D , 0x4C
}; // whitening sequence channel 37 ( 0 - index ; 2 - rf channel; 37 - ble spec)

uint8_t swapbits_old(uint8_t a){
// reverse the bit order in a single byte
uint8_t v = 0;
if(a & 0x80) v |= 0x01;
if(a & 0x40) v |= 0x02;
if(a & 0x20) v |= 0x04;
if(a & 0x10) v |= 0x08;
if(a & 0x08) v |= 0x10;
if(a & 0x04) v |= 0x20;
if(a & 0x02) v |= 0x40;
if(a & 0x01) v |= 0x80;
return v;
}

// from https://graphics.stanford.edu/~seander/bithacks.html#ReverseByteWith32Bits
// reverse the bit order in a single byte
uint8_t swapbits(uint8_t a){
unsigned int b = a;
b = ((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
return b;
}

void btLeWhiten(uint8_t* data, uint8_t len, uint8_t whitenCoeff)
{
	// Implementing whitening with LFSR
	uint8_t m;
	while(len--)
		{
		for(m = 1; m; m <<= 1)
			{
			if(whitenCoeff & 0x80)
				{
				whitenCoeff ^= 0x11;
				(*data) ^= m;
				}
			whitenCoeff <<= 1;
			}
		data++;
	}
}
static inline uint8_t btLeWhitenStart(uint8_t chan){
//the value we actually use is what BT'd use left shifted one...makes our life easier
return swapbits(chan) | 2;
}



void btLePacketEncode(uint8_t* packet, uint8_t len, uint8_t chan){
// Assemble the packet to be transmitted
// Length is of packet, including crc. pre-populate crc in packet with initial crc value!
uint8_t i, dataLen = len - 3;


packet[len - 3] = 0x55; //CRC start value: 0x555555
packet[len - 2] = 0x55;
packet[len - 1] = 0x55;

btLeCrc(packet, dataLen, packet + dataLen);

for(i = 0; i < 3; i++, dataLen++)
	packet[dataLen] = swapbits(packet[dataLen]);

if (1)
{	
// faster array based whitening
for(i = 0; i < len; i++) 
	packet[i] ^=ble_whiten_37[i];
}
else // lfsr based
btLeWhiten(packet, len, btLeWhitenStart(chan));
	
}

#define RXDEBUG

#ifdef RXDEBUG
unsigned long packettime;
int channelcount[4];
int failcount;
int packetrx;
int packetpersecond;


int skipstats[12];
int afterskip[12];
//#warning "RX debug enabled"
#endif

// works with 4 and 5 addresses byte
#define XN297_ADDRESS_SIZE_BLE 5


// The MAC address of BLE advertizer -- just make one up
#define MY_MAC_0	BLE_QUAD_NUMBER
#define MY_MAC_1	0x22
#define MY_MAC_2	0x33
#define MY_MAC_3	0x44
#define MY_MAC_4	0x55
#define MY_MAC_5	0xF6

uint8_t buf[48];
int buffint[48];
static const uint8_t chRf[] = {2, 26,80};
static const uint8_t chLe[] = {37,38,39};
uint8_t ch = 0; // RF channel for frequency hopping

int payloadsize;


void bleinit()
{
// Set access addresses (TX address in nRF24L01) to BLE advertising 0x8E89BED6
// Remember that both bit and byte orders are reversed for BLE packet format
	
int txaddr[5];

/*	
	// 4 byte address
txaddr[0] = swapbits(0x8E)^xn297_scramble[3];
txaddr[1] = swapbits(0x89)^xn297_scramble[2];
txaddr[2] = swapbits(0xBE)^xn297_scramble[1];
txaddr[3] = swapbits(0xD6)^xn297_scramble[0];
txaddr[4] = 0;
*/

/*	
		// 4 byte address - optimized
txaddr[0] = 0x71^0xea;
txaddr[1] = 0x91^0x4b;
txaddr[2] = 0x7d^0xb1;
txaddr[3] = 0x6b^0xe3;
txaddr[4] = 0;
*/

// using 5 byte address because it's shared with rx (rx protocol has 5 bytes)
// saves changing the address size everytime we send
txaddr[0] = 0x71^0x85;
txaddr[1] = 0x91^0xea;
txaddr[2] = 0x7d^0x4b;
txaddr[3] = 0x6b^0xb1;
txaddr[4] = 0xaa^0xe3;	// preamble

	
xn_writetxaddress( txaddr );	



//	xn_writereg( EN_AA , 0 );	// aa disabled -- duplicated
//	xn_writereg( RF_SETUP , B00111011);  // high power xn297L only
//	xn_writereg( SETUP_RETR , 0 ); // no retransmissions  -- duplicated
// -- duplicated
//	xn_writereg( SETUP_AW , XN297_ADDRESS_SIZE_BLE - 2 ); // address size (4 bytes for ble)

//  xn_writereg( 0x1d, B00111000 ); // 64 bit payload , software ce

}

void send_beacon(void);

int loopcounter = 0;
unsigned int ble_txtime;
int ble_send = 0;
int oldchan = 0;

#define BLE_TX_TIMEOUT 10000

void beacon_sequence()
{
static int beacon_seq_state = 0;
	
 switch ( beacon_seq_state )
 {
	 case 0:
		 // send data if enough time passed since last send
	   if ( gettime() - ble_txtime > BLE_INTERVAL )
		 {
		 ble_send = 1;
		 oldchan = rf_chan;
		 send_beacon();
	   beacon_seq_state++;
		 }
	 break;
	 
	 case 1:
		 // wait for data to finish transmitting
			if( (xn_readreg(0x17)&B00010000)  ) 
			{
				xn_writereg( 0 , XN_TO_RX ); 
				xn_writereg(0x25, rfchannel[oldchan]);
			 beacon_seq_state++;
			 goto next;
			}
			else
			{// if it takes too long we get rid of it
				if ( gettime() - ble_txtime > BLE_TX_TIMEOUT )
				{
				 xn_command( FLUSH_TX);
					xn_writereg( 0 , XN_TO_RX ); 
				 beacon_seq_state++;
				 ble_send = 0;
				}
			}
	 break;
	 
		
	 case 2:
		 next:
		 // restore radio settings to protocol compatible
	   // mainly channel here
		 ble_send = 0;	
		if ( rxmode == 0 )
		{
			xn_writereg(0x25, 0 ); // Set channel frequency	, bind
		}
		 beacon_seq_state++;
	 break;
	 
	 default:
		 beacon_seq_state = 0;		 
	 break;
	
 
	 
 }

}

int interleave = 0;

void send_beacon()
{
	
// Channel hopping
ch++;
if (ch>2 ) 
{
  ch = 0;
}
// sending on channel 37 only to use whitening array
ch = 0;


xn_writereg(RF_CH, chRf[ch]);

uint8_t L=0;


extern float vbattfilt;

int vbatt = vbattfilt *1000.0f;

unsigned int time = gettime();

time = time>>20; // divide by 1024*1024, no time for accuracy here
time = time * 10;
 
L=0;
buf[L++] = B00100010; //PDU type, given address is random; 0x42 for Android and 0x40 for iPhone
//buf[L++] = 0x42; //PDU type, given address is random; 0x42 for Android and 0x40 for iPhone

// max len 27 with 5 byte address = 37 total payload bytes
buf[L++] = 10+ 21; // length of payload
buf[L++] = MY_MAC_0;
buf[L++] = MY_MAC_1;
buf[L++] = MY_MAC_2;
buf[L++] = MY_MAC_3;
buf[L++] = MY_MAC_4;
buf[L++] = MY_MAC_5;

// packet data unit
buf[L++] = 2; //flags lenght(LE-only, limited discovery mode)
buf[L++] = 0x01; // compulsory flags
buf[L++] = 0x06; // flag value
buf[L++] =  0x03;  // Length of next block
buf[L++] =  0x03;  // Param: Service List
buf[L++] =  0xAA;  // Eddystone ID - 16 bit 0xFEAA
buf[L++] =  0xFE;  // Eddystone ID
buf[L++] =  0x11;  // Length of next block
buf[L++] =  0x16;  // Service Data
buf[L++] =  0xAA;  // Eddystone ID
buf[L++] =  0xFE;  // Eddystone ID
buf[L++] =  0x20;  // TLM flag
buf[L++] =  0x00;  // TLM version
buf[L++] =  vbatt>>8;  // Battery voltage
buf[L++] =  vbatt;  // Battery voltage
buf[L++] =  0x80;  // temperature 8.8 fixed point
buf[L++] =  0x00;  // temperature 8.8 fixed point
buf[L++] =  0x00;  // advertisment count 0
buf[L++] =  0x00;  // advertisment count 1
buf[L++] =  packetpersecond>>8&0xff;  // advertisment count 2
buf[L++] =  packetpersecond&0xff;  // advertisment count 3
buf[L++] =  time>>24;  // powerup time 0 
buf[L++] =  time>>16;  // powerup time 1
buf[L++] =  time>>8;  // powerup time 2
buf[L++] =  time;  // powerup time 3 in seconds times 10.

L=L+3; //crc


btLePacketEncode(buf, L, chLe[ch]);

// undo xn297 data whitening
for (uint8_t i = 0; i < L; ++i) 
{
buf[i] = buf[i] ^ xn297_scramble_rev[i+ XN297_ADDRESS_SIZE_BLE] ;
}
 

for( int i = 0 ; i < L ; i++) buffint[i] = buf[i];


xn_command( FLUSH_TX);

xn_writereg( 0 , XN_TO_TX );

payloadsize = L;
xn_writepayload( buffint , L );

ble_txtime = gettime();

return;	
}



static char checkpacket()
{
	int status = xn_readreg( 7 );

	if ( status&(1<<MASK_RX_DR) )
	{	 // rx clear bit
		// this is not working well
	 // xn_writereg( STATUS , (1<<MASK_RX_DR) );
		//RX packet received
		//return 1;
	}
	if( (status & B00001110) != B00001110 )
	{
		// rx fifo not empty		
		return 2;	
	}
	
  return 0;
}


int rxdata[15];


float packettodata( int *  data)
{
	return ( ( ( data[0]&0x0003) * 256 + data[1] ) - 512 ) * 0.001953125 ;	
}


static int decodepacket( void)
{
	if ( rxdata[0] == 165 )
	{
		 int sum = 0;
		 for(int i=0; i<14; i++) 
		 {
			sum += rxdata[i];
		 }	
		if ( (sum&0xFF) == rxdata[14] )
		{
            rx[0] = packettodata( &rxdata[4] );
            rx[1] = packettodata( &rxdata[6] );
            rx[2] = packettodata( &rxdata[10] );
			// throttle		
            rx[3] = ( (rxdata[8]&0x0003) * 256 + rxdata[9] ) * 0.000976562;  

		

		if  (rxdata[1] != 0xfa) 
		{// low rates
			for ( int i = 0 ; i <3; i++)
			{
				rx[i] = rx[i] * (float) BAYANG_LOWRATE_MULTIPLIER;
			}
		}

//
        #ifdef f042_1s_bayang
            if(tx_config){
                if(showcase==0 && rx[3]>0.01f){
                    aux[ARMING] = 1;
                }
                else{
                   aux[ARMING] = 0; 
                }
                aux[11] = (rxdata[2] & 0x04) ? 1 : 0;
                aux[12] = (rxdata[2] & 0x40) ? 1 : 0;
                
                if(mode_config==0){
                    aux[LEVELMODE] = 0;
                    aux[RACEMODE] = 0;
                    aux[HORIZON] = 0;
                }
                else if(mode_config==1){
                    aux[LEVELMODE] = 1;
                    aux[RACEMODE] = 0;
                    aux[HORIZON] = 0;
                }
                else if(mode_config==2){
                    aux[LEVELMODE] = 1;
                    aux[RACEMODE] = 1;
                    aux[HORIZON] = 0;
                }
                else if(mode_config==3){
                    aux[LEVELMODE] = 1;
                    aux[RACEMODE] = 1;
                    aux[HORIZON] = 1;
                }
                else{
                    aux[LEVELMODE] = 1;
                    aux[RACEMODE] = 0;
                    aux[HORIZON] = 1;
                }
            }
            else{
                if(T8SG_config){
                    aux[CH_RTH] = (rxdata[3] & 0x80)?1:0; // inverted flag   //6 chan                
                    aux[CH_VID] = (rxdata[2] & 0x10) ? 1 : 0;                //7 chan                                       
                    aux[CH_PIC] = (rxdata[2] & 0x20) ? 1 : 0;		             //8 chan										
                    aux[CH_FLIP] = (rxdata[2] & 0x08) ? 1 : 0;               //0 chan
                    aux[CH_EXPERT] = (rxdata[1] == 0xfa) ? 1 : 0;            //1 chan
                    aux[CH_HEADFREE] = (rxdata[2] & 0x02) ? 1 : 0;           //2 chan
                    aux[CH_INV] = (rxdata[2] & 0x01) ? 1 : 0;	// rth channel  //3chan
                }
                else{
                    aux[CH_INV] = (rxdata[3] & 0x80)?1:0; // inverted flag   //6 chan                
                    aux[CH_VID] = (rxdata[2] & 0x10) ? 1 : 0;                //7 chan                                       
                    aux[CH_PIC] = (rxdata[2] & 0x20) ? 1 : 0;		             //8 chan										
                    aux[CH_FLIP] = (rxdata[2] & 0x08) ? 1 : 0;               //0 chan
                    aux[CH_EXPERT] = (rxdata[1] == 0xfa) ? 1 : 0;            //1 chan
                    aux[CH_HEADFREE] = (rxdata[2] & 0x02) ? 1 : 0;           //2 chan
                    aux[CH_RTH] = (rxdata[2] & 0x01) ? 1 : 0;	// rth channel  //3chan
                }
            }	
            
			aux[LEDS_ON] = led_config;	
        #else
            aux[CH_INV] = (rxdata[3] & 0x80)?1:0; // inverted flag   //6 chan                
            aux[CH_VID] = (rxdata[2] & 0x10) ? 1 : 0;                //7 chan                                       
            aux[CH_PIC] = (rxdata[2] & 0x20) ? 1 : 0;		             //8 chan										
            aux[CH_FLIP] = (rxdata[2] & 0x08) ? 1 : 0;               //0 chan
            aux[CH_EXPERT] = (rxdata[1] == 0xfa) ? 1 : 0;            //1 chan
            aux[CH_HEADFREE] = (rxdata[2] & 0x02) ? 1 : 0;           //2 chan
            aux[CH_RTH] = (rxdata[2] & 0x01) ? 1 : 0;	// rth channel 
        #endif
            if (aux[LEVELMODE])    //8
            {			//2                     7
                    if (aux[RACEMODE] && !aux[HORIZON])
                    {
                            if ( ANGLE_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ANGLE_EXPO_ROLL);
                            if ( ACRO_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ACRO_EXPO_PITCH);
                            if ( ANGLE_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ANGLE_EXPO_YAW);
                            
                    }
                    else if (aux[HORIZON])
                    {
                            if ( ANGLE_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ACRO_EXPO_ROLL);
                            if ( ACRO_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ACRO_EXPO_PITCH);
                            if ( ANGLE_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ANGLE_EXPO_YAW);
                            
                    }
                    else
                    {
                            if ( ANGLE_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ANGLE_EXPO_ROLL);
                            if ( ANGLE_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ANGLE_EXPO_PITCH);
                            if ( ANGLE_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ANGLE_EXPO_YAW);
                            
                    }
            }
            else
            {
                    if ( ACRO_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ACRO_EXPO_ROLL);
                    if ( ACRO_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ACRO_EXPO_PITCH);
                    if ( ACRO_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ACRO_EXPO_YAW);
                    
            }		

			for ( int i = 0 ; i < AUXNUMBER - 2 ; i++)
			{
				auxchange[i] = 0;
				if ( lastaux[i] != aux[i] ) auxchange[i] = 1;
				lastaux[i] = aux[i];
			}
			
			return 1;	// valid packet	
		}
	 return 0; // sum fail
	}
return 0; // first byte different
}



void nextchannel()
{
	rf_chan++;
	rf_chan%=4;
	xn_writereg(0x25, rfchannel[rf_chan]);
}



unsigned long lastrxtime;
unsigned long failsafetime;
unsigned long secondtimer;

//int failsafe = 0;


unsigned int skipchannel = 0;
int lastrxchan;
int timingfail = 0;

// packet period in uS
#define PACKET_PERIOD 3000

// was 250 ( uS )
#define PACKET_OFFSET 250

#ifdef USE_STOCK_TX
#undef PACKET_OFFSET
#define PACKET_OFFSET -250
#endif

// how many times to hop ahead if no reception
#define HOPPING_NUMBER 4

void checkrx(void)
{
	int packetreceived = checkpacket();
	int pass = 0;
	if (packetreceived)
	  {
		  if (rxmode == RXMODE_BIND)
		    {		// rx startup , bind mode	
			    xn_readpayload(rxdata, 15);     //Read 15 bytes
					
			    if (rxdata[0] == 164)
			      {	// bind packet
				      rfchannel[0] = rxdata[6];
				      rfchannel[1] = rxdata[7];
				      rfchannel[2] = rxdata[8];
				      rfchannel[3] = rxdata[9];
							
							int rxaddress[5];
				      rxaddress[0] = rxdata[1];
				      rxaddress[1] = rxdata[2];
				      rxaddress[2] = rxdata[3];
				      rxaddress[3] = rxdata[4];
				      rxaddress[4] = rxdata[5];
				      
				      xn_writerxaddress(rxaddress);
				      xn_writereg(0x25, rfchannel[rf_chan]);	// Set channel frequency 
							rxmode = RXMODE_NORMAL;

#ifdef SERIAL
				      printf(" BIND \n");
#endif
			      }
		    }
		  else
		    {		// normal mode  
#ifdef RXDEBUG
			    channelcount[rf_chan]++;
			    packettime = gettime() - lastrxtime;
					
					if ( skipchannel&& !timingfail ) afterskip[skipchannel]++;
					if ( timingfail ) afterskip[0]++;

#endif

unsigned long temptime = gettime();
	
			    nextchannel();

			    xn_readpayload(rxdata, 15);
			    pass = decodepacket();

			    if (pass)
			      {
#ifdef RXDEBUG
				      packetrx++;
#endif
							skipchannel = 0;
							timingfail = 0;
							lastrxchan = rf_chan;
							lastrxtime = temptime;
				      failsafetime = temptime;
				      failsafe = 0;
			      }
			    else
			      {
#ifdef RXDEBUG
				      failcount++;
#endif
			      }

		    }		// end normal rx mode
				bind_safety++;
				if (bind_safety > 9){								//requires 10 good frames to come in before rx_ready safety can be toggled to 1
				rx_ready = 1;											// because aux channels initialize low and clear the binding while armed flag before aux updates high
				bind_safety = 10;	}
	  }			// end packet received

	beacon_sequence();
		
	unsigned long time = gettime();

		

	// sequence period 12000
	if (time - lastrxtime > (HOPPING_NUMBER*PACKET_PERIOD + 1000) && rxmode != RXMODE_BIND)
	  {			
			//  channel with no reception   
		  lastrxtime = time;
			// set channel to last with reception
			if (!timingfail) rf_chan = lastrxchan;
			// advance to next channel
		  nextchannel();
			// set flag to discard packet timing
			timingfail = 1;
	  }
		
	if ( !timingfail && !ble_send && skipchannel < HOPPING_NUMBER+1 && rxmode != RXMODE_BIND)
		{
			unsigned int temp = time - lastrxtime ;

			if ( temp > 1000 && ( temp - (PACKET_OFFSET) )/((int) PACKET_PERIOD) >= (skipchannel + 1) ) 
			{
				nextchannel();
#ifdef RXDEBUG				
				skipstats[skipchannel]++;
#endif				
				skipchannel++;
			}
		}	
	
	if (time - failsafetime > FAILSAFETIME)
	  {	//  failsafe
		  failsafe = 1;
		  rx[0] = 0;
		  rx[1] = 0;
		  rx[2] = 0;
		  rx[3] = 0;
	  }
#ifdef RXDEBUG
	if (gettime() - secondtimer > 1000000)
	  {
		  packetpersecond = packetrx;
		  packetrx = 0;
		  secondtimer = gettime();
	  }
#endif

}


/****************************************************************************SBUS***********************************************************************/
void sbus_init(void)
{
    // make sure there is some time to program the board
    if ( gettime() < 2000000 ) return;
    
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
#ifdef ENABLE_UART1_TX 
    GPIO_InitStructure.GPIO_Pin = SERIAL_RX_PIN | SERIAL_TX_PIN;
    GPIO_Init(SERIAL_RX_PORT, &GPIO_InitStructure); 
    GPIO_PinAFConfig(SERIAL_RX_PORT, SERIAL_RX_SOURCE | SERIAL_TX_SOURCE , SERIAL_RX_CHANNEL);
#else 
    GPIO_InitStructure.GPIO_Pin = SERIAL_RX_PIN ;
    GPIO_Init(SERIAL_RX_PORT, &GPIO_InitStructure); 
    GPIO_PinAFConfig(SERIAL_RX_PORT, SERIAL_RX_SOURCE  , SERIAL_RX_CHANNEL);
#endif
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = SERIAL_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_2;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
#ifdef ENABLE_UART1_TX     
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//USART_Mode_Rx | USART_Mode_Tx;
#else 
    USART_InitStructure.USART_Mode = USART_Mode_Rx ;//USART_Mode_Rx ;
#endif
    USART_Init(USART1, &USART_InitStructure);

// invert signal ( default sbus )
   if (SBUS_INVERT) USART_InvPinCmd(USART1, USART_InvPin_Rx , ENABLE );//USART_InvPin_Tx

// swap rx/tx pins
#ifdef SERIAL_RX_SWD
    USART_SWAPPinCmd( USART1, ENABLE);
#endif
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART1, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    rxmode = !RXMODE_BIND;

// set setup complete flag
 framestarted = 0;
}


void sbus_rx_init(void)
{
    
}

int Tempch[4];
void sbus_checkrx()
{
 
if ( framestarted == 0)
{
    while (  rx_end != rx_start )
    { 
    if ( rx_buffer[rx_start] == 0x0f )
            {
                // start detected
                framestart = rx_start;
                framestarted = 1;  
                stat_framestartcount++; 
            break;                
            }         
    rx_start++;
    rx_start%=(RX_BUFF_SIZE);
            
    stat_garbage++;
    }
            
}
else if ( framestarted == 1)
{
 // frame already has begun
 int size = 0;
    if (rx_end > framestart ) size = rx_end - framestart;
    else size = RX_BUFF_SIZE - framestart + rx_end;
 if ( size >= 24 )
    {    
    int timing_fail = 0; 
        
    for ( int i = framestart ; i <framestart + 25; i++  )  
    {
      data[ i - framestart] = rx_buffer[i%(RX_BUFF_SIZE)];
      int symboltime = rx_time[i%(RX_BUFF_SIZE)];
      //stat_timing[ i - framestart] = symboltime;
      if ( symboltime > 1024 &&  i - framestart > 0 ) timing_fail = 1;
    }    

   if (!timing_fail) 
   {
       frame_received = 1;
          
      if (data[23] & (1<<2)) 
      {       
       // frame lost bit
       if ( !time_siglost ) time_siglost = gettime();
       if ( gettime() - time_siglost > 1000000 ) 
       {
           failsafe_siglost = 1;   
       }
      }
      else
      {
        time_siglost = 0;  
        failsafe_siglost = 0;
      }

      
      if (data[23] & (1<<3)) 
      {
        // failsafe bit
        failsafe_sbus_failsafe = 1;
      }
      else{
          failsafe_sbus_failsafe = 0;
      }
      
           
   }else if (sbus_stats) stat_timing_fail++; 
    
   last_byte = data[24];

  
    rx_start = rx_end;
    framestarted = 0;
    bind_safety++;
    } // end frame complete  
    
}// end frame pending
else
    if ( framestarted < 0)
    {
        // initialize sbus
      sbus_init();
       // set in routine above "framestarted = 0;"    
    }
      
if ( frame_received )
{

    sbus_channels[0]  = ((data[1]|data[2]<< 8) & 0x07FF);
    sbus_channels[1]  = ((data[2]>>3|data[3]<<5) & 0x07FF);
    sbus_channels[2]  = ((data[3]>>6|data[4]<<2|data[5]<<10) & 0x07FF);
	
    sbus_channels[3]  = ((data[5]>>1|data[6]<<7) & 0x07FF);
    sbus_channels[4]  = ((data[6]>>4|data[7]<<4) & 0x07FF);
    sbus_channels[5]  = ((data[7]>>7|data[8]<<1|data[9]<<9) & 0x07FF);
    sbus_channels[6]  = ((data[9]>>2|data[10]<<6) & 0x07FF);
    sbus_channels[7]  = ((data[10]>>5|data[11]<<3) & 0x07FF);
    sbus_channels[8]  = ((data[12]|data[13]<< 8) & 0x07FF);
    Tempch[0] = sbus_channels[0];
    Tempch[1] = sbus_channels[1];
    Tempch[2] = sbus_channels[2];
    Tempch[3] = sbus_channels[3];
    if ( rx_state == 0)
    {
     // wait for valid sbus signal
     static int frame_count = 0; 
     failsafe = 1;
     rxmode = RXMODE_BIND; 
     // if throttle < 10%   
     if (  sbus_channels[2] < 336 ) frame_count++;
     if (frame_count  > 130 )
     {
         if( stat_frames_second > 30 )
         {
             rx_state++; 
             rxmode = !RXMODE_BIND; 
         }
         else
         {
             frame_count = 0;
         }
     }
      
    }
    
    if ( rx_state == 1)
    {
      // normal rx mode
        
      // AETR channel order
        sbus_channels[0] -= 993;           
        sbus_channels[1] -= 993;
        sbus_channels[3] -= 993;      
        
        rx[0] = sbus_channels[0];  
        rx[1] = sbus_channels[1]; 
        rx[2] = sbus_channels[3];  
      
        for ( int i = 0 ; i < 3 ; i++)
        {
         rx[i] *= 0.00122026f;             
        }
        
        sbus_channels[2]-= 173; 
        rx[3] = 0.000610128f * sbus_channels[2]; 
        
        if ( rx[3] > 1 ) rx[3] = 1;
				
							if (aux[LEVELMODE]){
								if (aux[RACEMODE] && !aux[HORIZON]){
									
									if ( ANGLE_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ANGLE_EXPO_ROLL);
									if ( ACRO_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ACRO_EXPO_PITCH);
									if ( ANGLE_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ANGLE_EXPO_YAW);
								}else if (aux[HORIZON]){
									
									if ( ANGLE_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ACRO_EXPO_ROLL);
									if ( ACRO_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ACRO_EXPO_PITCH);
									if ( ANGLE_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ANGLE_EXPO_YAW);
								}else{
									
									if ( ANGLE_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ANGLE_EXPO_ROLL);
									if ( ANGLE_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ANGLE_EXPO_PITCH);
									if ( ANGLE_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ANGLE_EXPO_YAW);}
							}else{
								
								if ( ACRO_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ACRO_EXPO_ROLL);
								if ( ACRO_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ACRO_EXPO_PITCH);
								if ( ACRO_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ACRO_EXPO_YAW);
							}
							
			aux[CHAN_5] = (sbus_channels[4] > 993) ? 1 : 0;
		    aux[CHAN_6] = (sbus_channels[5] > 993) ? 1 : 0;
		    aux[CHAN_7] = (sbus_channels[6] > 993) ? 1 : 0;
		    aux[CHAN_8] = (sbus_channels[7] > 993) ? 1 : 0;
			aux[CHAN_9] = (sbus_channels[8] > 993) ? 1 : 0;
        
        time_lastframe = gettime(); 
        if (sbus_stats) stat_frames_accepted++;
				if (bind_safety > 9){								//requires 10 good frames to come in before rx_ready safety can be toggled to 1
				rx_ready = 1;											// because aux channels initialize low and clear the binding while armed flag before aux updates high
				bind_safety = 10;}								
    }
 

// stats
    static int fps = 0;
    static unsigned long secondtime = 0;
    
    if ( gettime() - secondtime > 1000000 )
    {
       stat_frames_second = fps;
       fps = 0;
       secondtime = gettime();
    }
    fps++;
    
frame_received = 0;    
} // end frame received


if ( gettime() - time_lastframe > 1000000 )
{
    failsafe_noframes = 1;
}else failsafe_noframes = 0;

// add the 3 failsafes together
    failsafe = failsafe_noframes || failsafe_siglost || failsafe_sbus_failsafe;

}



/**************************************************************DSMX**************************************************************************/
#ifndef BUZZER_ENABLE 																									// use the convenience macros from buzzer.c for bind pulses
#define PIN_OFF( port , pin ) GPIO_ResetBits( port , pin)
#define PIN_ON( port , pin ) GPIO_SetBits( port , pin)
#endif

 //Receive ISR callback
void USART1_IRQHandler(void)
{ 
    static uint8_t spekFramePosition = 0;
    unsigned long  maxticks = SysTick->LOAD;	
    unsigned long ticks = SysTick->VAL;	
    unsigned long spekTimeInterval;	
    static unsigned long lastticks;	
	  if(sbus_dsmx_flag == 0)    //DSMX
		{
				if (ticks < lastticks) 
						spekTimeInterval = lastticks - ticks;	
				else
						{// overflow ( underflow really)
						spekTimeInterval = lastticks + ( maxticks - ticks);	
						}
				lastticks = ticks;
			
				if ( USART_GetFlagStatus(USART1 , USART_FLAG_ORE ) ){
					// overflow means something was lost 
					USART_ClearFlag( USART1 , USART_FLAG_ORE );
				}    	
				if (spekTimeInterval > SPEKTRUM_NEEDED_FRAME_INTERVAL) {
						spekFramePosition = 0;
				}
				if (spekFramePosition < SPEK_FRAME_SIZE) {	
					 spekFrame[spekFramePosition++] = USART_ReceiveData(USART1);		
					 if (spekFramePosition < SPEK_FRAME_SIZE) {
							 rcFrameComplete = 0;
					 }else{
							 rcFrameComplete = 1;
					 }
				}
				spekFramePosition%=(SPEK_FRAME_SIZE);
	}
	else
	{
		rx_buffer[rx_end] = USART_ReceiveData(USART1);
    // calculate timing since last rx
    unsigned long elapsedticks;	

    if (ticks < lastticks) 
        elapsedticks = lastticks - ticks;	
    else
        {// overflow ( underflow really)
        elapsedticks = lastticks + ( maxticks - ticks);	
        }

    if ( elapsedticks < 65536 ) rx_time[rx_end] = elapsedticks; //
    else rx_time[rx_end] = 65535;  //ffff

    lastticks = ticks;
       
    if ( USART_GetFlagStatus(USART1 , USART_FLAG_ORE ) )
    {
      // overflow means something was lost 
      rx_time[rx_end]= 0xFFFe;
      USART_ClearFlag( USART1 , USART_FLAG_ORE );
      if ( sbus_stats ) stat_overflow++;
    }    
        
    rx_end++;
    rx_end%=(RX_BUFF_SIZE);
	}
} 



void spektrumFrameStatus(void)
{
    if (rcFrameComplete == 0) {
			rx_frame_pending = 1;															//flags when last time through we had a frame and this time we dont
    }else{
			rcFrameComplete = 0;															//isr callback triggers alert of fresh data in buffer

			for (int b = 3; b < SPEK_FRAME_SIZE; b += 2) {                                  //stick data in channels buckets
        const uint8_t spekChannel = 0x0F & (spekFrame[b - 1] >> spek_chan_shift);
        if (spekChannel < CHANNEL_COUNT && spekChannel < SPEKTRUM_MAX_SUPPORTED_CHANNEL_COUNT) {
                dsmx_channels[spekChannel] = ((uint32_t)(spekFrame[b - 1] & spek_chan_mask) << 8) + spekFrame[b];
						  	framestarted = 1;											
								rx_frame_pending = 0;                   //flags when last time through we didn't have a frame and this time we do	
				        bind_safety++;                          // incriments up as good frames come in till we pass a safe point where aux channels are updated 
								
        }
			}     
		}		
}


void dsm_init(void)
{
    // make sure there is some time to program the board
    if ( gettime() < 2000000 ) return;    
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;   
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
#ifdef ENABLE_UART1_TX
    GPIO_InitStructure.GPIO_Pin = SERIAL_RX_PIN | SERIAL_TX_PIN;
    GPIO_Init(SERIAL_RX_PORT, &GPIO_InitStructure); 
    GPIO_PinAFConfig(SERIAL_RX_PORT, SERIAL_RX_SOURCE | SERIAL_TX_SOURCE , SERIAL_RX_CHANNEL);
#else 
    GPIO_InitStructure.GPIO_Pin = SERIAL_RX_PIN ;
    GPIO_Init(SERIAL_RX_PORT, &GPIO_InitStructure); 
    GPIO_PinAFConfig(SERIAL_RX_PORT, SERIAL_RX_SOURCE  , SERIAL_RX_CHANNEL);
#endif
    GPIO_InitStructure.GPIO_Pin = SERIAL_RX_PIN;
    GPIO_Init(SERIAL_RX_PORT, &GPIO_InitStructure); 
    GPIO_PinAFConfig(SERIAL_RX_PORT, SERIAL_RX_SOURCE , SERIAL_RX_CHANNEL);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = DSMX_SERIAL_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  
    USART_InitStructure.USART_Parity = USART_Parity_No;    //sbus is even parity
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
#ifdef ENABLE_UART1_TX     
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//USART_Mode_Rx | USART_Mode_Tx;
#else 
    USART_InitStructure.USART_Mode = USART_Mode_Rx ;//USART_Mode_Rx ;
#endif
    USART_Init(USART1, &USART_InitStructure);
// swap rx/tx pins
#ifdef SERIAL_RX_SWD
    USART_SWAPPinCmd( USART1, ENABLE);
#endif
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
// set setup complete flag
 framestarted = 0;
}




 //Send Spektrum bind pulses
void lite_2S_rx_spektrum_bind(void)
{
        GPIO_InitTypeDef    GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = SERIAL_RX_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(SERIAL_RX_PORT, &GPIO_InitStructure); 
        
        // RX line, set high
        PIN_ON(SERIAL_RX_PORT, SERIAL_RX_PIN);
        // Bind window is around 20-140ms after powerup
        delay(60000);

        for (uint8_t i = 0; i < BIND_PULSES; i++) { // 9 pulses for internal dsmx 11ms, 3 pulses for internal dsm2 22ms          
                // RX line, drive low for 120us
                PIN_OFF(SERIAL_RX_PORT, SERIAL_RX_PIN);
                delay(120);
            
                // RX line, drive high for 120us
                PIN_ON(SERIAL_RX_PORT, SERIAL_RX_PIN);
                delay(120);
		}
}

 //Send Spektrum bind pulses to a GPIO e.g. TX1
void rx_spektrum_bind(void)
{
#ifdef SERIAL_RX_PIN
	rx_bind_enable = fmc_read_float(56);
	if (rx_bind_enable == 0){
        GPIO_InitTypeDef    GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = SERIAL_RX_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(SERIAL_RX_PORT, &GPIO_InitStructure); 
        
        // RX line, set high
        PIN_ON(SERIAL_RX_PORT, SERIAL_RX_PIN);
        // Bind window is around 20-140ms after powerup
        delay(60000);

        for (uint8_t i = 0; i < BIND_PULSES; i++) { // 9 pulses for internal dsmx 11ms, 3 pulses for internal dsm2 22ms          
                // RX line, drive low for 120us
                PIN_OFF(SERIAL_RX_PORT, SERIAL_RX_PIN);
                delay(120);
            
                // RX line, drive high for 120us
                PIN_ON(SERIAL_RX_PORT, SERIAL_RX_PIN);
                delay(120);
        }
	}
#endif
#if defined(Lite_BrushlessRX) ||defined(Lite_Brushless) ||defined(Lite_BrushedRX)
    // Lite_BrushlessRX & Lite_Brushless use the bind key for binding
#else
        GPIO_InitTypeDef    GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = SERIAL_RX_SPEKBIND_BINDTOOL_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(SERIAL_RX_PORT, &GPIO_InitStructure); 
        
        // RX line, set high
        PIN_ON(SERIAL_RX_PORT, SERIAL_RX_SPEKBIND_BINDTOOL_PIN);
        // Bind window is around 20-140ms after powerup
        delay(60000);

        for (uint8_t i = 0; i < BIND_PULSES; i++) { // 9 pulses for internal dsmx 11ms, 3 pulses for internal dsm2 22ms          
                // RX line, drive low for 120us
                PIN_OFF(SERIAL_RX_PORT, SERIAL_RX_SPEKBIND_BINDTOOL_PIN);
                delay(120);
            
                // RX line, drive high for 120us
                PIN_ON(SERIAL_RX_PORT, SERIAL_RX_SPEKBIND_BINDTOOL_PIN);
                delay(120);
        }	
#endif
}

void dsmx_rx_init(void)
{
    
}

void dsmx_checkrx()
{
if ( framestarted < 0){									
			failsafe = 1;																														//kill motors while initializing usart (maybe not necessary)		  
      dsm_init();																															// toggles "framestarted = 0;" after initializing
			rxmode = !RXMODE_BIND; 																									// put LEDS in normal signal status
}   																													

if ( framestarted == 0){ 																											// this is the failsafe condition
		failsafe = 1;																															//keeps motors off while waiting for first frame and if no new frame for more than 1s
} 
 

rx_frame_pending_last = rx_frame_pending;
spektrumFrameStatus();		
if (rx_frame_pending != rx_frame_pending_last) flagged_time = gettime();  		//updates flag to current time only on changes of losing a frame or getting one back
if (gettime() - flagged_time > FAILSAFETIME) framestarted = 0;            		//watchdog if more than 1 sec passes without a frame causes failsafe
		
         
if ( framestarted == 1){
				if ((bind_safety < 900) && (bind_safety > 0)) rxmode = RXMODE_BIND;																								// normal rx mode - removes waiting for bind led leaving failsafe flashes as data starts to come in
		   
      // AETR channel order
	#ifdef RX_DSMX_2048

        rx[0] = (dsmx_channels[1] - 1024.0f) * dsmx_scalefactor;
        rx[1] = (dsmx_channels[2] - 1024.0f) * dsmx_scalefactor;
        rx[2] = (dsmx_channels[3] - 1024.0f) * dsmx_scalefactor;
        rx[3] =((dsmx_channels[0] - 1024.0f) * dsmx_scalefactor * 0.5f) + 0.5f;

        if ( rx[3] > 1 ) rx[3] = 1;	
        if ( rx[3] < 0 ) rx[3] = 0;
	#endif

	#ifdef RX_DSM2_1024
        rx[0] = (dsmx_channels[1]*0.00199601f)-1.02195767f;  
        rx[1] = (dsmx_channels[2]*0.00199601f)-1.02195767f; 
        rx[2] = (dsmx_channels[3]*0.00199601f)-1.02195767f;
        rx[3] = (dsmx_channels[0]*0.000998005f)-0.0109780552f;
				if ( rx[3] > 1 ) rx[3] = 1;	
				if ( rx[3] < 0 ) rx[3] = 0;
	#endif
				
				if (aux[LEVELMODE]){
							if (aux[RACEMODE] && !aux[HORIZON]){
									
									if ( ANGLE_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ANGLE_EXPO_ROLL);
									if ( ACRO_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ACRO_EXPO_PITCH);
									if ( ANGLE_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ANGLE_EXPO_YAW);
							}else if (aux[HORIZON]){
									
									if ( ANGLE_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ACRO_EXPO_ROLL);
									if ( ACRO_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ACRO_EXPO_PITCH);
									if ( ANGLE_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ANGLE_EXPO_YAW);
							}else{
									
									if ( ANGLE_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ANGLE_EXPO_ROLL);
									if ( ANGLE_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ANGLE_EXPO_PITCH);
									if ( ANGLE_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ANGLE_EXPO_YAW);}
				}else{
						
						if ( ACRO_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ACRO_EXPO_ROLL);
						if ( ACRO_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ACRO_EXPO_PITCH);
						if ( ACRO_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ACRO_EXPO_YAW);
				}
							
	#ifdef RX_DSMX_2048		
				aux[CHAN_5] = (dsmx_channels[4] > 1100) ? 1 : 0;													//1100 cutoff intentionally selected to force aux channels low if 
				aux[CHAN_6] = (dsmx_channels[5] > 1100) ? 1 : 0;													//being controlled by a transmitter using a 3 pos switch in center state
				aux[CHAN_7] = (dsmx_channels[6] > 1100) ? 1 : 0;
				aux[CHAN_8] = (dsmx_channels[7] > 1100) ? 1 : 0;
				aux[CHAN_9] = (dsmx_channels[8] > 1100) ? 1 : 0;
				aux[CHAN_10] = (dsmx_channels[9] > 1100) ? 1 : 0;							
	#endif

	#ifdef RX_DSM2_1024		
				aux[CHAN_5] = (dsmx_channels[4] > 550) ? 1 : 0;													//550 cutoff intentionally selected to force aux channels low if 
				aux[CHAN_6] = (dsmx_channels[5] > 550) ? 1 : 0;													//being controlled by a transmitter using a 3 pos switch in center state
				aux[CHAN_7] = (dsmx_channels[6] > 550) ? 1 : 0;						
	#endif


				if (bind_safety > 900){								//requires 10 good frames to come in before rx_ready safety can be toggled to 1.  900 is about 2 seconds of good data
					rx_ready = 1;												// because aux channels initialize low and clear the binding while armed flag before aux updates high
					failsafe = 0;												// turn off failsafe delayed a bit to emmulate led behavior of sbus protocol - optional either here or just above here
					rxmode = !RXMODE_BIND;							// restores normal led operation
					bind_safety = 901;									// reset counter so it doesnt wrap
				}


						
	}
}	
#endif




