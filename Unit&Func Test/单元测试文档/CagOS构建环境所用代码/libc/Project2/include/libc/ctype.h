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
 * This file contains.
 */

#ifndef __CTYPE_H__
#define __CTYPE_H__

/*
 * NOTE! This ctype does not handle EOF like the standard C
 * library is required to.
 */

#ifndef __GNU__
#define __GNU__
#endif

#ifdef __GNU__
#define likely(x) __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)
#endif

/* There is no consistent ctype implementation, just a smattering of
 * functions.  Individually, they are okay, but a more standard, data lookup
 * approach would make more sense if used extensively.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Name: isspace
 *
 * Description:
 *   Checks  for  white-space characters.  In the "C" and "POSIX" locales,
 *   these are: space, form-feed ('\f'), newline ('\n'), carriage return
 *   ('\r'), horizontal tab ('\t'), and vertical tab ('\v').
 *
 ****************************************************************************/

#define isspace(c) \
  ((c) == ' '  || (c) == '\t' || (c) == '\n' || \
   (c) == '\r' || (c) == '\f' || c== '\v')

/****************************************************************************
 * Name: isascii
 *
 * Description:
 *   Checks whether c is a 7-bit unsigned char value that fits into the
 *   ASCII character set.
 *
 ****************************************************************************/

#define isascii(c)   ((c) >= 0 && (c) <= 0x7f)

/****************************************************************************
 * Name: isprint
 *
 * Description:
 *   Checks for a printable character (including space)
 *
 ****************************************************************************/

#define isprint(c)   ((c) >= 0x20 && (c) < 0x7f)

/****************************************************************************
 * Name: isgraph
 *
 * Description:
 *   Checks for a printable character (excluding space)
 *
 ****************************************************************************/

#define isgraph(c)   ((c) > 0x20 && (c) < 0x7f)

/****************************************************************************
 * Name: iscntrl
 *
 * Description:
 *   Checks for control character.
 *
 ****************************************************************************/

#define iscntrl(c) (!isprint(c))

/****************************************************************************
 * Name: islower
 *
 * Description:
 *   Checks for an lowercase letter.
 *
 ****************************************************************************/

#define islower(c)   ((c) >= 'a' && (c) <= 'z')

/****************************************************************************
 * Name: isupper
 *
 * Description:
 *   Checks for an uppercase letter.
 *
 ****************************************************************************/

#define isupper(c)   ((c) >= 'A' && (c) <= 'Z')

/****************************************************************************
 * Name: isalpha
 *
 * Description:
 *   Checks for an alphabetic character
 *
 ****************************************************************************/

#define isalpha(c)   (islower(c) || isupper(c))

/****************************************************************************
 * Name: isdigit
 *
 * Description:
 *   ANSI standard isdigit implementation.
 *
 ****************************************************************************/

#define isdigit(c)   ((c) >= '0' && (c) <= '9')

/****************************************************************************
 * Name: isalnum
 *
 * Description:
 *   Checks for an alphanumeric character
 *
 ****************************************************************************/

#define isalnum(c)   (isalpha(c) || isdigit(c))

/****************************************************************************
 * Name: ispunct
 *
 * Description:
 *   Checks for a printable character which is not a space or an
 *   alphanumeric character
 *
 ****************************************************************************/

#define ispunct(c)   (isgraph(c) && !isalnum(c))

/****************************************************************************
 * Name: isxdigit
 *
 * Description:
 *   isxdigit() checks for a hexadecimal digits, i.e. one of {0-9,a-f,A-F}
 *
 ****************************************************************************/

#define isxdigit(c) \
  (((c) >= '0' && (c) <= '9') || \
   ((c) >= 'a' && (c) <= 'f') || \
   ((c) >= 'A' && (c) <= 'F'))

/****************************************************************************
 * Name: toupper
 *
 * Description:
 *   toupper() converts the letter c to upper case, if possible.
 *
 ****************************************************************************/

#define toupper(c) \
  (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))

/****************************************************************************
 * Name: tolower
 *
 * Description:
 *   tolower() converts the letter c to lower case, if possible.
 *
 ****************************************************************************/

#define tolower(c) \
  (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

#endif
