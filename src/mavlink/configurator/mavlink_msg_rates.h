#pragma once
// MESSAGE rates PACKING

#define MAVLINK_MSG_ID_rates 8

MAVPACKED(
typedef struct __mavlink_rates_t {
 float roll_rate; /*<  roll_rate*/
 float roll_rc_rate; /*<  roll_rc_rate*/
 float roll_rc_curve; /*<  roll_rc_curve*/
 float pitch_rate; /*<  pitch_rate*/
 float pitch_rc_rate; /*<  pitch_rc_rate*/
 float pitch_rc_curve; /*<  pitch_rc_curve*/
 float yaw_rate; /*<  yaw_rate*/
 float yaw_rc_rate; /*<  yaw_rc_rate*/
 float yaw_rc_curve; /*<  yaw_rc_curve*/
}) mavlink_rates_t;

#define MAVLINK_MSG_ID_rates_LEN 36
#define MAVLINK_MSG_ID_rates_MIN_LEN 36
#define MAVLINK_MSG_ID_8_LEN 36
#define MAVLINK_MSG_ID_8_MIN_LEN 36

#define MAVLINK_MSG_ID_rates_CRC 17
#define MAVLINK_MSG_ID_8_CRC 17



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_rates { \
    8, \
    "rates", \
    9, \
    {  { "roll_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_rates_t, roll_rate) }, \
         { "roll_rc_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_rates_t, roll_rc_rate) }, \
         { "roll_rc_curve", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_rates_t, roll_rc_curve) }, \
         { "pitch_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_rates_t, pitch_rate) }, \
         { "pitch_rc_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_rates_t, pitch_rc_rate) }, \
         { "pitch_rc_curve", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_rates_t, pitch_rc_curve) }, \
         { "yaw_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_rates_t, yaw_rate) }, \
         { "yaw_rc_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_rates_t, yaw_rc_rate) }, \
         { "yaw_rc_curve", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_rates_t, yaw_rc_curve) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_rates { \
    "rates", \
    9, \
    {  { "roll_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_rates_t, roll_rate) }, \
         { "roll_rc_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_rates_t, roll_rc_rate) }, \
         { "roll_rc_curve", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_rates_t, roll_rc_curve) }, \
         { "pitch_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_rates_t, pitch_rate) }, \
         { "pitch_rc_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_rates_t, pitch_rc_rate) }, \
         { "pitch_rc_curve", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_rates_t, pitch_rc_curve) }, \
         { "yaw_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_rates_t, yaw_rate) }, \
         { "yaw_rc_rate", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_rates_t, yaw_rc_rate) }, \
         { "yaw_rc_curve", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_rates_t, yaw_rc_curve) }, \
         } \
}
#endif

/**
 * @brief Pack a rates message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param roll_rate  roll_rate
 * @param roll_rc_rate  roll_rc_rate
 * @param roll_rc_curve  roll_rc_curve
 * @param pitch_rate  pitch_rate
 * @param pitch_rc_rate  pitch_rc_rate
 * @param pitch_rc_curve  pitch_rc_curve
 * @param yaw_rate  yaw_rate
 * @param yaw_rc_rate  yaw_rc_rate
 * @param yaw_rc_curve  yaw_rc_curve
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rates_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float roll_rate, float roll_rc_rate, float roll_rc_curve, float pitch_rate, float pitch_rc_rate, float pitch_rc_curve, float yaw_rate, float yaw_rc_rate, float yaw_rc_curve)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_rates_LEN];
    _mav_put_float(buf, 0, roll_rate);
    _mav_put_float(buf, 4, roll_rc_rate);
    _mav_put_float(buf, 8, roll_rc_curve);
    _mav_put_float(buf, 12, pitch_rate);
    _mav_put_float(buf, 16, pitch_rc_rate);
    _mav_put_float(buf, 20, pitch_rc_curve);
    _mav_put_float(buf, 24, yaw_rate);
    _mav_put_float(buf, 28, yaw_rc_rate);
    _mav_put_float(buf, 32, yaw_rc_curve);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_rates_LEN);
#else
    mavlink_rates_t packet;
    packet.roll_rate = roll_rate;
    packet.roll_rc_rate = roll_rc_rate;
    packet.roll_rc_curve = roll_rc_curve;
    packet.pitch_rate = pitch_rate;
    packet.pitch_rc_rate = pitch_rc_rate;
    packet.pitch_rc_curve = pitch_rc_curve;
    packet.yaw_rate = yaw_rate;
    packet.yaw_rc_rate = yaw_rc_rate;
    packet.yaw_rc_curve = yaw_rc_curve;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_rates_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_rates;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_rates_MIN_LEN, MAVLINK_MSG_ID_rates_LEN, MAVLINK_MSG_ID_rates_CRC);
}

/**
 * @brief Pack a rates message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param roll_rate  roll_rate
 * @param roll_rc_rate  roll_rc_rate
 * @param roll_rc_curve  roll_rc_curve
 * @param pitch_rate  pitch_rate
 * @param pitch_rc_rate  pitch_rc_rate
 * @param pitch_rc_curve  pitch_rc_curve
 * @param yaw_rate  yaw_rate
 * @param yaw_rc_rate  yaw_rc_rate
 * @param yaw_rc_curve  yaw_rc_curve
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rates_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float roll_rate,float roll_rc_rate,float roll_rc_curve,float pitch_rate,float pitch_rc_rate,float pitch_rc_curve,float yaw_rate,float yaw_rc_rate,float yaw_rc_curve)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_rates_LEN];
    _mav_put_float(buf, 0, roll_rate);
    _mav_put_float(buf, 4, roll_rc_rate);
    _mav_put_float(buf, 8, roll_rc_curve);
    _mav_put_float(buf, 12, pitch_rate);
    _mav_put_float(buf, 16, pitch_rc_rate);
    _mav_put_float(buf, 20, pitch_rc_curve);
    _mav_put_float(buf, 24, yaw_rate);
    _mav_put_float(buf, 28, yaw_rc_rate);
    _mav_put_float(buf, 32, yaw_rc_curve);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_rates_LEN);
#else
    mavlink_rates_t packet;
    packet.roll_rate = roll_rate;
    packet.roll_rc_rate = roll_rc_rate;
    packet.roll_rc_curve = roll_rc_curve;
    packet.pitch_rate = pitch_rate;
    packet.pitch_rc_rate = pitch_rc_rate;
    packet.pitch_rc_curve = pitch_rc_curve;
    packet.yaw_rate = yaw_rate;
    packet.yaw_rc_rate = yaw_rc_rate;
    packet.yaw_rc_curve = yaw_rc_curve;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_rates_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_rates;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_rates_MIN_LEN, MAVLINK_MSG_ID_rates_LEN, MAVLINK_MSG_ID_rates_CRC);
}

/**
 * @brief Encode a rates struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param rates C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rates_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_rates_t* rates)
{
    return mavlink_msg_rates_pack(system_id, component_id, msg, rates->roll_rate, rates->roll_rc_rate, rates->roll_rc_curve, rates->pitch_rate, rates->pitch_rc_rate, rates->pitch_rc_curve, rates->yaw_rate, rates->yaw_rc_rate, rates->yaw_rc_curve);
}

/**
 * @brief Encode a rates struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param rates C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rates_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_rates_t* rates)
{
    return mavlink_msg_rates_pack_chan(system_id, component_id, chan, msg, rates->roll_rate, rates->roll_rc_rate, rates->roll_rc_curve, rates->pitch_rate, rates->pitch_rc_rate, rates->pitch_rc_curve, rates->yaw_rate, rates->yaw_rc_rate, rates->yaw_rc_curve);
}

/**
 * @brief Send a rates message
 * @param chan MAVLink channel to send the message
 *
 * @param roll_rate  roll_rate
 * @param roll_rc_rate  roll_rc_rate
 * @param roll_rc_curve  roll_rc_curve
 * @param pitch_rate  pitch_rate
 * @param pitch_rc_rate  pitch_rc_rate
 * @param pitch_rc_curve  pitch_rc_curve
 * @param yaw_rate  yaw_rate
 * @param yaw_rc_rate  yaw_rc_rate
 * @param yaw_rc_curve  yaw_rc_curve
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_rates_send(mavlink_channel_t chan, float roll_rate, float roll_rc_rate, float roll_rc_curve, float pitch_rate, float pitch_rc_rate, float pitch_rc_curve, float yaw_rate, float yaw_rc_rate, float yaw_rc_curve)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_rates_LEN];
    _mav_put_float(buf, 0, roll_rate);
    _mav_put_float(buf, 4, roll_rc_rate);
    _mav_put_float(buf, 8, roll_rc_curve);
    _mav_put_float(buf, 12, pitch_rate);
    _mav_put_float(buf, 16, pitch_rc_rate);
    _mav_put_float(buf, 20, pitch_rc_curve);
    _mav_put_float(buf, 24, yaw_rate);
    _mav_put_float(buf, 28, yaw_rc_rate);
    _mav_put_float(buf, 32, yaw_rc_curve);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_rates, buf, MAVLINK_MSG_ID_rates_MIN_LEN, MAVLINK_MSG_ID_rates_LEN, MAVLINK_MSG_ID_rates_CRC);
#else
    mavlink_rates_t packet;
    packet.roll_rate = roll_rate;
    packet.roll_rc_rate = roll_rc_rate;
    packet.roll_rc_curve = roll_rc_curve;
    packet.pitch_rate = pitch_rate;
    packet.pitch_rc_rate = pitch_rc_rate;
    packet.pitch_rc_curve = pitch_rc_curve;
    packet.yaw_rate = yaw_rate;
    packet.yaw_rc_rate = yaw_rc_rate;
    packet.yaw_rc_curve = yaw_rc_curve;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_rates, (const char *)&packet, MAVLINK_MSG_ID_rates_MIN_LEN, MAVLINK_MSG_ID_rates_LEN, MAVLINK_MSG_ID_rates_CRC);
#endif
}

/**
 * @brief Send a rates message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_rates_send_struct(mavlink_channel_t chan, const mavlink_rates_t* rates)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_rates_send(chan, rates->roll_rate, rates->roll_rc_rate, rates->roll_rc_curve, rates->pitch_rate, rates->pitch_rc_rate, rates->pitch_rc_curve, rates->yaw_rate, rates->yaw_rc_rate, rates->yaw_rc_curve);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_rates, (const char *)rates, MAVLINK_MSG_ID_rates_MIN_LEN, MAVLINK_MSG_ID_rates_LEN, MAVLINK_MSG_ID_rates_CRC);
#endif
}

#if MAVLINK_MSG_ID_rates_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_rates_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float roll_rate, float roll_rc_rate, float roll_rc_curve, float pitch_rate, float pitch_rc_rate, float pitch_rc_curve, float yaw_rate, float yaw_rc_rate, float yaw_rc_curve)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, roll_rate);
    _mav_put_float(buf, 4, roll_rc_rate);
    _mav_put_float(buf, 8, roll_rc_curve);
    _mav_put_float(buf, 12, pitch_rate);
    _mav_put_float(buf, 16, pitch_rc_rate);
    _mav_put_float(buf, 20, pitch_rc_curve);
    _mav_put_float(buf, 24, yaw_rate);
    _mav_put_float(buf, 28, yaw_rc_rate);
    _mav_put_float(buf, 32, yaw_rc_curve);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_rates, buf, MAVLINK_MSG_ID_rates_MIN_LEN, MAVLINK_MSG_ID_rates_LEN, MAVLINK_MSG_ID_rates_CRC);
#else
    mavlink_rates_t *packet = (mavlink_rates_t *)msgbuf;
    packet->roll_rate = roll_rate;
    packet->roll_rc_rate = roll_rc_rate;
    packet->roll_rc_curve = roll_rc_curve;
    packet->pitch_rate = pitch_rate;
    packet->pitch_rc_rate = pitch_rc_rate;
    packet->pitch_rc_curve = pitch_rc_curve;
    packet->yaw_rate = yaw_rate;
    packet->yaw_rc_rate = yaw_rc_rate;
    packet->yaw_rc_curve = yaw_rc_curve;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_rates, (const char *)packet, MAVLINK_MSG_ID_rates_MIN_LEN, MAVLINK_MSG_ID_rates_LEN, MAVLINK_MSG_ID_rates_CRC);
#endif
}
#endif

#endif

// MESSAGE rates UNPACKING


/**
 * @brief Get field roll_rate from rates message
 *
 * @return  roll_rate
 */
static inline float mavlink_msg_rates_get_roll_rate(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field roll_rc_rate from rates message
 *
 * @return  roll_rc_rate
 */
static inline float mavlink_msg_rates_get_roll_rc_rate(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field roll_rc_curve from rates message
 *
 * @return  roll_rc_curve
 */
static inline float mavlink_msg_rates_get_roll_rc_curve(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field pitch_rate from rates message
 *
 * @return  pitch_rate
 */
static inline float mavlink_msg_rates_get_pitch_rate(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field pitch_rc_rate from rates message
 *
 * @return  pitch_rc_rate
 */
static inline float mavlink_msg_rates_get_pitch_rc_rate(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field pitch_rc_curve from rates message
 *
 * @return  pitch_rc_curve
 */
static inline float mavlink_msg_rates_get_pitch_rc_curve(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field yaw_rate from rates message
 *
 * @return  yaw_rate
 */
static inline float mavlink_msg_rates_get_yaw_rate(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field yaw_rc_rate from rates message
 *
 * @return  yaw_rc_rate
 */
static inline float mavlink_msg_rates_get_yaw_rc_rate(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field yaw_rc_curve from rates message
 *
 * @return  yaw_rc_curve
 */
static inline float mavlink_msg_rates_get_yaw_rc_curve(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  32);
}

/**
 * @brief Decode a rates message into a struct
 *
 * @param msg The message to decode
 * @param rates C-struct to decode the message contents into
 */
static inline void mavlink_msg_rates_decode(const mavlink_message_t* msg, mavlink_rates_t* rates)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    rates->roll_rate = mavlink_msg_rates_get_roll_rate(msg);
    rates->roll_rc_rate = mavlink_msg_rates_get_roll_rc_rate(msg);
    rates->roll_rc_curve = mavlink_msg_rates_get_roll_rc_curve(msg);
    rates->pitch_rate = mavlink_msg_rates_get_pitch_rate(msg);
    rates->pitch_rc_rate = mavlink_msg_rates_get_pitch_rc_rate(msg);
    rates->pitch_rc_curve = mavlink_msg_rates_get_pitch_rc_curve(msg);
    rates->yaw_rate = mavlink_msg_rates_get_yaw_rate(msg);
    rates->yaw_rc_rate = mavlink_msg_rates_get_yaw_rc_rate(msg);
    rates->yaw_rc_curve = mavlink_msg_rates_get_yaw_rc_curve(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_rates_LEN? msg->len : MAVLINK_MSG_ID_rates_LEN;
        memset(rates, 0, MAVLINK_MSG_ID_rates_LEN);
    memcpy(rates, _MAV_PAYLOAD(msg), len);
#endif
}
