/* strcat.c - concatenate one string to another, string */

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
* strcat - concatenate one string to another (ANSI)
*
* This routine appends a copy of string <append> to the end of string 
* <destination>.  The resulting string is null-terminated.
*
* INCLUDE FILES: string.h
*
* RETURNS: A pointer to <destination>.
*/

char * strcat
    (
    char *       destination, /* string to be appended to */
    const char * append       /* string to append to <destination> */
    )
    {
    char *save = destination;

    while (*destination++ != '\0')		/* find end of string */
        ;

    destination--;

    while ((*destination++ = *append++) != '\0')
	;

    return (save);
    }
