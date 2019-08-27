/* strlen.c - file for string */

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
* strlen - determine the length of a string (ANSI)
*
* This routine returns the number of characters in <s>, not including EOS.
*
* INCLUDE FILES: string.h
*
* RETURNS: The number of non-null characters in the string.
*/

size_t strlen
    (
    const char * s        /* string */
    )
    {
    const char *save = s + 1;

    while (*s++ != EOS)
	;

    return (s - save);
    }
