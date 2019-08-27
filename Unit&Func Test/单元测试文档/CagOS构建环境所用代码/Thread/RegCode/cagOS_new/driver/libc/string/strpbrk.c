/* strpbrk.c - string search, string */

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
* strpbrk - find the first occurrence in a string of a character from a given set (ANSI)
*
* This routine locates the first occurrence in string <s1> of any character
* from string <s2>.
*
* INCLUDE FILES: string.h
*
* RETURNS:
* A pointer to the character found in <s1>, or
* NULL if no character from <s2> occurs in <s1>.
*
* SEE ALSO: strcspn()
*/

char * strpbrk
(
    const char * s1,       /* string to search */
    const char * s2        /* set of characters to look for in <s1> */
)
{
    char *scanp;
    int   c;
    int   sc;

    while ((c = *s1++) != 0) {		/* wait until end of string */
        /* loop, searching for character */

        for (scanp = CHAR_FROM_CONST(s2); (sc = *scanp++) != 0;) {
            if (sc == c)
                return (CHAR_FROM_CONST(s1 - 1));
        }
    }

    return (NULL);
}
