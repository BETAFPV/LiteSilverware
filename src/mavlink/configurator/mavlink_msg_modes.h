#pragma once
// MESSAGE modes PACKING

#define MAVLINK_MSG_ID_modes 12

MAVPACKED(
typedef struct __mavlink_modes_t {
 uint8_t acron; /*<  acron*/
 uint8_t acros; /*<  acros*/
 uint8_t leveln; /*<  leveln*/
 uint8_t levels; /*<  levels*/
 uint8_t horn; /*<  horn*/
 uint8_t hors; /*<  hors*/
 uint8_t racean; /*<  racean*/
 uint8_t raceas; /*<  raceas*/
 uint8_t racehn; /*<  racehn*/
 uint8_t racehs; /*<  racehs*/
 uint8_t turtlen; /*<  turtlen*/
 uint8_t turtles; /*<  turtles*/
}) mavlink_modes_t;

#define MAVLINK_MSG_ID_modes_LEN 12
#define MAVLINK_MSG_ID_modes_MIN_LEN 12
#define MAVLINK_MSG_ID_12_LEN 12
#define MAVLINK_MSG_ID_12_MIN_LEN 12

#define MAVLINK_MSG_ID_modes_CRC 111
#define MAVLINK_MSG_ID_12_CRC 111



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_modes { \
    12, \
    "modes", \
    12, \
    {  { "acron", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_modes_t, acron) }, \
         { "acros", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_modes_t, acros) }, \
         { "leveln", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_modes_t, leveln) }, \
         { "levels", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_modes_t, levels) }, \
         { "horn", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_modes_t, horn) }, \
         { "hors", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_modes_t, hors) }, \
         { "racean", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_modes_t, racean) }, \
         { "raceas", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_modes_t, raceas) }, \
         { "racehn", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_modes_t, racehn) }, \
         { "racehs", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_modes_t, racehs) }, \
         { "turtlen", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_modes_t, turtlen) }, \
         { "turtles", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_modes_t, turtles) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_modes { \
    "modes", \
    12, \
    {  { "acron", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_modes_t, acron) }, \
         { "acros", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_modes_t, acros) }, \
         { "leveln", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_modes_t, leveln) }, \
         { "levels", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_modes_t, levels) }, \
         { "horn", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_modes_t, horn) }, \
         { "hors", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_modes_t, hors) }, \
         { "racean", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_modes_t, racean) }, \
         { "raceas", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_modes_t, raceas) }, \
         { "racehn", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_modes_t, racehn) }, \
         { "racehs", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_modes_t, racehs) }, \
         { "turtlen", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_modes_t, turtlen) }, \
         { "turtles", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_modes_t, turtles) }, \
         } \
}
#endif

