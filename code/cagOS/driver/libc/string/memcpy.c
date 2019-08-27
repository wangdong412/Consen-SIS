/* memcpy.c - memory copy file for string */

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
* memcpy - copy memory from one location to another (ANSI)
*
* This routine copies <size> characters from the object pointed
* to by <source> into the object pointed to by <destination>. If copying
* takes place between objects that overlap, the behavior is undefined.
*
* INCLUDE FILES: string.h
*
* RETURNS: A pointer to <destination>.
*/

void * memcpy
(
    void *       destination,   /* destination of copy */
    const void * source,        /* source of copy */
    size_t       size           /* size of memory to copy */
)
{
    bcopy ((char *) source, (char *) destination, (size_t) size);
    return (destination);
}

