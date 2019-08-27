/* sysCpuALib.s - Assembly support file and init routines */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  arch exceptions stub code for PPC85XX.
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
 * This file contains the exceptions entries
 */

#define _ASMLANGUAGE
#include <libc/cagboot.h>
#include <config.h>
#include <p1020rdb.h>
#include <ppc/toolPpc.h>
#include <ppc/archPpc.h>
#include <ppc/mmuE500Lib.h>
#include <ppc/asmPpc.h>
#include <ppc/excPpcLib.h>

FUNC_EXPORT(start)
FUNC_EXPORT(_start)
FUNC_EXPORT(sysL2CTLSet)
FUNC_EXPORT(sysL2CTLGet)
FUNC_EXPORT(sysCacheFlush)
FUNC_EXPORT(sysCacheEnable)
FUNC_IMPORT(sysEntry)
FUNC_EXPORT(sysPCGet)       /* get the value of the PC register */
FUNC_EXPORT(sysL1Csr1Set)
FUNC_EXPORT(sysTimeBaseLGet)
FUNC_EXPORT(sysCpu1Start)
FUNC_EXPORT(sysCpu1Stop)
FUNC_EXPORT(BOOTDEBUG)
FUNC_EXPORT(sysTlbUnmapAll)

FUNC_IMPORT(sysCpu1Loop)
FUNC_IMPORT(OSIntEnter)
FUNC_IMPORT(OSIntHandler)
FUNC_IMPORT(OSIntExit)

DATA_IMPORT(e500EarlyMapTlbId) 
DATA_IMPORT(e500ImaTlbCnt)  

#define MAS3_RPN    0xFFFFF000
#define MAS3_U0     0x00000200
#define MAS3_U1     0x00000100
#define MAS3_U2     0x00000080
#define MAS3_U3     0x00000040
#define MAS3_UX     0x00000020
#define MAS3_SX     0x00000010
#define MAS3_UW     0x00000008
#define MAS3_SW     0x00000004
#define MAS3_UR     0x00000002
#define MAS3_SR     0x00000001

#define MAS0_TLBSEL_MSK 0x30000000
#define MAS0_TLBSEL(x)  (((x) << 28) & MAS0_TLBSEL_MSK)
#define MAS0_TLBSEL(x)  (((x) << 28) & MAS0_TLBSEL_MSK)
#define MAS0_ESEL_MSK   0x0FFF0000
#define MAS0_ESEL(x)    (((x) << 16) & MAS0_ESEL_MSK)
#define MAS0_NV(x)  ((x) & 0x00000FFF)

#define MAS1_VALID  0x80000000
#define MAS1_IPROT  0x40000000
#define MAS1_TID(x) (((x) << 16) & 0x3FFF0000)
#define MAS1_TS     0x00001000
#define MAS1_TSIZE(x)   (((x) << 7) & 0x00000F80)
#define TSIZE_TO_BYTES(x) (1ULL << ((x) + 10))

#define FSL_BOOKE_MAS0(tlbsel,esel,nv) \
        (MAS0_TLBSEL(tlbsel) | MAS0_ESEL(esel) | MAS0_NV(nv))
#define FSL_BOOKE_MAS1(v,iprot,tid,ts,tsize) \
        ((((v) << 31) & MAS1_VALID)             |\
        (((iprot) << 30) & MAS1_IPROT)          |\
        (MAS1_TID(tid))             |\
        (((ts) << 12) & MAS1_TS)                |\
        (MAS1_TSIZE(tsize)))
#define FSL_BOOKE_MAS2(epn, wimge) \
        (((epn) & MAS3_RPN) | (wimge))
#define FSL_BOOKE_MAS3(rpn, user, perms) \
        (((rpn) & MAS3_RPN) | (user) | (perms))