/**
 * @brief Pack a modes message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param acron  acron
 * @param acros  acros
 * @param leveln  leveln
 * @param levels  levels
 * @param horn  horn
 * @param hors  hors
 * @param racean  racean
 * @param raceas  raceas
 * @param racehn  racehn
 * @param racehs  racehs
 * @param turtlen  turtlen
 * @param turtles  turtles
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_modes_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t acron, uint8_t acros, uint8_t leveln, uint8_t levels, uint8_t horn, uint8_t hors, uint8_t racean, uint8_t raceas, uint8_t racehn, uint8_t racehs, uint8_t turtlen, uint8_t turtles)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_modes_LEN];
    _mav_put_uint8_t(buf, 0, acron);
    _mav_put_uint8_t(buf, 1, acros);
    _mav_put_uint8_t(buf, 2, leveln);
    _mav_put_uint8_t(buf, 3, levels);
    _mav_put_uint8_t(buf, 4, horn);
    _mav_put_uint8_t(buf, 5, hors);
    _mav_put_uint8_t(buf, 6, racean);
    _mav_put_uint8_t(buf, 7, raceas);
    _mav_put_uint8_t(buf, 8, racehn);
    _mav_put_uint8_t(buf, 9, racehs);
    _mav_put_uint8_t(buf, 10, turtlen);
    _mav_put_uint8_t(buf, 11, turtles);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_modes_LEN);
#else
    mavlink_modes_t packet;
    packet.acron = acron;
    packet.acros = acros;
    packet.leveln = leveln;
    packet.levels = levels;
    packet.horn = horn;
    packet.hors = hors;
    packet.racean = racean;
    packet.raceas = raceas;
    packet.racehn = racehn;
    packet.racehs = racehs;
    packet.turtlen = turtlen;
    packet.turtles = turtles;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_modes_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_modes;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_modes_MIN_LEN, MAVLINK_MSG_ID_modes_LEN, MAVLINK_MSG_ID_modes_CRC);
}

/**
 * @brief Pack a modes message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param acron  acron
 * @param acros  acros
 * @param leveln  leveln
 * @param levels  levels
 * @param horn  horn
 * @param hors  hors
 * @param racean  racean
 * @param raceas  raceas
 * @param racehn  racehn
 * @param racehs  racehs
 * @param turtlen  turtlen
 * @param turtles  turtles
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_modes_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t acron,uint8_t acros,uint8_t leveln,uint8_t levels,uint8_t horn,uint8_t hors,uint8_t racean,uint8_t raceas,uint8_t racehn,uint8_t racehs,uint8_t turtlen,uint8_t turtles)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_modes_LEN];
    _mav_put_uint8_t(buf, 0, acron);
    _mav_put_uint8_t(buf, 1, acros);
    _mav_put_uint8_t(buf, 2, leveln);
    _mav_put_uint8_t(buf, 3, levels);
    _mav_put_uint8_t(buf, 4, horn);
    _mav_put_uint8_t(buf, 5, hors);
    _mav_put_uint8_t(buf, 6, racean);
    _mav_put_uint8_t(buf, 7, raceas);
    _mav_put_uint8_t(buf, 8, racehn);
    _mav_put_uint8_t(buf, 9, racehs);
    _mav_put_uint8_t(buf, 10, turtlen);
    _mav_put_uint8_t(buf, 11, turtles);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_modes_LEN);
#else
    mavlink_modes_t packet;
    packet.acron = acron;
    packet.acros = acros;
    packet.leveln = leveln;
    packet.levels = levels;
    packet.horn = horn;
    packet.hors = hors;
    packet.racean = racean;
    packet.raceas = raceas;
    packet.racehn = racehn;
    packet.racehs = racehs;
    packet.turtlen = turtlen;
    packet.turtles = turtles;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_modes_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_modes;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_modes_MIN_LEN, MAVLINK_MSG_ID_modes_LEN, MAVLINK_MSG_ID_modes_CRC);
}

/**
 * @brief Encode a modes struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param modes C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_modes_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_modes_t* modes)
{
    return mavlink_msg_modes_pack(system_id, component_id, msg, modes->acron, modes->acros, modes->leveln, modes->levels, modes->horn, modes->hors, modes->racean, modes->raceas, modes->racehn, modes->racehs, modes->turtlen, modes->turtles);
}

/**
 * @brief Encode a modes struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param modes C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_modes_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_modes_t* modes)
{
    return mavlink_msg_modes_pack_chan(system_id, component_id, chan, msg, modes->acron, modes->acros, modes->leveln, modes->levels, modes->horn, modes->hors, modes->racean, modes->raceas, modes->racehn, modes->racehs, modes->turtlen, modes->turtles);
}

/**
 * @brief Send a modes message
 * @param chan MAVLink channel to send the message
 *
 * @param acron  acron
 * @param acros  acros
 * @param leveln  leveln
 * @param levels  levels
 * @param horn  horn
 * @param hors  hors
 * @param racean  racean
 * @param raceas  raceas
 * @param racehn  racehn
 * @param racehs  racehs
 * @param turtlen  turtlen
 * @param turtles  turtles
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_modes_send(mavlink_channel_t chan, uint8_t acron, uint8_t acros, uint8_t leveln, uint8_t levels, uint8_t horn, uint8_t hors, uint8_t racean, uint8_t raceas, uint8_t racehn, uint8_t racehs, uint8_t turtlen, uint8_t turtles)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_modes_LEN];
    _mav_put_uint8_t(buf, 0, acron);
    _mav_put_uint8_t(buf, 1, acros);
    _mav_put_uint8_t(buf, 2, leveln);
    _mav_put_uint8_t(buf, 3, levels);
    _mav_put_uint8_t(buf, 4, horn);
    _mav_put_uint8_t(buf, 5, hors);
    _mav_put_uint8_t(buf, 6, racean);
    _mav_put_uint8_t(buf, 7, raceas);
    _mav_put_uint8_t(buf, 8, racehn);
    _mav_put_uint8_t(buf, 9, racehs);
    _mav_put_uint8_t(buf, 10, turtlen);
    _mav_put_uint8_t(buf, 11, turtles);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_modes, buf, MAVLINK_MSG_ID_modes_MIN_LEN, MAVLINK_MSG_ID_modes_LEN, MAVLINK_MSG_ID_modes_CRC);
#else
    mavlink_modes_t packet;
    packet.acron = acron;
    packet.acros = acros;
    packet.leveln = leveln;
    packet.levels = levels;
    packet.horn = horn;
    packet.hors = hors;
    packet.racean = racean;
    packet.raceas = raceas;
    packet.racehn = racehn;
    packet.racehs = racehs;
    packet.turtlen = turtlen;
    packet.turtles = turtles;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_modes, (const char *)&packet, MAVLINK_MSG_ID_modes_MIN_LEN, MAVLINK_MSG_ID_modes_LEN, MAVLINK_MSG_ID_modes_CRC);
#endif
}

/**
 * @brief Send a modes message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_modes_send_struct(mavlink_channel_t chan, const mavlink_modes_t* modes)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_modes_send(chan, modes->acron, modes->acros, modes->leveln, modes->levels, modes->horn, modes->hors, modes->racean, modes->raceas, modes->racehn, modes->racehs, modes->turtlen, modes->turtles);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_modes, (const char *)modes, MAVLINK_MSG_ID_modes_MIN_LEN, MAVLINK_MSG_ID_modes_LEN, MAVLINK_MSG_ID_modes_CRC);
#endif
}

#if MAVLINK_MSG_ID_modes_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_modes_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t acron, uint8_t acros, uint8_t leveln, uint8_t levels, uint8_t horn, uint8_t hors, uint8_t racean, uint8_t raceas, uint8_t racehn, uint8_t racehs, uint8_t turtlen, uint8_t turtles)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, acron);
    _mav_put_uint8_t(buf, 1, acros);
    _mav_put_uint8_t(buf, 2, leveln);
    _mav_put_uint8_t(buf, 3, levels);
    _mav_put_uint8_t(buf, 4, horn);
    _mav_put_uint8_t(buf, 5, hors);
    _mav_put_uint8_t(buf, 6, racean);
    _mav_put_uint8_t(buf, 7, raceas);
    _mav_put_uint8_t(buf, 8, racehn);
    _mav_put_uint8_t(buf, 9, racehs);
    _mav_put_uint8_t(buf, 10, turtlen);
    _mav_put_uint8_t(buf, 11, turtles);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_modes, buf, MAVLINK_MSG_ID_modes_MIN_LEN, MAVLINK_MSG_ID_modes_LEN, MAVLINK_MSG_ID_modes_CRC);
#else
    mavlink_modes_t *packet = (mavlink_modes_t *)msgbuf;
    packet->acron = acron;
    packet->acros = acros;
    packet->leveln = leveln;
    packet->levels = levels;
    packet->horn = horn;
    packet->hors = hors;
    packet->racean = racean;
    packet->raceas = raceas;
    packet->racehn = racehn;
    packet->racehs = racehs;
    packet->turtlen = turtlen;
    packet->turtles = turtles;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_modes, (const char *)packet, MAVLINK_MSG_ID_modes_MIN_LEN, MAVLINK_MSG_ID_modes_LEN, MAVLINK_MSG_ID_modes_CRC);
#endif
}
#endif

#endif

// MESSAGE modes UNPACKING


/**
 * @brief Get field acron from modes message
 *
 * @return  acron
 */
