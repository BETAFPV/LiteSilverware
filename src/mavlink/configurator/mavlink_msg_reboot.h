#pragma once
// MESSAGE reboot PACKING

#define MAVLINK_MSG_ID_reboot 0

MAVPACKED(
typedef struct __mavlink_reboot_t {
 uint8_t reset; /*<  double*/
 uint8_t msp; /*<  double*/
}) mavlink_reboot_t;

#define MAVLINK_MSG_ID_reboot_LEN 2
#define MAVLINK_MSG_ID_reboot_MIN_LEN 2
#define MAVLINK_MSG_ID_0_LEN 2
#define MAVLINK_MSG_ID_0_MIN_LEN 2

#define MAVLINK_MSG_ID_reboot_CRC 159
#define MAVLINK_MSG_ID_0_CRC 159



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_reboot { \
    0, \
    "reboot", \
    2, \
    {  { "reset", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_reboot_t, reset) }, \
         { "msp", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_reboot_t, msp) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_reboot { \
    "reboot", \
    2, \
    {  { "reset", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_reboot_t, reset) }, \
         { "msp", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_reboot_t, msp) }, \
         } \
}
#endif

/**
 * @brief Pack a reboot message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param reset  double
 * @param msp  double
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_reboot_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t reset, uint8_t msp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_reboot_LEN];
    _mav_put_uint8_t(buf, 0, reset);
    _mav_put_uint8_t(buf, 1, msp);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_reboot_LEN);
#else
    mavlink_reboot_t packet;
    packet.reset = reset;
    packet.msp = msp;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_reboot_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_reboot;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_reboot_MIN_LEN, MAVLINK_MSG_ID_reboot_LEN, MAVLINK_MSG_ID_reboot_CRC);
}

/**
 * @brief Pack a reboot message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param reset  double
 * @param msp  double
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_reboot_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t reset,uint8_t msp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_reboot_LEN];
    _mav_put_uint8_t(buf, 0, reset);
    _mav_put_uint8_t(buf, 1, msp);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_reboot_LEN);
#else
    mavlink_reboot_t packet;
    packet.reset = reset;
    packet.msp = msp;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_reboot_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_reboot;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_reboot_MIN_LEN, MAVLINK_MSG_ID_reboot_LEN, MAVLINK_MSG_ID_reboot_CRC);
}

/**
 * @brief Encode a reboot struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param reboot C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_reboot_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_reboot_t* reboot)
{
    return mavlink_msg_reboot_pack(system_id, component_id, msg, reboot->reset, reboot->msp);
}

/**
 * @brief Encode a reboot struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param reboot C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_reboot_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_reboot_t* reboot)
{
    return mavlink_msg_reboot_pack_chan(system_id, component_id, chan, msg, reboot->reset, reboot->msp);
}

/**
 * @brief Send a reboot message
 * @param chan MAVLink channel to send the message
 *
 * @param reset  double
 * @param msp  double
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_reboot_send(mavlink_channel_t chan, uint8_t reset, uint8_t msp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_reboot_LEN];
    _mav_put_uint8_t(buf, 0, reset);
    _mav_put_uint8_t(buf, 1, msp);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_reboot, buf, MAVLINK_MSG_ID_reboot_MIN_LEN, MAVLINK_MSG_ID_reboot_LEN, MAVLINK_MSG_ID_reboot_CRC);
#else
    mavlink_reboot_t packet;
    packet.reset = reset;
    packet.msp = msp;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_reboot, (const char *)&packet, MAVLINK_MSG_ID_reboot_MIN_LEN, MAVLINK_MSG_ID_reboot_LEN, MAVLINK_MSG_ID_reboot_CRC);
#endif
}

/**
 * @brief Send a reboot message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_reboot_send_struct(mavlink_channel_t chan, const mavlink_reboot_t* reboot)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_reboot_send(chan, reboot->reset, reboot->msp);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_reboot, (const char *)reboot, MAVLINK_MSG_ID_reboot_MIN_LEN, MAVLINK_MSG_ID_reboot_LEN, MAVLINK_MSG_ID_reboot_CRC);
#endif
}

#if MAVLINK_MSG_ID_reboot_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_reboot_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t reset, uint8_t msp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, reset);
    _mav_put_uint8_t(buf, 1, msp);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_reboot, buf, MAVLINK_MSG_ID_reboot_MIN_LEN, MAVLINK_MSG_ID_reboot_LEN, MAVLINK_MSG_ID_reboot_CRC);
#else
    mavlink_reboot_t *packet = (mavlink_reboot_t *)msgbuf;
    packet->reset = reset;
    packet->msp = msp;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_reboot, (const char *)packet, MAVLINK_MSG_ID_reboot_MIN_LEN, MAVLINK_MSG_ID_reboot_LEN, MAVLINK_MSG_ID_reboot_CRC);
#endif
}
#endif

#endif

// MESSAGE reboot UNPACKING


/**
 * @brief Get field reset from reboot message
 *
 * @return  double
 */
static inline uint8_t mavlink_msg_reboot_get_reset(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field msp from reboot message
 *
 * @return  double
 */
static inline uint8_t mavlink_msg_reboot_get_msp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Decode a reboot message into a struct
 *
 * @param msg The message to decode
 * @param reboot C-struct to decode the message contents into
 */
static inline void mavlink_msg_reboot_decode(const mavlink_message_t* msg, mavlink_reboot_t* reboot)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    reboot->reset = mavlink_msg_reboot_get_reset(msg);
    reboot->msp = mavlink_msg_reboot_get_msp(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_reboot_LEN? msg->len : MAVLINK_MSG_ID_reboot_LEN;
        memset(reboot, 0, MAVLINK_MSG_ID_reboot_LEN);
    memcpy(reboot, _MAV_PAYLOAD(msg), len);
#endif
}
