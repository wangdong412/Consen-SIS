/* access.h - accessor routines */

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

#ifndef __INCaccessh__
#define __INCaccessh__

#include <libc/cagboot.h>
#include <libc/stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CAG_CONFIG_ARCH_HAS_MEM_ACCESSORS
__inline__ UINT32 readl(void* addr)
{
    return *(volatile UINT32 *)(addr);
}
__inline__ UINT16 readw(void* addr)
{
    return *(volatile UINT16 *)(addr);
}
__inline__ UINT8 readb(void* addr)
{
    return *(volatile UINT8 *)(addr);
}
__inline__ void writel(void* addr, UINT32 val)
{
    *(volatile UINT32 *)(addr) = val;
}
__inline__ void writew(void* addr, UINT16 val)
{
    *(volatile UINT16 *)(addr) = val;
}
__inline__ void writeb(void* addr, UINT8 val)
{
    *(volatile UINT8 *)(addr) = val;
}

#define readl_noOrder readl
#define readw_noOrder readw
#define readn_noOrder readb
#define writel_noOrder writel
#define writew_noOrder writew
#define writeb_noOrder writeb
#else
extern UINT32 readl(void* addr);
extern UINT16 readw(void* addr);
extern UINT8 readb(void* addr);
extern void writel(void* addr, UINT32 val);
extern void writew(void* addr, UINT16 val);
extern void writeb(void* addr, UINT8 val);

extern UINT32 readl_noOrder(void* addr);
extern UINT16 readw_noOrder(void* addr);
extern UINT8 readb_noOrder(void* addr);
extern void writel_noOrder(void* addr, UINT32 val);
extern void writew_noOrder(void* addr, UINT16 val);
extern void writeb_noOrder(void* addr, UINT8 val);
#endif

#ifndef _CAG_CONFIG_ARCH_HAS_IO_ACCESSORS
#define inl readl
#define inw readw
#define inb readb
#define outl writel
#define outw writew
#define outb writeb
#else
extern UINT32 inl(UINT16 addr);
extern UINT16 inw(UINT16 addr);
extern UINT8 inb(UINT16 addr);

extern void outl(UINT16 addr, UINT32 val);
extern void outw(UINT16 addr, UINT16 val);
extern void outb(UINT16 addr, UINT8 val);
#endif

#ifdef __cplusplus
}
#endif

#endif

