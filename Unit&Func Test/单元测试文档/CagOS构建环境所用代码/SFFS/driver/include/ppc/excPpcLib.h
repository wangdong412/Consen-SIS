/* excPpcLib.h - PowerPC exception library header */

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

#ifndef __INCexcPcLibh
#define __INCexcPcLibh

#ifdef __cplusplus
extern "C" {
#endif

/* exception information valid bits */

#define _EXC_CODE_SIZE	16	/* size of exc code */

#define EXC_VEC_LOW	0x00000	/* lowest vector entry */
#define EXC_VEC_HIGH	0x02fff	/* highest vector entry */

#define _PPC_EXC_VEC_BASE_LOW	0x00000000
#define _PPC_EXC_VEC_BASE_HIGH	0xfff00000

#define XR1         0
#define XBLK        (XR1+_PPC_REG_SIZE)
#define XR0         (XBLK+_PPC_REG_SIZE)
#define XSRR0       (XR0+_PPC_REG_SIZE)
#define XSRR1       (XSRR0+_PPC_REG_SIZE)
#define XCTR        (XSRR1+_PPC_REG_SIZE)
#define XXER        (XCTR+_PPC_REG_SIZE)
#define XCR         (XXER+_PPC_REG_SIZE)
#define XLR         (XCR+_PPC_REG_SIZE)
#define XR2         (XLR+_PPC_REG_SIZE)
#define XR3         (XR2+_PPC_REG_SIZE)
#define XR4         (XR3+_PPC_REG_SIZE)
#define XR5         (XR4+_PPC_REG_SIZE)
#define XR6         (XR5+_PPC_REG_SIZE)
#define XR7         (XR6+_PPC_REG_SIZE)
#define XR8         (XR7+_PPC_REG_SIZE)
#define XR9         (XR8+_PPC_REG_SIZE)
#define XR10        (XR9+_PPC_REG_SIZE)
#define XR11        (XR10+_PPC_REG_SIZE)
#define XR12        (XR11+_PPC_REG_SIZE)
#define XR13        (XR12+_PPC_REG_SIZE)
#define XR14        (XR13+_PPC_REG_SIZE)
#define XR15        (XR14+_PPC_REG_SIZE)
#define XR16        (XR15+_PPC_REG_SIZE)
#define XR17        (XR16+_PPC_REG_SIZE)
#define XR18        (XR17+_PPC_REG_SIZE)
#define XR19        (XR18+_PPC_REG_SIZE)
#define XR20        (XR19+_PPC_REG_SIZE)
#define XR21        (XR20+_PPC_REG_SIZE)
#define XR22        (XR21+_PPC_REG_SIZE)
#define XR23        (XR22+_PPC_REG_SIZE)
#define XR24        (XR23+_PPC_REG_SIZE)
#define XR25        (XR24+_PPC_REG_SIZE)
#define XR26        (XR25+_PPC_REG_SIZE)
#define XR27        (XR26+_PPC_REG_SIZE)
#define XR28        (XR27+_PPC_REG_SIZE)
#define XR29        (XR28+_PPC_REG_SIZE)
#define XR30        (XR29+_PPC_REG_SIZE)
#define XR31        (XR30+_PPC_REG_SIZE)
#define XMSR        (XR31+_PPC_REG_SIZE)
#define EXC_FRAME_SIZE   (XMSR+_PPC_REG_SIZE)

/* exception vector offset */

/*
 * This processor implements the Book E Interrupt Vector Prefix Register
 * and Interrupt Vector Offset Registers, instead of the Exception Vector
 * Prefix Register used (with fixed offsets) in older designs.  The new
 * architecture enables us to locate exception vectors "anywhere".
 *
 * NOTE:  The frameless nesting code in intALib.s requires that "normal"
 * (as opposed to "critical") vectors on processors which implement critical
 * interrupts be at addresses divisible by 0x0080.
 */
#define	_EXC_OFF_CRTL		0x0100	/* Critical Input */
#define	_EXC_OFF_MACH		0x0200	/* Machine Check */
#define	_EXC_OFF_DATA		0x0300	/* Data Storage */
#define	_EXC_OFF_INST		0x0400	/* Instruction Storage */
#define	_EXC_OFF_INTR		0x0500	/* External Input */
#define	_EXC_OFF_ALIGN		0x0600	/* Alignment */
#define	_EXC_OFF_PROG		0x0700	/* Program */
#define	_EXC_OFF_FPU		0x0800	/* Floating Point Unavailable */
#define	_EXC_OFF_SYSCALL	0x0900	/* System Call */
#define	_EXC_OFF_APU		0x0a00	/* Auxiliary Processor Unavailable */
#define	_EXC_OFF_DECR		0x0b00	/* Decrementer */
#define	_EXC_OFF_FIT		0x0c00	/* Fixed Interval Timer */
#define	_EXC_OFF_WD		0x0d00	/* Watchdog Timer */
#define	_EXC_OFF_DATA_MISS	0x0e00	/* Data TLB Error */
#define	_EXC_OFF_INST_MISS	0x0f00	/* Instruction TLB Error */
#define	_EXC_OFF_DBG		0x1000	/* Debug exception */
#define _EXC_OFF_SPE		0x1100	/* SPE */
#define _EXC_OFF_VEC_DATA	0x1200	/* vect fp data */
#define _EXC_OFF_VEC_RND	0x1300	/* vect fp round */
#define _EXC_OFF_PERF_MON	0x1400	/* performance monitor */
#define	_EXC_OFF_L1_PARITY     	0x1500	/* modify IVOR1 here for L1 parity */
#define	_EXC_OFF_END		0x1600	/* end of vectors */

/* Mappings between vector names and corresponding IVORs, for excALib.s */
#define	IVOR0_VAL   _EXC_OFF_CRTL	/* Critical Input */
#define	IVOR1_VAL   _EXC_OFF_MACH	/* Machine Check */
#define	IVOR2_VAL   _EXC_OFF_DATA	/* Data Storage */
#define	IVOR3_VAL   _EXC_OFF_INST	/* Instruction Storage */
#define	IVOR4_VAL   _EXC_OFF_INTR	/* External Input */
#define	IVOR5_VAL   _EXC_OFF_ALIGN	/* Alignment */
#define	IVOR6_VAL   _EXC_OFF_PROG	/* Program */
#define	IVOR7_VAL   _EXC_OFF_FPU	/* Floating Point Unavailable */
#define	IVOR8_VAL   _EXC_OFF_SYSCALL	/* System Call */
#define	IVOR9_VAL   _EXC_OFF_APU	/* Auxiliary Processor Unavailable */
#define	IVOR10_VAL  _EXC_OFF_DECR	/* Decrementer */
#define	IVOR11_VAL  _EXC_OFF_FIT	/* Fixed Interval Timer */
#define	IVOR12_VAL  _EXC_OFF_WD		/* Watchdog Timer */
#define	IVOR13_VAL  _EXC_OFF_DATA_MISS	/* Data TLB Error */
#define	IVOR14_VAL  _EXC_OFF_INST_MISS	/* Instruction TLB Error */
#define	IVOR15_VAL  _EXC_OFF_DBG	/* Debug exception */

#ifdef _CAG_SPE_SUPPORT
#define IVOR32_VAL  _EXC_OFF_SPE	/* SPE */
#define IVOR33_VAL  _EXC_OFF_VEC_DATA	/* vect fp data */
#define IVOR34_VAL  _EXC_OFF_VEC_RND	/* vect fp round */
#endif /* _CAG_SPE_SUPPORT */

#define IVOR35_VAL  _EXC_OFF_PERF_MON	/* performance monitor */

/*
 * Decrementer and PIT are by and large functionally equivalent,
 * except that the _DECR vector does not need to be relocated.
 */
#if	defined(_EXC_OFF_DECR)
#define	_EXC_OFF_SYSTIMER	_EXC_OFF_DECR
#define	_EXC_NEW_OFF_SYSTIMER	_EXC_OFF_DECR
#elif	defined(_EXC_OFF_PIT)
#define	_EXC_OFF_SYSTIMER	_EXC_OFF_PIT
#define	_EXC_NEW_OFF_SYSTIMER	_EXC_NEW_OFF_PIT
#else
#undef	_EXC_OFF_SYSTIMER
#undef	_EXC_NEW_OFF_SYSTIMER
#endif

/* SRR1 bit definition for program exception */

#ifdef  _PPC_MSR_FP
# define _EXC_PROG_SRR1_FPU	0x00100000	/* floating-point enabled */
#endif /* _PPC_MSR_FP */
#define _EXC_PROG_SRR1_ILL	0x00080000	/* illegal instruction */
#define _EXC_PROG_SRR1_PRIV	0x00040000	/* privileged instruction */
#define _EXC_PROG_SRR1_TRAP	0x00020000	/* trap exception */
#define _EXC_PROG_SRR1_NIA	0x00010000	/* SRR0 has nia (else cia) */

/*
 * This processor implements "critical" exceptions,
 * which use different SRR's than other exceptions.
 * Abstract the names of those registers, since they are
 * different in the 440 and 85XX than in previous 40x processors.
 */
#  define	SRR_CI_PC	CSRR0
#  define	SRR_CI_MSR	CSRR1

/* Retain earlier names for compatibility */
# define CRIT_SAVE_PC	SRR_CI_PC
# define CRIT_SAVE_MSR	SRR_CI_MSR

#ifndef _ASMLANGUAGE

/* function declarations */
typedef UINT32  vecTblOffset;   /* Offset in vector table, e.g. _EXC_OFF_* */
typedef char *  codeBase;       /* Base address in normal code space */
typedef char *  vectorBase;     /* Base address in vector execution space */
typedef char *  codePtr;        /* Pointer into normal code space */
typedef char *  vectorPtr;      /* Pointer into vector execution space */

extern void		excVecSet (FUNCPTR *, FUNCPTR);
extern FUNCPTR		excVecGet (FUNCPTR *);
extern void		excVecBaseSet (FUNCPTR *);
extern FUNCPTR *	excVecBaseGet (void);

IMPORT void	sysMsrSet (UINT32);          /* machine status reg */
IMPORT UINT32	sysMsrGet (void);
IMPORT void     sysSdaSet (void * pRegs);

IMPORT UINT32   sysDearGet (void);           /* data exc addr reg */
IMPORT void     sysDearSet (UINT32);
IMPORT UINT32	sysCsrr0Get (void);          /* save/restore reg 0 */
IMPORT void	sysCsrr0Set (UINT32);
IMPORT UINT32	sysCsrr1Get (void);          /* save/restore reg 1 */
IMPORT void	sysCsrr1Set (UINT32);
IMPORT UINT32	sysMcsrr0Get (void);         /* save/restore reg 0 */
IMPORT void	sysMcsrr0Set (UINT32);
IMPORT UINT32	sysMcsrr1Get (void);         /* save/restore reg 1 */
IMPORT void	sysMcsrr1Set (UINT32);
#ifdef _CAG_SPE_SUPPORT
IMPORT UINT32	sysSpefscrGet (void);        /* FP status control reg */
IMPORT void	sysSpefscrSet (UINT32);
#endif /* _CAG_SPE_SUPPORT */
IMPORT void	sysDecarSet (UINT32);        /* dec auto reload reg */
IMPORT UINT32	sysL1CSR0Get (void);         /* L1 control status reg 0 */
IMPORT void	sysL1CSR0Set (UINT32);
IMPORT UINT32	sysL1CSR1Get (void);         /* L1 control status reg 1 */
IMPORT void	sysL1CSR1Set (UINT32);
IMPORT UINT32	sysL1CFG0Get (void);         /* L1 config reg 0 */
IMPORT UINT32	sysL1CFG1Get (void);         /* L1 config reg 1 */
IMPORT void	sysIvor1Set (UINT32);        /* Set Mchk exception offset */

IMPORT void	sysSrr0Set (UINT32);         /* save/restore reg 0 */
IMPORT UINT32	sysSrr0Get (void);
IMPORT void	sysSrr1Set (UINT32);         /* save/restore reg 1 */
IMPORT UINT32	sysSrr1Get (void);
IMPORT UINT32	sysFirstBit (UINT32);        /* get 1st bit set using cntlzw */
IMPORT UINT32	sysPvrGet (void);            /* processor version reg */

IMPORT UINT32	sysSvrGet (void);            /* system version reg */
IMPORT void	sysDecSet (UINT32);          /* decrementer reg */
IMPORT UINT32	sysDecGet (void);
IMPORT UINT32	sysDecReload (UINT32);       /* reload with lantency skew adj */

IMPORT void	sysHid1Set (UINT32);         /* hardware imp dependent reg 1 */
IMPORT UINT32	sysHid1Get (void);

IMPORT void	sysC1StateSet (void);
IMPORT void	sysTimeBaseSet (UINT32 tbu, UINT32 tbl); /* time base up/lower */
IMPORT void	sysTimeBaseGet (UINT32 * pTbu, UINT32 * pTbl);
IMPORT void	sysFpscrSet (UINT32);        /* FP status control reg */
IMPORT UINT32	sysFpscrGet (void);

IMPORT void     sysDecIntEnable (void);
IMPORT void     sysDecIntAck (void);

IMPORT void     sysFitIntEnable (void);      /* timer ctrl reg set FIT int en */
IMPORT void     sysFitIntDisable (void);
IMPORT void     sysFitIntAck (void);         /* clear TSR FIT pending bit */

IMPORT UINT32   sysExisrGet (void);          /* ext interrupt status reg */
IMPORT void     sysExisrClear (UINT32);
IMPORT UINT32   sysExierGet (void);          /* ext interrupt enable reg */
IMPORT void     sysExierSet (UINT32);

IMPORT void     sysHid0Set (UINT32);         /* hardware imp dependent reg 0 */
IMPORT UINT32   sysHid0Get (void);

IMPORT UINT32   sysIvprGet (void);
IMPORT void     sysIvprSet (UINT32);

IMPORT UINT32   sysTcrGet (void);            /* timer ctrl reg */
IMPORT void     sysTcrSet (UINT32);
IMPORT UINT32   sysTsrGet (void);            /* timer status reg */
IMPORT void     sysTsrSet (UINT32);

IMPORT UINT32   sysDbcr0Get (void);
IMPORT void     sysDbcr0Set (UINT32);
IMPORT UINT32   sysDbcr1Get (void);
IMPORT void     sysDbcr1Set (UINT32);
IMPORT UINT32   sysDbcr2Get (void);
IMPORT void     sysDbcr2Set (UINT32);

IMPORT UINT32   sysDbsrGet (void);           /* debug status reg */
IMPORT void     sysDbsrClear (UINT32);
IMPORT UINT32   sysDac1Get (void);           /* data addr cmp reg 1 */
IMPORT void     sysDac1Set (UINT32);
IMPORT UINT32   sysDac2Get (void);           /* data addr cmp reg 2 */
IMPORT void     sysDac2Set (UINT32);
IMPORT UINT32   sysIac1Get (void);           /* instr addr cmp reg 1 */
IMPORT void     sysIac1Set (UINT32);
IMPORT UINT32   sysIac2Get (void);           /* instr addr cmp reg 2 */
IMPORT void     sysIac2Set (UINT32);

IMPORT UINT32   sysIac3Get (void);
IMPORT void     sysIac3Set (UINT32);
IMPORT UINT32   sysIac4Get (void);
IMPORT void     sysIac4Set (UINT32);

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* __INCexcPpcLibh */
