/* puts.c - put a string on standard output */

/* Copyright 1992-1993 CAG Systems, Inc. */

/*
modification history
--------------------
11jan15,Jeff created.
*/

#include <libc/stdio.h>

/******************************************************************************
*
* puts - put a string on standard output
*
* This routine puts a string on standard output.
*
* RETURNS: a constant, non-negative integer (0 is returned here).
*
* ERRNO: N/A
*
* \NOMANUAL
*/

int puts
(
    const char *s
)
{
    const char *t = s;
    while (*t != '\0') {
        (void)putchar(*t);
        t++;
    }
    return 0;
}
