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

// Drive fowards for s seconds at p power
// If s is negative, drive  foward forever.
void NXT_driveforward(NXT *nxt,time_t s,unsigned char p,
                      motor_port l,motor_port r){
    motorstate lst = { l,truncate_power(p),REGULATION,SYNCHRONIZATION,0,RUNNING,
                       0,0,0,0 };
    motorstate rst = { r,truncate_power(p),REGULATION,SYNCHRONIZATION,0,RUNNING,
                       0,0,0,0 };
    NXT_set_motorstate(nxt,&lst,false,NULL);
    NXT_set_motorstate(nxt,&rst,false,NULL);
    if (s < 0){
        return;
    }
    sleep((unsigned int) s);
    rst.power = 0;
    lst.power = 0;
    NXT_set_motorstate(nxt,&lst,false,NULL);
    NXT_set_motorstate(nxt,&rst,false,NULL);
}

// Drive backwards for s seconds at p power.
// If s is negative, drive  foward forever.
void NXT_drivebackward(NXT *nxt,time_t s,unsigned char p,
                       motor_port l,motor_port r){
    motorstate lst = { l,-truncate_power(p),SYNCHRONIZATION,0,RUNNING,0,0,0,0 };
    motorstate rst = { r,-truncate_power(p),SYNCHRONIZATION,0,RUNNING,0,0,0,0 };
    NXT_set_motorstate(nxt,&lst,false,NULL);
    NXT_set_motorstate(nxt,&rst,false,NULL);
    if (s < 0){
        return;
    }
    sleep((unsigned int) s);
    rst.power = 0;
    lst.power = 0;
    NXT_set_motorstate(nxt,&lst,false,NULL);
    NXT_set_motorstate(nxt,&rst,false,NULL);
}
