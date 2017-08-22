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
#include <time.h>
#include <ctype.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#ifdef  WITH_SHARED
#include <benly/file.h>
#include <benly/memory.h>
#else
#include "./libbenly/src/file.h"
#include "./libbenly/src/memory.h"
/* WITH_SHARED */
#endif

char* get_mailbox_env(char* path)
{
    int     i       = 0;

    char*   inbox   = NULL,
        *   fmt[]   = {INBOX_MBOX, INBOX_MDIR, NULL};

    struct  stat    st;

    if (path == NULL)
        return NULL;

    while (*(fmt + i) != NULL) {
        if ((inbox = (char*)
                    smalloc(sizeof(char) * (strlen(path) + strlen(*(fmt + i))),
                        NULL)) == NULL)
            return NULL;

        memcpy(inbox, path, strlen(path) + 1);
        memcpy(inbox + strlen(inbox), *(fmt + i), strlen(*(fmt + i)));
        *(inbox + strlen(inbox)) = '\0';

        if (stat(inbox, &st) != 0) {
            free(inbox);
            inbox = NULL;
        } else {
            break;
        }
        i++;
    }

    return inbox;
}

int check_biff_file_stat(char* path)
{
    struct  stat st;

    if (path == NULL) {
        fprintf(stderr, "%s: mailbox not found, try resetting env $MAIL or use -f options\n",
                PROGNAME);

        return -1;
    }
    if (stat(path, &st) != 0) {
        fprintf(stderr, "%s[%d]: %s: %s\n",
                PROGNAME, getpid(), path, strerror(ENOENT));

        return -2;
    }
    if ((st.st_mode & S_IREAD) == 0) {
        fprintf(stderr, "%s[%d]: %s: %s\n",
                PROGNAME, getpid(), path, strerror(EACCES));

        return -3;
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
    while (i < argnum) {
        switch (i) {
            case    0:
#ifdef  WITH_ADD_INFO
                if (fp == stdout)
                    fprintf(fp, "%s: %s[%d]: [INFO]: ",
                            dstr, PROGNAME, getpid());
                else if (fp == stderr)
                    fprintf(fp, "%s: %s[%d]: [WARN]: ",
                            dstr, PROGNAME, getpid());
#else
                fprintf(fp, "%s: %s[%d]: ",
                        dstr, PROGNAME, getpid());
/* WITH_ADD_INFO */
#endif
                i++;
                break;
            default:
                str = va_arg(list, char*);
                fprintf(fp, "%s", str);
                i++;
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
/* WITH_USLEEP */
#endif
