#pragma once
// MESSAGE pid PACKING

#define MAVLINK_MSG_ID_pid 6

MAVPACKED(
    typedef struct __mavlink_pid_t
{
    float roll_kp; /*<  roll_kp*/
    float roll_ki; /*<  roll_ki*/
    float roll_kd; /*<  roll_kd*/
    float pitch_kp; /*<  pitch_kp*/
    float pitch_ki; /*<  pitch_ki*/
    float pitch_kd; /*<  pitch_kd*/
    float yaw_kp; /*<  yaw_kp*/
    float yaw_ki; /*<  yaw_ki*/
    float yaw_kd; /*<  yaw_kd*/
}) mavlink_pid_t;

#define MAVLINK_MSG_ID_pid_LEN 36
#define MAVLINK_MSG_ID_pid_MIN_LEN 36
#define MAVLINK_MSG_ID_6_LEN 36
#define MAVLINK_MSG_ID_6_MIN_LEN 36

#define MAVLINK_MSG_ID_pid_CRC 2
#define MAVLINK_MSG_ID_6_CRC 2



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_pid { \
    6, \
    "pid", \
    9, \
    {  { "roll_kp", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_pid_t, roll_kp) }, \
         { "roll_ki", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_pid_t, roll_ki) }, \
         { "roll_kd", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_pid_t, roll_kd) }, \
         { "pitch_kp", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_pid_t, pitch_kp) }, \
         { "pitch_ki", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_pid_t, pitch_ki) }, \
         { "pitch_kd", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_pid_t, pitch_kd) }, \
         { "yaw_kp", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_pid_t, yaw_kp) }, \
         { "yaw_ki", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_pid_t, yaw_ki) }, \
         { "yaw_kd", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_pid_t, yaw_kd) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_pid { \
    "pid", \
    9, \
    {  { "roll_kp", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_pid_t, roll_kp) }, \
         { "roll_ki", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_pid_t, roll_ki) }, \
         { "roll_kd", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_pid_t, roll_kd) }, \
         { "pitch_kp", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_pid_t, pitch_kp) }, \
         { "pitch_ki", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_pid_t, pitch_ki) }, \
         { "pitch_kd", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_pid_t, pitch_kd) }, \
         { "yaw_kp", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_pid_t, yaw_kp) }, \
         { "yaw_ki", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_pid_t, yaw_ki) }, \
         { "yaw_kd", NULL, MAVLINK_TYPE_FLOAT, 0, 32, offsetof(mavlink_pid_t, yaw_kd) }, \
         } \
}
#endif

