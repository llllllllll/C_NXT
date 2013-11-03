// Joe Jevnik
// 2.11.2013
// implementation of the msg_t.

#include <stdlib.h>
#include <string.h>

#include "msg.h"

typedef unsigned char u_char;

// allocates and initializes a new msg_t.
msg_t *alloc_msg(char type, char opcode){
    msg_t *tmp = malloc(sizeof(msg_t));
    tmp->txt = malloc(256 * sizeof(u_char));
    tmp->txt[0] = type;
    tmp->txt[1] = opcode;
    tmp->len = 2;
    tmp->cap = 256;
    return tmp;
}

void free_msg(msg_t *msg){
    free(msg->txt);
    free(msg);
}

// equivelent to { free(msg); msg = alloc(type,opcode); }.
void msg_reset(msg_t *msg,char type, char opcode){
    free(msg->txt);
    msg->txt = malloc(256 * sizeof(u_char));
    msg->txt[0] = type;
    msg->txt[1] = opcode;
    msg->len = 2;
    msg->cap = 256;
}

// adds a signed char to the end of msg.
void msg_addS8(msg_t *msg,signed char a){
    msg->len += 1;
    if (msg->len == msg->cap){
	u_char *tmp = malloc((msg->cap + 256) * sizeof(u_char));
	for (int n = 0;n < msg->len - 1;n++){
	    tmp[n] = msg->txt[n];
	}
	free(msg->txt);
	msg->txt = tmp;
    }
    msg->txt[msg->len - 1] =  a;
}

// add an u_char to the msg.
void msg_addU8(msg_t *msg,u_char a){
    msg->len += 1;
    if (msg->len == msg->cap){
	u_char *tmp = malloc((msg->cap + 256) * sizeof(u_char));
	for (int n = 0;n < msg->len - 1;n++){
	    tmp[n] = msg->txt[n];
	}
	free(msg->txt);
	msg->txt = tmp;
    }
    msg->txt[msg->len - 1] = a;
}

// add a signed short to the msg.
void msg_addS16(msg_t *msg,signed short int a){
    msg->len += 2;
        if (msg->len >= msg->cap){
	u_char *tmp = malloc((msg->cap + 256) * sizeof(u_char));
	for (int n = 0;n < msg->len - 1;n++){
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
void msg_addU16(msg_t *msg,unsigned short int a){
    msg->len += 2;
    if (msg->len >= msg->cap){
	u_char *tmp = malloc((msg->cap + 256) * sizeof(u_char));
	for (int n = 0;n < msg->len - 1;n++){
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
void msg_addS32(msg_t *msg,signed int a){
    msg->len += 4;
    if (msg->len >= msg->cap){
	u_char *tmp = malloc((msg->cap + 256) * sizeof(u_char));
	for (int n = 0;n < msg->len - 1;n++){
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
void msg_addU32(msg_t *msg,unsigned int a){
    msg->len += 4;
    if (msg->len >= msg->cap){
	u_char *tmp = malloc((msg->cap + 256) * sizeof(u_char));
	for (int n = 0;n < msg->len - 1;n++){
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

// converts a message to a signed short int.
signed short int msg_bytes2S16(char *str){
    return (signed short int) ((0xff & str[0]) 
			       | ((0xff & str[1]) << 8));
}

// converts a message to an unsigned short int.
unsigned short int msg_bytes2U16(char *str){;
    return (unsigned short int) ((0xff & str[0]) 
				 | ((0xff & str[1]) << 8));
}

// converts a message to a signed int.
signed int msg_bytes2S32(char *str){
    return (signed int) ((0xff & str[0])
			 | ((0xff & str[1]) << 8)
			 | ((0xff & str[2]) << 16)
			 | ((0xff & str[3]) << 24));
}

// converts a message an unsigned int.
unsigned int msg_bytes2U32(char *str){
    return (unsigned int) ((0xff & str[0])
			   | ((0xff & str[1]) << 8)
			   | ((0xff & str[2]) << 16)
			   | ((0xff & str[3]) << 24));
}
