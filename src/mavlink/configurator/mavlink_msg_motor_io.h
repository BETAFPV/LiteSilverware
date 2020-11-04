#pragma once
// MESSAGE motor_io PACKING

#define MAVLINK_MSG_ID_motor_io 4

MAVPACKED(
typedef struct __mavlink_motor_io_t {
 uint8_t motor_dir; /*<  motor_dir*/
 uint8_t motor_min; /*<  motor_min*/
 uint8_t motor_dshot; /*<  motor_dshot*/
 uint8_t motor3; /*<  motor3*/
 uint8_t motor4; /*<  motor4*/
}) mavlink_motor_io_t;

#define MAVLINK_MSG_ID_motor_io_LEN 5
#define MAVLINK_MSG_ID_motor_io_MIN_LEN 5
#define MAVLINK_MSG_ID_4_LEN 5
#define MAVLINK_MSG_ID_4_MIN_LEN 5

#define MAVLINK_MSG_ID_motor_io_CRC 164
#define MAVLINK_MSG_ID_4_CRC 164



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_motor_io { \
    4, \
    "motor_io", \
    5, \
    {  { "motor_dir", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_motor_io_t, motor_dir) }, \
         { "motor_min", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_motor_io_t, motor_min) }, \
         { "motor_dshot", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_motor_io_t, motor_dshot) }, \
         { "motor3", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_motor_io_t, motor3) }, \
         { "motor4", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_motor_io_t, motor4) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_motor_io { \
    "motor_io", \
    5, \
    {  { "motor_dir", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_motor_io_t, motor_dir) }, \
         { "motor_min", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_motor_io_t, motor_min) }, \
         { "motor_dshot", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_motor_io_t, motor_dshot) }, \
         { "motor3", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_motor_io_t, motor3) }, \
         { "motor4", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_motor_io_t, motor4) }, \
         } \
}
#endif

