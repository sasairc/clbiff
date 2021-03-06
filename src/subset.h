/*
 * clbiff -  simple mail notify program
 *
 * subset.h
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#ifndef SUBSET_H
#define SUBSET_H

#include "./clbiff.h"

extern char* get_mailbox_env(char* path);
extern int check_biff_file_stat(char* path);
extern int print_msg(int argnum, ...);
extern int print_start_msg(clbiff_t* clb);
extern int print_usage(void);
extern int print_version(void);

/* SUBSET_H */
#endif
