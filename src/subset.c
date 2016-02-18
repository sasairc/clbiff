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
#include "./file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>

char* get_mailbox_env(char* path)
{
    int     i       = 0;

    char*   inbox   = NULL,
        *   fmt[]   = {INBOX_MBOX, INBOX_MDIR, NULL};

    struct  stat    st;

    if (path == NULL)
        return NULL;

    for (i = 0; fmt[i] != NULL; i++) {
        if ((inbox = (char*)
                    malloc(sizeof(char) * (strlen(path) + strlen(fmt[i])))) == NULL) {

            return NULL;
        } else {
            strcpy(inbox, path);
            strcat(inbox, fmt[i]);
        }
        if (stat(inbox, &st) != 0) {
            free(inbox);
            inbox = NULL;
        } else {
            break;
        }
    }

    return inbox;
}

int strisdigit(char* str)
{
    int i   = 0;

    while (i < strlen(str)) {
        if (!isdigit(*(str + i))) {
            fprintf(stderr, "%s: %s: invalid number of interval\n",
                    PROGNAME, str);

            return -1;
        }
        i++;
    }

    return 0;
}

int check_file_stat(char* path)
{
    struct  stat st;

    if (path == NULL) {
        fprintf(stderr, "%s: mailbox not found, try resetting env $MAIL or use -f options\n",
                PROGNAME);

        return 1;
    }
    if (stat(path, &st) != 0) {
        fprintf(stderr, "%s[%d]: %s: no such file or directory\n",
                PROGNAME, getpid(), path);

        return 2;
    }
    if (access(path, R_OK) != 0) {
        fprintf(stderr, "%s[%d]: %s: permission denied\n",
                PROGNAME, getpid(), path);

        return 3;
    }

    return 0;
}

int print_msg(int argnum, ...)
{
    int         i           = 0;

    char        dstr[128]   = {'\0'},
        *       str         = NULL;

    FILE*       fp          = NULL;

    time_t      timer;

    struct tm*  date;

    va_list     list;       /* list of variable arguments */

    timer = time(NULL);
    date = localtime(&timer);
    strftime(dstr, 128, "%Y-%m-%d %H:%M:%S %Z", date);

    /* processing of variable arguments */
    va_start(list, argnum);

    fp = va_arg(list, FILE*);
    for(i = 0; i < argnum; i++) {
        switch (i) {
            case    0:
#ifdef  WITH_ADD_INFO
                if (fp == stdout) {
                    fprintf(fp, "%s[%d]: %s: [INFO]: ",
                            PROGNAME, getpid(), dstr);
                } else if (fp == stderr) {
                    fprintf(fp, "%s[%d]: %s: [WARN]: ",
                            PROGNAME, getpid(), dstr);
                }
#else
                fprintf(fp, "%s[%d]: %s: ",
                        PROGNAME, getpid(), dstr);
#endif
                break;
            default:
                str = va_arg(list, char*);
                fprintf(fp, "%s", str);
                continue;
        }
    }
    va_end(list);

    return 0;
}

#ifndef WITH_USLEEP
int print_start_msg(clbiff_t* cl_t)
{
    fprintf(stdout, "%s %d.%d%s\n",
            PROGNAME, VERSION, PATCHLEVEL, EXTRAVERSION);

    fprintf(stdout, "\
pid      = %d\n\
file     = %s\n\
interval = %d sec\n\
command  = %s\n\
",
        getpid(), cl_t->farg, cl_t->iarg, cl_t->carg);

    return 0;
}
#else
int print_start_msg(clbiff_t* cl_t)
{
    fprintf(stdout, "%s %d.%d%s\n",
            PROGNAME, VERSION, PATCHLEVEL, EXTRAVERSION);

    fprintf(stdout, "\
pid      = %d\n\
file     = %s\n\
interval = %d usec\n\
command  = %s\n\
",
        getpid(), cl_t->farg, cl_t->iarg, cl_t->carg);

    return 0;
}
#endif
