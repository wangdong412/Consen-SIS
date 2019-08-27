/* sysCount.c - kernel/sysOs CPU COUNT LEAD ZEROs LOOKUP TABLE */

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


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    MICRIUM_SOURCE
#include  <kernel/sysOs.h>

/*
*********************************************************************************************************
*                                  CPU COUNT LEAD ZEROs LOOKUP TABLE
*
* Note(s) : (1) Index into bit pattern table determines the number of leading zeros in an 8-bit value :
*
*                         b07  b06  b05  b04  b03  b02  b01  b00    # Leading Zeros
*                         ---  ---  ---  ---  ---  ---  ---  ---    ---------------
*                          1    x    x    x    x    x    x    x            0
*                          0    1    x    x    x    x    x    x            1
*                          0    0    1    x    x    x    x    x            2
*                          0    0    0    1    x    x    x    x            3
*                          0    0    0    0    1    x    x    x            4
*                          0    0    0    0    0    1    x    x            5
*                          0    0    0    0    0    0    1    x            6
*                          0    0    0    0    0    0    0    1            7
*                          0    0    0    0    0    0    0    0            8
*********************************************************************************************************
*/

static  const  CPU_INT08U  CPU_CntLeadZerosTbl[256] = {                             /* Data vals :                      */
    /*   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F   */
    8u,  7u,  6u,  6u,  5u,  5u,  5u,  5u,  4u,  4u,  4u,  4u,  4u,  4u,  4u,  4u,  /*   0x00 to 0x0F                   */
    3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  /*   0x10 to 0x1F                   */
    2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  /*   0x20 to 0x2F                   */
    2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  /*   0x30 to 0x3F                   */
    1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x40 to 0x4F                   */
    1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x50 to 0x5F                   */
    1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x60 to 0x6F                   */
    1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x70 to 0x7F                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0x80 to 0x8F                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0x90 to 0x9F                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xA0 to 0xAF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xB0 to 0xBF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xC0 to 0xCF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xD0 to 0xDF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xE0 to 0xEF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u   /*   0xF0 to 0xFF                   */
};

/*
*********************************************************************************************************
*                                         CPU_CntLeadZeros()
*
* Description : Count the number of contiguous, most-significant, leading zero bits in a data value.
*
* Argument(s) : val         Data value to count leading zero bits.
*
* Return(s)   : Number of contiguous, most-significant, leading zero bits in 'val', if NO error(s).
*
*               DEF_INT_CPU_U_MAX_VAL,                                              otherwise.
*
* Caller(s)   : CPU_CntTrailZeros(),
*               Application.
*
*               This function is a CPU module application programming interface (API) function & MAY
*               be called by application function(s).
*
* Note(s)     : (1) (a) Supports the following data value sizes :
*
*                       (1)  8-bits
*                       (2) 16-bits
*                       (3) 32-bits
*                       (4) 64-bits
*
*                       See also 'cpu_def.h  CPU WORD CONFIGURATION  Note #1'.
*
*                   (b) (1) For  8-bit values :
*
*                                  b07  b06  b05  b04  b03  b02  b01  b00    # Leading Zeros
*                                  ---  ---  ---  ---  ---  ---  ---  ---    ---------------
*                                   1    x    x    x    x    x    x    x            0
*                                   0    1    x    x    x    x    x    x            1
*                                   0    0    1    x    x    x    x    x            2
*                                   0    0    0    1    x    x    x    x            3
*                                   0    0    0    0    1    x    x    x            4
*                                   0    0    0    0    0    1    x    x            5
*                                   0    0    0    0    0    0    1    x            6
*                                   0    0    0    0    0    0    0    1            7
*                                   0    0    0    0    0    0    0    0            8
*
*
*                       (2) For 16-bit values :
*
*                             b15  b14  b13  ...  b04  b03  b02  b01  b00    # Leading Zeros
*                             ---  ---  ---       ---  ---  ---  ---  ---    ---------------
*                              1    x    x         x    x    x    x    x            0
*                              0    1    x         x    x    x    x    x            1
*                              0    0    1         x    x    x    x    x            2
*                              :    :    :         :    :    :    :    :            :
*                              :    :    :         :    :    :    :    :            :
*                              0    0    0         1    x    x    x    x           11
*                              0    0    0         0    1    x    x    x           12
*                              0    0    0         0    0    1    x    x           13
*                              0    0    0         0    0    0    1    x           14
*                              0    0    0         0    0    0    0    1           15
*                              0    0    0         0    0    0    0    0           16
*
*                       (3) For 32-bit values :
*
*                             b31  b30  b29  ...  b04  b03  b02  b01  b00    # Leading Zeros
*                             ---  ---  ---       ---  ---  ---  ---  ---    ---------------
*                              1    x    x         x    x    x    x    x            0
*                              0    1    x         x    x    x    x    x            1
*                              0    0    1         x    x    x    x    x            2
*                              :    :    :         :    :    :    :    :            :
*                              :    :    :         :    :    :    :    :            :
*                              0    0    0         1    x    x    x    x           27
*                              0    0    0         0    1    x    x    x           28
*                              0    0    0         0    0    1    x    x           29
*                              0    0    0         0    0    0    1    x           30
*                              0    0    0         0    0    0    0    1           31
*                              0    0    0         0    0    0    0    0           32
*
*
*                       (4) For 64-bit values :
*
*                             b63  b62  b61  ...  b04  b03  b02  b01  b00    # Leading Zeros
*                             ---  ---  ---       ---  ---  ---  ---  ---    ---------------
*                              1    x    x         x    x    x    x    x            0
*                              0    1    x         x    x    x    x    x            1
*                              0    0    1         x    x    x    x    x            2
*                              :    :    :         :    :    :    :    :            :
*                              :    :    :         :    :    :    :    :            :
*                              0    0    0         1    x    x    x    x           59
*                              0    0    0         0    1    x    x    x           60
*                              0    0    0         0    0    1    x    x           61
*                              0    0    0         0    0    0    1    x           62
*                              0    0    0         0    0    0    0    1           63
*                              0    0    0         0    0    0    0    0           64
*
*
*                       See also 'CPU COUNT LEAD ZEROs LOOKUP TABLE  Note #1'.
*********************************************************************************************************
*/

