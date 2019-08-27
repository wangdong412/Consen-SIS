/* p1020rdb.h - Freescale P1020RDB board header */

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

/*
 * This file contains I/O addresses and related constants for the
 * Freescale P1020RDB board.
*/

#ifndef __INCp1020rdbh
#define __INCp1020rdbh

#include <libc/cagboot.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define P1020RDB_PC_REVB

/* Base Address of Memory Mapped Registers */

#define CCSBAR_RESET            0xff700000
#define CCSR_SIZE               0x00100000

#define MP_MAGIC_RBV            0xfeedfeed
#define MP_MAGIC_CACHE_FLUSHED  0xD07ED07E

#define M85XX_ALTCBAR(base)     (CAST(VUINT32 *)((base) + 0x08))
#define M85XX_ALTCAR(base)      (CAST(VUINT32 *)((base) + 0x10))
#define M85XX_BPTR(base)        (CAST(VUINT32 *)((base) + 0x20))

#define BPTR_EN                 0x80000000

/*
 * Space for the SMP-reboot detection structure.  This should go in
 * memory with the same volatility as DRAM -- i.e. it will survive
 * a reset via the PIXIS but not a power-cycle.  It would be nice
 * for it to be accessible without setting up the DRAM controller,
 * but absent knowledge of any such space on the board we'll just
 * put it in DRAM (and therefore it can't be tested until after the
 * DRAM controller has been set up).
 */

#define RBV_ADRS                (RAM_LOW_ADRS - 16)

/*
 * Structure written at RBV_ADRS by SMP sysToMonitor when rebooting
 * with multiple processors running, examined by romInit to detect
 * an SMP warm boot.  The assembly version is defined for all builds,
 */

#ifdef  _ASMLANGUAGE
#   define OFF_RBV_STARTTYPE    0
#   define OFF_RBV_MAGIC1       4
#   define OFF_RBV_MAGIC2       8
#   define OFF_RBV_MAGIC3       12
#else   /* _ASMLANGUAGE */
#   ifdef _CAG_CONFIG_SMP
struct rebootVec {
    int startType;
    VUINT32 magic1;
    VUINT32 magic2;
    VUINT32 magic3;
};
#   endif /* _CAG_CONFIG_SMP */
#endif  /* _ASMLANGUAGE */

#define CPU1CHECK  (sysHid1Get() & 0x1)

/* SOC PVR and SVR */

#define PPC_PVR_85XX            0x80200000
#define PPC_PVR_MASK            0xFFF00000

#define PPC_SVR_P2020           0x80E00000
#define PPC_SVR_MASK            0xFFF00000

/* Define Clock Speed and source  */

#define FREQ_33_MHZ             33333300
#define FREQ_66_MHZ             66666600
#define FREQ_50_MHZ             50000000
#define FREQ_100_MHZ            100000000
#define FREQ_400_MHZ            400000000
#define FREQ_500_MHZ            500000000

#define DEFAULT_SYS_CLK_FREQ    FREQ_100_MHZ
#define DDR_CLK_FREQ            FREQ_33_MHZ

#define DEFAULT_SYSCLKFREQ      FREQ_500_MHZ

#define PLAT_RATIO_800_MHZ      20
#define PLAT_RATIO_533_MHZ      16
#define PLAT_RATIO_400_MHZ      12
#define PLAT_RATIO_333_MHZ      8

/* Clock rates */

#define SYS_CLK_RATE_MIN        1       /* minimum system clock rate */
#define SYS_CLK_RATE_MAX        8000    /* maximum system clock rate */
#define FIT_CLK_RATE_MIN        (FREQ_400_MHZ / (1 << 24))
#define FIT_CLK_RATE_MAX        (FREQ_400_MHZ / (1 << 13))
#define AUX_CLK_RATE_MIN        1
#define AUX_CLK_RATE_MAX        50000
#define AUX_CLK_RATE            20000

/* Frequency applied to the  "RTC" pin */

#define HPCN_RTC_FREQ           14318000

/* possibly this is not supported check your DDR undefined for safety */

#undef  INCLUDE_DDR_ECC

/* 60x bus adrs to PCI (non-prefetchable) memory address */

#define LOCAL2PCI_MEMIO(x)      ((int)(x) + PCI_MSTR_MEM_BUS)

#define ETHERNET_MAC_HANDLER

#define WR_ENET0                0x00  /* WR specific portion of MAC (MSB->LSB) */
#define WR_ENET1                0xA0
#define WR_ENET2                0x1E

#define CUST_ENET3_0            0xA0  /* Customer portion of MAC address */
#define CUST_ENET3_1            0xA1
#define CUST_ENET3_2            0xA2
#define CUST_ENET3_3            0xA3
#define CUST_ENET4              0xAA
#define CUST_ENET5              0xA0

#define MAX_MAC_ADRS            3
#define MAC_ADRS_LEN            6
#define NV_MAC_ADRS_OFFSET      0x200

/* OpenPic timer vector definition */

#define OPENPIC_TIMERA0_INT_VEC 0x4C
#define OPENPIC_TIMERA1_INT_VEC 0x4D
#define OPENPIC_TIMERA2_INT_VEC 0x4E
#define OPENPIC_TIMERA3_INT_VEC 0x4F
#define OPENPIC_TIMERB0_INT_VEC 0x50
#define OPENPIC_TIMERB1_INT_VEC 0x51
#define OPENPIC_TIMERB2_INT_VEC 0x52
#define OPENPIC_TIMERB3_INT_VEC 0x53

