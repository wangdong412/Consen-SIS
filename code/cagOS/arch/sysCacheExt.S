/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  arch cache code for PPC85XX.
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
 * This file contains the L1cache operation.
 */

/*
DESCRIPTION
This library contains routines to manipulate the PowerPC E500 family caches.

INCLUDE FILES: cacheLib.h

SEE ALSO: cacheLib
*/

#define _ASMLANGUAGE
#include <libc/cagboot.h>
#include <config.h>
#include <ppc/toolPpc.h>
#include <ppc/archPpc.h>
#include <ppc/mmuE500Lib.h>
#include <ppc/asmPpc.h>
#include <ppc/excPpcLib.h>


/* need verify in the future */

	/* r3 = addr, r4 = value, clobbers r5, r11, r12 */
	/* Lock two cache lines into I-Cache */
	sync
	mfspr	r11, SPRN_L1CSR1
	rlwinm	r11, r11, 0, ~L1CSR1_ICUL
	sync
	isync
	mtspr	SPRN_L1CSR1, r11
	isync

	mflr	r12
	bl	5f
5:	mflr	r5
	addi	r5, r5, 2f - 5b
	icbtls	0, 0, r5
	addi	r5, r5, 64

	sync
	mfspr	r11, SPRN_L1CSR1
3:	andi.	r11, r11, L1CSR1_ICUL
	bne	3b

	icbtls	0, 0, r5
	addi	r5, r5, 64

	sync
	mfspr	r11, SPRN_L1CSR1
3:	andi.	r11, r11, L1CSR1_ICUL
	bne	3b

	b	2f
	.align	6
	/* Inside a locked cacheline, wait a while, write, then wait a while */
2:	sync

	/*
	 * Fill out the rest of this cache line and the next with nops,
	 * to ensure that nothing outside the locked area will be
	 * fetched due to a branch.
	 */
	.rept 19
	nop
	.endr

	sync
	mfspr	r11, SPRN_L1CSR1
	rlwinm	r11, r11, 0, ~L1CSR1_ICUL
	sync
	isync
	mtspr	SPRN_L1CSR1, r11
	isync

	mtlr	r12
	blr


	/* Allocate Initial RAM in data cache.
	 */
	lis	r3,CONFIG_SYS_INIT_RAM_ADDR@h
	ori	r3,r3,CONFIG_SYS_INIT_RAM_ADDR@l
	mfspr	r2, L1CFG0
	andi.	r2, r2, 0x1ff
	/* cache size * 1024 / (2 * L1 line size) */
	slwi	r2, r2, (10 - 1 - L1_CACHE_SHIFT)
	mtctr	r2
	li	r0,0
1:
	dcbz	r0,r3
	dcbtls	0,r0,r3
	addi	r3,r3,CONFIG_SYS_CACHELINE_SIZE
	bdnz	1b

.globl unlock_ram_in_cache
unlock_ram_in_cache:
	/* invalidate the INIT_RAM section */
	lis	r3,(CONFIG_SYS_INIT_RAM_ADDR & ~(CONFIG_SYS_CACHELINE_SIZE-1))@h
	ori	r3,r3,(CONFIG_SYS_INIT_RAM_ADDR & ~(CONFIG_SYS_CACHELINE_SIZE-1))@l
	mfspr	r4,L1CFG0
	andi.	r4,r4,0x1ff
	slwi	r4,r4,(10 - 1 - L1_CACHE_SHIFT)
	mtctr	r4
1:	dcbi	r0,r3
	dcblc	r0,r3
	addi	r3,r3,CONFIG_SYS_CACHELINE_SIZE
	bdnz	1b
	sync

