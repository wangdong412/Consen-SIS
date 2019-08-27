/* ppc85xx.h - PowerPC specific header */

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

#ifndef __INCppc85xxh
#define __INCppc85xxh

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum cache line size for the cpu */
#define	_CPU_CACHE_ALIGN_SHIFT	5	/* cache align size = 32 */

#undef	_CPU_CACHE_ALIGN_SIZE

#define	_CPU_CACHE_ALIGN_SIZE	32	/* cannot use (1<<5) because compiler
					 * directive __attribute__ cant hdl */

/* MMU ASID context register name for _CAG_MMU_CLASS_PID_REG */
#define	_PPC_PID_REG		PID0

/* upper Machine State Register (MSR) mask */

#define _PPC_MSR_CM_U		0x8000	/* sixty-four bit mode (not
					 * implemented for 32-bit machine) */
#define _PPC_MSR_POW_U		0x0004	/* power managemnet enable */
#define _PPC_MSR_ILE_U		0x0001	/* little endian mode */

/* lower Machine State Register (MSR) mask */

#define	_PPC_MSR_EE		0x8000	/* external interrupt enable */
#define _PPC_MSR_PR		0x4000	/* privilege level */
#define _PPC_MSR_FP		0x2000	/* floating-point available */
#define _PPC_MSR_ME		0x1000	/* machine check enable  */
#define _PPC_MSR_FE0		0x0800	/* floating-point exception mode 0 */
#define _PPC_MSR_SE		0x0400	/* single-step trace enable */
#define _PPC_MSR_BE		0x0200	/* branch trace enable */
#define _PPC_MSR_FE1		0x0100	/* floating-point exception mode 1 */
#define _PPC_MSR_IP		0x0040	/* exception prefix */
#define _PPC_MSR_IR		0x0020	/* instruction address translation */
#define _PPC_MSR_DR		0x0010	/* data address translation */
#define _PPC_MSR_RI		0x0002	/* recoverable interrupt */
#define _PPC_MSR_LE		0x0001	/* little-endian mode */

#define _PPC_MSR_POWERUP	0x0040	/* state of MSR at powerup */

/* MSR bit definitions common to all PPC arch. */

#define _PPC_MSR_BIT_EE		16	/* MSR Ext. Intr. Enable bit - EE */
#define _PPC_MSR_BIT_PR		17	/* MSR Privilege Level bit - PR */
#define _PPC_MSR_BIT_ME		19	/* MSR Machine Check Enable bit - ME */
#define _PPC_MSR_BIT_LE		31	/* MSR Little Endian mode bit - LE */

/* archPpc.h */

#ifdef _CAG_SPE_SUPPORT
#undef _PPC_MSR_FP
#define _PPC_MSR_SPE_U		0x0200
#define _PPC_MSR_SPE		0x02000000
#define MSR_SPE_BIT_SHIFT	25
#define _PPC_MSR_BIT_SPE    6
#endif /* _CAG_SPE_SUPPORT */

/* #define IMMR	638	* bogus hack */

/* Special Purpose Register (SPR) numbers */
#define CSRR0   58      /* Critical SRR0 */
#define CSRR1   59      /* Critical SRR1 */
#define DEAR    61      /* Data Exception Address Register */
#define ESR     62      /* Exception Syndrome Register */
#define IVPR    63      /* Interrupt Vector Prefix Register */
#define IVOR0   400     /* IVOR Critical Input */
#define IVOR1   401     /* IVOR Machine Check */
#define IVOR2   402     /* IVOR Data Storage */
#define IVOR3   403     /* IVOR Instruction Storage */
#define IVOR4   404     /* IVOR External Input */
#define IVOR5   405     /* IVOR Alignment */
#define IVOR6   406     /* IVOR Program */
#define IVOR7   407     /* IVOR Floating Point Unavailable */
#define IVOR8   408     /* IVOR System Call */
#define IVOR9   409     /* IVOR Auxiliary Processor Unavailable */
#define IVOR10  410     /* IVOR Decrementer */
#define IVOR11  411     /* IVOR Fixed Interval Timer */
#define IVOR12  412     /* IVOR Watchdog Timer */
#define IVOR13  413     /* IVOR Data TLB Error */
#define IVOR14  414     /* IVOR Instruction TLB Error */
#define IVOR15  415     /* IVOR Debug */
#ifdef _CAG_SPE_SUPPORT
#define IVOR32  528     /* IVOR SPE */
#define IVOR33  529     /* IVOR Vector FP Data */
#define IVOR34  530     /* IVOR Vector FP Round */
#endif	/* _CAG_SPE_SUPPORT */
#define IVOR35  531     /* IVOR Performance Monitor */
#define MCSRR0  570     /* Machine Check SRR0 */
#define MCSRR1  571     /* Machine Check SRR1 */
#define MCAR    573     /* Machine Check Address Register */

