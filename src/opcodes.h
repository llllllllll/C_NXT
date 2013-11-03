// Joe Jevnik
// 2.11.2013
// opcode header.

#ifndef OPCODE_H
#define OPCODE_H

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

// Sensor types.
enum sensor_t{
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
};

// Sensor modes.
enum sensor_mode{
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
};
//TODO: ANGLESTEPMODE and MODEMASK have the same value. Document is wrong

void play_tone(bluecomm_t*,
	       unsigned short int freq,
	       unsigned short int time,
	       int answer = 0,
	       unsigned char *status = NULL);

unsigned short int battery_level(bluecomm_t*);


/**
 * Set output state of the motors
 *  @param port Output port: 0 -> A, 1 -> B, 2 -> C, 0xFF -> ALL
 *  @param power Power set point(Range: -100 - 100)
 *  @param mode Mode byte(0x01: turn on the specified motor, 0x02:
    Use run/brake instead of run/float in PWM, 0x04: Turns on the regulation)
 *  @param regMode Regulation mode(0x00: disabled, 0x01: Power control enabled,
    0x02: Synchronization enabled(Needs enabled on two output))
 *  @param turnRatio Turn Ratio(-100 - 100)
 *  @param runState (0x00: idle, 0x10: ramp-up, 0x20: running, 0x40:ramp-down)
 *  @param tachoLimit (0, run forever)
 *  @param answer if true, request answer from brick
 *  @param status must be not null and reserved if answer==true
 *
 *
 */
void set_output_state(bluecomm_t*,motorstate_t,int ans,unsigned char *status = NULL);
        /**
         * Get motor states
         *
         * @param port Output port: 0 -> A, 1 -> B, 2 -> C
         */
motorstate_t get_output_state(bluecomm_t*,unsigned char port);

/**
 * Set the type and mode of the sensor plugged to a given port
 *
 * @param port Input port(Range: 0 - 3)
 * @param type Sensor type (enumerated)
 * @param mode Sensor mode (enumerated)
 */
void set_input_mode(bluecomm_t*,
		    unsigned char port,
		    SensorType type,
		    SensorMode mode,
		    int ans = 0,
		    unsigned char *status = NULL);


SensorState get_input_values(bluecomm_t*,unsigned char port);

/**
 * Resets the motor counter
 *
 * @param port Input port(Range: 0 - 2)
 * @param relative True: reset position relative to last movement,
          False: reset absolute position
 */
void reset_motor_position(bluecomm_t*,
			  unsigned char port,
			  int relative,
			  int ans = 0,
			  unsigned char *status = 
#endif /* OPCODE_H */
