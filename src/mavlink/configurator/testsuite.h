/** @file
 *    @brief MAVLink comm protocol testsuite generated from configurator.xml
 *    @see http://qgroundcontrol.org/mavlink/
 */
#pragma once
#ifndef CONFIGURATOR_TESTSUITE_H
#define CONFIGURATOR_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL

static void mavlink_test_configurator(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{

    mavlink_test_configurator(system_id, component_id, last_msg);
}
#endif




static void mavlink_test_reboot(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_reboot >= 256)
    {
        return;
    }
#endif
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;
    mavlink_reboot_t packet_in =
    {
        5
    };
    mavlink_reboot_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));
    packet1.flashfirmware = packet_in.flashfirmware;


#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        // cope with extensions
        memset(MAVLINK_MSG_ID_reboot_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_reboot_MIN_LEN);
    }
#endif
    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_reboot_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_reboot_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_reboot_pack(system_id, component_id, &msg, packet1.flashfirmware );
    mavlink_msg_reboot_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_reboot_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg, packet1.flashfirmware );
    mavlink_msg_reboot_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_to_send_buffer(buffer, &msg);
    for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++)
    {
        comm_send_ch(MAVLINK_COMM_0, buffer[i]);
    }
    mavlink_msg_reboot_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_reboot_send(MAVLINK_COMM_1, packet1.flashfirmware );
    mavlink_msg_reboot_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_calibrate(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_calibrate >= 256)
    {
        return;
    }
#endif
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;
    mavlink_calibrate_t packet_in =
    {
        5
    };
    mavlink_calibrate_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));
    packet1.calibrate = packet_in.calibrate;


#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        // cope with extensions
        memset(MAVLINK_MSG_ID_calibrate_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_calibrate_MIN_LEN);
    }
#endif
    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_calibrate_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_calibrate_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_calibrate_pack(system_id, component_id, &msg, packet1.calibrate );
    mavlink_msg_calibrate_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_calibrate_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg, packet1.calibrate );
    mavlink_msg_calibrate_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_to_send_buffer(buffer, &msg);
    for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++)
    {
        comm_send_ch(MAVLINK_COMM_0, buffer[i]);
    }
    mavlink_msg_calibrate_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_calibrate_send(MAVLINK_COMM_1, packet1.calibrate );
    mavlink_msg_calibrate_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_tx(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_tx >= 256)
    {
        return;
    }
#endif
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;
    mavlink_tx_t packet_in =
    {
        17.0,45.0,73.0,101.0,18067,18171,18275,18379
    };
    mavlink_tx_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));
    packet1.roll = packet_in.roll;
    packet1.pitch = packet_in.pitch;
    packet1.yaw = packet_in.yaw;
    packet1.throttle = packet_in.throttle;
    packet1.aux1 = packet_in.aux1;
    packet1.aux2 = packet_in.aux2;
    packet1.aux3 = packet_in.aux3;
    packet1.aux4 = packet_in.aux4;


#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        // cope with extensions
        memset(MAVLINK_MSG_ID_tx_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_tx_MIN_LEN);
    }
#endif
    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_tx_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx_pack(system_id, component_id, &msg, packet1.roll, packet1.pitch, packet1.yaw, packet1.throttle, packet1.aux1, packet1.aux2, packet1.aux3, packet1.aux4 );
    mavlink_msg_tx_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg, packet1.roll, packet1.pitch, packet1.yaw, packet1.throttle, packet1.aux1, packet1.aux2, packet1.aux3, packet1.aux4 );
    mavlink_msg_tx_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_to_send_buffer(buffer, &msg);
    for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++)
    {
        comm_send_ch(MAVLINK_COMM_0, buffer[i]);
    }
    mavlink_msg_tx_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx_send(MAVLINK_COMM_1, packet1.roll, packet1.pitch, packet1.yaw, packet1.throttle, packet1.aux1, packet1.aux2, packet1.aux3, packet1.aux4 );
    mavlink_msg_tx_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_motor_value(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_motor_value >= 256)
    {
        return;
    }
