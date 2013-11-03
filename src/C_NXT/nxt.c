// Joe Jevnik
// 2.11.2013
// implementation of the nxt.

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

#include "nxt.h"
#include "msg.h"

// Allocates an initializes a NXT.
NXT *alloc_NXT(){
    NXT *tmp  = malloc(sizeof(NXT));
    tmp->dev_id = hci_get_route(NULL);
    tmp->local_sock = hci_open_dev(tmp->dev_id);
    if (tmp->dev_id < 0 || tmp->local_sock < 0){
	return NULL;
    }
    tmp->remote_sock = socket(AF_BLUETOOTH,SOCK_STREAM,BTPROTO_RFCOMM);
    return tmp;
}

// Safely frees a NXT*.
void free_NXT(NXT *nxt){
    close(nxt->local_sock);
    free(nxt);
}

// Connects a NXT to a given mac address.
// On success returns 0.
// On failure returns -1.
int NXT_connect(NXT *nxt,const char *mac){
    struct sockaddr_rc addr = {0};
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( mac, &addr.rc_bdaddr );
    return connect(nxt->remote_sock,(struct sockaddr*)&addr,sizeof(addr));
}

// disconnects the nxt.
void NXT_disconnect(NXT *nxt){
    close(nxt->remote_sock);
}

