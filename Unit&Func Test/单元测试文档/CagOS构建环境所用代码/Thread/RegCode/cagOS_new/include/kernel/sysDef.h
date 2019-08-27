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

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This CPU definition header file is protected from multiple pre-processor inclusion
*               through use of the CPU definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  CPU_DEF_MODULE_PRESENT
#define  CPU_DEF_MODULE_PRESENT

/*
*********************************************************************************************************
*                                       CPU WORD CONFIGURATION
*
* Note(s) : (1) Configure CPU_CFG_ADDR_SIZE & CPU_CFG_DATA_SIZE in 'cpu.h' with CPU's word sizes :
*
*                   CPU_WORD_SIZE_08             8-bit word size
*                   CPU_WORD_SIZE_16            16-bit word size
*                   CPU_WORD_SIZE_32            32-bit word size
*                   CPU_WORD_SIZE_64            64-bit word size
*
*           (2) Configure CPU_CFG_ENDIAN_TYPE in 'cpu.h' with CPU's data-word-memory order :
*
*               (a) CPU_ENDIAN_TYPE_BIG         Big-   endian word order (CPU words' most  significant
*                                                                         octet @ lowest memory address)
*               (b) CPU_ENDIAN_TYPE_LITTLE      Little-endian word order (CPU words' least significant
*                                                                         octet @ lowest memory address)
*********************************************************************************************************
*/

                                                        /* ---------------------- CPU WORD SIZE ----------------------- */
#define  CPU_WORD_SIZE_08                          1u   /*  8-bit word size (in octets).                                */
#define  CPU_WORD_SIZE_16                          2u   /* 16-bit word size (in octets).                                */
#define  CPU_WORD_SIZE_32                          4u   /* 32-bit word size (in octets).                                */
#define  CPU_WORD_SIZE_64                          8u   /* 64-bit word size (in octets).                                */


                                                        /* ------------------ CPU WORD-ENDIAN ORDER ------------------- */
#define  CPU_ENDIAN_TYPE_NONE                      0u
#define  CPU_ENDIAN_TYPE_BIG                       1u   /* Big-   endian word order (see Note #1a).                     */
#define  CPU_ENDIAN_TYPE_LITTLE                    2u   /* Little-endian word order (see Note #1b).                     */


/*
*********************************************************************************************************
*                                       CPU STACK CONFIGURATION
*
* Note(s) : (1) Configure CPU_CFG_STK_GROWTH in 'cpu.h' with CPU's stack growth order :
*
*               (a) CPU_STK_GROWTH_LO_TO_HI     CPU stack pointer increments to the next higher  stack
*                                                   memory address after data is pushed onto the stack
*               (b) CPU_STK_GROWTH_HI_TO_LO     CPU stack pointer decrements to the next lower   stack
*                                                   memory address after data is pushed onto the stack
*********************************************************************************************************
*/

                                                        /* ------------------ CPU STACK GROWTH ORDER ------------------ */
#define  CPU_STK_GROWTH_NONE                       0u
#define  CPU_STK_GROWTH_LO_TO_HI                   1u   /* CPU stk incs towards higher mem addrs (see Note #1a).        */
#define  CPU_STK_GROWTH_HI_TO_LO                   2u   /* CPU stk decs towards lower  mem addrs (see Note #1b).        */


/*
*********************************************************************************************************
*                                   CRITICAL SECTION CONFIGURATION
*
* Note(s) : (1) Configure CPU_CFG_CRITICAL_METHOD with CPU's/compiler's critical section method :
*
*                                                       Enter/Exit critical sections by ...
*
*                   CPU_CRITICAL_METHOD_INT_DIS_EN      Disable/Enable interrupts
*                   CPU_CRITICAL_METHOD_STATUS_STK      Push/Pop       interrupt status onto stack
*                   CPU_CRITICAL_METHOD_STATUS_LOCAL    Save/Restore   interrupt status to local variable
*
*               (a) CPU_CRITICAL_METHOD_INT_DIS_EN  is NOT a preferred method since it does NOT support
*                   multiple levels of interrupts.  However, with some CPUs/compilers, this is the only
*                   available method.
*
*               (b) CPU_CRITICAL_METHOD_STATUS_STK    is one preferred method since it supports multiple
*                   levels of interrupts.  However, this method assumes that the compiler provides C-level
*                   &/or assembly-level functionality for the following :
*
*                     ENTER CRITICAL SECTION :
*                       (1) Push/save   interrupt status onto a local stack
*                       (2) Disable     interrupts
*
*                     EXIT  CRITICAL SECTION :
*                       (3) Pop/restore interrupt status from a local stack
*
*               (c) CPU_CRITICAL_METHOD_STATUS_LOCAL  is one preferred method since it supports multiple
*                   levels of interrupts.  However, this method assumes that the compiler provides C-level
*                   &/or assembly-level functionality for the following :
*
*                     ENTER CRITICAL SECTION :
*                       (1) Save    interrupt status into a local variable
*                       (2) Disable interrupts
*
*                     EXIT  CRITICAL SECTION :
*                       (3) Restore interrupt status from a local variable
*
*           (2) Critical section macro's most likely require inline assembly.  If the compiler does NOT
*               allow inline assembly in C source files, critical section macro's MUST call an assembly
*               subroutine defined in a 'cpu_a.asm' file located in the following software directory :
*
*                   \<CPU-Compiler Directory>\<cpu>\<compiler>\
*
*                       where
*                               <CPU-Compiler Directory>    directory path for common   CPU-compiler software
*                               <cpu>                       directory name for specific CPU
*                               <compiler>                  directory name for specific compiler
*
*           (3) (a) To save/restore interrupt status, a local variable 'cpu_sr' of type 'CPU_SR' MAY need
*                   to be declared (e.g. if 'CPU_CRITICAL_METHOD_STATUS_LOCAL' method is configured).
*
*                   (1) 'cpu_sr' local variable SHOULD be declared via the CPU_SR_ALLOC() macro which,
*                        if used, MUST be declared following ALL other local variables (see any 'cpu.h
*                        CRITICAL SECTION CONFIGURATION  Note #3a1').
*
*                        Example :
*
*                           void  Fnct (void)
*                           {
*                               CPU_INT08U  val_08;
*                               CPU_INT16U  val_16;
*                               CPU_INT32U  val_32;
*                               CPU_SR_ALLOC();         MUST be declared after ALL other local variables
*                                   :
*                                   :
*                           }
*
*               (b) Configure 'CPU_SR' data type with the appropriate-sized CPU data type large enough to
*                   completely store the CPU's/compiler's status word.
*********************************************************************************************************
*/

                                                        /* --------------- CPU CRITICAL SECTION METHODS --------------- */