#endif
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;
    mavlink_motor_value_t packet_in =
    {
        963497464,17443,17547,17651,17755
    };
    mavlink_motor_value_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));
    packet1.time_boot_ms = packet_in.time_boot_ms;
    packet1.motor1 = packet_in.motor1;
    packet1.motor2 = packet_in.motor2;
    packet1.motor3 = packet_in.motor3;
    packet1.motor4 = packet_in.motor4;


#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        // cope with extensions
        memset(MAVLINK_MSG_ID_motor_value_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_motor_value_MIN_LEN);
    }
#endif
    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_motor_value_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_motor_value_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_motor_value_pack(system_id, component_id, &msg, packet1.time_boot_ms, packet1.motor1, packet1.motor2, packet1.motor3, packet1.motor4 );
    mavlink_msg_motor_value_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_motor_value_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg, packet1.time_boot_ms, packet1.motor1, packet1.motor2, packet1.motor3, packet1.motor4 );
    mavlink_msg_motor_value_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_to_send_buffer(buffer, &msg);
    for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++)
    {
        comm_send_ch(MAVLINK_COMM_0, buffer[i]);
    }
    mavlink_msg_motor_value_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_motor_value_send(MAVLINK_COMM_1, packet1.time_boot_ms, packet1.motor1, packet1.motor2, packet1.motor3, packet1.motor4 );
    mavlink_msg_motor_value_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_motor_io(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_motor_io >= 256)
    {
        return;
    }
#endif
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;
    mavlink_motor_io_t packet_in =
    {
        5,72,139,206,17
    };
    mavlink_motor_io_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));
    packet1.motor_dir = packet_in.motor_dir;
    packet1.motor_min = packet_in.motor_min;
    packet1.motor_dshot = packet_in.motor_dshot;
    packet1.motor3 = packet_in.motor3;
    packet1.motor4 = packet_in.motor4;


#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        // cope with extensions
        memset(MAVLINK_MSG_ID_motor_io_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_motor_io_MIN_LEN);
    }
#endif
    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_motor_io_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_motor_io_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_motor_io_pack(system_id, component_id, &msg, packet1.motor_dir, packet1.motor_min, packet1.motor_dshot, packet1.motor3, packet1.motor4 );
    mavlink_msg_motor_io_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_motor_io_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg, packet1.motor_dir, packet1.motor_min, packet1.motor_dshot, packet1.motor3, packet1.motor4 );
    mavlink_msg_motor_io_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_to_send_buffer(buffer, &msg);
    for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++)
    {
        comm_send_ch(MAVLINK_COMM_0, buffer[i]);
    }
    mavlink_msg_motor_io_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_motor_io_send(MAVLINK_COMM_1, packet1.motor_dir, packet1.motor_min, packet1.motor_dshot, packet1.motor3, packet1.motor4 );
    mavlink_msg_motor_io_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_attitude(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_attitude >= 256)
    {
        return;
    }
#endif
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;
    mavlink_attitude_t packet_in =
    {
        963497464,45.0,73.0,101.0,129.0,157.0,185.0
    };
    mavlink_attitude_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));
    packet1.time_boot_ms = packet_in.time_boot_ms;
    packet1.accroll = packet_in.accroll;
    packet1.accpitch = packet_in.accpitch;
    packet1.accyaw = packet_in.accyaw;
    packet1.gyroroll = packet_in.gyroroll;
    packet1.gyropitch = packet_in.gyropitch;
    packet1.gyroyaw = packet_in.gyroyaw;


#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        // cope with extensions
        memset(MAVLINK_MSG_ID_attitude_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_attitude_MIN_LEN);
    }
