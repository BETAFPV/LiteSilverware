#pragma once
// MESSAGE motor_value PACKING

#define MAVLINK_MSG_ID_motor_value 3

MAVPACKED(
typedef struct __mavlink_motor_value_t {
 uint16_t motor1; /*<  motor1*/
 uint16_t motor2; /*<  motor2*/
 uint16_t motor3; /*<  motor3*/
 uint16_t motor4; /*<  motor4*/
}) mavlink_motor_value_t;

#define MAVLINK_MSG_ID_motor_value_LEN 8
#define MAVLINK_MSG_ID_motor_value_MIN_LEN 8
#define MAVLINK_MSG_ID_3_LEN 8
#define MAVLINK_MSG_ID_3_MIN_LEN 8

#define MAVLINK_MSG_ID_motor_value_CRC 92
#define MAVLINK_MSG_ID_3_CRC 92



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_motor_value { \
    3, \
    "motor_value", \
    4, \
    {  { "motor1", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_motor_value_t, motor1) }, \
         { "motor2", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_motor_value_t, motor2) }, \
         { "motor3", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_motor_value_t, motor3) }, \
         { "motor4", NULL, MAVLINK_TYPE_UINT16_T, 0, 6, offsetof(mavlink_motor_value_t, motor4) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_motor_value { \
    "motor_value", \
    4, \
    {  { "motor1", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_motor_value_t, motor1) }, \
         { "motor2", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_motor_value_t, motor2) }, \
         { "motor3", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_motor_value_t, motor3) }, \
         { "motor4", NULL, MAVLINK_TYPE_UINT16_T, 0, 6, offsetof(mavlink_motor_value_t, motor4) }, \
         } \
}
#endif

