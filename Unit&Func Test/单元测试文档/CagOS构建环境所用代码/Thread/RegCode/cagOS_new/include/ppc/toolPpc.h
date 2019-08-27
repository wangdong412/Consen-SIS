/* toolPpc.h - tool dependent headers */

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

#ifndef __INCtoolPpch
#define __INCtoolPpch

#ifdef __cplusplus
extern "C" {
#endif

#define	_PPC_USE_SDA		/* define this if using SDA */

#if	!defined(_DIAB_TOOL) && !defined(_GNU_TOOL)
#define	_GNU_TOOL
#endif

/*
 * While it is possible to use different stack alignments for different
 * PPC processors, current compilers use 16-byte alignment for all.
 */

#define _CPU_STACK_ALIGN_SIZE	16	/* stack alignment (for all PPC) */
#define	_CPU_STACK_ALIGN_SHIFT	4

#define _CPU_ALLOC_ALIGN_SIZE	16	/* allocation alignment */

#define FRAMEHEADERSZ		8	/* stack frame header size */

/* minimum stack frame size */

#define FRAMEBASESZ	((FRAMEHEADERSZ + (_STACK_ALIGN_SIZE - 1)) & \
			 ~(_STACK_ALIGN_SIZE - 1))

#ifdef	_ASMLANGUAGE

/* register definition */

/* volatile registers that are not saved across subroutine calls */

#define p0	r3	/* argument register, volatile */
#define p1	r4	/* argument register, volatile */
#define p2	r5	/* argument register, volatile */
#define p3	r6	/* argument register, volatile */
#define p4	r7	/* argument register, volatile */
#define p5	r8	/* argument register, volatile */
#define p6	r9	/* argument register, volatile */
#define p7	r10	/* argument register, volatile */
#define glr0	r0	/* prologs(PO,EABI), epilogs, glink routines(EABI) /
			 * language specific purpose(SVR4), volatile */
#define glr1	r11	/* prologs, epilogs, as Pascal environment pointer(EABI)
			 * language specific purpose (SVR4)
			 * calls by pointer, as Pascal environment(PO),
			 * volatile */
#define glr2	r12	/* prologs, epilogs, glink routines, calls by
			 * pointer(EABI), language specific purpose (SVR4),
			 * glue code, exception handling (PO), volatile */
#define retval0	r3	/* return register 0, volatile */
#define retval1	r4	/* return register 1, volatile */

/* non-volatile and dedicated registers saved across subroutine calls */

#define	sp	r1	/* stack pointer, dedicated */

#define t0	r14	/* temporary registers, non-volatile */
#define t1	r15	/* temporary registers, non-volatile */
#define t2	r16	/* temporary registers, non-volatile */
#define t3	r17	/* temporary registers, non-volatile */
#define t4	r18	/* temporary registers, non-volatile */
#define t5	r19	/* temporary registers, non-volatile */
#define t6	r20	/* temporary registers, non-volatile */
#define t7	r21	/* temporary registers, non-volatile */
#define t8	r22	/* temporary registers, non-volatile */
#define t9	r23	/* temporary registers, non-volatile */
#define t10	r24	/* temporary registers, non-volatile */
#define t11	r25	/* temporary registers, non-volatile */
#define t12	r26	/* temporary registers, non-volatile */
#define t13	r27	/* temporary registers, non-volatile */
#define t14	r28	/* temporary registers, non-volatile */
#define t15	r29	/* temporary registers, non-volatile */
#define t16	r30	/* temporary registers, non-volatile */
#define t17	r31	/* temporary registers, non-volatile */

#ifdef	_GNU_TOOL

/* GP register names */

#define r0	0
#define r1	1
#define r2	2
#define r3	3
#define r4	4
#define r5	5
#define r6	6
#define r7	7
#define r8	8
#define r9	9
#define r10	10
#define r11	11
#define r12	12
#define r13	13
#define r14	14
#define r15	15
#define r16	16
#define r17	17
#define r18	18
#define r19	19
#define r20	20
#define r21	21
#define r22	22
#define r23	23
#define r24	24
#define r25	25
#define r26	26
#define r27	27
#define r28	28
#define r29	29
#define r30	30
#define r31	31

#ifdef	_PPC_MSR_FP
/* FP register names */

