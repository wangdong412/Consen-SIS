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

#ifndef  CPU_CORE_MODULE_PRESENT                                /* See Note #1.                                         */
#define  CPU_CORE_MODULE_PRESENT


/*
*********************************************************************************************************
*                                          CPU_TYPE_CREATE()
*
* Description : Creates a generic type value.
*
* Argument(s) : char_1      1st ASCII character to create generic type value.
*
*               char_2      2nd ASCII character to create generic type value.
*
*               char_3      3rd ASCII character to create generic type value.
*
*               char_4      4th ASCII character to create generic type value.
*
* Return(s)   : 32-bit generic type value.
*
* Caller(s)   : various.
*
* Note(s)     : (1) (a) Generic type values should be #define'd with large, non-trivial values to trap
*                       & discard invalid/corrupted objects based on type value.
*
*                       In other words, by assigning large, non-trivial values to valid objects' type
*                       fields; the likelihood that an object with an unassigned &/or corrupted type
*                       field will contain a value is highly improbable & therefore the object itself
*                       will be trapped as invalid.
*
*                   (b) (1) CPU_TYPE_CREATE()  creates a 32-bit type value from four values.
*
*                       (2) Ideally, generic type values SHOULD be created from 'CPU_CHAR' characters to
*                           represent ASCII string abbreviations of the specific object types.  Memory
*                           displays of object type values will display the specific object types with
*                           their chosen ASCII names.
*
*                           Examples :
*
*                               #define  FILE_TYPE  CPU_TYPE_CREATE('F', 'I', 'L', 'E')
*                               #define  BUF_TYPE   CPU_TYPE_CREATE('B', 'U', 'F', ' ')
*********************************************************************************************************
*/

#if     (CPU_CFG_ENDIAN_TYPE == CPU_ENDIAN_TYPE_BIG)
#define  CPU_TYPE_CREATE(char_1, char_2, char_3, char_4)        (((CPU_INT32U)((CPU_INT08U)(char_1)) << (3u * DEF_OCTET_NBR_BITS)) | \
                                                                 ((CPU_INT32U)((CPU_INT08U)(char_2)) << (2u * DEF_OCTET_NBR_BITS)) | \
                                                                 ((CPU_INT32U)((CPU_INT08U)(char_3)) << (1u * DEF_OCTET_NBR_BITS)) | \
                                                                 ((CPU_INT32U)((CPU_INT08U)(char_4))))

#else

#if    ((CPU_CFG_DATA_SIZE   == CPU_WORD_SIZE_64) || \
        (CPU_CFG_DATA_SIZE   == CPU_WORD_SIZE_32))
#define  CPU_TYPE_CREATE(char_1, char_2, char_3, char_4)        (((CPU_INT32U)((CPU_INT08U)(char_1))) | \
                                                                 ((CPU_INT32U)((CPU_INT08U)(char_2)) << (1u * DEF_OCTET_NBR_BITS)) | \
                                                                 ((CPU_INT32U)((CPU_INT08U)(char_3)) << (2u * DEF_OCTET_NBR_BITS)) | \
                                                                 ((CPU_INT32U)((CPU_INT08U)(char_4)) << (3u * DEF_OCTET_NBR_BITS)))


#elif   (CPU_CFG_DATA_SIZE   == CPU_WORD_SIZE_16)
#define  CPU_TYPE_CREATE(char_1, char_2, char_3, char_4)        (((CPU_INT32U)((CPU_INT08U)(char_1)) << (2u * DEF_OCTET_NBR_BITS)) | \
                                                                 ((CPU_INT32U)((CPU_INT08U)(char_2)) << (3u * DEF_OCTET_NBR_BITS)) | \
                                                                 ((CPU_INT32U)((CPU_INT08U)(char_3))) | \
                                                                 ((CPU_INT32U)((CPU_INT08U)(char_4)) << (1u * DEF_OCTET_NBR_BITS)))

#else                                                           /* Dflt CPU_WORD_SIZE_08.                               */
#define  CPU_TYPE_CREATE(char_1, char_2, char_3, char_4)        (((CPU_INT32U)((CPU_INT08U)(char_1)) << (3u * DEF_OCTET_NBR_BITS)) | \
                                                                 ((CPU_INT32U)((CPU_INT08U)(char_2)) << (2u * DEF_OCTET_NBR_BITS)) | \
                                                                 ((CPU_INT32U)((CPU_INT08U)(char_3)) << (1u * DEF_OCTET_NBR_BITS)) | \
                                                                 ((CPU_INT32U)((CPU_INT08U)(char_4))))
#endif
#endif

CPU_DATA         CPU_CntLeadZeros         (CPU_DATA    val);
CPU_DATA         CPU_CntLeadZeros32       (CPU_INT32U  val);

#endif                                                          /* End of CPU core module include.                      */

