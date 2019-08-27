/* bLib.c - buffer manipulation library */

/*
 * Copyright (c) 2015 CAG Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable CAG license agreement.
 */

/*
modification history
--------------------
11jan15,Jeff created.
*/

/*
DESCRIPTION
This library contains routines to manipulate buffers of variable-length
byte arrays.  Operations are performed on long words when possible, even
though the buffer lengths are specified in bytes.  This occurs only when
source and destination buffers start on addresses that are both odd or both
even.  If one buffer is even and the other is odd, operations must be done
one byte at a time, thereby slowing down the process.

Certain applications, such as byte-wide memory-mapped peripherals, may
require that only byte operations be performed.  For this purpose, the
routines bcopyBytes() and bfillBytes() provide the same functions as bcopy()
and bfill(), but use only byte-at-a-time operations.  These routines do
not check for null termination.

\IFSET_START KERNEL
INCLUDE FILES: string.h
\IFSET_END
\IFSET_START USER
INCLUDE FILES: strings.h
\IFSET_END

SEE ALSO: 'ansiString'
*/

/* LINTLIBRARY */

#include <libc/string.h>
#include <libc/stddef.h>
#include <libc/system.h>

#define _CAG_PORTABLE_bLib
#define _CAG_PORTABLE_swab

#undef bcmp /* so routine gets built for those who don't include header files */
/*******************************************************************************
*
* bcmp - compare one buffer to another
*
* This routine compares the first <nbytes> characters of <buf1> to <buf2>.
*
* RETURNS
* \ms
* \m -
* 0 if the first <nbytes> of <buf1> and <buf2> are identical,
* \m -
* less than 0 if <buf1> is less than <buf2>, or
* \m -
* greater than 0 if <buf1> is greater than <buf2>.
* \me
*
* ERRNO: N/A
*/

int bcmp
(
    const void * buf1,            /* pointer to first buffer    */
    const void * buf2,            /* pointer to second buffer   */
    size_t nbytes         	  /* number of bytes to compare */
)
{
    const unsigned char *p1 = (const unsigned char *)buf1;
    const unsigned char *p2 = (const unsigned char *)buf2;

    /* size of memory is zero */

    if (nbytes == 0)
        return (0);

    while (*p1++ == *p2++) {
        if (--nbytes == 0)
            return (0);
    }

    return ((*--p1) - (*--p2));
}

/*******************************************************************************
*
* binvert - invert the order of bytes in a buffer
*
* This routine inverts an entire buffer, byte by byte.  For example,
* the buffer {1, 2, 3, 4, 5} would become {5, 4, 3, 2, 1}.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void binvert
(
    void *  buf,             /* pointer to buffer to invert  */
    size_t  nbytes           /* number of bytes in buffer    */
)
{
    char * bufc = (char *) buf;
    char * buf_end = bufc + nbytes - 1;
    char temp;

    while (bufc < buf_end) {
        temp       = *bufc;
        *bufc      = *buf_end;
        *buf_end   = temp;

        buf_end--;
        bufc++;
    }
}

/*******************************************************************************
*
* bswap - swap buffers
*
* This routine exchanges the first <nbytes> of the two specified buffers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void bswap
(
    void *  	  buf1,            /* pointer to first buffer  */
    void *        buf2,            /* pointer to second buffer */
    size_t        nbytes           /* number of bytes to swap  */
)
{
    char * buffer1 = (char *) buf1;
    char * buffer2 = (char *) buf2;
    char temp;

    while (nbytes >= 1) {
        temp = *buffer1;
        *buffer1++ = *buffer2;
        *buffer2++ = temp;
        nbytes--;
    }
}

