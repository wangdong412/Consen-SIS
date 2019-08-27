/* sysCpuLib.c - Freescale P1020RDB board system-dependent library */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  the cagOs entry code.
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
 * This library provides board-specific routines for P1020RDB
 */

/*
DESCRIPTION
This library provides board-specific routines.  The chip drivers included are:

    sysFslSpi.c         - FreeScale SPI controller support.
    spS25flxxSpiFlash.c - S25FL128P SPI flash driver.
    sysL2Cache.c        - L2 cache support.

INCLUDE FILES: sysLib.h
*/

/* includes */

#include <libc/cagboot.h>
#include <config.h>
#include <drv_config.h>
#include <libc/stdio.h>
#include <libc/access.h>
#include <libc/system.h>
#include <libc/string.h>
#include <ppc/ppc85xx.h>
#include <ppc/mmuE500Lib.h>
#include <ppc/toolPpc.h>
#include <ppc/excPpcLib.h>
#include <libc/gpio.h>
#include "sysL2Cache.h"
#include "../driver/device/pcie/pcie.h"

#define CACHE_LIBRARY_MODE   (CACHE_COPYBACK | CACHE_SNOOP_ENABLE)
#define CAM_DRAM_CACHE_MODE  _MMU_TLB_ATTR_M
#define TLB_CACHE_MODE       (VM_STATE_CACHEABLE | VM_STATE_MEM_COHERENCY)

/* globals */

typedef enum                            /* CACHE_TYPE */
{
    INSTRUCTION_CACHE = _INSTRUCTION_CACHE,
    DATA_CACHE        = _DATA_CACHE
} CACHE_TYPE;

TLB_ENTRY_DESC sysStaticTlbDesc [] = {

    /* effAddr,  Unused,  realAddr, ts | size | attributes | permissions */

    /* TLB #0.  Flash */

    /* needed be first entry here */

    {
        _CAG_CONFIG_FLASH_BASE_ADRS, 0x0, _CAG_CONFIG_FLASH_BASE_ADRS,
        _MMU_TLB_TS_0   | _MMU_TLB_SZ_64M | _MMU_TLB_IPROT   |
        _MMU_TLB_PERM_W | _MMU_TLB_PERM_X   | _MMU_TLB_ATTR_I  |
        _MMU_TLB_ATTR_G
    },

    {
        0x00000000, 0x0, 0x00000000,
        _MMU_TLB_TS_0   | _MMU_TLB_SZ_1G      | _MMU_TLB_PERM_W |
        _MMU_TLB_PERM_X | CAM_DRAM_CACHE_MODE | _MMU_TLB_IPROT  |
        _MMU_TLB_ATTR_M
    },

    {
        _CAG_CONFIG_CCSBAR, 0x0, _CAG_CONFIG_CCSBAR,
        _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M   | _MMU_TLB_ATTR_I  |
        _MMU_TLB_ATTR_G | _MMU_TLB_PERM_W  | _MMU_TLB_IPROT
    },


    {
        FPGA_PCIE1_BAR_BASE, 0x0, FPGA_PCIE1_BAR_BASE,
        _MMU_TLB_TS_0   | _MMU_TLB_SZ_256M | _MMU_TLB_ATTR_I  |
        _MMU_TLB_ATTR_G | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
    },
    {
        FPGA_PCIE2_BAR_BASE, 0x0, FPGA_PCIE2_BAR_BASE,
        _MMU_TLB_TS_0   | _MMU_TLB_SZ_256M | _MMU_TLB_ATTR_I  |
        _MMU_TLB_ATTR_G | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
    },
    {
        _CAG_CONFIG_SRAM_ADDR, 0x0, _CAG_CONFIG_SRAM_ADDR,
        _MMU_TLB_TS_0   | _MMU_TLB_SZ_1M | _MMU_TLB_ATTR_I  |
        _MMU_TLB_ATTR_G | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
    },
    {
        _CAG_CONFIG_FPGA_ADDR, 0x0, _CAG_CONFIG_FPGA_ADDR,
        _MMU_TLB_TS_0   | _MMU_TLB_SZ_64K | _MMU_TLB_ATTR_I  |
        _MMU_TLB_ATTR_G | _MMU_TLB_PERM_X | _MMU_TLB_PERM_W
    },

};


int sysStaticTlbDescNumEnt = (int)NELEMENTS (sysStaticTlbDesc);

//int sysStaticTlbDescNumEnt = 3;
UINT32 e500EarlyMapTlbId = 0;
UINT32 e500ImaTlbCnt = 2;

/* global variables */

IMPORT UINT8 edata[];
IMPORT UINT8 end[];
IMPORT char _cag_kernel_text_start[];

/* macro to compose 64-bit PHYS_ADDRs */

UINT32  ppcE500ICACHE_LINE_NUM  = (128 * 12);
UINT32  ppcE500DCACHE_LINE_NUM  = (128 * 12);
UINT32  ppcE500DCACHE_FLUSH_NUM;
UINT32  ppcE500CACHE_ALIGN_SIZE = 32;
char * cachePpcReadOrigin;

