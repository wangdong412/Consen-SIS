/* memchr.c - search memory for a character, string */

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
* memchr - search a block of memory for a character (ANSI)
*
* This routine searches for the first element of an array of `unsigned char',
* beginning at the address <m> with size <n>, that equals <c> converted to
* an `unsigned char'.
*
* INCLUDE FILES: string.h
*
* RETURNS: If successful, it returns the address of the matching element;
* otherwise, it returns a null pointer.
*/

void * memchr
(
    const void * m,		/* block of memory */
    int 	 c,		/* character to search for */
    size_t 	 n		/* size of memory to search */
)
{
    uchar_t *p = (uchar_t *) CHAR_FROM_CONST(m);

    if (n != 0)
        do {
            if (*p++ == (unsigned char) c)
                return (VOID_FROM_CONST(p - 1));

        } while (--n != 0);

    return (NULL);
}
