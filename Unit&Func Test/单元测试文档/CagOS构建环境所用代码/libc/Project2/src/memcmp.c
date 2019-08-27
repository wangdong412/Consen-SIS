/* memcmp.c - memory compare file for string */

/* Copyright 1992-1993 CAG Systems, Inc. */

/*
modification history
--------------------
11jan15,Jeff created.
*/

/*
DESCRIPTION

INCLUDE FILE: string.h

SEE ALSO: American National Standard X3.159-1989

NOMANUAL
*/

#include <libc/system.h>
#include <libc/string.h>

/*******************************************************************************
*
* memcmp - compare two blocks of memory (ANSI)
*
* This routine compares successive elements from two arrays of `unsigned char',
* beginning at the addresses <s1> and <s2> (both of size <n>), until it finds
* elements that are not equal.
*
* INCLUDE FILES: string.h
*
* RETURNS:
* If all elements are equal, zero.  If elements differ and the differing
* element from <s1> is greater than the element from <s2>, the routine
* returns a positive number; otherwise, it returns a negative number.
*/

int memcmp
    (
    const void * s1,		/* array 1 */
    const void * s2,		/* array 2 */
    size_t       n		/* size of memory to compare */
    )
    {
    const unsigned char *p1;
    const unsigned char *p2;

    /* size of memory is zero */

    if (n == 0)
	return (0);

    /* compare array 2 into array 1 */

    p1 = s1;
    p2 = s2;

    while (*p1++ == *p2++)
	{
	if (--n == 0)
	    return (0);
        }

    return ((*--p1) - (*--p2));
    }
