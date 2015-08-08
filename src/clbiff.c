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
#include "./info.h"
#include "./signal.h"
#include "./file.h"
#include "./string.h"
#include "./memory.h"
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <sys/stat.h>

int hflag = 0;  /* monitor() loop flag */

int main(int argc, char* argv[])
{
    int     i       = 0,
            ret     = 0,
            res     = 0,
            index   = 0;

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
    siglist_t siglist[] = {
        {SIGINT,    catch_signal},
        {SIGTERM,   catch_signal},
        {0,         NULL},
    };
    if (set_signal_siglist(siglist) != 0) {
        print_msg(2, stderr, "set_signal_siglist() failure\n");

        return 1;
    };

    /* procssing of arguments */
    while ((res = getopt_long(argc, argv, "i:f:c:qv", opts, &index)) != -1) {
        switch (res) {
            case    'i':
                for (i = 0; i < strlen(optarg); i++) {
                    if (!isdigit(*(optarg + i))) {
                        fprintf(stderr, "%s: %s: invalid number of interval\n",
                                PROGNAME, optarg);

                        return -1;
                    }
                }
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
            case    1:
                print_version();
            case    '?':
                return -1;
        }
    }

    /* setting value */
    if ((ret = init(&cl_t)) != 0) {

        return ret;
    }
    /* verbose message */
    if (cl_t.vflag == 1) {
        print_start_msg(&cl_t);
    }
    /* prevention zombie process */
    handl_zombie_proc();

    /* do main loop */
    return monitor(&cl_t);
}

int init(clbiff_t* cl_t)
{
    if (cl_t->iflag == 0) {
        cl_t->iarg = DEFAULT_TMSEC;
    }
    if (cl_t->fflag == 0 && (cl_t->farg = get_mailbox_env()) == NULL) {
        fprintf(stderr, "%s: mailbox not found, try setting env $MAIL or use -f options\n",
                PROGNAME);

        return 1;
    }
    if (check_file_stat(cl_t->farg) != 0) {

        return 2;
    }
    if (cl_t->cflag == 0) {
        cl_t->carg = DEFAULT_EXEC;
    }
    /* str to array */
    if ((cl_t->args = str_to_args(cl_t->carg)) == NULL) { 

        return 3;
    }

    return 0;
}

int monitor(clbiff_t* cl_t)
{
    int             e_errno = 0;

    struct  stat    stat_now,
                    stat_ago;

    while (hflag == 0) {
        if (stat(cl_t->farg, &stat_now) != 0) {
            print_msg(2, stderr, "stat()failure\n");
            release(cl_t);

            return errno;
        }
#ifndef WITH_USLEEP
        sleep(cl_t->iarg);
#else
        usleep(cl_t->iarg);
#endif
        if (stat(cl_t->farg, &stat_ago) != 0) {
            print_msg(2, stderr, "stat()failure\n");
            release(cl_t);

            return errno;
        }
        if (stat_now.st_mtime != stat_ago.st_mtime) {
            if ((e_errno = exec_cmd(cl_t->args, cl_t->vflag)) > 0) {

                return errno;
            }
        }
    }

    /* interrupt handling */
    if (cl_t->vflag) {
        fprintf(stdout, "\n%s[%d]: exiting on signal %d\n",
                PROGNAME, getpid(), hflag);
    }
    /* release memory */
    release(cl_t);

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
        print_msg(2, stderr, "fork() on exec_cmd() failure\n");

        return errno;
    } else if (pid == 0) {
        if (vflag == 1)
            print_msg(4, stdout, "exec ", args[0], "\n");
        /* exec proccess */
        execvp(args[0], args);
        /* do error */
        print_msg(4, stderr, "exec ", args[0], " failure\n");

        exit(errno);
    }

    return 0;
}

void catch_signal(int sig)
{
    hflag = sig;    /* brak monitor loop */
}

void release(clbiff_t* cl_t)
{
#ifdef  DEBUG
    int i;

    fprintf(stderr, "DEBUG: release(): cl_t->farg(%p) = %s\n",
            cl_t->farg, cl_t->farg);
    fprintf(stderr, "DEBUG: release(): cl_t->args(%p)\n",
            cl_t->args);

    for (i = 0; i <= p_count_file_lines(cl_t->args); i++)
        fprintf(stderr, "DEBUG: release(): cl_t->args[%d](%p) = %s\n",
                i, cl_t->args[i], cl_t->args[i]);
#endif

    if (cl_t->fflag == 0 && cl_t->farg != NULL) {
        free(cl_t->farg);
        cl_t->farg = NULL;
    }
    free2d(cl_t->args, p_count_file_lines(cl_t->args));

    return;
}