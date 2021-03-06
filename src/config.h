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
#define PATCHLEVEL      10
#define SUBLEVEL        0
#define EXTRAVERSION    ""

#define AUTHOR          "sasairc"
#define MAIL_TO         "sasairc@ssiserver.moe.hm"

#define DEFAULT_TMSEC   30
#define DEFAULT_EXEC    "eject"

#define INBOX_MBOX      "/inbox"    /* support mh */
#define INBOX_MDIR      "/new"      /* support maildir */

#undef  WITH_ADD_INFO
#undef  WITH_USLEEP
#undef  DEBUG

/* CONFIG_H */
#endif