static inline uint8_t mavlink_msg_modes_get_acron(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field acros from modes message
 *
 * @return  acros
 */
static inline uint8_t mavlink_msg_modes_get_acros(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Get field leveln from modes message
 *
 * @return  leveln
 */
static inline uint8_t mavlink_msg_modes_get_leveln(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Get field levels from modes message
 *
 * @return  levels
 */
static inline uint8_t mavlink_msg_modes_get_levels(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  3);
}

/**
 * @brief Get field horn from modes message
 *
 * @return  horn
 */
static inline uint8_t mavlink_msg_modes_get_horn(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field hors from modes message
 *
 * @return  hors
 */
static inline uint8_t mavlink_msg_modes_get_hors(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Get field racean from modes message
 *
 * @return  racean
 */
static inline uint8_t mavlink_msg_modes_get_racean(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  6);
}

/**
 * @brief Get field raceas from modes message
 *
 * @return  raceas
 */
static inline uint8_t mavlink_msg_modes_get_raceas(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  7);
}

/**
 * @brief Get field racehn from modes message
 *
 * @return  racehn
 */
static inline uint8_t mavlink_msg_modes_get_racehn(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field racehs from modes message
 *
 * @return  racehs
 */
static inline uint8_t mavlink_msg_modes_get_racehs(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  9);
}

/**
 * @brief Get field turtlen from modes message
 *
 * @return  turtlen
 */
static inline uint8_t mavlink_msg_modes_get_turtlen(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  10);
}

/**
 * @brief Get field turtles from modes message
 *
 * @return  turtles
 */
static inline uint8_t mavlink_msg_modes_get_turtles(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  11);
}

/**
 * @brief Decode a modes message into a struct
 *
 * @param msg The message to decode
 * @param modes C-struct to decode the message contents into
 */
static inline void mavlink_msg_modes_decode(const mavlink_message_t* msg, mavlink_modes_t* modes)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    modes->acron = mavlink_msg_modes_get_acron(msg);
    modes->acros = mavlink_msg_modes_get_acros(msg);
    modes->leveln = mavlink_msg_modes_get_leveln(msg);
    modes->levels = mavlink_msg_modes_get_levels(msg);
    modes->horn = mavlink_msg_modes_get_horn(msg);
    modes->hors = mavlink_msg_modes_get_hors(msg);
    modes->racean = mavlink_msg_modes_get_racean(msg);
    modes->raceas = mavlink_msg_modes_get_raceas(msg);
    modes->racehn = mavlink_msg_modes_get_racehn(msg);
    modes->racehs = mavlink_msg_modes_get_racehs(msg);
    modes->turtlen = mavlink_msg_modes_get_turtlen(msg);
    modes->turtles = mavlink_msg_modes_get_turtles(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_modes_LEN? msg->len : MAVLINK_MSG_ID_modes_LEN;
        memset(modes, 0, MAVLINK_MSG_ID_modes_LEN);
    memcpy(modes, _MAV_PAYLOAD(msg), len);
#endif
}
