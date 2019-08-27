/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  platform IRQ code for p1020rdb.
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
 * This file contains the board EPIC component
 */

/* includes */

#include <config.h>
#include <p1020rdb.h>
#include <kernel/sysPic.h>
#include <libc/cagboot.h>
#include <ppc/toolPpc.h>

/* defines */

#define PPC_PVR_E500V2  0x80210000
#define PPC_SVR_MASK    0xFFF00000
#define NULL 0

/* forward declarations */

int	sysEpicIntEnable (UINT32 vector);
int	sysEpicIntDisable (UINT32 vector);

void      sysEpicInit (void);
STATUS    sysEpicIntSourceSet (UINT32 srcAddr, int polarity,
                               int sense, int priority, int vector);
int       sysEpicCurTaskPrioSet (int prioNum, int cpu);
UINT32    sysEpicCpuAvailGet (void);

/* locals */

LOCAL UINT32 pEpicRegBase = _CAG_CONFIG_CCSBAR;

/* externs */
IMPORT void  OSTimeTick (void);

/* globals */

UINT32 * sysEpicIntAckReg;
UINT16 sysEpicSpuriousVector=0xff;		/* Superious Vector Number */
UINT32 numInInts = 0;

/*******************************************************************************
*
* sysEpicInit - initialize the sysEpic controller
*
* This routine resets the global Configuration Register, thus it:
*  -  disables all interrupts
*  -  sets sysEpic registers to reset values
*
* It then sets the EPIC operation mode to Mixed Mode (vs. Pass Through
* mode). At this point only mixed mode is supported which means the EPIC
* is not configured for the pass through mode.
*
* Only direct interrupt sources is supported by the PIC.  Serial
* stype interrupt is not available.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysEpicInit (void)
{
    UINT32   gcrVal;
    UINT32   idrVal;
    UINT32  brr1;
    int     irq = 0;
    int     i = 0 ;
    BOOL    onBootCpu = (EPIC_REG_READ(EPIC_WHO_AM_I_REG)) == 0
                        ? TRUE : FALSE;

    UINT32 exPolar = EPIC_EX_DFT_POLAR;
    UINT32 exSense = EPIC_EX_DFT_SENSE;
    UINT32 inPolar = EPIC_IN_DFT_POLAR;

    sysEpicIntAckReg = (UINT32 *) ((UINT32) pEpicRegBase + EPIC_INT_ACK_REG);

    irqInit();/*initialize irq related variables*/

    /* Only one kernel image should initialize Epic and that is CPU0 */

    if (onBootCpu) {

        gcrVal = EPIC_REG_READ(EPIC_GLOBAL_REG);

        gcrVal |= (EPIC_GCR_RESET);

        EPIC_REG_WRITE(EPIC_GLOBAL_REG, gcrVal);

        /* wait for the reset sequence to be completed */

        while (EPIC_REG_READ(EPIC_GLOBAL_REG) & EPIC_GCR_RESET) {
            ; /* do nothing */
        }

        /* set the spurious vector to match linux */
        EPIC_REG_WRITE(EPIC_SPUR_VEC_REG, sysEpicSpuriousVector);

        gcrVal = EPIC_REG_READ(EPIC_GLOBAL_REG);

        gcrVal |= (EPIC_GCR_MODE_MIXED);	/* configure for mixed mode */

        EPIC_REG_WRITE(EPIC_GLOBAL_REG, gcrVal);
    }

    while (((UINT16)EPIC_REG_READ(EPIC_INT_ACK_REG) & 0xffff) !=
           (UINT16)sysEpicSpuriousVector) {
        /* do nothing */
        _CAG_ASM ("isync;sync");
    }

    for ( irq = 0 ; irq < EPIC_EX_IRQ_NUM ; irq++ ) {

        /* Only initialize interrupts if boot cpu */
        if (onBootCpu) {
            sysEpicIntDisable (irq);
            idrVal = EPIC_REG_READ(EPIC_EX_DEST_REG(irq));
            idrVal &= ~0x1;
            EPIC_REG_WRITE(EPIC_EX_DEST_REG(irq), idrVal);
        }

        if (onBootCpu)
            sysEpicIntSourceSet (EPIC_EX_VEC_REG(irq),
                                 exPolar, exSense, 0x0, 0x0);
    }

    for ( irq = 0; i < EPIC_IN_IRQ_NUM; i++ ) {
        if (onBootCpu) {
            sysEpicIntDisable (irq);
            idrVal = EPIC_REG_READ(EPIC_IN_DEST_REG(irq));
            idrVal &= ~0x1;
            EPIC_REG_WRITE(EPIC_IN_DEST_REG(irq), idrVal);
        }
        if (onBootCpu)
            sysEpicIntSourceSet (EPIC_IN_VEC_REG(irq), inPolar, 0x0, 0x0, 0x0);
    }

    if (onBootCpu) {
        for ( i = 0 ; i < sysEpicCpuAvailGet() ; i++ )
            sysEpicCurTaskPrioSet (EPIC_PRIORITY_MIN, i);
    }

    brr1 = EPIC_REG_READ(EPIC_BRR1);

    if(brr1 == FSL_EPIC_441) {
        numInInts = 128;
    } else {
        numInInts = 64;
    }

