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


msg_t *alloc_msg(char,char);
void free_msg(msg_t*);
void msg_reset(msg_t*,char,char);
void msg_addS8(msg_t*,signed char);
void msg_addU8(msg_t*,unsigned char);
void msg_addS16(msg_t*,signed short int);
void msg_addU16(msg_t*,unsigned short int);
void msg_addS32(msg_t*,signed int);
void msg_addU32(msg_t*,unsigned int);
signed short int msg_bytes2S16(char*);
unsigned short int msg_bytes2U16(char*);
signed int msg_bytes2S32(char*);
unsigned int msg_bytes2U32(char*);


#endif /* MSG_H */
