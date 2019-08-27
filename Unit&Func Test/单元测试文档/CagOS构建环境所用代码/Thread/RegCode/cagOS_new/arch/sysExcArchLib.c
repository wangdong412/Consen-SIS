/* excArchLib.c - PowerPC exception handling facilities */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  arch exception vector code for PPC85XX.
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
 * This file contains the vectors
 */

/*
This module provides architecture-dependent facilities for handling PowerPC
exceptions.  See excLib for the portions that are architecture independent.
*/

/* LINTLIBRARY */

#include <libc/cagboot.h>
#include <config.h>
#include <libc/stdio.h>
#include <libc/system.h>
#include <ppc/archPpc.h>
#include <ppc/ppc85xx.h>
#include <ppc/excPpcLib.h>
#include "sysExcLib.h"

/*
 * Base address of vector table in normal code space.  Usually zero, but
 * may be set otherwise by the BSP, e.g. by changing VEC_BASE_ALT_ADRS.
 */
IMPORT codeBase  excVecBaseAltAdrs;

/* locals */

LOCAL vectorBase  excVecBase = (vectorBase)NULL;

typedef struct {
    UINT32	gpr;
    char * 	gprMsg;
} EXC_GPR_MSG;

EXC_GPR_MSG preg [] = {
    {0,"msr"},
    {4,"r31"},
    {8,"r30"},
    {12,"r29"},
    {16,"r28"},
    {20,"r27"},
    {24,"r26"},
    {28,"r25"},
    {32,"r24"},
    {36,"r23"},
    {40,"r22"},
    {44,"r21"},
    {48,"r20"},
    {52,"r19"},
    {56,"r18"},
    {60,"r17"},
    {64,"r16"},
    {68,"r15"},
    {72,"r14"},
    {76,"r13"},
    {80,"r12"},
    {84,"r11"},
    {88,"r10"},
    {92,"r9"},
    {96,"r8"},
    {100,"r7"},
    {104,"r6"},
    {108,"r5"},
    {112,"r4"},
    {116,"r3"},
    {120,"r2"},
    {124,"lr"},
    {128,"cr"},
    {132,"xer"},
    {136,"ctr"},
    {140,"srr1"},
    {144,"srr0"},
    {148,"r0"},
    {152,"NULL"},
    {156,"r1"}
};

/* forward declarations */

#ifdef IVOR0
IMPORT void  excIvorInit (void);
#endif /* IVOR0 */
IMPORT void executeException(UINT8 vector);
STATUS installE500ParityErrorRecovery (void);
IMPORT void panic(const char *fmt, ...);

/*******************************************************************************
*
* excVecInit - initialize the exception vectors
*
* This routine sets up PowerPC exception vectors to point to the
* appropriate default exception handlers.
*
* WHEN TO CALL
* This routine is usually called from the system start-up routine
* usrInit() in usrConfig.c, before interrupts are enabled.
*
* SMP CONSIDERATIONS
* This API is spinlock and intCpuLock restricted.
*
* RETURNS: OK, or ERROR on the PPC440x5 if the Machine Check Handler's code
*	   will not fit in the available space.
*
* SEE ALSO: excLib
*
* INTERNAL:  excVecConnectCommon is now used to connect all exc types.
*/

STATUS excVecInit
(
    FUNCPTR * baseAddr		/* new vector base address */
)
{

    excVecBaseSet(baseAddr);

#ifdef	IVOR0
    excIvorInit();
#endif	/* IVOR0 */

    sysMsrSet (sysMsrGet() | _PPC_MSR_ME
#ifdef	_PPC_MSR_RI
               | _PPC_MSR_RI
#endif	/* _PPC_MSR_RI */
              );

    installE500ParityErrorRecovery();

    return (OK);
}

/*******************************************************************************
*
* excVecBaseSet - set the exception vector base address
*
* RETURNS: N/A
*
* SEE ALSO: excVecBaseGet(), excVecGet(), excVecSet()
*
* \NOMANUAL
*/

void excVecBaseSet
(
    FUNCPTR * baseAddr		/* new vector base address */
)
{
    excVecBase = (vectorBase)((UINT32)baseAddr & 0x0ffff0000);
    sysIvprSet ((int) excVecBase);
}

/*******************************************************************************
*
* excVecBaseGet - get the vector base address
*
* This routine returns the current vector base address that has been set
* with the intVecBaseSet() routine.
*
* RETURNS: The current vector base address.
*
* SEE ALSO: intVecBaseSet()
*
* \NOMANUAL
*/

FUNCPTR * excVecBaseGet (void)
{
    return (FUNCPTR *)excVecBase;
}


/*************************************************************************
*
* installE500ParityErrorRecovery - installs the L1 parity error handler
* The handler code is with the E500 cache library.  Handler not used until
* IVOR1 set.
*
* RETURNS: OK or ERROR
*
* \NOMANUAL
*/

STATUS installE500ParityErrorRecovery (void)
{
    sysIvor1Set(_EXC_OFF_L1_PARITY);
    sysL1CSR0Set(sysL1CSR0Get()|_PPC_L1CSR_CPE);  /* Data  parity */
    sysL1CSR1Set(sysL1CSR1Get()|_PPC_L1CSR_CPE);  /* Instr parity */
    return OK;
}

