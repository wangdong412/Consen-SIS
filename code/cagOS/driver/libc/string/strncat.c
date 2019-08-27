/* strncat.c - file for string */

/* Copyright 1992-1993 CAG Systems, Inc. */

/*
modification history
--------------------
11jan15,Jeff created.
*/

/*
DESCRIPTION

INCLUDE FILES: string.h

SEE ALSO: American National Standard X3.159-1989

NOMANUAL
*/

#include <libc/system.h>
#include <libc/string.h>

/*******************************************************************************
*
* strncat - concatenate characters from one string to another (ANSI)
*
* This routine appends up to <n> characters from string <src> to the
* end of string <dst>.
*
* INCLUDE FILES: string.h
*
* RETURNS: A pointer to the null-terminated string <s1>.
*/

char * strncat
(
    char *	 dst,  	/* string to append to */
    const char * src,   /* string to append */
    size_t	 n     	/* max no. of characters to append */
)
{
    if (n != 0) {
        char *d = dst;

        while (*d++ != EOS)			/* find end of string */
            ;

        d--;					/* rewind back of EOS */

        while (((*d++ = *src++) != EOS) && (--n > 0))
            ;

        if (n == 0)
            *d = EOS;				/* NULL terminate string */
    }

    return (dst);
}