#define FSL_BOOKE_MAS7(rpn) \
        (((u64)(rpn)) >> 32)

 .macro  create_tlb1_entry esel ts tsize epn wimg rpn perm phy_high scratch
 lis \scratch, FSL_BOOKE_MAS0(1, \esel, 0)@h
 ori \scratch, \scratch, FSL_BOOKE_MAS0(1, \esel, 0)@l
 mtspr   MAS0, \scratch
 lis \scratch, FSL_BOOKE_MAS1(1, 1, 0, \ts, \tsize)@h
 ori \scratch, \scratch, FSL_BOOKE_MAS1(1, 1, 0, \ts, \tsize)@l
 mtspr   MAS1, \scratch
 lis \scratch, FSL_BOOKE_MAS2(\epn, \wimg)@h
 ori \scratch, \scratch, FSL_BOOKE_MAS2(\epn, \wimg)@l
 mtspr   MAS2, \scratch
 lis \scratch, FSL_BOOKE_MAS3(\rpn, 0, \perm)@h
 ori \scratch, \scratch, FSL_BOOKE_MAS3(\rpn, 0, \perm)@l
 mtspr   MAS3, \scratch
 lis \scratch, \phy_high@h
 ori \scratch, \scratch, \phy_high@l
 mtspr   MAS7, \scratch
 isync
 msync
 tlbwe
 isync
 .endm

#define TLB1_ENTRY_0 0
#define TLB1_ENTRY_1 1
    _CAG_TEXT_SEG_START

#define CACHE_ALIGN_SHIFT   5   /* Cache line size == 2**5 */

/*******************************************************************************
*
* RETURNS: N/A.
*/



   . = 0x20
FUNC_LABEL(_start)
FUNC_BEGIN(start)

    bl sysCpuInit

    xor     p0, p0, p0
    mtspr   TCR, p0

    mbar 0

#if defined(_CAG_CONFIG_AMP1)
	/*create 2 tlb for core1 starting using*/
	create_tlb1_entry TLB1_ENTRY_0,0, 14 ,0, \
                  _MMU_TLB_ATTR_M, \
                  0,    \
                  MAS3_SX|MAS3_SW|MAS3_SR,  \
                  0,r6
	create_tlb1_entry TLB1_ENTRY_1,0, 14 ,0x01000000 , \
                  _MMU_TLB_ATTR_M, \
                  0x01000000,  \
                  MAS3_SX|MAS3_SW|MAS3_SR , \
                  0,r6
#endif
    /* initialize the stack pointer */

    lis    sp, HIADJ(KERNEL_STACK_HIGH)
    addi   sp, sp, LO(KERNEL_STACK_HIGH)
    addi   sp, sp, -FRAMEBASESZ     /* get frame stack */

    li     r3, 0
    b      sysEntry
FUNC_END(start)

/*******************************************************************************
*
* RETURNS: N/A.
*/

    . = _EXC_OFF_CRTL
FUNC_BEGIN(critical)
    mtspr   SPRG2,r1
    lis	r1, EXC_STACK_HIGH@h
    ori r1, r1, EXC_STACK_HIGH@l

    stwu   r1,-EXC_FRAME_SIZE(r1)
    stw     r0,XR0(r1)
    mfmsr   r0
    stw     r0,XMSR(r1)
    mfspr   r0,SRR0
    stw     r0,XSRR0(r1)
    mfspr   r0,SRR1
    stw     r0,XSRR1(r1)
    mfctr   r0
    stw     r0,XCTR(r1)
    mfxer r0
    stw     r0,XXER(r1)
    mfcr    r0
    stw     r0,XCR(r1)
    mfspr   r0,LR
    stw     r0,XLR(r1)

    bl saveContext

    mfspr r3,SPRG2
    mfspr r4,DEAR
    mfspr r5,ESR
	lis	r6, _EXC_OFF_CRTL@h
    ori r6, r6, _EXC_OFF_CRTL@l
    bl generalException
FUNC_END(critical)

/*******************************************************************************
*
* RETURNS: N/A.
*/

   . =_EXC_OFF_MACH
FUNC_BEGIN(machineCheck)
    mtspr   SPRG2,r1
    lis	r1, EXC_STACK_HIGH@h
    ori	r1, r1, EXC_STACK_HIGH@l
    stwu   r1,-EXC_FRAME_SIZE(r1)
    stw     r0,XR0(r1)
    mfmsr   r0
    stw     r0,XMSR(r1)
    mfspr   r0,MCSRR0
    stw     r0,XSRR0(r1)
    mfspr   r0,MCSRR1
    stw     r0,XSRR1(r1)
    mfctr   r0
    stw     r0,XCTR(r1)
    mfxer r0
    stw     r0,XXER(r1)
    mfcr    r0
    stw     r0,XCR(r1)
    mfspr   r0,LR
    stw     r0,XLR(r1)

    bl saveContext

    mfspr r3,SPRG2
    mfspr r4,MCAR
    mfspr r5,MCSR
	lis	r6, _EXC_OFF_MACH@h
    ori r6, r6, _EXC_OFF_MACH@l
    bl generalException
