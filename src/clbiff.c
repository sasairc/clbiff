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

    polyaness_t*    pt      = NULL;

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
    if ((ret = init(&cl_t)) < 0)
        return 3;

    /* verbose message */
    if (cl_t.vflag == 1)
        print_start_msg(&cl_t);

    /* prevention zombie process */
    handl_zombie_proc();

    /* do main loop */
    return monitor(&cl_t, pt);
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

int init(clbiff_t* cl_t)
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

    if (check_file_stat(cl_t->farg) != 0)
        return -2;

    /* setting default exec command */
    if (cl_t->cflag == 0)
        cl_t->carg = DEFAULT_EXEC;

    /* str to array */
    if ((cl_t->args = split_args(cl_t->carg)) == NULL)
        return -3;

    return 0;
}

char*** split_args(char* str)
{
    int     i       = 0,
            head    = 0,
            tail    = 0,
            cnt     = 0;

    char*   tmp     = NULL,
        *** args    = NULL;

    while (str[i] != '\0') {
        if (str[i] == '|')
            cnt++;
        i++;
    }
    if ((args = (char***)
                malloc(sizeof(char**) * (cnt + 2))) == NULL)
        return NULL;

    cnt = head = tail = 0;
    while (1) {
        if (str[head] == '|' || str[head] == '\0') {
            if ((tmp = (char*)
                        malloc(sizeof(char) * (head - tail))) == NULL) {
                fprintf(stderr, "%s: malloc() failure\n",
                        PROGNAME);

                goto ERR;
            }

            memcpy(tmp, str + tail, head - tail);
            tmp[head - tail] = '\0';
            trim(tmp);
            args[cnt] = str_to_args(tmp);
            free(tmp);
            cnt++;
            if (str[head] == '\0')
                break;
            head++;
            tail = head;
        } else {
            head++;
        }
    }
    args[cnt] = NULL;

    return args;

ERR:

    if (args != NULL) {
        while (args[cnt] != NULL) {
            i = 0;
            while (args[cnt][i] != NULL) {
                free(args[cnt][i]);
                i++;
            }
            free(args[cnt]);
            cnt--;
        }
        free(args);
    }

    return NULL;
}

int monitor(clbiff_t* cl_t, polyaness_t* pt)
{
    int             e_errno = 0;

    struct  stat    stat_now,
                    stat_ago;

    while (hflag == 0) {
        if (stat(cl_t->farg, &stat_now) != 0) {
            print_msg(2, stderr, "stat()failure\n");
            release(cl_t, pt);

            return errno;
        }
#ifndef WITH_USLEEP
        sleep(cl_t->iarg);
#else
        usleep(cl_t->iarg);
#endif
        if (stat(cl_t->farg, &stat_ago) != 0) {
            print_msg(2, stderr, "stat()failure\n");
            release(cl_t, pt);

            return errno;
        }
        if (stat_now.st_mtime != stat_ago.st_mtime)
            if ((e_errno = exec_cmd(cl_t->args, 0, cl_t->vflag, STDIN_FILENO)) > 0)
                return errno;
    }

    /* interrupt handling */
    if (cl_t->vflag)
        fprintf(stdout, "\n%s[%d]: exiting on signal %d\n",
                PROGNAME, getpid(), hflag);

    /* release memory */
    release(cl_t, pt);

    return 0;
}

void redirect(int oldfd, int newfd)
{
    if (oldfd != newfd)
        if (dup2(oldfd, newfd) != -1)
            close(oldfd);

    return;
}

int exec_cmd(char*** args, int pos, int vflag, int in_fd)
{
    /*
     * single
     */
    if (args[pos + 1] == NULL && pos == 0) {
        switch (fork()) {
            case    -1:
                print_msg(2, stderr, "fork() on exec_cmd() failure\n");

                return errno;
            case    0:
                print_msg(4, stdout, "exec ", args[0][0], "\n");
                execvp(args[0][0], args[0]);
                print_msg(4, stderr, "exec ", args[0][0], " failure\n");

                exit(errno);
            default:
                return 0;
        }
    }

    /*
     * recursive
     */
    if (args[pos + 1] == NULL) {
        redirect(in_fd, STDIN_FILENO);
        /* exec proccess */
        execvp(args[pos][0], args[pos]);
        /* do error */
        print_msg(4, stderr, "exec ", args[pos], " failure\n");

        exit(errno);
    }

    switch (fork()) {
        case    -1:
            print_msg(2, stderr, "fork() on exec_cmd() failure\n");

            return errno;
        case    0:
            break;
        default:
            return 0;
    }

    int fd[2]   = {0};

    /* create pipe */
    if (pipe(fd) < 0) {
        print_msg(2, stderr, "pipe() on exec_cmd() failure\n");

        return errno;
    }

    switch (fork()) {
        case    -1:
            print_msg(2, stderr, "fork() on exec_cmd() failure\n");

            return errno;
        case    0:
            close(fd[0]);
            redirect(in_fd, STDIN_FILENO);
            redirect(fd[1], STDOUT_FILENO);

            if (vflag == 1)
                print_msg(4, stdout, "exec ", args[pos][0], "\n");

            /* exec proccess */
            execvp(args[pos][0], args[pos]);
            /* do error */
            print_msg(4, stderr, "exec ", args[pos], " failure\n");

            exit(errno);
        default:
            close(fd[1]);
            close(in_fd);
            exec_cmd(args, pos + 1, vflag, fd[0]);
    }

    exit(0);
}

void catch_signal(int sig)
{
    hflag = sig;    /* break monitor loop */
}

void release(clbiff_t* cl_t, polyaness_t* pt)
{
    int i   = 0,
        j   = 0;

    if (pt != NULL)
        release_polyaness(pt);

    if (cl_t->farg != NULL) {
        free(cl_t->farg);
        cl_t->farg = NULL;
    }

    if (cl_t->args != NULL) {
        while (cl_t->args[i] != NULL) {
            j = 0;
            while (cl_t->args[i][j] != NULL) {
                free(cl_t->args[i][j]);
                cl_t->args[i][j] = NULL;
                j++;
            }
            free(cl_t->args[i]);
            i++;
        }
        free(cl_t->args);
    }

    return;
}