#define MCSR    572     /* Machine Check Syndrome Register */

/* PVR bit definitions */

#define _PPC_PVR_ID(pvr)        (((pvr) >>  16) & 0xFFFF)   /* version */
#define _PPC_PVR_MAJ(pvr)       (((pvr) >>  4) & 0xF)       /* Major revision */
#define _PPC_PVR_MIN(pvr)       ((pvr) & 0xF)               /* Minor revision */

#define _PPC_PVR_VER_E500V2     0x8021
#define _PPC_PVR_VER_E500MC     0x8023
#define _PPC_PVR_VER_E5500      0x8024
#define _PPC_PVR_VER_E6500      0x8040

/* MCSR bit definitions */
#define	_PPC_MCSR_BIT_MCP	0	/* Machine check input pin */
#define	_PPC_MCSR_BIT_ICPERR	1	/* Instruction cache parity error */
#define	_PPC_MCSR_BIT_DCP_PERR	2	/* Data cache push parity error */
#define	_PPC_MCSR_BIT_DCPERR	3	/* Data cache parity error */
#define	_PPC_MCSR_BIT_GL_CI	15	/* guarded load etc, see manual. using it for clear MCSR */
#define	_PPC_MCSR_BIT_BUS_IAERR	24	/* Bus instruction address error */
#define	_PPC_MCSR_BIT_BUS_RAERR	25	/* Bus read address error */
#define	_PPC_MCSR_BIT_BUS_WAERR	26	/* Bus write address error */
#define	_PPC_MCSR_BIT_BUS_IBERR	27	/* Bus instruction data bus error */
#define	_PPC_MCSR_BIT_BUS_RBERR	28	/* Bus read data bus error */
#define	_PPC_MCSR_BIT_BUS_WBERR	29	/* Bus write bus error */
#define	_PPC_MCSR_BIT_BUS_IPERR	30	/* Bus instruction parity error */
#define	_PPC_MCSR_BIT_BUS_RPERR	31	/* Bus read parity error */

#ifdef _CAG_SPE_SUPPORT
#define SPEFSCR 512     /* SPE Floating-pt Status and Control Register */

/* High word error bits */
#define _PPC_SPEFSCR_SOVH    0x80000000
#define _PPC_SPEFSCR_OVH     0x40000000
#define _PPC_SPEFSCR_FGH     0x20000000
#define _PPC_SPEFSCR_FXH     0x10000000
#define _PPC_SPEFSCR_FINVH   0x08000000
#define _PPC_SPEFSCR_FDBZH   0x04000000
#define _PPC_SPEFSCR_FUNFH   0x02000000
#define _PPC_SPEFSCR_FOVFH   0x01000000

/* Status Bits */
#define _PPC_SPEFSCR_FINXS   0x00200000
#define _PPC_SPEFSCR_FINVS   0x00100000
#define _PPC_SPEFSCR_FDBZS   0x00080000
#define _PPC_SPEFSCR_FUNFS   0x00040000
#define _PPC_SPEFSCR_FOVFS   0x00020000
#define _PPC_SPEFSCR_MODE    0x00010000

#define _PPC_SPEFSCR_SOV     0x00008000
#define _PPC_SPEFSCR_OV      0x00004000
#define _PPC_SPEFSCR_FG      0x00002000
#define _PPC_SPEFSCR_FX      0x00001000
#define _PPC_SPEFSCR_FINV    0x00000800
#define _PPC_SPEFSCR_FDBZ    0x00000400
#define _PPC_SPEFSCR_FUNF    0x00000200
#define _PPC_SPEFSCR_FOVF    0x00000100
#define _SPEFSCR_EXC_ENABLE_MASK (0x0000007C)
#define _PPC_SPEFSCR_FINXE   0x00000040
#define _PPC_SPEFSCR_FINVE   0x00000020
#define _PPC_SPEFSCR_FDBZE   0x00000010
#define _PPC_SPEFSCR_FUNFE   0x00000008
#define _PPC_SPEFSCR_FOVFE   0x00000004
#define _PPC_SPEFSCR_FRMC_RND_NR 0x00000000
#define _PPC_SPEFSCR_FRMC_RND_ZERO 0x00000001
#define _PPC_SPEFSCR_FRMC_RND_PINF 0x00000002
#define _PPC_SPEFSCR_FRMC_RND_NINF 0x00000003
#endif /* _CAG_SPE_SUPPORT */

/* SPRG0-SPRG3 are defined correctly in asmPpc.h */

#define SPRG4   276     /* 0x114 */
#define SPRG5   277     /* 0x115 */
#define SPRG6   278     /* 0x116 */
#define SPRG7   279     /* 0x117 */

