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


#ifndef __BITS_H__
#define __BITS_H__
#include <libc/stddef1.h>
#define  BIT_WIDTH 32
INT32 get_bit_val(UINT32 * src, INT16 seq);
void set_bit_val(UINT32 * src, INT16 seq);
void clear_bit_val(UINT32 *src, INT16 seq);
INT32 get_all_one(UINT32 n);
#endif
