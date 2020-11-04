#pragma once
// MESSAGE rx PACKING

#define MAVLINK_MSG_ID_rx 7

MAVPACKED(
typedef struct __mavlink_rx_t {
 uint8_t rx_select; /*<  rx_select*/
}) mavlink_rx_t;

#define MAVLINK_MSG_ID_rx_LEN 1
#define MAVLINK_MSG_ID_rx_MIN_LEN 1
#define MAVLINK_MSG_ID_7_LEN 1
#define MAVLINK_MSG_ID_7_MIN_LEN 1

#define MAVLINK_MSG_ID_rx_CRC 205
#define MAVLINK_MSG_ID_7_CRC 205



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_rx { \
    7, \
    "rx", \
    1, \
    {  { "rx_select", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_rx_t, rx_select) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_rx { \
    "rx", \
    1, \
    {  { "rx_select", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_rx_t, rx_select) }, \
         } \
}
#endif

/**
 * @brief Pack a rx message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param rx_select  rx_select
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rx_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t rx_select)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_rx_LEN];
    _mav_put_uint8_t(buf, 0, rx_select);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_rx_LEN);
#else
    mavlink_rx_t packet;
    packet.rx_select = rx_select;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_rx_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_rx;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_rx_MIN_LEN, MAVLINK_MSG_ID_rx_LEN, MAVLINK_MSG_ID_rx_CRC);
}

/**
 * @brief Pack a rx message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param rx_select  rx_select
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rx_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t rx_select)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_rx_LEN];
    _mav_put_uint8_t(buf, 0, rx_select);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_rx_LEN);
#else
    mavlink_rx_t packet;
    packet.rx_select = rx_select;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_rx_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_rx;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_rx_MIN_LEN, MAVLINK_MSG_ID_rx_LEN, MAVLINK_MSG_ID_rx_CRC);
}

/**
 * @brief Encode a rx struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param rx C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rx_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_rx_t* rx)
{
    return mavlink_msg_rx_pack(system_id, component_id, msg, rx->rx_select);
}

/**
 * @brief Encode a rx struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param rx C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rx_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_rx_t* rx)
{
    return mavlink_msg_rx_pack_chan(system_id, component_id, chan, msg, rx->rx_select);
}

/**
 * @brief Send a rx message
 * @param chan MAVLink channel to send the message
 *
 * @param rx_select  rx_select
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_rx_send(mavlink_channel_t chan, uint8_t rx_select)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_rx_LEN];
    _mav_put_uint8_t(buf, 0, rx_select);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_rx, buf, MAVLINK_MSG_ID_rx_MIN_LEN, MAVLINK_MSG_ID_rx_LEN, MAVLINK_MSG_ID_rx_CRC);
#else
    mavlink_rx_t packet;
    packet.rx_select = rx_select;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_rx, (const char *)&packet, MAVLINK_MSG_ID_rx_MIN_LEN, MAVLINK_MSG_ID_rx_LEN, MAVLINK_MSG_ID_rx_CRC);
#endif
}

/**
 * @brief Send a rx message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_rx_send_struct(mavlink_channel_t chan, const mavlink_rx_t* rx)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_rx_send(chan, rx->rx_select);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_rx, (const char *)rx, MAVLINK_MSG_ID_rx_MIN_LEN, MAVLINK_MSG_ID_rx_LEN, MAVLINK_MSG_ID_rx_CRC);
#endif
}

#if MAVLINK_MSG_ID_rx_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_rx_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t rx_select)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, rx_select);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_rx, buf, MAVLINK_MSG_ID_rx_MIN_LEN, MAVLINK_MSG_ID_rx_LEN, MAVLINK_MSG_ID_rx_CRC);
#else
    mavlink_rx_t *packet = (mavlink_rx_t *)msgbuf;
    packet->rx_select = rx_select;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_rx, (const char *)packet, MAVLINK_MSG_ID_rx_MIN_LEN, MAVLINK_MSG_ID_rx_LEN, MAVLINK_MSG_ID_rx_CRC);
#endif
}
#endif

#endif

// MESSAGE rx UNPACKING


/**
 * @brief Get field rx_select from rx message
 *
 * @return  rx_select
 */
static inline uint8_t mavlink_msg_rx_get_rx_select(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a rx message into a struct
 *
 * @param msg The message to decode
 * @param rx C-struct to decode the message contents into
 */
static inline void mavlink_msg_rx_decode(const mavlink_message_t* msg, mavlink_rx_t* rx)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    rx->rx_select = mavlink_msg_rx_get_rx_select(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_rx_LEN? msg->len : MAVLINK_MSG_ID_rx_LEN;
        memset(rx, 0, MAVLINK_MSG_ID_rx_LEN);
    memcpy(rx, _MAV_PAYLOAD(msg), len);
#endif
}