#define SPRG4_R 260     /* Special Purpose Register General 4, read */
#define SPRG4_W 276     /* Special Purpose Register General 4, write */
#define SPRG5_R 261     /* Special Purpose Register General 5, read */
#define SPRG5_W 277     /* Special Purpose Register General 5, write */
#define SPRG6_R 262     /* Special Purpose Register General 6, read */
#define SPRG6_W 278     /* Special Purpose Register General 6, write */
#define SPRG7_R 263     /* Special Purpose Register General 7, read */
#define SPRG7_W 279     /* Special Purpose Register General 7, write */
#define TBL_R   268     /* Time Base Lower, read */
#define TBL_W   284     /* Time Base Lower, write */
#define TBU_R   269     /* Time Base Upper, read */
#define TBU_W   285     /* Time Base Upper, write */
#define TCR     340     /* Timer Control Register */
#define TSR     336     /* Timer Status Register */
#define USPRG0  256     /* User Special Purpose Register General 0 */
#define DBCR0   308     /* Debug Control Register 0 */
#define DBCR1   309     /* Debug Control Register 1 */
#define DBCR2   310     /* Debug Control Register 2 */
#define DBSR    304     /* Debug Status Register */
#define IAC1    312     /* Instr Address Compare Register 1 */
#define IAC2    313     /* Instr Address Compare Register 2 */
#define DAC1    316     /* Data Address Compare Register 1 */
#define DAC2    317     /* Data Address Compare Register 2 */
#define L1CFG0  515     /* L1 Config Register 0 */
#define L1CFG1  516     /* L1 Config Register 1 */
#define L1CSR0  1010    /* L1 Control Status Register 0 */
#define L1CSR1  1011    /* L1 Control Status Register 1 */

#define MAS0    624     /* MMU Assist Register 0 */
#define MAS1    625     /* MMU Assist Register 1 */
#define MAS2    626     /* MMU Assist Register 2 */
#define MAS3    627     /* MMU Assist Register 3 */
#define MAS4    628     /* MMU Assist Register 4 */
#define MAS6    630     /* MMU Assist Register 6 */
#define MAS7    944     /* MMU Assist Register 7 */

#define TLB0CFG 688     /* TLB 0 Config Register */
#define TLB1CFG 689     /* TLB 1 Config Register */
#define MMUCSR0 1012    /* MMU Control Status Register 0 */
#define MMUCFG  1015    /* MMU Config Register */

#define PIR     286
#define SVR     1023    /* silicon version register */

/* MSR (upper half) definitions */
/* ppcArch.h defines a generic MSR.  Here we define changes from that base */

#undef  _PPC_MSR_CM_U           /* 64 bit mode not implemented */
#undef  _PPC_MSR_ILE_U          /* little endian mode not supported */

/* wait state enable is equivalent to the power down enable for 60x */
#define _PPC_MSR_WE_U   0x0004          /* wait state enable */
#undef _PPC_MSR_POW_U
#define _PPC_MSR_POW_U  _PPC_MSR_WE_U

#define _PPC_MSR_WE     0x00040000      /* wait state enable */

#define _PPC_MSR_CE_U   0x0002          /* critical interrupt enable */
#define _PPC_MSR_CE     0x00020000      /* critical interrupt enable */


/* MSR (lower half) definitions */

#undef  _PPC_MSR_IR             /* insn address translation (always on) */
#undef  _PPC_MSR_DR             /* data address translation (always on) */
#undef  _PPC_MSR_SE             /* single step unsupported */
#undef  _PPC_MSR_BE             /* branch trace not supported */
#undef  _PPC_MSR_IP             /* exception prefix bit not supported */
#undef  _PPC_MSR_RI             /* recoverable interrupt unsupported */
#undef  _PPC_MSR_LE             /* little endian mode unsupported */
#undef  _PPC_MSR_BIT_LE         /* little endian mode unsupported */

/*
 * Machine check exception class is new to PPC in E500.  Although
 * bit position is same as classic MSR[ME] and is named the same,
 * the define of _PPC_MSR_MCE signifies the present of this
 * class of exception.  If present, both _PPC_MSR_MCE and
 * _PPC_MSR_ME should be defined to the mask of 0x1000.
 * In addition, Critical Exception Class is also a requirement.
 * The critical exception code stub does not mask exceptions
 * and is used for machine check exception class as well.
 * Therefore, _EXC_OFF_CRTL should be defined.
 */
#define _PPC_MSR_MCE    _PPC_MSR_ME	/* machine check enable */

#define _PPC_MSR_DE     0x0200		/* debug exception enable */
#define _PPC_MSR_IS     0x0020		/* insn address space selector */
#define _PPC_MSR_DS     0x0010		/* data address space selector */

