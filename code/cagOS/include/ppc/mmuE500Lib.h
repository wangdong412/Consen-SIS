/* mmuE500Lib.h - mmu library for PowerPC E500 series */

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

#ifndef __INCmmuE500Libh
#define __INCmmuE500Libh

#include <ppc/ppc85xx.h>

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

#define _INSTRUCTION_CACHE      0       /* Instruction Cache(s) */
#define _DATA_CACHE             1       /* Data Cache(s) */
#define _BRANCH_CACHE           2       /* Branch Cache(s) */

#define MMU_STATE_IPROT 0x40000000
#define MMU_ROM_ACCESS (MMU_STATE_CACHEABLE_WRITETHROUGH | MMU_STATE_CACHEABLE | \
                        MMU_STATE_MEM_COHERENCY          | MMU_STATE_GUARDED)

#define MMU_PAGE_SIZE           0x1000          /* pages are 4K bytes */
#define	MMU_MIN_PAGE_SIZE_BITS	12		/* minimu page size in bits */
#define	MMU_PAGE_SIZE_BITS	MMU_MIN_PAGE_SIZE_BITS
/* default page size in bits */

/*
 * MMU_I_ADDR_TRANS and MMU_D_ADDR_TRANS are used in the mmuE500Lib /
 * mmuPpcALib & cacheALib call interface. They are not useful at the
 * application programming level.
 */

#define MMU_I_ADDR_TRANS        0
#define MMU_D_ADDR_TRANS        1

/*
 * MMU_INST and MMU_DATA are used at the usrMmuInit
 * interface.  They are not useful at the application programming
 * level.
 */

#define MMU_INST                0x01
#define MMU_DATA                0x02

/*
 * MMU_STATE attributes
 *
 */

/* V, WIMGE bits */
#define MMU_STATE_VALID                         0x80000000
#define MMU_STATE_VALID_NOT                     0x00000000
#define MMU_STATE_LOCK                          0x00800000
#define MMU_STATE_UNLOCK                        0x00000000
#define MMU_STATE_CACHEABLE_WRITETHROUGH        0x00000010
#define MMU_STATE_CACHEABLE_COPYBACK            0x00000000
#define MMU_STATE_CACHEABLE                     0x00000000
#define MMU_STATE_CACHEABLE_NOT                 0x00000008
#define MMU_STATE_MEM_COHERENCY                 0x00000004
#define MMU_STATE_MEM_COHERENCY_NOT             0x00000000
#define MMU_STATE_GUARDED                       0x00000002
#define MMU_STATE_GUARDED_NOT                   0x00000000
#define MMU_STATE_ENDIAN_BIG                    0x00000000
#define MMU_STATE_ENDIAN_LITTLE                 0x00000001

#define MMU_STATE_MASK_VALID                    0x80000000
#define MMU_STATE_MASK_LOCK                     0x00800000
#define MMU_STATE_MASK_WIMG                     0x0000001F
#define MMU_STATE_MASK_WRITETHROUGH             0x00000010
#define MMU_STATE_MASK_CACHEABLE                0x00000018
#define MMU_STATE_MASK_MEM_COHERENCY            0x00000004
#define MMU_STATE_MASK_GUARDED                  0x00000002
#define MMU_STATE_MASK_ENDIAN                   0x00000001

/* UX/SX, UW/SW, UR/SR bits */

/* drice - need to look at these values */
#define MMU_STATE_EXECUTE                       0x00003000
#define MMU_STATE_EXECUTE_NOT                   0x00000000
#define MMU_STATE_WRITABLE                      0x00000c00
#define MMU_STATE_WRITABLE_NOT                  0x00000000
#define MMU_STATE_MASK_EXECUTE                  0x00003000
#define MMU_STATE_MASK_WRITABLE                 0x00000c00
#define MMU_STATE_MASK_WRITABLE_EXECUTE         0x00003c00
#define MMU_STATE_MASK_WIMG_WRITABLE_EXECUTE    0x00000000
#define MMU_STATE_MASK_PROTECTION               0x00003f00
#define MMU_STATE_PROTECTION_BIT_SHIFT          8

/*
 * Protection Bits
 *
 * Each TLB entry controls access to one page of memory. The protection
 * mode is controlled by 6 bits in the TLB word, one for each of Read,
 * Write, Execute for Supervisor and User.
 *
 * These protection bits are mapped directly into MMU_STATE_* macros,
 * defined below:
 */

/* MMU_STATE_* protection mode sub-fields, mapping <<8 with hardware */

#define _MMU_UX                 0x00002000
#define _MMU_SX                 0x00001000
#define _MMU_UW                 0x00000800
#define _MMU_SW                 0x00000400
#define _MMU_UR                 0x00000200
#define _MMU_SR                 0x00000100

#define _MMU_URWX                  (_MMU_UR | _MMU_UW | _MMU_UX)

/* MMU_STATE_* protection modes macros */