/*
 * Local Access Windows Regster Offsets from CCSBAR
 *
 * LAWBARx
 * 0-11 Reserved - read 0
 * 12-31 Base address - Most significant 20 bits
 *
 * LAWARx
 * 0   Enable window
 * 1-7 Reserved
 * 8-11 Target interface - 0000 Reserved
 *                       - 0001 PCIEX2
 *                       - 0010 PCIEX1
 *                       - 0011 Reserved
 *                       - 0011 Reserved
 *                       - 0100 Local Bus memory controller eg SDRAM/L2SRAM
 *                       - 0101 -> 1110 Reserved
 *                       - 1111 DDR SDRAM
 * 12-25 Reserved - read 0
 * 26-31 Size of Window  - min 001011 -> 4KBytes
 *                         step power of 2
 *                       - max 011110 -> 2 Gbytes
 */

/* Used for Nor Flash */

#define  M85XX_LAWBAR0(base)    (CAST(VUINT32 *)((base) + 0xc08))
#define  M85XX_LAWAR0(base)     (CAST(VUINT32 *)((base) + 0xc10))

/* Used for PCIEX1_MEM, PCIEX1_MEM_IO, PCIEX1_IO  */

#define  M85XX_LAWBAR1(base)    (CAST(VUINT32 *)((base) + 0xc28))
#define  M85XX_LAWAR1(base)     (CAST(VUINT32 *)((base) + 0xc30))

/* Used for PCIEX2_MEM, PCIEX2_MEM_IO, PCIEX2_IO  */

#define  M85XX_LAWBAR2(base)    (CAST(VUINT32 *)((base) + 0xc48))
#define  M85XX_LAWAR2(base)     (CAST(VUINT32 *)((base) + 0xc50))

/* Unused */

#define  M85XX_LAWBAR3(base)    (CAST(VUINT32 *)((base) + 0xc68))
#define  M85XX_LAWAR3(base)     (CAST(VUINT32 *)((base) + 0xc70))

/* Unused */

#define  M85XX_LAWBAR4(base)    (CAST(VUINT32 *)((base) + 0xc88))
#define  M85XX_LAWAR4(base)     (CAST(VUINT32 *)((base) + 0xc90))

/* Used for VSC7385 interface */

#define  M85XX_LAWBAR5(base)    (CAST(VUINT32 *)((base) + 0xca8))
#define  M85XX_LAWAR5(base)     (CAST(VUINT32 *)((base) + 0xcb0))

/* Used for NAND Flash interface */

#define  M85XX_LAWBAR6(base)    (CAST(VUINT32 *)((base) + 0xcc8))
#define  M85XX_LAWAR6(base)     (CAST(VUINT32 *)((base) + 0xcd0))

/* Unused */

#define  M85XX_LAWBAR7(base)    (CAST(VUINT32 *)((base) + 0xce8))
#define  M85XX_LAWAR7(base)     (CAST(VUINT32 *)((base) + 0xcf0))

/* Unused */

#define  M85XX_LAWBAR8(base)    (CAST(VUINT32 *)((base) + 0xd08))
#define  M85XX_LAWAR8(base)     (CAST(VUINT32 *)((base) + 0xd10))

/* Unused */

#define  M85XX_LAWBAR9(base)    (CAST(VUINT32 *)((base) + 0xd28))
#define  M85XX_LAWAR9(base)     (CAST(VUINT32 *)((base) + 0xd30))


/* Unused */

#define  M85XX_LAWBAR10(base)   (CAST(VUINT32 *)((base) + 0xd48))
#define  M85XX_LAWAR10(base)    (CAST(VUINT32 *)((base) + 0xd50))

/* Used for DDR-II RAM */

#define  M85XX_LAWBAR11(base)   (CAST(VUINT32 *)((base) + 0xd68))
#define  M85XX_LAWAR11(base)    (CAST(VUINT32 *)((base) + 0xd70))

#define  LAWBAR_ADRS_SHIFT      12
#define  LAWAR_ENABLE           0x80000000
#define  LAWAR_TGTIF_PCIEX3     0x00000000
#define  LAWAR_TGTIF_PCIEX2     0x00100000
#define  LAWAR_TGTIF_PCIEX1     0x00200000
#define  LAWAR_TGTIF_LBC        0x00400000
#define  LAWAR_TGTIF_RAPIDIO    0x00C00000
#define  LAWAR_TGTIF_RAPIDIO2   0x00D00000
#define  LAWAR_TGTIF_DDRSDRAM   0x00F00000

/* LAWAR SIZE Settings */

#define  LAWAR_SIZE_4KB         0x0000000B
#define  LAWAR_SIZE_8KB         0x0000000C
#define  LAWAR_SIZE_16KB        0x0000000D
#define  LAWAR_SIZE_32KB        0x0000000E
#define  LAWAR_SIZE_64KB        0x0000000F
#define  LAWAR_SIZE_128KB       0x00000010
#define  LAWAR_SIZE_256KB       0x00000011
#define  LAWAR_SIZE_512KB       0x00000012
#define  LAWAR_SIZE_1MB         0x00000013
#define  LAWAR_SIZE_2MB         0x00000014
#define  LAWAR_SIZE_4MB         0x00000015
#define  LAWAR_SIZE_8MB         0x00000016
#define  LAWAR_SIZE_16MB        0x00000017
#define  LAWAR_SIZE_32MB        0x00000018
#define  LAWAR_SIZE_64MB        0x00000019
#define  LAWAR_SIZE_128MB       0x0000001A
#define  LAWAR_SIZE_256MB       0x0000001B
#define  LAWAR_SIZE_512MB       0x0000001C
#define  LAWAR_SIZE_1GB         0x0000001D
#define  LAWAR_SIZE_2GB         0x0000001E

