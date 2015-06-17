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
#include "./subset.h"
#include "./file.h"
#include "./memory.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <sys/stat.h>

int hflag = 0;  /* monitor() loop flag */

int main(int argc, char* argv[])
{
    int     res,
            index;

    /* flag and args */
    clbiff_t cl_t = {
        0, 0, 0, 0, 0, 0, NULL, NULL, NULL,
    };
    /* option for getopt_long() */
    struct  option opts[] = {
        {"interval",    required_argument,  NULL, 'i'},
        {"file",        required_argument,  NULL, 'f'},
        {"command",     required_argument,  NULL, 'c'},
        {"quiet",       no_argument,        NULL, 'q'},
        {"verbose",     no_argument,        NULL, 'v'},
        {"help",        no_argument,        NULL,  0 },
        {"version",     no_argument,        NULL,  1 },
        {0, 0, 0, 0},
    };
    /* setting signal handler */
    if (set_signal(SIGINT) != 0 || set_signal(SIGTERM) != 0) {
        return 1;
    }
    /* procssing of arguments */
    while ((res = getopt_long(argc, argv, "i:f:c:qv", opts, &index)) != -1) {
        switch (res) {
            case    'i':
                cl_t.iflag = 1;
                cl_t.iarg = atoi(optarg);
                break;
            case    'f':
                cl_t.fflag = 1;
                cl_t.farg = optarg;
                break;
            case    'c':
                cl_t.cflag = 1;
                cl_t.carg = optarg;
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

    /* setting value */
    if (cl_t.iflag == 0) {
        cl_t.iarg = DEFAULT_TMSEC;
    }
    if (cl_t.fflag == 0) {
        cl_t.farg = DEFAULT_INBOX;
    }
    if (check_file_stat(cl_t.farg) != 0) {
        return 2;
    }
    if (cl_t.cflag == 0) {
        cl_t.carg = DEFAULT_EXEC;
    }
    if (cl_t.vflag == 1) {
        print_start_msg(&cl_t);
    }
    /* str to array */
    if ((cl_t.args = str_to_args(cl_t.carg)) == NULL)
        return 3;

    return monitor(&cl_t);
}

int monitor(clbiff_t* cl_t)
{
    int     e_errno;
    struct  stat stat_now;
    struct  stat stat_ago;

    while (hflag == 0) {
        if (stat(cl_t->farg, &stat_now) != 0) {
            fprintf(stderr, "%s[%d]: stat() failure\n", PROGNAME, getpid());
            release(cl_t->args);

            return errno;
        }

        sleep(cl_t->iarg);

        if (stat(cl_t->farg, &stat_ago) != 0) {
            fprintf(stderr, "%s[%d]: stat() failure\n", PROGNAME, getpid());
            release(cl_t->args);

            return errno;
        }

        if (stat_now.st_mtime != stat_ago.st_mtime) {
            if ((e_errno = exec_cmd(cl_t->args, cl_t->vflag)) > 0) {

                return errno;
            }
        }
    }

    if (cl_t->vflag) {
        fprintf(stdout, "%s[%d] exiting on signal %d\n", PROGNAME, getpid(), hflag);
    }
    release(cl_t->args);    /* release memory */

    return 0;
}

int exec_cmd(char** args, int vflag)
{
    pid_t   pid;

    /*
     *  pid = 0: child process
     *  pid > 0: parent process
     */
    if ((pid = fork()) < 0) {
        fprintf(stderr, "%s[%d]: fork() on exec_cmd() falure\n", PROGNAME, getpid());

        return errno;
    } else if (pid == 0) {
        if (vflag == 1)
            fprintf(stdout, "%s[%d]: exec %s\n", PROGNAME, getpid(), args[0]);
        /* exec proccess */
        execvp(args[0], args);

        /* do error */
        fprintf(stderr, "%s[%d]: exec %s failure\n", PROGNAME, getpid(), args[0]);

        return errno;
    }

    return 0;
}

int set_signal(int sig)
{
    if (signal(sig, catch_signal) == SIG_ERR) {
        fprintf(stderr, "%s[%d]: set_signal() failure\n", PROGNAME, getpid());

        return 1;
    }

    return 0;
}

void catch_signal(int sig)
{
    hflag = sig;    /* brak monitor loop */
}

void release(char** args)
{

#ifdef  DEBUG
    int i;

    fprintf(stderr, "DEBUG: release(): args(%p)\n", args);
    for (i = 0; i <= p_count_file_lines(args); i++)
        fprintf(stderr, "DEBUG: release(): args[%d](%p) = %s\n", i, args[i], args[i]);
#endif

    free2d(args, p_count_file_lines(args));

    return;
}