#endif
    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_attitude_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_attitude_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_attitude_pack(system_id, component_id, &msg, packet1.time_boot_ms, packet1.accroll, packet1.accpitch, packet1.accyaw, packet1.gyroroll, packet1.gyropitch, packet1.gyroyaw );
    mavlink_msg_attitude_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_attitude_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg, packet1.time_boot_ms, packet1.accroll, packet1.accpitch, packet1.accyaw, packet1.gyroroll, packet1.gyropitch, packet1.gyroyaw );
    mavlink_msg_attitude_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_to_send_buffer(buffer, &msg);
    for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++)
    {
        comm_send_ch(MAVLINK_COMM_0, buffer[i]);
    }
    mavlink_msg_attitude_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_attitude_send(MAVLINK_COMM_1, packet1.time_boot_ms, packet1.accroll, packet1.accpitch, packet1.accyaw, packet1.gyroroll, packet1.gyropitch, packet1.gyroyaw );
    mavlink_msg_attitude_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_pid(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_pid >= 256)
    {
        return;
    }
#endif
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;
    mavlink_pid_t packet_in =
    {
        17.0,45.0,73.0,101.0,129.0,157.0,185.0,213.0,241.0
    };
    mavlink_pid_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));
    packet1.roll_kp = packet_in.roll_kp;
    packet1.roll_ki = packet_in.roll_ki;
    packet1.roll_kd = packet_in.roll_kd;
    packet1.pitch_kp = packet_in.pitch_kp;
    packet1.pitch_ki = packet_in.pitch_ki;
    packet1.pitch_kd = packet_in.pitch_kd;
    packet1.yaw_kp = packet_in.yaw_kp;
    packet1.yaw_ki = packet_in.yaw_ki;
    packet1.yaw_kd = packet_in.yaw_kd;


#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        // cope with extensions
        memset(MAVLINK_MSG_ID_pid_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_pid_MIN_LEN);
    }
#endif
    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_pid_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_pid_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_pid_pack(system_id, component_id, &msg, packet1.roll_kp, packet1.roll_ki, packet1.roll_kd, packet1.pitch_kp, packet1.pitch_ki, packet1.pitch_kd, packet1.yaw_kp, packet1.yaw_ki, packet1.yaw_kd );
    mavlink_msg_pid_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_pid_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg, packet1.roll_kp, packet1.roll_ki, packet1.roll_kd, packet1.pitch_kp, packet1.pitch_ki, packet1.pitch_kd, packet1.yaw_kp, packet1.yaw_ki, packet1.yaw_kd );
    mavlink_msg_pid_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_to_send_buffer(buffer, &msg);
    for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++)
    {
        comm_send_ch(MAVLINK_COMM_0, buffer[i]);
    }
    mavlink_msg_pid_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_pid_send(MAVLINK_COMM_1, packet1.roll_kp, packet1.roll_ki, packet1.roll_kd, packet1.pitch_kp, packet1.pitch_ki, packet1.pitch_kd, packet1.yaw_kp, packet1.yaw_ki, packet1.yaw_kd );
    mavlink_msg_pid_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_rx(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_rx >= 256)
    {
        return;
    }
#endif
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;
    mavlink_rx_t packet_in =
    {
        5
    };
    mavlink_rx_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));
    packet1.rx_select = packet_in.rx_select;


#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        // cope with extensions
        memset(MAVLINK_MSG_ID_rx_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_rx_MIN_LEN);
    }
#endif
    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rx_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_rx_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rx_pack(system_id, component_id, &msg, packet1.rx_select );
    mavlink_msg_rx_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rx_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg, packet1.rx_select );
    mavlink_msg_rx_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_to_send_buffer(buffer, &msg);
    for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++)
    {
        comm_send_ch(MAVLINK_COMM_0, buffer[i]);
    }
    mavlink_msg_rx_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rx_send(MAVLINK_COMM_1, packet1.rx_select );
    mavlink_msg_rx_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_rates(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_rates >= 256)
    {
        return;
    }
#endif
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;
    mavlink_rates_t packet_in =
    {
        17.0,45.0,73.0,101.0,129.0,157.0,185.0,213.0,241.0
    };
    mavlink_rates_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));
    packet1.roll_rate = packet_in.roll_rate;
    packet1.roll_rc_rate = packet_in.roll_rc_rate;
    packet1.roll_rc_curve = packet_in.roll_rc_curve;
    packet1.pitch_rate = packet_in.pitch_rate;
    packet1.pitch_rc_rate = packet_in.pitch_rc_rate;
    packet1.pitch_rc_curve = packet_in.pitch_rc_curve;
    packet1.yaw_rate = packet_in.yaw_rate;
    packet1.yaw_rc_rate = packet_in.yaw_rc_rate;
    packet1.yaw_rc_curve = packet_in.yaw_rc_curve;