FUNC_END(machineCheck)

/*******************************************************************************
*
* RETURNS: N/A.
*/

  . =_EXC_OFF_DATA
FUNC_BEGIN(dsi)
    mtspr   SPRG2,r1
    lis	r1, EXC_STACK_HIGH@h
    ori	r1, r1, EXC_STACK_HIGH@l

    stwu   r1,-EXC_FRAME_SIZE(r1)
    stw     r0,XR0(r1)
    mfmsr   r0
    stw     r0,XMSR(r1)
    mfspr   r0,SRR0
    stw     r0,XSRR0(r1)
    mfspr   r0,SRR1
    stw     r0,XSRR1(r1)
    mfctr   r0
    stw     r0,XCTR(r1)
    mfxer r0
    stw     r0,XXER(r1)
    mfcr    r0
    stw     r0,XCR(r1)
    mfspr   r0,LR
    stw     r0,XLR(r1)

    bl saveContext

    mfspr r3,SPRG2
    mfspr r4,DEAR
    mfspr r5,ESR
	lis	r6, _EXC_OFF_DATA@h
    ori r6, r6, _EXC_OFF_DATA@l
    bl generalException
FUNC_END(dsi)

/*******************************************************************************
*
* RETURNS: N/A.
*/
    . =_EXC_OFF_INST
FUNC_BEGIN(isi)
   mtspr  SPRG2,r1
   lis	r1, EXC_STACK_HIGH@h
   ori	r1, r1, EXC_STACK_HIGH@l
   stwu    r1,-EXC_FRAME_SIZE(r1)
   stw     r0,XR0(r1)
   mfmsr   r0
   stw     r0,XMSR(r1)
   mfspr   r0,SRR0
   stw     r0,XSRR0(r1)
   mfspr   r0,SRR1
   stw     r0,XSRR1(r1)
   mfctr   r0
   stw     r0,XCTR(r1)
   mfxer   r0
   stw     r0,XXER(r1)
   mfcr    r0
   stw     r0,XCR(r1)
   mfspr   r0,LR
   stw     r0,XLR(r1)

   bl saveContext

   mfspr r3,SPRG2
   mfspr r4,DEAR
   mfspr r5,ESR
   lis r6, _EXC_OFF_INST@h
   ori r6, r6, _EXC_OFF_INST@l
   bl generalException
FUNC_END(isi)

/*******************************************************************************
*
* RETURNS: N/A.
*/

    . = _EXC_OFF_INTR
FUNC_BEGIN(extInt)

    stwu    r1,-EXC_FRAME_SIZE(r1)
    stw     r0,XR0(r1)
    mfmsr   r0
    stw     r0,XMSR(r1)
    mfspr   r0,SRR0

    stw     r0,XSRR0(r1)
    mfspr   r0,SRR1
    stw     r0,XSRR1(r1)
    mfctr   r0
    stw     r0,XCTR(r1)
    mfxer   r0
    stw     r0,XXER(r1)
    mfcr    r0
    stw     r0,XCR(r1)
    mfspr   r0,LR
    stw     r0,XLR(r1)

    bl saveContext

    xor     r0,r0,r0
    addis   r10,r0,OSTCBCurPtr@ha
    lwz r7, OSTCBCurPtr@l(r10)

    stw r1,0(r7)

    lis r1, IRQ_STACK_HIGH@h
    ori r1, r1, IRQ_STACK_HIGH@l
    bl OSIntEnter
    bl OSIntHandler
    bl OSIntExit

    xor r0,r0,r0
    addis   r10,r0,OSTCBCurPtr@ha
    lwz r7,OSTCBCurPtr@l(r10)

    bl loadContext

FUNC_END(extInt)

/*******************************************************************************
*
* RETURNS: N/A.
*/

    . = _EXC_OFF_ALIGN
FUNC_BEGIN(alignment)
   mtspr    SPRG2,r1
   lis	r1, EXC_STACK_HIGH@h
   ori	r1, r1, EXC_STACK_HIGH@l
   stwu    r1,-EXC_FRAME_SIZE(r1)
   stw     r0,XR0(r1)
   mfmsr   r0
   stw     r0,XMSR(r1)
   mfspr   r0,SRR0
   stw     r0,XSRR0(r1)
   mfspr   r0,SRR1
   stw     r0,XSRR1(r1)
   mfctr   r0
   stw     r0,XCTR(r1)
   mfxer   r0
   stw     r0,XXER(r1)
   mfcr    r0
   stw     r0,XCR(r1)
   mfspr   r0,LR
   stw     r0,XLR(r1)

   bl saveContext

   mfspr r3,SPRG2
   mfspr r4,DEAR
   mfspr r5,ESR
   lis r6, _EXC_OFF_ALIGN@h
   ori r6, r6, _EXC_OFF_ALIGN@l
   bl generalException
