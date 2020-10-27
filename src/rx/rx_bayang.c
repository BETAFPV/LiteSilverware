#include "rx_bayang.h"
#include "bus_softspi.h"
#include "binary.h"
#include "time.h"
#include "defines.h"
#include "xn297.h"
#include "util.h"


#define TX_POWER 7
#define RX_MODE_NORMAL RXMODE_NORMAL
#define RX_MODE_BIND RXMODE_BIND
// radio settings

// packet period in uS
#define PACKET_PERIOD 3000
#define PACKET_PERIOD_TELEMETRY 5000

// was 250 ( uS )
#define PACKET_OFFSET 0

// how many times to hop ahead if no reception
#define HOPPING_NUMBER 4


float rx[4];
char aux[AUXNUMBER];
char lastaux[AUXNUMBER];
char auxchange[AUXNUMBER];
float aux_analog[AUXNUMBER];
float lastaux_analog[AUXNUMBER];
char aux_analogchange[AUXNUMBER];


char lasttrim[4];

char rfchannel[4];
char rxaddress[5];
int telemetry_enabled = 0;
int rx_bind_enable = 0;
int rx_bind_load = 0;

int rxmode = 0;
int rf_chan = 0;
int rx_ready = 0;
int bind_safety = 0;

unsigned long autobindtime = 0;
int autobind_inhibit = 0;
int packet_period = PACKET_PERIOD;

void writeregs(uint8_t data[], uint8_t size)
{
    spi_cson();
    for (uint8_t i = 0; i < size; i++)
    {
        spi_sendbyte(data[i]);
    }
    spi_csoff();
}



void rx_init(void)
{
    // always on (CH_ON) channel set 1
    aux[AUXNUMBER - 2] = 1;
// always off (CH_OFF) channel set 0
    aux[AUXNUMBER - 1] = 0;

    static uint8_t demodcal[2] = { 0x39, B00000001 };
    writeregs(demodcal, sizeof(demodcal));

// powerup defaults
//static uint8_t rfcal2[7] = { 0x3a , 0x45 , 0x21 , 0xef , 0xac , 0x3a , 0x50};
//writeregs( rfcal2 , sizeof(rfcal2) );

    static uint8_t rfcal2[7] = { 0x3a, 0x45, 0x21, 0xef, 0x2c, 0x5a, 0x50};
    writeregs(rfcal2, sizeof(rfcal2));

    static uint8_t regs_1f[6] = { 0x3f, 0x0a, 0x6d, 0x67, 0x9c, 0x46 };
    writeregs(regs_1f, sizeof(regs_1f));


    static uint8_t regs_1e[4] = { 0x3e, 0xf6, 0x37, 0x5d };
    writeregs(regs_1e, sizeof(regs_1e));

#define XN_TO_RX B10001111
#define XN_TO_TX B10000010
#define XN_POWER B00000001|((TX_POWER&7)<<3)


    delay(100);

// write rx address " 0 0 0 0 0 "

    static uint8_t rxaddr[6] = { 0x2a, 0, 0, 0, 0, 0  };
    writeregs(rxaddr, sizeof(rxaddr));

    xn_writereg(EN_AA, 0);      // aa disabled
    xn_writereg(EN_RXADDR, 1);  // pipe 0 only
    xn_writereg(RF_SETUP, XN_POWER);    // power / data rate / lna
    xn_writereg(RX_PW_P0, 15);  // payload size
    xn_writereg(SETUP_RETR, 0); // no retransmissions ( redundant?)
    xn_writereg(SETUP_AW, 3);   // address size (5 bytes)
    xn_command(FLUSH_RX);
    xn_writereg(RF_CH, 0);      // bind on channel 0


#ifdef RADIO_XN297L
    xn_writereg(0x1d, B00111000);   // 64 bit payload , software ce
    spi_cson();
    spi_sendbyte(0xFD);         // internal CE high command
    spi_sendbyte(0);            // required for above
    spi_csoff();
#endif


    xn_writereg(0, XN_TO_RX);   // power up, crc enabled, rx mode

#ifdef RADIO_CHECK
    int rxcheck = xn_readreg(0x0f); // rx address pipe 5
    // should be 0xc6
    extern void failloop(int);
    if (rxcheck != 0xc6)
//        failloop(3);
#endif

        if (rx_bind_load)
        {
            uint8_t rxaddr_regs[6] = { 0x2a,  };
            for (int i = 1 ; i < 6; i++)
            {
                rxaddr_regs[i] = rxaddress[i - 1];
            }
            // write new rx address
            writeregs(rxaddr_regs, sizeof(rxaddr_regs));
            rxaddr_regs[0] = 0x30; // tx register ( write ) number

            // write new tx address
            writeregs(rxaddr_regs, sizeof(rxaddr_regs));

            xn_writereg(0x25, rfchannel[rf_chan]);    // Set channel frequency
            rxmode = RX_MODE_NORMAL;

            if (telemetry_enabled) packet_period = PACKET_PERIOD_TELEMETRY;
        }
        else
        {
            autobind_inhibit = 1;
        }
}