#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        // cope with extensions
        memset(MAVLINK_MSG_ID_rates_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_rates_MIN_LEN);
    }
#endif
    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rates_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_rates_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rates_pack(system_id, component_id, &msg, packet1.roll_rate, packet1.roll_rc_rate, packet1.roll_rc_curve, packet1.pitch_rate, packet1.pitch_rc_rate, packet1.pitch_rc_curve, packet1.yaw_rate, packet1.yaw_rc_rate, packet1.yaw_rc_curve );
    mavlink_msg_rates_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rates_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg, packet1.roll_rate, packet1.roll_rc_rate, packet1.roll_rc_curve, packet1.pitch_rate, packet1.pitch_rc_rate, packet1.pitch_rc_curve, packet1.yaw_rate, packet1.yaw_rc_rate, packet1.yaw_rc_curve );
    mavlink_msg_rates_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_to_send_buffer(buffer, &msg);
    for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++)
    {
        comm_send_ch(MAVLINK_COMM_0, buffer[i]);
    }
    mavlink_msg_rates_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rates_send(MAVLINK_COMM_1, packet1.roll_rate, packet1.roll_rc_rate, packet1.roll_rc_curve, packet1.pitch_rate, packet1.pitch_rc_rate, packet1.pitch_rc_curve, packet1.yaw_rate, packet1.yaw_rc_rate, packet1.yaw_rc_curve );
    mavlink_msg_rates_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_config_tab(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_config_tab >= 256)
    {
        return;
    }
#endif
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;
    mavlink_config_tab_t packet_in =
    {
        5
    };
    mavlink_config_tab_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));
    packet1.tab_index = packet_in.tab_index;


#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        // cope with extensions
        memset(MAVLINK_MSG_ID_config_tab_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_config_tab_MIN_LEN);
    }
#endif
    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_config_tab_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_config_tab_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_config_tab_pack(system_id, component_id, &msg, packet1.tab_index );
    mavlink_msg_config_tab_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_config_tab_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg, packet1.tab_index );
    mavlink_msg_config_tab_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_to_send_buffer(buffer, &msg);
    for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++)
    {
        comm_send_ch(MAVLINK_COMM_0, buffer[i]);
    }
    mavlink_msg_config_tab_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_config_tab_send(MAVLINK_COMM_1, packet1.tab_index );
    mavlink_msg_config_tab_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_reset(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_reset >= 256)
    {
        return;
    }
#endif
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;
    mavlink_reset_t packet_in =
    {
        5
    };
    mavlink_reset_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));
    packet1.reset = packet_in.reset;


#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        // cope with extensions
        memset(MAVLINK_MSG_ID_reset_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_reset_MIN_LEN);
    }
#endif
    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_reset_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_reset_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_reset_pack(system_id, component_id, &msg, packet1.reset );
    mavlink_msg_reset_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_reset_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg, packet1.reset );
    mavlink_msg_reset_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_to_send_buffer(buffer, &msg);
    for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++)
    {
        comm_send_ch(MAVLINK_COMM_0, buffer[i]);
    }
    mavlink_msg_reset_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_reset_send(MAVLINK_COMM_1, packet1.reset );
    mavlink_msg_reset_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_save(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_save >= 256)
    {
        return;
    }
#endif
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;
    mavlink_save_t packet_in =
    {
        5
    };
    mavlink_save_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));
    packet1.save = packet_in.save;


#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        // cope with extensions
        memset(MAVLINK_MSG_ID_save_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_save_MIN_LEN);
    }
#endif
    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_save_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_save_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_save_pack(system_id, component_id, &msg, packet1.save );
    mavlink_msg_save_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_save_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg, packet1.save );
    mavlink_msg_save_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_to_send_buffer(buffer, &msg);
    for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++)
    {
        comm_send_ch(MAVLINK_COMM_0, buffer[i]);
    }
    mavlink_msg_save_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_save_send(MAVLINK_COMM_1, packet1.save );
    mavlink_msg_save_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_modes(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_modes >= 256)
    {
        return;
    }