#define MMU_STATE_SUP_R            (_MMU_SR)
#define MMU_STATE_SUP_RW           (_MMU_SR|_MMU_SW)
#define MMU_STATE_SUP_RX           (_MMU_SR|        _MMU_SX)
#define MMU_STATE_SUP_RWX          (_MMU_SR|_MMU_SW|_MMU_SX)

#define MMU_STATE_SUP_R_USR_R      (MMU_STATE_SUP_R   | _MMU_UR)
#define MMU_STATE_SUP_RW_USR_R     (MMU_STATE_SUP_RW  | _MMU_UR)
#define MMU_STATE_SUP_RX_USR_R     (MMU_STATE_SUP_RX  | _MMU_UR)
#define MMU_STATE_SUP_RWX_USR_R    (MMU_STATE_SUP_RWX | _MMU_UR)

#define MMU_STATE_SUP_R_USR_W      (MMU_STATE_SUP_R   | _MMU_UW)
#define MMU_STATE_SUP_RW_USR_W     (MMU_STATE_SUP_RW  | _MMU_UW)
#define MMU_STATE_SUP_RX_USR_W     (MMU_STATE_SUP_RX  | _MMU_UW)
#define MMU_STATE_SUP_RWX_USR_W    (MMU_STATE_SUP_RWX | _MMU_UW)

#define MMU_STATE_SUP_R_USR_RW     (MMU_STATE_SUP_R   | _MMU_UR | _MMU_UW)
#define MMU_STATE_SUP_RW_USR_RW    (MMU_STATE_SUP_RW  | _MMU_UR | _MMU_UW)
#define MMU_STATE_SUP_RX_USR_RW    (MMU_STATE_SUP_RX  | _MMU_UR | _MMU_UW)
#define MMU_STATE_SUP_RWX_USR_RW   (MMU_STATE_SUP_RWX | _MMU_UR | _MMU_UW)

#define MMU_STATE_SUP_R_USR_X      (MMU_STATE_SUP_R   | _MMU_UX)
#define MMU_STATE_SUP_RW_USR_X     (MMU_STATE_SUP_RW  | _MMU_UX)
#define MMU_STATE_SUP_RX_USR_X     (MMU_STATE_SUP_RX  | _MMU_UX)
#define MMU_STATE_SUP_RWX_USR_X    (MMU_STATE_SUP_RWX | _MMU_UX)

#define MMU_STATE_SUP_R_USR_RX     (MMU_STATE_SUP_R   | _MMU_UR | _MMU_UX)
#define MMU_STATE_SUP_RW_USR_RX    (MMU_STATE_SUP_RW  | _MMU_UR | _MMU_UX)
#define MMU_STATE_SUP_RX_USR_RX    (MMU_STATE_SUP_RX  | _MMU_UR | _MMU_UX)
#define MMU_STATE_SUP_RWX_USR_RX   (MMU_STATE_SUP_RWX | _MMU_UR | _MMU_UX)

#define MMU_STATE_SUP_R_USR_WX     (MMU_STATE_SUP_R   | _MMU_UW | _MMU_UX)
#define MMU_STATE_SUP_RW_USR_WX    (MMU_STATE_SUP_RW  | _MMU_UW | _MMU_UX)
#define MMU_STATE_SUP_RX_USR_WX    (MMU_STATE_SUP_RX  | _MMU_UW | _MMU_UX)
#define MMU_STATE_SUP_RWX_USR_WX   (MMU_STATE_SUP_RWX | _MMU_UW | _MMU_UX)

#define MMU_STATE_SUP_R_USR_RWX    (MMU_STATE_SUP_R   | _MMU_URWX)
#define MMU_STATE_SUP_RW_USR_RWX   (MMU_STATE_SUP_RW  | _MMU_URWX)
#define MMU_STATE_SUP_RX_USR_RWX   (MMU_STATE_SUP_RX  | _MMU_URWX)
#define MMU_STATE_SUP_RWX_USR_RWX  (MMU_STATE_SUP_RWX | _MMU_URWX)

/* RPN field offset in TLB Word 0 (left shift this number of bits) */

#define MMU_RPN_SHIFT           12

/* TLB_ENTRY_DESC page attributes */

#define _MMU_TLB_TS_0           0x00000000      /* Translation Space 0 */
#define _MMU_TLB_TS_1           0x00001000      /* Translation Space 1 */

#define TLB_GLOBAL_BIT  25      /* global page map bit is at bit 0 of word 2 */
#define TLB_LOCK_BIT    26      /* lock page map bit is at bit 1 of word 2 */

