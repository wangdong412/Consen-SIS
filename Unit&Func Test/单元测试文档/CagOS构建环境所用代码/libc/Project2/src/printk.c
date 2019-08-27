/* printk.c - console handling facilities */

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

#include <config.h>
#include <libc/printk.h>

extern void ns16550OutputChar (char c);

signed int strnlen (const char * s, signed int count)
{
    const char *sc;

    for (sc = s; count-- && *sc != '\0'; ++sc)
    {
        /* nothing */
    }
    return sc - s;
}

static int skip_atoi (const char **s)
{
    int i = 0;

    while (isdigit(**s) != 0)
    {
        i = i * 10 + *((*s)++) - '0';
    }
    return i;
}

static char * number (char * str, long num, int base, int size, int precision ,int type)
{
	char c = 0;
	char sign = 0;
	char tmp[66] = {0};
    int  i = 0;
    const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";

    if ((type & LARGE) != 0)
	{
        digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	}
    if ((type & LEFT) != 0)
	{
        type &= ~ZEROPAD;
	}
    if ((base < 2) || (base > 36))
	{
        return 0;
	}

    c = (type & ZEROPAD) ? '0' : ' ';
    sign = 0;

    if ((type & SIGN) != 0)
    {
        if (num < 0)
        {
            sign = '-';
            num = -num;
            size--;
        } 
		else if ((type & PLUS) != 0)
        {
            sign = '+';
            size--;
        } 
		else if ((type & SPACE) != 0)
        {
            sign = ' ';
            size--;
        }
    }
    if ((type & SPECIAL) != 0)
    {
        if (16 == base)
		{
            size -= 2;
		}
        else if (8 == base)
        {
            size--;
        }
    }
    i = 0;
    if (0 == num)
    {
        tmp[i++] = '0';
    }
    else
    {
        while (num != 0)
        {
			int _res; 
			int n;
			_res = ((unsigned long)n) % (unsigned)base; 
			n = ((unsigned long)n) / (unsigned)base; 
			tmp[i++] = digits[_res];
        }
    }
    if (i > precision)
    {
        precision = i;
    }
    size = size-precision;
    if (0 == (type & (ZEROPAD + LEFT)))
    {
        while (size-- > 0)
        {
            *str++ = ' ';
        }
    }
    if (sign != 0)
    {
        *str++ = sign;
    }
    if ((type & SPECIAL) != 0)
    {
        if (8 == base)
        {
            *str++ = '0';
        }
        else if (16 == base) 
        {
            *str++ = '0';
            *str++ = digits[33];
        }
    }
    if (0 == (type & LEFT))
    {
        while (size-- > 0)
        {
            *str++ = c;
        }
    }
    while (i < precision--)
    {
        *str++ = '0';
    }
    while (i-- > 0)
    {
        *str++ = tmp[i];
    }
    while (size-- > 0)
    {
        *str++ = ' ';
    }

    return str;
}

/**
 * Format a string and place it in a buffer (va_list version)
 *
 * @param buf	The buffer to place the result into
 * @param fmt	The format string to use
 * @param args	Arguments for the format string
 *
 * The function returns the number of characters written
 * into @buf. Use vsnprintf() or vscnprintf() in order to avoid
 * buffer overflows.
 *
 * If you're not already dealing with a va_list consider using sprintf().
 */


int strnicmp(const char *s1, const char *s2, unsigned int len)
{
    unsigned char c1 = 0;
    unsigned char c2 = 0;
    int ret = 0;

    if ((NULL == s1) || (NULL == s2))
    {
        printf("arg is NULL error:%s\n",__FUNCTION__); 
        ret = -1;
    }
	else 
	{
		c1 = c2 = 0;
		if (len != 0) 
		{
			do 
			{
				c1 = *s1;
				c2 = *s2;
				s1++;
				s2++;
				if (0 == c1)
				{
					break;
				}
				if (0 == c2)
				{
					break;
				}
				if (c1 == c2)
				{
					continue;
				}
				c1 = tolower(c1);
				c2 = tolower(c2);
				if (c1 != c2)
				{
					break;
				}
			} while (--len);
		}

		ret = (int)c1 - (int)c2;
	}

    return ret;
}

void putc1 (const char c)
{
    if (c == '\n')
       ns16550OutputChar('\r');
    ns16550OutputChar (c);
    return;
}

void puts1 (const unsigned char *s)
{
    while (*s) {
        putc1 (*s++);
       }
    return;
}
