#pragma once
// MESSAGE status_build PACKING

#define MAVLINK_MSG_ID_status_build 14

MAVPACKED(
typedef struct __mavlink_status_build_t {
 float vol; /*<  vol*/
 uint16_t year; /*<  year*/
 uint8_t tar; /*<  tar*/
 uint8_t ver; /*<  version*/
 uint8_t month; /*<  month*/
 uint8_t day; /*<  day*/
 uint8_t hour; /*<  hour*/
 uint8_t min; /*<  min*/
 uint8_t sec; /*<  sec*/
 uint8_t cpu_load; /*<  cpu load*/
 uint8_t rx; /*<  rx*/
 uint8_t sensors; /*<  sensors*/
}) mavlink_status_build_t;

#define MAVLINK_MSG_ID_status_build_LEN 16
#define MAVLINK_MSG_ID_status_build_MIN_LEN 16
#define MAVLINK_MSG_ID_14_LEN 16
#define MAVLINK_MSG_ID_14_MIN_LEN 16

#define MAVLINK_MSG_ID_status_build_CRC 89
#define MAVLINK_MSG_ID_14_CRC 89



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_status_build { \
    14, \
    "status_build", \
    12, \
    {  { "tar", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_status_build_t, tar) }, \
         { "ver", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_status_build_t, ver) }, \
         { "year", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_status_build_t, year) }, \
         { "month", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_status_build_t, month) }, \
         { "day", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_status_build_t, day) }, \
         { "hour", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_status_build_t, hour) }, \
         { "min", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_status_build_t, min) }, \
         { "sec", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_status_build_t, sec) }, \
         { "cpu_load", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_status_build_t, cpu_load) }, \
         { "vol", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_status_build_t, vol) }, \
         { "rx", NULL, MAVLINK_TYPE_UINT8_T, 0, 14, offsetof(mavlink_status_build_t, rx) }, \
         { "sensors", NULL, MAVLINK_TYPE_UINT8_T, 0, 15, offsetof(mavlink_status_build_t, sensors) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_status_build { \
    "status_build", \
    12, \
    {  { "tar", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_status_build_t, tar) }, \
         { "ver", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_status_build_t, ver) }, \
         { "year", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_status_build_t, year) }, \
         { "month", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_status_build_t, month) }, \
         { "day", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_status_build_t, day) }, \
         { "hour", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_status_build_t, hour) }, \
         { "min", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_status_build_t, min) }, \
         { "sec", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_status_build_t, sec) }, \
         { "cpu_load", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_status_build_t, cpu_load) }, \
         { "vol", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_status_build_t, vol) }, \
         { "rx", NULL, MAVLINK_TYPE_UINT8_T, 0, 14, offsetof(mavlink_status_build_t, rx) }, \
         { "sensors", NULL, MAVLINK_TYPE_UINT8_T, 0, 15, offsetof(mavlink_status_build_t, sensors) }, \
         } \
}
#endif

