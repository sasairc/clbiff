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
    char**  args;
} clbiff_t;

/* This functions is required clbiff.c */
extern int read_clbiffrc(clbiff_t* cl_t, polyaness_t** pt);
extern int init(clbiff_t* cl_t);
extern int monitor(clbiff_t* cl_t, polyaness_t* pt);
extern int exec_cmd(char** args, int vflag);
extern void catch_signal(int sig);
extern void release(clbiff_t* cl_t, polyaness_t* pt);

#endif