/**
 * @brief Pack a motor_io message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param motor_dir  motor_dir
 * @param motor_min  motor_min
 * @param motor_dshot  motor_dshot
 * @param motor3  motor3
 * @param motor4  motor4
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_motor_io_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t motor_dir, uint8_t motor_min, uint8_t motor_dshot, uint8_t motor3, uint8_t motor4)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_motor_io_LEN];
    _mav_put_uint8_t(buf, 0, motor_dir);
    _mav_put_uint8_t(buf, 1, motor_min);
    _mav_put_uint8_t(buf, 2, motor_dshot);
    _mav_put_uint8_t(buf, 3, motor3);
    _mav_put_uint8_t(buf, 4, motor4);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_motor_io_LEN);
#else
    mavlink_motor_io_t packet;
    packet.motor_dir = motor_dir;
    packet.motor_min = motor_min;
    packet.motor_dshot = motor_dshot;
    packet.motor3 = motor3;
    packet.motor4 = motor4;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_motor_io_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_motor_io;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_motor_io_MIN_LEN, MAVLINK_MSG_ID_motor_io_LEN, MAVLINK_MSG_ID_motor_io_CRC);
}

/**
 * @brief Pack a motor_io message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param motor_dir  motor_dir
 * @param motor_min  motor_min
 * @param motor_dshot  motor_dshot
 * @param motor3  motor3
 * @param motor4  motor4
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_motor_io_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t motor_dir,uint8_t motor_min,uint8_t motor_dshot,uint8_t motor3,uint8_t motor4)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_motor_io_LEN];
    _mav_put_uint8_t(buf, 0, motor_dir);
    _mav_put_uint8_t(buf, 1, motor_min);
    _mav_put_uint8_t(buf, 2, motor_dshot);
    _mav_put_uint8_t(buf, 3, motor3);
    _mav_put_uint8_t(buf, 4, motor4);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_motor_io_LEN);
#else
    mavlink_motor_io_t packet;
    packet.motor_dir = motor_dir;
    packet.motor_min = motor_min;
    packet.motor_dshot = motor_dshot;
    packet.motor3 = motor3;
    packet.motor4 = motor4;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_motor_io_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_motor_io;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_motor_io_MIN_LEN, MAVLINK_MSG_ID_motor_io_LEN, MAVLINK_MSG_ID_motor_io_CRC);
}

/**
 * @brief Encode a motor_io struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param motor_io C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_motor_io_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_motor_io_t* motor_io)
{
    return mavlink_msg_motor_io_pack(system_id, component_id, msg, motor_io->motor_dir, motor_io->motor_min, motor_io->motor_dshot, motor_io->motor3, motor_io->motor4);
}

/**
 * @brief Encode a motor_io struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param motor_io C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_motor_io_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_motor_io_t* motor_io)
{
    return mavlink_msg_motor_io_pack_chan(system_id, component_id, chan, msg, motor_io->motor_dir, motor_io->motor_min, motor_io->motor_dshot, motor_io->motor3, motor_io->motor4);
}

/**
 * @brief Send a motor_io message
 * @param chan MAVLink channel to send the message
 *
 * @param motor_dir  motor_dir
 * @param motor_min  motor_min
 * @param motor_dshot  motor_dshot
 * @param motor3  motor3
 * @param motor4  motor4
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_motor_io_send(mavlink_channel_t chan, uint8_t motor_dir, uint8_t motor_min, uint8_t motor_dshot, uint8_t motor3, uint8_t motor4)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_motor_io_LEN];
    _mav_put_uint8_t(buf, 0, motor_dir);
    _mav_put_uint8_t(buf, 1, motor_min);
    _mav_put_uint8_t(buf, 2, motor_dshot);
    _mav_put_uint8_t(buf, 3, motor3);
    _mav_put_uint8_t(buf, 4, motor4);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_motor_io, buf, MAVLINK_MSG_ID_motor_io_MIN_LEN, MAVLINK_MSG_ID_motor_io_LEN, MAVLINK_MSG_ID_motor_io_CRC);
#else
    mavlink_motor_io_t packet;
    packet.motor_dir = motor_dir;
    packet.motor_min = motor_min;
    packet.motor_dshot = motor_dshot;
    packet.motor3 = motor3;
    packet.motor4 = motor4;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_motor_io, (const char *)&packet, MAVLINK_MSG_ID_motor_io_MIN_LEN, MAVLINK_MSG_ID_motor_io_LEN, MAVLINK_MSG_ID_motor_io_CRC);
#endif
}

/**
 * @brief Send a motor_io message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_motor_io_send_struct(mavlink_channel_t chan, const mavlink_motor_io_t* motor_io)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_motor_io_send(chan, motor_io->motor_dir, motor_io->motor_min, motor_io->motor_dshot, motor_io->motor3, motor_io->motor4);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_motor_io, (const char *)motor_io, MAVLINK_MSG_ID_motor_io_MIN_LEN, MAVLINK_MSG_ID_motor_io_LEN, MAVLINK_MSG_ID_motor_io_CRC);
#endif
}

#if MAVLINK_MSG_ID_motor_io_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_motor_io_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t motor_dir, uint8_t motor_min, uint8_t motor_dshot, uint8_t motor3, uint8_t motor4)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, motor_dir);
    _mav_put_uint8_t(buf, 1, motor_min);
    _mav_put_uint8_t(buf, 2, motor_dshot);
    _mav_put_uint8_t(buf, 3, motor3);
    _mav_put_uint8_t(buf, 4, motor4);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_motor_io, buf, MAVLINK_MSG_ID_motor_io_MIN_LEN, MAVLINK_MSG_ID_motor_io_LEN, MAVLINK_MSG_ID_motor_io_CRC);
#else
    mavlink_motor_io_t *packet = (mavlink_motor_io_t *)msgbuf;
    packet->motor_dir = motor_dir;
    packet->motor_min = motor_min;
    packet->motor_dshot = motor_dshot;
    packet->motor3 = motor3;
    packet->motor4 = motor4;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_motor_io, (const char *)packet, MAVLINK_MSG_ID_motor_io_MIN_LEN, MAVLINK_MSG_ID_motor_io_LEN, MAVLINK_MSG_ID_motor_io_CRC);
#endif
}
#endif

#endif

// MESSAGE motor_io UNPACKING


/**
 * @brief Get field motor_dir from motor_io message
 *
 * @return  motor_dir
 */
static inline uint8_t mavlink_msg_motor_io_get_motor_dir(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field motor_min from motor_io message
 *
 * @return  motor_min
 */
static inline uint8_t mavlink_msg_motor_io_get_motor_min(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Get field motor_dshot from motor_io message
 *
 * @return  motor_dshot
 */
static inline uint8_t mavlink_msg_motor_io_get_motor_dshot(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Get field motor3 from motor_io message
 *
 * @return  motor3
 */
static inline uint8_t mavlink_msg_motor_io_get_motor3(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  3);
}

/**
 * @brief Get field motor4 from motor_io message
 *
 * @return  motor4
 */
static inline uint8_t mavlink_msg_motor_io_get_motor4(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Decode a motor_io message into a struct
 *
 * @param msg The message to decode
 * @param motor_io C-struct to decode the message contents into
 */
static inline void mavlink_msg_motor_io_decode(const mavlink_message_t* msg, mavlink_motor_io_t* motor_io)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    motor_io->motor_dir = mavlink_msg_motor_io_get_motor_dir(msg);
    motor_io->motor_min = mavlink_msg_motor_io_get_motor_min(msg);
    motor_io->motor_dshot = mavlink_msg_motor_io_get_motor_dshot(msg);
    motor_io->motor3 = mavlink_msg_motor_io_get_motor3(msg);
    motor_io->motor4 = mavlink_msg_motor_io_get_motor4(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_motor_io_LEN? msg->len : MAVLINK_MSG_ID_motor_io_LEN;
        memset(motor_io, 0, MAVLINK_MSG_ID_motor_io_LEN);
    memcpy(motor_io, _MAV_PAYLOAD(msg), len);
#endif
}