FUNC_END(alignment)

/*******************************************************************************
*
* RETURNS: N/A.
*/
    . = _EXC_OFF_PROG
FUNC_BEGIN(programCheck)
   mtspr SPRG2,r1
   lis	r1, EXC_STACK_HIGH@h
   ori	r1, r1, EXC_STACK_HIGH@l
   stwu    r1,-EXC_FRAME_SIZE(r1)
   stw     r0,XR0(r1)
   mfmsr   r0
   stw     r0,XMSR(r1)
   mfspr   r0,SRR0
   stw     r0,XSRR0(r1)
   mfspr   r0,SRR1
   stw     r0,XSRR1(r1)
   mfctr   r0
   stw     r0,XCTR(r1)
   mfxer   r0
   stw     r0,XXER(r1)
   mfcr    r0
   stw     r0,XCR(r1)
   mfspr   r0,LR
   stw     r0,XLR(r1)

   bl saveContext

   mfspr r3,SPRG2
   mfspr r4,DEAR
   mfspr r5,ESR
   lis r6, _EXC_OFF_PROG@h
   ori r6, r6, _EXC_OFF_PROG@l
   bl generalException
FUNC_END(programCheck)

/*******************************************************************************
*
* RETURNS: N/A.
*/
    . = _EXC_OFF_FPU
FUNC_BEGIN(fpexception)
    mtspr   SPRG2,r1
    lis	r1, EXC_STACK_HIGH@h
    ori	r1, r1, EXC_STACK_HIGH@l
    stwu    r1,-EXC_FRAME_SIZE(r1)
    stw     r0,XR0(r1)
    mfmsr   r0
    stw     r0,XMSR(r1)
    mfspr   r0,SRR0
    stw     r0,XSRR0(r1)
    mfspr   r0,SRR1
    stw     r0,XSRR1(r1)
    mfctr   r0
    stw     r0,XCTR(r1)
    mfxer   r0
    stw     r0,XXER(r1)
    mfcr    r0
    stw     r0,XCR(r1)
    mfspr   r0,LR
    stw     r0,XLR(r1)

    bl saveContext

    mfspr r3,SPRG2
    mfspr r4,DEAR
    mfspr r5,ESR
    lis r6, _EXC_OFF_FPU@h
    ori r6, r6, _EXC_OFF_FPU@l
    bl generalException
FUNC_END(fpexception)

/*******************************************************************************
*
* RETURNS: N/A.
*/
    . = _EXC_OFF_DECR
FUNC_BEGIN(decexception)
    mtspr   SPRG2,r1
    lis	r1, EXC_STACK_HIGH@h
    ori	r1, r1, EXC_STACK_HIGH@l
    stwu    r1,-EXC_FRAME_SIZE(r1)
    stw     r0,XR0(r1)
    mfmsr   r0
    stw     r0,XMSR(r1)
    mfspr   r0,SRR0
    stw     r0,XSRR0(r1)
    mfspr   r0,SRR1
    stw     r0,XSRR1(r1)
    mfctr   r0
    stw     r0,XCTR(r1)
    mfxer   r0
    stw     r0,XXER(r1)
    mfcr    r0
    stw     r0,XCR(r1)
    mfspr   r0,LR
    stw     r0,XLR(r1)

    bl saveContext

    mfspr r3,SPRG2
    mfspr r4,DEAR
    mfspr r5,ESR
    lis r6, _EXC_OFF_DECR@h
    ori r6, r6,_EXC_OFF_DECR@l
    bl generalException
FUNC_END(decexception)
/*******************************************************************************
*
* RETURNS: N/A.
*/

   . = _EXC_OFF_DATA_MISS
