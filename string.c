/*
 * clbiff -  simple mail notify program
 *
 * string.c
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <locale.h>

#ifdef  WITH_GLIB
#include <glib.h>
#endif

int strrep(char* src, char* haystack, char* needle)
{
    char* find = NULL;

    if (src == NULL || haystack == NULL || needle == NULL) {

        return 1;
    }

    /* seach strings */
    if ((find = strstr(src, haystack)) == NULL) {

        return 3;       /* word not found */
    }
    if (strlen(haystack) < strlen(needle)) {
        src = (char*)realloc(src, strlen(src) + strlen(needle) + 1 - strlen(haystack)); /* reallocate memory */
        /* move match word to specified location in memory */
        memmove(
            find + strlen(needle),
            find + strlen(haystack),
            strlen(src) - strlen(haystack) - (find - src) + 1
        );
        memcpy(find, haystack, strlen(needle));
    } else {
        memcpy(find, needle, strlen(needle));
        /* move match word to specified location in memory */
        if (strlen(haystack) > strlen(needle)) {
            memmove(
                find + strlen(needle),
                find + strlen(haystack),
                strlen(src) - strlen(haystack) - (find - src) + 1
            );
        }
    }

    return 0;
}

char* strlion(int argnum, ...)
{
    char*   buf     = NULL;
    char**  argmnt  = NULL;
    int     i       = 0;
    size_t  size    = 0;    /* string size */
    va_list list;           /* list of variable arguments */

    argmnt = (char**)malloc(sizeof(char*) * argnum);
    if (argmnt == NULL)
        return NULL;

    /* processing of variable arguments */
    va_start(list, argnum);
    for (i = 0; i < argnum; i++) {
        argmnt[i] = va_arg(list, char*);
    }
    va_end(list);

    /* count of string size */
    for (i = 0; i < argnum; i++) {
        size = size + strlen(argmnt[i]);
    }
    buf = (char*)malloc(sizeof(char) * (size + 1)); /* memory allocation */

    for (i = 0; i < argnum; i++) {
        strcat(buf, argmnt[i]);     /* string concatenate */
    }
    free(argmnt);

    return buf;
}

#ifdef  WITH_GLIB
int mbstrlen(char* src)
{
    int         i    = 0;
    int         ch   = 0;
    int         len  = 0;
    gunichar*   cpoints;

    setlocale(LC_CTYPE, LOCALE);            /* set locale (string.h) */

    while (src[i] != '\0') {
        ch = mblen(&src[i], MB_CUR_MAX);    /* get string length */
        if (ch > 1) {
            cpoints = g_utf8_to_ucs4_fast(&src[i], sizeof(src[i]), NULL);   /* get unicode code point */

            /*
             * multi byte
             * true : hankaku kana
             * false: other
             */
            if (cpoints[0] >= 0xff65 && cpoints[0] <= 0xff9f) {
                len++;
            } else {
                len += 2;
            }

            g_free(cpoints);
        } else {
            len++;                          /* ascii */
        }
        i += ch;                            /* seek offset */
    }

    return len;
}
#else
int mbstrlen(char* src)
{
    int i = 0;
    int ch = 0;
    int len = 0;

    setlocale(LC_CTYPE, LOCALE);            /* set locale (string.h) */

    while (src[i] != '\0') {
        ch = mblen(&src[i], MB_CUR_MAX);    /* get string length */
        if (ch > 1) {
            len += 2;                       /* multi byte */
        } else {
            len++;                          /* ascii */
        }
        i += ch;                            /* seek offset */
    }

    return len;
}
#endif

int strunesc(char* src)
{
    int i = 0;
    int count = 0;

    while (src[i] != '\0') {
        if (src[i] == '\t' || src[i] == '\b') {
            src[i] = ' ';
            count++;
        }
        i++;
    }

    return count;
}
            
int strmax(int val, char** src)
{
    int i;
    int len = 0;
    int max = 0;

    for (i = 0; i < val; i++) {
        len = mbstrlen(src[i]);
        if (max < len)  max = len;
    }

    return max;
}

int strlftonull(char* str)
{
    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';

        return 1;
    }

    return 0;
}

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