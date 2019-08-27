/* sysBoard.c - hardware initialization and boot for p1020rdb */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  platform code for p1020rdb.
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

#include <libc/cagboot.h>
#include <config.h>
#include <libc/stdio.h>
#include <libc/access.h>
#include <ppc/ppc85xx.h>
#include <ppc/toolPpc.h>
#include <ppc/excPpcLib.h>
#include <p1020rdb.h>

IMPORT void sysSdaInit(void);
IMPORT UINT32 sysTimeBaseLGet(void);
IMPORT void setTimeBase(UINT32, UINT32);
IMPORT void sysHid0Set(UINT32);
IMPORT UINT32 sysHid0Get(void);
IMPORT void sysTimeBaseInit (UINT32 tbHI, UINT32 tbLO);
IMPORT void boardReset (void);
IMPORT void panic(const char *fmt, ...);

static UINT32  coreFreq;
static UINT32  core1Freq;
static UINT32  ddrFreq;
UINT32  systemFreq;
UINT32  sysClkFreqGet(void);

/* Define Clock Speed and source  */

#define FREQ_33_MHZ             33333300
#define FREQ_50_MHZ             50000000
#define FREQ_100_MHZ            100000000
#define FREQ_500_MHZ            500000000
#define FREQ_400_MHZ            400000000

#define SYS_CLK_FREQ            FREQ_100_MHZ
#define DEFAULT_SYSCLKFREQ      FREQ_500_MHZ

#define PLAT_RATIO_800_MHZ      20
#define PLAT_RATIO_533_MHZ      16
#define PLAT_RATIO_400_MHZ      12
#define PLAT_RATIO_333_MHZ      8

/* defines */

#define SYS_MODEL_P2020E        "Freescale P2020E - Security Engine"
#define SYS_MODEL_P2020         "Freescale P2020"
#define SYS_MODEL_P2010E        "Freescale P2010E - Security Engine"
#define SYS_MODEL_P2010         "Freescale P2010"
#define SYS_MODEL_P1020E        "Freescale P1020E - Security Engine"
#define SYS_MODEL_P1020         "Freescale P1020"
#define SYS_MODEL_P1011E        "Freescale P1011E - Security Engine"
#define SYS_MODEL_P1011         "Freescale P1011"
#define SYS_MODEL               "Freescale P1020"

#define SYS_MODEL_E500          "Freescale E500 : Unknown system version"
#define SYS_MODEL_UNKNOWN       "Freescale Unknown processor"

#define SYS_PCB_REV_A           0
#define SYS_PCB_REV_B           1
#define SYS_PCB_REV_C           2
#define SYS_PCB_REV_D           3
#define SYS_PCB_REV_UNKNOWN     4
#define SYS_PCB_REV_MASK        0x10100000
#define SYS_CLK_SW_MASK         0x00200000

LOCAL char * physTop = NULL;
LOCAL char * memTop  = NULL;

/* Clock Ratio Tables */

#ifndef _CAG_CONFIG_FREQ
#define MAX_VALUE_DDR_RATIO     16
static UINT32 ddrRatioTable[MAX_VALUE_DDR_RATIO] = {
    0,  0,  0,  3,  4,  0, 6,  0,
    8,  0,  10, 0,  12, 0, 14, 0
};

#define MAX_VALUE_PLAT_RATIO    32
static UINT32 platRatioTable [MAX_VALUE_PLAT_RATIO][2] = {
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 3, 0 },
    { 4,  0 }, { 5, 0 }, { 6,  0 }, { 0, 0 },
    { 8,  0 }, { 9, 0 }, { 10, 0 }, { 0, 0 },
    { 12, 0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 },
    { 0,  0 }, { 0, 0 }, { 0,  0 }, { 0, 0 }
};

#define MAX_VALUE_E500_RATIO    10
static UINT32 e500RatioTable [MAX_VALUE_PLAT_RATIO][2] = {
    { 0, 0 }, { 0, 0 }, { 1, 0 }, { 3, 1 }, { 2, 0 },
    { 5, 1 }, { 3, 0 }, { 7, 1 }, { 4, 0 }, { 9, 1 }
};
#endif

/*******************************************************************************
*
* sysClkFreqGet - return the clock freq of the system bus(CCB FREQ)
*
* This routine returns the clock freq of the system bus
*
* RETURNS: Clock freq of the system bus
*
* ERRNO: N/A
*
* \NOMANUAL
*/