/* externs */
IMPORT int l2CacheLineNum;
IMPORT void mmuE500TlbDynamicInvalidate(void);
IMPORT void mmuE500TlbStaticInvalidate(void);
IMPORT void mmuE500TlbStaticInit (int numDescs, TLB_ENTRY_DESC *pTlbDesc, BOOL cacheAllow);

IMPORT void sysSdaInit (void);
IMPORT void sysCpuLibInit (void);

IMPORT void sysEnableBP(void);
IMPORT void sysDisableBP(void);
IMPORT void sysL2CacheInit(void);
IMPORT int excVecInit (FUNCPTR * baseAddr);
IMPORT STATUS cacheArchEnable (CACHE_TYPE cache);

IMPORT void sysL1Csr1Set(UINT32);
IMPORT UINT32 sysL2CTLSet(UINT32 value, VUINT32 *addr);
IMPORT void usrShell(void);
IMPORT void speExcInit (void);
IMPORT void sysMiscInit(void);
IMPORT void sysBanner(void);
IMPORT int  sysOs(void);
IMPORT void ns16550DevInit(void);

IMPORT int32_t fs_init(int32_t flash, int32_t dfsid, int32_t fs);
IMPORT void dfs_init(void);
IMPORT void iic_init(void);
IMPORT void nor_init(void);
IMPORT void spi_init(void);
IMPORT void test_test(void);
IMPORT INT32 fs_mount(INT32 type);
IMPORT int sffs_idx;
IMPORT int sffs_idx2;
IMPORT int dfd;
IMPORT int dfd2;

#if defined(SFFS_MULTI_DEVICE) && defined(CONFIG_DEMO)
IMPORT void sffs_multi_demo(void);
#endif

/* forward declarations */

LOCAL   void sysL1CacheQuery(void);

/* includes */

#define HID0_MCP                0x80000000
#define HID1_ABE                0x00001000
#define HID1_ASTME              0x00002000
#define HID1_RXFE               0x00020000

/*******************************************************************************
*
* sysCpuStart - initialize the system hardware
*
* This routine initializes various feature of the P1020RDB board. It sets up
* the control registers, initializes various devices if they are present.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysCpuStart (void)
{
    l2CacheLineNum = ((2048 + 4096) * L2_CACHE_SIZE);

    /* Disable L1 Icache */
#if 0
    sysL1Csr1Set(sysL1CSR1Get() & ~0x1);
#endif
    /* Put cachePpcReadOrigin at the start of data section */

    cachePpcReadOrigin = (char *) &_cag_kernel_text_start;

    /* Check for architecture support for 36 bit physical addressing */

    sysHid0Set(_PPC_HID0_MAS7EN|sysHid0Get());

    /* Enable machine check pin */

    sysHid0Set(HID0_MCP | sysHid0Get());

    /* disable the DEC interrupt */

    sysTcrSet (sysTcrGet() & ~ (_PPC_TCR_DIE));

    /* clear the DEC counter and DECAR */

    sysDecSet (0);
    sysDecarSet (0);

    /* clear the pending DEC interrupt */

    sysTsrSet (_PPC_TSR_DIS);

    sysL1CacheQuery();

    ppcE500DCACHE_FLUSH_NUM = ppcE500DCACHE_LINE_NUM;

#if defined(_CAG_CONFIG_AMP0)
    /* Initialize L2CTL register to reset value */
    sysL2CTLSet(0, M85XX_L2CTL(_CAG_CONFIG_CCSBAR));
#endif

    /*
     * Need to setup static TLB entries for bootrom or any non-MMU
     * enabled images
     */

#if !defined(_CAG_CONFIG_AMP0)
    sysTlbUnmapAll (1, 14);
    mmuE500TlbDynamicInvalidate();
    mmuE500TlbStaticInvalidate();
    mmuE500TlbStaticInit(sysStaticTlbDescNumEnt, &sysStaticTlbDesc[0], TRUE);
    sysTlbUnmapAll (0, sysStaticTlbDescNumEnt - 1);//ezio 10.19
#endif

    sysHid1Set(HID1_ABE); /* Address Broadcast enable */

    /* Init the L2 cache */

    sysL2CacheInit();

    /* Machine check via RXFE for RIO */

    sysHid1Set (sysHid1Get()| HID1_ASTME | HID1_RXFE);

#ifdef _CAG_SPE_SUPPORT
    speExcInit ();
#endif /*  _CAG_SPE_SUPPORT */

    sysEnableBP();

    sysMiscInit();

    _CAG_ASM("isync");
    /* vsc7385 initialize */
    /* DRV_SPI_S25FLXX */
    /* INCLUDE_NAND_FLASH */
    /* hardWareInterFaceInit(); */
    /* CACHE_PIPE_FLUSH(); */
}

