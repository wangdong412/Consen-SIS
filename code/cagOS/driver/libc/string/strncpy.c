/* strncpy.c - string copy, string */

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
* strncpy - copy characters from one string to another (ANSI)
*
* This routine copies <n> characters from string <s2> to string <s1>.
* If <n> is greater than the length of <s2>, nulls are added to <s1>.
* If <n> is less than or equal to the length of <s2>, the target
* string will not be null-terminated.
*
* INCLUDE FILES: string.h
*
* RETURNS: A pointer to <s1>.
*/

char *strncpy
(
    char *      s1,   	/* string to copy to */
    const char *s2,   	/* string to copy from */
    size_t      n      	/* max no. of characters to copy */
)
{
    FAST char *d = s1;

    if (n != 0) {
        while ((*d++ = *s2++) != 0) {	/* copy <s2>, checking size <n> */
            if (--n == 0)
                return (s1);
        }

        while (--n > 0)
            *d++ = EOS;			/* NULL terminate string */
    }

    return (s1);
}