/*
 * Local Bus Controller (LBC) Registers - unmentioned bits are reserved
 *
 * BRx 0-16 Base Address
 *     17-18 Extended Base Address
 *     19-20 Port Size - 00 reserved
 *                     - 01 8bit
 *                     - 10 16bit
 *                     - 11 32bit
 *     21-22 Data Error Correction
 *                     - 00 reserved
 *                     - 01 Normal parity
 *                     - 10 RMW parity generation (32-bit)
 *                     - 11 reserved
 *     23    Write Protect
 *     24-26 Machine Select = 000 GPCM
 *                          - 001->010 reserved
 *                          - 011 SDRAM
 *                          - 100->110 UPMA->UPMC
 *                          - 111 reserved
 *     28-29 Atomic Access  - 00 No atomic access
 *                          - 01 Read-after-write
 *                          - 10 Write-after-read
 *                          - 11 reserved
 *     31    Valid
 *
 * ORx for SDRAM
 *     0-16  Address mask
 *     17-18 Extended address mask
 *     19-21 Column address lines - 000->111 7->14
 *     23-25 Number of row address lines - 000->110 9->15
 *                                       - 111 Reserved
 *     26    Page mode select
 *     31    External address latch delay
 *
 * ORx for GPCM Mode
 *     0-16  Address mask
 *     17-18 Extended address mask
 *     19    Buffer Control Disable
 *     20    Chip select negation
 *     21-22 Address to chip select setup
 *     23    Extra Address to chip select setup
 *     24-27 Cycle length in Bus clocks - 0000->1111 0->15 wait states
 *     28    External address termination
 *     29    Timing relaxed
 *     30    Extended hold time for read access
 *     31    External address latch delay
 *
 */

#define  M85XX_BR0(base)        (CAST(VUINT32 *)((base) + 0x5000))
#define  M85XX_OR0(base)        (CAST(VUINT32 *)((base) + 0x5004))
#define  M85XX_BR1(base)        (CAST(VUINT32 *)((base) + 0x5008))
#define  M85XX_OR1(base)        (CAST(VUINT32 *)((base) + 0x500c))
#define  M85XX_BR2(base)        (CAST(VUINT32 *)((base) + 0x5010))
#define  M85XX_OR2(base)        (CAST(VUINT32 *)((base) + 0x5014))
#define  M85XX_BR3(base)        (CAST(VUINT32 *)((base) + 0x5018))
#define  M85XX_OR3(base)        (CAST(VUINT32 *)((base) + 0x501c))
#define  M85XX_BR4(base)        (CAST(VUINT32 *)((base) + 0x5020))
#define  M85XX_OR4(base)        (CAST(VUINT32 *)((base) + 0x5024))
#define  M85XX_BR5(base)        (CAST(VUINT32 *)((base) + 0x5028))
#define  M85XX_OR5(base)        (CAST(VUINT32 *)((base) + 0x502C))
#define  M85XX_BR6(base)        (CAST(VUINT32 *)((base) + 0x5030))
#define  M85XX_OR6(base)        (CAST(VUINT32 *)((base) + 0x5034))
#define  M85XX_BR7(base)        (CAST(VUINT32 *)((base) + 0x5038))
#define  M85XX_OR7(base)        (CAST(VUINT32 *)((base) + 0x503C))
#define  M85XX_MAR(base)        (CAST(VUINT32 *)((base) + 0x5068))
#define  M85XX_MAMR(base)       (CAST(VUINT32 *)((base) + 0x5070))
#define  M85XX_MBMR(base)       (CAST(VUINT32 *)((base) + 0x5074))
#define  M85XX_MCMR(base)       (CAST(VUINT32 *)((base) + 0x5078))
#define  M85XX_MRTPR(base)      (CAST(VUINT32 *)((base) + 0x5084))
#define  M85XX_MDR(base)        (CAST(VUINT32 *)((base) + 0x5088))
#define  M85XX_LSDMR(base)      (CAST(VUINT32 *)((base) + 0x5094))
#define  M85XX_LBCR(base)       (CAST(VUINT32 *)((base) + 0x50D0))
#define  M85XX_LCRR(base)       (CAST(VUINT32 *)((base) + 0x50D4))

#define  MRTPR_PTP_MASK         0xff000000
#define  MRTPR_PTP_WRITE(x)     ((x << 24) & MRTPR_PTP_MASK)
#define  LSDMR_RFEN             0x40000000 /* Refresh Enable */

/*
 * Shift 36bit physical address to 24 bit ie lowest 4k predefined as 0xffc
 * We only every use 32 bit for this so shift by 8 not 12
 */

#define BPTR_BOOT_PAGE_SHIFT    12

/*
 * LSDMR OP - 000 Normal operation
 *          - 001 Auto Refresh  (Initialization)
 *          - 010 Self Refresh
 *          - 011 Mode Register Write (Initialization)
 *          - 100 Precharge Bank
 *          - 101 Precharge all banks (Initialization)
 *          - 110 Activate Bank
 *          - 111 Read/Write without valid transfer
 */

#define  LSDMR_OP_MASK          0x38000000
#define  LSDMR_OP_SHIFT(x)      ((x << 27) & LSDMR_OP_MASK)

/*
 * Bank Select Multiplexed address line - 000 lines 12:13
 *                                      - 001       13:14
 *                                      - 010       14:15
 *                                      - 011       15:16
 *                                      - 100       16:17
 *                                      - 101       17:18
 *                                      - 110       18:19
 *                                      - 111       19:20
 */