/**
 * @brief Pack a pid message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param roll_kp  roll_kp
 * @param roll_ki  roll_ki
 * @param roll_kd  roll_kd
 * @param pitch_kp  pitch_kp
 * @param pitch_ki  pitch_ki
 * @param pitch_kd  pitch_kd
 * @param yaw_kp  yaw_kp
 * @param yaw_ki  yaw_ki
 * @param yaw_kd  yaw_kd
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_pid_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
        float roll_kp, float roll_ki, float roll_kd, float pitch_kp, float pitch_ki, float pitch_kd, float yaw_kp, float yaw_ki, float yaw_kd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_pid_LEN];
    _mav_put_float(buf, 0, roll_kp);
    _mav_put_float(buf, 4, roll_ki);
    _mav_put_float(buf, 8, roll_kd);
    _mav_put_float(buf, 12, pitch_kp);
    _mav_put_float(buf, 16, pitch_ki);
    _mav_put_float(buf, 20, pitch_kd);
    _mav_put_float(buf, 24, yaw_kp);
    _mav_put_float(buf, 28, yaw_ki);
    _mav_put_float(buf, 32, yaw_kd);

    memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_pid_LEN);
#else
    mavlink_pid_t packet;
    packet.roll_kp = roll_kp;
    packet.roll_ki = roll_ki;
    packet.roll_kd = roll_kd;
    packet.pitch_kp = pitch_kp;
    packet.pitch_ki = pitch_ki;
    packet.pitch_kd = pitch_kd;
    packet.yaw_kp = yaw_kp;
    packet.yaw_ki = yaw_ki;
    packet.yaw_kd = yaw_kd;

    memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_pid_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_pid;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_pid_MIN_LEN, MAVLINK_MSG_ID_pid_LEN, MAVLINK_MSG_ID_pid_CRC);
}

/**
 * @brief Pack a pid message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param roll_kp  roll_kp
 * @param roll_ki  roll_ki
 * @param roll_kd  roll_kd
 * @param pitch_kp  pitch_kp
 * @param pitch_ki  pitch_ki
 * @param pitch_kd  pitch_kd
 * @param yaw_kp  yaw_kp
 * @param yaw_ki  yaw_ki
 * @param yaw_kd  yaw_kd
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_pid_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
        mavlink_message_t* msg,
        float roll_kp,float roll_ki,float roll_kd,float pitch_kp,float pitch_ki,float pitch_kd,float yaw_kp,float yaw_ki,float yaw_kd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_pid_LEN];
    _mav_put_float(buf, 0, roll_kp);
    _mav_put_float(buf, 4, roll_ki);
    _mav_put_float(buf, 8, roll_kd);
    _mav_put_float(buf, 12, pitch_kp);
    _mav_put_float(buf, 16, pitch_ki);
    _mav_put_float(buf, 20, pitch_kd);
    _mav_put_float(buf, 24, yaw_kp);
    _mav_put_float(buf, 28, yaw_ki);
    _mav_put_float(buf, 32, yaw_kd);

    memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_pid_LEN);
#else
    mavlink_pid_t packet;
    packet.roll_kp = roll_kp;
    packet.roll_ki = roll_ki;
    packet.roll_kd = roll_kd;
    packet.pitch_kp = pitch_kp;
    packet.pitch_ki = pitch_ki;
    packet.pitch_kd = pitch_kd;
    packet.yaw_kp = yaw_kp;
    packet.yaw_ki = yaw_ki;
    packet.yaw_kd = yaw_kd;

    memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_pid_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_pid;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_pid_MIN_LEN, MAVLINK_MSG_ID_pid_LEN, MAVLINK_MSG_ID_pid_CRC);
}

/**
 * @brief Encode a pid struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param pid C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_pid_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_pid_t* pid)
{
    return mavlink_msg_pid_pack(system_id, component_id, msg, pid->roll_kp, pid->roll_ki, pid->roll_kd, pid->pitch_kp, pid->pitch_ki, pid->pitch_kd, pid->yaw_kp, pid->yaw_ki, pid->yaw_kd);
}

/**
 * @brief Encode a pid struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param pid C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_pid_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_pid_t* pid)
{
    return mavlink_msg_pid_pack_chan(system_id, component_id, chan, msg, pid->roll_kp, pid->roll_ki, pid->roll_kd, pid->pitch_kp, pid->pitch_ki, pid->pitch_kd, pid->yaw_kp, pid->yaw_ki, pid->yaw_kd);
}

/**
 * @brief Send a pid message
 * @param chan MAVLink channel to send the message
 *
 * @param roll_kp  roll_kp
 * @param roll_ki  roll_ki
 * @param roll_kd  roll_kd
 * @param pitch_kp  pitch_kp
 * @param pitch_ki  pitch_ki
 * @param pitch_kd  pitch_kd
 * @param yaw_kp  yaw_kp
 * @param yaw_ki  yaw_ki
 * @param yaw_kd  yaw_kd
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_pid_send(mavlink_channel_t chan, float roll_kp, float roll_ki, float roll_kd, float pitch_kp, float pitch_ki, float pitch_kd, float yaw_kp, float yaw_ki, float yaw_kd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_pid_LEN];
    _mav_put_float(buf, 0, roll_kp);
    _mav_put_float(buf, 4, roll_ki);
    _mav_put_float(buf, 8, roll_kd);
    _mav_put_float(buf, 12, pitch_kp);
    _mav_put_float(buf, 16, pitch_ki);
    _mav_put_float(buf, 20, pitch_kd);
    _mav_put_float(buf, 24, yaw_kp);
    _mav_put_float(buf, 28, yaw_ki);
    _mav_put_float(buf, 32, yaw_kd);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_pid, buf, MAVLINK_MSG_ID_pid_MIN_LEN, MAVLINK_MSG_ID_pid_LEN, MAVLINK_MSG_ID_pid_CRC);
#else
    mavlink_pid_t packet;
    packet.roll_kp = roll_kp;
    packet.roll_ki = roll_ki;
    packet.roll_kd = roll_kd;
    packet.pitch_kp = pitch_kp;
    packet.pitch_ki = pitch_ki;
    packet.pitch_kd = pitch_kd;
    packet.yaw_kp = yaw_kp;
    packet.yaw_ki = yaw_ki;
    packet.yaw_kd = yaw_kd;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_pid, (const char *)&packet, MAVLINK_MSG_ID_pid_MIN_LEN, MAVLINK_MSG_ID_pid_LEN, MAVLINK_MSG_ID_pid_CRC);
#endif
}

/**
 * @brief Send a pid message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_pid_send_struct(mavlink_channel_t chan, const mavlink_pid_t* pid)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_pid_send(chan, pid->roll_kp, pid->roll_ki, pid->roll_kd, pid->pitch_kp, pid->pitch_ki, pid->pitch_kd, pid->yaw_kp, pid->yaw_ki, pid->yaw_kd);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_pid, (const char *)pid, MAVLINK_MSG_ID_pid_MIN_LEN, MAVLINK_MSG_ID_pid_LEN, MAVLINK_MSG_ID_pid_CRC);
#endif
}

#if MAVLINK_MSG_ID_pid_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_pid_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float roll_kp, float roll_ki, float roll_kd, float pitch_kp, float pitch_ki, float pitch_kd, float yaw_kp, float yaw_ki, float yaw_kd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, roll_kp);
    _mav_put_float(buf, 4, roll_ki);
    _mav_put_float(buf, 8, roll_kd);
    _mav_put_float(buf, 12, pitch_kp);
    _mav_put_float(buf, 16, pitch_ki);
    _mav_put_float(buf, 20, pitch_kd);
    _mav_put_float(buf, 24, yaw_kp);
    _mav_put_float(buf, 28, yaw_ki);
    _mav_put_float(buf, 32, yaw_kd);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_pid, buf, MAVLINK_MSG_ID_pid_MIN_LEN, MAVLINK_MSG_ID_pid_LEN, MAVLINK_MSG_ID_pid_CRC);
#else
    mavlink_pid_t *packet = (mavlink_pid_t *)msgbuf;
    packet->roll_kp = roll_kp;
    packet->roll_ki = roll_ki;
    packet->roll_kd = roll_kd;
    packet->pitch_kp = pitch_kp;
    packet->pitch_ki = pitch_ki;
    packet->pitch_kd = pitch_kd;
    packet->yaw_kp = yaw_kp;
    packet->yaw_ki = yaw_ki;
    packet->yaw_kd = yaw_kd;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_pid, (const char *)packet, MAVLINK_MSG_ID_pid_MIN_LEN, MAVLINK_MSG_ID_pid_LEN, MAVLINK_MSG_ID_pid_CRC);
#endif
}
#endif

#endif

// MESSAGE pid UNPACKING


/**
 * @brief Get field roll_kp from pid message
 *
 * @return  roll_kp
 */
