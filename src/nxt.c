/* nxt.c --- Implementation of the C_NXT library.
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

#include "nxt.h"

// A static array off all NXT's.
NXT   *opened_nxtv[256];
size_t opened_nxtc = 0;

// A custom handler for the lego NXT robot.
// If a segfault is encountered, it will stop all the motors, release all
// resources, and print an error message, then call abort().
void NXT_sigsegv(int sig __attribute__((unused))){
    size_t n;
    for (n = 0;n < opened_nxtc;n++){
        if (opened_nxtv[n]){
            NXT_destroy(opened_nxtv[n]);
        }
    }
    write(STDERR_FILENO,"Oops, invalid memory access!\n",29);
    abort();
}

// Initializes a previously allocate NXT structure.
// return: 0 on success, non-zero on failure.
int NXT_init(NXT *nxt){
    if (!nxt){
        return -1;
    }
    nxt->dev_id = hci_get_route(NULL);
    nxt->local_sock = hci_open_dev(nxt->dev_id);
    if (nxt->dev_id < 0 || nxt->local_sock < 0){
	return -2;
    }
    nxt->remote_sock = socket(AF_BLUETOOTH,SOCK_STREAM,BTPROTO_RFCOMM);
    if (opened_nxtc >= 255){
        errno = ENOBUFS;
        return -1;
    }
    nxt->index                 = opened_nxtc;
    opened_nxtv[opened_nxtc++] = nxt;
    return 0;
}

// Safely releases all resources held by this NXT structure.
// return: 0 on success, non-zero on failure.
int NXT_destroy(NXT *nxt){
    opened_nxtv[nxt->index] = NULL;
    return NXT_stopallmotors(nxt)
        || close(nxt->local_sock);
}

// Connects an NXT to a given mac address.
// return: 0 on success, non-zero on failure.
int NXT_connect(NXT *nxt,const char *mac){
    struct sockaddr_rc addr = { 0 };
    addr.rc_family          = AF_BLUETOOTH;
    addr.rc_channel         = (uint8_t) 1;
    str2ba(mac,&addr.rc_bdaddr);
    return connect(nxt->remote_sock,(struct sockaddr*) &addr,sizeof(addr));
}

// Disconnects the NXT structure from the robot.
// return: 0 on success, non-zero on failure.
int NXT_disconnect(NXT *nxt){
    return close(nxt->remote_sock);
}

// sends a buffer to the nxt.
// return: 0 on success, non-zero on failure.
int NXT_send_buffer(NXT *nxt,char *b,unsigned short len){
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

// Sends a nxt_msg to the NXT.
// On success returns 0.
// On failure returns -1.
int NXT_send_msg(NXT *nxt,nxt_msg *msg){
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
// return:
// on success returns 0.
// if the buffer is too small, returns -1.
// if the read fails, returns -2.
int NXT_recv_buffer(NXT *nxt,char *b,unsigned short len){
    char l2[2];
    unsigned short msg_size;
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
    return 0;
}

// Plays a tone on the nxt.
// return: 0 on success, non-zero on failure.
int NXT_play_tone(NXT *nxt,unsigned short freq,unsigned short time,
                  bool ans,unsigned char *status){
    nxt_msg *msg;
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
	    return -1;
	}
	*status = buf[2];
    }
    free_msg(msg);
    usleep(time * 1000);
    return 0;
}

// Returns the remaining mV of charge on the nxt.
// return: 0 on success, non-zero on failure.
int NXT_battery_level(NXT *nxt){
    nxt_msg *msg = alloc_msg(0,0x0b);
    char buf[5];
    int err;
    if (NXT_send_msg(nxt,msg)) {
        free_msg(msg);
        return -1;
    }
    err = NXT_recv_buffer(nxt,buf,5);
    free_msg(msg);
    if (err) {
        return err;
    }
    return (unsigned short) (buf[3] + 256 * buf[4]);
}

// sets the output state on the nxt.
// return: 0 on success, non-zero on failure.
int NXT_set_motorstate(NXT *nxt,motorstate *st,bool ans,
                       unsigned char *status){
    nxt_msg *msg;
    char     buf[3];
    if(st->power > 100 || st->power < -100){
        return -2;
    }
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
	NXT_recv_buffer(nxt,buf,3);
	if(buf[0] != 0x02 || buf[1] != 0x04){
	    return -1;
	}
	*status=buf[2];
        return 0;
    }
    return 0;
}

// returns the motorstate of the nxt on the given port.
int NXT_get_motorstate(NXT *nxt,motor_port port,motorstate *st){
    nxt_msg *msg = alloc_msg(0x0,0x6);
    char     buf[25];
    msg_addU8(msg,port);
    NXT_send_msg(nxt,msg);
    NXT_recv_buffer(nxt,buf,25);
    free_msg(msg);
    if(buf[0]!= 0x02 || buf[1] != 0x06){
        return -1;
    }
    if(buf[2] != 0){
        return -2;
    }
    st->port              = buf[3];
    st->power             = buf[4];
    st->mode              = buf[5];
    st->reg_mode          = buf[6];
    st->turn_ratio        = buf[7];
    st->run_state         = buf[8];
    st->tacho_limit       = msg_bytes2U32(&(buf[9]));
    st->tacho_count       = msg_bytes2S32(&(buf[13]));
    st->block_tacho_count = msg_bytes2S32(&(buf[17]));
    st->rotation_count    = msg_bytes2S32(&(buf[21]));
    return 0;
}

// Initializes all motors by turning them on.
// return: 0 on success, non-zero on failure.
int NXT_initmotors(NXT *nxt){
    motorstate st = { MOTOR_ALL,0,RUN_BRAKE,SYNCHRONIZATION,0,0,0,0,0,0 };
    return NXT_set_motorstate(nxt,&st,false,NULL);
}

// sets the input mode of the nxt.
// return: 0 on success, non-zero on failure.
int NXT_set_input_mode(NXT *nxt,sensor_port port,sensor_type type,
                       sensor_mode mode,bool ans,unsigned char *status){
    nxt_msg *msg;
    if(ans){
	msg = alloc_msg(0x00,0x05);
    } else {
	msg = alloc_msg(0x80,0x05);
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
	    return -1;
	}
	*status = buf[2];
    }
    free_msg(msg);
    return 0;
}

// Retrieves the sensor state off the NXT of the given port, storing the
// result in r.
// return: 0 on success, non-zero on failure.
int NXT_get_sensorstate(NXT *nxt,sensor_port port,sensorstate *st){
    nxt_msg *msg = alloc_msg(0x00,0x07);
    char     buf[16];
    msg_addU8(msg,port);
    NXT_send_msg(nxt,msg);
    NXT_recv_buffer(nxt,buf,16);
    free_msg(msg);
    if(buf[0] != 0x02 || buf[1] != 0x07){
        return -1;
    }
    if(buf[2] != 0){
        return -2;
    }
    st->port             = buf[3];
    st->valid            = (buf[4] > 0) ? 1 : 0;
    st->calibrated       = (buf[5] > 0) ? 1 : 0;
    st->type             = (sensor_type) ((int) buf[6]);
    st->mode             = (sensor_mode) ((int) buf[7]);
    st->raw_value        = msg_bytes2U16(&(buf[8]));
    st->normalized_value = msg_bytes2U16(&(buf[10]));
    st->scaled_value     = msg_bytes2S16(&(buf[12]));
    st->calibrated_value = msg_bytes2S16(&(buf[14]));
    return 0;
}

// resets the motor position of a port.
int NXT_reset_motor_position(NXT *nxt,motor_port port,bool relative,
                             bool ans,unsigned char *status){
    nxt_msg *msg;
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
	    return -1;
	}
	*status = buf[2];
    }
    free_msg(msg);
    return 0;
}

// sends a message to stay alive to the nxt.
// return: 0 on success, non-zero on failure.
int NXT_stay_alive(NXT* nxt){
    nxt_msg *msg = alloc_msg(0x80,0x0D);
    int      r   = NXT_send_msg(nxt,msg);
    free_msg(msg);
    return r;
}