#define _MMU_TLB_SZ_4K          0x00000100      /* 4KB page size */
#define _MMU_TLB_SZ_16K         0x00000200      /* 16KB page size */
#define _MMU_TLB_SZ_64K         0x00000300      /* 64KB page size */
#define _MMU_TLB_SZ_256K        0x00000400      /* 256KB page size */
#define _MMU_TLB_SZ_1M          0x00000500      /* 1MB page size */
#define _MMU_TLB_SZ_4M          0x00000600      /* 4MB page size */
#define _MMU_TLB_SZ_16M         0x00000700      /* 16MB page size */
#define _MMU_TLB_SZ_64M         0x00000800      /* 64MB page size */
#define _MMU_TLB_SZ_256M        0x00000900      /* 256MB page size */
#define _MMU_TLB_SZ_1G          0x00000a00      /* 1GB page size (e500v2/mc) */
#define _MMU_TLB_SZ_4G          0x00000b00      /* 4GB page size (e500v2/mc) */
#define _MMU_TLB_SZ_MASK        0x00000f00      /* mask for page size field */
#define _MMU_TLB_SZ_SHIFT       8               /* offset of page size field */

#define _MMU_TLB_ATTR_W         0x00000010      /* writethrough */
#define _MMU_TLB_ATTR_I         0x00000008      /* inhibit caching */
#define _MMU_TLB_ATTR_M         0x00000004      /* memory coherency */
#define _MMU_TLB_ATTR_G         0x00000002      /* guarded */
#define _MMU_TLB_ATTR_E         0x00000001      /* endianess */

#define _MMU_TLB_PERM_X         0x00000020      /* executable */
#define _MMU_TLB_PERM_W         0x00000040      /* writable */
#define _MMU_TLB_IPROT          0x00000080      /* invalidate protection */
#define _MMU_TLB_PERM_UR        0x00010000      /* user read */
#define _MMU_TLB_PERM_UW        0x00020000      /* user write */
#define _MMU_TLB_PERM_UX        0x00040000      /* user execute */

#define _MMU_TLB_EXRPN_MASK     0x0000000f      /* extend real page number mask */

/* used to encode/decode size field of TLB/Page entry */

/* multiple for TLB size */

#define _MMU_TLB_SZ_MULT        4
#define _MMU_TLB_SZ_MULT_SHIFT  2

/* TLB size */

#define _MMU_TLB_SZ_MIN_SHIFT          12
#define N_TLB_ENTRIES_BITS             8        /* # TLB0: read TLB0CFG */
#define N_TLB_ENTRIES                  256      /* # TLB0: read TLB0CFG */
#define MMUE500_NUM_STATIC_TLB_ENTIRES 0x10

#ifndef _ASMLANGUAGE
/* TLB Entry description, used for mmu initialization */

typedef struct {
    UINT32      effAddr;        /* Effective address of base of page */
    UINT32      realAddrExt;    /* top 4 bits of real address */
    UINT32      realAddr;       /* rest of real address */
    UINT32      attr;           /* page attributes */
} TLB_ENTRY_DESC;

typedef union e500_pte { /* pte format for e6500/e500mc/e500v2/e200 */
    struct {                    /* Bit field description */
        /* pte format for e500mc/e500v2/e200 */

        /* word 0 */

        UINT32 v:1;             /* valid bit */
        UINT32 iprot:1;         /* invalidate protect bit */
        UINT32 rsvd1:6;         /* reserved */
        UINT32 tid:8;           /* translation ID */
        UINT32 rsvd2:3;         /* reserved */
        UINT32 ts:1;            /* translation space */
        UINT32 tsize:4;         /* translation size */
        UINT32 rsvd3:8;         /* reserved */

        /* word 1 */

        UINT32 epn:20;          /* effective page number */
        UINT32 rsvd4:2;         /* reserved */
        UINT32 sharen:1;        /* enables cache fills to use shared cache state */
        UINT32 rsvd5:2;         /* reserved */
        UINT32 global:1;        /* global bit for internal use */

        UINT32 lock:1;         /* lock bit label indicating static TLB used */

        UINT32 w:1;             /* write thru/back */
        UINT32 i:1;             /* cache inhibited */
        UINT32 m:1;             /* memory coherent */
        UINT32 g:1;             /* memory guarded  */
        UINT32 e:1;             /* little endian bit */

        /* word 2 */

        UINT32 rpn:20;          /* real page number */
        UINT32 rsvd6:2;         /* reserved */
        UINT32 u0:1;            /* user attribute 0 */
        UINT32 u1:1;            /* user attribute 1 */
        UINT32 u2:1;            /* user attribute 2 */
        UINT32 u3:1;            /* user attribute 3 */
        UINT32 ux:1;            /* user execute protection */
        UINT32 sx:1;            /* supervisor execute protection */
        UINT32 uw:1;            /* user write protection */
        UINT32 sw:1;            /* supervisor write protection */
        UINT32 ur:1;            /* user read protection */
        UINT32 sr:1;            /* supervisor read protection */

        /* word3 */

        UINT32 rsvd7:28;        /* reserved*/
        UINT32 erpn:4;          /* extended real page number*/

    } field;

    struct {
        UINT32 word0;           /* word 0 */
        UINT32 word1;           /* word 1 */
        UINT32 word2;           /* word 2 */
        UINT32 word3;           /* word3 */
    } words;

} MMUE500_PTE;

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* __INCmmuPpcE500Libh */
