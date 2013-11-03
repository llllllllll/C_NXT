C_NXT
===========

A library for interfacing with the LEGO NXT robot over a bluetooth connection
based of the lestat library (C++) in C.

Written by Joe Jevnik.


Overview
--------

While writing my program for the IEEE club fall NXT challenge, I realized the
the only suggested means of programming the robot required non-free software or
libraries. In an attempt to find free solutions, I found lestat, a GPLv2 project
for providing an abstraction to the LEGO NXT in C++. I decided to do a rewrite
of this library for my own use, as well as change how some things are done,
using C.


NXT
---

The NXT is a struct that represents a connection with the lego nxt. Functions
that deal with the NXT will be prefixed like:

    NXT_*

and take an `NXT*` as their first parameter.


msg_t
-----

The `msg_t` is an `unsigned char*`, the type of data that will be sent to the
nxt over bluetooth. functions that deal with messages will be prefixed like:

    msg_*

and take a `msg_t` as their first parameter.


TODO:
-----

- Documentation

- Testing (need lab access)

- Motor control abstractions (they seem a little cryptic)
