/*
 * clbiff -  simple mail notify program
 *
 * info.c
 *
 * Copyright (c) 2015 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./config.h"
#include "./info.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef WITH_USLEEP
int print_usage(void)
{
    fprintf(stdout, "\
%s %d.%d.%d%s, simple mail notify program\n\
Usage: clbiff [OPTION]...\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
\n\
  -i,  --interval=SECONDS    interval time (default %d sec)\n\
  -c,  --command=COMMAND     specifies command (default %s)\n\
  -f,  --file=file           monitored file (default $MAIL eval)\n\
  -q,  --quiet               quiet mode\n\
  -v,  --verbose             verbose mode\n\
\n\
       --help                display this help and exit\n\
       --version             output version infomation and exit\n\
\n\
Report %s bugs to %s <%s>\n\
",
        PROGNAME, VERSION, PATCHLEVEL, SUBLEVEL, EXTRAVERSION,
        DEFAULT_TMSEC, DEFAULT_EXEC,
        PROGNAME, AUTHOR, MAIL_TO);

    exit(0);
}
#else
int print_usage(void)
{
    fprintf(stdout, "\
%s %d.%d.%d%s, simple mail notify program with usleep\n\
Usage: clbiff [OPTION]...\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
\n\
  -i,  --interval=USECONDS   interval time (default %d usec)\n\
  -c,  --command=COMMAND     specifies command (default %s)\n\
  -f,  --file=file           monitored file (default $MAIL eval)\n\
  -q,  --quiet               quiet mode\n\
  -v,  --verbose             verbose mode\n\
\n\
       --help                display this help and exit\n\
       --version             output version infomation and exit\n\
\n\
Report %s bugs to %s <%s>\n\
",
        PROGNAME, VERSION, PATCHLEVEL, SUBLEVEL, EXTRAVERSION,
        DEFAULT_TMSEC, DEFAULT_EXEC,
        PROGNAME, AUTHOR, MAIL_TO);

    exit(0);
}
#endif

int print_version(void)
{
    fprintf(stdout, "%s %d.%d.%d%s (%s)\n",
            PROGNAME, VERSION, PATCHLEVEL, SUBLEVEL, EXTRAVERSION, ARCH);

    exit(0);
}