UINT32 sysClkFreqGet (void)
{
#ifndef _CAG_CONFIG_FREQ
    UINT32  sysClkFreq;
    UINT32  e500Ratio;
    UINT32  e5001Ratio;
    UINT32  platRatio;
    UINT32  ddrRatio;

    platRatio = M85XX_PORPLLSR_PLAT_RATIO(_CAG_CONFIG_CCSBAR);
    ddrRatio = M85XX_PORPLLSR_DDR_RATIO(_CAG_CONFIG_CCSBAR);

    if (   (platRatio >= ARRAY_SIZE(platRatioTable))
           || (platRatioTable[platRatio] == 0)
       )
        return(DEFAULT_SYSCLKFREQ); /* A default value better than zero or -1 */

    systemFreq = FREQ_66_MHZ;
    ddrFreq = DDR_CLK_FREQ;

    ddrFreq = ddrFreq * ddrRatioTable[ddrRatio];
    sysClkFreq = ((UINT32)(systemFreq * platRatioTable[platRatio][0])) >> \
                 ((UINT32)platRatioTable[platRatio][1]);

    e500Ratio = M85XX_PORPLLSR_E500_RATIO(_CAG_CONFIG_CCSBAR);
    e5001Ratio = M85XX_PORPLLSR_E500_1_RATIO(_CAG_CONFIG_CCSBAR);

    coreFreq = ((UINT32)(sysClkFreq * e500RatioTable[e500Ratio][0])) >> \
               ((UINT32)e500RatioTable[e500Ratio][1]);
    core1Freq = ((UINT32)(sysClkFreq * e500RatioTable[e5001Ratio][0]))>> \
                ((UINT32)e500RatioTable[e5001Ratio][1]);

    return(sysClkFreq);
#else
    return FREQ_400_MHZ;
#endif
}

/*******************************************************************************
*
* sysUsDelay - delay at least the specified amount of time (in microseconds)
*
* This routine will delay for at least the specified amount of time using the
* lower 32 bit "word" of the Time Base register as the timer.
*
* NOTE:  This routine will not relinquish the CPU; it is meant to perform a
* busy loop delay.  The minimum delay that this routine will provide is
* approximately 10 microseconds.  The maximum delay is approximately the
* size of uint32_t32; however, there is no roll-over compensation for the total
* delay time, so it is necessary to back off two times the system tick rate
* from the maximum.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void sysUsDelay
(
    int    delay        /* length of time in microsec to delay */
)
{
    register UINT32 baselineTickCount;
    register UINT32 curTickCount;
    register UINT32 terminalTickCount;
    register int actualRollover = 0;
    register int calcRollover = 0;
    UINT32 ticksToWait;
    UINT32 requestedDelay;
    UINT32 oneUsDelay;

    /* Exit if no delay count */

    if ((requestedDelay = delay) == 0)
        return;

    /*
     * Get the Time Base Lower register tick count, this will be used
     * as the baseline.
     */

    baselineTickCount = sysTimeBaseLGet();

    /*
     * Calculate number of ticks equal to 1 microsecond
     *
     * The Time Base register and the Decrementer count at the same rate:
     * once per 8 System Bus cycles.
     *
     * e.g., 199999999 cycles     1 tick      1 second            25 ticks
     *       ----------------  *  ------   *  --------         ~  --------
     *       second               8 cycles    1000000 microsec    microsec
     */

    /* add to round up before div to provide "at least" */

    oneUsDelay = (((sysClkFreqGet() >> 3) + 1000000) / 1000000);

    /* Convert delay time into ticks */

    ticksToWait = requestedDelay * oneUsDelay;

    /* Compute when to stop */

    terminalTickCount = baselineTickCount + ticksToWait;

    /* Check for expected rollover */

    if (terminalTickCount < baselineTickCount) {
        calcRollover = 1;
    }

    do {

        /*
         * Get current Time Base Lower register count.
         * The Time Base counts UP from 0 to
         * all F's.
         */

        curTickCount = sysTimeBaseLGet();

        /* Check for actual rollover */

        if (curTickCount < baselineTickCount) {
            actualRollover = 1;
        }

        if (((curTickCount >= terminalTickCount)
             && (actualRollover == calcRollover)) ||
            ((curTickCount < terminalTickCount)
             && (actualRollover > calcRollover))) {

            /* Delay time met */

            break;
        }
    } while (TRUE); /* breaks above when delay time is met */
}