#define  CPU_CRITICAL_METHOD_NONE                  0u   /*                                                              */
#define  CPU_CRITICAL_METHOD_INT_DIS_EN            1u   /* DIS/EN       ints                    (see Note #1a).         */
#define  CPU_CRITICAL_METHOD_STATUS_STK            2u   /* Push/Pop     int status onto stk     (see Note #1b).         */
#define  CPU_CRITICAL_METHOD_STATUS_LOCAL          3u   /* Save/Restore int status to local var (see Note #1c).         */

/*
*********************************************************************************************************
*                                          STANDARD DEFINES
*********************************************************************************************************
*/

#define  DEF_NULL                                          0


                                                                /* ----------------- BOOLEAN DEFINES ------------------ */
#define  DEF_FALSE                                         0u
#define  DEF_TRUE                                          1u

#define  DEF_NO                                            0u
#define  DEF_YES                                           1u

#define  DEF_DISABLED                                      0u
#define  DEF_ENABLED                                       1u

#define  DEF_INACTIVE                                      0u
#define  DEF_ACTIVE                                        1u

#define  DEF_INVALID                                       0u
#define  DEF_VALID                                         1u

#define  DEF_OFF                                           0u
#define  DEF_ON                                            1u

#define  DEF_CLR                                           0u
#define  DEF_SET                                           1u

#define  DEF_FAIL                                          0u
#define  DEF_OK                                            1u


                                                                /* ------------------- BIT DEFINES -------------------- */
#define  DEF_BIT_NONE                                   0x00u

#define  DEF_BIT_00                                     0x01u
#define  DEF_BIT_01                                     0x02u
#define  DEF_BIT_02                                     0x04u
#define  DEF_BIT_03                                     0x08u
#define  DEF_BIT_04                                     0x10u
#define  DEF_BIT_05                                     0x20u
#define  DEF_BIT_06                                     0x40u
#define  DEF_BIT_07                                     0x80u

#define  DEF_BIT_08                                   0x0100u
#define  DEF_BIT_09                                   0x0200u
#define  DEF_BIT_10                                   0x0400u
#define  DEF_BIT_11                                   0x0800u
#define  DEF_BIT_12                                   0x1000u
#define  DEF_BIT_13                                   0x2000u
#define  DEF_BIT_14                                   0x4000u
#define  DEF_BIT_15                                   0x8000u

#define  DEF_BIT_16                               0x00010000u
#define  DEF_BIT_17                               0x00020000u
#define  DEF_BIT_18                               0x00040000u
#define  DEF_BIT_19                               0x00080000u
#define  DEF_BIT_20                               0x00100000u
#define  DEF_BIT_21                               0x00200000u
#define  DEF_BIT_22                               0x00400000u
#define  DEF_BIT_23                               0x00800000u

#define  DEF_BIT_24                               0x01000000u
#define  DEF_BIT_25                               0x02000000u
#define  DEF_BIT_26                               0x04000000u
#define  DEF_BIT_27                               0x08000000u
#define  DEF_BIT_28                               0x10000000u
#define  DEF_BIT_29                               0x20000000u
#define  DEF_BIT_30                               0x40000000u
#define  DEF_BIT_31                               0x80000000u
#define  DEF_BIT_32                       0x0000000100000000u
#define  DEF_BIT_33                       0x0000000200000000u
#define  DEF_BIT_34                       0x0000000400000000u
#define  DEF_BIT_35                       0x0000000800000000u
#define  DEF_BIT_36                       0x0000001000000000u
#define  DEF_BIT_37                       0x0000002000000000u
#define  DEF_BIT_38                       0x0000004000000000u
#define  DEF_BIT_39                       0x0000008000000000u

#define  DEF_BIT_40                       0x0000010000000000u
#define  DEF_BIT_41                       0x0000020000000000u
#define  DEF_BIT_42                       0x0000040000000000u
#define  DEF_BIT_43                       0x0000080000000000u
#define  DEF_BIT_44                       0x0000100000000000u
#define  DEF_BIT_45                       0x0000200000000000u
#define  DEF_BIT_46                       0x0000400000000000u
#define  DEF_BIT_47                       0x0000800000000000u

#define  DEF_BIT_48                       0x0001000000000000u
#define  DEF_BIT_49                       0x0002000000000000u
#define  DEF_BIT_50                       0x0004000000000000u
#define  DEF_BIT_51                       0x0008000000000000u
#define  DEF_BIT_52                       0x0010000000000000u
#define  DEF_BIT_53                       0x0020000000000000u
#define  DEF_BIT_54                       0x0040000000000000u
#define  DEF_BIT_55                       0x0080000000000000u

#define  DEF_BIT_56                       0x0100000000000000u
#define  DEF_BIT_57                       0x0200000000000000u
#define  DEF_BIT_58                       0x0400000000000000u
#define  DEF_BIT_59                       0x0800000000000000u
#define  DEF_BIT_60                       0x1000000000000000u
#define  DEF_BIT_61                       0x2000000000000000u
#define  DEF_BIT_62                       0x4000000000000000u
#define  DEF_BIT_63                       0x8000000000000000u


                                                                /* ------------------ ALIGN DEFINES ------------------- */
#define  DEF_ALIGN_MAX_NBR_OCTETS                       4096u


                                                                /* ------------------ OCTET DEFINES ------------------- */
#define  DEF_OCTET_NBR_BITS                                8u
#define  DEF_OCTET_MASK                                 0xFFu

#define  DEF_OCTET_TO_BIT_NBR_BITS                         3u
#define  DEF_OCTET_TO_BIT_SHIFT                          DEF_OCTET_TO_BIT_NBR_BITS
#define  DEF_OCTET_TO_BIT_MASK                          0x07u