#define	_PPC_MSR_BIT_WE		13
#define	_PPC_MSR_BIT_CE		14
#define	_PPC_MSR_BIT_DE		22
#define _PPC_MSR_BIT_IS		26
#define _PPC_MSR_BIT_DS		27

/* INT_MASK definition (mask EE & CE bits) : overwrite the one in asmPpc.h */

#define ME_MASK(src, des) \
    rlwinm  des, src, 0, _PPC_MSR_BIT_ME+1, _PPC_MSR_BIT_ME-1

#undef  INT_MASK
#define INT_MASK(src, des) \
	rlwinm  des, src, 0, _PPC_MSR_BIT_EE+1, _PPC_MSR_BIT_EE-1; \
	rlwinm  des, des, 0, _PPC_MSR_BIT_CE+1, _PPC_MSR_BIT_CE-1

#undef  INT_WE_MASK
#define INT_WE_MASK(src, des) \
	rlwinm  des, src, 0, _PPC_MSR_BIT_WE+1, _PPC_MSR_BIT_WE-1

#define HID0  1008
#define HID1  1009
#define DECAR 54
#define BUCSR 1013

#define _PPC_BUCSR_FI 0x200            /* Invalidate branch cache */
#define _PPC_BUCSR_E 0x1               /* Enable branch prediction */

#define  _PPC_BUCSR_STAC_EN 0x01000000      /* Segment target addr cache enable */
#define  _PPC_BUCSR_LS_EN   0x00400000      /* Link stack enable */
#define  _PPC_BUCSR_BBFI    0x00000200      /* Branch buffer flash invalidate */
#define  _PPC_BUCSR_BPEN    0x00000001      /* Branch prediction enable */
#define  _PPC_BUCSR_ENABLE (_PPC_BUCSR_STAC_EN|_PPC_BUCSR_LS_EN|_PPC_BUCSR_BBFI|_PPC_BUCSR_BPEN)

/* E500core other than e500mc has no FPU */
#ifndef _CAG_HARDWARE_FP
#undef  _PPC_MSR_FP             /* floating point not available */
#undef  _PPC_MSR_FE1            /* floating point not available */
#undef  _PPC_MSR_FE0            /* floating point not available */
#undef  _PPC_MSR_BIT_FP         /* MSR Floating Point Aval. bit - FP */
#undef  _PPC_MSR_BIT_FE0        /* MSR FP exception mode 0 bit - FE0 */
#undef  _PPC_MSR_BIT_FE1        /* MSR FP exception mode 1 bit - FE1 */
#undef  _EXC_PROG_SRR1_FPU      /* floating point not available */
#else	/* _CAG_HARDWARE_FP */
#define _CAG_FP_CLASS_HW	1
#define  _PPC_MSR_BIT_FP         18
#define  _PPC_MSR_BIT_FE0        20
#define  _PPC_MSR_BIT_FE1        23
#endif	/* _CAG_HARDWARE_FP */

/* MSR MMU Bit extraction */

#define _PPC_MSR_CM_OR(reg)
#define _PPC_MSR_MMU_RI_EXTRACT(src, dst) \
	rlwinm	dst, src, 0, _PPC_MSR_BIT_IS, _PPC_MSR_BIT_DS

#ifdef	_CAG_HARDWARE_FP
#define	_PPC_MSR_MMU_RI_FP_EXTRACT(src, dst) \
	li	dst, _PPC_MSR_FP | _PPC_MSR_IS | _PPC_MSR_DS ; \
	_PPC_MSR_CM_OR(dst); \
	and	dst, dst, src
#else	/* _CAG_HARDWARE_FP */
#ifdef _CAG_SPE_SUPPORT
#define	_PPC_MSR_MMU_RI_FP_EXTRACT(src, dst) \
	lis	dst, _PPC_MSR_SPE_U ;\
	ori	dst, dst, _PPC_MSR_IS | _PPC_MSR_DS ; \
	and	dst, dst, src
#else /* !_CAG_SPE_SUPPORT */
#define	_PPC_MSR_MMU_RI_FP_EXTRACT(src, dst) \
	ori	dst, dst, _PPC_MSR_IS | _PPC_MSR_DS ; \
	and	dst, dst, src
#endif /* _CAG_SPE_SUPPORT */
#endif	/* _CAG_HARDWARE_FP */

/*
 * Exception syndrome register mask bits:
 * 0 - error not occured 1 - error occured
 */