#define  LSDMR_BSMA_MASK        0x00E00000
#define  LSDMR_BSMA_SHIFT(x)    ((x << 23) & LSDMR_BSMA_MASK)

/*
 * RFCR Refresh recovery 000 - reserved
 *                       001->110 - 3->8 clocks
 *                       111 - 16 clocks
 */

#define  LSDMR_RFCR_MASK        0x00038000
#define  LSDMR_RFCR_SHIFT(x)    ((x << 15) & LSDMR_RFCR_MASK)

#define  M85XX_UART_THR0(base)   (CAST(VUINT32 *)((base) + 0x4500))

/* Incomplete LSDMR definitions */

#define  M85XX_LURT(base)       (CAST(VUINT32 *)((base) + 0x50A0))
#define  M85XX_LSRT(base)       (CAST(VUINT32 *)((base) + 0x50A4))
#define  M85XX_LTESR(base)      (CAST(VUINT32 *)((base) + 0x50B0))
#define  M85XX_LTEDR(base)      (CAST(VUINT32 *)((base) + 0x50B4))
#define  M85XX_LTEIR(base)      (CAST(VUINT32 *)((base) + 0x50B8))
#define  M85XX_LTEATR(base)     (CAST(VUINT32 *)((base) + 0x50BC))
#define  M85XX_LTEAR(base)      (CAST(VUINT32 *)((base) + 0x50C0))

#define  M85XX_DCR0(base)       (CAST(VUINT32 *)((base) + 0xe0f1c))
#define  M85XX_DCR1(base)       (CAST(VUINT32 *)((base) + 0xe0f20))

#define CPU1CHECK               (sysHid1Get() & 0x1)

/* ECM Registers */

#define ECM_OFFSET              0x1000
#define ECMBA                   (_CAG_CONFIG_CCSBAR + ECM_OFFSET)

#define EEB_ABCR(base)          (CAST(VUINT32 *)((base) + ECM_OFFSET + 0x0))
#define EEB_DBCR(base)          (CAST(VUINT32 *)((base) + ECM_OFFSET + 0x8))
#define EEB_PCR(base)           (CAST(VUINT32 *)((base) + ECM_OFFSET + 0x10))
#define EEB_REV1(base)          (CAST(VUINT32 *)((base) + ECM_OFFSET + 0xBF8))
#define EEB_REV2(base)          (CAST(VUINT32 *)((base) + ECM_OFFSET + 0xBFC))
#define EEB_EDR(base)           (CAST(VUINT32 *)((base) + ECM_OFFSET + 0xE00))
#define EEB_EER(base)           (CAST(VUINT32 *)((base) + ECM_OFFSET + 0xE08))
#define EEB_EATR(base)          (CAST(VUINT32 *)((base) + ECM_OFFSET + 0xE0C))
#define EEB_ELADR(base)         (CAST(VUINT32 *)((base) + ECM_OFFSET + 0xE10))
#define EEB_EHADR(base)         (CAST(VUINT32 *)((base) + ECM_OFFSET + 0xE14))

#define EEB_PCR_CORE1_ENABLE    0x02000000
#define EEB_PCR_CORE0_ENABLE    0x01000000

/* Offsets for DDR registers */

#define DDR_OFFSET              0x2000
#define DDRBA                   (_CAG_CONFIG_CCSBAR | DDR_OFFSET)

#define CS0_BNDS                0x000
#define CS1_BNDS                0x008
#define CS2_BNDS                0x010
#define CS3_BNDS                0x018
#define CS0_CONFIG              0x080
#define CS1_CONFIG              0x084
#define CS2_CONFIG              0x088
#define CS3_CONFIG              0x08C
#define CS0_CONFIG_2            0x0c0
#define CS1_CONFIG_2            0x0c4
#define CS2_CONFIG_2            0x0c8
#define CS3_CONFIG_2            0x0CC
#define TIMING_CFG_3            0x100
#define TIMING_CFG_0            0x104
#define TIMING_CFG_1            0x108
#define TIMING_CFG_2            0x10C
#define DDR_SDRAM_CFG           0x110
#define DDR_SDRAM_CFG_2         0x114
#define DDR_SDRAM_MODE_CFG      0x118
#define DDR_SDRAM_MODE_CFG_2    0x11c
#define DDR_SDRAM_MD_CNTL       0x120
#define DDR_SDRAM_INTERVAL      0x124
#define DDR_DATA_INIT           0x128
#define DDR_SDRAM_CLK_CTRL      0x130
#define DDR_INIT_ADDR           0x148
#define DDR_INIT_EXT_ADDRESS    0x14C
#define TIMING_CFG_4            0x160
#define TIMING_CFG_5            0x164
#define DDR_DDR_ZQ_CNTL         0x170
#define DDR_DDR_WRLVL_CNTL      0x174
#define DDR_DDRDSR_1            0xB20
#define DDR_DDRDSR_2            0xB24
#define DDR_DDRCDR_1            0xB28
#define DDR_DDRCDR_2            0xB2C
#define DDR_DDR_IP_REV1         0xBF8
#define DDR_DDR_IP_REV2         0xBFC
#define DATA_ERR_INJECT_HI      0xE00
#define DATA_ERR_INJECT_LO      0xE04
#define ECC_ERR_INJECT          0xE08
#define CAPTURE_DATA_HI         0xE20
#define CAPTURE_DATA_LO         0xE24
#define CAPTURE_ECC             0xE28
#define ERR_DETECT              0xE40
#define ERR_DISABLE             0xE44
#define ERR_INT_EN              0xE48
#define CAPTURE_ATTRIBUTES      0xE4c
#define CAPTURE_ADDRESS         0xE50
#define ERR_SBE                 0xE58

