/*
 * clbiff -  simple mail notify program
 *
 * clbiff.c
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./config.h"
#include "./clbiff.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    int     res,
            index;

    clbiff_t cl_t = {
        0, 0, 0, 0, 0, 0, NULL, NULL, NULL,
    };
    struct  option opts[] = {
        {"time",    required_argument,  NULL, 't'},
        {"inbox",   required_argument,  NULL, 'b'},
        {"exec",    required_argument,  NULL, 'e'},
        {"quiet",   no_argument,        NULL, 'q'},
        {"verbose", no_argument,        NULL, 'v'},
        {"help",    no_argument,        NULL,  0 },
        {"version", no_argument,        NULL,  1 },
        {0, 0, 0, 0},
    };

    while ((res = getopt_long(argc, argv, "t:b:e:qv", opts, &index)) != -1) {
        switch (res) {
            case    't':
                cl_t.tflag = 1;
                cl_t.targ = atoi(optarg);
                break;
            case    'b':
                cl_t.bflag = 1;
                cl_t.barg = optarg;
                break;
            case    'e':
                cl_t.eflag = 1;
                cl_t.earg = optarg;
                break;
            case    'q':
                cl_t.qflag = 1;
                break;
            case    'v':
                cl_t.vflag = 1;
                break;
            case    0:
                print_usage();
                break;
            case    1:
                print_version();
                break;
            case    '?':
                return -1;
        }
    }

    if (cl_t.tflag == 0) {
        cl_t.targ = DEFAULT_TMSEC;
    }
    if (cl_t.bflag == 0) {
        cl_t.barg = DEFAULT_INBOX;
    }
    if (cl_t.eflag == 0) {
        cl_t.earg = DEFAULT_EXEC;
    }
    if ((cl_t.args = str_to_args(cl_t.earg)) == NULL)
        return 1;

    return monitor(&cl_t);
}

int monitor(clbiff_t* cl_t)
{
    struct  stat stat_now;
    struct  stat stat_ago;

    while (1) {
        if (stat(cl_t->barg, &stat_now) != 0)
            return 10;

        sleep(cl_t->targ);

        if (stat(cl_t->barg, &stat_ago) != 0)
            return 11;

        if (stat_now.st_mtime != stat_ago.st_mtime) {
            exec_cmd(cl_t->args, cl_t->vflag);
        }
    }

    return 12;
}

int exec_cmd(char** args, int vflag)
{
    pid_t   pid;

    if ((pid = fork()) < 0) {
        fprintf(stderr, "[FAIL] %s: fork() on exec_cmd() falure\n", PROGNAME);

        return errno;
    } else if (pid == 0) {
        if (vflag == 1)
            fprintf(stdout, "[INFO] %s: exec %s\n", PROGNAME, args[0]);
        execvp(args[0], args);
        fprintf(stdout, "[FAIL] %s: exec %s failure\n", PROGNAME, args[0]);
    }

    return 0;
}

char** str_to_args(char* str)
{
    if (str == NULL)
        return NULL;

    int     i,
            sx, ax, ay, xt,
            elmc,
            dspf;
    char**  args;
    
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

    if (elmc > 1) {
        args = (char**)malloc(sizeof(char*) * (elmc + 1));
        if (args == NULL)
            return NULL;
    } else {
        return NULL;
    }

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
            xt++;
            ax++;
            ay++;
            dspf = 1;
        } else {
            dspf = 0;
        }
    }
    args[ay + 1] = NULL;

#ifdef  DEBUG
    for (i = 0; i <= elmc; i++)
        fprintf(stdout, "DEBUG: args[%d] = %s\n", i, args[i]);
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

/*
int set_signal(int sig)
{
    if (signal(sig, catch_signal) == SIG_ERR) {
        fprintf(stderr, "%s: set_signal() failure\n", PROGNAME);
    }

    return 1;
}

void catch_signal(int sig)
{
    return;
}
*/

int print_usage(void)
{
    fprintf(stdout, "\
%s %d.%d, simple mail notify programn\n\
Usage: clbiff [OPTION]...\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
\n\
  -t,  --time=SEC            interval (sec)\n\
  -b,  --inbox=PATH          mail inbox\n\
  -e,  --exec=COMMAND        called command\n\
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
