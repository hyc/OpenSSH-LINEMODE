/* $Id$ */

#ifndef _BASENAME_H 
#define _BASENAME_H
#include "config.h"

#if !defined(HAVE_BASENAME)

char *getcwd(char *pt, size_t size);

#endif /* !defined(HAVE_BASENAME) */
#endif /* _BASENAME_H */