#define DDR_IO_OVCR             0x000E0F24

/* DUART */

#define UART0_BASE                  (_CAG_CONFIG_CCSBAR + 0x4500)
#define UART1_BASE                  (_CAG_CONFIG_CCSBAR + 0x4600)
#define DUART_REG_ADDR_INTERVAL     1

/* VSC7385 */

#define VSC7385_BASE                0xf1000000
#define VSC7385_SIZE                0x00020000

/* NAND Flash */

#define NAND_BUFFER_BASE            0xf0000000
#define NAND_BUFFER_SIZE            0x00080000
#define NAND_PAGE_SIZE              512
#define NAND_BLOCK_SIZE             (16 * 1024)
#define NAND_PAGE_PER_BLOCK         32
#define NAND_BANK                   1

/* PCIEX */

#define PCIEX1_REGBASE              (_CAG_CONFIG_CCSBAR + 0xa000)   /* Mini PCI-EX slot */
#define PCIEX2_REGBASE              (_CAG_CONFIG_CCSBAR + 0x9000)   /* PCI-EX slot */

/* PCIEX interrupt level */

#define PCIEX2_XINT1_LVL            4
#define PCIEX2_XINT2_LVL            5
#define PCIEX2_XINT3_LVL            6
#define PCIEX2_XINT4_LVL            7

/* PCIEX slots number */

#define NUM_PCIEX2_SLOTS            0x1

#define PCI_LAT_TIMER               0x40  /* latency timer value, 64 PCI clocks */

/* PCIEX based addresses */

#define PCIEX2_LAW_BASE             0x90000000          /* PCI LAW window */
#define PCIEX2_LAW_SIZE             0x10000000          /* PCI LAW size   */
#define PCIEX2_MEM_SIZE             0x04000000          /* 64 MB */
#define PCIEX2_MEMIO_SIZE           0x04000000          /* 64 MB */
#define PCIEX2_IO_SIZE              0x04000000          /* 64 MB */
#define PCIEX2_MEM_ADRS             (PCIEX2_LAW_BASE)
#define PCIEX2_MEMIO_ADRS           (PCIEX2_MEM_ADRS   + PCIEX2_MEM_SIZE)
#define PCIEX2_IO_ADRS              (PCIEX2_MEMIO_ADRS + PCIEX2_MEMIO_SIZE)

#define PCI_MMU_TLB_SZ              _MMU_TLB_SZ_256M

/* CPU address */

#define PCI_MSTR_MEM_BUS            LOCAL_MEM_LOCAL_ADRS
#define PCI_BRIDGE_PIMMR_BASE_ADRS  0xc0000000

/* PCI Express Host Controller */

/* ATMU register defines */

#define PCI_WINDOW_ENABLE_BIT       0x80000000
#define PCI_SNOOP_ENABLE            0x40000000
#define PCI_PREFETCHABLE            0x20000000

/* outbound window attributes */

#define PCI_OUT_ATTR_RTT_MEM        0x00040000
#define PCI_OUT_ATTR_RTT_IO         0x00080000
#define PCI_OUT_ATTR_WTT_MEM        0x00004000
#define PCI_OUT_ATTR_WTT_IO         0x00008000

#define PCI_ATTR_WS_4K              0x0000000B
#define PCI_ATTR_WS_8K              0x0000000C
#define PCI_ATTR_WS_16K             0x0000000D
#define PCI_ATTR_WS_32K             0x0000000E
#define PCI_ATTR_WS_64K             0x0000000F
#define PCI_ATTR_WS_128K            0x00000010
#define PCI_ATTR_WS_256K            0x00000011
#define PCI_ATTR_WS_512K            0x00000012
#define PCI_ATTR_WS_1M              0x00000013
#define PCI_ATTR_WS_2M              0x00000014
#define PCI_ATTR_WS_4M              0x00000015
#define PCI_ATTR_WS_8M              0x00000016
#define PCI_ATTR_WS_16M             0x00000017
#define PCI_ATTR_WS_32M             0x00000018
#define PCI_ATTR_WS_64M             0x00000019
#define PCI_ATTR_WS_128M            0x0000001A
#define PCI_ATTR_WS_256M            0x0000001B
#define PCI_ATTR_WS_512M            0x0000001C
#define PCI_ATTR_WS_1G              0x0000001D
#define PCI_ATTR_WS_2G              0x0000001E
#define PCI_ATTR_WS_4G              0x0000001F
#define PCI_ATTR_WS_8G              0x00000020
#define PCI_ATTR_WS_16G             0x00000021
#define PCI_ATTR_WS_32G             0x00000022
#define PCI_ATTR_WS_64G             0x00000023

/* inbound window attributes */

#define PCI_IN_ATTR_TGI_LM                                  0x00f00000
#define PCI_IN_ATTR_RTT_LM_READ_NO_SNOOP                    0x00040000
#define PCI_IN_ATTR_RTT_LM_READ_SNOOP                       0x00050000
#define PCI_IN_ATTR_RTT_LM_READ_UNLOCK_L2_CACHE_LINE        0x00070000
#define PCI_IN_ATTR_RTT_LM_WRITE_NO_SNOOP                   0x00004000
#define PCI_IN_ATTR_RTT_LM_WRITE_SNOOP                      0x00005000
#define PCI_IN_ATTR_RTT_LM_WRITE_ALLOC_L2_CACHE_LINE        0x00006000
#define PCI_IN_ATTR_RTT_LM_WRITE_ALLOC_LOCK_L2_CACHE_LINE   0x00007000

/* USB */

