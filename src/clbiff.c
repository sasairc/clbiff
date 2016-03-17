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
#include "./cmd.h"
#include "./info.h"
#include "./signal.h"
#include "./file.h"
#include "./string.h"
#include "./memory.h"
#include "./polyaness.h"
#include "./env.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <sys/stat.h>

int hflag   = 0;    /* monitor() loop flag */

int main(int argc, char* argv[])
{
    int             ret     = 0,
                    res     = 0,
                    index   = 0;

    cmd_t*          cmd     = NULL,
         *          start   = NULL;

    polyaness_t*    pt      = NULL;

    /* flag and args */
    clbiff_t cl_t = {
        CLBIFF_ALLNO_FLAG,
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
                if (strisdigit(optarg) < 0)
                    return -1;

                cl_t.iflag = 1;
                cl_t.iarg = atoi(optarg);
                break;
            case    'f':
                cl_t.fflag = 1;
                if ((cl_t.farg = (char*)
                            malloc(sizeof(char) * (strlen(optarg) + 1))) == NULL) {
                    fprintf(stderr, "%s: malloc() failure\n",
                            PROGNAME);

                    return -1;
                }
                memcpy(cl_t.farg, optarg, strlen(optarg) + 1);
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

    /* read ~/.clbiffrc */
    if (read_clbiffrc(&cl_t, &pt) < 0) {
        release_polyaness(pt);

        return 2;
    }

    /* setting value */
    if ((ret = init(&cl_t, &cmd, &start)) < 0)
        return 3;

    /* verbose message */
    if (cl_t.vflag == 1)
        print_start_msg(&cl_t);

    /* do main loop */
    return monitor(&cl_t, start, pt);
}

int read_clbiffrc(clbiff_t* cl_t, polyaness_t** pt)
{
    int     i   = 0;

    char*   val = NULL,
        *   rc  = NULL;

    FILE*   fp  = NULL;

    /* read ~/.clbiffrc */
    rc = strlion(2, getenv("HOME"), "/.clbiffrc");
    if ((fp = fopen(rc, "r")) == NULL)
        return 0;
    else
        free(rc);

    /* parse .clbiffrc */
    init_polyaness(fp, 0, pt);
    if (parse_polyaness(fp, 0, pt) < 0) {
        fprintf(stderr, "%s: parse_polyaness() failure\n",
                PROGNAME);

        return -1;
    }
    fclose(fp);

    /* set values */
    while (i < (*pt)->recs) {
        if ((val = get_polyaness("interval", i, pt)) != NULL) {
            trim(val);
            if (strisdigit(val) < 0)
                return -2;

            cl_t->iflag = 1;
            cl_t->iarg = (int)atoi(val);
        }
        if ((val = get_polyaness("file", i, pt)) != NULL) {
            cl_t->fflag = 1;
            if ((cl_t->farg = (char*)
                        malloc(sizeof(char) * (strlen(val) + 1))) == NULL) {
                fprintf(stderr, "%s: malloc() failure\n",
                        PROGNAME);

                return - 3;
            }
            memcpy(cl_t->farg, val, strlen(val) + 1);
        }
        if ((val = get_polyaness("command", i, pt)) != NULL) {
            cl_t->cflag = 1;
            cl_t->carg = val;
        }
        if ((val = get_polyaness("verbose", i, pt)) != NULL)
            cl_t->vflag = 1;

        i++;
    }

    return 0;
}

int init(clbiff_t* cl_t, cmd_t** cmd, cmd_t** start)
{
    int     i       = 0;

    char*   tmp     = NULL;

    env_t*  envt    = NULL;

    /* setting default interval */
    if (cl_t->iflag == 0)
        cl_t->iarg = DEFAULT_TMSEC;

    /* do seach $MAIL on mailbox */
    if (cl_t->fflag == 0) {
        if ((envt = split_env(getenv("MAIL"))) == NULL) {
            fprintf(stderr, "%s: mailbox not found, try setting env $MAIL or use -f options\n",
                    PROGNAME);

            return -1;
        }

        do {
            if ((cl_t->farg = get_mailbox_env(envt->envs[i])) != NULL)
                break;

            i++;
        } while (i < envt->envc);

        release_env_t(envt);
    }
    trim(cl_t->farg);

    /*
     * -f ~/hoge
     */
    if (cl_t->farg[0] == '~' && cl_t->farg[1] == '/') {
        tmp = getenv("HOME");
        if ((cl_t->farg = (char*)
                    realloc(cl_t->farg,
                        sizeof(char) * (strlen(cl_t->farg) + strlen(tmp)))) == NULL) {
            fprintf(stderr, "%s: realloc() failure",
                    PROGNAME);

            return -1;
        }
        memmove(cl_t->farg + strlen(tmp) - 1, cl_t->farg, strlen(cl_t->farg) + 1);
        memcpy(cl_t->farg, tmp, strlen(tmp));
    }

    if (check_biff_file_stat(cl_t->farg) != 0)
        return -2;

    /* setting default exec command */
    if (cl_t->cflag == 0)
        cl_t->carg = DEFAULT_EXEC;

    /* str to array */
    if (parse_cmdline(cl_t->carg, *&cmd, *&start) < 0)
        return -3;

    return 0;
}

int monitor(clbiff_t* cl_t, cmd_t* cmd, polyaness_t* pt)
{
    struct  stat    stat_now,
                    stat_ago;

    while (hflag == 0) {
        if (stat(cl_t->farg, &stat_now) != 0) {
            print_msg(2, stderr, "stat()failure\n");
            release(cl_t, cmd, pt);

            return errno;
        }
#ifndef WITH_USLEEP
        sleep(cl_t->iarg);
#else
        usleep(cl_t->iarg);
#endif
        if (stat(cl_t->farg, &stat_ago) != 0) {
            print_msg(2, stderr, "stat()failure\n");
            release(cl_t, cmd, pt);

            return errno;
        }
        if (stat_now.st_mtime != stat_ago.st_mtime)
            exec_cmd(cmd, STDIN_FILENO);
    }

    /* interrupt handling */
    if (cl_t->vflag)
        fprintf(stdout, "\n%s[%d]: exiting on signal %d\n",
                PROGNAME, getpid(), hflag);

    /* release memory */
    release(cl_t, cmd, pt);

    return 0;
}

void catch_signal(int sig)
{
    hflag = sig;    /* break monitor loop */
}

void release(clbiff_t* cl_t, cmd_t* cmd, polyaness_t* pt)
{
    if (pt != NULL)
        release_polyaness(pt);

    if (cl_t->farg != NULL) {
        free(cl_t->farg);
        cl_t->farg = NULL;
    }

    if (cmd != NULL)
        release_cmd_t(cmd);

    return;
}