#if _CAG_CONFIG_AMP1 == 1
    while (EPIC_REG_READ(EPIC_EX_VEC_REG(0x2)) & EPIC_EIVPR_INTR_MSK) {
        printf("Waiting the EPIC finish init\n"); /* do nothing */
    }
#endif

}


/*******************************************************************************
*
* sysEpicCurTaskPrioSet - set the priority of the current task.
*
* NOTES
*
* sysEpicCurTaskPrioSet sets the priority of the Processor Current Task
* Priority register to the value of the prioNum parameter.  This function
* should be called after sysEpicInit() to lower the priority of the processor
* current task. Note that valid priority values are 0 through 15 (15 being
* the highest priority)
*
* \NOMANUAL
*
* RETURNS: previous priority of the task.
*
* ERRNO: N/A
*/

int sysEpicCurTaskPrioSet
(
    int prioNum,
    int cpu
)
{
    INT32 oldPrio;

    if ((prioNum < EPIC_PRIORITY_MIN) || (prioNum > EPIC_PRIORITY_MAX)) {
        return ((int)EPIC_INV_PRIO_ERROR);
    }

    oldPrio = EPIC_REG_READ(EPIC_CTASK_PRIn_REG(cpu));
    EPIC_REG_WRITE(EPIC_CTASK_PRIn_REG(cpu), prioNum);

    return (oldPrio);
}

/*******************************************************************************
*
* sysEpicIntEnable - enable an EPIC interrupt, given its IVPR
*
* This function clears the mask bit of an external, an internal or
* a Timer register to enable the interrupt.
*
* <srcAddr> is the address offset of the Vector Priority register
*
* \NOMANUAL
*
* RETURNS: OK or an error code if the IVPR passed in was invalid.
*
* ERRNO: N/A
*/

int sysEpicIntEnable
(
    UINT32 	vector
)
{
    UINT32 	srcVal;
    UINT32 	errCode;
    UINT32 	srcAddr = EPIC_EX_VEC_REG(vector);

    if (vector >= EPIC_MAX_ALL_IRQS)
        return (ERROR);

    if(srcAddr > EPIC_EX_INT11_VEC_REG)
        errCode = EPIC_IN_INTERRUPT;
    else
        errCode = EPIC_EX_INTERRUPT;

    srcVal = EPIC_REG_READ(srcAddr);

    switch (errCode) {
    case EPIC_EX_INTERRUPT:
        srcVal &= ~(EPIC_EIVPR_INTR_MSK);   /* clear the mask bit */
        break;

    case EPIC_IN_INTERRUPT:
        srcVal &= ~(EPIC_IIVPR_INTR_MSK);   /* clear the mask bit */
        break;
    default:
        return (ERROR);
    }

    EPIC_REG_WRITE(srcAddr, srcVal);

    return OK;
}

/*******************************************************************************
*
* sysEpicIntDisable - disable an EPIC interrupt, given its IVPR
*
* This function sets the mask bit of an external, an internal,
* a timer, a message, or an IPI register to disable the interrupt.
* <srcAddr> is the address offset of the Vector Priority register.
*
* \NOMANUAL
*
* RETURNS: OK or an error code if the IVPR passed in was invalid.
*
* ERRNO: N/A
*/

int sysEpicIntDisable
(
    UINT32 	vector
)
{
    UINT32 	srcVal;
    UINT32 	errCode;
    UINT32 	srcAddr = EPIC_EX_VEC_REG(vector);

    if (vector >= EPIC_MAX_ALL_IRQS)
        return (ERROR);

    if(srcAddr > EPIC_EX_INT11_VEC_REG)
        errCode = EPIC_IN_INTERRUPT;
    else
        errCode = EPIC_EX_INTERRUPT;

    srcVal = EPIC_REG_READ(srcAddr);

    switch (errCode) {
    case EPIC_EX_INTERRUPT:
        srcVal |= EPIC_EIVPR_INTR_MSK;	/* set the mask bit */
        break;

    case EPIC_IN_INTERRUPT:
        srcVal |= EPIC_IIVPR_INTR_MSK;	/* set the mask bit */
        break;

    default:
        return (ERROR);
    }

    EPIC_REG_WRITE(srcAddr, srcVal);

    return OK;
}