#define USB1_BASE                   (_CAG_CONFIG_CCSBAR + 0x22000)
#define USB2_BASE                   (_CAG_CONFIG_CCSBAR + 0x23000)

/* EHCI registers */

#define EHCI_CAPLENGTH(base)        ((base) + 0x00100)

#define EHCI_BURSTSIZE(base)        ((base) + 0x00160)
#define EHCI_TXTTFILLTUNING_L(base) ((base) + 0x00164)
#define EHCI_TXTTFILLTUNING_H(base) ((base) + 0x00168)
#define EHCI_CONFIGFLAG(base)       ((base) + 0x00180)
#define EHCI_PORTSC(base)           ((base) + 0x00184)
#define EHCI_USBMODE(base)          ((base) + 0x001a8)

#define EHCI_SNOOP1(base)           ((base) + 0x00400)
#define EHCI_SNOOP2(base)           ((base) + 0x00404)
#define EHCI_AGE_CNT_THRESH(base)   ((base) + 0x00408)
#define EHCI_PRI_CTRL(base)         ((base) + 0x0040c)
#define EHCI_SI_CTRL(base)          ((base) + 0x00410)
#define EHCI_CONTROL(base)          ((base) + 0x00500)

#define EHCI_USBMODE_IDLE           0x00000000
#define EHCI_USBMODE_DEVICE         0x00000002
#define EHCI_USBMODE_HOST           0x00000003

#define EHCI_PORTSC_ULPI            0x80000000

#define EHCI_SNOOP_SIZE_4K          0x0000000B
#define EHCI_SNOOP_SIZE_8K          0x0000000C
#define EHCI_SNOOP_SIZE_16K         0x0000000D
#define EHCI_SNOOP_SIZE_32K         0x0000000E
#define EHCI_SNOOP_SIZE_64K         0x0000000F
#define EHCI_SNOOP_SIZE_128K        0x00000010
#define EHCI_SNOOP_SIZE_256K        0x00000011
#define EHCI_SNOOP_SIZE_512K        0x00000012
#define EHCI_SNOOP_SIZE_1M          0x00000013
#define EHCI_SNOOP_SIZE_2M          0x00000014
#define EHCI_SNOOP_SIZE_4M          0x00000015
#define EHCI_SNOOP_SIZE_8M          0x00000016
#define EHCI_SNOOP_SIZE_16M         0x00000017
#define EHCI_SNOOP_SIZE_32M         0x00000018
#define EHCI_SNOOP_SIZE_64M         0x00000019
#define EHCI_SNOOP_SIZE_128M        0x0000001A
#define EHCI_SNOOP_SIZE_256M        0x0000001B
#define EHCI_SNOOP_SIZE_512M        0x0000001C
#define EHCI_SNOOP_SIZE_1G          0x0000001D
#define EHCI_SNOOP_SIZE_2G          0x0000001E

#ifdef INCLUDE_AMP
#   define EHCI_SNOOP_SIZE          EHCI_SNOOP_SIZE_256M
#else /* INCLUDE_AMP */
#   define EHCI_SNOOP_SIZE          EHCI_SNOOP_SIZE_512M
#endif /* INCLUDE_AMP */

#define EHCI_CONTROL_USB_EN         0x00000004

/* PIC Base Address */

#define PIC_OFFSET                  0x40000
#define PCIBA                       (_CAG_CONFIG_CCSBAR | PIC_OFFSET)

/* DMA registers base address and offsets */

#define DMA1_REGBASE                (_CAG_CONFIG_CCSBAR + 0x21000)  //0x21100->0x21000

#define DMA_CHAN_OFFSET             0x80
#define DMA_CHAN_NUM                4

/* Global Function Registers */

/* PORPLL used to detect clocking ratio for CCB/CPM for serial devices */
/* Plat Ratio not working on board need to test!!!!*/


#define M85XX_PORPLLSR(base)    (CAST(VUINT32 *)((base) + 0xE0000))

#define M85XX_PORPLLSR_E500_1_RATIO_MASK    0x3f000000
#define M85XX_PORPLLSR_E500_RATIO_MASK      0x003f0000
#define M85XX_PORPLLSR_DDR_RATIO_MASK       0x00003e00
#define M85XX_PORPLLSR_PLAT_RATIO_MASK      0x0000003e
#define M85XX_PORPLLSR_E500_RATIO(base)     ((*M85XX_PORPLLSR(base) & M85XX_PORPLLSR_E500_RATIO_MASK) >> 16)
#define M85XX_PORPLLSR_E500_1_RATIO(base)   ((*M85XX_PORPLLSR(base) & M85XX_PORPLLSR_E500_1_RATIO_MASK) >> 24)
#define M85XX_PORPLLSR_DDR_RATIO(base)      ((*M85XX_PORPLLSR(base) & M85XX_PORPLLSR_DDR_RATIO_MASK) >> 9)
#define M85XX_PORPLLSR_PLAT_RATIO(base)     ((*M85XX_PORPLLSR(base) & M85XX_PORPLLSR_PLAT_RATIO_MASK) >> 1)

#define M85XX_DDRDLLCR(base)            (CAST(VUINT32 *)((base) + 0xE0E10))
#define M85XX_LBCDLLSR(base)            (CAST(VUINT32 *)((base) + 0xE0E20))