/*******************************************************************************
*
* sysRoot - the root task
*
* This is the first task to run under the multitasking kernel.  It performs
* all final initialization and then starts other tasks.
*
* RETURNS: N/A
*/

void sysRoot (void)
{
#if _CAG_CONFIG_SHELL == 1
    usrShell();
#endif

#if 0
    sysMsrSet(sysMsrGet() | taskMsrDefault);
    /* initialize floating point facilities */
    floatInit ();			/* initialize floating point I/O */
    /* install software floating point emulation (if applicable) */
    mathSoftInit ();		/* use software emulation for fp math */
    /* install hardware floating point support (if applicable) */
#ifdef	INCLUDE_HW_FP
    mathHardInit (); 		/* do fppInit() & install hw fp math */
#endif	/* INCLUDE_HW_FP */
    /* initialize Spe library (if applicable) */
#endif

#ifdef _CAG_CONFIG_KERNEL
    sysOs();
#endif /* _CAG_CONFIG_KERNEL */
}

/******************************************************************************
*
* clearBss - clear bss section
*
* This routine clears bss section.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void clearBss(void)
{
    extern unsigned char cag_kernel_bss_start[];
    extern unsigned char cag_kernel_bss_end[];

    memset(cag_kernel_bss_start, 0,
           cag_kernel_bss_end - cag_kernel_bss_start);
}

void sysDeviceInit(void)
{
    printf("...sysDeviceInit\n");
#if 0
    sysAmpShare ();
#endif

#ifdef _CAG_CONFIG_AMP1
    sysPirSet(1);
#endif
    //init device filesystem
    dfs_init();

#ifdef FSL_BOARD
#ifdef CONFIG_IIC
    iic_init();
#endif
#ifdef CONFIG_SPI_FLASH
    printf("Spi init\n");
    spi_init();
#endif
#endif

#ifdef CONFIG_NOR_FLASH
    printf("Nor init\n");
    nor_init();
#endif

#ifdef CONFIG_FS
    printf("fs Init\n");
#ifdef SFFS_MULTI_DEVICE
    sffs_idx = fs_init(0,dfd,0);
    sffs_idx2 = fs_init(0,dfd2,0);
#else
    sffs_idx = fs_init(0,dfd,0);
#endif
#endif

}

/*******************************************************************************
*
* sysEntry - user-defined system initialization routine
*
* This is the first C code executed after the system boots.  This routine is
* called by the assembly language start-up routine sysEntry() which is in the
* sysALib module of the target-specific directory.  It is called with
* interrupts locked out.  The kernel is not multitasking at this point.
*
* RETURNS: N/A
*
*/

void sysEntry
(
    int startType
)
{

    _CAG_ASM ("");
    sysSdaInit ();
    _CAG_ASM ("");

#if 0
    bzero (edata, end - edata);		/* zero out bss variables */
#endif
    clearBss ();

#if 0
    cacheLibInit (USER_I_CACHE_MODE, USER_D_CACHE_MODE);
#endif

    excVecInit ((FUNCPTR *) VEC_BASE_ADRS);	/* install exception vectors */

    sysCpuLibInit();		                /* init CPU identification data */

    sysCpuStart ();					        /* initialize system hardware */

    ns16550DevInit();

#if !defined(_CAG_CONFIG_AMP0)
    cacheArchEnable (INSTRUCTION_CACHE);
    cacheArchEnable (DATA_CACHE);
#endif

    sysBanner();

    sysDeviceInit();

    sysRoot();
}

/*******************************************************************************
*
* sysL1CacheQuery - configure L1 cache size and alignment
*
* Populates L1 cache size and alignment from configuration registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysL1CacheQuery (void)
{
    UINT32 temp;
    UINT32 align;
    UINT32 cachesize;

    temp = sysL1CFG0Get();

    cachesize = (temp & 0xFF) << 10;

    align = (temp >> 23) & 0x3;

    switch (align) {
    case 0:
        ppcE500CACHE_ALIGN_SIZE = 32;
        break;
    case 1:
        ppcE500CACHE_ALIGN_SIZE = 64;
        break;
    default:
        ppcE500CACHE_ALIGN_SIZE = 32;
        break;
    }

    ppcE500DCACHE_LINE_NUM = (cachesize / ppcE500CACHE_ALIGN_SIZE);
    ppcE500ICACHE_LINE_NUM = (cachesize / ppcE500CACHE_ALIGN_SIZE);

    /*
     * The core manual suggests for a 32 byte cache line and 8 lines per set
     * we actually need 12 unique address loads to flush the set.
     * The number of lines to flush should be ( 3/2 * cache lines )
     */

    ppcE500DCACHE_LINE_NUM = (3 * ppcE500DCACHE_LINE_NUM) >> 1;
    ppcE500ICACHE_LINE_NUM = (3 * ppcE500ICACHE_LINE_NUM) >> 1;

}
