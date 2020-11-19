#include "stm32f0xx.h"
#include "stdbool.h"



#define SERIAL_RX_PIN GPIO_Pin_7
#define SERIAL_RX_PORT GPIOB
#define SERIAL_RX_SOURCE GPIO_PinSource7
#define SERIAL_RX_CHANNEL GPIO_AF_0


#define SERIAL_BAUDRATE 100000


#define SBUS_INVERT 1




void sbus_init(void);


void sbus_check(void);


typedef struct sbusChannels_s
{
    // 176 bits of data (11 bits per channel * 16 channels) = 22 bytes.
    unsigned int chan0 : 11;
    unsigned int chan1 : 11;
    unsigned int chan2 : 11;
    unsigned int chan3 : 11;
    unsigned int chan4 : 11;
    unsigned int chan5 : 11;
    unsigned int chan6 : 11;
    unsigned int chan7 : 11;
    unsigned int chan8 : 11;
    unsigned int chan9 : 11;
    unsigned int chan10 : 11;
    unsigned int chan11 : 11;
    unsigned int chan12 : 11;
    unsigned int chan13 : 11;
    unsigned int chan14 : 11;
    unsigned int chan15 : 11;
    uint8_t flags;
} __attribute__((__packed__)) sbusChannels_t;


#define SBUS_CHANNEL_DATA_LENGTH sizeof(sbusChannels_t)


#define SBUS_FRAME_SIZE (SBUS_CHANNEL_DATA_LENGTH + 2)



#define SBUS_MAX_CHANNEL 18

#define SBUS_FLAG_SIGNAL_LOSS       (1 << 2)
#define SBUS_FLAG_FAILSAFE_ACTIVE   (1 << 3)

#define SBUS_FLAG_CHANNEL_17        (1 << 0)
#define SBUS_FLAG_CHANNEL_18        (1 << 1)

#define SBUS_DIGITAL_CHANNEL_MIN 173
#define SBUS_DIGITAL_CHANNEL_MAX 1812


struct sbusFrame_s
{
    uint8_t syncByte;
    sbusChannels_t channels;
    /**
     * The endByte is 0x00 on FrSky and some futaba RX's, on Some SBUS2 RX's the value indicates the telemetry byte that is sent after every 4th sbus frame.
     *
     * See https://github.com/cleanflight/cleanflight/issues/590#issuecomment-101027349
     * and
     * https://github.com/cleanflight/cleanflight/issues/590#issuecomment-101706023
     */
    uint8_t endByte;
} __attribute__((__packed__));

typedef union sbusFrame_u
{
    uint8_t bytes[SBUS_FRAME_SIZE];
    struct sbusFrame_s frame;
} sbusFrame_t;

typedef struct sbusFrameData_s
{
    sbusFrame_t frame;
    uint32_t startAtUs;
    uint8_t position;
    bool done;
} sbusFrameData_t;


typedef enum
{
    RX_FRAME_PENDING = 0,
    RX_FRAME_COMPLETE = (1 << 0),
    RX_FRAME_FAILSAFE = (1 << 1),
    RX_FRAME_PROCESSING_REQUIRED = (1 << 2),
    RX_FRAME_DROPPED = (1 << 3)
} rxFrameState_e;