#define M85XX_DEVDISR(base)             (CAST(VUINT32 *)((base) + 0xE0070))
#define M85XX_DEVDISR_PCIE1             0x20000000
#define M85XX_DEVDISR_LBC               0x08000000
#define M85XX_DEVDISR_PCIE2             0x04000000
#define M85XX_DEVDISR_PCIE3             0x02000000
#define M85XX_DEVDISR_SEC               0x01000000
#define M85XX_DEVDISR_PME               0x00800000
#define M85XX_DEVDISR_TLU1              0x00400000
#define M85XX_DEVDISR_TLU2              0x00200000
#define M85XX_DEVDISR_SRIO              0x00080000
#define M85XX_DEVDISR_D2                0x00020000
#define M85XX_DEVDISR_D1                0x00010000
#define M85XX_DEVDISR_TB0               0x00004000
#define M85XX_DEVDISR_E500_CORE0        0x00008000
#define M85XX_DEVDISR_E500_CORE1        0x00002000
#define M85XX_DEVDISR_TB1               0x00001000
#define M85XX_DEVDISR_DMA1              0x00000400
#define M85XX_DEVDISR_DMA2              0x00000200
#define M85XX_DEVDISR_TSEC1             0x00000080
#define M85XX_DEVDISR_TSEC2             0x00000040
#define M85XX_DEVDISR_TSEC3             0x00000020
#define M85XX_DEVDISR_TSEC4             0x00000010
#define M85XX_DEVDISR_FEC               0x00000008
#define M85XX_DEVDISR_I2C               0x00000004
#define M85XX_DEVDISR_DUART             0x00000002

#define M85XX_POWMGTCSR(base)           (CAST(VUINT32 *)((base) + 0xE0080))
#define M85XX_POWMGTCSR_CORE0_IRQ_MSK   0x80000000
#define M85XX_POWMGTCSR_CORE0_CI_MSK    0x40000000
#define M85XX_POWMGTCSR_CORE1_IRQ_MSK   0x20000000
#define M85XX_POWMGTCSR_CORE1_CI_MSK    0x10000000
#define M85XX_POWMGTCSR_CORE0_DOZ       0x00080000
#define M85XX_POWMGTCSR_CORE1_DOZ       0x00040000
#define M85XX_POWMGTCSR_SLP             0x00020000
#define M85XX_POWMGTCSR_CORE1_DOZING    0x00000020
#define M85XX_POWMGTCSR_CORE1_NAPPING   0x00000010
#define M85XX_POWMGTCSR_CORE0_DOZING    0x00000008
#define M85XX_POWMGTCSR_CORE0_NAPPING   0x00000004
#define M85XX_POWMGTCSR_SLPING          0x00000002

/* Processor initialization register */

#define M85XX_EPICPIR(base)             (CAST(VUINT32 *)((base) + 0x41090))

/* Global Utilities Block Register */

#define M85XX_AUTORSTSR(base)           (CAST(VUINT32 *)((base) + 0xe009c))
#define READY_P1                        (0x01 << 14)

#define M85XX_EPICPIR_HRST_CORE0        0x00000001
#define M85XX_EPICPIR_HRST_CORE1        0x00000002

#define M85XX_PVR(base)                 (CAST(VUINT32 *)((base) + 0xE00A0))
#define M85XX_SVR(base)                 (CAST(VUINT32 *)((base) + 0xE00A4))

#define M85XX_RSTCR(base)               (CAST(VUINT32 *)((base) + 0xE00B0))

#define _PPC_BUCSR_FI                   0x200   /* Invalidate branch cache */
#define _PPC_BUCSR_E                    0x1     /* Enable branch prediction */

/* EPIC defines */

#define EPIC_EX_IRQ_NUM                 12
#define EPIC_IN_IRQ_NUM                 64
#define EPIC_GT_IRQ_NUM                 4
#define EPIC_MSG_IRQ_NUM                8
#define EPIC_SMSG_IRQ_NUM               8
#define EPIC_IPI_IRQ_NUM                4
#define EPIC_EX_REG_BASE                0x50000
#define EPIC_IN_REG_BASE                0x50200
#define EPIC_GT_A_REG_BASE              0x41120
#define EPIC_GT_B_REG_BASE              0x42120
#define EPIC_MSG_REG_BASE               0x51600
#define EPIC_SMSG_REG_BASE              0x51c00
#define EPIC_IPI_REG_BASE               0x410a0
#define EPIC_EX_REG_INTERVAL            0x20
#define EPIC_IN_REG_INTERVAL            0x20
#define EPIC_GT_REG_INTERVAL            0x40
#define EPIC_MSG_REG_INTERVAL           0x20
#define EPIC_SMSG_REG_INTERVAL          0x20
#define EPIC_IPI_REG_INTERVAL           0x10

/* interrupt vectors */

#define EPIC_DMA1_CHAN0_INT_NUM         4
#define EPIC_DMA1_CHAN0_INT_VEC         (EPIC_DMA1_CHAN0_INT_NUM +  \
                                         EPIC_VEC_IN_IRQ0)
#define EPIC_DMA1_CHAN1_INT_NUM         5
#define EPIC_DMA1_CHAN1_INT_VEC         (EPIC_DMA1_CHAN1_INT_NUM +  \
                                         EPIC_VEC_IN_IRQ0)
#define EPIC_DMA1_CHAN2_INT_NUM         6
#define EPIC_DMA1_CHAN2_INT_VEC         (EPIC_DMA1_CHAN2_INT_NUM +  \
                                         EPIC_VEC_IN_IRQ0)
#define EPIC_DMA1_CHAN3_INT_NUM         7
#define EPIC_DMA1_CHAN3_INT_VEC         (EPIC_DMA1_CHAN3_INT_NUM +  \
                                         EPIC_VEC_IN_IRQ0)
#define EPIC_USB1_INT_NUM               12
#define EPIC_USB1_INT_VEC               (EPIC_USB1_INT_NUM + \
                                        EPIC_VEC_IN_IRQ0)
