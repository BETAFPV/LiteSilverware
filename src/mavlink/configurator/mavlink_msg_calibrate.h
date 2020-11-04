#pragma once
// MESSAGE calibrate PACKING

#define MAVLINK_MSG_ID_calibrate 1

MAVPACKED(
typedef struct __mavlink_calibrate_t {
 uint8_t calibrate; /*<  calibration*/
}) mavlink_calibrate_t;

#define MAVLINK_MSG_ID_calibrate_LEN 1
#define MAVLINK_MSG_ID_calibrate_MIN_LEN 1
#define MAVLINK_MSG_ID_1_LEN 1
#define MAVLINK_MSG_ID_1_MIN_LEN 1

#define MAVLINK_MSG_ID_calibrate_CRC 175
#define MAVLINK_MSG_ID_1_CRC 175



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_calibrate { \
    1, \
    "calibrate", \
    1, \
    {  { "calibrate", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_calibrate_t, calibrate) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_calibrate { \
    "calibrate", \
    1, \
    {  { "calibrate", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_calibrate_t, calibrate) }, \
         } \
}
#endif

/**
 * @brief Pack a calibrate message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param calibrate  calibration
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_calibrate_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t calibrate)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_calibrate_LEN];
    _mav_put_uint8_t(buf, 0, calibrate);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_calibrate_LEN);
#else
    mavlink_calibrate_t packet;
    packet.calibrate = calibrate;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_calibrate_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_calibrate;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_calibrate_MIN_LEN, MAVLINK_MSG_ID_calibrate_LEN, MAVLINK_MSG_ID_calibrate_CRC);
}

/**
 * @brief Pack a calibrate message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param calibrate  calibration
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_calibrate_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t calibrate)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_calibrate_LEN];
    _mav_put_uint8_t(buf, 0, calibrate);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_calibrate_LEN);
#else
    mavlink_calibrate_t packet;
    packet.calibrate = calibrate;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_calibrate_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_calibrate;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_calibrate_MIN_LEN, MAVLINK_MSG_ID_calibrate_LEN, MAVLINK_MSG_ID_calibrate_CRC);
}

/**
 * @brief Encode a calibrate struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param calibrate C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_calibrate_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_calibrate_t* calibrate)
{
    return mavlink_msg_calibrate_pack(system_id, component_id, msg, calibrate->calibrate);
}

/**
 * @brief Encode a calibrate struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param calibrate C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_calibrate_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_calibrate_t* calibrate)
{
    return mavlink_msg_calibrate_pack_chan(system_id, component_id, chan, msg, calibrate->calibrate);
}

/**
 * @brief Send a calibrate message
 * @param chan MAVLink channel to send the message
 *
 * @param calibrate  calibration
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_calibrate_send(mavlink_channel_t chan, uint8_t calibrate)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_calibrate_LEN];
    _mav_put_uint8_t(buf, 0, calibrate);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_calibrate, buf, MAVLINK_MSG_ID_calibrate_MIN_LEN, MAVLINK_MSG_ID_calibrate_LEN, MAVLINK_MSG_ID_calibrate_CRC);
#else
    mavlink_calibrate_t packet;
    packet.calibrate = calibrate;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_calibrate, (const char *)&packet, MAVLINK_MSG_ID_calibrate_MIN_LEN, MAVLINK_MSG_ID_calibrate_LEN, MAVLINK_MSG_ID_calibrate_CRC);
#endif
}

/**
 * @brief Send a calibrate message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_calibrate_send_struct(mavlink_channel_t chan, const mavlink_calibrate_t* calibrate)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_calibrate_send(chan, calibrate->calibrate);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_calibrate, (const char *)calibrate, MAVLINK_MSG_ID_calibrate_MIN_LEN, MAVLINK_MSG_ID_calibrate_LEN, MAVLINK_MSG_ID_calibrate_CRC);
#endif
}

#if MAVLINK_MSG_ID_calibrate_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_calibrate_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t calibrate)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, calibrate);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_calibrate, buf, MAVLINK_MSG_ID_calibrate_MIN_LEN, MAVLINK_MSG_ID_calibrate_LEN, MAVLINK_MSG_ID_calibrate_CRC);
#else
    mavlink_calibrate_t *packet = (mavlink_calibrate_t *)msgbuf;
    packet->calibrate = calibrate;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_calibrate, (const char *)packet, MAVLINK_MSG_ID_calibrate_MIN_LEN, MAVLINK_MSG_ID_calibrate_LEN, MAVLINK_MSG_ID_calibrate_CRC);
#endif
}
#endif

#endif

// MESSAGE calibrate UNPACKING


/**
 * @brief Get field calibrate from calibrate message
 *
 * @return  calibration
 */
static inline uint8_t mavlink_msg_calibrate_get_calibrate(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a calibrate message into a struct
 *
 * @param msg The message to decode
 * @param calibrate C-struct to decode the message contents into
 */
static inline void mavlink_msg_calibrate_decode(const mavlink_message_t* msg, mavlink_calibrate_t* calibrate)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    calibrate->calibrate = mavlink_msg_calibrate_get_calibrate(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_calibrate_LEN? msg->len : MAVLINK_MSG_ID_calibrate_LEN;
        memset(calibrate, 0, MAVLINK_MSG_ID_calibrate_LEN);
    memcpy(calibrate, _MAV_PAYLOAD(msg), len);
#endif
}
