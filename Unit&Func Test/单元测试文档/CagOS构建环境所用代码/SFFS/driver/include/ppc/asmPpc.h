/* asmPpc.h - PowerPC assembler definitions header file */

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

#ifndef __INCasmPpch
#define __INCasmPpch

#ifdef __cplusplus
extern "C" {
#endif

/* special purpose register encoding */

#define XER		1	/* external exception register */
#define LR		8	/* link register (return address) */
#define	CTR		9	/* counter register */

# define _PPC_KERNEL_MTMSR(msr, scr, label)                                   \
        mtmsr   msr

# define _PPC_KERNEL_MTMSR_WITH_BLR(msr, scr)                                 \
        mtmsr   msr;                                                          \
        blr

#ifdef __cplusplus
}
#endif

#endif /* __INCasmPpch */
