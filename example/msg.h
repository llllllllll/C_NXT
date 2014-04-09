/* msg.h --- Interface to the message type section of the C_NXT library.
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

#ifndef MSG_H
#define MSG_H

#include <stdlib.h>
#include <string.h>

// Shorthand notation.
typedef unsigned char u_char;

// The type that represents a message to be passed to the NXT over bluetooth.
// vector type.
typedef struct {
  unsigned char* txt; // The data to send.
  size_t         len; // The length of a message.
  size_t         cap; // The capacity of the message (will increase if needed).
} nxt_msg;

// Allocates a message starting with the type and opcodes.
// Returns a pointer to the malloc'd message.
// Mallocs a nxt_msg, be sure to free_msg() it elsewhere.
nxt_msg *alloc_msg(char,char);

// Safely frees a nxt_msg by freeing it's txt and then the rest.
void free_msg(nxt_msg*);

// msg_reset() == { free_msg(); alloc_msg(); }
void msg_reset(nxt_msg*,char,char);

// Adds a signed 8 bit value to the msg.
void msg_addS8(nxt_msg*,char);

// Adds an unsigned 8 bit value to the msg.
void msg_addU8(nxt_msg*,unsigned char);

// Adds a signed 16 bit value to the msg.
void msg_addS16(nxt_msg*,signed short int);

// Adds an unsigned 16 bit value to the msg.
void msg_addU16(nxt_msg*,unsigned short int);

// Adds a signed 32 bit value to the msg.
void msg_addS32(nxt_msg*,signed int);

// Adds an unsigned 32 bit value to the msg.
void msg_addU32(nxt_msg*,unsigned int);

// return: the message as a signed short.
short msg_bytes2S16(char*);

// return: the message as an unsigned int.
unsigned short msg_bytes2U16(char*);

// return: the message as a signed int.
int msg_bytes2S32(char*);

// return: the message as an unsigned short.
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


#endif