/*************************************************************************
*
* machineException -
*
* \NOMANUAL
*/

void machineException(UINT32 mcsr)
{
    printf("Caused by (from mcsr): 0x%08x\n", mcsr);
    if (mcsr & 0x80000000)
        printf("Machine check input pin\n");
    if (mcsr & 0x40000000)
        printf("Instruction cache parity error\n");
    if (mcsr & 0x20000000)
        printf("Data cache push parity error\n");
    if (mcsr & 0x10000000)
        printf("Data cache parity error\n");
    if (mcsr & 0x00000080)
        printf("Bus instruction address error\n");
    if (mcsr & 0x00000040)
        printf("Bus Read address error\n");
    if (mcsr & 0x00000020)
        printf("Bus Write address error\n");
    if (mcsr & 0x00000010)
        printf("Bus Instruction data bus error\n");
    if (mcsr & 0x00000008)
        printf("Bus Read data bus error\n");
    if (mcsr & 0x00000004)
        printf("Bus Write bus error\n");
    if (mcsr & 0x00000002)
        printf("Bus Instruction parity error\n");
    if (mcsr & 0x00000001)
        printf("Bus Read parity error\n");
}

/*************************************************************************
*
* SpeException -
*
* \NOMANUAL
*/

void SpeException(void)
{
    UINT32 spefscr;
    spefscr = sysSpefscrGet ();

    if ((spefscr & _PPC_SPEFSCR_FINV) != 0)
        printf("Set when an input value on the high side is a NaN, Inf, or \
		   Denorm. Also set on a divide if both the dividend and divisor are zero.\n");
    if ((spefscr & _PPC_SPEFSCR_FDBZ) != 0)
        printf("Embedded floating-point divide-by-zero error. Set if the dividend is non-zero and the divisor is zero.\n");
    if ((spefscr & _PPC_SPEFSCR_FUNF) != 0)
        printf("Embedded floating-point underflow error\n");
    if ((spefscr & _PPC_SPEFSCR_FOVF) != 0)
        printf("Embedded floating-point overflow error\n");
}

/*************************************************************************
*
* generalException -
*
* \NOMANUAL
*/

void generalException(UINT32 sp,UINT32 dear,UINT32 esr,UINT32 type)
{
    UINT32 msr=0;
    UINT32 i=0;
    __asm__ __volatile__ ("mfmsr %0":"=r" (msr):);
    printf("\n########################################\n");
    printf("\n Enter the PPCE500 exception....\n");
    printf("----------------------------------------->\n");
    printf(" The system will hang\n");
    printf("<-----------------------------------------\n");

    printf("task sp:%x,effective address:%x,exception conditions:%x,msr:%x\n",sp,dear,esr,msr);

    switch(type) {
    case _EXC_OFF_CRTL:
        printf("you meet the unsupport Critical exception\n");
        break;
    case _EXC_OFF_MACH:
        printf("you meet the mache check exception\n");
        machineException (esr);
        executeException(EXCEPTION_MACHINE_CHECK);
        break;
    case _EXC_OFF_DATA:
        printf("you meet the data storage exception\n");
        executeException(EXCEPTION_DSI);
        break;
    case _EXC_OFF_INST:
        printf("you meet the inst storage exception\n");
        executeException(EXCEPTION_ISI);
        break;
    case _EXC_OFF_INTR:
        printf("you meet the unsupport extern interrupt exception\n");
        break;
    case _EXC_OFF_ALIGN:
        printf("you meet the aligned exception\n");
        executeException(EXCEPTION_ALIGMENT);
        break;
    case _EXC_OFF_PROG:
        printf("you meet the program exception\n");
        executeException(EXCEPTION_PROGRAM);
        break;
    case _EXC_OFF_DECR:
        printf("you meet the unsupport dec interrupt\n");
        break;
    case _EXC_OFF_DATA_MISS:
        printf("you meet the data miss exception\n");
        break;
    case _EXC_OFF_INST_MISS:
        printf("you meet the inst miss exception\n");
        break;
    case _EXC_OFF_DBG:
        printf("you meet the unsupport dbg exception\n");
        break;
    case _EXC_OFF_SPE:
        printf("you meet the SPE unavaiable exception\n");
        break;
    case _EXC_OFF_VEC_DATA:
        printf("you meet the SPE data exception\n");
        SpeException();
        executeException(EXCEPTION_SPE);
        break;
    case _EXC_OFF_VEC_RND:
        printf("you meet the SPE round exception\n");
        executeException(EXCEPTION_SPE);
        break;
    default :
        printf("??exceptiont 0x%x\n",type);
        break;
    }

    UINT32 *ptrExc = (UINT32 *)(EXC_STACK_HIGH - EXC_FRAME_SIZE);

    for(i=0; i<40; i++) {
        printf("%8s=%8x ",preg[i].gprMsg,*ptrExc);
        ptrExc++;
        if(i%5==0) {
            printf("\n");
        }
    }

    /* system hang here */
    panic ("the target will reboot\n");
}
