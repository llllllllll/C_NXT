// Joe Jevnik
// 2.11.2013
// implementation of the msg_t.

#include <stdlib.h>
#include <string.h>

#include "msg.h"

// allocates and initializes a new msg_t.
msg_t alloc_msg_t(char type, char opcode){
    msg_t tmp = malloc(2 * sizeof(unsigned char));
    if (!tmp){
	return NULL;
    }
    tmp[0] = type;
    tmp[1] = opcode;
    return tmp;
}

// equivelent to { free(msg); msg = alloc(type,opcode); }.
void msg_reset(msg_t msg,char type, char opcode){
    free(msg);
    msg = malloc(2 * sizeof(unsigned char));
    msg[0] = type;
    msg[1] = opcode;
}

// adds a signed char to the end of msg.
void msg_addS8(msg_t msg,signed char a){
    msg_t tmp = realloc(msg,sizeof(*msg) + sizeof(unsigned char));
    if (!tmp){
	return;
    }
    msg = tmp;
    msg[msg_len(msg) - 1] = (unsigned char) a;
}

// add an unsigned char to the msg.
void msg_addU8(msg_t msg,unsigned char a){
    msg_t tmp = realloc(msg,sizeof(*msg) + sizeof(unsigned char));
    if (!tmp){
	return;
    }
    msg = tmp;
    msg[msg_len(msg) - 1] = a;
}

// add a signed short to the msg.
void msg_addS16(msg_t msg,signed short int a){
    msg_t tmp = realloc(msg,sizeof(*msg) + 2 * sizeof(unsigned char));
    size_t len;
    if (!tmp){
	return;
    }
    msg = tmp;
    len = strlen((char*) msg);
#ifdef _NXT_LITTLE_ENDIAN
    msg[len - 2] = (unsigned char) (a & 0xff);
    msg[len - 1] = (unsigned char) ((a >> 8) & 0xff);
#elif _NXT_BIG_ENDIAN
    msg[len - 2] = (unsigned char) ((a >> 8) & 0xff);
    msg[len - 1] = (unsigned char) (a & 0xff);
#endif
}

// add an unsinged short the msg.
void msg_addU16(msg_t msg,unsigned short int a){
    msg_t tmp = realloc(msg,sizeof(*msg) + 2 * sizeof(unsigned char));
    size_t len;
    if (!tmp){
	return;
    }
    msg = tmp;
    len = msg_len(msg);
#ifdef _NXT_LITTLE_ENDIAN
    msg[len - 2] = (unsigned char) (a & 0xff);
    msg[len - 1] = (unsigned char) ((a>>8) &0xff);
#elif _NXT_BIG_ENDIAN
    msg[len - 2] = (unsigned char) ((a >> 8) & 0xff);
    msg[len - 1] = (unsigned char) (a & 0xff);
#endif
}


// add a signed int to the msg.
void msg_addS32(msg_t msg,signed int a){
    msg_t tmp = realloc(msg,sizeof(*msg) + 4 * sizeof(unsigned char));
    size_t len;
    if (!tmp){
	return;
    }
    msg = tmp;
    len = msg_len(msg);
#ifdef _NXT_LITTLE_ENDIAN
    msg[len - 4] = (unsigned char) (a & 0xff);
    msg[len - 3] = (unsigned char) ((a >> 8) & 0xff);
    msg[len - 2] = (unsigned char) ((a >> 16) & 0xff);
    msg[len - 1] = (unsigned char) ((a >> 24) & 0xff);
#elif _NXT_BIG_ENDIAN
    msg[len - 4] = (unsigned char) ((a >> 24) & 0xff);
    msg[len - 3] = (unsigned char) ((a >> 16) & 0xff);
    msg[len - 2] = (unsigned char) ((a >> 8) & 0xff);
    msg[len - 1] = (unsigned char) (a & 0xff);
#endif
}

// add an unsigned int to the msg.
void msg_addU32(msg_t msg,unsigned int a){
    msg_t tmp = realloc(msg,sizeof(*msg) + 4 * sizeof(unsigned char));
    size_t len;
    if (!tmp){
	return;
    }
    msg = tmp;
    len = msg_len(msg);
#ifdef _NXT_LITTLE_ENDIAN
    msg[len - 4] = (unsigned char) (a & 0xff);
    msg[len - 3] = (unsigned char) ((a >> 8) & 0xff);
    msg[len - 2] = (unsigned char) ((a >> 16) & 0xff);
    msg[len - 1] = (unsigned char) ((a >> 24) & 0xff);
#elif _NXT_BIG_ENDIAN
    msg[len - 4] = (unsigned char) ((a >> 24) & 0xff);
    msg[len - 3] = (unsigned char) ((a >> 16) & 0xff);
    msg[len - 2] = (unsigned char) ((a >> 8) & 0xff);
    msg[len - 1] = (unsigned char) (a & 0xff);
#endif
}

// returns the length of the message.
unsigned int msg_len(msg_t msg){
    return strlen((char*) msg);
}

// converts a message to a signed short int.
signed short int msg_bytes2S16(msg_t msg){
    return (signed short int) ((0xff & msg[0]) | ((0xff & msg[1]) << 8));
}

// converts a message to an unsigned short int.
unsigned short int msg_bytes2U16(msg_t msg){;
    return (unsigned short int) ((0xff & msg[0]) | ((0xff & msg[1]) << 8));
}

// converts a message to a signed int.
signed int msg_bytes2S32(msg_t msg){
    return (signed int) ((0xff & msg[0])
			 | ((0xff & msg[1]) << 8)
			 | ((0xff & msg[2]) << 16)
			 | ((0xff & msg[3]) << 24));
}

// converts a message an unsigned int.
unsigned int msg_bytes2U32(msg_t msg){
    return (unsigned int) ((0xff & msg[0])
			   | ((0xff & msg[1]) << 8)
			   | ((0xff & msg[2]) << 16)
			   | ((0xff & msg[3]) << 24));
}
