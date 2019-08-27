/* memmove.c - memory move file for string */

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
* memmove - copy memory from one location to another (ANSI)
*
* This routine copies <size> characters from the memory location <source> to
* the location <destination>.  It ensures that the memory is not corrupted
* even if <source> and <destination> overlap.
*
* INCLUDE FILES: string.h
*
* RETURNS: A pointer to <destination>.
*/

void * memmove
(
    void *	 destination,	/* destination of copy */
    const void * source,	/* source of copy */
    size_t 	 size		/* size of memory to copy */
)
{
    bcopy ((char *)source, (char *)destination, size);
    return (destination);
}