/**
 * @brief Pack a status_build message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param tar  tar
 * @param ver  version
 * @param year  year
 * @param month  month
 * @param day  day
 * @param hour  hour
 * @param min  min
 * @param sec  sec
 * @param cpu_load  cpu load
 * @param vol  vol
 * @param rx  rx
 * @param sensors  sensors
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_status_build_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t tar, uint8_t ver, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec, uint8_t cpu_load, float vol, uint8_t rx, uint8_t sensors)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_status_build_LEN];
    _mav_put_float(buf, 0, vol);
    _mav_put_uint16_t(buf, 4, year);
    _mav_put_uint8_t(buf, 6, tar);
    _mav_put_uint8_t(buf, 7, ver);
    _mav_put_uint8_t(buf, 8, month);
    _mav_put_uint8_t(buf, 9, day);
    _mav_put_uint8_t(buf, 10, hour);
    _mav_put_uint8_t(buf, 11, min);
    _mav_put_uint8_t(buf, 12, sec);
    _mav_put_uint8_t(buf, 13, cpu_load);
    _mav_put_uint8_t(buf, 14, rx);
    _mav_put_uint8_t(buf, 15, sensors);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_status_build_LEN);
#else
    mavlink_status_build_t packet;
    packet.vol = vol;
    packet.year = year;
    packet.tar = tar;
    packet.ver = ver;
    packet.month = month;
    packet.day = day;
    packet.hour = hour;
    packet.min = min;
    packet.sec = sec;
    packet.cpu_load = cpu_load;
    packet.rx = rx;
    packet.sensors = sensors;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_status_build_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_status_build;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_status_build_MIN_LEN, MAVLINK_MSG_ID_status_build_LEN, MAVLINK_MSG_ID_status_build_CRC);
}

/**
 * @brief Pack a status_build message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param tar  tar
 * @param ver  version
 * @param year  year
 * @param month  month
 * @param day  day
 * @param hour  hour
 * @param min  min
 * @param sec  sec
 * @param cpu_load  cpu load
 * @param vol  vol
 * @param rx  rx
 * @param sensors  sensors
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_status_build_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t tar,uint8_t ver,uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec,uint8_t cpu_load,float vol,uint8_t rx,uint8_t sensors)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_status_build_LEN];
    _mav_put_float(buf, 0, vol);
    _mav_put_uint16_t(buf, 4, year);
    _mav_put_uint8_t(buf, 6, tar);
    _mav_put_uint8_t(buf, 7, ver);
    _mav_put_uint8_t(buf, 8, month);
    _mav_put_uint8_t(buf, 9, day);
    _mav_put_uint8_t(buf, 10, hour);
    _mav_put_uint8_t(buf, 11, min);
    _mav_put_uint8_t(buf, 12, sec);
    _mav_put_uint8_t(buf, 13, cpu_load);
    _mav_put_uint8_t(buf, 14, rx);
    _mav_put_uint8_t(buf, 15, sensors);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_status_build_LEN);
#else
    mavlink_status_build_t packet;
    packet.vol = vol;
    packet.year = year;
    packet.tar = tar;
    packet.ver = ver;
    packet.month = month;
    packet.day = day;
    packet.hour = hour;
    packet.min = min;
    packet.sec = sec;
    packet.cpu_load = cpu_load;
    packet.rx = rx;
    packet.sensors = sensors;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_status_build_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_status_build;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_status_build_MIN_LEN, MAVLINK_MSG_ID_status_build_LEN, MAVLINK_MSG_ID_status_build_CRC);
}

/**
 * @brief Encode a status_build struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param status_build C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_status_build_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_status_build_t* status_build)
{
    return mavlink_msg_status_build_pack(system_id, component_id, msg, status_build->tar, status_build->ver, status_build->year, status_build->month, status_build->day, status_build->hour, status_build->min, status_build->sec, status_build->cpu_load, status_build->vol, status_build->rx, status_build->sensors);
}

/**
 * @brief Encode a status_build struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param status_build C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_status_build_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_status_build_t* status_build)
{
    return mavlink_msg_status_build_pack_chan(system_id, component_id, chan, msg, status_build->tar, status_build->ver, status_build->year, status_build->month, status_build->day, status_build->hour, status_build->min, status_build->sec, status_build->cpu_load, status_build->vol, status_build->rx, status_build->sensors);
}

/**
 * @brief Send a status_build message
 * @param chan MAVLink channel to send the message
 *
 * @param tar  tar
 * @param ver  version
 * @param year  year
 * @param month  month
 * @param day  day
 * @param hour  hour
 * @param min  min
 * @param sec  sec
 * @param cpu_load  cpu load
 * @param vol  vol
 * @param rx  rx
 * @param sensors  sensors
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_status_build_send(mavlink_channel_t chan, uint8_t tar, uint8_t ver, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec, uint8_t cpu_load, float vol, uint8_t rx, uint8_t sensors)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_status_build_LEN];
    _mav_put_float(buf, 0, vol);
    _mav_put_uint16_t(buf, 4, year);
    _mav_put_uint8_t(buf, 6, tar);
    _mav_put_uint8_t(buf, 7, ver);
    _mav_put_uint8_t(buf, 8, month);
    _mav_put_uint8_t(buf, 9, day);
    _mav_put_uint8_t(buf, 10, hour);
    _mav_put_uint8_t(buf, 11, min);
    _mav_put_uint8_t(buf, 12, sec);
    _mav_put_uint8_t(buf, 13, cpu_load);
    _mav_put_uint8_t(buf, 14, rx);
    _mav_put_uint8_t(buf, 15, sensors);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_status_build, buf, MAVLINK_MSG_ID_status_build_MIN_LEN, MAVLINK_MSG_ID_status_build_LEN, MAVLINK_MSG_ID_status_build_CRC);
#else
    mavlink_status_build_t packet;
    packet.vol = vol;
    packet.year = year;
    packet.tar = tar;
    packet.ver = ver;
    packet.month = month;
    packet.day = day;
    packet.hour = hour;
    packet.min = min;
    packet.sec = sec;
    packet.cpu_load = cpu_load;
    packet.rx = rx;
    packet.sensors = sensors;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_status_build, (const char *)&packet, MAVLINK_MSG_ID_status_build_MIN_LEN, MAVLINK_MSG_ID_status_build_LEN, MAVLINK_MSG_ID_status_build_CRC);
#endif
}

/**
 * @brief Send a status_build message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_status_build_send_struct(mavlink_channel_t chan, const mavlink_status_build_t* status_build)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_status_build_send(chan, status_build->tar, status_build->ver, status_build->year, status_build->month, status_build->day, status_build->hour, status_build->min, status_build->sec, status_build->cpu_load, status_build->vol, status_build->rx, status_build->sensors);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_status_build, (const char *)status_build, MAVLINK_MSG_ID_status_build_MIN_LEN, MAVLINK_MSG_ID_status_build_LEN, MAVLINK_MSG_ID_status_build_CRC);
#endif
}

#if MAVLINK_MSG_ID_status_build_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_status_build_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t tar, uint8_t ver, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec, uint8_t cpu_load, float vol, uint8_t rx, uint8_t sensors)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, vol);
    _mav_put_uint16_t(buf, 4, year);
    _mav_put_uint8_t(buf, 6, tar);
    _mav_put_uint8_t(buf, 7, ver);
    _mav_put_uint8_t(buf, 8, month);
    _mav_put_uint8_t(buf, 9, day);
    _mav_put_uint8_t(buf, 10, hour);
    _mav_put_uint8_t(buf, 11, min);
    _mav_put_uint8_t(buf, 12, sec);
    _mav_put_uint8_t(buf, 13, cpu_load);
    _mav_put_uint8_t(buf, 14, rx);
    _mav_put_uint8_t(buf, 15, sensors);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_status_build, buf, MAVLINK_MSG_ID_status_build_MIN_LEN, MAVLINK_MSG_ID_status_build_LEN, MAVLINK_MSG_ID_status_build_CRC);
#else
    mavlink_status_build_t *packet = (mavlink_status_build_t *)msgbuf;
    packet->vol = vol;
    packet->year = year;
    packet->tar = tar;
    packet->ver = ver;
    packet->month = month;
    packet->day = day;
    packet->hour = hour;
    packet->min = min;
    packet->sec = sec;
    packet->cpu_load = cpu_load;
    packet->rx = rx;
    packet->sensors = sensors;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_status_build, (const char *)packet, MAVLINK_MSG_ID_status_build_MIN_LEN, MAVLINK_MSG_ID_status_build_LEN, MAVLINK_MSG_ID_status_build_CRC);
#endif
}
#endif

#endif

// MESSAGE status_build UNPACKING


/**
 * @brief Get field tar from status_build message
 *
 * @return  tar
 */
