/* $Id$ */

#ifndef _GETOPT_H
#define _GETOPT_H

#include "config.h"

#ifndef HAVE_GETOPT_H

int BSDgetopt(int argc, char * const *argv, const char *opts);

#endif

#endif /* _GETOPT_H */
