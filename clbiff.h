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

typedef struct CLBIFF_T {
    int     tflag;
    int     bflag;
    int     eflag;
    int     qflag;
    int     vflag;
    int     targ;
    char*   barg;
    char*   earg;
    char**  args;
} clbiff_t;

extern int monitor(clbiff_t* cl_t);
extern int exec_cmd(char** args, int vflag);
extern char** str_to_args(char* str);
//extern int set_signal(int sig);
//extern void catch_signal(int sig);
extern int print_usage(void);
extern int print_version(void);

#endif