FUNC_BEGIN(dtlbException)
   mtspr    SPRG2,r1
   lis	r1, EXC_STACK_HIGH@h
   ori	r1, r1, EXC_STACK_HIGH@l
   stwu    r1,-EXC_FRAME_SIZE(r1)
   stw     r0,XR0(r1)
   mfmsr   r0
   stw     r0,XMSR(r1)
   mfspr   r0,SRR0
   stw     r0,XSRR0(r1)
   mfspr   r0,SRR1
   stw     r0,XSRR1(r1)
   mfctr   r0
   stw     r0,XCTR(r1)
   mfxer   r0
   stw     r0,XXER(r1)
   mfcr    r0
   stw     r0,XCR(r1)
   mfspr   r0,LR
   stw     r0,XLR(r1)

   bl saveContext

   mfspr r3,SPRG2
   mfspr r4,DEAR
   mfspr r5,ESR
   lis r6, _EXC_OFF_DATA_MISS@h
   ori r6, r6, _EXC_OFF_DATA_MISS@l
   bl generalException
FUNC_END(dtlbException)

/*******************************************************************************
*
* RETURNS: N/A.
*/
   . = _EXC_OFF_INST_MISS
FUNC_BEGIN(itlbException)
   mtspr SPRG2,r1
   lis	r1, EXC_STACK_HIGH@h
   ori	r1, r1, EXC_STACK_HIGH@l
   stwu    r1,-EXC_FRAME_SIZE(r1)
   stw     r0,XR0(r1)
   mfmsr   r0
   stw     r0,XMSR(r1)
   mfspr   r0,SRR0
   stw     r0,XSRR0(r1)
   mfspr   r0,SRR1
   stw     r0,XSRR1(r1)
   mfctr   r0
   stw     r0,XCTR(r1)
   mfxer   r0
   stw     r0,XXER(r1)
   mfcr    r0
   stw     r0,XCR(r1)
   mfspr   r0,LR
   stw     r0,XLR(r1)

   bl saveContext

   mfspr r3,SPRG2
   mfspr r4,DEAR
   mfspr r5,ESR
   lis r6, _EXC_OFF_INST_MISS@h
   ori r6, r6, _EXC_OFF_INST_MISS@l
   bl generalException
FUNC_END(itlbException)

/*******************************************************************************
*
* RETURNS: N/A.
*/
   . = _EXC_OFF_SPE
FUNC_BEGIN(unSpeException)
   mtspr SPRG2,r1
   lis	r1, EXC_STACK_HIGH@h
   ori	r1, r1, EXC_STACK_HIGH@l
   stwu    r1,-EXC_FRAME_SIZE(r1)
   stw     r0,XR0(r1)
   mfmsr   r0
   stw     r0,XMSR(r1)
   mfspr   r0,SRR0
   stw     r0,XSRR0(r1)
   mfspr   r0,SRR1
   stw     r0,XSRR1(r1)
   mfctr   r0
   stw     r0,XCTR(r1)
   mfxer   r0
   stw     r0,XXER(r1)
   mfcr    r0
   stw     r0,XCR(r1)
   mfspr   r0,LR
   stw     r0,XLR(r1)

   bl saveContext

   mfspr r3,SPRG2
   mfspr r4,DEAR
   mfspr r5,ESR
   lis r6, _EXC_OFF_SPE@h
   ori r6, r6, _EXC_OFF_SPE@l
   bl generalException
FUNC_END(unSpeException)

/*******************************************************************************
*
* RETURNS: N/A.
*/

   . = _EXC_OFF_VEC_DATA
FUNC_BEGIN(speDataException)
   mtspr SPRG2,r1
   lis	r1, EXC_STACK_HIGH@h
   ori	r1, r1, EXC_STACK_HIGH@l
   stwu    r1,-EXC_FRAME_SIZE(r1)
   stw     r0,XR0(r1)
   mfmsr   r0
   stw     r0,XMSR(r1)
   mfspr   r0,SRR0
   stw     r0,XSRR0(r1)
   mfspr   r0,SRR1
   stw     r0,XSRR1(r1)
   mfctr   r0
   stw     r0,XCTR(r1)
   mfxer   r0
   stw     r0,XXER(r1)
   mfcr    r0
   stw     r0,XCR(r1)
   mfspr   r0,LR
   stw     r0,XLR(r1)

   bl saveContext

   mfspr r3,SPRG2
   mfspr r4,DEAR
   mfspr r5,ESR
   lis r6, _EXC_OFF_VEC_DATA@h
   ori r6, r6, _EXC_OFF_VEC_DATA@l
   bl generalException
FUNC_END(speDataException)

/*******************************************************************************
*
* RETURNS: N/A.
*/

   . = _EXC_OFF_VEC_RND