/******************************************************************************
*
* sysUartClkRateGet - return console clock rate
*
* This routine returns system console clock rate
*
* RETURNS: system console clock rate
*
* ERRNO: N/A
*
* \NOMANUAL
*/

UINT32 sysUartClkRateGet(void)
{
    return FREQ_400_MHZ;
}

/******************************************************************************
*
* sysMiscInit - initialize board specific settings
*
* This routine initializes board specific settings
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void sysMiscInit(void)
{
    /* enable time base for sysUsDelay() */

    sysTimeBaseInit(0, 0);
    sysHid0Set(sysHid0Get() | _PPC_HID0_TBEN);

    sysUsDelay(500000);
}


/*******************************************************************************
*
* coreLbcShow - Show routine for local bus controller
*
* This routine shows the local bus controller registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void coreLbcShow (void)
{
    VINT32 tmp, tmp2;

    tmp = * (VINT32 *) M85XX_BR0(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR0(_CAG_CONFIG_CCSBAR);
    printf("Local bus BR0 = 0x%x\tOR0 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR1(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR1(_CAG_CONFIG_CCSBAR);
    printf("Local bus BR1 = 0x%x\tOR1 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR2(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR2(_CAG_CONFIG_CCSBAR);
    printf("Local bus BR2 = 0x%x\tOR2 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR3(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR3(_CAG_CONFIG_CCSBAR);
    printf("Local bus BR3 = 0x%x\tOR3 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR4(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR4(_CAG_CONFIG_CCSBAR);
    printf("Local bus BR4 = 0x%x\tOR4 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR5(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR5(_CAG_CONFIG_CCSBAR);
    printf("Local bus BR5 = 0x%x\tOR5 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR6(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR6(_CAG_CONFIG_CCSBAR);
    printf("Local bus BR6 = 0x%x\tOR6 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_BR7(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_OR7(_CAG_CONFIG_CCSBAR);
    printf("Local bus BR7 = 0x%x\tOR7 = 0x%x\n", tmp, tmp2);

    tmp = * (VINT32 *) M85XX_LBCR(_CAG_CONFIG_CCSBAR);
    printf("Local bus LBCR = 0x%x\n", tmp);

    tmp = * (VINT32 *) M85XX_LCRR(_CAG_CONFIG_CCSBAR);
    printf("Local bus LCRR = 0x%x\n", tmp);
}

void coreLawShow (void)
{
    VINT32 tmp,tmp2,i;
    i=0;
    tmp  = * (VINT32 *) M85XX_LAWBAR0(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_LAWAR0(_CAG_CONFIG_CCSBAR);
    printf("Local Access Window %d BAR = 0x%8x, AR = 0x%8x\n",i++,tmp,tmp2);

    tmp  = * (VINT32 *) M85XX_LAWBAR1(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_LAWAR1(_CAG_CONFIG_CCSBAR);
    printf("Local Access Window %d BAR = 0x%8x, AR = 0x%8x\n",i++,tmp,tmp2);

    tmp  = * (VINT32 *) M85XX_LAWBAR2(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_LAWAR2(_CAG_CONFIG_CCSBAR);
    printf("Local Access Window %d BAR = 0x%8x, AR = 0x%8x\n",i++,tmp,tmp2);

    tmp  = * (VINT32 *) M85XX_LAWBAR3(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_LAWAR3(_CAG_CONFIG_CCSBAR);
    printf("Local Access Window %d BAR = 0x%8x, AR = 0x%8x\n",i++,tmp,tmp2);

    tmp  = * (VINT32 *) M85XX_LAWBAR4(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_LAWAR4(_CAG_CONFIG_CCSBAR);
    printf("Local Access Window %d BAR = 0x%8x, AR = 0x%8x\n",i++,tmp,tmp2);

    tmp  = * (VINT32 *) M85XX_LAWBAR5(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_LAWAR5(_CAG_CONFIG_CCSBAR);
    printf("Local Access Window %d BAR = 0x%8x, AR = 0x%8x\n",i++,tmp,tmp2);

    tmp  = * (VINT32 *) M85XX_LAWBAR6(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_LAWAR6(_CAG_CONFIG_CCSBAR);
    printf("Local Access Window %d BAR = 0x%8x, AR = 0x%8x\n",i++,tmp,tmp2);

    tmp  = * (VINT32 *) M85XX_LAWBAR7(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_LAWAR7(_CAG_CONFIG_CCSBAR);
    printf("Local Access Window %d BAR = 0x%8x, AR = 0x%8x\n",i++,tmp,tmp2);

    tmp  = * (VINT32 *) M85XX_LAWBAR8(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_LAWAR8(_CAG_CONFIG_CCSBAR);
    printf("Local Access Window %d BAR = 0x%8x, AR = 0x%8x\n",i++,tmp,tmp2);

    tmp  = * (VINT32 *) M85XX_LAWBAR9(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_LAWAR9(_CAG_CONFIG_CCSBAR);
    printf("Local Access Window %d BAR = 0x%8x, AR = 0x%8x\n",i++,tmp,tmp2);

    tmp  = * (VINT32 *) M85XX_LAWBAR10(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_LAWAR10(_CAG_CONFIG_CCSBAR);
    printf("Local Access Window %d BAR = 0x%8x, AR = 0x%8x\n",i++,tmp,tmp2);

    tmp  = * (VINT32 *) M85XX_LAWBAR11(_CAG_CONFIG_CCSBAR);
    tmp2 = * (VINT32 *) M85XX_LAWAR11(_CAG_CONFIG_CCSBAR);
    printf("Local Access Window %d BAR = 0x%8x, AR = 0x%8x\n",i++,tmp,tmp2);

}


#   define xbit0(x, n)    ((x & (1 << (31 - n))) >> (31 - n))  /* 0..31 */
#   define xbit32(x, n)   ((x & (1 << (63 - n))) >> (63 - n))  /* 32..63 */
#   define onoff0(x, n)   xbit0(x, n) ? "ON", "OFF"
#   define onoff32(x, n)  xbit32(x, n) ? "ON", "OFF"