#ifdef	_CAG_PORTABLE_swab
/*******************************************************************************
*
* swab - swap bytes
*
* This routine gets the specified number of bytes from <source>,
* exchanges the adjacent even and odd bytes, and puts them in <destination>.
* The buffers <source> and <destination> should not overlap.
*
* \&NOTE:  On some CPUs, swab() will cause an exception if the buffers are
* unaligned.  In such cases, use uswab() for unaligned swaps.  On ARM
* family CPUs, swab() may reorder the bytes incorrectly without causing
* an exception if the buffers are unaligned.  Again, use uswab() for
* unaligned swaps.
*
* The value of <nBytes> must not be odd.  Failure to adhere to this may yield
* incorrect results.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: uswab()
*/

void swab
(
    const void * source,              /* pointer to source buffer      */
    void *       destination,         /* pointer to destination buffer */
    size_t       nbytes               /* number of bytes to exchange   */
)
{
    FAST unsigned short *src = (unsigned short *) source;
    FAST unsigned short *dst = (unsigned short *) destination;
#if defined(_CAG_IMPROVED_PORTABLE_SWAB)
    /*
     * This generates much better code for the ARM, and might well be
     * faster on gcc-based compilers for other architectures.
     */

    FAST unsigned short *dst_end =
        (unsigned short *) ((char *) destination + nbytes);
#else /* _CAG_IMPROVED_PORTABLE_SWAB */
    FAST unsigned short *dst_end = dst + (nbytes / (sizeof (unsigned short)));
#endif /* _CAG_IMPROVED_PORTABLE_SWAB */

    for (; dst < dst_end; dst++, src++) {
        *dst = (short)(((*src & 0x00ff) << 8) | ((*src & 0xff00) >> 8));
    }
}
#endif	/* _CAG_PORTABLE_swab */

/*******************************************************************************
*
* uswab - swap bytes with buffers that are not necessarily aligned
*
* This routine gets the specified number of bytes from <source>,
* exchanges the adjacent even and odd bytes, and puts them in <destination>.
*
* \&NOTE:  Due to speed considerations, this routine should only be used when
* absolutely necessary.  Use swab() for aligned swaps.
*
* The value of <nBytes> must not be odd.  Failure to adhere to this may yield
* incorrect results.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: swab()
*/

void uswab
(
    const void * source,               /* pointer to source buffer      */
    void *       destination,          /* pointer to destination buffer */
    size_t       nbytes                /* number of bytes to exchange   */
)
{
    char * src = (char *) source;
    char * dst = (char *) destination;
    char * dst_end = dst + nbytes;
    char byte1;
    char byte2;

    while (dst < dst_end) {
        byte1 = *src++;
        byte2 = *src++;
        *dst++ = byte2;
        *dst++ = byte1;
    }
}

/*******************************************************************************
*
* bzero - zero out a buffer
*
* This routine fills the first <nbytes> characters of the
* specified buffer with 0.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void bzero
(
    void *  buffer,               /* buffer to be zeroed       */
    size_t  nbytes                /* number of bytes in buffer */
)
{
    bfill (buffer, nbytes, 0);
}

#ifdef _CAG_PORTABLE_bLib
/*******************************************************************************
*
* bcopy - copy one buffer to another
*
* This routine copies the first <nbytes> characters from <source> to
* <destination>.  Overlapping buffers are handled correctly.  Copying is done
* in the most efficient way possible, which may include long-word, or even
* multiple-long-word moves on some architectures.  In general, the copy
* will be significantly faster if both buffers are long-word aligned.
* (For copying that is restricted to byte, word, or long-word moves, see
* the manual entries for bcopyBytes(), bcopyWords(), and bcopyLongs().)
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: bcopyBytes(), bcopyWords(), bcopyLongs()
*/