#define _PPC_ESR_PIL_U 0x0800      /* Pgm Interrupt -- Illegal Insn */
#define _PPC_ESR_PPR_U 0x0400      /* Pgm Interrupt -- Previleged Insn */
#define _PPC_ESR_PTR_U 0x0200      /* Pgm Interrupt -- Trap */
#define _PPC_ESR_ST_U  0x0080      /* Store Operation */
#define _PPC_ESR_DLK_U 0x0020      /* Data Locked -- DSI occured */
#define _PPC_ESR_ILK_U 0x0010      /* Inst Locked -- DSI occured */
#define _PPC_ESR_AP_U  0x0008      /* AP Operation */
#define _PPC_ESR_BO_U  0x0002      /* Byte Ordering Exception */
#define _PPC_ESR_PIL   0x08000000  /* Pgm Interrupt -- Illegal Insn */
#define _PPC_ESR_PPR   0x04000000  /* Pgm Interrupt -- Previleged Insn */
#define _PPC_ESR_PTR   0x02000000  /* Pgm Interrupt -- Trap */
#define _PPC_ESR_ST    0x00800000  /* Store Operation */
#define _PPC_ESR_DLK   0x00200000  /* Data Storage Interrupt -- Locking */
#define _PPC_ESR_ILK   0x00100000  /* Inst Locked -- DSI occured */
#define _PPC_ESR_AP    0x00080000  /* AP Operation */
#define _PPC_ESR_BO    0x00020000  /* Byte Ordering Exception */
#ifdef _CAG_SPE_SUPPORT
#define _PPC_ESR_SPE   0x00000080  /* SPE exception */
#endif /* _CAG_SPE_SUPPORT */

/* Bits in the upper half of TCR */
#define _PPC_TCR_WP_U     0xc000  /* Watchdog Timer Period */
#define _PPC_TCR_WRC_U    0x3000  /* Watchdog Timer Reset Control: */
#define _PPC_TCR_WRC_MC   0x1000  /* 01 - machine check */
#define _PPC_TCR_WRC_HR   0x2000  /* 10 - hard reset */
#define _PPC_TCR_WRC_CR   0x2000  /* 11 - core reset */
#define _PPC_TCR_WIE_U    0x0800  /* Watchdog Timer Interrupt Enable */
#define _PPC_TCR_DIE_U    0x0400  /* Decrementer Interrupt Enable */
#define _PPC_TCR_FP_U     0x0300  /* Fixed Interval Timer Period */
#define _PPC_TCR_FIE_U    0x0080  /* Fixed Interval Timer Interrupt Enable */
#define _PPC_TCR_ARE_U    0x0040  /* Decrementer Auto-Reload Enable */
#define _PPC_TCR_WPEXT_U  0x001E  /* Watchdog Timer Period extension */
#define _PPC_TCR_FPEXT    0x1E000 /* Fixed-interval timer period extension */

/* Bits in the upper half of TSR */
#define _PPC_TSR_ENW_U  0x8000  /* Enable Next Watchdog Timer Exception */
#define _PPC_TSR_WIS_U  0x4000  /* Watchdog Timer Interrupt Status */
#define _PPC_TSR_CAG_U  0x3000  /* Watchdog Timer Reset Status */
#define _PPC_TSR_DIS_U  0x0800  /* Decrementer Interrupt Status */
#define _PPC_TSR_FIS_U  0x0400  /* Fixed Interval Timer Interrupt Status */

/* versions of the aligned for 32-bit TCR/TSR register access */
#define _PPC_TCR_DIE (_PPC_TCR_DIE_U << 16)
#define _PPC_TSR_DIS (_PPC_TSR_DIS_U << 16)

/* hardware dependent register 0 */
#define _PPC_HID0_DOZE_U    0x0080      /* DOZE power management mode */
#define _PPC_HID0_NAP_U	    0x0040      /* NAP power management mode */
#define _PPC_HID0_SLEEP_U   0x0020      /* SLEEP power management mode */
#define _PPC_HID0_DOZE  0x00800000      /* DOZE power management mode */
#define _PPC_HID0_NAP   0x00400000      /* NAP power management mode */
#define _PPC_HID0_SLEEP 0x00200000      /* SLEEP power management mode */
#define _PPC_HID0_TBEN  0x00004000      /* time base enable */
#define _PPC_HID0_EMCP  0x80000000      /* Enable machine check pin */

#define _PPC_HID0_MAS7EN  0x00000080    /* Enable use of MAS7 for tlbre */
#define _PPC_HID0_DCFA    0x00000040    /* Use this bit to flush only valid entries same as 74XX */
#define _PPC_HID0_BIT_MAS7EN   24
#define _PPC_HID0_BIT_DCFA     25
#define _PPC_85XX_USE_DCFA

/* hardware dependent register 1 */

#define _PPC_HID1_ABE    0x00001000     /* Address broadcast enable */
#define _PPC_HID1_ASTME  0x00002000  /* Address bus streaming mode enable */
#define _PPC_HID1_RXFE   0x00020000   /* Read fault exception enable*/
#define _PPC_HID1_MBDD   (1<<6)       /* optimized sync instruction */

