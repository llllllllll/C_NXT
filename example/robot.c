/* robot.c --- Example use of the c_nxt library.
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

// Where execution begins:
int main(void){
    // Setup (don't worry too much about this):
    NXT  nxt;
    NXT *robot = &nxt;
    if (NXT_init(robot) || NXT_connect(robot,"00:16:53:1A:14:6A")){
        perror("main: nxt_init");
        return EXIT_FAILURE;
    }
    signal(SIGSEGV,NXT_sigsegv);

    // Tell the robot there is a button on sensor port 2.
    NXT_initbutton(robot,SENSOR_2);

    // Write "Running..." to the screen.
    puts("Running...");

    // Control (your code goes here):

    // Make the robot drive forward forever.
    NXT_driveforward(robot,FOREVER,100,MOTOR_A,MOTOR_B);

    // Wait for the button to be pressed.
    while(!NXT_ispressed(robot,SENSOR_2));

    // Drive backwards for 3 seconds.
    NXT_drivebackward(robot,3,80,MOTOR_A,MOTOR_B);

    // Turn to the left for 3 seconds.
    NXT_turnleft(robot,3,100,MOTOR_A,MOTOR_B);

    // Cleanup:
    NXT_destroy(robot);
    return EXIT_SUCCESS;
}
