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

    for (sc = s; count-- && *sc != '\0'; ++sc) {
        /* nothing */
    }
    return sc - s;
}

static int skip_atoi (const char **s)
{
    int i = 0;

    while (isdigit(**s) != 0) {
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

    if ((type & LARGE) != 0) {
        digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    if ((type & LEFT) != 0) {
        type &= ~ZEROPAD;
    }
    if ((base < 2) || (base > 36)) {
        return 0;
    }

    c = (type & ZEROPAD) ? '0' : ' ';
    sign = 0;

    if ((type & SIGN) != 0) {
        if (num < 0) {
            sign = '-';
            num = -num;
            size--;
        } else if ((type & PLUS) != 0) {
            sign = '+';
            size--;
        } else if ((type & SPACE) != 0) {
            sign = ' ';
            size--;
        }
    }
    if ((type & SPECIAL) != 0) {
        if (16 == base) {
            size -= 2;
        } else if (8 == base) {
            size--;
        }
    }
    i = 0;
    if (0 == num) {
        tmp[i++] = '0';
    } else {
        while (num != 0) {
            tmp[i++] = digits[do_div(num,base)];
        }
    }
    if (i > precision) {
        precision = i;
    }
    size -= precision;
    if (0 == (type & (ZEROPAD + LEFT))) {
        while (size-- > 0) {
            *str++ = ' ';
        }
    }
    if (sign != 0) {
        *str++ = sign;
    }
    if ((type & SPECIAL) != 0) {
        if (8 == base) {
            *str++ = '0';
        } else if (16 == base) {
            *str++ = '0';
            *str++ = digits[33];
        }
    }
    if (0 == (type & LEFT)) {
        while (size-- > 0) {
            *str++ = c;
        }
    }
    while (i < precision--) {
        *str++ = '0';
    }
    while (i-- > 0) {
        *str++ = tmp[i];
    }
    while (size-- > 0) {
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

int vsprintf (char *buf, const char *fmt, va_list args)
{
    int len;
    int i;
    int flags;		    // flags to number()
    int base;
    int field_width;	//width of output field
    int precision;		//min. # of digits for integers; max number of chars for from string
    int qualifier;		//'h', 'l', or 'q' for integer fields
    char * str;
    const char *s;
    unsigned long num;

    for (str = buf; *fmt; ++fmt) {
        if (*fmt != '%') {
            *str++ = *fmt;
            continue;
        }

        flags = 0;     //process flags

repeat:
        ++fmt;		   //this also skips first '%'
        switch (*fmt) {
        case '-':
            flags |= LEFT;
            goto repeat;
        case '+':
            flags |= PLUS;
            goto repeat;
        case ' ':
            flags |= SPACE;
            goto repeat;
        case '#':
            flags |= SPECIAL;
            goto repeat;
        case '0':
            flags |= ZEROPAD;
            goto repeat;
        }

        field_width = -1;  //get field width
        if (isdigit(*fmt) != 0) {

            field_width = skip_atoi(&fmt);
        } else if ('*' == *fmt) {
            ++fmt;
            field_width = va_arg(args, int); //it's the next argument
            if (field_width < 0) {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

        precision = -1;      //get the precision
        if ('.' == *fmt) {
            ++fmt;
            if (isdigit(*fmt)) {
                precision = skip_atoi(&fmt);
            } else if ('*' == *fmt) {
                ++fmt;
                precision = va_arg(args, int); //it's the next argument
            }
            if (precision < 0) {
                precision = 0;
            }
        }

        qualifier = -1;         //get the conversion qualifier
        if (('h' == *fmt) || ('l' == *fmt) || ('q' == *fmt)) {
            qualifier = *fmt;
            ++fmt;
        }

        base = 10;          //default base
        switch (*fmt) {
        case 'c': {
            if (0 == (flags & LEFT)) {
                while (--field_width > 0) {
                    *str++ = ' ';
                }
            }
            *str++ = (unsigned char) va_arg(args, int);
            while (--field_width > 0) {
                *str++ = ' ';
            }
            continue;
        }
        case 's': {
            s = va_arg(args, char *);
            if (!s) {
                s = "<NULL>";
            }
            len = strnlen(s, precision);
            if (0 == (flags & LEFT)) {
                while (len < field_width--) {
                    *str++ = ' ';
                }
            }
            for (i = 0; i < len; ++i) {
                *str++ = *s++;
            }
            while (len < field_width--) {
                *str++ = ' ';
            }
            continue;
        }
        case 'p': {
            if (-1 == field_width) {
                field_width = 2 * sizeof(void *);
                flags |= ZEROPAD;
            }
            str = number(str, (unsigned long)va_arg(args, void *), 16, field_width, precision, flags);
            continue;
        }
        case 'n': {
            if ('l' == qualifier) {
                long * ip = va_arg(args, long *);
                *ip = (str - buf);
            } else {
                int * ip = va_arg(args, int *);
                *ip = (str - buf);
            }
            continue;
        }
        case '%': {
            *str++ = '%';
            continue;
        }

        case 'o': { //integer number formats - set up the flags and "break"
            base = 8;
            break;
        }

        case 'X': {
            flags |= LARGE;
        }
        case 'x': {
            base = 16;
            break;
        }
        case 'd':
        case 'i': {
            flags |= SIGN;
        }
        case 'u': {
            break;
        }
        default: {
            *str++ = '%';
            if (0 == *fmt) {
                *str++ = *fmt;
            } else {
                --fmt;
            }
            continue;
        }
        }//end switch

#ifdef CFG_64BIT_VSPRINTF
        if ('q' == qualifier) { /* "quad" for 64 bit variables */
            num = va_arg(args, unsigned long long);
        } else {
            /* nothing */
        }
#endif
        if ('l' == qualifier) {
            num = va_arg(args, unsigned long);
        } else if ('h' == qualifier) {
            num = (unsigned short) va_arg(args, int);
            if (0 == (flags & SIGN)) {
                num = (short) num;
            }
        } else if (0 == (flags & SIGN)) {
            num = va_arg(args, int);
        } else {
            num = va_arg(args, unsigned int);
        }

        str = number(str, num, base, field_width, precision, flags);

    }//end for
    *str = '\0';

    return str-buf;
}

void sprintf (char * buf,const char *fmt,...)
{
    va_list args;
    char printbuffer[CFG_PBSIZE];

    va_start(args, fmt);
    vsprintf(printbuffer, fmt, args);
    va_end(args);

    strcpy(buf,printbuffer);   // Print the string to buf
}

int strnicmp(const char *s1, const char *s2, unsigned int len)
{
    unsigned char c1 = 0;
    unsigned char c2 = 0;
    int ret = 0;

    if ((NULL == s1) || (NULL == s2)) {
        printf("arg is NULL error:%s\n",__FUNCTION__);
        ret = -1;
    } else {
        c1 = c2 = 0;
        if (len != 0) {
            do {
                c1 = *s1;
                c2 = *s2;
                s1++;
                s2++;
                if (0 == c1) {
                    break;
                }
                if (0 == c2) {
                    break;
                }
                if (c1 == c2) {
                    continue;
                }
                c1 = tolower(c1);
                c2 = tolower(c2);
                if (c1 != c2) {
                    break;
                }
            } while (--len);
        }

        ret = (int)c1 - (int)c2;
    }

    return ret;
}

void putc (const char c)
{
    if (c == '\n')
        ns16550OutputChar('\r');
    ns16550OutputChar (c);
    return;
}

void puts (const unsigned char *s)
{
    while (*s) {
        putc (*s++);
    }
    return;
}

void printf (const char *fmt,...)
{
    va_list args;
    char printbuffer[CFG_PBSIZE];

    va_start(args, fmt);
    vsprintf(printbuffer, fmt, args);
    va_end(args);

    puts((const unsigned char *)printbuffer);  // Print the string
}
