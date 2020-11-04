#pragma once
// MESSAGE attitude PACKING

#define MAVLINK_MSG_ID_attitude 5

MAVPACKED(
    typedef struct __mavlink_attitude_t
{
    uint32_t time_boot_ms; /*< [ms] Timestamp*/
    float accroll; /*<  accroll*/
    float accpitch; /*<  accpitch*/
    float accyaw; /*<  accyaw*/
    float gyroroll; /*<  gyroroll*/
    float gyropitch; /*<  gyropitch*/
    float gyroyaw; /*<  gyroyaw*/
}) mavlink_attitude_t;

#define MAVLINK_MSG_ID_attitude_LEN 28
#define MAVLINK_MSG_ID_attitude_MIN_LEN 28
#define MAVLINK_MSG_ID_5_LEN 28
#define MAVLINK_MSG_ID_5_MIN_LEN 28

#define MAVLINK_MSG_ID_attitude_CRC 243
#define MAVLINK_MSG_ID_5_CRC 243



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_attitude { \
    5, \
    "attitude", \
    7, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_attitude_t, time_boot_ms) }, \
         { "accroll", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_attitude_t, accroll) }, \
         { "accpitch", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_attitude_t, accpitch) }, \
         { "accyaw", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_attitude_t, accyaw) }, \
         { "gyroroll", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_attitude_t, gyroroll) }, \
         { "gyropitch", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_attitude_t, gyropitch) }, \
         { "gyroyaw", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_attitude_t, gyroyaw) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_attitude { \
    "attitude", \
    7, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_attitude_t, time_boot_ms) }, \
         { "accroll", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_attitude_t, accroll) }, \
         { "accpitch", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_attitude_t, accpitch) }, \
         { "accyaw", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_attitude_t, accyaw) }, \
         { "gyroroll", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_attitude_t, gyroroll) }, \
         { "gyropitch", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_attitude_t, gyropitch) }, \
         { "gyroyaw", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_attitude_t, gyroyaw) }, \
         } \
}
#endif

/**
 * @brief Pack a attitude message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp
 * @param accroll  accroll
 * @param accpitch  accpitch
 * @param accyaw  accyaw
 * @param gyroroll  gyroroll
 * @param gyropitch  gyropitch
 * @param gyroyaw  gyroyaw
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_attitude_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
        uint32_t time_boot_ms, float accroll, float accpitch, float accyaw, float gyroroll, float gyropitch, float gyroyaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_attitude_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, accroll);
    _mav_put_float(buf, 8, accpitch);
    _mav_put_float(buf, 12, accyaw);
    _mav_put_float(buf, 16, gyroroll);
    _mav_put_float(buf, 20, gyropitch);
    _mav_put_float(buf, 24, gyroyaw);

    memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_attitude_LEN);
#else
    mavlink_attitude_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.accroll = accroll;
    packet.accpitch = accpitch;
    packet.accyaw = accyaw;
    packet.gyroroll = gyroroll;
    packet.gyropitch = gyropitch;
    packet.gyroyaw = gyroyaw;

    memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_attitude_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_attitude;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_attitude_MIN_LEN, MAVLINK_MSG_ID_attitude_LEN, MAVLINK_MSG_ID_attitude_CRC);
}

/**
 * @brief Pack a attitude message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Timestamp
 * @param accroll  accroll
 * @param accpitch  accpitch
 * @param accyaw  accyaw
 * @param gyroroll  gyroroll
 * @param gyropitch  gyropitch
 * @param gyroyaw  gyroyaw
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_attitude_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
        mavlink_message_t* msg,
        uint32_t time_boot_ms,float accroll,float accpitch,float accyaw,float gyroroll,float gyropitch,float gyroyaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_attitude_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, accroll);
    _mav_put_float(buf, 8, accpitch);
    _mav_put_float(buf, 12, accyaw);
    _mav_put_float(buf, 16, gyroroll);
    _mav_put_float(buf, 20, gyropitch);
    _mav_put_float(buf, 24, gyroyaw);

    memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_attitude_LEN);
#else
    mavlink_attitude_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.accroll = accroll;
    packet.accpitch = accpitch;
    packet.accyaw = accyaw;
    packet.gyroroll = gyroroll;
    packet.gyropitch = gyropitch;
    packet.gyroyaw = gyroyaw;

    memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_attitude_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_attitude;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_attitude_MIN_LEN, MAVLINK_MSG_ID_attitude_LEN, MAVLINK_MSG_ID_attitude_CRC);
}

/**
 * @brief Encode a attitude struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param attitude C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_attitude_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_attitude_t* attitude)
{
    return mavlink_msg_attitude_pack(system_id, component_id, msg, attitude->time_boot_ms, attitude->accroll, attitude->accpitch, attitude->accyaw, attitude->gyroroll, attitude->gyropitch, attitude->gyroyaw);
}

/**
 * @brief Encode a attitude struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param attitude C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_attitude_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_attitude_t* attitude)
{
    return mavlink_msg_attitude_pack_chan(system_id, component_id, chan, msg, attitude->time_boot_ms, attitude->accroll, attitude->accpitch, attitude->accyaw, attitude->gyroroll, attitude->gyropitch, attitude->gyroyaw);
}

/**
 * @brief Send a attitude message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Timestamp
 * @param accroll  accroll
 * @param accpitch  accpitch
 * @param accyaw  accyaw
 * @param gyroroll  gyroroll
 * @param gyropitch  gyropitch
 * @param gyroyaw  gyroyaw
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_attitude_send(mavlink_channel_t chan, uint32_t time_boot_ms, float accroll, float accpitch, float accyaw, float gyroroll, float gyropitch, float gyroyaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_attitude_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, accroll);
    _mav_put_float(buf, 8, accpitch);
    _mav_put_float(buf, 12, accyaw);
    _mav_put_float(buf, 16, gyroroll);
    _mav_put_float(buf, 20, gyropitch);
    _mav_put_float(buf, 24, gyroyaw);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_attitude, buf, MAVLINK_MSG_ID_attitude_MIN_LEN, MAVLINK_MSG_ID_attitude_LEN, MAVLINK_MSG_ID_attitude_CRC);
#else
    mavlink_attitude_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.accroll = accroll;
    packet.accpitch = accpitch;
    packet.accyaw = accyaw;
    packet.gyroroll = gyroroll;
    packet.gyropitch = gyropitch;
    packet.gyroyaw = gyroyaw;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_attitude, (const char *)&packet, MAVLINK_MSG_ID_attitude_MIN_LEN, MAVLINK_MSG_ID_attitude_LEN, MAVLINK_MSG_ID_attitude_CRC);
#endif
}

/**
 * @brief Send a attitude message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_attitude_send_struct(mavlink_channel_t chan, const mavlink_attitude_t* attitude)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_attitude_send(chan, attitude->time_boot_ms, attitude->accroll, attitude->accpitch, attitude->accyaw, attitude->gyroroll, attitude->gyropitch, attitude->gyroyaw);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_attitude, (const char *)attitude, MAVLINK_MSG_ID_attitude_MIN_LEN, MAVLINK_MSG_ID_attitude_LEN, MAVLINK_MSG_ID_attitude_CRC);
#endif
}

#if MAVLINK_MSG_ID_attitude_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_attitude_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, float accroll, float accpitch, float accyaw, float gyroroll, float gyropitch, float gyroyaw)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_float(buf, 4, accroll);
    _mav_put_float(buf, 8, accpitch);
    _mav_put_float(buf, 12, accyaw);
    _mav_put_float(buf, 16, gyroroll);
    _mav_put_float(buf, 20, gyropitch);
    _mav_put_float(buf, 24, gyroyaw);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_attitude, buf, MAVLINK_MSG_ID_attitude_MIN_LEN, MAVLINK_MSG_ID_attitude_LEN, MAVLINK_MSG_ID_attitude_CRC);
#else
    mavlink_attitude_t *packet = (mavlink_attitude_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->accroll = accroll;
    packet->accpitch = accpitch;
    packet->accyaw = accyaw;
    packet->gyroroll = gyroroll;
    packet->gyropitch = gyropitch;
    packet->gyroyaw = gyroyaw;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_attitude, (const char *)packet, MAVLINK_MSG_ID_attitude_MIN_LEN, MAVLINK_MSG_ID_attitude_LEN, MAVLINK_MSG_ID_attitude_CRC);
#endif
}
#endif

#endif

// MESSAGE attitude UNPACKING


/**
 * @brief Get field time_boot_ms from attitude message
 *
 * @return [ms] Timestamp
 */