#define  DEF_NIBBLE_NBR_BITS                               4u
#define  DEF_NIBBLE_MASK                                0x0Fu


                                                                /* --------------- NUMBER BASE DEFINES ---------------- */
#define  DEF_NBR_BASE_BIN                                  2u
#define  DEF_NBR_BASE_OCT                                  8u
#define  DEF_NBR_BASE_DEC                                 10u
#define  DEF_NBR_BASE_HEX                                 16u


                                                                /* ----------------- INTEGER DEFINES ------------------ */
#define  DEF_INT_08_NBR_BITS                               8u
#define  DEF_INT_08_MASK                                0xFFu

#define  DEF_INT_08U_MIN_VAL                               0u
#define  DEF_INT_08U_MAX_VAL                             255u

#define  DEF_INT_08S_MIN_VAL_ONES_CPL                  (-127)
#define  DEF_INT_08S_MAX_VAL_ONES_CPL                    127

#define  DEF_INT_08S_MIN_VAL                            (DEF_INT_08S_MIN_VAL_ONES_CPL - 1)
#define  DEF_INT_08S_MAX_VAL                             DEF_INT_08S_MAX_VAL_ONES_CPL

#define  DEF_INT_08U_NBR_DIG_MIN                           1u
#define  DEF_INT_08U_NBR_DIG_MAX                           3u

#define  DEF_INT_08S_NBR_DIG_MIN                           3u
#define  DEF_INT_08S_NBR_DIG_MAX                           3u



#define  DEF_INT_16_NBR_BITS                              16u
#define  DEF_INT_16_MASK                              0xFFFFu

#define  DEF_INT_16U_MIN_VAL                               0u
#define  DEF_INT_16U_MAX_VAL                           65535u

#define  DEF_INT_16S_MIN_VAL_ONES_CPL                (-32767)
#define  DEF_INT_16S_MAX_VAL_ONES_CPL                  32767

#define  DEF_INT_16S_MIN_VAL                            (DEF_INT_16S_MIN_VAL_ONES_CPL - 1)
#define  DEF_INT_16S_MAX_VAL                             DEF_INT_16S_MAX_VAL_ONES_CPL

#define  DEF_INT_16U_NBR_DIG_MIN                           1u
#define  DEF_INT_16U_NBR_DIG_MAX                           5u

#define  DEF_INT_16S_NBR_DIG_MIN                           5u
#define  DEF_INT_16S_NBR_DIG_MAX                           5u



#define  DEF_INT_32_NBR_BITS                              32u
#define  DEF_INT_32_MASK                          0xFFFFFFFFu

#define  DEF_INT_32U_MIN_VAL                               0u
#define  DEF_INT_32U_MAX_VAL                      4294967295u

#define  DEF_INT_32S_MIN_VAL_ONES_CPL           (-2147483647)
#define  DEF_INT_32S_MAX_VAL_ONES_CPL             2147483647

#define  DEF_INT_32S_MIN_VAL                            (DEF_INT_32S_MIN_VAL_ONES_CPL - 1)
#define  DEF_INT_32S_MAX_VAL                             DEF_INT_32S_MAX_VAL_ONES_CPL

#define  DEF_INT_32U_NBR_DIG_MIN                           1u
#define  DEF_INT_32U_NBR_DIG_MAX                          10u

#define  DEF_INT_32S_NBR_DIG_MIN                          10u
#define  DEF_INT_32S_NBR_DIG_MAX                          10u



#define  DEF_INT_64_NBR_BITS                              64u
#define  DEF_INT_64_MASK                  0xFFFFFFFFFFFFFFFFu

#define  DEF_INT_64U_MIN_VAL                               0u
#define  DEF_INT_64U_MAX_VAL            18446744073709551615u

#define  DEF_INT_64S_MIN_VAL_ONES_CPL  (-9223372036854775807)
#define  DEF_INT_64S_MAX_VAL_ONES_CPL    9223372036854775807

#define  DEF_INT_64S_MIN_VAL                            (DEF_INT_64S_MIN_VAL_ONES_CPL - 1)
#define  DEF_INT_64S_MAX_VAL                             DEF_INT_64S_MAX_VAL_ONES_CPL

#define  DEF_INT_64U_NBR_DIG_MIN                           1u
#define  DEF_INT_64U_NBR_DIG_MAX                          20u

#define  DEF_INT_64S_NBR_DIG_MIN                          19u
#define  DEF_INT_64S_NBR_DIG_MAX                          19u


                                                                /* --------------- CPU INTEGER DEFINES ---------------- */
#define  DEF_INT_CPU_NBR_BITS                           (CPU_CFG_DATA_SIZE     * DEF_OCTET_NBR_BITS)
#define  DEF_INT_CPU_NBR_BITS_MAX                       (CPU_CFG_DATA_SIZE_MAX * DEF_OCTET_NBR_BITS)



#if     (DEF_INT_CPU_NBR_BITS == DEF_INT_08_NBR_BITS)


#define  DEF_INT_CPU_MASK                                DEF_INT_08_MASK

#define  DEF_INT_CPU_U_MIN_VAL                           DEF_INT_08U_MIN_VAL
#define  DEF_INT_CPU_U_MAX_VAL                           DEF_INT_08U_MAX_VAL

#define  DEF_INT_CPU_S_MIN_VAL                           DEF_INT_08S_MIN_VAL
#define  DEF_INT_CPU_S_MAX_VAL                           DEF_INT_08S_MAX_VAL

#define  DEF_INT_CPU_S_MIN_VAL_ONES_CPL                  DEF_INT_08S_MIN_VAL_ONES_CPL
#define  DEF_INT_CPU_S_MAX_VAL_ONES_CPL                  DEF_INT_08S_MAX_VAL_ONES_CPL



#elif   (DEF_INT_CPU_NBR_BITS == DEF_INT_16_NBR_BITS)


#define  DEF_INT_CPU_MASK                                DEF_INT_16_MASK

#define  DEF_INT_CPU_U_MIN_VAL                           DEF_INT_16U_MIN_VAL
#define  DEF_INT_CPU_U_MAX_VAL                           DEF_INT_16U_MAX_VAL

#define  DEF_INT_CPU_S_MIN_VAL                           DEF_INT_16S_MIN_VAL
#define  DEF_INT_CPU_S_MAX_VAL                           DEF_INT_16S_MAX_VAL

