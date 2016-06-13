/* procedures.c --- Premade procedures for the C_NXT library.
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

// Truncates the power argument to fit the range accepted.
// return: the power after being truncated.
#define truncate_power(p) ((p > 100) ? 100 : p)

// Tells the nxt that there is a button plugged in on this port.
// return: 0 on success, non-zero on failure.
int NXT_initbutton(NXT *nxt,sensor_port p){
    return NXT_set_input_mode(nxt,p,SWITCH,RAW_MODE,0,NULL);
}

// Tells the nxt that there is a light sensor plugged in on this port.
// return: 0 on success, non-zero on failure.
int NXT_initlight(NXT *nxt,sensor_port p){
    return NXT_set_input_mode(nxt,p,LIGHT_ACTIVE,BOOLEAN_MODE,0,NULL);
}

// Queries the button plugged into port p.
// return: true if the button is being pressed, false otherwise.
int NXT_ispressed(NXT *nxt,sensor_port p){
    sensorstate st;
    if (NXT_get_sensorstate(nxt,p,&st)){
        return -1;
    }
    return st.raw_value != 1023;
}

// Queries the value of the light sensor plugged into port p.
// return: -1 on failure, the light value otherwise.
int NXT_readlight(NXT *nxt,sensor_port p){
    sensorstate st;
    if (NXT_get_sensorstate(nxt,p,&st)){
        return -1;
    }
    return st.raw_value;
}

// Drive fowards for s seconds at p power
// If s is negative, drive  foward forever.
int NXT_driveforward(NXT *nxt,time_t s,power p,
                      motor_port l,motor_port r){
    motorstate lst = { l,truncate_power(p),REGULATION,SYNCHRONIZATION,0,
                       RUNNING,0,0,0,0 };
    motorstate rst = { r,truncate_power(p),REGULATION,SYNCHRONIZATION,0,
                       RUNNING,0,0,0,0 };
    if (NXT_set_motorstate(nxt,&lst,false,NULL) ||
        NXT_set_motorstate(nxt,&rst,false,NULL)) {
        return -1;
    }
    if (s < 0){
        NXT_stay_alive(nxt);
        return 0;
    }
    sleep((unsigned int) s);
    return (NXT_stopmotor(nxt,l) ||
            NXT_stopmotor(nxt,r));
}

// Drive backwards for s seconds at p power.
// If s is negative, drive  foward forever.
int NXT_drivebackward(NXT *nxt,time_t s,power p,
                       motor_port l,motor_port r){
    motorstate lst = { l,-truncate_power(p),REGULATION,SYNCHRONIZATION,0,
                       RUNNING,0,0,0,0 };
    motorstate rst = { r,-truncate_power(p),REGULATION,SYNCHRONIZATION,0,
                       RUNNING,0,0,0,0 };
    if (NXT_set_motorstate(nxt,&lst,false,NULL) ||
        NXT_set_motorstate(nxt,&rst,false,NULL)) {
        return -1;
    }
    if (s < 0){
        NXT_stay_alive(nxt);
        return 0;
    }
    sleep((unsigned int) s);
    return (NXT_stopmotor(nxt,l) ||
            NXT_stopmotor(nxt,r));
}

// Turns the robot left for s seconds (forever if s < 0).
int NXT_turnleft(NXT *nxt,time_t s,power p,
                  motor_port l,motor_port r){
    motorstate lst = { l,-truncate_power(p),REGULATION,POWER_CONTROL,0,
                       RUNNING,0,0,0,0 };
    motorstate rst = { r,truncate_power(p),REGULATION,POWER_CONTROL,0,
                       RUNNING,0,0,0,0 };
    if (NXT_set_motorstate(nxt,&lst,false,NULL) ||
        NXT_set_motorstate(nxt,&rst,false,NULL)) {
        return -1;
    }
    if (s < 0){
        NXT_stay_alive(nxt);
        return 0;
    }
    sleep((unsigned int) s);
    return (NXT_stopmotor(nxt,l) ||
            NXT_stopmotor(nxt,r));
}

// Turns the robot right for s seconds (forever if s < 0).
int NXT_turnright(NXT *nxt,time_t s,power p,
                  motor_port l,motor_port r){
    motorstate lst = { l,truncate_power(p),REGULATION,POWER_CONTROL,0,
                       RUNNING,0,0,0,0 };
    motorstate rst = { r,-truncate_power(p),REGULATION,POWER_CONTROL,0,
                       RUNNING,0,0,0,0 };
    if (NXT_set_motorstate(nxt,&lst,false,NULL) ||
        NXT_set_motorstate(nxt,&rst,false,NULL)) {
        return -1;
    }
    if (s < 0){
        NXT_stay_alive(nxt);
        return 0;
    }
    sleep((unsigned int) s);
    return (NXT_stopmotor(nxt,l) ||
            NXT_stopmotor(nxt,r));
}

// Set the velocity of a motor.
// return: 0 on success, non-zero on failure.
int NXT_setmotor(NXT *nxt,motor_port p,char v){
    motorstate st;
    st.port      = p;
    st.power     = v;
    st.mode      = RUN_BRAKE;
    st.run_state = RUNNING;
    st.reg_mode  = POWER_CONTROL;
    return NXT_set_motorstate(nxt,&st,false,NULL)
        || NXT_stay_alive(nxt);
}


// Stop a motor (set velocity to 0).
// return: 0 on success, non-zero on failure.
int NXT_stopmotor(NXT *nxt,motor_port p){
    return NXT_setmotor(nxt,p,0);
}

// Stops all motors (sets velocity to 0).
// return: 0 on success, non-zero on failure.
int NXT_stopallmotors(NXT *nxt){
    return NXT_stopmotor(nxt,MOTOR_A)
        || NXT_stopmotor(nxt,MOTOR_B)
        || NXT_stopmotor(nxt,MOTOR_C);
}