static inline uint32_t mavlink_msg_attitude_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field accroll from attitude message
 *
 * @return  accroll
 */
static inline float mavlink_msg_attitude_get_accroll(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field accpitch from attitude message
 *
 * @return  accpitch
 */
static inline float mavlink_msg_attitude_get_accpitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field accyaw from attitude message
 *
 * @return  accyaw
 */
static inline float mavlink_msg_attitude_get_accyaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field gyroroll from attitude message
 *
 * @return  gyroroll
 */
static inline float mavlink_msg_attitude_get_gyroroll(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field gyropitch from attitude message
 *
 * @return  gyropitch
 */
static inline float mavlink_msg_attitude_get_gyropitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field gyroyaw from attitude message
 *
 * @return  gyroyaw
 */
static inline float mavlink_msg_attitude_get_gyroyaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Decode a attitude message into a struct
 *
 * @param msg The message to decode
 * @param attitude C-struct to decode the message contents into
 */
static inline void mavlink_msg_attitude_decode(const mavlink_message_t* msg, mavlink_attitude_t* attitude)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    attitude->time_boot_ms = mavlink_msg_attitude_get_time_boot_ms(msg);
    attitude->accroll = mavlink_msg_attitude_get_accroll(msg);
    attitude->accpitch = mavlink_msg_attitude_get_accpitch(msg);
    attitude->accyaw = mavlink_msg_attitude_get_accyaw(msg);
    attitude->gyroroll = mavlink_msg_attitude_get_gyroroll(msg);
    attitude->gyropitch = mavlink_msg_attitude_get_gyropitch(msg);
    attitude->gyroyaw = mavlink_msg_attitude_get_gyroyaw(msg);
#else
    uint8_t len = msg->len < MAVLINK_MSG_ID_attitude_LEN? msg->len : MAVLINK_MSG_ID_attitude_LEN;
    memset(attitude, 0, MAVLINK_MSG_ID_attitude_LEN);
    memcpy(attitude, _MAV_PAYLOAD(msg), len);
#endif
}
