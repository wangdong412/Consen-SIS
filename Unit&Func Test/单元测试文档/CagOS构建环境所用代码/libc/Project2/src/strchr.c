/* strchr.c - search string for character, string */

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

/******************************************************************************
*
* strchr - find the first occurrence of a character in a string (ANSI)
*
* This routine finds the first occurrence of character <c>
* in string <s>.  The terminating null is considered to be part of the string.
*
* INCLUDE FILES: string.h
*
* RETURNS:
* The address of the located character, or NULL if the character is not found.
*/

char * strchr
    (
    const char * s,         /* string in which to search */
    int 	 c          /* character to find in string */
    )
    {
    char *r = CHAR_FROM_CONST(s); 

    while (*r != (char) c)		/* search loop */
	{
	if (*r++ == EOS)		/* end of string */
	    return (NULL);
        }

    return (r);
    }