int packetrx;
int packetpersecond;




static char checkpacket()
{
    int status = xn_readreg(7);

    if (status & (1 << MASK_RX_DR))
    {
        // rx clear bit
        // this is not working well
        // xn_writereg( STATUS , (1<<MASK_RX_DR) );
        //RX packet received
        //return 1;
    }
    if ((status & B00001110) != B00001110)
    {
        // rx fifo not empty
        return 2;
    }

    return 0;
}


int rxdata[15];


float packettodata(int *data)
{
    return (((data[0] & 0x0003) * 256 + data[1]) - 512) * 0.001953125;
}

float bytetodata(int byte)
{
    //return (byte - 128) * 0.0078125; // -1 to 1
    return byte * 0.00390625; // 0 to 1
}


static int decodepacket(void)
{
    if (rxdata[0] == 165)
    {
        int sum = 0;
        for (int i = 0; i < 14; i++)
        {
            sum += rxdata[i];
        }
        if ((sum & 0xFF) == rxdata[14])
        {
            rx[0] = packettodata(&rxdata[4]);
            rx[1] = packettodata(&rxdata[6]);
            rx[2] = packettodata(&rxdata[10]);
            // throttle
            rx[3] =
                ((rxdata[8] & 0x0003) * 256 +
                 rxdata[9]) * 0.000976562f;

            aux[CH_INV] = (rxdata[3] & 0x80) ? 1 : 0; // inverted flag   //6 chan
            aux[CH_VID] = (rxdata[2] & 0x10) ? 1 : 0;                //7 chan
            aux[CH_PIC] = (rxdata[2] & 0x20) ? 1 : 0;                    //8 chan
            aux[CH_FLIP] = (rxdata[2] & 0x08) ? 1 : 0;               //0 chan
            aux[CH_EXPERT] = (rxdata[1] == 0xfa) ? 1 : 0;            //1 chan
            aux[CH_HEADFREE] = (rxdata[2] & 0x02) ? 1 : 0;           //2 chan
            aux[CH_RTH] = (rxdata[2] & 0x01) ? 1 : 0;   // rth channel  //3chan

            if (aux[LEVELMODE])    //8
            {
                //2                     7
                if (aux[RACEMODE] && !aux[HORIZON])
                {
                    if (ANGLE_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ANGLE_EXPO_ROLL);
                    if (ACRO_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ACRO_EXPO_PITCH);
                    if (ANGLE_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ANGLE_EXPO_YAW);

                }
                else if (aux[HORIZON])
                {
                    if (ANGLE_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ACRO_EXPO_ROLL);
                    if (ACRO_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ACRO_EXPO_PITCH);
                    if (ANGLE_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ANGLE_EXPO_YAW);

                }
                else
                {
                    if (ANGLE_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ANGLE_EXPO_ROLL);
                    if (ANGLE_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ANGLE_EXPO_PITCH);
                    if (ANGLE_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ANGLE_EXPO_YAW);

                }
            }
            else
            {
                if (ACRO_EXPO_ROLL > 0.01) rx[0] = rcexpo(rx[0], ACRO_EXPO_ROLL);
                if (ACRO_EXPO_PITCH > 0.01) rx[1] = rcexpo(rx[1], ACRO_EXPO_PITCH);
                if (ACRO_EXPO_YAW > 0.01) rx[2] = rcexpo(rx[2], ACRO_EXPO_YAW);

            }

            for (int i = 0 ; i < AUXNUMBER - 2 ; i++)
            {
                auxchange[i] = 0;
                if (lastaux[i] != aux[i]) auxchange[i] = 1;
                lastaux[i] = aux[i];
            }

            return 1;   // valid packet
        }
        return 0; // sum fail
    }
    return 0; // first byte different
}



