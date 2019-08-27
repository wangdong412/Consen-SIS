/* memset.c - set a block of memory, string */

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
* memset - set a block of memory (ANSI)
*
* This routine stores <c> converted to an `unsigned char' in each of the
* elements of the array of `unsigned char' beginning at <m>, with size <size>.
*
* INCLUDE FILES: string.h
*
* RETURNS: A pointer to <m>.
*/

void * memset
(
    void * m,                   /* block of memory */
    int    c,                   /* character to store */
    size_t size                 /* size of memory */
)
{
    bfill ((char *) m, size, c);
    return (m);
}
