#pragma once
// MESSAGE other PACKING

#define MAVLINK_MSG_ID_other 13

MAVPACKED(
typedef struct __mavlink_other_t {
 float low_vol; /*<  low_vol*/
 uint8_t max_angle; /*<  max_angle*/
}) mavlink_other_t;

#define MAVLINK_MSG_ID_other_LEN 5
#define MAVLINK_MSG_ID_other_MIN_LEN 5
#define MAVLINK_MSG_ID_13_LEN 5
#define MAVLINK_MSG_ID_13_MIN_LEN 5

#define MAVLINK_MSG_ID_other_CRC 101
#define MAVLINK_MSG_ID_13_CRC 101



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_other { \
    13, \
    "other", \
    2, \
    {  { "max_angle", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_other_t, max_angle) }, \
         { "low_vol", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_other_t, low_vol) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_other { \
    "other", \
    2, \
    {  { "max_angle", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_other_t, max_angle) }, \
         { "low_vol", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_other_t, low_vol) }, \
         } \
}
#endif

/**
 * @brief Pack a other message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param max_angle  max_angle
 * @param low_vol  low_vol
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_other_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t max_angle, float low_vol)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_other_LEN];
    _mav_put_float(buf, 0, low_vol);
    _mav_put_uint8_t(buf, 4, max_angle);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_other_LEN);
#else
    mavlink_other_t packet;
    packet.low_vol = low_vol;
    packet.max_angle = max_angle;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_other_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_other;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_other_MIN_LEN, MAVLINK_MSG_ID_other_LEN, MAVLINK_MSG_ID_other_CRC);
}

/**
 * @brief Pack a other message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param max_angle  max_angle
 * @param low_vol  low_vol
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_other_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t max_angle,float low_vol)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_other_LEN];
    _mav_put_float(buf, 0, low_vol);
    _mav_put_uint8_t(buf, 4, max_angle);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_other_LEN);
#else
    mavlink_other_t packet;
    packet.low_vol = low_vol;
    packet.max_angle = max_angle;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_other_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_other;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_other_MIN_LEN, MAVLINK_MSG_ID_other_LEN, MAVLINK_MSG_ID_other_CRC);
}

/**
 * @brief Encode a other struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param other C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_other_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_other_t* other)
{
    return mavlink_msg_other_pack(system_id, component_id, msg, other->max_angle, other->low_vol);
}

/**
 * @brief Encode a other struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param other C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_other_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_other_t* other)
{
    return mavlink_msg_other_pack_chan(system_id, component_id, chan, msg, other->max_angle, other->low_vol);
}

/**
 * @brief Send a other message
 * @param chan MAVLink channel to send the message
 *
 * @param max_angle  max_angle
 * @param low_vol  low_vol
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_other_send(mavlink_channel_t chan, uint8_t max_angle, float low_vol)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_other_LEN];
    _mav_put_float(buf, 0, low_vol);
    _mav_put_uint8_t(buf, 4, max_angle);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_other, buf, MAVLINK_MSG_ID_other_MIN_LEN, MAVLINK_MSG_ID_other_LEN, MAVLINK_MSG_ID_other_CRC);
#else
    mavlink_other_t packet;
    packet.low_vol = low_vol;
    packet.max_angle = max_angle;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_other, (const char *)&packet, MAVLINK_MSG_ID_other_MIN_LEN, MAVLINK_MSG_ID_other_LEN, MAVLINK_MSG_ID_other_CRC);
#endif
}

/**
 * @brief Send a other message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_other_send_struct(mavlink_channel_t chan, const mavlink_other_t* other)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_other_send(chan, other->max_angle, other->low_vol);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_other, (const char *)other, MAVLINK_MSG_ID_other_MIN_LEN, MAVLINK_MSG_ID_other_LEN, MAVLINK_MSG_ID_other_CRC);
#endif
}

#if MAVLINK_MSG_ID_other_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_other_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t max_angle, float low_vol)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, low_vol);
    _mav_put_uint8_t(buf, 4, max_angle);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_other, buf, MAVLINK_MSG_ID_other_MIN_LEN, MAVLINK_MSG_ID_other_LEN, MAVLINK_MSG_ID_other_CRC);
#else
    mavlink_other_t *packet = (mavlink_other_t *)msgbuf;
    packet->low_vol = low_vol;
    packet->max_angle = max_angle;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_other, (const char *)packet, MAVLINK_MSG_ID_other_MIN_LEN, MAVLINK_MSG_ID_other_LEN, MAVLINK_MSG_ID_other_CRC);
#endif
}
#endif

#endif

// MESSAGE other UNPACKING


/**
 * @brief Get field max_angle from other message
 *
 * @return  max_angle
 */
static inline uint8_t mavlink_msg_other_get_max_angle(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field low_vol from other message
 *
 * @return  low_vol
 */
static inline float mavlink_msg_other_get_low_vol(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Decode a other message into a struct
 *
 * @param msg The message to decode
 * @param other C-struct to decode the message contents into
 */
static inline void mavlink_msg_other_decode(const mavlink_message_t* msg, mavlink_other_t* other)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    other->low_vol = mavlink_msg_other_get_low_vol(msg);
    other->max_angle = mavlink_msg_other_get_max_angle(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_other_LEN? msg->len : MAVLINK_MSG_ID_other_LEN;
        memset(other, 0, MAVLINK_MSG_ID_other_LEN);
    memcpy(other, _MAV_PAYLOAD(msg), len);
#endif
}