#define  DEF_INT_CPU_S_MIN_VAL_ONES_CPL                  DEF_INT_16S_MIN_VAL_ONES_CPL
#define  DEF_INT_CPU_S_MAX_VAL_ONES_CPL                  DEF_INT_16S_MAX_VAL_ONES_CPL



#elif   (DEF_INT_CPU_NBR_BITS == DEF_INT_32_NBR_BITS)


#define  DEF_INT_CPU_MASK                                DEF_INT_32_MASK

#define  DEF_INT_CPU_U_MIN_VAL                           DEF_INT_32U_MIN_VAL
#define  DEF_INT_CPU_U_MAX_VAL                           DEF_INT_32U_MAX_VAL

#define  DEF_INT_CPU_S_MIN_VAL                           DEF_INT_32S_MIN_VAL
#define  DEF_INT_CPU_S_MAX_VAL                           DEF_INT_32S_MAX_VAL

#define  DEF_INT_CPU_S_MIN_VAL_ONES_CPL                  DEF_INT_32S_MIN_VAL_ONES_CPL
#define  DEF_INT_CPU_S_MAX_VAL_ONES_CPL                  DEF_INT_32S_MAX_VAL_ONES_CPL



#elif   (DEF_INT_CPU_NBR_BITS == DEF_INT_64_NBR_BITS)


#define  DEF_INT_CPU_MASK                                DEF_INT_64_MASK

#define  DEF_INT_CPU_U_MIN_VAL                           DEF_INT_64U_MIN_VAL
#define  DEF_INT_CPU_U_MAX_VAL                           DEF_INT_64U_MAX_VAL

#define  DEF_INT_CPU_S_MIN_VAL                           DEF_INT_64S_MIN_VAL
#define  DEF_INT_CPU_S_MAX_VAL                           DEF_INT_64S_MAX_VAL

#define  DEF_INT_CPU_S_MIN_VAL_ONES_CPL                  DEF_INT_64S_MIN_VAL_ONES_CPL
#define  DEF_INT_CPU_S_MAX_VAL_ONES_CPL                  DEF_INT_64S_MAX_VAL_ONES_CPL



#else

#error  "CPU_CFG_DATA_SIZE  illegally #defined in 'cpu.h'      "
#error  "                   [See 'cpu.h  CONFIGURATION ERRORS']"

#endif

/*
*********************************************************************************************************
*                                             BIT MACRO'S
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              DEF_BIT()
*
* Description : Create bit mask with single, specified bit set.
*
* Argument(s) : bit         Bit number of bit to set.
*
* Return(s)   : Bit mask with single, specified bit set.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'bit' SHOULD be a non-negative integer.
*
*               (2) (a) 'bit' values that overflow the target CPU &/or compiler environment (e.g. negative
*                       or greater-than-CPU-data-size values) MAY generate compiler warnings &/or errors.
*********************************************************************************************************
*/

#define  DEF_BIT(bit)                                                   (1u << (bit))


/*
*********************************************************************************************************
*                                             DEF_BITxx()
*
* Description : Create bit mask of specified bit size with single, specified bit set.
*
* Argument(s) : bit         Bit number of bit to set.
*
* Return(s)   : Bit mask with single, specified bit set.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'bit' SHOULD be a non-negative integer.
*
*               (2) (a) 'bit' values that overflow the target CPU &/or compiler environment (e.g. negative
*                       or greater-than-CPU-data-size values) MAY generate compiler warnings &/or errors.
*
*                   (b) To avoid overflowing any target CPU &/or compiler's integer data type, unsigned
*                       bit constant '1' is cast to specified integer data type size.
*
*               (3) Ideally, DEF_BITxx() macro's should be named DEF_BIT_xx(); however, these names already
*                   previously-released for bit constant #define's (see 'STANDARD DEFINES  BIT DEFINES').
*********************************************************************************************************
*/

#define  DEF_BIT08(bit)                        ((CPU_INT08U)((CPU_INT08U)1u  << (bit)))

#define  DEF_BIT16(bit)                        ((CPU_INT16U)((CPU_INT16U)1u  << (bit)))

#define  DEF_BIT32(bit)                        ((CPU_INT32U)((CPU_INT32U)1u  << (bit)))

#define  DEF_BIT64(bit)                        ((CPU_INT64U)((CPU_INT64U)1u  << (bit)))


/*
*********************************************************************************************************
*                                           DEF_BIT_MASK()
*
* Description : Shift a bit mask.
*
* Argument(s) : bit_mask    Bit mask to shift.
*
*               bit_shift   Number of bit positions to left-shift bit mask.
*
* Return(s)   : Shifted bit mask.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) (a) 'bit_mask'  SHOULD be an unsigned    integer.
*
*                   (b) 'bit_shift' SHOULD be a non-negative integer.
*
*               (2) 'bit_shift' values that overflow the target CPU &/or compiler environment (e.g. negative
*                   or greater-than-CPU-data-size values) MAY generate compiler warnings &/or errors.
*********************************************************************************************************
*/

#define  DEF_BIT_MASK(bit_mask, bit_shift)                                     ((bit_mask) << (bit_shift))


/*
*********************************************************************************************************
*                                          DEF_BIT_MASK_xx()
*
* Description : Shift a bit mask of specified bit size.
*
* Argument(s) : bit_mask    Bit mask to shift.
*
*               bit_shift   Number of bit positions to left-shift bit mask.
*
* Return(s)   : Shifted bit mask.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) (a) 'bit_mask'  SHOULD be an unsigned    integer.
*
*                   (b) 'bit_shift' SHOULD be a non-negative integer.
*
*               (2) 'bit_shift' values that overflow the target CPU &/or compiler environment (e.g. negative
*                   or greater-than-CPU-data-size values) MAY generate compiler warnings &/or errors.
*********************************************************************************************************
*/

#define  DEF_BIT_MASK_08(bit_mask, bit_shift)         ((CPU_INT08U)((CPU_INT08U)(bit_mask) << (bit_shift)))

#define  DEF_BIT_MASK_16(bit_mask, bit_shift)         ((CPU_INT16U)((CPU_INT16U)(bit_mask) << (bit_shift)))

