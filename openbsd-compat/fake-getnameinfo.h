/* $Id$ */

#ifndef _FAKE_GETNAMEINFO_H
#define _FAKE_GETNAMEINFO_H

#include "config.h"

#ifndef HAVE_GETNAMEINFO
int getnameinfo(const struct sockaddr *, size_t, char *, size_t, 
    char *, size_t, int);
#endif /* !HAVE_GETNAMEINFO */

#ifndef NI_MAXSERV
# define NI_MAXSERV 32
#endif /* !NI_MAXSERV */
#ifndef NI_MAXHOST
# define NI_MAXHOST 1025
#endif /* !NI_MAXHOST */

#endif /* _FAKE_GETNAMEINFO_H */