FUNC_BEGIN(speRoundException)
   mtspr SPRG2,r1
   lis	r1, EXC_STACK_HIGH@h
   ori	r1, r1, EXC_STACK_HIGH@l
   stwu    r1,-EXC_FRAME_SIZE(r1)
   stw     r0,XR0(r1)
   mfmsr   r0
   stw     r0,XMSR(r1)
   mfspr   r0,SRR0
   stw     r0,XSRR0(r1)
   mfspr   r0,SRR1
   stw     r0,XSRR1(r1)
   mfctr   r0
   stw     r0,XCTR(r1)
   mfxer   r0
   stw     r0,XXER(r1)
   mfcr    r0
   stw     r0,XCR(r1)
   mfspr   r0,LR
   stw     r0,XLR(r1)

   bl saveContext

   mfspr r3,SPRG2
   mfspr r4,DEAR
   mfspr r5,ESR
   lis r6, _EXC_OFF_VEC_RND@h
   ori r6, r6, _EXC_OFF_VEC_RND@l
   bl generalException
FUNC_END(speRoundException)

/*******************************************************************************
*
* RETURNS: N/A.
*/

FUNC_BEGIN(saveContext)
    stw     r2,XR2(r1)
    stw     r3,XR3(r1)
    stw     r4,XR4(r1)
    stw     r5,XR5(r1)
    stw     r6,XR6(r1)
    stw     r7,XR7(r1)
    stw     r8,XR8(r1)
    stw     r9,XR9(r1)
    stw     r10,XR10(r1)
    stw     r11,XR11(r1)
    stw     r12,XR12(r1)
    stw     r13,XR13(r1)
    stw     r14,XR14(r1)
    stw     r15,XR15(r1)
    stw     r16,XR16(r1)
    stw     r17,XR17(r1)
    stw     r18,XR18(r1)
    stw     r19,XR19(r1)
    stw     r20,XR20(r1)
    stw     r21,XR21(r1)
    stw     r22,XR22(r1)
    stw     r23,XR23(r1)
    stw     r24,XR24(r1)
    stw     r25,XR25(r1)
    stw     r26,XR26(r1)
    stw     r27,XR27(r1)
    stw     r28,XR28(r1)
    stw     r29,XR29(r1)
    stw     r30,XR30(r1)
    stw     r31,XR31(r1)
    blr
FUNC_END(saveContext)

/*******************************************************************************
*
* RETURNS: N/A.
*/

FUNC_BEGIN(loadContext)
    lwz r1,0(r7)
    lwz r2,XR2(r1)
    lwz r3,XR3(r1)
    lwz r4,XR4(r1)
    lwz r5,XR5(r1)
    lwz r6,XR6(r1)
    lwz r7,XR7(r1)
    lwz r8,XR8(r1)
    lwz r9,XR9(r1)
    lwz r10,XR10(r1)
    lwz r11,XR11(r1)
    lwz r12,XR12(r1)
    lwz r13,XR13(r1)
    lwz r14,XR14(r1)
    lwz r15,XR15(r1)
    lwz r16,XR16(r1)
    lwz r17,XR17(r1)
    lwz r18,XR18(r1)
    lwz r19,XR19(r1)
    lwz r20,XR20(r1)
    lwz r21,XR21(r1)
    lwz r22,XR22(r1)
    lwz r23,XR23(r1)
    lwz r24,XR24(r1)
    lwz r25,XR25(r1)
    lwz r26,XR26(r1)
    lwz r27,XR27(r1)
    lwz r28,XR28(r1)
    lwz r29,XR29(r1)
    lwz r30,XR30(r1)
    lwz r31,XR31(r1)
    lwz r0, XLR(r1)
    mtlr r0
    lwz  r0,XCR(r1)
    mtcrf   255,r0
    lwz     r0,XXER(r1)
    mtxer r0
    lwz     r0,XCTR(r1)
    mtctr r0
    lwz     r0,XSRR0(r1)
    mtspr   SRR0,r0
    lwz     r0,XR0(r1)
    mtspr SPRG0,r0
    lwz   r0,XSRR1(r1)
    mtspr SRR1,r0
    mfspr r0,SPRG0
    addi  r1,r1,EXC_FRAME_SIZE
    SYNC
    rfi
FUNC_END(loadContext)

