#pragma once
// MESSAGE save PACKING

#define MAVLINK_MSG_ID_save 11

MAVPACKED(
    typedef struct __mavlink_save_t
{
    uint8_t save; /*<  save*/
}) mavlink_save_t;

#define MAVLINK_MSG_ID_save_LEN 1
#define MAVLINK_MSG_ID_save_MIN_LEN 1
#define MAVLINK_MSG_ID_11_LEN 1
#define MAVLINK_MSG_ID_11_MIN_LEN 1

#define MAVLINK_MSG_ID_save_CRC 75
#define MAVLINK_MSG_ID_11_CRC 75



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_save { \
    11, \
    "save", \
    1, \
    {  { "save", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_save_t, save) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_save { \
    "save", \
    1, \
    {  { "save", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_save_t, save) }, \
         } \
}
#endif

/**
 * @brief Pack a save message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param save  save
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_save_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
        uint8_t save)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_save_LEN];
    _mav_put_uint8_t(buf, 0, save);

    memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_save_LEN);
#else
    mavlink_save_t packet;
    packet.save = save;

    memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_save_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_save;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_save_MIN_LEN, MAVLINK_MSG_ID_save_LEN, MAVLINK_MSG_ID_save_CRC);
}

/**
 * @brief Pack a save message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param save  save
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_save_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
        mavlink_message_t* msg,
        uint8_t save)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_save_LEN];
    _mav_put_uint8_t(buf, 0, save);

    memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_save_LEN);
#else
    mavlink_save_t packet;
    packet.save = save;

    memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_save_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_save;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_save_MIN_LEN, MAVLINK_MSG_ID_save_LEN, MAVLINK_MSG_ID_save_CRC);
}

/**
 * @brief Encode a save struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param save C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_save_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_save_t* save)
{
    return mavlink_msg_save_pack(system_id, component_id, msg, save->save);
}

/**
 * @brief Encode a save struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param save C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_save_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_save_t* save)
{
    return mavlink_msg_save_pack_chan(system_id, component_id, chan, msg, save->save);
}

/**
 * @brief Send a save message
 * @param chan MAVLink channel to send the message
 *
 * @param save  save
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_save_send(mavlink_channel_t chan, uint8_t save)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_save_LEN];
    _mav_put_uint8_t(buf, 0, save);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_save, buf, MAVLINK_MSG_ID_save_MIN_LEN, MAVLINK_MSG_ID_save_LEN, MAVLINK_MSG_ID_save_CRC);
#else
    mavlink_save_t packet;
    packet.save = save;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_save, (const char *)&packet, MAVLINK_MSG_ID_save_MIN_LEN, MAVLINK_MSG_ID_save_LEN, MAVLINK_MSG_ID_save_CRC);
#endif
}

/**
 * @brief Send a save message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_save_send_struct(mavlink_channel_t chan, const mavlink_save_t* save)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_save_send(chan, save->save);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_save, (const char *)save, MAVLINK_MSG_ID_save_MIN_LEN, MAVLINK_MSG_ID_save_LEN, MAVLINK_MSG_ID_save_CRC);
#endif
}

#if MAVLINK_MSG_ID_save_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_save_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t save)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, save);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_save, buf, MAVLINK_MSG_ID_save_MIN_LEN, MAVLINK_MSG_ID_save_LEN, MAVLINK_MSG_ID_save_CRC);
#else
    mavlink_save_t *packet = (mavlink_save_t *)msgbuf;
    packet->save = save;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_save, (const char *)packet, MAVLINK_MSG_ID_save_MIN_LEN, MAVLINK_MSG_ID_save_LEN, MAVLINK_MSG_ID_save_CRC);
#endif
}
#endif

#endif

// MESSAGE save UNPACKING


/**
 * @brief Get field save from save message
 *
 * @return  save
 */
static inline uint8_t mavlink_msg_save_get_save(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a save message into a struct
 *
 * @param msg The message to decode
 * @param save C-struct to decode the message contents into
 */
static inline void mavlink_msg_save_decode(const mavlink_message_t* msg, mavlink_save_t* save)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    save->save = mavlink_msg_save_get_save(msg);
#else
    uint8_t len = msg->len < MAVLINK_MSG_ID_save_LEN? msg->len : MAVLINK_MSG_ID_save_LEN;
    memset(save, 0, MAVLINK_MSG_ID_save_LEN);
    memcpy(save, _MAV_PAYLOAD(msg), len);
#endif
}