/**
 * @brief Pack a motor_value message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param motor1  motor1
 * @param motor2  motor2
 * @param motor3  motor3
 * @param motor4  motor4
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_motor_value_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint16_t motor1, uint16_t motor2, uint16_t motor3, uint16_t motor4)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_motor_value_LEN];
    _mav_put_uint16_t(buf, 0, motor1);
    _mav_put_uint16_t(buf, 2, motor2);
    _mav_put_uint16_t(buf, 4, motor3);
    _mav_put_uint16_t(buf, 6, motor4);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_motor_value_LEN);
#else
    mavlink_motor_value_t packet;
    packet.motor1 = motor1;
    packet.motor2 = motor2;
    packet.motor3 = motor3;
    packet.motor4 = motor4;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_motor_value_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_motor_value;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_motor_value_MIN_LEN, MAVLINK_MSG_ID_motor_value_LEN, MAVLINK_MSG_ID_motor_value_CRC);
}

/**
 * @brief Pack a motor_value message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param motor1  motor1
 * @param motor2  motor2
 * @param motor3  motor3
 * @param motor4  motor4
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_motor_value_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint16_t motor1,uint16_t motor2,uint16_t motor3,uint16_t motor4)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_motor_value_LEN];
    _mav_put_uint16_t(buf, 0, motor1);
    _mav_put_uint16_t(buf, 2, motor2);
    _mav_put_uint16_t(buf, 4, motor3);
    _mav_put_uint16_t(buf, 6, motor4);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_motor_value_LEN);
#else
    mavlink_motor_value_t packet;
    packet.motor1 = motor1;
    packet.motor2 = motor2;
    packet.motor3 = motor3;
    packet.motor4 = motor4;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_motor_value_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_motor_value;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_motor_value_MIN_LEN, MAVLINK_MSG_ID_motor_value_LEN, MAVLINK_MSG_ID_motor_value_CRC);
}

/**
 * @brief Encode a motor_value struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param motor_value C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_motor_value_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_motor_value_t* motor_value)
{
    return mavlink_msg_motor_value_pack(system_id, component_id, msg, motor_value->motor1, motor_value->motor2, motor_value->motor3, motor_value->motor4);
}

/**
 * @brief Encode a motor_value struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param motor_value C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_motor_value_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_motor_value_t* motor_value)
{
    return mavlink_msg_motor_value_pack_chan(system_id, component_id, chan, msg, motor_value->motor1, motor_value->motor2, motor_value->motor3, motor_value->motor4);
}

/**
 * @brief Send a motor_value message
 * @param chan MAVLink channel to send the message
 *
 * @param motor1  motor1
 * @param motor2  motor2
 * @param motor3  motor3
 * @param motor4  motor4
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_motor_value_send(mavlink_channel_t chan, uint16_t motor1, uint16_t motor2, uint16_t motor3, uint16_t motor4)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_motor_value_LEN];
    _mav_put_uint16_t(buf, 0, motor1);
    _mav_put_uint16_t(buf, 2, motor2);
    _mav_put_uint16_t(buf, 4, motor3);
    _mav_put_uint16_t(buf, 6, motor4);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_motor_value, buf, MAVLINK_MSG_ID_motor_value_MIN_LEN, MAVLINK_MSG_ID_motor_value_LEN, MAVLINK_MSG_ID_motor_value_CRC);
#else
    mavlink_motor_value_t packet;
    packet.motor1 = motor1;
    packet.motor2 = motor2;
    packet.motor3 = motor3;
    packet.motor4 = motor4;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_motor_value, (const char *)&packet, MAVLINK_MSG_ID_motor_value_MIN_LEN, MAVLINK_MSG_ID_motor_value_LEN, MAVLINK_MSG_ID_motor_value_CRC);
#endif
}

/**
 * @brief Send a motor_value message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_motor_value_send_struct(mavlink_channel_t chan, const mavlink_motor_value_t* motor_value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_motor_value_send(chan, motor_value->motor1, motor_value->motor2, motor_value->motor3, motor_value->motor4);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_motor_value, (const char *)motor_value, MAVLINK_MSG_ID_motor_value_MIN_LEN, MAVLINK_MSG_ID_motor_value_LEN, MAVLINK_MSG_ID_motor_value_CRC);
#endif
}

#if MAVLINK_MSG_ID_motor_value_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_motor_value_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint16_t motor1, uint16_t motor2, uint16_t motor3, uint16_t motor4)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint16_t(buf, 0, motor1);
    _mav_put_uint16_t(buf, 2, motor2);
    _mav_put_uint16_t(buf, 4, motor3);
    _mav_put_uint16_t(buf, 6, motor4);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_motor_value, buf, MAVLINK_MSG_ID_motor_value_MIN_LEN, MAVLINK_MSG_ID_motor_value_LEN, MAVLINK_MSG_ID_motor_value_CRC);
#else
    mavlink_motor_value_t *packet = (mavlink_motor_value_t *)msgbuf;
    packet->motor1 = motor1;
    packet->motor2 = motor2;
    packet->motor3 = motor3;
    packet->motor4 = motor4;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_motor_value, (const char *)packet, MAVLINK_MSG_ID_motor_value_MIN_LEN, MAVLINK_MSG_ID_motor_value_LEN, MAVLINK_MSG_ID_motor_value_CRC);
#endif
}
#endif

#endif

// MESSAGE motor_value UNPACKING


/**
 * @brief Get field motor1 from motor_value message
 *
 * @return  motor1
 */
static inline uint16_t mavlink_msg_motor_value_get_motor1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Get field motor2 from motor_value message
 *
 * @return  motor2
 */
static inline uint16_t mavlink_msg_motor_value_get_motor2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  2);
}

/**
 * @brief Get field motor3 from motor_value message
 *
 * @return  motor3
 */
static inline uint16_t mavlink_msg_motor_value_get_motor3(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  4);
}

/**
 * @brief Get field motor4 from motor_value message
 *
 * @return  motor4
 */
static inline uint16_t mavlink_msg_motor_value_get_motor4(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  6);
}

/**
 * @brief Decode a motor_value message into a struct
 *
 * @param msg The message to decode
 * @param motor_value C-struct to decode the message contents into
 */
static inline void mavlink_msg_motor_value_decode(const mavlink_message_t* msg, mavlink_motor_value_t* motor_value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    motor_value->motor1 = mavlink_msg_motor_value_get_motor1(msg);
    motor_value->motor2 = mavlink_msg_motor_value_get_motor2(msg);
    motor_value->motor3 = mavlink_msg_motor_value_get_motor3(msg);
    motor_value->motor4 = mavlink_msg_motor_value_get_motor4(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_motor_value_LEN? msg->len : MAVLINK_MSG_ID_motor_value_LEN;
        memset(motor_value, 0, MAVLINK_MSG_ID_motor_value_LEN);
    memcpy(motor_value, _MAV_PAYLOAD(msg), len);
#endif
}