/* Cache Defines */

/* Instruction and Data Cache bit fields are the same */

#define _PPC_L1CSR_E   0x00000001 /* Enable */
#define _PPC_L1CSR_FI  0x00000002 /* Flash Invalidate */
#define _PPC_L1CSR_FLR 0x00000100 /* Lock Bits Flash */
#define _PPC_L1CSR_LO  0x00000200 /* Lock Overflow */
#define _PPC_L1CSR_UL  0x00000400 /* Unable to lock   - status bit */
#define _PPC_L1CSR_UL_V(x)  (x >> 10)
#define _PPC_L1CSR_SLC 0x00000800 /* Snoop lock clear  - status bit */
#define _PPC_L1CSR_SLC_V(x) (x >> 11)
#define _PPC_L1CSR_PIE 0x00008000 /* Parity Injection Enable */
#define _PPC_L1CSR_CPE 0x00010000 /* Parity Enable */

#define  _PPC_L1CSR0_CPE   0x00010000      /* Data Cache Parity Enable */
#define  _PPC_L1CSR0_CUL   0x00000400      /* (D-)Cache Unable to Lock */
#define  _PPC_L1CSR0_DCLFR 0x00000100      /* D-Cache Lock Flash Reset */
#define  _PPC_L1CSR0_DCFI  0x00000002      /* Data Cache Flash Invalidate */
#define  _PPC_L1CSR0_DCE   0x00000001      /* Data Cache Enable */

#define  _PPC_L1CSR1_CPE   0x00010000      /* Instruction Cache Parity Enable */
#define  _PPC_L1CSR1_ICUL  0x00000400      /* I-Cache Unable to Lock */
#define  _PPC_L1CSR1_ICLFR 0x00000100      /* I-Cache Lock Flash Reset */
#define  _PPC_L1CSR1_ICFI  0x00000002      /* Instruction Cache Flash Invalidate */
#define  _PPC_L1CSR1_ICE   0x00000001      /* Instruction Cache Enable */


/* Instruction and Datat Cache bit fields are the same */
#define _PPC_L1CFG_SIZE_MASK   0x00000FFF
#define _PPC_L1CFG_NWAY_MASK   0x000FF000
#define _PPC_L1CFG_NWAY_V(x)   (x >> 12)
#define _PPC_L1CFG_PA_MASK     0x00100000
#define _PPC_L1CFG_PA_V(x)     (x >> 16)
#define _PPC_L1CFG_LA_MASK     0x00200000
#define _PPC_L1CFG_LA_V(x)     (x >> 17)
#define _PPC_L1CFG_REPL_MASK   0x00400000
#define _PPC_L1CFG_REPL_V(x)   (x >> 18)
#define _PPC_L1CFG_BSIZE_MASK  0x01800000
#define _PPC_L1CFG_BSIZE_V(x)  (x >> 19)
#define _PPC_L1CFG_CARCH_MASK  0xC0000000    /* L1CFG0 only */
#define _PPC_L1CFG_CARCH_V(x)  (x >> 30)

/* MMU register defines */

#define PID     48
#define PID_MASK 0x0FF
#define PID0    48
#define PID1    633
#define PID2    634

#define _PPC_MMUCSR0_L2TLB1_FI  0x00000002
#define _PPC_MMUCSR0_L2TLB1_FI_V(x) (x >> 1)
#define _PPC_MMUCSR0_L2TLB0_FI  0x00000004
#define _PPC_MMUCSR0_L2TLB0_FI_V(x) (x >> 2)
#define _PPC_MMUCSR0_DL1MMU_FI  0x00000008
#define _PPC_MMUCSR0_DL1MMU_FI_V(x) (x >> 3)
#define _PPC_MMUCSR0_IL1MMU_FI  0x00000010
#define _PPC_MMUCSR0_IL1MMU_FI_V(x) (x >> 4)

#define _PPC_MAS0_NV           0x00000000

#define _PPC_MAS0_ESEL_BIT_MIN 6
#define _PPC_MAS0_ESEL_BIT_MAX 15
#define _PPC_MAS0_ESEL_MASK    0x03ff0000

#define _PPC_MAS0_ESEL_BIT     16

#define _PPC_MAS0_ESEL_V(x)    (x >> _PPC_MAS0_ESEL_BIT)
#define _PPC_MAS0_TLBSEL1      0x10000000
#define _PPC_MAS0_TLBSEL1_U    0x1000
#define _PPC_MAS0_TLBSEL_MASK  0x30000000

#define _PPC_MAS0_TLBSEL(x)	(((x) << 28) & _PPC_MAS0_TLBSEL_MASK)
#define _PPC_MAS0_ESEL_MSK	0x0FFF0000
#define _PPC_MAS0_ESEL(x)	(((x) << 16) & _PPC_MAS0_ESEL_MSK)

