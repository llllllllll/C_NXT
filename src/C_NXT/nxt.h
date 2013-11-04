// Joe Jevnik
// 2.11.2013
// NXT header.
#ifndef NXT_H
#define NXT_H

#include "msg.h"

// A representation of an NXT.
typedef struct{
    int dev_id;
    int local_sock;
    int remote_sock;
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
    motor_port     port;
    char           power;              // Range [-100,100]
    motor_mode     mode;
    motor_reg_mode reg_mode;
    char           turn_ratio;
    motor_reg_mode run_state;
    unsigned int   tacho_limit;
    int            tacho_count;        // 0 = run forevor.
    int            block_tacho_count;
    int            rotation_count;
} motorstate_t;

// The state of a sensor.
typedef struct{
    sensor_port        port;
    int                valid;
    int                calibrated;
    sensor_type        type;
    sensor_mode        mode;
    unsigned short int raw_value;
    unsigned short int normalized_value;
    short int          scaled_value;
    short int          calibrated_value;
} sensorstate_t;

// Allocates and initializes an NXT.
// On success returns a pointer to the NXT.
// On failure returns NULL.
NXT *alloc_NXT();

// Closes the connections and then frees the NXT.
void free_NXT(NXT*);

// Connects to the given MAC address.
// On success returns 0.
// On failure returns -1.
int NXT_connect(NXT*,const char*);

// Closes the connection of the given NXT.
void NXT_disconnect(NXT*);

// Sends a raw buffer to the NXT of a given length.
// On success returns 0.
// On failure returns -1.
int NXT_send_buffer(NXT*,char*,unsigned short int);

// Sends a msg_t to the NXT.
// On success returns 0.
// On failure returns -1.
int NXT_send_msg(NXT*,msg_t*);

// Receives a buffer from the NXT of a given length.
// On success returns 0.
// If the buffer is too small, returns -1.
// If the read fails, returns -2.
int NXT_recv_buffer(NXT*,char*,unsigned short int);

// Plays a tone of a given frequency for a certain amount of time on the NXT.
// If ans, status will be set to the response of the NXT
// Otherwise, status can be NULL.
void NXT_play_tone(NXT*,
		   unsigned short int freq,
		   unsigned short int time,
		   int ans,
		   unsigned char *status);

// Returns the battery level in mV of the NXT.
unsigned short int NXT_battery_level(NXT*);

// Sets a motorstate on the NXT. See motorstate_t for information on
// constructiong a proper state to send.
// If ans, status will be set to the response of the NXT
// Otherwise, status can be NULL.
void NXT_set_motorstate(NXT*,motorstate_t*,int ans,unsigned char *status);

// Returns the motorstate of a given port (MOTOR_A,MOTOR_B,MOTOR_C,or
// MOTOR_ALL).
// Mallocs the motorstate_t, be sure to free it elsewhere.
motorstate_t *NXT_get_motorstate(NXT*,motor_port port);

// Sets the input mode of a given port to the type and mode specified.
// If ans, status will be set to the response of the NXT
// Otherwise, status can be NULL.
void NXT_set_input_mode(NXT*,sensor_port,sensor_type type,sensor_mode mode,
			int ans,unsigned char *status);

// Returns a pointer to the sensorstate_t of a given sensor port (SENSOR_1,
// SENSOR_2,SENSOR_3,SENSOR_4).
// Mallocs the sensorstate_t, be sure to free it elsewhere.
sensorstate_t *NXT_get_sensorstate(NXT*,sensor_port);

// Resets the position of the motor on port (MOTOR_A,MOTOR_B,MOTOR_C,or
// MOTOR_ALL).
// If relative, resets it to the last position before motion, else resets it's
// absolute position.
// If ans, status will be set to the response of the NXT
// Otherwise, status can be NULL.
void NXT_reset_motor_position(NXT*,motor_port,int relative,int ans,
			      unsigned char *status);

// Sends a STAY_ALIVE message to the NXT. Normal commands do not tell the NXT
// Not to turn off, and thus you should be sure to send one of these every so
// often.
void NXT_stay_alive(NXT*);

#endif /* NXT_H */