/*******************************************************************************
*
* sysCacheEnable - Enable the L1 cache (D-catch and I-catch)
*
* This routine returns the value written.
*
* SYNOPSIS
* \ss
* VOID sysCacheEnable(void)
* \se
*
* RETURNS: N/A.
*/
FUNC_BEGIN(sysCacheEnable)

    /* Invalidate the Data cache */

    li     r3, 0x0002
    msync
    isync
    mtspr  L1CSR0, r3
    msync
    isync

    /* enable the data cache */

    li     r3, 0x0001
    msync
    isync
    mtspr  L1CSR0, r3
    msync
    isync

    /*
     * Write to 0 and then write to 1 to flash clear the valid bit of all
     * entries in the instruction cache
     */

    li     r3, 0x0000
    msync
    isync
    mtspr  L1CSR1, r3
    msync
    isync
    li     r3, 0x0002
    msync
    isync
    mtspr  L1CSR1, r3
    msync
    isync

    /* enable the instruction cache */

    li     r3, 0x0001
    msync
    isync
    mtspr   L1CSR1, r3
    msync
    isync

    blr
FUNC_END(sysCacheEnable)

    .balign 32
/*******************************************************************************
*
* sysL2CTLSet - Set the value of the L@ cache control register
*
* This routine returns the value written.
*
* SYNOPSIS
* \ss
* UINT32 sysL2CTLSet
*     (
*     UINT32 value,
*     UINT32 *addr
*     )
* \se
*
* RETURNS: This routine returns the value in the L2CTL reg.
*/

FUNC_BEGIN(sysL2CTLSet)
    mbar   0
    isync
    stw    p0, 0(p1)
    lwz    p0, 0(p1)
    mbar    0
    isync
    blr
FUNC_END(sysL2CTLSet)

    .balign 32
/*******************************************************************************
*
* sysL2CTLGet - Get the value of the L2 cache control register
*
* SYNOPSIS
* \ss
* UINT32 sysL2CTLGet
*     (
*     UINT32 *addr
*     )
* \se
*
* RETURNS: This routine returns the value in the L2CTL reg.
*/
FUNC_BEGIN(sysL2CTLGet)
    mbar    0
    isync
    lwz    p1, 0x0(p0)
    addi   p0, p1, 0x0
    mbar   0
    isync
    blr
FUNC_END(sysL2CTLGet)

/*******************************************************************************
*
* sysCacheFlush - just flushes cache - assume int lock
*
* p0 - cache line num
* p1 - buffer origin
* p2 - cache align size
*
*/

FUNC_BEGIN(sysCacheFlush)

    srwi   p3, p0, (MMU_RPN_SHIFT - CACHE_ALIGN_SHIFT)
    mtspr  CTR, p3
    addi   p6, p1,0
    li     p5, MMU_PAGE_SIZE
    subf   p3, p5, p1

    b      cacheL2DisableLoadItlb
cacheL2DisableLoadDtlb:
    add    p3, p3, p5
    lbzu   p4, 0(p3)
    bdnz   cacheL2DisableLoadDtlb
    mtctr  p0
    subf   p1, p2, p1
l2DisableFlush:
    add    p1, p2, p1
    lbzu   p3, 0x0(p1)
    bdnz   l2DisableFlush
    sync
    isync
    mtctr  p0
    addi   p1, p6, 0
    subf   p1, p2, p1
l2DisableClear:
    add    p1, p2, p1
    dcbf   0, p1
    bdnz   l2DisableClear
    sync
    isync
    blr
cacheL2DisableLoadItlb:
    b      cacheL2DisableLoadDtlb
FUNC_END(sysCacheFlush)

/*******************************************************************************
*
* sysPCGet - Get the value of the PC (Program Counter)
*
* This routine returns the value of the PC.
*
* SYNOPSIS
* \ss
* UINT32 sysPCGet
*     (
*     void
*     )
* \se
*
* RETURNS: the Program Counter Register (PC) value.
*/

FUNC_BEGIN(sysPCGet)
    mflr    r3
    blr
FUNC_END(sysPCGet)

/*******************************************************************************
*
* sysTimeBaseLGet - Get lower half of Time Base Register
*
* SYNOPSIS
* \ss
* UINT32 sysTimeBaseLGet(void)
* \se
*
* This routine will read the contents the lower half of the Time
* Base Register (TBL - TBR 268).
*
* RETURNS: value of TBR 268 (in r3)
*/

FUNC_BEGIN(sysTimeBaseLGet)
    mfspr  r3, 268
    bclr   20, 0            /* Return to caller */
