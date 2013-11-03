// Joe Jevnik
// 2.11.2013
// bluecomm_t header.
#ifndef BLUECOMM_H
#define BLUECOMM_H

typedef struct{
    int dev_id;
    int local_sock;
    int remote_sock;
} bluecomm_t;

//void find(bluecomm_t*);
void connect(bluecomm_t*,const char*);
//void connect_name(bluecomm_t*,const char* name);
void disconnect(bluecomm_t*);
void send_buffer(bluecomm_t*,const char*,unsigned short int);
//void send_string(const string str);
void recv_buffer(bluecomm_t*,char*,unsigned short int);

#endif /* BLUECOMM_H */
