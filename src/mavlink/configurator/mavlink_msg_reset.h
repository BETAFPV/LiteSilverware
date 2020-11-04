#pragma once
// MESSAGE reset PACKING

#define MAVLINK_MSG_ID_reset 10

MAVPACKED(
typedef struct __mavlink_reset_t {
 uint8_t reset; /*<  reset*/
}) mavlink_reset_t;

#define MAVLINK_MSG_ID_reset_LEN 1
#define MAVLINK_MSG_ID_reset_MIN_LEN 1
#define MAVLINK_MSG_ID_10_LEN 1
#define MAVLINK_MSG_ID_10_MIN_LEN 1

#define MAVLINK_MSG_ID_reset_CRC 243
#define MAVLINK_MSG_ID_10_CRC 243



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_reset { \
    10, \
    "reset", \
    1, \
    {  { "reset", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_reset_t, reset) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_reset { \
    "reset", \
    1, \
    {  { "reset", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_reset_t, reset) }, \
         } \
}
#endif

/**
 * @brief Pack a reset message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param reset  reset
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_reset_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t reset)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_reset_LEN];
    _mav_put_uint8_t(buf, 0, reset);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_reset_LEN);
#else
    mavlink_reset_t packet;
    packet.reset = reset;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_reset_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_reset;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_reset_MIN_LEN, MAVLINK_MSG_ID_reset_LEN, MAVLINK_MSG_ID_reset_CRC);
}

/**
 * @brief Pack a reset message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param reset  reset
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_reset_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t reset)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_reset_LEN];
    _mav_put_uint8_t(buf, 0, reset);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_reset_LEN);
#else
    mavlink_reset_t packet;
    packet.reset = reset;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_reset_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_reset;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_reset_MIN_LEN, MAVLINK_MSG_ID_reset_LEN, MAVLINK_MSG_ID_reset_CRC);
}

/**
 * @brief Encode a reset struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param reset C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_reset_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_reset_t* reset)
{
    return mavlink_msg_reset_pack(system_id, component_id, msg, reset->reset);
}

/**
 * @brief Encode a reset struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param reset C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_reset_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_reset_t* reset)
{
    return mavlink_msg_reset_pack_chan(system_id, component_id, chan, msg, reset->reset);
}

/**
 * @brief Send a reset message
 * @param chan MAVLink channel to send the message
 *
 * @param reset  reset
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_reset_send(mavlink_channel_t chan, uint8_t reset)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_reset_LEN];
    _mav_put_uint8_t(buf, 0, reset);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_reset, buf, MAVLINK_MSG_ID_reset_MIN_LEN, MAVLINK_MSG_ID_reset_LEN, MAVLINK_MSG_ID_reset_CRC);
#else
    mavlink_reset_t packet;
    packet.reset = reset;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_reset, (const char *)&packet, MAVLINK_MSG_ID_reset_MIN_LEN, MAVLINK_MSG_ID_reset_LEN, MAVLINK_MSG_ID_reset_CRC);
#endif
}

/**
 * @brief Send a reset message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_reset_send_struct(mavlink_channel_t chan, const mavlink_reset_t* reset)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_reset_send(chan, reset->reset);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_reset, (const char *)reset, MAVLINK_MSG_ID_reset_MIN_LEN, MAVLINK_MSG_ID_reset_LEN, MAVLINK_MSG_ID_reset_CRC);
#endif
}

#if MAVLINK_MSG_ID_reset_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_reset_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t reset)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, reset);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_reset, buf, MAVLINK_MSG_ID_reset_MIN_LEN, MAVLINK_MSG_ID_reset_LEN, MAVLINK_MSG_ID_reset_CRC);
#else
    mavlink_reset_t *packet = (mavlink_reset_t *)msgbuf;
    packet->reset = reset;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_reset, (const char *)packet, MAVLINK_MSG_ID_reset_MIN_LEN, MAVLINK_MSG_ID_reset_LEN, MAVLINK_MSG_ID_reset_CRC);
#endif
}
#endif

#endif

// MESSAGE reset UNPACKING


/**
 * @brief Get field reset from reset message
 *
 * @return  reset
 */
static inline uint8_t mavlink_msg_reset_get_reset(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a reset message into a struct
 *
 * @param msg The message to decode
 * @param reset C-struct to decode the message contents into
 */
static inline void mavlink_msg_reset_decode(const mavlink_message_t* msg, mavlink_reset_t* reset)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    reset->reset = mavlink_msg_reset_get_reset(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_reset_LEN? msg->len : MAVLINK_MSG_ID_reset_LEN;
        memset(reset, 0, MAVLINK_MSG_ID_reset_LEN);
    memcpy(reset, _MAV_PAYLOAD(msg), len);
#endif
}