/*******************************************************************************
*
* coreShow - Show routine for core registers
*
* This routine shows the core registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void coreShow (void)
{
    VUINT32 tmp, tmp2;

    tmp = sysMsrGet();
    printf("MSR - 0x%x\n", tmp);
    printf("      UCLE-%x SPE-%x WE-%x CE-%x EE-%x PR-%x ME-%x\n",
           xbit32(tmp,37), xbit32(tmp,38), xbit32(tmp,45), xbit32(tmp,46),
           xbit32(tmp,48), xbit32(tmp,49), xbit32(tmp,51));
    printf("      UBLE-%x DE-%x IS-%x DS-%x PMM-%x\n",
           xbit32(tmp,53), xbit32(tmp,54), xbit32(tmp,58), xbit32(tmp,59),
           xbit32(tmp,61));
    tmp = sysHid0Get();
    tmp2 = sysHid1Get();
    printf("HID0 = 0x%x, HID1 = 0x%x\n", tmp, tmp2);
    tmp = sysL1CSR0Get();
    printf("L1CSR0: cache is %s - 0x%x\n", tmp&1?"ON":"OFF", tmp);
    tmp = sysL1CSR1Get();
    printf("L1CSR1: Icache is %s - 0x%x\n", tmp&1?"ON":"OFF", tmp);
    tmp = sysL1CFG0Get();
    tmp2 = sysL1CFG1Get();
    printf("L1CFG0 = 0x%x, L1CFG1 = 0x%x\n", tmp, tmp2);
    tmp = *(VUINT32 *) (_CAG_CONFIG_CCSBAR + 0x20000);
    printf("L2CTL - 0x%x\n", tmp);
    printf("        l2 is %s\n", tmp&0x80000000?"ON":"OFF");
    printf("        l2siz-%x l2blksz-%x l2do-%x l2io-%x\n",
           (xbit0(tmp,2)<<1)|xbit0(tmp,3), (xbit0(tmp,4)<<1)|xbit0(tmp,5),
           xbit0(tmp,9), xbit0(tmp,10));
    printf("        l2pmextdis-%x l2intdis-%x l2sram-%x\n",
           xbit0(tmp,11), xbit0(tmp,12),
           (xbit0(tmp,13)<<2)|(xbit0(tmp,14)<<1)|xbit0(tmp,15));
    tmp = *(VUINT32 *) (_CAG_CONFIG_CCSBAR + 0x20100);
    tmp2 = *(VUINT32 *) (_CAG_CONFIG_CCSBAR + 0x20108);
    printf("L2SRBAR0 - 0x%x\n", tmp);
    printf("L2SRBAR1 - 0x%x\n", tmp2);

    printf("Core CPU0 Freq = %3d Hz\n", coreFreq);
    printf("Core CPU1 Freq = %3d Hz\n", core1Freq);
    printf("DDR Freq = %3d Hz\n", ddrFreq);
    printf("CCB Freq = %3d Hz\n", sysClkFreqGet());
    printf("System Freq = %3d Hz\n", systemFreq);

}

/*******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* RETURNS: A pointer to the string.
*
* ERRNO: N/A
*/

