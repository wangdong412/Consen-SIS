/* archPpc.h - PowerPC specific header */

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

#ifndef __INCarchPpch
#define __INCarchPpch

#ifdef __cplusplus
extern "C" {
#endif

#ifdef	_ASMLANGUAGE

#define CLRRLPI	clrrwi
#define CMPLLPI	cmplwi
#define CMPLLP  cmplw
#define CMPLP	  cmpw
#define CMPLPI	 cmpwi
#define LDLP    lwz
#define LDLPU   lwzu
#define LDLPX   lwzx
#define SLLPI	  slwi
#define SRLPI	  srwi
#define STLP    stw
#define STLPU   stwu
#define	RFI	    rfi
#define LDARX   lwarx
#define STDCX   stwcx

#define DSISR		18	/* data storage interrupt status */
#define	DAR		19	/* data address register */
#define	DEC		22	/* decrement register */
#define SDR1		25	/* storage description register 1 */

#define SRR0		26	/* save and restore register 0 */
#define SRR1		27	/* save and restore register 1 */

#define SPRG0		272	/* software program register 0 */
#define SPRG1		273	/* software program register 1 */
#define SPRG2		274	/* software program register 2 */
#define	SPRG3		275	/* software program register 3 */

#define ASR		280	/* address space register
				 * (64 bit implementation only) */
#define EAR		282	/* external address register */

#define TBL		284	/* lower time base register */
#define TBU		285	/* upper time base register */

#define PVR		287	/* processor version register */

/* physical addresses are represented on 64-bit for all PowerPC CPUs */

#define _TYPE_PHYS_ADDR		typedef UINT64 PHYS_ADDR
#endif 	/* _ASMLANGUAGE */

/* default register size */

#define _SIZE_OF_LONG_BITS	2
#define _PPC_REG_SIZE_BITS	2

#define _SIZE_OF_LONG		(1 << _SIZE_OF_LONG_BITS)

#define	_PPC_REG_SIZE		(1 << _PPC_REG_SIZE_BITS)

#define _PPC_WORD_SIZE		32	/* number of bits in a word */

#define _PPC_TEXT_SEG_ALIGN     4       /* 4 byte text segment alignment */

#define	RFI_OPCODE		0x4c000064

#define	_CACHE_ALIGN_SIZE	_CPU_CACHE_ALIGN_SIZE

#ifdef __cplusplus
}
#endif

#endif /* __INCarchPpch */