void bcopy
(
    const void * source,        /* pointer to source buffer      */
    void       * destination,   /* pointer to destination buffer */
    size_t       nbytes        	/* number of bytes to copy       */
)
{
    char * src = (char *) source;
    char * dst = (char *) destination;
    char * dstend;
    long * srcl;
    long * dstl;
    char * dsttmp;

    /*
     * Note: the condition (size_t)(destination - source) >= nbytes
     * is true also when destination < source, provided the source buffer
     * does not wrap around the end of memory and then past destination.
     * The only cases that this algorithm cannot handle are those
     * where the source and destination buffers overlap at both
     * ends, not just one. This requires wraparound; an example
     * is the following:
     *
     *   0                                               WRAP
     *   |                                               |
     *         S------------------------------>|
     *                                    D--------------
     *   ----------------->|
     *
     * In such cases, starting the transfer at either end of the
     * source buffer results in overwriting a source datum that hasn't
     * been read yet when first data is stored in the destination
     * buffer. One could perhaps start somewhere in the middle,
     * but the complication to the code is not worth it, for this
     * ultimate edge case involving both wrapping and buffers
     * longer than half of the address space.
     *
     * We also do not bother to optimize the unusual case
     * that (destination == source).
     */

    if ((size_t)(dst - src) >= nbytes) {
        /* forward copy */

        dstend = dst + nbytes;

        /*
         * Do a byte copy if nbytes is too small for amount of blind
         * copying, or if the source and destination alignments do not match.
         */

        if (nbytes < (2 * sizeof (long) - 1) ||
            (((long)dst ^ (long)src) & (sizeof (long) - 1)))
            goto byte_copy_fwd;

        /* if odd-aligned copy byte */

        while ((long)dst & (sizeof (long) - 1))
            *dst++ = *src++;

        srcl = (long *) src;
        dstl = (long *) dst;

        /*
             * dsttmp is highest sizeof(long) aligned address no greater than
             * dstend
             */

        dsttmp = (char *)((size_t)dstend & ~(sizeof(long) - 1));

        do {
            *dstl++ = *srcl++;
        } while ((char *)dstl != dsttmp);

        dst = (char *)dstl;
        src      = (char *)srcl;

byte_copy_fwd:
        while (dst != dstend)
            *dst++ = *src++;
    } else {
        /* backward copy */

        dstend = dst;
        dst += nbytes;
        src += nbytes;

        /* do byte copy if less than ten or alignment mismatch */

        if (nbytes < (2 * sizeof(long) - 1) ||
            (((long)dst ^ (long)src) & (sizeof(long) - 1)))
            goto byte_copy_bwd;

        /* if odd-aligned copy byte */

        while ((long)dst & (sizeof(long) - 1))
            *--dst = *--src;

        srcl = (long *) src;
        dstl = (long *) dst;

        /*
         * dsttmp is the smallest sizeof(long) aligned address
         * no less than dstend;
         */

        dsttmp = (char *)(((size_t)dstend + sizeof(long) - 1) &
                          ~(sizeof(long) - 1));
        do {
            *--dstl = *--srcl;
        } while ((char *)dstl != dsttmp);

        dst = (char *)dstl;
        src = (char *)srcl;

byte_copy_bwd:
        while (dst != dstend)
            *--dst = *--src;
    }
}

/*******************************************************************************
*
* bcopyBytes - copy one buffer to another one byte (8 bits) at a time
*
* This routine copies the first <nbytes> characters from <source> to
* <destination> one byte at a time.  This may be desirable if a buffer can
* only be accessed with byte instructions, as in certain byte-wide
* memory-mapped peripherals.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: bcopy()
*/

void bcopyBytes
(
    const void * source,        /* pointer to source buffer      */
    void *       destination,   /* pointer to destination buffer */
    size_t       nbytes         /* number of bytes to copy       */
)
{
    char * src = (char *) source;
    char * dst = (char *) destination;
    char * dstend;
    size_t offset = (size_t)(dst - src);

    if (offset == 0)
        return;

    if (offset >= nbytes) { /* true also when destination < source if no wrap */
        /* forward copy */

        dstend = dst + nbytes;

        while (dst != dstend)
            *dst++ = *src++;
    } else {
        /* backward copy */

        dstend = dst;
        dst += nbytes;
        src += nbytes;

        while (dst != dstend)
            *--dst = *--src;
    }
}

