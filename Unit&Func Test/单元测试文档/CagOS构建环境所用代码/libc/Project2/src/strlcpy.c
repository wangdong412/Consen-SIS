/* strlcpy - safe version of strncpy */

/* Copyright 1992-1993 CAG Systems, Inc. */

/*
modification history
--------------------
11jan15,Jeff created.
*/

/*	$OpenBSD: strlcpy.c,v 1.8 2003/06/17 21:56:24 millert Exp $	*/

/*
 * Copyright (c) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char *rcsid = "$OpenBSD: strlcpy.c,v 1.8 2003/06/17 21:56:24 millert Exp $";
#endif /* LIBC_SCCS and not lint */

#include <libc/system.h>
#include <libc/string.h>

/*******************************************************************************
*
* strcpy - copy one string to another, safely
*
* Copy src to string dst of size siz.  At most siz-1 characters
* will be copied.  Always NUL terminates (unless siz == 0).
*
* INCLUDE FILES: string.h
*
* RETURNS:
* strlen(src); if retval >= siz, truncation occurred.
*/

size_t strlcpy
    (
    char *		dst,	/* pointer to destination */
    const char *	src,	/* pointer to source */
    size_t		siz	/* number of bytes to copy */
    )
    {
    register char *d = dst;
    register const char *s = src;
    register size_t n = siz;

    /* Copy as many bytes as will fit */

    if (n != 0 && --n != 0)
	{
	do
	    {
	    if ((*d++ = *s++) == 0)
		break;
	    } while (--n != 0);
	}

    /* Not enough room in dst, add NUL and traverse rest of src */

    if (n == 0)
	{
	if (siz != 0)
	    *d = '\0';		/* NUL-terminate dst */
	while (*s++)
	    ;
	}

    return (s - src - 1);	/* count does not include NUL */
    }