#endif
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;
    mavlink_modes_t packet_in =
    {
        5,72,139,206,17,84,151,218,29,96,163,230
    };
    mavlink_modes_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));
    packet1.acron = packet_in.acron;
    packet1.acros = packet_in.acros;
    packet1.leveln = packet_in.leveln;
    packet1.levels = packet_in.levels;
    packet1.horn = packet_in.horn;
    packet1.hors = packet_in.hors;
    packet1.racean = packet_in.racean;
    packet1.raceas = packet_in.raceas;
    packet1.racehn = packet_in.racehn;
    packet1.racehs = packet_in.racehs;
    packet1.turtlen = packet_in.turtlen;
    packet1.turtles = packet_in.turtles;


#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        // cope with extensions
        memset(MAVLINK_MSG_ID_modes_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_modes_MIN_LEN);
    }
#endif
    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_modes_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_modes_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_modes_pack(system_id, component_id, &msg, packet1.acron, packet1.acros, packet1.leveln, packet1.levels, packet1.horn, packet1.hors, packet1.racean, packet1.raceas, packet1.racehn, packet1.racehs, packet1.turtlen, packet1.turtles );
    mavlink_msg_modes_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_modes_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg, packet1.acron, packet1.acros, packet1.leveln, packet1.levels, packet1.horn, packet1.hors, packet1.racean, packet1.raceas, packet1.racehn, packet1.racehs, packet1.turtlen, packet1.turtles );
    mavlink_msg_modes_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_to_send_buffer(buffer, &msg);
    for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++)
    {
        comm_send_ch(MAVLINK_COMM_0, buffer[i]);
    }
    mavlink_msg_modes_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_modes_send(MAVLINK_COMM_1, packet1.acron, packet1.acros, packet1.leveln, packet1.levels, packet1.horn, packet1.hors, packet1.racean, packet1.raceas, packet1.racehn, packet1.racehs, packet1.turtlen, packet1.turtles );
    mavlink_msg_modes_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_other(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_other >= 256)
    {
        return;
    }
#endif
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;
    mavlink_other_t packet_in =
    {
        17.0,17
    };
    mavlink_other_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));
    packet1.low_vol = packet_in.low_vol;
    packet1.max_angle = packet_in.max_angle;


#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        // cope with extensions
        memset(MAVLINK_MSG_ID_other_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_other_MIN_LEN);
    }
#endif
    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_other_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_other_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_other_pack(system_id, component_id, &msg, packet1.max_angle, packet1.low_vol );
    mavlink_msg_other_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_other_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg, packet1.max_angle, packet1.low_vol );
    mavlink_msg_other_decode(&msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_to_send_buffer(buffer, &msg);
    for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++)
    {
        comm_send_ch(MAVLINK_COMM_0, buffer[i]);
    }
    mavlink_msg_other_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

    memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_other_send(MAVLINK_COMM_1, packet1.max_angle, packet1.low_vol );
    mavlink_msg_other_decode(last_msg, &packet2);
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_configurator(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_reboot(system_id, component_id, last_msg);
    mavlink_test_calibrate(system_id, component_id, last_msg);
    mavlink_test_tx(system_id, component_id, last_msg);
    mavlink_test_motor_value(system_id, component_id, last_msg);
    mavlink_test_motor_io(system_id, component_id, last_msg);
    mavlink_test_attitude(system_id, component_id, last_msg);
    mavlink_test_pid(system_id, component_id, last_msg);
    mavlink_test_rx(system_id, component_id, last_msg);
    mavlink_test_rates(system_id, component_id, last_msg);
    mavlink_test_config_tab(system_id, component_id, last_msg);
    mavlink_test_reset(system_id, component_id, last_msg);
    mavlink_test_save(system_id, component_id, last_msg);
    mavlink_test_modes(system_id, component_id, last_msg);
    mavlink_test_other(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // CONFIGURATOR_TESTSUITE_H
