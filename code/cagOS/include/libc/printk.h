/* printk.h - serial handling facilities */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:
 * Copyright (c) 2013, 2014 CAG Systems, Inc.
 *------------------------------------------------------------
 *
 *    @@@@     @@@      @@@@@       ####      ######
 *  @@       @@  @@    @@         ##    ##   ##
 * @@       @@    @@  @@   @@@@  ##      ## ######
 * @@      @@@@@@@@@@ @@    @@    ##    ##      ##
 *  @@@@@ @@@      @@@  @@@@@@      ####  #######
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable CAG license agreement.
 *
 * 1 tab == 4 spaces!
 *------------------------------------------------------------
 * modification history
 * --------------------
 * 03jun15,Jeff created
 *
 * DESCRIPTION
 * This file contains
 */

#include "stdarg.h"
#include "ctype.h"

#ifndef _PRINTK_H
#define _PRINTK_H

//******** printf referrend Function declarations begin **********
void          printf (const char *fmt, ...);
int           strnicmp(const char *s1, const char *s2, unsigned int len);
unsigned int  strlen (const char * s);
static int    skip_atoi (const char **s);
signed        strnlen (const char * s, signed int count);
static char * number (char * str, long num, int base, int size, int precision ,int type);
int 	      vsprintf (char *buf, const char *fmt, va_list args);
char        * strcpy(char *to, const char *from);
void          sprintf(char * buf,const char *fmt,...);

#define do_div(n,base) ({ \
	int _res; \
	_res = ((unsigned long) n) % (unsigned) base; \
	n = ((unsigned long) n) / (unsigned) base; \
	_res; \
})

#define ZEROPAD	1		/* pad with zero */
#define SIGN   	2		/* unsigned/signed long */
#define PLUS   	4		/* show plus */
#define SPACE  	8		/* space if plus */
#define LEFT  	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define LARGE  	64		/* use 'ABCDEF' instead of 'abcdef' */
#define SMALL  	64		/* use 'abcdef' instead of 'ABCDEF' */

#ifndef NULL
#define NULL ((void *)0)
#endif

#define uint unsigned int


#define CFG_CBSIZE      256     /* Console I/O Buffer Size  */
#define CFG_PBSIZE (CFG_CBSIZE + sizeof(CFG_PROMPT) + 16) /* Print Buffer Size  */
#define CFG_PROMPT      "=>"        /* Monitor Command Prompt   */

#endif

