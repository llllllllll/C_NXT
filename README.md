C_NXT
===========

A library for interfacing with the LEGO NXT robot over a bluetooth connection
in C.

Written by Joe Jevnik.


Overview
--------

While writing my program for the IEEE club fall NXT challenge, I realized the
the only suggested means of programming the robot required non-free software or
libraries. In an attempt to find free solutions, I found lestat, a GPLv2 project
for providing an abstraction to the LEGO NXT in C++. I decided to use C to
program the robot, so I chose to write my own library using lestat as a starting
point, and changing things as I saw fit. The internals have been modified, and
the interface has been changed. I chose to abstract away the interface by adding
types to represent things that were previously represented in a more primitive
way. This allows for the use to have more type safety, and clarity when writing
their functions.


NXT
---

The NXT is a struct that represents a connection with the lego nxt. Functions
that deal with the NXT will be prefixed like:

    NXT_*

and take an `NXT*` as their first parameter, except for `alloc_NXT()` and 
`free_NXT()`


msg_t
-----

The `msg_t` is a struct that represents the type of data that will be sent to
the nxt over bluetooth. functions that deal with messages will be prefixed like:

    msg_*

and take a `msg_t` as their first parameter except for `alloc_msg()` and 
`free_msg()`


TODO:
-----

- Documentation

- Testing (need lab access)

- Larger default library, like `void NXT_turn(NXT* nxt,motor_port left,
  motor_port right,int degrees)` or things like that.