#define _PPC_MAS1_TSIZE(x)	(((x) << 7) & 0x00000F80)

#define _PPC_MAS1_V_BIT         0
#define _PPC_MAS1_V             0x80000000
#define _PPC_MAS1_V_U           0x8000

#define _PPC_MAS1_IPROT_BIT     1
#define _PPC_MAS1_IPROT         0x40000000
#define _PPC_MAS1_IPROT_U       0x4000

#define _PPC_MAS1_TS_BIT        19
#define _PPC_MAS1_TS            0x00001000

#define _PPC_MAS1_TSIZE_BIT_MIN 20
#define _PPC_MAS1_TSIZE_BIT_MAX 23
#define _PPC_MAS2_EPN_BIT_MAX   19

#define _PPC_MAS2_W_BIT         27
#define _PPC_MAS2_W             0x00000010

#define _PPC_MAS2_I_BIT         28
#define _PPC_MAS2_I             0x00000008

#define _PPC_MAS2_M_BIT         29
#define _PPC_MAS2_M             0x00000004

#define _PPC_MAS2_G_BIT         30
#define _PPC_MAS2_G             0x00000002

#define _PPC_MAS2_E_BIT         31
#define _PPC_MAS2_E             0x00000001

#define _PPC_MAS3_RPN_BIT_MAX   19

#define _PPC_MAS3_UX_BIT        26
#define _PPC_MAS3_UX            0x00000020
#define _PPC_MAS3_SX            0x00000010
#define _PPC_MAS3_UW            0x00000008
#define _PPC_MAS3_SW            0x00000004
#define _PPC_MAS3_UR            0x00000002
#define _PPC_MAS3_SR            0x00000001

#define _PPC_MAS6_SIND         0x00000002

#define _PPC_MAS7_RPN_MASK      0x0000000F

#define _PPC_TLBnCFG_NENTRY_MASK    0xFFF
#define _PPC_TLBnCFG_SIZE_MASK      0xF

/* processor version register value */

#define PVR_VALUE       0x80210030      /* expected value in PVR */

/* Range of hardware context numbers (PID register & TLB TID field) */

#define MMU_ASID_MIN            1
#define MMU_ASID_MAX            255
#define MMU_ASID_GLOBAL         MMU_ASID_MIN

/* debug control register 0 */
#define _DBCR0_IDM_U    0x4000          /* internal debug mode */
#define _DBCR0_RST_U    0x3000          /* reset */
#define _DBCR0_ICMP_U   0x0800          /* instruction completion debug event */
#define _DBCR0_BRT_U    0x0400          /* branch taken debug event */
#define _DBCR0_IRPT_U   0x0200          /* interrupt debug event */
#define _DBCR0_TRAP_U   0x0100          /* trap debug event */
#define _DBCR0_IAC1_U   0x0080          /* instruction address compare 1 */
#define _DBCR0_IAC2_U   0x0040          /* instruction address compare 2 */
#define _DBCR0_DAC1R_U  0x0008          /* data address compare 1 Read */
#define _DBCR0_DAC1W_U  0x0004          /* data address compare 1 Write */
#define _DBCR0_DAC2R_U  0x0002          /* data address compare 2 Read */
#define _DBCR0_DAC2W_U  0x0001          /* data address compare 2 Write */
#define _DBCR0_IDM      0x40000000      /* internal debug mode */
#define _DBCR0_RST      0x30000000      /* reset */
#define _DBCR0_ICMP     0x08000000      /* instruction completion debug event */
#define _DBCR0_BRT      0x04000000      /* branch taken */
#define _DBCR0_IRPT     0x02000000      /* exception debug event */
#define _DBCR0_TRAP     0x01000000      /* trap debug event */
#define _DBCR0_IAC1     0x00800000      /* instruction address compare 1 */
#define _DBCR0_IAC2     0x00400000      /* instruction address compare 2 */
#define _DBCR0_DAC1R    0x00080000      /* data address compare 1 Read */
#define _DBCR0_DAC1W    0x00040000      /* data address compare 1 Write */
#define _DBCR0_DAC2R    0x00020000      /* data address compare 2 Read */
#define _DBCR0_DAC2W    0x00010000      /* data address compare 2 Write */
#define _DBCR0_RET      0x00008000      /* return debug event */
#define _DBCR0_FT       0x00000001      /* freeze timers on debug */