CPU_DATA  CPU_CntLeadZeros (CPU_DATA  val)
{
    CPU_DATA  nbr_lead_zeros;

    nbr_lead_zeros = CPU_CntLeadZeros32((CPU_INT32U)val);

    return (nbr_lead_zeros);
}

/*
*********************************************************************************************************
*                                        CPU_CntLeadZeros32()
*
* Description : Count the number of contiguous, most-significant, leading zero bits in a 32-bit data value.
*
* Argument(s) : val         Data value to count leading zero bits.
*
* Return(s)   : Number of contiguous, most-significant, leading zero bits in 'val'.
*
* Caller(s)   : CPU_CntLeadZeros(),
*               CPU_CntTrailZeros32(),
*               Application.
*
*               This function is a CPU module application programming interface (API) function & MAY be
*               called by application function(s).
*
* Note(s)     : (1) Supports 32-bit values :
*
*                          b31  b30  b29  ...  b04  b03  b02  b01  b00    # Leading Zeros
*                          ---  ---  ---       ---  ---  ---  ---  ---    ---------------
*                           1    x    x         x    x    x    x    x            0
*                           0    1    x         x    x    x    x    x            1
*                           0    0    1         x    x    x    x    x            2
*                           :    :    :         :    :    :    :    :            :
*                           :    :    :         :    :    :    :    :            :
*                           0    0    0         1    x    x    x    x           27
*                           0    0    0         0    1    x    x    x           28
*                           0    0    0         0    0    1    x    x           29
*                           0    0    0         0    0    0    1    x           30
*                           0    0    0         0    0    0    0    1           31
*                           0    0    0         0    0    0    0    0           32
*
*
*                   See also 'CPU COUNT LEAD ZEROs LOOKUP TABLE  Note #1'.
*********************************************************************************************************
*/

#if (CPU_CFG_DATA_SIZE_MAX >= CPU_WORD_SIZE_32)
CPU_DATA  CPU_CntLeadZeros32 (CPU_INT32U  val)
{
    CPU_DATA  ix;
    CPU_DATA  nbr_lead_zeros;
    /* ---------- ASM-OPTIMIZED ----------- */

    if (val > 0x0000FFFFu) {
        if (val > 0x00FFFFFFu) {                                                /* Chk bits [31:24] :                   */
            /* .. Nbr lead zeros =               .. */
            ix             = (CPU_DATA)(val >> 24u);                /* .. lookup tbl ix  = 'val' >> 24 bits */
            nbr_lead_zeros = (CPU_DATA)(CPU_CntLeadZerosTbl[ix]);               /* .. plus nbr msb lead zeros =  0 bits.*/

        } else {                                                                /* Chk bits [23:16] :                   */
            /* .. Nbr lead zeros =               .. */
            ix             = (CPU_DATA)(val >> 16u);                /* .. lookup tbl ix  = 'val' >> 16 bits */
            nbr_lead_zeros = (CPU_DATA)((CPU_DATA)CPU_CntLeadZerosTbl[ix] +  8u);/* .. plus nbr msb lead zeros =  8 bits.*/
        }

    } else {
        if (val > 0x000000FFu) {                                                /* Chk bits [15:08] :                   */
            /* .. Nbr lead zeros =               .. */
            ix             = (CPU_DATA)(val >>  8u);                /* .. lookup tbl ix  = 'val' >>  8 bits */
            nbr_lead_zeros = (CPU_DATA)((CPU_DATA)CPU_CntLeadZerosTbl[ix] + 16u);/* .. plus nbr msb lead zeros = 16 bits.*/

        } else {                                                                /* Chk bits [07:00] :                   */
            /* .. Nbr lead zeros =               .. */
            ix             = (CPU_DATA)(val >>  0u);                /* .. lookup tbl ix  = 'val' >>  0 bits */
            nbr_lead_zeros = (CPU_DATA)((CPU_DATA)CPU_CntLeadZerosTbl[ix] + 24u);/* .. plus nbr msb lead zeros = 24 bits.*/
        }
    }
    return (nbr_lead_zeros);
}
#endif