/*******************************************************************************
*
* bcopyWords - copy one buffer to another one word (16 bits) at a time
*
* This routine copies the first <nwords> words from <source> to <destination>
* one word at a time.  This may be desirable if a buffer can only be accessed
* with word instructions, as in certain word-wide memory-mapped peripherals.
* The source and destination must be word-aligned.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: bcopy()
*/

void bcopyWords
(
    const void * source,       /* pointer to source buffer      */
    void *       destination,  /* pointer to destination buffer */
    size_t       nwords        /* number of words to copy       */
)
{
    short *dstend;
    short *src = (short *) source;
    short *dst = (short *) destination;
    size_t nbytes = nwords << 1;           /* convert to bytes */
    size_t offset = (size_t)destination - (size_t)source;

    if (offset == 0)
        return;

    if (offset >= nbytes) { /* true also when destination < source if no wrap */
        /* forward copy */
        dstend = dst + nwords;
        while (dst != dstend)
            *dst++ = *src++;
    } else {
        /* backward copy */
        dstend = dst;
        dst   += nwords;
        src   += nwords;
        while (dst != dstend)
            *--dst = *--src;
    }
}

/*******************************************************************************
*
* bcopyLongs - copy one buffer to another one long word (32 bits) at a time
*
* This routine copies the first <nlongs> 32-bit words from <source> to
* <destination> one 32-bit word at a time.  This may be desirable if a buffer
* can only be accessed with 32-bit loads and stores, as in certain 32-bit-wide
* memory-mapped peripherals.  The source and destination must be
* 4-byte aligned.
*
* Note that in LP64 builds, a 'long' is 64 bits; nevertheless, this
* routine copies in units of 32-bits, and <nlongs> is the number of
* 32-bit data in the buffer.  See bcopyQuads() to copy data in units
* of 64 bits.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: bcopy()
*/

void bcopyLongs
(
    const void * source,       /* pointer to source buffer      */
    void *       destination,  /* pointer to destination buffer */
    size_t       nlongs        /* number of longs to copy       */
)
{
    UINT32 *dstend;
    UINT32 *src = (UINT32 *) source;
    UINT32 *dst = (UINT32 *) destination;
    size_t nbytes = nlongs * sizeof(UINT32);    /* convert to bytes */
    size_t offset = (size_t)destination - (size_t)source;

    if (offset == 0)
        return;

    if (offset >= nbytes) { /* true also when destination < source if no wrap */
        /* forward copy */
        dstend = dst + nlongs;
        while (dst != dstend)
            *dst++ = *src++;
    } else {
        /* backward copy */
        dstend = dst;
        dst   += nlongs;
        src   += nlongs;
        while (dst != dstend)
            *--dst = *--src;
    }
}

/*******************************************************************************
*
* bcopyQuads - copy one buffer to another quad word (64 bits) at a time (64-bit)
*
* This routine copies the first <nquads> 64-bit words from <source> to
* <destination>, one 64-bit word at a time.  This may be desirable if a buffer
* can only be accessed with 64-bit loads and stores, as in certain 64-bit-wide
* memory-mapped peripherals.  The source and destination must be
* 8-byte aligned.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: bcopy()
*/

void bcopyQuads
(
    const void * source,       /* pointer to source buffer      */
    void *       destination,  /* pointer to destination buffer */
    size_t       nquads        /* number of quads (64-bit units) to copy */
)
{
    UINT64 *dstend;
    UINT64 *src = (UINT64 *) source;
    UINT64 *dst = (UINT64 *) destination;
    size_t nbytes = nquads * sizeof(UINT64);    /* convert to bytes */
    size_t offset = (size_t)(dst - src );

    if (offset == 0)
        return;

    if (offset >= nbytes) { /* true also when destination < source if no wrap */
        /* forward copy */
        dstend = dst + nquads;
        while (dst != dstend)
            *dst++ = *src++;
    } else {
        /* backward copy */
        dstend = dst;
        dst   += nquads;
        src   += nquads;
        while (dst != dstend)
            *--dst = *--src;
    }
}

