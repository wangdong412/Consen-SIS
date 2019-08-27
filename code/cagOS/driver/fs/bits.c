/*
 *------------------------------------------------------------
 * Project: cagOS
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
 * 2015.9.3, Ezio created
 *
 * DECRIPTION
 * get and set 1-bit in one piece of memory
 *
 */

#include "bits.h"
INT32 get_bit_val(UINT32* src, INT16 seq)
{
    INT32 x;
    x= src[seq/BIT_WIDTH];
    x &= 1<<(seq%BIT_WIDTH);
    return (x == 0)?0:1;
}


void set_bit_val(UINT32* src, INT16 seq)
{
    INT32 x;
    x= src[seq/BIT_WIDTH];
    x |= 1<<(seq%BIT_WIDTH);
    src[seq/BIT_WIDTH] = x;
}

void clear_bit_val(UINT32 *src, INT16 seq)
{
    INT32 x,y;
    x = src[seq/BIT_WIDTH];
    y = ~(1<<(seq%BIT_WIDTH));
    x &= y;
    src[seq/BIT_WIDTH] = x;
}


INT32 get_all_one(UINT32 n)
{
    UINT32 c =0 ;
    for (c =0; n; c++) {
        /* 清除最低位的1*/
        n &= (n -1) ;
    }
    return c ;
}