#define  DEF_BIT_MASK_32(bit_mask, bit_shift)         ((CPU_INT32U)((CPU_INT32U)(bit_mask) << (bit_shift)))

#define  DEF_BIT_MASK_64(bit_mask, bit_shift)         ((CPU_INT64U)((CPU_INT64U)(bit_mask) << (bit_shift)))


/*
*********************************************************************************************************
*                                           DEF_BIT_FIELD()
*
* Description : Create & shift a contiguous bit field.
*
* Argument(s) : bit_field   Number of contiguous bits to set in the bit field.
*
*               bit_shift   Number of bit positions   to left-shift bit field.
*
* Return(s)   : Shifted bit field.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'bit_field' & 'bit_shift' SHOULD be non-negative integers.
*
*               (2) (a) 'bit_field'/'bit_shift' values that overflow the target CPU &/or compiler
*                       environment (e.g. negative or greater-than-CPU-data-size values) MAY generate
*                       compiler warnings &/or errors.
*
*                   (b) To avoid overflowing any target CPU &/or compiler's integer data type, unsigned
*                       bit constant '1' is suffixed with 'L'ong integer modifier.
*
*                       This may still be insufficient for CPUs &/or compilers that support 'long long'
*                       integer data types, in which case 'LL' integer modifier should be suffixed.
*                       However, since almost all 16- & 32-bit CPUs & compilers support 'long' integer
*                       data types but many may NOT support 'long long' integer data types, only 'long'
*                       integer data types & modifiers are supported.
*
*                       See also 'DEF_BIT_FIELD_xx()  Note #1b'.
*********************************************************************************************************
*/

#define  DEF_BIT_FIELD(bit_field, bit_shift)                                 ((((bit_field) >= DEF_INT_CPU_NBR_BITS) ? (DEF_INT_CPU_U_MAX_VAL)     \
                                                                                                                     : (DEF_BIT(bit_field) - 1uL)) \
                                                                                                                            << (bit_shift))

/*
*********************************************************************************************************
*                                         DEF_BIT_FIELD_xx()
*
* Description : Create & shift a contiguous bit field of specified bit size.
*
* Argument(s) : bit_field   Number of contiguous bits to set in the bit field.
*
*               bit_shift   Number of bit positions   to left-shift bit field.
*
* Return(s)   : Shifted bit field.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'bit_field' & 'bit_shift' SHOULD be non-negative integers.
*
*               (2) (a) 'bit_field'/'bit_shift' values that overflow the target CPU &/or compiler
*                       environment (e.g. negative or greater-than-CPU-data-size values) MAY generate
*                       compiler warnings &/or errors.
*
*                   (b) To avoid overflowing any target CPU &/or compiler's integer data type, unsigned
*                       bit constant '1' is cast to specified integer data type size.
*********************************************************************************************************
*/

#define  DEF_BIT_FIELD_08(bit_field, bit_shift)     ((CPU_INT08U)((((CPU_INT08U)(bit_field) >= (CPU_INT08U)DEF_INT_08_NBR_BITS) ? (CPU_INT08U)(DEF_INT_08U_MAX_VAL)                    \
                                                                                                                                : (CPU_INT08U)(DEF_BIT08(bit_field) - (CPU_INT08U)1u)) \
                                                                                                                                                     << (bit_shift)))

#define  DEF_BIT_FIELD_16(bit_field, bit_shift)     ((CPU_INT16U)((((CPU_INT16U)(bit_field) >= (CPU_INT16U)DEF_INT_16_NBR_BITS) ? (CPU_INT16U)(DEF_INT_16U_MAX_VAL)                    \
                                                                                                                                : (CPU_INT16U)(DEF_BIT16(bit_field) - (CPU_INT16U)1u)) \
                                                                                                                                                     << (bit_shift)))

#define  DEF_BIT_FIELD_32(bit_field, bit_shift)     ((CPU_INT32U)((((CPU_INT32U)(bit_field) >= (CPU_INT32U)DEF_INT_32_NBR_BITS) ? (CPU_INT32U)(DEF_INT_32U_MAX_VAL)                    \
                                                                                                                                : (CPU_INT32U)(DEF_BIT32(bit_field) - (CPU_INT32U)1u)) \
                                                                                                                                                     << (bit_shift)))

#define  DEF_BIT_FIELD_64(bit_field, bit_shift)     ((CPU_INT64U)((((CPU_INT64U)(bit_field) >= (CPU_INT64U)DEF_INT_64_NBR_BITS) ? (CPU_INT64U)(DEF_INT_64U_MAX_VAL)                    \
                                                                                                                                : (CPU_INT64U)(DEF_BIT64(bit_field) - (CPU_INT64U)1u)) \
                                                                                                                                                     << (bit_shift)))


/*
*********************************************************************************************************
*                                            DEF_BIT_SET()
*
* Description : Set specified bit(s) in a value.
*
* Argument(s) : val         Value to modify by setting specified bit(s).
*
*               mask        Mask of bits to set.
*
* Return(s)   : Modified value with specified bit(s) set.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'val' & 'mask' SHOULD be unsigned integers.
*********************************************************************************************************
*/

#define  DEF_BIT_SET(val, mask)                        ((val) = ((val) | (mask)))


/*
*********************************************************************************************************
*                                          DEF_BIT_SET_xx()
*
* Description : Set specified bit(s) in a value of specified bit size.
*
* Argument(s) : val         Value to modify by setting specified bit(s).
*
*               mask        Mask of bits to set.
*
* Return(s)   : Modified value with specified bit(s) set.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'val' & 'mask' SHOULD be unsigned integers.
*
*               (2) These macros are deprecated and should be replaced by the DEF_BIT_SET macro.
*********************************************************************************************************
*/

#define  DEF_BIT_SET_08(val, mask)                     DEF_BIT_SET((val), (mask))

#define  DEF_BIT_SET_16(val, mask)                     DEF_BIT_SET((val), (mask))

#define  DEF_BIT_SET_32(val, mask)                     DEF_BIT_SET((val), (mask))

#define  DEF_BIT_SET_64(val, mask)                     DEF_BIT_SET((val), (mask))