// sends a buffer to the nxt.
// on sucess, returns 0.
// on failure returns -1.
int NXT_send_buffer(NXT *nxt,char *b,unsigned short int len){
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

int NXT_send_msg(NXT *nxt,msg_t *msg){
    char *b = malloc(msg->len * sizeof(char)),l2[2];
    int status;
#ifdef _NXT_LITTLE_ENDIAN
    l2[0] = msg->len & 0xff;
    l2[1] = (msg->len >> 8) & 0xff;
#elif _NXT_BIG_ENDIAN
    l2[0] = (msg->len >> 8) & 0xff;
    l2[1] = msg->len & 0xff;
#endif
    memcpy(b,msg->txt,msg->len);
    status = write(nxt->remote_sock,l2,2);
    if (status < 0){
	free(b);
	return -1;
    }
    status = write(nxt->remote_sock,b,msg->len);
    if (status < 0){
	free(b);
	return -1;
    }
    free(b);
    return 0;
}

// Reads from the nxt and sets the message into b.
// on success returns 0.
// if the buffer is too small, returns -1.
// if the read fails, returns -2.
int NXT_recv_buffer(NXT *nxt,char *b,unsigned short int len){
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

// Plays a tone on the nxt.
void NXT_play_tone(NXT *nxt,unsigned short int freq,unsigned short int time,
		   int ans,unsigned char *status){
    msg_t *msg;
    if(ans){
        msg = alloc_msg(0x0,0x3);
    } else {
        msg = alloc_msg(0x80,0x3);
    }
    msg_addU16(msg,freq);
    msg_addU16(msg,time);
    NXT_send_msg(nxt,msg);
    if(ans){
	char buf[3];
	NXT_recv_buffer(nxt,buf,3);
	if(buf[0] != 0x02 || buf[1] != 0x03){
	    free_msg(msg);
	    return;
	}
	*status = buf[2];
    }
    free_msg(msg);
    usleep(time * 1000);
}

// Returns the remaining mV of charge on the nxt.
unsigned short int NXT_battery_level(NXT* nxt){
    msg_t *msg = alloc_msg(0,0x0b);
    char buf[5];
    NXT_send_msg(nxt,msg);
    NXT_recv_buffer(nxt,buf,5);
    free_msg(msg);
    return (unsigned short int) (buf[3] + 256 * buf[4]);
}

// sets the output state on the nxt.
void NXT_set_motorstate(NXT *nxt,motorstate_t *st,int ans,
			unsigned char *status){
    msg_t *msg;
    if(ans){
        msg = alloc_msg(0x0,0x4);
    } else {
        msg = alloc_msg(0x80,0x4);
    }
    msg_addU8(msg,st->port);
    msg_addS8(msg,st->power);
    msg_addU8(msg,st->mode);
    msg_addU8(msg,st->reg_mode);
    msg_addS8(msg,st->turn_ratio);
    msg_addU8(msg,st->run_state);
    msg_addU32(msg,st->tacho_limit);
    NXT_send_msg(nxt,msg);
    free_msg(msg);
    if(ans){
	char buf[3];
	NXT_recv_buffer(nxt,buf,3);
	if(buf[0] != 0x02 || buf[1] != 0x04){
	    return;
	}
	*status=buf[2];
    }
}

// returns the motorstate_t of the nxt on the given port.
motorstate_t *NXT_get_motorstate(NXT *nxt,unsigned char port){
    msg_t *msg = alloc_msg(0x0,0x6);
    char buf[25];
    motorstate_t *st = malloc(sizeof(motorstate_t));
    msg_addU8(msg,port);
    NXT_send_msg(nxt,msg);
    NXT_recv_buffer(nxt,buf,25);
    free_msg(msg);
    if(buf[0]!= 0x02 || buf[1] != 0x06){
        return NULL;
    }
    if(buf[2] != 0){
        return NULL;
    }
    st->port = buf[3];
    st->power = buf[4];
    st->mode = buf[5];
    st->reg_mode = buf[6];
    st->turn_ratio = buf[7];
    st->run_state = buf[8];
    st->tacho_limit = msg_bytes2U32(&(buf[9]));
    st->tacho_count = msg_bytes2S32(&(buf[13]));
    st->block_tacho_count = msg_bytes2S32(&(buf[17]));
    st->rotation_count = msg_bytes2S32(&(buf[21]));
    return st;
}

// sets the input mode of the nxt.
void NXT_set_input_mode(NXT *nxt,unsigned char port,sensor_type type,
			sensor_mode mode,int ans,unsigned char *status){
    msg_t *msg;
    if(ans){
	msg = alloc_msg(0x0,0x5);
    } else {
	msg = alloc_msg(0x80,0x5);
    }
    msg_addU8(msg,port);
    msg_addU8(msg,type);
    msg_addU8(msg,mode);
    NXT_send_msg(nxt,msg);

    if(ans){
	char buf[3];
	NXT_recv_buffer(nxt,buf,3);
	if(buf[0] != 0x02 || buf[1] != 0x05){
	    free_msg(msg);
	    return;
	}
	*status = buf[2];
    }
    free_msg(msg);
}

// returns the sensorstate_t of a given port.
sensorstate_t *NXT_get_sensorstate(NXT *nxt,unsigned char port){
    msg_t *msg = alloc_msg(0x0,0x7);
    char buf[16];
    sensorstate_t *st = malloc(sizeof(sensorstate_t));
    msg_addU8(msg,port);
    NXT_send_msg(nxt,msg);
    NXT_recv_buffer(nxt,buf,16);
    free(msg);
    if(buf[0] != 0x02 || buf[1] != 0x07){
        return NULL;
    }
    if(buf[2] != 0){
        return NULL;
    }
    st->port = buf[3];
    st->valid = (buf[4] > 0) ? 1 : 0;
    st->calibrated = (buf[5] > 0) ? 1 : 0;
    st->type = (sensor_type) ((int) buf[6]);
    st->mode = (sensor_mode) ((int) buf[7]);
    st->raw_value = msg_bytes2U16(&(buf[8]));
    st->normalized_value = msg_bytes2U16(&(buf[10]));
    st->scaled_value = msg_bytes2S16(&(buf[12]));
    st->calibrated_value = msg_bytes2S16(&(buf[14]));
    return st;
}

// resets the motor position of a port.
void NXT_reset_motor_position(NXT *nxt,unsigned char port,
			      int relative,int ans,unsigned char *status){
    msg_t *msg;
    if(ans){
        msg = alloc_msg(0x0,0xa);
    }else{
        msg = alloc_msg(0x80,0xa);
    }
    msg_addU8(msg,port);
    if(relative){
        msg_addU8(msg,1);
    } else {
        msg_addU8(msg,0);
    }
    NXT_send_msg(nxt,msg);
    if(ans){
	char buf[3];
	NXT_recv_buffer(nxt,buf,3);
	if(buf[0] != 0x02 || buf[1] != 0x0a){
	    free_msg(msg);
	    return;
	}
	*status = buf[2];
    }
    free_msg(msg);
}

// sends a message to stay alive to the nxt.
void NXT_stay_alive(NXT* nxt){
    msg_t *msg = alloc_msg(0x80,0x0D);
    NXT_send_msg(nxt,msg);
    free_msg(msg);
}