#define	fp0	0
#define	fr0	0
#define	f0	0
#define	fp1	1
#define	fr1	1
#define	f1	1
#define	fp2	2
#define	fr2	2
#define	f2	2
#define	fp3	3
#define	fr3	3
#define	f3	3
#define	fp4	4
#define	fr4	4
#define	f4	4
#define	fp5	5
#define	fr5	5
#define	f5	5
#define	fp6	6
#define	fr6	6
#define	f6	6
#define	fp7	7
#define	fr7	7
#define	f7	7
#define	fp8	8
#define	fr8	8
#define	f8	8
#define	fp9	9
#define	fr9	9
#define	f9	9
#define	fp10	10
#define	fr10	10
#define	f10	10
#define	fp11	11
#define	fr11	11
#define	f11	11
#define	fp12	12
#define	fr12	12
#define	f12	12
#define	fp13	13
#define	fr13	13
#define	f13	13
#define	fp14	14
#define	fr14	14
#define	f14	14
#define	fp15	15
#define	fr15	15
#define	f15	15
#define	fp16	16
#define	fr16	16
#define	f16	16
#define	fp17	17
#define	fr17	17
#define	f17	17
#define	fp18	18
#define	fr18	18
#define	f18	18
#define	fp19	19
#define	fr19	19
#define	f19	19
#define	fp20	20
#define	fr20	20
#define	f20	20
#define	fp21	21
#define	fr21	21
#define	f21	21
#define	fp22	22
#define	fr22	22
#define	f22	22
#define	fp23	23
#define	fr23	23
#define	f23	23
#define	fp24	24
#define	fr24	24
#define	f24	24
#define	fp25	25
#define	fr25	25
#define	f25	25
#define	fp26	26
#define	fr26	26
#define	f26	26
#define	fp27	27
#define	fr27	27
#define	f27	27
#define	fp28	28
#define	fr28	28
#define	f28	28
#define	fp29	29
#define	fr29	29
#define	f29	29
#define	fp30	30
#define	fr30	30
#define	f30	30
#define	fp31	31
#define	fr31	31
#define	f31	31
#endif	/* _PPC_MSR_FP */

/* Condition register names */

#define cr0	0
#define cr1	1
#define cr2	2
#define cr3	3
#define cr4	4
#define cr5	5
#define cr6	6
#define cr7	7

/* Macro for hiadjust and lo */
#define HIADJ(arg)	(arg)@ha
#define HI(arg)		(arg)@h
#define LO(arg)		(arg)@l

#define SDA21(arg)	(arg)@sda21(0)

#endif	/* _GNU_TOOL */

#define FUNC(func)	    func
#define FUNC_LABEL(func)    func:

#define FUNC_DECL(range, func)
#define VAR_DECL(var)   var
#define VAR(var)        var(r0)

#define GTEXT(sym) FUNC(sym) ;  .type   FUNC(sym),@function
#define GDATA(sym) VAR_DECL(sym) ;  .type   VAR_DECL(sym),@object

/* Introduced to abstract assembler idiosyncrasies */
#define	IMPORT  extern
#define	FUNC_EXPORT(func)	.globl	GTEXT(func)
#define	DATA_EXPORT(var)	.globl	GDATA(var)
#define	FUNC_IMPORT(func)	.extern	FUNC(func)
#define	DATA_IMPORT(var)	.extern	VAR_DECL(var)
#define	FUNC_BEGIN(func)	FUNC_LABEL(func)
#define	FUNC_END(func)		.size	FUNC(func), . - FUNC(func)

/* Macro for beginning a text segment */
#define _CAG_TEXT_SEG_START \
	.text ; .balign _PPC_TEXT_SEG_ALIGN

#define	FRAMESZ(nregs)	\
    	  ROUND_UP((FRAMEBASESZ + nregs * _PPC_REG_SIZE), _STACK_ALIGN_SIZE)

#define	LOADPTR(reg,const32) \
	  addis reg,r0,HIADJ(const32); addi reg,reg,LO(const32)

#define	LOADVAR(reg,const32) \
	  addis reg,r0,HIADJ(const32); lwz reg,LO(const32)(reg)

#ifdef	_PPC_USE_SDA

#define	LOADPTR_SDA(reg,const32) \
	la	reg,SDA21(const32)

#define	LOADVAR_SDA(reg,const32) \
	lwz	reg,SDA21(const32)

#define	STOREVAR_SDA(v,sc,const32) \
	stw	v,SDA21(const32)

#else	/* _PPC_USE_SDA */

#define	LOADPTR_SDA(reg,const32)	LOADPTR(reg,const32)

#define	LOADVAR_SDA(reg,const32)	LOADVAR(reg,const32)

#define	STOREVAR_SDA(v,reg,const32) \
	addis reg,r0,HIADJ(const32); stw v,LO(const32)(reg)

#endif	/* _PPC_USE_SDA */

#else	/* _ASMLANGUAGE */

#define	IMPORT  extern
#define	FUNCREF(func)	func

#endif	/* _ASMLANGUAGE */

/*
 * Use constant sizes if known (when building for a specific CPU type)
 * else fetch from a global variable (when building for generic PPC32).
 * The latter is not supported, and should not be needed, for .s files.
 */

#ifdef	_CPU_STACK_ALIGN_SIZE
#define	_STACK_ALIGN_SIZE	_CPU_STACK_ALIGN_SIZE
#else	/* _CPU_STACK_ALIGN_SIZE */
# ifndef _ASMLANGUAGE
#define	_STACK_ALIGN_SIZE	_ppcStackAlignSize
extern	int	_ppcStackAlignSize;
# endif	/* _ASMLANGUAGE */
#endif	/* _CPU_STACK_ALIGN_SIZE */

#ifdef	_CPU_ALLOC_ALIGN_SIZE
#define	_ALLOC_ALIGN_SIZE	_CPU_ALLOC_ALIGN_SIZE
#else	/* _CPU_ALLOC_ALIGN_SIZE */
# ifndef _ASMLANGUAGE
#define	_ALLOC_ALIGN_SIZE	_ppcAllocationQuantumSize
extern	int	_ppcAllocationQuantumSize;
# endif	/* _ASMLANGUAGE */
#endif	/* _CPU_ALLOC_ALIGN_SIZE */

#ifdef __cplusplus
}
#endif

#endif /* __INCtoolPpch */
