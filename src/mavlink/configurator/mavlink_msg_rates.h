#pragma once
// MESSAGE rates PACKING

#define MAVLINK_MSG_ID_rates 8

MAVPACKED(
typedef struct __mavlink_rates_t {
 uint8_t rspb; /*<  reset save PB*/
 uint8_t rrc; /*<  roll_rate*/
 uint8_t rr; /*<  roll_rc_rate*/
 uint8_t re; /*<  roll_rc_curve*/
 uint8_t prc; /*<  pitch_rate*/
 uint8_t pr; /*<  pitch_rc_rate*/
 uint8_t pe; /*<  pitch_rc_curve*/
 uint8_t yrc; /*<  yaw_rate*/
 uint8_t yr; /*<  yaw_rc_rate*/
 uint8_t ye; /*<  yaw_rc_curve*/
}) mavlink_rates_t;

#define MAVLINK_MSG_ID_rates_LEN 10
#define MAVLINK_MSG_ID_rates_MIN_LEN 10
#define MAVLINK_MSG_ID_8_LEN 10
#define MAVLINK_MSG_ID_8_MIN_LEN 10

#define MAVLINK_MSG_ID_rates_CRC 95
#define MAVLINK_MSG_ID_8_CRC 95



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_rates { \
    8, \
    "rates", \
    10, \
    {  { "rspb", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_rates_t, rspb) }, \
         { "rrc", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_rates_t, rrc) }, \
         { "rr", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_rates_t, rr) }, \
         { "re", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_rates_t, re) }, \
         { "prc", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_rates_t, prc) }, \
         { "pr", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_rates_t, pr) }, \
         { "pe", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_rates_t, pe) }, \
         { "yrc", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_rates_t, yrc) }, \
         { "yr", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_rates_t, yr) }, \
         { "ye", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_rates_t, ye) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_rates { \
    "rates", \
    10, \
    {  { "rspb", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_rates_t, rspb) }, \
         { "rrc", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_rates_t, rrc) }, \
         { "rr", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_rates_t, rr) }, \
         { "re", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_rates_t, re) }, \
         { "prc", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_rates_t, prc) }, \
         { "pr", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_rates_t, pr) }, \
         { "pe", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_rates_t, pe) }, \
         { "yrc", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_rates_t, yrc) }, \
         { "yr", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_rates_t, yr) }, \
         { "ye", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_rates_t, ye) }, \
         } \
}
#endif

