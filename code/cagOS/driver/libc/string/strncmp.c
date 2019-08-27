/* strncmp.c - string compare, string */

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
* strncmp - compare the first <n> characters of two strings (ANSI)
*
* This routine compares up to <n> characters of string <s1> to string <s2>
* lexicographically.
*
* INCLUDE FILES: string.h
*
* RETURNS:
* An integer greater than, equal to, or less than 0, according to whether
* <s1> is lexicographically greater than, equal to, or less than <s2>,
* respectively.
*/

int strncmp
(
    const char * s1,           	/* string to compare */
    const char * s2,           	/* string to compare <s1> to */
    size_t       n             	/* max no. of characters to compare */
)
{
    if (n == 0)
        return (0);

    while (*s1++ == *s2++) {
        if ((s1 [-1] == EOS) || (--n == 0))
            return (0);
    }

    return ((int)((unsigned char)s1 [-1] - (unsigned char)s2 [-1]));
}
