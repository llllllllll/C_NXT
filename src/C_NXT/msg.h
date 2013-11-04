// Joe Jevnik
// 2.11.2013
// message_t header.

#ifndef MSG_H
#define MSG_H

#include <stdlib.h>

typedef struct {
  unsigned char* txt;
  size_t len;
  size_t cap;
} msg_t;

// Allocates a message starting with the type and opcodes.
// Returns a pointer to the malloc'd message.
// Mallocs a msg_t, be sure to free_msg() it elsewhere.
msg_t *alloc_msg(char,char);

// Safely frees a msg_t by freeing it's txt and then the rest.
void free_msg(msg_t*);

// msg_reset() == { free_msg(); alloc_msg(); }
void msg_reset(msg_t*,char,char);

// Adds a signed 8 bit value to the msg.
void msg_addS8(msg_t*,signed char);

// Adds an unsigned 8 bit value to the msg.
void msg_addU8(msg_t*,unsigned char);

// Adds a signed 16 bit value to the msg.
void msg_addS16(msg_t*,signed short int);

// Adds an unsigned 16 bit value to the msg.
void msg_addU16(msg_t*,unsigned short int);

// Adds a signed 32 bit value to the msg.
void msg_addS32(msg_t*,signed int);

// Adds an unsigned 32 bit value to the msg.
void msg_addU32(msg_t*,unsigned int);

// Converts a message txt into a signed 16 bit value.
signed short int msg_bytes2S16(char*);

// Converts a message txt into an unsigned 16 bit value.
unsigned short int msg_bytes2U16(char*);

// Converts a message txt into a signed 32 bit value.
signed int msg_bytes2S32(char*);

// Converts a message txt into an unsigned 32 bit value.
unsigned int msg_bytes2U32(char*);


/*
message opcodes:
  0x00: (start_program, _parse_simple),
  0x01: (stop_program, _parse_simple),
  0x02: (play_sound_file, _parse_simple),
  0x03: (play_tone, _parse_simple),
  0x04: (set_output_state, _parse_simple),
  0x05: (set_input_mode, _parse_simple),
  0x06: (get_output_state, _parse_get_output_state),
  0x07: (get_input_values, _parse_get_input_values),
  0x08: (reset_input_scaled_value, _parse_simple),
  0x09: (message_write, _parse_simple),
  0x0A: (reset_motor_position, _parse_simple),
  0x0B: (get_battery_level, _parse_get_battery_level),
  0x0C: (stop_sound_playback, _parse_simple),
  0x0D: (keep_alive, _parse_keep_alive),
  0x0E: (ls_get_status, _parse_ls_get_status),
  0x0F: (ls_write, _parse_simple),
  0x10: (ls_read, _parse_ls_read),
  0x11: (get_current_program_name, _parse_get_current_program_name),
  0x13: (message_read, _parse_message_read),
*/


#endif /* MSG_H */
