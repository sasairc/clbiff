/*
 * clbiff -  simple mail notify program
 *
 * subset.c
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./subset.h"
#include "./config.h"
#include "./clbiff.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int check_file_stat(char* path)
{
    struct  stat st;

    if (stat(path, &st) != 0) {
        fprintf(stderr, "%s[%d]: %s: no such file or directory\n", PROGNAME, getpid(), path);

        return 1;
    }
    if (access(path, R_OK) != 0) {
        fprintf(stderr, "%s[%d]: %s: permission denied\n", PROGNAME, getpid(), path);

        return 2;
    }

    return 0;
}

int print_start_msg(clbiff_t* cl_t)
{
    fprintf(stdout, "%s %d.%d\n", PROGNAME, VERSION, PATCHLEVEL);
    fprintf(stdout, "\
pid      = %d\n\
file     = %s\n\
interval = %d sec\n\
command  = %s\n\
", getpid(), cl_t->farg, cl_t->iarg, cl_t->carg);

    return 0;
}

int print_usage(void)
{
    fprintf(stdout, "\
%s %d.%d, simple mail notify program\n\
Usage: clbiff [OPTION]...\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
\n\
  -i,  --interval=SECONDS    interval time (sec)\n\
  -c,  --command=COMMAND     specifies command\n\
  -f,  --file=file           monitored file\n\
  -q,  --quiet               quiet mode\n\
  -v,  --verbose             verbose mode\n\
\n\
       --help                display this help and exit\n\
       --version             output version infomation and exit\n\
\n\
Report %s bugs to %s <%s>\n\
", PROGNAME, VERSION, PATCHLEVEL, PROGNAME, AUTHOR, MAIL_TO);

    exit(0);
}

int print_version(void)
{
    fprintf(stdout, "%s %d.%d\n", PROGNAME, VERSION, PATCHLEVEL);

    exit(0);
}
