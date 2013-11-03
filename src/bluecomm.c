// Joe Jevnik
// 2.11.2013
// implementation of the bluecomm.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>

#include "bluecomm.h"

// Allocates an initializes a bluecomm_t.
bluecomm_t *alloc_bluecomm(){
    bluecomm_t *tmp  = malloc(sizeof(bluecomm_t));
    tmp->dev_id = hci_get_route(NULL);
    tmp->local_sock = hci_open_dev(dev_id);
    if (tmp->dev_id < 0 || tmp->local_sock < 0){
	return NULL;
    }
    tmp->remote_sock = socket(AF_BLUETOOTH,SOCK_STREAM,BTPROTO_RFCOMM);
    return tmp;
}

// Safely frees a bluecomm_t*.
void free_bluecomm(bluecomm_t *nxt){
    close(nxt->local_sock);
    free(nxt);
}

// Connects a bluecomm_t to a given mac address.
// On success returns 0.
// On failure returns -1.
int connect(bluecomm_t *nxt,const char *mac){
    sockaddr_rc addr = {0};
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( mac, &addr.rc_bdaddr );
    return connect(nxt->remote_sock,(sockaddr*)&addr,sizeof(addr));
}

// disconnects the nxt.
void disconnect(bluecomm_t *nxt){
    close(nxt->remote_sock);
}

// sends a buffer to the nxt.
// on sucess, returns 0.
// on failure returns -1.
int send_buffer(bluecomm_t *nxt,const char *b,unsigned short int len){
    char *b2 = malloc(len * sizeof(char)),l2[2];
    int status;
#ifdef _NXT_LITTLE_ENDIAN
    l2[0] = len & 0xff;
    l2[1] = (len >> 8) & 0xff;
#elif _NXT_BIG_ENDIAN
    l2[0] = (len >> 8) & 0xff;
    l2[1] = len & 0xff;
#endif
    memcpy(b2,b,len);
    status = write(nxt->remote_sock,l2,2);
    if (status < 0){
	free(b2);
	return -1;
    }
    status = write(nxt->remote_sock,b2,len);
    if (status < 0){
	free(b2);
	return -1;
    }
    free(b2);
    return 0;
}

// Reads from the nxt and sets the message into b.
// on success returns 0.
// if the buffer is too small, returns -1.
// if the read fails, returns -2.
int recv_buffer(bluecomm_t *nxt,char *b,unsigned short int len){
    char l2[2];
    unsigned short int msg_size;
    int bytes_read;
    read(nxt->remote_sock,l2,2);
    msg_size = l2[0] + 256 * l2[1];
    if (len < msg_size){
	return -1;
    }
    bytes_read = read(nxt->remote_sock,b,len);
    if (bytes_read < 0){
	return -2;
    }
}
