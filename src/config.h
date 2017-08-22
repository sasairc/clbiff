/*
 * clbiff -  simple mail notify program
 *
 * config.h
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#ifndef CONFIG_H
#define CONFIG_H

#define PROGNAME        "clbiff"
#define VERSION         0
#define PATCHLEVEL      11
#define SUBLEVEL        0
#define EXTRAVERSION    "-devel"

#define AUTHOR          "sasairc"
#define MAIL_TO         "sasairc@ssiserver.moe.hm"

#ifndef DEFAULT_TMSEC
#define DEFAULT_TMSEC   30
/* DEFAULT_TMSEC */
#endif
#ifndef DEFAULT_EXEC
#define DEFAULT_EXEC    "eject"
/* DEFAULT_EXEC */
#endif
#ifndef INBOX_MBOX
#define INBOX_MBOX      "/inbox"    /* support mh */
/* INBOX_MBOX */
#endif
#ifndef INBOX_MDIR
#define INBOX_MDIR      "/new"      /* support maildir */
/* INBOX_MDIR */
#endif

#undef  DEBUG

/* CONFIG_H */
#endif