/**
 * @brief Pack a rates message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param rspb  reset save PB
 * @param rrc  roll_rate
 * @param rr  roll_rc_rate
 * @param re  roll_rc_curve
 * @param prc  pitch_rate
 * @param pr  pitch_rc_rate
 * @param pe  pitch_rc_curve
 * @param yrc  yaw_rate
 * @param yr  yaw_rc_rate
 * @param ye  yaw_rc_curve
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rates_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t rspb, uint8_t rrc, uint8_t rr, uint8_t re, uint8_t prc, uint8_t pr, uint8_t pe, uint8_t yrc, uint8_t yr, uint8_t ye)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_rates_LEN];
    _mav_put_uint8_t(buf, 0, rspb);
    _mav_put_uint8_t(buf, 1, rrc);
    _mav_put_uint8_t(buf, 2, rr);
    _mav_put_uint8_t(buf, 3, re);
    _mav_put_uint8_t(buf, 4, prc);
    _mav_put_uint8_t(buf, 5, pr);
    _mav_put_uint8_t(buf, 6, pe);
    _mav_put_uint8_t(buf, 7, yrc);
    _mav_put_uint8_t(buf, 8, yr);
    _mav_put_uint8_t(buf, 9, ye);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_rates_LEN);
#else
    mavlink_rates_t packet;
    packet.rspb = rspb;
    packet.rrc = rrc;
    packet.rr = rr;
    packet.re = re;
    packet.prc = prc;
    packet.pr = pr;
    packet.pe = pe;
    packet.yrc = yrc;
    packet.yr = yr;
    packet.ye = ye;

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
 * @param rspb  reset save PB
 * @param rrc  roll_rate
 * @param rr  roll_rc_rate
 * @param re  roll_rc_curve
 * @param prc  pitch_rate
 * @param pr  pitch_rc_rate
 * @param pe  pitch_rc_curve
 * @param yrc  yaw_rate
 * @param yr  yaw_rc_rate
 * @param ye  yaw_rc_curve
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rates_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t rspb,uint8_t rrc,uint8_t rr,uint8_t re,uint8_t prc,uint8_t pr,uint8_t pe,uint8_t yrc,uint8_t yr,uint8_t ye)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_rates_LEN];
    _mav_put_uint8_t(buf, 0, rspb);
    _mav_put_uint8_t(buf, 1, rrc);
    _mav_put_uint8_t(buf, 2, rr);
    _mav_put_uint8_t(buf, 3, re);
    _mav_put_uint8_t(buf, 4, prc);
    _mav_put_uint8_t(buf, 5, pr);
    _mav_put_uint8_t(buf, 6, pe);
    _mav_put_uint8_t(buf, 7, yrc);
    _mav_put_uint8_t(buf, 8, yr);
    _mav_put_uint8_t(buf, 9, ye);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_rates_LEN);
#else
    mavlink_rates_t packet;
    packet.rspb = rspb;
    packet.rrc = rrc;
    packet.rr = rr;
    packet.re = re;
    packet.prc = prc;
    packet.pr = pr;
    packet.pe = pe;
    packet.yrc = yrc;
    packet.yr = yr;
    packet.ye = ye;

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
    return mavlink_msg_rates_pack(system_id, component_id, msg, rates->rspb, rates->rrc, rates->rr, rates->re, rates->prc, rates->pr, rates->pe, rates->yrc, rates->yr, rates->ye);
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
    return mavlink_msg_rates_pack_chan(system_id, component_id, chan, msg, rates->rspb, rates->rrc, rates->rr, rates->re, rates->prc, rates->pr, rates->pe, rates->yrc, rates->yr, rates->ye);
}

/**
 * @brief Send a rates message
 * @param chan MAVLink channel to send the message
 *
 * @param rspb  reset save PB
 * @param rrc  roll_rate
 * @param rr  roll_rc_rate
 * @param re  roll_rc_curve
 * @param prc  pitch_rate
 * @param pr  pitch_rc_rate
 * @param pe  pitch_rc_curve
 * @param yrc  yaw_rate
 * @param yr  yaw_rc_rate
 * @param ye  yaw_rc_curve
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_rates_send(mavlink_channel_t chan, uint8_t rspb, uint8_t rrc, uint8_t rr, uint8_t re, uint8_t prc, uint8_t pr, uint8_t pe, uint8_t yrc, uint8_t yr, uint8_t ye)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_rates_LEN];
    _mav_put_uint8_t(buf, 0, rspb);
    _mav_put_uint8_t(buf, 1, rrc);
    _mav_put_uint8_t(buf, 2, rr);
    _mav_put_uint8_t(buf, 3, re);
    _mav_put_uint8_t(buf, 4, prc);
    _mav_put_uint8_t(buf, 5, pr);
    _mav_put_uint8_t(buf, 6, pe);
    _mav_put_uint8_t(buf, 7, yrc);
    _mav_put_uint8_t(buf, 8, yr);
    _mav_put_uint8_t(buf, 9, ye);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_rates, buf, MAVLINK_MSG_ID_rates_MIN_LEN, MAVLINK_MSG_ID_rates_LEN, MAVLINK_MSG_ID_rates_CRC);
#else
    mavlink_rates_t packet;
    packet.rspb = rspb;
    packet.rrc = rrc;
    packet.rr = rr;
    packet.re = re;
    packet.prc = prc;
    packet.pr = pr;
    packet.pe = pe;
    packet.yrc = yrc;
    packet.yr = yr;
    packet.ye = ye;

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
    mavlink_msg_rates_send(chan, rates->rspb, rates->rrc, rates->rr, rates->re, rates->prc, rates->pr, rates->pe, rates->yrc, rates->yr, rates->ye);
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
static inline void mavlink_msg_rates_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t rspb, uint8_t rrc, uint8_t rr, uint8_t re, uint8_t prc, uint8_t pr, uint8_t pe, uint8_t yrc, uint8_t yr, uint8_t ye)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, rspb);
    _mav_put_uint8_t(buf, 1, rrc);
    _mav_put_uint8_t(buf, 2, rr);
    _mav_put_uint8_t(buf, 3, re);
    _mav_put_uint8_t(buf, 4, prc);
    _mav_put_uint8_t(buf, 5, pr);
    _mav_put_uint8_t(buf, 6, pe);
    _mav_put_uint8_t(buf, 7, yrc);
    _mav_put_uint8_t(buf, 8, yr);
    _mav_put_uint8_t(buf, 9, ye);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_rates, buf, MAVLINK_MSG_ID_rates_MIN_LEN, MAVLINK_MSG_ID_rates_LEN, MAVLINK_MSG_ID_rates_CRC);
#else
    mavlink_rates_t *packet = (mavlink_rates_t *)msgbuf;
    packet->rspb = rspb;
    packet->rrc = rrc;
    packet->rr = rr;
    packet->re = re;
    packet->prc = prc;
    packet->pr = pr;
    packet->pe = pe;
    packet->yrc = yrc;
    packet->yr = yr;
    packet->ye = ye;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_rates, (const char *)packet, MAVLINK_MSG_ID_rates_MIN_LEN, MAVLINK_MSG_ID_rates_LEN, MAVLINK_MSG_ID_rates_CRC);
#endif
}
#endif

#endif

// MESSAGE rates UNPACKING


/**
 * @brief Get field rspb from rates message
 *
 * @return  reset save PB
 */
