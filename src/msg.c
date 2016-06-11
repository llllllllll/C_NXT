/* msg.c --- Implementation of the message type section of the C_NXT library.
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

#include "msg.h"

// allocates and initializes a new nxt_msg.
nxt_msg *alloc_msg(char type, char opcode){
    nxt_msg *tmp = malloc(sizeof(nxt_msg));
    tmp->txt     = malloc(64 * sizeof(u_char));
    tmp->txt[0]  = type;
    tmp->txt[1]  = opcode;
    tmp->len     = 2;
    tmp->cap     = 256;
    return tmp;
}

void free_msg(nxt_msg *msg){
    free(msg->txt);
    free(msg);
}

// equivelent to { free(msg); msg = alloc(type,opcode); }.
void msg_reset(nxt_msg *msg,char type, char opcode){
    free(msg->txt);
    msg->txt    = malloc(64 * sizeof(u_char));
    msg->txt[0] = type;
    msg->txt[1] = opcode;
    msg->len    = 2;
    msg->cap    = 256;
}

// adds a signed char to the end of msg.
void msg_addS8(nxt_msg *msg,char a){
    u_char *tmp;
    size_t  n;
    msg->len += 1;
    if (msg->len == msg->cap){
        tmp = malloc((msg->cap + 256) * sizeof(u_char));
	for (n = 0;n < msg->len - 1;n++){
	    tmp[n] = msg->txt[n];
	}
	free(msg->txt);
	msg->txt = tmp;
    }
    msg->txt[msg->len - 1] = a;
}

// add an u_char to the msg.
void msg_addU8(nxt_msg *msg,u_char a){
    u_char *tmp;
    size_t  n;
    msg->len += 1;
    if (msg->len == msg->cap){
        tmp = malloc((msg->cap + 256) * sizeof(u_char));
	for (n = 0;n < msg->len - 1;n++){
	    tmp[n] = msg->txt[n];
	}
	free(msg->txt);
	msg->txt = tmp;
    }
    msg->txt[msg->len - 1] = a;
}

// add a signed short to the msg.
void msg_addS16(nxt_msg *msg,signed short int a){
    u_char *tmp;
    size_t  n;
    msg->len += 2;
        if (msg->len >= msg->cap){
	tmp = malloc((msg->cap + 256) * sizeof(u_char));
	for (n = 0;n < msg->len - 1;n++){
	    tmp[n] = msg->txt[n];
	}
	free(msg->txt);
	msg->txt = tmp;
    }
#ifdef _NXT_LITTLE_ENDIAN
    msg->txt[msg->len - 2] = a & 0xff;
    msg->txt[msg->len - 1] =  (a >> 8) & 0xff;
#elif _NXT_BIG_ENDIAN
    msg->txt[msg->len - 2] = (a >> 8) & 0xff;
    msg->txt[msg->len - 1] = a & 0xff;
#endif
}

// add an unsinged short the msg.
void msg_addU16(nxt_msg *msg,unsigned short int a){
    u_char *tmp;
    size_t  n;
    msg->len += 2;
    if (msg->len >= msg->cap){
	tmp = malloc((msg->cap + 256) * sizeof(u_char));
	for (n = 0;n < msg->len - 1;n++){
	    tmp[n] = msg->txt[n];
	}
	free(msg->txt);
	msg->txt = tmp;
    }
#ifdef _NXT_LITTLE_ENDIAN
    msg->txt[msg->len - 2] = (u_char) (a & 0xff);
    msg->txt[msg->len - 1] = (u_char) ((a>>8) &0xff);
#elif _NXT_BIG_ENDIAN
    msg->txt[msg->len - 2] = (u_char) ((a >> 8) & 0xff);
    msg->txt[msg->len - 1] = (u_char) (a & 0xff);
#endif
}


// add a signed int to the msg.
void msg_addS32(nxt_msg *msg,signed int a){
    u_char *tmp;
    size_t  n;
    msg->len += 4;
    if (msg->len >= msg->cap){
	tmp = malloc((msg->cap + 256) * sizeof(u_char));
	for (n = 0;n < msg->len - 1;n++){
	    tmp[n] = msg->txt[n];
	}
	free(msg->txt);
	msg->txt = tmp;
    }
#ifdef _NXT_LITTLE_ENDIAN
    msg->txt[msg->len - 4] = a & 0xff;
    msg->txt[msg->len - 3] = (a >> 8) & 0xff;
    msg->txt[msg->len - 2] = (a >> 16) & 0xff;
    msg->txt[msg->len - 1] = (a >> 24) & 0xff;
#elif _NXT_BIG_ENDIAN
    msg->txt[msg->len - 4] = (a >> 24) & 0xff;
    msg->txt[msg->len - 3] = (a >> 16) & 0xff;
    msg->txt[msg->len - 2] = (a >> 8) & 0xff;
    msg->txt[msg->len - 1] = a & 0xff;
#endif
}

// add an unsigned int to the msg.
void msg_addU32(nxt_msg *msg,unsigned int a){
    u_char *tmp;
    size_t  n;
    msg->len += 4;
    if (msg->len >= msg->cap){
	tmp = malloc((msg->cap + 256) * sizeof(u_char));
	for (n = 0;n < msg->len - 1;n++){
	    tmp[n] = msg->txt[n];
	}
	free(msg->txt);
	msg->txt = tmp;
    }
#ifdef _NXT_LITTLE_ENDIAN
    msg->txt[msg->len - 4] = a & 0xff;
    msg->txt[msg->len - 3] = (a >> 8) & 0xff;
    msg->txt[msg->len - 2] = (a >> 16) & 0xff;
    msg->txt[msg->len - 1] = (a >> 24) & 0xff;
#elif _NXT_BIG_ENDIAN
    msg->txt[msg->len - 4] = (a >> 24) & 0xff;
    msg->txt[msg->len - 3] = (a >> 16) & 0xff;
    msg->txt[msg->len - 2] = (a >> 8) & 0xff;
    msg->txt[msg->len - 1] = a & 0xff;
#endif
}

// return: the message as a signed short.
short msg_bytes2S16(char *str){
    return (short) ((0xff & str[0]) | ((0xff & str[1]) << 8));
}

// return: the message as an unsigned int.
unsigned short msg_bytes2U16(char *str){;
    return (unsigned short) ((0xff & str[0]) | ((0xff & str[1]) << 8));
}

// return: the message as a signed int.
int msg_bytes2S32(char *str){
    return (int) ((0xff & str[0])
                  | ((0xff & str[1]) << 8)
                  | ((0xff & str[2]) << 16)
                  | ((0xff & str[3]) << 24));
}

// return: the message as an unsigned int.
unsigned int msg_bytes2U32(char *str){
    return (unsigned int) ((0xff & str[0])
			   | ((0xff & str[1]) << 8)
			   | ((0xff & str[2]) << 16)
			   | ((0xff & str[3]) << 24));
}