#undef bfill /* so bfill gets built for those who don't include header files */
/*******************************************************************************
*
* bfill - fill a buffer with a specified character
*
* This routine fills the first <nbytes> characters of a buffer with the
* character <ch>.  Filling is done in the most efficient way possible,
* which may be long-word, or even multiple-long-word stores, on some
* architectures.  In general, the fill will be significantly faster if
* the buffer is long-word aligned.  (For filling that is restricted to
* byte stores, see the manual entry for bfillBytes().)
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: bfillBytes()
*/

void bfill
(
    void * buf,          /* pointer to buffer              */
    size_t nbytes,       /* number of bytes to fill        */
    int    ch            /* char with which to fill buffer */
)
{
    char * bufc = (char *) buf;
    long * pBuf;
    char * bufend = bufc + nbytes;
    char * buftmp;
    long val;

    if (nbytes < 2 * sizeof(long) - 1)
        goto byte_fill;

    /*
     * Start on necessary alignment. This loop advances buf
     * at most sizeof(long) - 1 bytes.
     */

    while ((long)bufc & (sizeof(long) - 1)) {
        *bufc++ = (char) ch;
    }

    /* write sizeof(long) bytes at a time */
    val = (unsigned char)ch;  /* memset() wants the cast */
    val |= (val << 8);
    val |= (val << 16);

    /* Find the last long word boundary at or before bufend */
    buftmp = (char *)((long)bufend & ~(sizeof(long) - 1));

    pBuf = (long *)bufc;

    /* fill 4 bytes (8 bytes for LP64) at a time; don't exceed buf endpoint */

    do {
        /* Assert: buftmp and pBuf are sizeof(long) aligned */
        /* Assert: buftmp - (char*)pBuf >= sizeof (long) */
        *pBuf++ = val;
    } while ((char *)pBuf != buftmp);

    bufc = (char *)pBuf;

    /* fill remaining bytes one at a time */

byte_fill:
    while (bufc != bufend) {
        *bufc++ = (char) ch;
    }
}

/*******************************************************************************
*
* bfillBytes - fill buffer with a specified character one byte at a time
*
* This routine fills the first <nbytes> characters of the specified buffer
* with the character <ch> one byte at a time.  This may be desirable if a
* buffer can only be accessed with byte instructions, as in certain
* byte-wide memory-mapped peripherals.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: bfill()
*/

void bfillBytes
(
    void * buf,        /* pointer to buffer              */
    size_t nbytes,     /* number of bytes to fill        */
    int    ch          /* char with which to fill buffer */
)
{
    char * bufc = (char *) buf;
    char * bufend = bufc + nbytes;

    while (bufc != bufend)
        *bufc++ = (char) ch;
}
#endif  /* _CAG_PORTABLE_bLib */

#undef index /* so index gets built for those who don't include header files */
/*******************************************************************************
*
* index - find the first occurrence of a character in a string
*
* This routine finds the first occurrence of character <c>
* in string <s>.
*
* RETURNS:
* A pointer to the located character, or
* NULL if <c> is not found.
*
* ERRNO: N/A
*
* SEE ALSO: strchr().
*/

char *index
(
    const char * s,      /* string in which to find character */
    int          c       /* character to find in string       */
)
{
    char ch;

    while (((ch = *(s++)) != c) && (ch != EOS))
        ;

    return (((ch == EOS) && c != EOS) ? NULL : (char *) --s);
}

#undef rindex /* so rindex is built for those who don't include header files */
/*******************************************************************************
*
* rindex - find the last occurrence of a character in a string
*
* This routine finds the last occurrence of character <c>
* in string <s>.
*
* RETURNS:
* A pointer to <c>, or
* NULL if <c> is not found.
*
* ERRNO: N/A
*/

char *rindex
(
    const char * s,   /* string in which to find character */
    int               c    /* character to find in string       */
)
{
    long i;	/* must be signed! */

    i = (long) strlen (s);	/* point to null terminator of s */

    while (i >= 0) {
        if (s [i] == c)
            return ((char *) &s [i]);

        i--;
    }

    return ((char *) NULL);
}

