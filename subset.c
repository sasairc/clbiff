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
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

char** str_to_args(char* str)
{
    /*
     * # note
     *
     *  sx   : current strings array pointer
     *  xt   : temporary string array pointer
     *  ax   : args array pointer (X)
     *  ay   : args array pointer (Y)
     *  elmc : elements counter
     *  dspf : double space flag
     */
    int     i,
            sx, xt, ax, ay,
            elmc,
            dspf;
    char**  args;
    
    /* count elements */
    for (i = dspf = 0, elmc = 1; str[i] != '\0'; i++) {
        if (str[i] == ' ' || str[i] == '\t') {
            if (dspf == 1)
                continue;

            elmc++;
            dspf = 1;
        } else {
            dspf = 0;
        }
    }

    if (elmc >= 1) {
        args = (char**)malloc(sizeof(char*) * (elmc + 1));
        if (args == NULL)
            return NULL;
    } else {
        return NULL;
    }
    /* string to args */
    for (dspf = sx = ay = ax = xt = 0; sx <= strlen(str); sx++) {
        if (str[sx] == ' ' || str[sx] == '\t' || str[sx] == '\0') {
            if (dspf == 1) {
                xt++;
                continue;
            }
            if ((args[ay] = (char*)malloc(sizeof(char) * (sx - xt + 1))) == NULL)
                goto ERR;

            for (ax = 0; xt < sx; xt++, ax++) {
                    args[ay][ax] = str[xt];
            }
            args[ay][ax] = '\0';
            xt++;
            ay++;
            dspf = 1;
        } else {
            dspf = 0;
        }
    }

#ifdef  DEBUG
    fprintf(stderr, "DEBUG: str_to_args(): args(%p)\n", args);
    for (i = 0; i <= elmc; i++)
        fprintf(stderr, "DEBUG: str_to_args(): args[%d](%p) = %s\n", i, args[i], args[i]);
#endif

    return args;


ERR:
    for (i = 0; i < elmc; i++) {
        if (args[i] != NULL) {
            free(args[i]);
            args[i] = NULL;
        }
    }
    free(args);

    return NULL;
}

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
%s %d.%d, simple mail notify programn\n\
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