/* debug control register 1 */
#define _DBCR1_IAC1US_U   0xc000        /* IAC 1 User/Supervisor */
#define _DBCR1_IAC1ER_U   0x3000        /* IAC 1 Effective/Real */
#define _DBCR1_IAC2US_U   0x0c00        /* IAC 2 User/Supervisor */
#define _DBCR1_IAC2ER_U   0x0300        /* IAC 2 Effective/Real */
#define _DBCR1_IAC12M_U   0x00c0        /* IAC 1/2 Mode */
#define _DBCR1_IAC12AT_U  0x0001        /* IAC 1/2 Auto-Toggle Enable */
#define _DBCR1_IAC1US     0xc0000000    /* IAC 1 User/Supervisor */
#define _DBCR1_IAC1ER     0x30000000    /* IAC 1 Effective/Real */
#define _DBCR1_IAC2US     0x0c000000    /* IAC 2 User/Supervisor */
#define _DBCR1_IAC2ER     0x03000000    /* IAC 2 Effective/Real */
#define _DBCR1_IAC12M     0x00c00000    /* IAC 1/2 Mode */

/* debug control register 2 */
#define _DBCR2_DAC1US_U   0xc000        /* DAC 1 User/Supervisor */
#define _DBCR2_DAC1ER_U   0x3000        /* DAC 1 Effective/Real */
#define _DBCR2_DAC2US_U   0x0c00        /* DAC 2 User/Supervisor */
#define _DBCR2_DAC2ER_U   0x0300        /* DAC 2 Effective/Real */
#define _DBCR2_DAC12M_U   0x00c0        /* DAC 1/2 Mode */
#define _DBCR2_DAC1US     0xc0000000    /* DAC 1 User/Supervisor */
#define _DBCR2_DAC1ER     0x30000000    /* DAC 1 Effective/Real */
#define _DBCR2_DAC2US     0x0c000000    /* DAC 2 User/Supervisor */
#define _DBCR2_DAC2ER     0x03000000    /* DAC 2 Effective/Real */
#define _DBCR2_DAC12M     0x00c00000    /* DAC 1/2 Mode mask */
#define _DBCR2_DAC12M_INC 0x00800000    /* DAC 1/2 Mode: Inclusive range */

/* debug status register */
#define _DBSR_IDE_U     0x8000          /* Imprecise Debug Event */
#define _DBSR_UDE_U     0x4000          /* Unconditional Debug Event */
#define _DBSR_MRR_U     0x3000          /* Most Recent Reset */
#define _DBSR_ICMP_U    0x0800          /* Instruction Completion Debug Event */
#define _DBSR_BRT_U     0x0400          /* Branch Taken Debug Event */
#define _DBSR_IRPT_U    0x0200          /* Interrupt Debug Event */
#define _DBSR_TRAP_U    0x0100          /* Trap Debug Event */
#define _DBSR_IAC1_U    0x0080          /* IAC 1 Debug Event */
#define _DBSR_IAC2_U    0x0040          /* IAC 2 Debug Event */
#define _DBSR_DAC1R_U   0x0008          /* DAC/DVC 1 Read Debug Event */
#define _DBSR_DAC1W_U   0x0004          /* DAC/DVC 1 Write Debug Event */
#define _DBSR_DAC2R_U   0x0002          /* DAC/DVC 2 Read Debug Event */
#define _DBSR_DAC2W_U   0x0001          /* DAC/DVC 2 Write Debug Event */
#define _DBSR_IDE       0x80000000      /* Imprecise Debug Event */
#define _DBSR_UDE       0x40000000      /* Unconditional Debug Event */
#define _DBSR_MRR       0x30000000      /* Most Recent Reset */
#define _DBSR_ICMP      0x08000000      /* Instruction Completion Debug Event */
#define _DBSR_BRT       0x04000000      /* Branch Taken Debug Event */
#define _DBSR_IRPT      0x02000000      /* Interrupt Debug Event */
#define _DBSR_TRAP      0x01000000      /* Trap Debug Event */
#define _DBSR_IAC1      0x00800000      /* IAC 1 Debug Event */
#define _DBSR_IAC2      0x00400000      /* IAC 2 Debug Event */
#define _DBSR_DAC1R     0x00080000      /* DAC/DVC 1 Read Debug Event */
#define _DBSR_DAC1W     0x00040000      /* DAC/DVC 1 Write Debug Event */
#define _DBSR_DAC2R     0x00020000      /* DAC/DVC 2 Read Debug Event */
#define _DBSR_DAC2W     0x00010000      /* DAC/DVC 2 Write Debug Event */
#define _DBSR_RET       0x00008000      /* Return Debug Event */

/* mask for hardware breakpoints */
#define _DBSR_HWBP_MSK  ( _DBSR_IAC1 | _DBSR_IAC2 | \
			  _DBSR_DAC1R | _DBSR_DAC1W | \
			  _DBSR_DAC2R | _DBSR_DAC2W )

#ifdef __cplusplus
}
#endif

#endif /* __INCppc85xxh */
