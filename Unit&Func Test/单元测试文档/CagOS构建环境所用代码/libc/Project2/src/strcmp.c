/* strcmp.c - compare two strings, string */

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
* strcmp - compare two strings lexicographically (ANSI)
*
* This routine compares string <s1> to string <s2> lexicographically.
*
* INCLUDE FILES: string.h
*
* RETURNS: An integer greater than, equal to, or less than 0,
* according to whether <s1> is lexicographically
* greater than, equal to, or less than <s2>, respectively.
*/

int strcmp
    (
    const char * s1,   /* string to compare */
    const char * s2    /* string to compare <s1> to */
    )
    {
		while (*s1++ == *s2++)
		{
			if (s1[-1] == EOS)
				return (0);
		}
    return ((int)((unsigned char)s1 [-1] - (unsigned char)s2 [-1]));
    }