char * sysModel (void)
{
    UINT32 device;
    char* retChar = NULL;

    device = *M85XX_SVR(_CAG_CONFIG_CCSBAR);
    switch(device & 0xffff0000) {
    case 0x80E20000:
        retChar = SYS_MODEL_P2020;
        break;
    case 0x80EA0000:
        retChar = SYS_MODEL_P2020E;
        break;
    case 0x80E30000:
        retChar = SYS_MODEL_P2010;
        break;
    case 0x80EB0000:
        retChar = SYS_MODEL_P2010E;
        break;
    case 0x80E40000:
        retChar = SYS_MODEL_P1020;
        break;
    case 0x80EC0000:
        retChar = SYS_MODEL_P1020E;
        break;
    case 0x80E50000:
        retChar = SYS_MODEL_P1011;
        break;
    case 0x80ED0000:
        retChar = SYS_MODEL_P1011E;
        break;
    default:
        retChar = SYS_MODEL_E500;
        break;
    }

    device = *M85XX_PVR(_CAG_CONFIG_CCSBAR);

    if ((device & 0xffff0000) != 0x80210000)
        retChar =SYS_MODEL_UNKNOWN;

    return(retChar);
}

/*******************************************************************************
*
* sysPcbRev - return the PCB version of P1020/2020RDB board
*
* This routine will return the PCB version of P1020/2020RDB board.
*
* RETURNS: P1020/2020RDB PCB version.
*/

UINT32 sysPcbRev (void)
{
    UINT32 pcbVer;

    *((VUINT32 *)GPDIR) &= ~SYS_PCB_REV_MASK;

    pcbVer = *((VUINT32 *)GPDAT) & SYS_PCB_REV_MASK;

    switch(pcbVer) {
    case 0x10000000:
        pcbVer = SYS_PCB_REV_A;
        break;
    case 0x10100000:
        pcbVer = SYS_PCB_REV_B;
        break;
    case 0x00100000:
        pcbVer = SYS_PCB_REV_C;
        break;
    case 0x00000000:
        pcbVer = SYS_PCB_REV_D;
        break;
    default:
        pcbVer = SYS_PCB_REV_UNKNOWN;
        break;
    }

    return (pcbVer);
}

/*******************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* RETURNS: The address of the top of physical memory.
*
* ERRNO: N/A
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop (void)
{

    if (physTop == NULL) {
        physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);
    }

    return(physTop) ;
}

/*******************************************************************************
*
* sysMemTop - get the address of the top of memory
*
* This routine returns a pointer to the first byte of memory not
* controlled or used by os.
*
* The user can reserve memory space by defining the macro USER_RESERVED_MEM
* in config.h.  This routine returns the address of the reserved memory
* area.  The value of USER_RESERVED_MEM is in bytes.
*
* RETURNS: The address of the top of memory.
*
* ERRNO: N/A
*/

char * sysMemTop (void)
{

    if (memTop == NULL) {
        memTop = sysPhysMemTop () - USER_RESERVED_MEM;
    }

    return memTop;
}

void boardReset (void)
{
    /* Next try asserting HRESET_REQ */

    *M85XX_RSTCR(_CAG_CONFIG_CCSBAR) = 0x2;
    sysUsDelay(1000);
}

/**
 * hang - stop processing by staying in an endless loop
 *
 * The purpose of this function is to stop further execution of code cause
 * something went completely wrong.  To catch this and give some feedback to
 * the user one needs to catch the bootstage_error (see show_boot_progress())
 * in the board code.
 */
void hang(void)
{
    printf("### ERROR ### Please RESET the board ###\n");
    printf("Shut down here,%s\n",__FUNCTION__);
}

void panic(const char *fmt, ...)
{
    printf(" the cagOS is panic here\n");
#if defined(_CAG_CONFIG_HANG)
    hang();
#else
    sysUsDelay(100000);	/* allow messages to go out */
    boardReset();
#endif
    while (1)
        ;
}

void __assert_fail(const char *assertion, const char *file, unsigned line,
                   const char *function)
{
    /* This will not return */
    panic("%s:%u: %s: Assertion `%s' failed.", file, line, function,
          assertion);
}
