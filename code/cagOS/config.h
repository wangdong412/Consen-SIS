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
#define _CAG_CONFIG_SRAM_ADDR   0xFF600000
#define _CAG_CONFIG_SRAM_SIZE   0x100000
#define _CAG_CONFIG_FPGA_ADDR   0xFF700000
#define _CAG_CONFIG_FPGA_SIZE   0x10000

#if (_CAG_START_TEXT == 0x04eb0000 || _CAG_START_TEXT == 0x04fe0000 )
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

#define _CAG_CONFIG_SHELL 0
#define _CAG_CONFIG_EPIC  1



#if (_CAG_START_TEXT == 0x04eb0000 || _CAG_START_TEXT == 0x04fe0000)
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

#define TASK_SPESTACK_SIZE      400 /*0x00008000*/
#define TASK_STACK_SIZE         0x10000
#define STASK_SIZE              0x1000
#define GAP_SIZE                0x10
#define CYCLONE_BIN_RAM_HIGH    0x12c00000


#ifdef _CAG_CONFIG_AMP0 
#define GAP_0                 (0x05000000 + 0x00400000) 
#else
#define GAP_0                 (0x01000000 + 0x00580000) 
#endif

#define KERNEL_STACK_LOW      (GAP_0 + GAP_SIZE )
#define KERNEL_STACK_HIGH     (KERNEL_STACK_LOW + 0x08000)

#define GAP_1                 (KERNEL_STACK_HIGH)
#define TASK_STACK_LOW        (GAP_1 + GAP_SIZE)
#ifdef _CAG_CONFIG_AMP0
#define TASK_STACK_HIGH       (TASK_STACK_LOW + 3*(TASK_STACK_SIZE + GAP_SIZE)-GAP_SIZE)
#else
#define TASK_STACK_HIGH       (TASK_STACK_LOW + 4*(TASK_STACK_SIZE + GAP_SIZE)-GAP_SIZE)
#endif

#define GAP_2       (TASK_STACK_HIGH )
#define TASK_CLK_STACK_LOW          (GAP_2+GAP_SIZE)
#define TASK_CLK_STACK_HIGH         (TASK_CLK_STACK_LOW + STASK_SIZE)

#define GAP_3               (TASK_CLK_STACK_HIGH)
#define TASK_IDLE_STACK_LOW         (GAP_3 + GAP_SIZE)
#define TASK_IDLE_STACK_HIGH        (TASK_IDLE_STACK_LOW + STASK_SIZE)

#define GAP_4                       (TASK_IDLE_STACK_HIGH)
#define TASK_SPESTACK_LOW           (GAP_4+GAP_SIZE)
#define TASK_SPESTACK_HIGH          (TASK_SPESTACK_LOW + 6*(TASK_SPESTACK_SIZE+GAP_SIZE)-GAP_SIZE)

#define GAP_5               (TASK_SPESTACK_HIGH)
#define EXC_STACK_LOW         (GAP_5+GAP_SIZE)
#define EXC_STACK_HIGH        (EXC_STACK_LOW + 0x08000)

#define GAP_6                   (EXC_STACK_HIGH)
#define IRQ_STACK_LOW         (GAP_6 +GAP_SIZE)
#define IRQ_STACK_HIGH        (IRQ_STACK_LOW + 0x08000)

#define GAP_7               (IRQ_STACK_HIGH)
#define GAP_END             (GAP_7+GAP_SIZE)

#define DEC_STACK_LOW         0//(IRQ_STACK_HIGH)
#define DEC_STACK_HIGH        0//(DEC_STACK_LOW+0X10000) 