FUNC_END(sysTimeBaseLGet)

/*******************************************************************************
*
* sysL1Csr1Set - Set the value of L1CSR1
*
* SYNOPSIS
* \ss
* void sysL1Csr1Set
*     (
*     UINT32
*     )
* \se
*
* RETURNS: none
*/

FUNC_BEGIN(sysL1Csr1Set)
    msync
    isync
    mtspr  L1CSR1, r3
    msync
    isync
    blr
FUNC_END(sysL1Csr1Set)

/*******************************************************************************
*
* sysCpuInit - initialize CPU
*
* This routine initializes the primary CPU, including turning off the
* exceptions, reseting the internal timers, initializing the L1 Cache and
* clearing the CPU registers.
*
* SYNOPSIS
* \ss
* VOID sysCpuInit (void)
* \se
*
* NOTE that this routine must be placed in .text.sysinit section.
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysCpuInit)

    /* turn off exceptions */

    mfmsr   r5
    INT_MASK (r5, r6)
    ME_MASK (r6, r6)
    mtmsr   r6
    isync

    /* reset internal timers */

    xor     r5, r5, r5
    mtspr   TCR, r5
    msync
    isync

    /* Clear SRR0, CSRR0, MCSRR0, SRR1, CSRR1 , MCSRR1, IVPR */

    xor     r5, r5, r5
    mtspr   SRR0, r5
    mtspr   SRR1, r5
    mtspr   CSRR0, r5
    mtspr   CSRR1, r5
    mtspr   MCSRR0, r5
    mtspr   MCSRR1, r5
    mtspr   ESR, r5
    mtspr   MCSR, r5
    mtspr   DEAR, r5
    mtspr   DBCR0, r5
    mtspr   DBCR1, r5
    mtspr   DBCR2, r5
    mtspr   IAC1, r5
    mtspr   IAC2, r5
    mtspr   DAC1, r5
    mtspr   DAC2, r5
    mtspr   BUCSR, r5
    mfspr   r6, DBSR
    mtspr   DBSR, r6

    isync
    blr
FUNC_END(sysCpuInit)


FUNC_BEGIN(BOOTDEBUG)
     lis r5, HI(0xffe04503)      /*基址*/
     ori r5, r5, LO(0xffe04503)
     li      r6, 0x83
     stb     r6, 0(r5)
     sync
     eieio

     lis r5, HI(0xffe04500)      /*基址*/
     ori r5, r5, LO(0xffe04500)
     li      r6, 0xF
     stb     r6, 0(r5)
     sync
     eieio

     lis r5, HI(0xffe04501)      /*基址*/
     ori r5, r5, LO(0xffe04501)
     li      r6, 0x1
     stb     r6, 0(r5)
     sync
     eieio

     lis r5, HI(0xffe04503)      /*基址*/
     ori r5, r5, LO(0xffe04503)
     li      r6, 0x3
     stb     r6, 0(r5)
     sync
     eieio

     lis r5, HI(0xffe04500)      /*基址*/
     ori r5, r5, LO(0xffe04500)
     li      r6,0x42
     stb     r6,0(r5)
     sync

     lis r5, HI(0xffe04500)      /*基址*/
     ori r5, r5, LO(0xffe04500)
     li      r6,0x42
     stb     r6,0(r5)
     sync

     blr
     nop
     nop
     nop
FUNC_END(BOOTDEBUG)

/*******************************************************************************
*
* sysTlbUnmapAll - clear all TLB entries mapped for early initialization
*
* This routine clears all the TLB entries mapped for the early initialization
* process.
*
* SYNOPSIS
* \ss
* void sysTlbUnmapAll (void)
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysTlbUnmapAll)

    /* get current index and clear count */
    /* invlidate TLB entries */

earlyTlbInv:
    slwi    r5, r3, _PPC_MAS0_ESEL_BIT_MAX + 1
    oris    r5, r5, _PPC_MAS0_TLBSEL1_U
    mtspr   MAS0, r5
    tlbre

    /* clear V and IPROT */

    mfspr   r5, MAS1
    rlwinm  r5, r5, 0, _PPC_MAS1_IPROT_BIT + 1, 31
    mtspr   MAS1, r5
    tlbwe
    isync

    addi    r3, r3, 1
    cmpw    r3, r4
    ble     earlyTlbInv

    blr
FUNC_END(sysTlbUnmapAll)