static inline uint8_t mavlink_msg_status_build_get_tar(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  6);
}

/**
 * @brief Get field ver from status_build message
 *
 * @return  version
 */
static inline uint8_t mavlink_msg_status_build_get_ver(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  7);
}

/**
 * @brief Get field year from status_build message
 *
 * @return  year
 */
static inline uint16_t mavlink_msg_status_build_get_year(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  4);
}

/**
 * @brief Get field month from status_build message
 *
 * @return  month
 */
static inline uint8_t mavlink_msg_status_build_get_month(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field day from status_build message
 *
 * @return  day
 */
static inline uint8_t mavlink_msg_status_build_get_day(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  9);
}

/**
 * @brief Get field hour from status_build message
 *
 * @return  hour
 */
static inline uint8_t mavlink_msg_status_build_get_hour(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  10);
}

/**
 * @brief Get field min from status_build message
 *
 * @return  min
 */
static inline uint8_t mavlink_msg_status_build_get_min(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  11);
}

/**
 * @brief Get field sec from status_build message
 *
 * @return  sec
 */
static inline uint8_t mavlink_msg_status_build_get_sec(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Get field cpu_load from status_build message
 *
 * @return  cpu load
 */
static inline uint8_t mavlink_msg_status_build_get_cpu_load(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  13);
}

/**
 * @brief Get field vol from status_build message
 *
 * @return  vol
 */
static inline float mavlink_msg_status_build_get_vol(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field rx from status_build message
 *
 * @return  rx
 */
static inline uint8_t mavlink_msg_status_build_get_rx(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  14);
}

/**
 * @brief Get field sensors from status_build message
 *
 * @return  sensors
 */
static inline uint8_t mavlink_msg_status_build_get_sensors(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  15);
}

/**
 * @brief Decode a status_build message into a struct
 *
 * @param msg The message to decode
 * @param status_build C-struct to decode the message contents into
 */
static inline void mavlink_msg_status_build_decode(const mavlink_message_t* msg, mavlink_status_build_t* status_build)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    status_build->vol = mavlink_msg_status_build_get_vol(msg);
    status_build->year = mavlink_msg_status_build_get_year(msg);
    status_build->tar = mavlink_msg_status_build_get_tar(msg);
    status_build->ver = mavlink_msg_status_build_get_ver(msg);
    status_build->month = mavlink_msg_status_build_get_month(msg);
    status_build->day = mavlink_msg_status_build_get_day(msg);
    status_build->hour = mavlink_msg_status_build_get_hour(msg);
    status_build->min = mavlink_msg_status_build_get_min(msg);
    status_build->sec = mavlink_msg_status_build_get_sec(msg);
    status_build->cpu_load = mavlink_msg_status_build_get_cpu_load(msg);
    status_build->rx = mavlink_msg_status_build_get_rx(msg);
    status_build->sensors = mavlink_msg_status_build_get_sensors(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_status_build_LEN? msg->len : MAVLINK_MSG_ID_status_build_LEN;
        memset(status_build, 0, MAVLINK_MSG_ID_status_build_LEN);
    memcpy(status_build, _MAV_PAYLOAD(msg), len);
#endif
}
