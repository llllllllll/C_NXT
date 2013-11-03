// Joe Jevnik
// 2.11.2013
// message_t header.

#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct {
    unsigned char *msg;
} msg_t;

/*
Message(char type, char opcode);
void reset(char type, char opcode);
void addS8(signed char a);
void addU8(unsigned char a);
void addS16(signed short int a);
void addU16(unsigned short int a);
void addS32(signed int a);
void addU32(unsigned int a);
char *getMsg();
unsigned int getLen();
static signed short int bytes2S16(char *buf);
static unsigned short int bytes2U16(char *buf);
static signed int bytes2S32(char *buf);
static unsigned int bytes2U32(char *buf);
*/

#endif /* __MESSAGE_H__ */
