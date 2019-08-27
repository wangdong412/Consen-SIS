/* sysAccess.S - accessor routines for PowerPC architecture  */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  arch access code for PPC85XX.
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
 * This file contains the makefile rules for building cagBL
 */

#define _ASMLANGUAGE
#include <ppc/toolPpc.h>
#include <ppc/archPpc.h>
#include <ppc/mmuE500Lib.h>

	/* globals */

FUNC_EXPORT(readb)
FUNC_EXPORT(readw)
FUNC_EXPORT(readl)
FUNC_EXPORT(writeb)
FUNC_EXPORT(writew)
FUNC_EXPORT(writel)
FUNC_EXPORT(readb_noOrder)
FUNC_EXPORT(readw_noOrder)
FUNC_EXPORT(readl_noOrder)
FUNC_EXPORT(writeb_noOrder)
FUNC_EXPORT(writew_noOrder)
FUNC_EXPORT(writel_noOrder)

    _CAG_TEXT_SEG_START

/*******************************************************************************
*
* readb - read 1 byte of data from 'addr'
*
* This routine reads 1 byte of data from 'addr'. This routine guarantees
* that read operation is finished before it returns.
*
* RETURNS: value read
*
* ERRNO: N/A
*
* \NOMANUAL
*/

FUNC_BEGIN(readb)
	lbzx	r3, r0, r3
	eieio
	blr
FUNC_END(readb)

/*******************************************************************************
*
* readw - read w bytes of data from 'addr'
*
* This routine reads w bytes of data from 'addr'. This routine guarantees
* that read operation is finished before it returns.
*
* RETURNS: value read
*
* ERRNO: N/A
*
* \NOMANUAL
*/

FUNC_BEGIN(readw)
	lhzx	r3, r0, r3
	eieio
	blr
FUNC_END(readw)

/*******************************************************************************
*
* readl - read 4 bytes of data from 'addr'
*
* This routine reads 4 bytes of data from 'addr'. This routine guarantees
* that read operation is finished before it returns.
*
* RETURNS: value read
*
* ERRNO: N/A
*
* \NOMANUAL
*/

FUNC_BEGIN(readl)
	lwzx	r3, r0, r3
	eieio
	blr
FUNC_END(readl)

/*******************************************************************************
*
* writeb - write 1 byte of 'val' to 'addr'
*
* This routine writes 1 byte of 'val' to 'addr'. This routine guarantees
* that write operation is finished before it returns.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

FUNC_BEGIN(writeb)
	stbx	r4, r0, r3
	eieio
	blr
FUNC_END(writeb)

/*******************************************************************************
*
* writew - write 2 bytes of 'val' to 'addr'
*
* This routine writes 1 bytes of 'val' to 'addr'. This routine guarantees
* that write operation is finished before it returns.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

FUNC_BEGIN(writew)
	sthx	r4, r0, r3
	eieio
	blr
FUNC_END(writew)

/*******************************************************************************
*
* writel - write 4 bytes of 'val' to 'addr'
*
* This routine writes 4 bytes of 'val' to 'addr'. This routine guarantees
* that write operation is finished before it returns.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

FUNC_BEGIN(writel)
	stwx	r4, r0, r3
	eieio
	blr
FUNC_END(writel)

/*******************************************************************************
*
* readb_noOrder - read 1 byte of data from 'addr'
*
* This routine reads 1 byte of data from 'addr'.
*
* RETURNS: value read
*
* ERRNO: N/A
*
* \NOMANUAL
*/

FUNC_BEGIN(readb_noOrder)
	lbzx	r3, r0, r3
	blr
FUNC_END(readb_noOrder)

/*******************************************************************************
*
* readw_noOrder - read w bytes of data from 'addr'
*
* This routine reads w bytes of data from 'addr'.
*
* RETURNS: value read
*
* ERRNO: N/A
*
* \NOMANUAL
*/

FUNC_BEGIN(readw_noOrder)
	lhzx	r3, r0, r3
	blr
FUNC_END(readw_noOrder)

/*******************************************************************************
*
* readl_noOrder - read 4 bytes of data from 'addr'
*
* This routine reads 4 bytes of data from 'addr'.
*
* RETURNS: value read
*
* ERRNO: N/A
*
* \NOMANUAL
*/

FUNC_BEGIN(readl_noOrder)
	lwzx	r3, r0, r3
	blr
FUNC_END(readl_noOrder)

/*******************************************************************************
*
* writeb_noOrder - write 1 byte of 'val' to 'addr'
*
* This routine writes 1 byte of 'val' to 'addr'.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

FUNC_BEGIN(writeb_noOrder)
	stbx	r4, r0, r3
	blr
FUNC_END(writeb_noOrder)

/*******************************************************************************
*
* writew_noOrder - write 2 bytes of 'val' to 'addr'
*
* This routine writes 2 bytes of 'val' to 'addr'.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

FUNC_BEGIN(writew_noOrder)
	sthx	r4, r0, r3
	blr
FUNC_END(writew_noOrder)

/*******************************************************************************
*
* writel_noOrder - write 4 bytes of 'val' to 'addr'
*
* This routine writes 4 bytes of 'val' to 'addr'.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

FUNC_BEGIN(writel_noOrder)
	stwx	r4, r0, r3
	blr
FUNC_END(writel_noOrder)
