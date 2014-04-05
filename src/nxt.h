/* nxt.h --- Interface to the C_NXT library.
   Copyright (c) 2014 Joe Jevnik

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along with
   this program; if not, write to the Free Software Foundation, Inc., 51
   Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifndef NXT_H
#define NXT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>

#include "msg.h"

// A representation of an NXT.
typedef struct{
    int dev_id;      // Device ID.
    int local_sock;  // Local socket file descriptor.
    int remote_sock; // Remote socket file descriptor.
} NXT;

// The ports for a motor.
typedef enum{
    MOTOR_A   = 0x00,
    MOTOR_B   = 0x01,
    MOTOR_C   = 0x02,
    MOTOR_ALL = 0xFF
} motor_port;

// The ports for a sensor.
typedef enum{
    SENSOR_1 = 0x00,
    SENSOR_2 = 0x01,
    SENSOR_3 = 0x02,
    SENSOR_4 = 0x03
} sensor_port;

// Represents the modes a motor can operate under.
typedef enum{
    TURN_ON    = 0x00, // Turns on specified motor.
    RUN_BRAKE  = 0x02, // Use run/brake and not run/float.
    REGULATION = 0x04  // Enables power regulation.
} motor_mode;

// Represents the regulation modes a motor uses.
typedef enum{
    NO_REG          = 0x00, // No regulation.
    POWER_CONTROL   = 0x01, // Power control enabled.
    SYNCHRONIZATION = 0x02  // Synchronization enabled, use on 2 or more motors.
} motor_reg_mode;

// Motor run states
typedef enum{
    IDLE      = 0x00, // Idle the motor.
    RAMP_UP   = 0x10, // Increase the speed gradually.
    RUNNING   = 0x20, // Constant speed.
    RAMP_DOWN = 0x40  // Decrease speed gradually.
} motor_runstate;

// Sensor types.
typedef enum {
    NO_SENSOR          = 0x00,
    SWITCH             = 0x01,
    TEMPERATURE        = 0x02,
    REFLECTION         = 0x03,
    ANGLE              = 0x04,
    LIGHT_ACTIVE       = 0x05,
    LIGHT_INACTIVE     = 0x06,
    SOUND_DB           = 0x07,
    SOUND_DBA          = 0x08,
    CUSTOM             = 0x09,
    LOWSPEED           = 0x0a,
    LOWSPEED_9V        = 0x0b,
    NO_OF_SENSOR_TYPES = 0x0c
} sensor_type;
//TODO: ANGLESTEPMODE and MODEMASK have the same value. Document is wrong

// Sensor modes.
typedef enum{
    RAW_MODE            = 0x00,
    BOOLEAN_MODE        = 0x20,
    TRANSITION_CNT_MODE = 0x40,
    PERIOD_COUNTER_MODE = 0x60,
    PCT_FULL_SCALE_MODE = 0x80,
    CELSIUS_MODE        = 0xa0,
    FAHRENHEIT_MODE     = 0xc0,
    ANGLE_STEP_MODE     = 0xe0,
    SLOPE_MASK          = 0x1f,
    MODE_MASK           = 0xe0
} sensor_mode;

// The state of a motor.
typedef struct{
    motor_port     port;               // Range [MOTOR_A,MOTOR_ALL]
    char           power;              // Range [-100,100]
    motor_mode     mode;               // The mode that the motor is using.
    motor_reg_mode reg_mode;           // The regulation mode that it is using.
    char           turn_ratio;         // Range [-100,100]
    motor_runstate run_state;          // The runstate that it is using.
    unsigned int   tacho_limit;        // The tacho limit for the motor.
    int            tacho_count;        // 0 = run forevor.
    int            block_tacho_count;
    int            rotation_count;
} motorstate;

// The state of a sensor.
typedef struct{
    sensor_port    port;             // Range [SENSOR_1,SENSOR_4]
    int            valid;            // Range [0,1]
    int            calibrated;       // Range [0,1]
    sensor_type    type;             // The type of sensor.
    sensor_mode    mode;             // The mode the sensor is using.
    unsigned short raw_value;        // The raw data off the sensor.
    unsigned short normalized_value; // The normalized value of the sensor.
    short int      scaled_value;     // The scaled value off the sensor.
    short int      calibrated_value; // The calibrated value off the sensor.
} sensorstate;

// Initializes a previously allocate NXT structure.
// return: 0 on success, non-zero on failure.
int NXT_init(NXT*);

// Safely releases all resources held by this NXT structure.
// return: 0 on success, non-zero on failure.
int NXT_destroy(NXT*);

// Connects an NXT to a given mac address.
// return: 0 on success, non-zero on failure.
int NXT_connect(NXT*,const char*);

// Closes the connection of the given NXT.
// return: 0 on success, non-zero on failure.
int NXT_disconnect(NXT*);

// Sends a raw buffer to the NXT of a given length.
// return: 0 on success, non-zero on failure.
int NXT_send_buffer(NXT*,char*,unsigned short);

// Sends a nxt_msg to the NXT.
// return: 0 on success, non-zero on failure.
int NXT_send_msg(NXT*,nxt_msg*);

// Receives a buffer from the NXT of a given length.
// return: 0 on success, non-zero on failure.
int NXT_recv_buffer(NXT*,char*,unsigned short);

// Plays a tone of a given frequency for a certain amount of time on the NXT.
// If ans, status will be set to the response of the NXT
// Otherwise, status can be NULL.
// return: 0 on success, non-zero on failure.
int NXT_play_tone(NXT*,
                  unsigned short freq,
                  unsigned short time,
                  bool           ans,
                  unsigned char *status);

// Returns the battery level in mV of the NXT.
unsigned short NXT_battery_level(NXT*);

// Sets a motorstate on the NXT. See motorstate for information on
// constructiong a proper state to send.
// If ans, status will be set to the response of the NXT
// Otherwise, status can be NULL.
// return: 0 on success, non-zero on failure.
int NXT_set_motorstate(NXT*,
                       motorstate*,
                       bool,
                       unsigned char*);

// Gets the motorstate of a given port (MOTOR_A,MOTOR_B,MOTOR_C).
// return: 0 on success, non-zero on failure.
int NXT_get_motorstate(NXT*,
                       motor_port,
                       motorstate*);

// Sets the input mode of a given port to the type and mode specified.
// If ans, status will be set to the response of the NXT
// Otherwise, status can be NULL.
// return: 0 on success, non-zero on failure.
int NXT_set_input_mode(NXT*,
                       sensor_port,
                       sensor_type,
                       sensor_mode,
                       bool,
                       unsigned char*);

// Returns a pointer to the sensorstate_t of a given sensor port (SENSOR_1,
// SENSOR_2,SENSOR_3,SENSOR_4).
// return: 0 on success, non-zero on failure.
int NXT_get_sensorstate(NXT*,sensor_port,sensorstate*);

// Resets the position of the motor on port (MOTOR_A,MOTOR_B,MOTOR_C,or
// MOTOR_ALL).
// If relative, resets it to the last position before motion, else resets it's
// absolute position.
// If ans, status will be set to the response of the NXT
// Otherwise, status can be NULL.
// return: 0 on success, non-zero on failure.
int NXT_reset_motor_position(NXT*,
                             motor_port,
                             bool relative,
                             bool ans,
                             unsigned char*);

// Sends a STAY_ALIVE message to the NXT. Normal commands do not tell the NXT
// Not to turn off, and thus you should be sure to send one of these every so
// often.
// return: 0 on success, non-zero on failure.
int NXT_stay_alive(NXT*);

// Drive fowards for s seconds at p power
// If s is negative, drive  foward forever.
void NXT_driveforward(NXT*,time_t,unsigned char,motor_port,motor_port);

// Drive backwards for s seconds at p power.
// If s is negative, drive  foward forever.
void NXT_drivebackward(NXT*,time_t,unsigned char,motor_port,motor_port);

#endif
