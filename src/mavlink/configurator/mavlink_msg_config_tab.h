#pragma once
// MESSAGE config_tab PACKING

#define MAVLINK_MSG_ID_config_tab 9

MAVPACKED(
typedef struct __mavlink_config_tab_t {
 uint8_t tab_index; /*<  tab_index*/
 uint8_t sub_index; /*<  sub_index*/
}) mavlink_config_tab_t;

#define MAVLINK_MSG_ID_config_tab_LEN 2
#define MAVLINK_MSG_ID_config_tab_MIN_LEN 2
#define MAVLINK_MSG_ID_9_LEN 2
#define MAVLINK_MSG_ID_9_MIN_LEN 2

#define MAVLINK_MSG_ID_config_tab_CRC 45
#define MAVLINK_MSG_ID_9_CRC 45



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_config_tab { \
    9, \
    "config_tab", \
    2, \
    {  { "tab_index", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_config_tab_t, tab_index) }, \
         { "sub_index", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_config_tab_t, sub_index) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_config_tab { \
    "config_tab", \
    2, \
    {  { "tab_index", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_config_tab_t, tab_index) }, \
         { "sub_index", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_config_tab_t, sub_index) }, \
         } \
}
#endif

/**
 * @brief Pack a config_tab message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param tab_index  tab_index
 * @param sub_index  sub_index
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_config_tab_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t tab_index, uint8_t sub_index)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_config_tab_LEN];
    _mav_put_uint8_t(buf, 0, tab_index);
    _mav_put_uint8_t(buf, 1, sub_index);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_config_tab_LEN);
#else
    mavlink_config_tab_t packet;
    packet.tab_index = tab_index;
    packet.sub_index = sub_index;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_config_tab_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_config_tab;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_config_tab_MIN_LEN, MAVLINK_MSG_ID_config_tab_LEN, MAVLINK_MSG_ID_config_tab_CRC);
}

/**
 * @brief Pack a config_tab message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param tab_index  tab_index
 * @param sub_index  sub_index
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_config_tab_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t tab_index,uint8_t sub_index)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_config_tab_LEN];
    _mav_put_uint8_t(buf, 0, tab_index);
    _mav_put_uint8_t(buf, 1, sub_index);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_config_tab_LEN);
#else
    mavlink_config_tab_t packet;
    packet.tab_index = tab_index;
    packet.sub_index = sub_index;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_config_tab_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_config_tab;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_config_tab_MIN_LEN, MAVLINK_MSG_ID_config_tab_LEN, MAVLINK_MSG_ID_config_tab_CRC);
}

/**
 * @brief Encode a config_tab struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param config_tab C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_config_tab_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_config_tab_t* config_tab)
{
    return mavlink_msg_config_tab_pack(system_id, component_id, msg, config_tab->tab_index, config_tab->sub_index);
}

/**
 * @brief Encode a config_tab struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param config_tab C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_config_tab_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_config_tab_t* config_tab)
{
    return mavlink_msg_config_tab_pack_chan(system_id, component_id, chan, msg, config_tab->tab_index, config_tab->sub_index);
}

/**
 * @brief Send a config_tab message
 * @param chan MAVLink channel to send the message
 *
 * @param tab_index  tab_index
 * @param sub_index  sub_index
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_config_tab_send(mavlink_channel_t chan, uint8_t tab_index, uint8_t sub_index)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_config_tab_LEN];
    _mav_put_uint8_t(buf, 0, tab_index);
    _mav_put_uint8_t(buf, 1, sub_index);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_config_tab, buf, MAVLINK_MSG_ID_config_tab_MIN_LEN, MAVLINK_MSG_ID_config_tab_LEN, MAVLINK_MSG_ID_config_tab_CRC);
#else
    mavlink_config_tab_t packet;
    packet.tab_index = tab_index;
    packet.sub_index = sub_index;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_config_tab, (const char *)&packet, MAVLINK_MSG_ID_config_tab_MIN_LEN, MAVLINK_MSG_ID_config_tab_LEN, MAVLINK_MSG_ID_config_tab_CRC);
#endif
}

/**
 * @brief Send a config_tab message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_config_tab_send_struct(mavlink_channel_t chan, const mavlink_config_tab_t* config_tab)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_config_tab_send(chan, config_tab->tab_index, config_tab->sub_index);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_config_tab, (const char *)config_tab, MAVLINK_MSG_ID_config_tab_MIN_LEN, MAVLINK_MSG_ID_config_tab_LEN, MAVLINK_MSG_ID_config_tab_CRC);
#endif
}

#if MAVLINK_MSG_ID_config_tab_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_config_tab_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t tab_index, uint8_t sub_index)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, tab_index);
    _mav_put_uint8_t(buf, 1, sub_index);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_config_tab, buf, MAVLINK_MSG_ID_config_tab_MIN_LEN, MAVLINK_MSG_ID_config_tab_LEN, MAVLINK_MSG_ID_config_tab_CRC);
#else
    mavlink_config_tab_t *packet = (mavlink_config_tab_t *)msgbuf;
    packet->tab_index = tab_index;
    packet->sub_index = sub_index;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_config_tab, (const char *)packet, MAVLINK_MSG_ID_config_tab_MIN_LEN, MAVLINK_MSG_ID_config_tab_LEN, MAVLINK_MSG_ID_config_tab_CRC);
#endif
}
#endif

#endif

// MESSAGE config_tab UNPACKING


/**
 * @brief Get field tab_index from config_tab message
 *
 * @return  tab_index
 */
static inline uint8_t mavlink_msg_config_tab_get_tab_index(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field sub_index from config_tab message
 *
 * @return  sub_index
 */
static inline uint8_t mavlink_msg_config_tab_get_sub_index(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Decode a config_tab message into a struct
 *
 * @param msg The message to decode
 * @param config_tab C-struct to decode the message contents into
 */
static inline void mavlink_msg_config_tab_decode(const mavlink_message_t* msg, mavlink_config_tab_t* config_tab)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    config_tab->tab_index = mavlink_msg_config_tab_get_tab_index(msg);
    config_tab->sub_index = mavlink_msg_config_tab_get_sub_index(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_config_tab_LEN? msg->len : MAVLINK_MSG_ID_config_tab_LEN;
        memset(config_tab, 0, MAVLINK_MSG_ID_config_tab_LEN);
    memcpy(config_tab, _MAV_PAYLOAD(msg), len);
#endif
}