#define EPIC_I2C_INT_NUM                27
#define EPIC_I2C_INT_VEC                (EPIC_I2C_INT_NUM +  \
                                         EPIC_VEC_IN_IRQ0)
#define EPIC_USB2_INT_NUM               30
#define EPIC_USB2_INT_VEC               (EPIC_USB2_INT_NUM + \
                                         EPIC_VEC_IN_IRQ0)
#define EPIC_SPI_INT_NUM                43
#define EPIC_SPI_INT_VEC                (EPIC_SPI_INT_NUM + \
                                         EPIC_VEC_IN_IRQ0)
#define EPIC_SDHC_INT_NUM               56
#define EPIC_SDHC_INT_VEC               (EPIC_SDHC_INT_NUM + \
                                         EPIC_VEC_IN_IRQ0)

/* I2C RTC */

#define I2C_BUS1_BASE                   (_CAG_CONFIG_CCSBAR + 0x3000)
#define I2C_BUS2_BASE                   (_CAG_CONFIG_CCSBAR + 0x3100)

/* ETSEC */

#define ETSEC0_GROUP0_BASE              (_CAG_CONFIG_CCSBAR + 0xB0000)
#define ETSEC1_GROUP0_BASE              (_CAG_CONFIG_CCSBAR + 0xB1000)
#define ETSEC2_GROUP0_BASE              (_CAG_CONFIG_CCSBAR + 0xB2000)

#define ETSEC0_MDIO_BASE                (_CAG_CONFIG_CCSBAR + 0x24000)
#define ETSEC1_MDIO_BASE                (_CAG_CONFIG_CCSBAR + 0x25000)

/* eSDHC */

#define SDHC_BASE                       (_CAG_CONFIG_CCSBAR + 0x2E000)

#define SD_DATA                         0x80000000
#define SDHC_CD                         0x40000000
#define SDHC_WP                         0x20000000

/* VSC7385 */

#define VSC7385_RESET                   (void *)(VSC7385_BASE + 0x1c050)
#define VSC7385_CTRL                    (void *)(VSC7385_BASE + 0x1c040)
#define VSC7385_ADDR                    (void *)(VSC7385_BASE + 0x1c044)
#define VSC7385_DATA                    (void *)(VSC7385_BASE + 0x1c048)
#define VSC7385_REG_MAP                 (void *)(VSC7385_BASE + 0x1c070)

/* GPIO Registers */

#define GPIO_BASE                       (_CAG_CONFIG_CCSBAR + 0xF000)

#define GPDIR                           (GPIO_BASE + 0xC00)
#define GPODR                           (GPIO_BASE + 0xC04)
#define GPDAT                           (GPIO_BASE + 0xC08)
#define GPIER                           (GPIO_BASE + 0xC0C)
#define GPIMR                           (GPIO_BASE + 0xC10)
#define GPICR                           (GPIO_BASE + 0xC14)

#define USB_PORT_EN                     0x01000000

/* SPI defines */

#define SPI_REG_BASE                    (_CAG_CONFIG_CCSBAR + 0x7000)
#define ESPI_SPMODE                     (SPI_REG_BASE + 0)
#define ESPI_SPIE                       (SPI_REG_BASE + 0x4)
#define ESPI_SPIM                       (SPI_REG_BASE + 0x8)
#define ESPI_SPCOM                      (SPI_REG_BASE + 0xc)
#define ESPI_SPITF                      (SPI_REG_BASE + 0x10)
#define ESPI_SPIRF                      (SPI_REG_BASE + 0x14)
#define ESPI_SPMODE0                    (SPI_REG_BASE + 0x20)
#define ESPI_SPMODE1                    (SPI_REG_BASE + 0x24)
#define ESPI_SPMODE2                    (SPI_REG_BASE + 0x28)
#define ESPI_SPMODE3                    (SPI_REG_BASE + 0x2c)

#define SPI_CS_CFG_INIT                 0x24171108  //ezio 10.20

#define ESPI_COM_CS(n)                  (n<<30)
#define ESPI_COM_DO                     (1<<28)
#define ESPI_COM_TO                     (1<<27)
#define ESPI_COM_HLD                    (1<<26)
#define ESPI_COM_RXSKIP(n)              (n<<16)
#define ESPI_COM_TRANLEN(n)             (n)

#define ESPI_MODE_CI                        (1<<31)
#define ESPI_MODE_CP                        (1<<30)
#define ESPI_MODE_REV                       (1<<29)
#define ESPI_MODE_DIV                       (1<<28)
#define ESPI_MODE_PM(n)                     (n<<25)
#define ESPI_MODE_ODD                       (1<<23)
#define ESPI_MODE_POL                       (1<<20)
#define ESPI_MODE_LEN(n)                    (n<<16)
#define ESPI_MODE_CSBEF(n)                  (n<<12)
#define ESPI_MODE_CSAFT(n)                  (n<<8)
#define ESPI_MODE_CSCG(n)                   (n<<3)



/* Alternate Function Signal Multiplex Control Register */

#define M85XX_PMUXCR                    *((VUINT32 *)(_CAG_CONFIG_CCSBAR + 0xE0060))

/* flash information */

#define FLASH_ADRS                      _CAG_CONFIG_FLASH_BASE_ADRS
#define FLASH_SIZE                      _CAG_CONFIG_FLASH_SIZE
#define FLASH_WIDTH                     2
#define FLASH_CHIP_WIDTH                2
#define FLASH_SECTOR_SIZE               _CAG_CONFIG_FLASH_SECTOR_SIZE
#define FLASH_SECTOR_MASK               0xfffe0000

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCp1020rdbh */