/*
*********************************************************************************************************
*                                          DEF_BIT_CLR_xx()
*
* Description : Clear specified bit(s) in a value of specified bit size.
*
* Argument(s) : val         Value to modify by clearing specified bit(s).
*
*               mask        Mask of bits to clear.
*
* Return(s)   : Modified value with specified bit(s) clear.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'val' & 'mask' SHOULD be unsigned integers.
*********************************************************************************************************
*/

#if (CPU_CFG_DATA_SIZE > CPU_WORD_SIZE_08)
#define  DEF_BIT_CLR_08(val, mask)                     ((val) = ((val) & (~(mask))))
#else
#define  DEF_BIT_CLR_08(val, mask)                     ((val) = ((val) & (~((CPU_INT08U)mask))))
#endif

#if (CPU_CFG_DATA_SIZE > CPU_WORD_SIZE_16)
#define  DEF_BIT_CLR_16(val, mask)                     ((val) = ((val) & (~(mask))))
#else
#define  DEF_BIT_CLR_16(val, mask)                     ((val) = ((val) & (~((CPU_INT16U)mask))))
#endif

#if (CPU_CFG_DATA_SIZE > CPU_WORD_SIZE_32)
#define  DEF_BIT_CLR_32(val, mask)                     ((val) = ((val) & (~(mask))))
#else
#define  DEF_BIT_CLR_32(val, mask)                     ((val) = ((val) & (~((CPU_INT32U)mask))))
#endif

#define  DEF_BIT_CLR_64(val, mask)                     ((val) = ((val) & (~((CPU_INT64U)mask))))


/*
*********************************************************************************************************
*                                            DEF_BIT_CLR()
*
* Description : Clear specified bit(s) in a value.
*
* Argument(s) : val         Value to modify by clearing specified bit(s).
*
*               mask        Mask of bits to clear.
*
* Return(s)   : Modified value with specified bit(s) clear.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'val' & 'mask' SHOULD be unsigned integers.
*********************************************************************************************************
*/

#if     (CPU_CFG_DATA_SIZE_MAX == CPU_WORD_SIZE_08)

#define  DEF_BIT_CLR(val, mask)                 ((sizeof(val) == CPU_WORD_SIZE_08) ? DEF_BIT_CLR_08(val, mask) : 0)


#elif   (CPU_CFG_DATA_SIZE_MAX == CPU_WORD_SIZE_16)

#define  DEF_BIT_CLR(val, mask)                 ((sizeof(val) == CPU_WORD_SIZE_08) ? DEF_BIT_CLR_08(val, mask) :   \
                                                ((sizeof(val) == CPU_WORD_SIZE_16) ? DEF_BIT_CLR_16(val, mask) : 0))


#elif   (CPU_CFG_DATA_SIZE_MAX == CPU_WORD_SIZE_32)

#define  DEF_BIT_CLR(val, mask)                 ((sizeof(val) == CPU_WORD_SIZE_08) ? DEF_BIT_CLR_08(val, mask) :    \
                                                ((sizeof(val) == CPU_WORD_SIZE_16) ? DEF_BIT_CLR_16(val, mask) :    \
                                                ((sizeof(val) == CPU_WORD_SIZE_32) ? DEF_BIT_CLR_32(val, mask) : 0)))


#elif   (CPU_CFG_DATA_SIZE_MAX == CPU_WORD_SIZE_64)

#define  DEF_BIT_CLR(val, mask)                 ((sizeof(val) == CPU_WORD_SIZE_08) ? DEF_BIT_CLR_08(val, mask) :     \
                                                ((sizeof(val) == CPU_WORD_SIZE_16) ? DEF_BIT_CLR_16(val, mask) :     \
                                                ((sizeof(val) == CPU_WORD_SIZE_32) ? DEF_BIT_CLR_32(val, mask) :     \
                                                ((sizeof(val) == CPU_WORD_SIZE_64) ? DEF_BIT_CLR_64(val, mask) : 0))))

#else

#error  "CPU_CFG_DATA_SIZE_MAX  illegally #defined in 'cpu.h'      "
#error  "                       [See 'cpu.h  CONFIGURATION ERRORS']"

#endif


/*
*********************************************************************************************************
*                                            DEF_BIT_TOGGLE()
*
* Description : Toggles specified bit(s) in a value.
*
* Argument(s) : val         Value to modify by toggling specified bit(s).
*
*               mask        Mask of bits to toggle.
*
* Return(s)   : Modified value with specified bit(s) toggled.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'val' & 'mask' SHOULD be unsigned integers.
*********************************************************************************************************
*/

#define  DEF_BIT_TOGGLE(val, mask)                      ((val) ^= (mask))


/*
*********************************************************************************************************
*                                           DEF_BIT_FIELD_RD()
*
* Description : Reads a 'val' field, masked and shifted, given by mask 'field_mask'.
*
* Argument(s) : val         Value to read from.
*
*               field_mask  Mask of field to read. See note #1, #2 and #3.
*
* Return(s)   : Field value, masked and right-shifted to bit position 0.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'field_mask' argument must NOT be 0.
*
*               (2) 'field_mask' argument must contain a mask with contiguous set bits.
*
*               (3) 'val' & 'field_mask' SHOULD be unsigned integers.
*********************************************************************************************************
*/

#define  DEF_BIT_FIELD_RD(val, field_mask)              (((val) & (field_mask)) / ((field_mask) & ~((field_mask) << 1u)))


/*
*********************************************************************************************************
*                                          DEF_BIT_FIELD_ENC()
*
* Description : Encodes given 'field_val' at position given by mask 'field_mask'.
*
* Argument(s) : field_val   Value to encode.
*
*               field_mask  Mask of field to read. See note #1 and #2.
*
* Return(s)   : Field value, masked and left-shifted to field position.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'field_mask' argument must contain a mask with contiguous set bits.
*
*               (2) 'field_val' & 'field_mask' SHOULD be unsigned integers.
*********************************************************************************************************
*/

#define  DEF_BIT_FIELD_ENC(field_val, field_mask)       (((field_val) * ((field_mask) & ~((field_mask) << 1u))) & (field_mask))


