/* vi: set sw=4 ts=4: */
/*
 * strings implementation for busybox
 *
 * Copyright (c) 1980, 1987
 *	The Regents of the University of California.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Original copyright notice is retained at the end of this file.
 *
 * Modified for BusyBox by Erik Andersen <andersee@debian.org>
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <getopt.h> 
#include <unistd.h>
#include <ctype.h>
#include "busybox.h"

#define ISSTR(ch)	(isprint(ch) || ch == '\t')

int strings_main(int argc, char **argv) 
{
	extern char *optarg;
	extern int optind;
	int ch, cnt;
	int exitcode;
	int oflg, fflg;
	char *file;
	size_t foff, minlen;
	unsigned char *bfr, *C;


	exitcode = fflg = oflg = 0;
	minlen = -1;
	while ((ch = getopt(argc, argv, "an:of")) > 0)
		switch(ch) {
			case '-':
				break;
			case 'f':
				fflg = 1;
				break;
			case 'n':
				minlen = atoi(optarg);
				break;
			case 'o':
				oflg = 1;
				break;
			default:
				show_usage();
		}
	argc -= optind;
	argv += optind;

	if (minlen == -1)
		minlen = 4;

	bfr = xmalloc(minlen);
	bfr[minlen] = '\0';
	file = "stdin";
	do {
		if (*argv) {
			fprintf(stderr, "opening '%s'\n", *argv);
			file = *argv++;
			if (!freopen(file, "r", stdin)) {
				perror_msg("%s", file);
				exitcode = EXIT_FAILURE;
				continue;
			}
		}
		foff = 0;

		for (cnt = 0; (ch = getchar()) != EOF;) {
			if (ISSTR(ch)) {
				if (!cnt)
					C = bfr;
				*C++ = ch;
				if (++cnt < minlen)
					continue;
				if (fflg)
					printf("%s:", file);
				if (oflg)
					printf("%07ld %s", (long)(foff - minlen), (char *)bfr);
				else
					printf("%s", bfr);
				while ((ch = getchar()) != EOF && ISSTR(ch))
					putchar((char)ch);
				putchar('\n');
			}
			cnt = 0;
		}
	} while (*argv);
	exit(exitcode);
}

/*
 * Copyright (c) 1980, 1987
 *	The Regents of the University of California.  All rights reserved.
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
 * 3. <BSD Advertising Clause omitted per the July 22, 1999 licensing change 
 *		ftp://ftp.cs.berkeley.edu/pub/4bsd/README.Impt.License.Change> 
 *
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
