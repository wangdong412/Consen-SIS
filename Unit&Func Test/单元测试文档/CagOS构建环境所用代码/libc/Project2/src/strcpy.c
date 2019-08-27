/* strcpy.c - string copy, string */

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
* strcpy - copy one string to another (ANSI)
*
* This routine copies string <s2> (including EOS) to string <s1>.
*
* INCLUDE FILES: string.h
*
* RETURNS: A pointer to <s1>.
*/

char * strcpy
    (
    char *       s1,	/* string to copy to */
    const char * s2	/* string to copy from */
    )
    {
    char *save = s1;

    while ((*s1++ = *s2++) != EOS)
	;

    return (save);
    }