/*
*********************************************************************************************************
*                                           DEF_BIT_FIELD_WR()
*
* Description : Writes 'field_val' field at position given by mask 'field_mask' in variable 'var'.
*
* Argument(s) : var         Variable to write field to. See note #2.
*
*               field_val   Desired value for field. See note #2.
*
*               field_mask  Mask of field to write to. See note #1 and #2.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'field_mask' argument must contain a mask with contiguous set bits.
*
*               (2) 'var', 'field_val' & 'field_mask' SHOULD be unsigned integers.
*********************************************************************************************************
*/

#define  DEF_BIT_FIELD_WR(var, field_val, field_mask)   (var) = (((var) & ~(field_mask)) | DEF_BIT_FIELD_ENC((field_val), (field_mask)))


/*
*********************************************************************************************************
*                                          DEF_BIT_IS_SET()
*
* Description : Determine if specified bit(s) in a value are set.
*
* Argument(s) : val         Value to check for specified bit(s) set.
*
*               mask        Mask of bits to check if set (see Note #2).
*
* Return(s)   : DEF_YES, if ALL specified bit(s) are     set in value.
*
*               DEF_NO,  if ALL specified bit(s) are NOT set in value.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'val' & 'mask' SHOULD be unsigned integers.
*
*               (2) NULL 'mask' allowed; returns 'DEF_NO' since NO mask bits specified.
*********************************************************************************************************
*/

#define  DEF_BIT_IS_SET(val, mask)                    (((((val) & (mask)) == (mask)) && \
                                                         ((mask)          !=  0u))    ? (DEF_YES) : (DEF_NO))


/*
*********************************************************************************************************
*                                          DEF_BIT_IS_CLR()
*
* Description : Determine if specified bit(s) in a value are clear.
*
* Argument(s) : val         Value to check for specified bit(s) clear.
*
*               mask        Mask of bits to check if clear (see Note #2).
*
* Return(s)   : DEF_YES, if ALL specified bit(s) are     clear in value.
*
*               DEF_NO,  if ALL specified bit(s) are NOT clear in value.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'val' & 'mask' SHOULD be unsigned integers.
*
*               (2) NULL 'mask' allowed; returns 'DEF_NO' since NO mask bits specified.
*********************************************************************************************************
*/

#define  DEF_BIT_IS_CLR(val, mask)                    (((((val) & (mask)) ==  0u)  && \
                                                         ((mask)          !=  0u))  ? (DEF_YES) : (DEF_NO))


/*
*********************************************************************************************************
*                                        DEF_BIT_IS_SET_ANY()
*
* Description : Determine if any specified bit(s) in a value are set.
*
* Argument(s) : val         Value to check for specified bit(s) set.
*
*               mask        Mask of bits to check if set (see Note #2).
*
* Return(s)   : DEF_YES, if ANY specified bit(s) are     set in value.
*
*               DEF_NO,  if ALL specified bit(s) are NOT set in value.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'val' & 'mask' SHOULD be unsigned integers.
*
*               (2) NULL 'mask' allowed; returns 'DEF_NO' since NO mask bits specified.
*********************************************************************************************************
*/

#define  DEF_BIT_IS_SET_ANY(val, mask)               ((((val) & (mask)) ==  0u)     ? (DEF_NO ) : (DEF_YES))


/*
*********************************************************************************************************
*                                        DEF_BIT_IS_CLR_ANY()
*
* Description : Determine if any specified bit(s) in a value are clear.
*
* Argument(s) : val         Value to check for specified bit(s) clear.
*
*               mask        Mask of bits to check if clear (see Note #2).
*
* Return(s)   : DEF_YES, if ANY specified bit(s) are     clear in value.
*
*               DEF_NO,  if ALL specified bit(s) are NOT clear in value.
*
* Note(s)     : (1) 'val' & 'mask' SHOULD be unsigned integers.
*
*               (2) NULL 'mask' allowed; returns 'DEF_NO' since NO mask bits specified.
*********************************************************************************************************
*/

#define  DEF_BIT_IS_CLR_ANY(val, mask)               ((((val) & (mask)) == (mask))  ? (DEF_NO ) : (DEF_YES))


/*
*********************************************************************************************************
*                                            VALUE MACRO'S
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          DEF_CHK_VAL_MIN()
*
* Description : Validate a value as greater than or equal to a specified minimum value.
*
* Argument(s) : val        Value to validate.
*
*               val_min    Minimum value to test.
*
* Return(s)   : DEF_OK,    Value is greater than or equal to minimum value.
*
*               DEF_FAIL,  otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) DEF_CHK_VAL_MIN() avoids directly comparing any two values if only one of the values
*                   is negative since the negative value might be incorrectly promoted to an arbitrary
*                   unsigned value if the other value to compare is unsigned.
*
*               (2) Validation of values is limited to the range supported by the compiler &/or target
*                   environment.  All other values that underflow/overflow the supported range will
*                   modulo/wrap into the supported range as arbitrary signed or unsigned values.
*
*                   Therefore, any values that underflow the most negative signed value or overflow
*                   the most positive unsigned value supported by the compiler &/or target environment
*                   cannot be validated :
*
*                           (    N-1       N     ]
*                           ( -(2   )  ,  2  - 1 ]
*                           (                    ]
*
*                               where
*                                       N       Number of data word bits supported by the compiler
*                                                   &/or target environment
*
*                   (a) Note that the most negative value, -2^(N-1), is NOT included in the supported
*                       range since many compilers do NOT always correctly handle this value.
*
*               (3) 'val' and 'val_min' are compared to 1 instead of 0 to avoid warning generated for
*                   unsigned numbers.
*********************************************************************************************************
*/

#define  DEF_CHK_VAL_MIN(val, val_min)            (((!(((val)     >= 1) && ((val_min) < 1))) && \
                                                     ((((val_min) >= 1) && ((val)     < 1))  || \
                                                       ((val) < (val_min)))) ? DEF_FAIL : DEF_OK)


