/*
 * clbiff -  simple mail notify program
 *
 * clbiff.h 
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#ifndef CLBIFF_H
#define CLBIFF_H

#include "./cmd.h"
#include "./polyaness.h"

typedef struct CLBIFF_T {
    int     iflag;
    int     fflag;
    int     cflag;
    int     qflag;
    int     vflag;
    int     iarg;
    char*   farg;
    char*   carg;
} clbiff_t;

#define CLBIFF_ALLNO_FLAG   \
    0, 0, 0, 0, 0, 0, NULL, NULL

extern int read_clbiffrc(clbiff_t* cl_t, polyaness_t** pt);
extern int init(clbiff_t* cl_t, cmd_t** cmd, cmd_t** start);
extern int monitor(clbiff_t* cl_t, cmd_t* cmd, polyaness_t* pt);
extern void catch_signal(int sig);
extern void release(clbiff_t* cl_t, cmd_t* cmd, polyaness_t* pt);

/* CLBIFF_H */
#endif
