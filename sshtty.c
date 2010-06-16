/* $OpenBSD: sshtty.c,v 1.14 2010/01/09 05:04:24 djm Exp $ */
/*
 * Author: Tatu Ylonen <ylo@cs.hut.fi>
 * Copyright (c) 1995 Tatu Ylonen <ylo@cs.hut.fi>, Espoo, Finland
 *                    All rights reserved
 *
 * As far as I am concerned, the code I have written for this software
 * can be used freely for any purpose.  Any derived versions of this
 * software must be clearly marked as such, and if the derived work is
 * incompatible with the protocol description in the RFC file, it must be
 * called by a name other than "ssh" or "Secure Shell".
 */
/*
 * Copyright (c) 2001 Markus Friedl.  All rights reserved.
 * Copyright (c) 2001 Kevin Steves.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "includes.h"

#include <sys/types.h>
#include <stdio.h>
#include <termios.h>
#include <pwd.h>

#include "sshpty.h"

static struct termios _saved_tio;
static struct termios _orig_tio;
static int _in_raw_mode = 0;
static int _saved_orig = 0;
static int _cooked = 0;		/* server supports linemode */
static int _is_remote = 0;	/* we're inside a linemode session */

struct termios *
get_saved_tio(void)
{
	return _in_raw_mode ? &_saved_tio : NULL;
}

void
leave_raw_mode(int quiet)
{
	if (!_in_raw_mode || _is_remote)
		return;
	if (tcsetattr(fileno(stdin), TCSADRAIN, &_orig_tio) == -1) {
		if (!quiet)
			perror("tcsetattr");
	} else
		_in_raw_mode = 0;
}

void
enter_raw_mode(int quiet)
{
	struct termios tio;

	if (_cooked || _is_remote) return;

	if (tcgetattr(fileno(stdin), &tio) == -1) {
		if (!quiet)
			perror("tcgetattr");
		return;
	}
	_saved_tio = tio;
	if (!_saved_orig) {
		_saved_orig = 1;
		_orig_tio = _saved_tio;
	}
	if (tio.c_lflag & EXTPROC) {
		_is_remote = 1;
		return;
	}
	tio.c_iflag |= IGNPAR;
	tio.c_iflag &= ~(ISTRIP | INLCR | IGNCR | ICRNL | IXON | IXANY | IXOFF);
#ifdef IUCLC
	tio.c_iflag &= ~IUCLC;
#endif
	tio.c_lflag &= ~(ISIG | ICANON | ECHO | ECHOE | ECHOK | ECHONL);
#ifdef IEXTEN
	tio.c_lflag &= ~IEXTEN;
#endif
	tio.c_oflag &= ~OPOST;
	tio.c_cc[VMIN] = 1;
	tio.c_cc[VTIME] = 0;
	if (tcsetattr(fileno(stdin), TCSADRAIN, &tio) == -1) {
		if (!quiet)
			perror("tcsetattr");
	} else
		_in_raw_mode = 1;
}

void
cooked_mode()
{
	if (!_is_remote)
		_cooked = 1;
}

int
is_cooked()
{
	return _cooked && ((_saved_tio.c_lflag & (ECHO|ICANON)) == (ECHO|ICANON));
}