/*
*********************************************************************************************************
*                                          DEF_CHK_VAL_MAX()
*
* Description : Validate a value as less than or equal to a specified maximum value.
*
* Argument(s) : val        Value to validate.
*
*               val_max    Maximum value to test.
*
* Return(s)   : DEF_OK,    Value is less than or equal to maximum value.
*
*               DEF_FAIL,  otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) DEF_CHK_VAL_MAX() avoids directly comparing any two values if only one of the values
*                   is negative since the negative value might be incorrectly promoted to an arbitrary
*                   unsigned value if the other value to compare is unsigned.
*
*               (2) Validation of values is limited to the range supported by the compiler &/or target
*                   environment.  All other values that underflow/overflow the supported range will
*                   modulo/wrap into the supported range as arbitrary signed or unsigned values.
*
*                   Therefore, any values that underflow the most negative signed value or overflow
*                   the most positive unsigned value supported by the compiler &/or target environment
*                   cannot be validated :
*
*                           (    N-1       N     ]
*                           ( -(2   )  ,  2  - 1 ]
*                           (                    ]
*
*                               where
*                                       N       Number of data word bits supported by the compiler
*                                                   &/or target environment
*
*                   (a) Note that the most negative value, -2^(N-1), is NOT included in the supported
*                       range since many compilers do NOT always correctly handle this value.
*
*               (3) 'val' and 'val_max' are compared to 1 instead of 0 to avoid warning generated for
*                   unsigned numbers.
*********************************************************************************************************
*/

#define  DEF_CHK_VAL_MAX(val, val_max)            (((!(((val_max) >= 1) && ((val)     < 1))) && \
                                                     ((((val)     >= 1) && ((val_max) < 1))  || \
                                                       ((val) > (val_max)))) ? DEF_FAIL : DEF_OK)


/*
*********************************************************************************************************
*                                            DEF_CHK_VAL()
*
* Description : Validate a value as greater than or equal to a specified minimum value & less than or
*                   equal to a specified maximum value.
*
* Argument(s) : val        Value to validate.
*
*               val_min    Minimum value to test.
*
*               val_max    Maximum value to test.
*
* Return(s)   : DEF_OK,    Value is greater than or equal to minimum value AND
*                                   less    than or equal to maximum value.
*
*               DEF_FAIL,  otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) DEF_CHK_VAL() avoids directly comparing any two values if only one of the values
*                   is negative since the negative value might be incorrectly promoted to an arbitrary
*                   unsigned value if the other value to compare is unsigned.
*
*               (2) Validation of values is limited to the range supported by the compiler &/or target
*                   environment.  All other values that underflow/overflow the supported range will
*                   modulo/wrap into the supported range as arbitrary signed or unsigned values.
*
*                   Therefore, any values that underflow the most negative signed value or overflow
*                   the most positive unsigned value supported by the compiler &/or target environment
*                   cannot be validated :
*
*                           (    N-1       N     ]
*                           ( -(2   )  ,  2  - 1 ]
*                           (                    ]
*
*                               where
*                                       N       Number of data word bits supported by the compiler
*                                                   &/or target environment
*
*                   (a) Note that the most negative value, -2^(N-1), is NOT included in the supported
*                       range since many compilers do NOT always correctly handle this value.
*
*               (3) DEF_CHK_VAL() does NOT validate that the maximum value ('val_max') is greater than
*                   or equal to the minimum value ('val_min').
*********************************************************************************************************
*/

#define  DEF_CHK_VAL(val, val_min, val_max)          (((DEF_CHK_VAL_MIN((val), (val_min)) == DEF_FAIL) ||                  \
                                                       (DEF_CHK_VAL_MAX((val), (val_max)) == DEF_FAIL)) ? DEF_FAIL : DEF_OK)


/*
*********************************************************************************************************
*                                         DEF_GET_U_MAX_VAL()
*
* Description : Get the maximum unsigned value that can be represented in an unsigned integer variable
*                   of the same data type size as an object.
*
* Argument(s) : obj         Object or data type to return maximum unsigned value (see Note #1).
*
* Return(s)   : Maximum unsigned integer value that can be represented by the object, if NO error(s).
*
*               0,                                                                    otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) 'obj' SHOULD be an integer object or data type but COULD also be a character or
*                   pointer object or data type.
*********************************************************************************************************
*/

#if     (CPU_CFG_DATA_SIZE_MAX == CPU_WORD_SIZE_08)

#define  DEF_GET_U_MAX_VAL(obj)                 ((sizeof(obj) == CPU_WORD_SIZE_08) ? DEF_INT_08U_MAX_VAL : 0)


#elif   (CPU_CFG_DATA_SIZE_MAX == CPU_WORD_SIZE_16)

#define  DEF_GET_U_MAX_VAL(obj)                 ((sizeof(obj) == CPU_WORD_SIZE_08) ? DEF_INT_08U_MAX_VAL :   \
                                                ((sizeof(obj) == CPU_WORD_SIZE_16) ? DEF_INT_16U_MAX_VAL : 0))


#elif   (CPU_CFG_DATA_SIZE_MAX == CPU_WORD_SIZE_32)

#define  DEF_GET_U_MAX_VAL(obj)                 ((sizeof(obj) == CPU_WORD_SIZE_08) ? DEF_INT_08U_MAX_VAL :    \
                                                ((sizeof(obj) == CPU_WORD_SIZE_16) ? DEF_INT_16U_MAX_VAL :    \
                                                ((sizeof(obj) == CPU_WORD_SIZE_32) ? DEF_INT_32U_MAX_VAL : 0)))


#elif   (CPU_CFG_DATA_SIZE_MAX == CPU_WORD_SIZE_64)

#define  DEF_GET_U_MAX_VAL(obj)                 ((sizeof(obj) == CPU_WORD_SIZE_08) ? DEF_INT_08U_MAX_VAL :     \
                                                ((sizeof(obj) == CPU_WORD_SIZE_16) ? DEF_INT_16U_MAX_VAL :     \
                                                ((sizeof(obj) == CPU_WORD_SIZE_32) ? DEF_INT_32U_MAX_VAL :     \
                                                ((sizeof(obj) == CPU_WORD_SIZE_64) ? DEF_INT_64U_MAX_VAL : 0))))

#else

#error  "CPU_CFG_DATA_SIZE_MAX  illegally #defined in 'cpu.h'      "
#error  "                       [See 'cpu.h  CONFIGURATION ERRORS']"

#endif

/*
*********************************************************************************************************
*                                             MODULE END
*
* Note(s) : (1) See 'lib_def.h  MODULE'.
*********************************************************************************************************
*/

#endif                                                          /* End of lib def module include.                       */