static inline uint8_t mavlink_msg_rates_get_rspb(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field rrc from rates message
 *
 * @return  roll_rate
 */
static inline uint8_t mavlink_msg_rates_get_rrc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Get field rr from rates message
 *
 * @return  roll_rc_rate
 */
static inline uint8_t mavlink_msg_rates_get_rr(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Get field re from rates message
 *
 * @return  roll_rc_curve
 */
static inline uint8_t mavlink_msg_rates_get_re(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  3);
}

/**
 * @brief Get field prc from rates message
 *
 * @return  pitch_rate
 */
static inline uint8_t mavlink_msg_rates_get_prc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field pr from rates message
 *
 * @return  pitch_rc_rate
 */
static inline uint8_t mavlink_msg_rates_get_pr(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Get field pe from rates message
 *
 * @return  pitch_rc_curve
 */
static inline uint8_t mavlink_msg_rates_get_pe(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  6);
}

/**
 * @brief Get field yrc from rates message
 *
 * @return  yaw_rate
 */
static inline uint8_t mavlink_msg_rates_get_yrc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  7);
}

/**
 * @brief Get field yr from rates message
 *
 * @return  yaw_rc_rate
 */
static inline uint8_t mavlink_msg_rates_get_yr(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field ye from rates message
 *
 * @return  yaw_rc_curve
 */
static inline uint8_t mavlink_msg_rates_get_ye(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  9);
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
    rates->rspb = mavlink_msg_rates_get_rspb(msg);
    rates->rrc = mavlink_msg_rates_get_rrc(msg);
    rates->rr = mavlink_msg_rates_get_rr(msg);
    rates->re = mavlink_msg_rates_get_re(msg);
    rates->prc = mavlink_msg_rates_get_prc(msg);
    rates->pr = mavlink_msg_rates_get_pr(msg);
    rates->pe = mavlink_msg_rates_get_pe(msg);
    rates->yrc = mavlink_msg_rates_get_yrc(msg);
    rates->yr = mavlink_msg_rates_get_yr(msg);
    rates->ye = mavlink_msg_rates_get_ye(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_rates_LEN? msg->len : MAVLINK_MSG_ID_rates_LEN;
        memset(rates, 0, MAVLINK_MSG_ID_rates_LEN);
    memcpy(rates, _MAV_PAYLOAD(msg), len);
#endif
}