static inline float mavlink_msg_pid_get_roll_kp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field roll_ki from pid message
 *
 * @return  roll_ki
 */
static inline float mavlink_msg_pid_get_roll_ki(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field roll_kd from pid message
 *
 * @return  roll_kd
 */
static inline float mavlink_msg_pid_get_roll_kd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field pitch_kp from pid message
 *
 * @return  pitch_kp
 */
static inline float mavlink_msg_pid_get_pitch_kp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field pitch_ki from pid message
 *
 * @return  pitch_ki
 */
static inline float mavlink_msg_pid_get_pitch_ki(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field pitch_kd from pid message
 *
 * @return  pitch_kd
 */
static inline float mavlink_msg_pid_get_pitch_kd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field yaw_kp from pid message
 *
 * @return  yaw_kp
 */
static inline float mavlink_msg_pid_get_yaw_kp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field yaw_ki from pid message
 *
 * @return  yaw_ki
 */
static inline float mavlink_msg_pid_get_yaw_ki(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field yaw_kd from pid message
 *
 * @return  yaw_kd
 */
static inline float mavlink_msg_pid_get_yaw_kd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  32);
}

/**
 * @brief Decode a pid message into a struct
 *
 * @param msg The message to decode
 * @param pid C-struct to decode the message contents into
 */
static inline void mavlink_msg_pid_decode(const mavlink_message_t* msg, mavlink_pid_t* pid)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    pid->roll_kp = mavlink_msg_pid_get_roll_kp(msg);
    pid->roll_ki = mavlink_msg_pid_get_roll_ki(msg);
    pid->roll_kd = mavlink_msg_pid_get_roll_kd(msg);
    pid->pitch_kp = mavlink_msg_pid_get_pitch_kp(msg);
    pid->pitch_ki = mavlink_msg_pid_get_pitch_ki(msg);
    pid->pitch_kd = mavlink_msg_pid_get_pitch_kd(msg);
    pid->yaw_kp = mavlink_msg_pid_get_yaw_kp(msg);
    pid->yaw_ki = mavlink_msg_pid_get_yaw_ki(msg);
    pid->yaw_kd = mavlink_msg_pid_get_yaw_kd(msg);
#else
    uint8_t len = msg->len < MAVLINK_MSG_ID_pid_LEN? msg->len : MAVLINK_MSG_ID_pid_LEN;
    memset(pid, 0, MAVLINK_MSG_ID_pid_LEN);
    memcpy(pid, _MAV_PAYLOAD(msg), len);
#endif
}