/*******************************************************************************
*
* sysEpicIntSourceSet - set interrupt parameters for an interrupt register
*
* \NOMANUAL
*
* RETURNS: OK or one of errors above
*
* ERRNO: N/A
*/

STATUS sysEpicIntSourceSet
(
    UINT32 	srcAddr,
    int 	polarity,
    int 	sense,
    int 	priority,
    int 	vector
)
{
    UINT32 	srcVal;
    UINT32	errCode;

    if(srcAddr > EPIC_EX_INT11_VEC_REG)
        errCode = EPIC_IN_INTERRUPT;
    else
        errCode = EPIC_EX_INTERRUPT;

    srcVal = EPIC_REG_READ(srcAddr);

    switch (errCode) {
    case EPIC_EX_INTERRUPT:
        if (srcVal & EPIC_EIVPR_INTR_ACTIVE) {
            return (EPIC_INTER_IN_SERVICE);
        }
        /* mask off current settings */
        srcVal &= ~(EPIC_EIVPR_INTR_POLARITY |
                    EPIC_EIVPR_INTR_SENSE |
                    EPIC_EIVPR_PRIORITY_MSK  |
                    EPIC_EIVPR_VECTOR_MSK);
        /* set new values */
        srcVal |= (EPIC_EIVPR_POLARITY (polarity) |
                   EPIC_EIVPR_SENS (sense) |
                   EPIC_EIVPR_PRIORITY (priority) |
                   EPIC_EIVPR_VECTOR (vector));
        break;

    case EPIC_IN_INTERRUPT:
        if (srcVal & EPIC_IIVPR_INTR_ACTIVE) {
            return (EPIC_INTER_IN_SERVICE);
        }
        /* mask off current settings */
        srcVal &= ~(EPIC_IIVPR_INTR_POLARITY |
                    EPIC_IIVPR_PRIORITY_MSK  |
                    EPIC_IIVPR_VECTOR_MSK);
        /* set new values */
        srcVal |= (EPIC_IIVPR_POLARITY (polarity) |
                   EPIC_IIVPR_PRIORITY (priority) |
                   EPIC_IIVPR_VECTOR (vector));
        break;

    default:
        return (ERROR);
    }

    EPIC_REG_WRITE(srcAddr, srcVal);

    return (OK);
}


/*******************************************************************************
*
* sysEpicCpuAvailGet - method to obtain number of CPUs available
*
*/

UINT32 sysEpicCpuAvailGet ( )
{
    UINT32 cpuNum;
    UINT32 frr = EPIC_REG_READ(EPIC_FEATURES_REG);
    cpuNum  = ((frr & EPIC_FRR_NCPU_MASK) >> EPIC_FRR_NCPU_SHIFT) + 1;
    return (cpuNum);
}


/*******************************************************************************
*
* sysEpicConfig - config the epic fpga ext int
*
*/

void sysEpicConfig(void)
{
    UINT32 irq = 0x2;
    UINT32 idrVal = 0;
    UINT32 vecVal = 0;
    irqRregister(irq, OSTimeTick, NULL, "FPGA_EXT");
#if  _CAG_CONFIG_AMP0 == 1
    /* CORE 0 */
    vecVal = EPIC_REG_READ(EPIC_EX_VEC_REG(irq));
    vecVal |= 0x20002;
    EPIC_REG_WRITE(EPIC_EX_VEC_REG(irq), vecVal);
    idrVal = EPIC_REG_READ(EPIC_EX_DEST_REG(irq));
    idrVal |= 0x1;
    EPIC_REG_WRITE(EPIC_EX_DEST_REG(irq), idrVal);
    sysEpicIntEnable (irq);
#else
    /* CORE 1 */
    vecVal = EPIC_REG_READ(EPIC_EX_VEC_REG(irq));
    vecVal |= 0x20002;
    EPIC_REG_WRITE(EPIC_EX_VEC_REG(irq), vecVal);
    idrVal = EPIC_REG_READ(EPIC_EX_DEST_REG(irq));
    idrVal |= 0x2;
    //idrVal |= 0x3;
    EPIC_REG_WRITE(EPIC_EX_DEST_REG(irq), idrVal);
    sysEpicIntEnable (irq);
#endif
}
