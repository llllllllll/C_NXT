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
typedef enum {
    RAWMODE           = 0x00,
    BOOLEANMODE       = 0x20,
    TRANSITIONCNTMODE = 0x40,
    PERIODCOUNTERMODE = 0x60,
    PCTFULLSCALEMODE  = 0x80,
    CELSIUSMODE       = 0xa0,
    FAHRENHEITMODE    = 0xc0,
    ANGLESTEPMODE     = 0xe0,
    SLOPEMASK         = 0x1f,
    MODEMASK          = 0xe0
} sensor_mode;

// The state of a motor.
typedef struct{
    unsigned char port;
    char power;
    unsigned char mode;
    unsigned char reg_mode;
    char turn_ratio;
    unsigned char run_state;
    unsigned int tacho_limit;
    int tacho_count;
    int block_tacho_count;
    int rotation_count;
} motorstate_t;

// The state of a sensor.
typedef struct {
    unsigned char port;
    int valid;
    int calibrated;
    sensor_type type;
    sensor_mode mode;
    unsigned short int raw_value;
    unsigned short int normalized_value;
    short int scaled_value;
    short int calibrated_value;
} sensorstate_t;

NXT *alloc_NXT();
void free_NXT();
int NXT_connect(NXT*,const char*);
void NXT_disconnect(NXT*);
int NXT_send_buffer(NXT*,char*,unsigned short int);
int NXT_send_msg(NXT*,msg_t*);
int NXT_recv_buffer(NXT*,char*,unsigned short int);
void NXT_play_tone(NXT*,
		   unsigned short int freq,
		   unsigned short int time,
		   int ans,
		   unsigned char *status);
unsigned short int NXT_battery_level(NXT*);
void NXT_set_motorstate(NXT*,motorstate_t*,int ans,unsigned char *status);
motorstate_t *NXT_get_motorstate(NXT*,unsigned char port);
void NXT_set_input_mode(NXT*,
			unsigned char port,
			sensor_type type,
			sensor_mode mode,
			int ans,
			unsigned char *status);
sensorstate_t *NXT_get_sensorstate(NXT*,unsigned char port);
void NXT_reset_motor_position(NXT*,
			      unsigned char port,
			      int relative,
			      int ans,
			      unsigned char *status);

#endif /* NXT_H */
