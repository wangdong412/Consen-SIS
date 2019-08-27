/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  SYSTEM Global configure
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
 * This file contains the board clock component
 */

 /* system layout configure */
 
#define _CAG_CONFIG_FLASH_SIZE 0x01000000
#define _CAG_CONFIG_FLASH_BASE_ADRS 0xEC000000
#define _CAG_CONFIG_HEAP_SIZE 0x200000
#define _CAG_CONFIG_CCSBAR 0xFFE00000
#define _CAG_CONFIG_ROM_TEXT_ADRS 0xFFE00000
#define _CAG_CONFIG_SIO_BAUDRATE 115200
#define _CAG_CONFIG_FLASH_SECTOR_SIZE 0x20000
#define _CAG_CONFIG_LINK_ADDR 0x00000000

#if (_CAG_START_TEXT == 0x100000)
#define _CAG_CONFIG_SIO_BASE 0xFFE04500
#else
#define _CAG_CONFIG_SIO_BASE 0xFFE04600
#endif

/* Function Enable */

#define _CAG_CONFIG_NS16550_SIO 1
#define _CAG_CONFIG_SIO_REG_INTERVAL 1
#define _CAG_CONFIG_SIO 1
#define _CAG_CONFIG_ARCH_HAS_MEM_ACCESSORS 0
#define _CAG_SPE_SUPPORT 1
#define _CAG_CONFIG_KERNEL 1
#define _CAG_CONFIG_VSNPRINTF 1
#define _CAG_CONFIG_HANG 1
#define _CAG_CONFIG_FREQ 1

#define _CAG_CONFIG_SHELL 1

#if (_CAG_START_TEXT == 0x100000)
#define _CAG_CONFIG_AMP0 1
#undef _CAG_CONFIG_AMP1
#else
#undef _CAG_CONFIG_AMP0
#define _CAG_CONFIG_AMP1 1
#endif

/* the taget help configure */

#define _CAG_CONFIG_CPU_NUM 0x2
#define _CAG_CONFIG_SHELL_PROMPT "cagOs$"

/* system DDR MAP */

#define L2_CACHE_SIZE          2
#define USER_RESERVED_MEM      0x00000000
#define LOCAL_MEM_SIZE 	       0x20000000
#define LOCAL_MEM_LOCAL_ADRS   _CAG_START_TEXT
#define VEC_BASE_ADRS LOCAL_MEM_LOCAL_ADRS

/* AMP for CORE0/1 */

#define TASK_STACK_SIZE       0x00040000
#define AMP_KERNEL_SWAP       0x01E00000
#define TASK_SPESTACK_HIGH    (_CAG_START_TEXT + 0x01A00000)
#define TASK_SPESTACK_LOW     (_CAG_START_TEXT + 0x01900000)
#define TASK_STACK_HIGH       (_CAG_START_TEXT + 0x01900000)
#define TASK_STACK_LOW        (_CAG_START_TEXT + 0x01100000)
#define IRQ_STACK_HIGH        (_CAG_START_TEXT + 0x01100000)
#define IRQ_STACK_LOW         (_CAG_START_TEXT + 0x01000000)
#define EXC_STACK_HIGH        (_CAG_START_TEXT + 0x01000000) 
#define EXC_STACK_LOW         (_CAG_START_TEXT + 0x00F00000)
#define DEC_STACK_HIGH        (_CAG_START_TEXT + 0x00F00000)
#define DEC_STACK_LOW         (_CAG_START_TEXT + 0x00E00000)
#define KERNEL_STACK_HIGH     (_CAG_START_TEXT + 0x00E00000)
#define KERNEL_STACK_LOW      (_CAG_START_TEXT + 0x00A00000)

/*--------------------------*/

#define BOOT_BLOCK_NUM 6 
#define MAX_FILE_SIZE  (2*1024*1024)
#define MAX_BOOT_SIZE  (BOOT_BLOCK_NUM*128*1024)     
#define RUN_START_ADDR (0xb000000+MAX_BOOT_SIZE)