void nextchannel()
{
    rf_chan++;
    rf_chan &= 3; // same as %4
    xn_writereg(0x25, rfchannel[rf_chan]);
}


unsigned long lastrxtime;
unsigned long failsafetime;
unsigned long secondtimer;

int failsafe = 0;


unsigned int skipchannel = 0;
int lastrxchan;
int timingfail = 0;



void checkrx(void)
{
    int packetreceived = checkpacket();
    int pass = 0;
    if (packetreceived)
    {
        if (rxmode == RX_MODE_BIND)
        {
            // rx startup , bind mode
            xn_readpayload(rxdata, 15);

            if (rxdata[0] == 0xa4 || rxdata[0] == 0xa3)
            {
                // bind packet
                if (rxdata[0] == 0xa3)
                {
                    telemetry_enabled = 1;
                    packet_period = PACKET_PERIOD_TELEMETRY;
                }

                rfchannel[0] = rxdata[6];
                rfchannel[1] = rxdata[7];
                rfchannel[2] = rxdata[8];
                rfchannel[3] = rxdata[9];


                uint8_t rxaddr_regs[6] = { 0x2a,  };

                for (int i = 1 ; i < 6; i++)
                {
                    rxaddr_regs[i] = rxdata[i];
                    rxaddress[i - 1] = rxdata[i];
                }
                // write new rx address
                writeregs(rxaddr_regs, sizeof(rxaddr_regs));
                rxaddr_regs[0] = 0x30; // tx register ( write ) number

                // write new tx address
                writeregs(rxaddr_regs, sizeof(rxaddr_regs));

                xn_writereg(0x25, rfchannel[rf_chan]);    // Set channel frequency

                rxmode = RX_MODE_NORMAL;

                extern int rx_bind_enable;
                rx_bind_enable = 1;

//                extern void flash_save(void);
//                flash_save();
//                extern unsigned long lastlooptime;
//                lastlooptime = gettime();

            }
        }
        else
        {
            unsigned long temptime = gettime();

            xn_readpayload(rxdata, 15);
            pass = decodepacket();

            if (pass)
            {
                packetrx++;
                skipchannel = 0;
                timingfail = 0;
                lastrxchan = rf_chan;
                lastrxtime = temptime;
                failsafetime = temptime;
                failsafe = 0;

                nextchannel();
            }
            else
            {

            }
            bind_safety++;
            if (bind_safety > 9)                                //requires 10 good frames to come in before rx_ready safety can be toggled to 1
            {
                rx_ready = 1;                                           // because aux channels initialize low and clear the binding while armed flag before aux updates high
                bind_safety = 10;
            }
        }                   // end normal rx mode

    }                         // end packet received


    unsigned long time = gettime();


    if (time - lastrxtime > (HOPPING_NUMBER * packet_period + 1000)
            && rxmode != RX_MODE_BIND)
    {
        //  channel with no reception
        lastrxtime = time;
        // set channel to last with reception
        if (!timingfail)
            rf_chan = lastrxchan;
        // advance to next channel
        nextchannel();
        // set flag to discard packet timing
        timingfail = 1;
    }

    if (!timingfail && skipchannel < HOPPING_NUMBER + 1
            && rxmode != RX_MODE_BIND)
    {
        unsigned int temp = time - lastrxtime;

        if (temp > 1000
                && (temp - (PACKET_OFFSET)) / ((int) packet_period) >=
                (skipchannel + 1))
        {
            nextchannel();

            skipchannel++;
        }
    }

    if (time - failsafetime > FAILSAFETIME)
    {
        //  failsafe
        failsafe = 1;
        rx[0] = 0;
        rx[1] = 0;
        rx[2] = 0;
        rx[3] = 0;
    }


    if (gettime() - secondtimer > 1000000)
    {
        packetpersecond = packetrx;
        packetrx = 0;
        secondtimer = gettime();
    }


}







