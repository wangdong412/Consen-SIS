/* sysAtomicALib.S  - atomic assembly routines */

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
 * This file contains SPE management.
 */

/* includes */

#define _ASMLANGUAGE
#include <config.h>
#include <libc/cagboot.h>
#include <ppc/toolPpc.h>
#include <ppc/archPpc.h>
#include <ppc/asmPpc.h>
#include <ppc/ppc85xx.h>

/*
DESCRIPTION
This library provides routines to perform a number of atomic operations
on a memory location: add, subtract, increment, decrement, bitwise OR,
bitwise NOR, bitwise AND, bitwise NAND, set, clear and compare-and-swap.
*/

#define _ASMLANGUAGE

	FUNC_EXPORT(atomic32Set)
	FUNC_EXPORT(atomic32Get)
	FUNC_EXPORT(atomic32Add)
	FUNC_EXPORT(atomic32And)
	FUNC_EXPORT(atomic32Cas)
	FUNC_EXPORT(atomic32Or)
	FUNC_EXPORT(atomic32Xor)
	FUNC_EXPORT(atomic32Clear)
	FUNC_EXPORT(atomic32Sub)

/* 
 * For UP, _WRS_PORTABLE_vxAtomicLib is defined, i.e. use portable, 
 * to support cache being disabled. For SMP, enabling the cache is
 * a requirement.
 *
 * This file is built under the following three conditions:
 *   kernel UP  - only the atomicXxx set of API is exported.
 *                _WRS_PORTABLE_vxAtomicLib is defined.
 *   kernel SMP - both the atomicXxx and vxAtomicXxx API sets are
 *                exported. The two sets have identical implementation.
 *                _WRS_PORTABLE_vxAtomicLib is not defined.
 *   user mode  - both the atomicXxx and vxAtomicXxx API are exported.
 *                Only the vxAtomicXxx set checks the status of 
 *                vxAtomicOpOptimized. _WRS_PORTABLE_vxAtomicLib is 
 *                not defined.
 *
 * Note that the kernel mode UP implementation of atomicXxx API duplicates
 * code. Fixes in one set should be applied to the other set, if applicable.
 */


/*******************************************************************************
*
* atomic32Clear - atomically clear a memory location
*
* This routine atomically clears the contents of <target> and returns the old
* value that was in <target>.  Note that all CPU architectures supported by
* VxWorks can atomically clear a variable of size atomic32_t
* without the need to use this routine.  This routine is intended for software
* that needs to atomically fetch and clear the value of a memory location.
* Various CPU architectures may impose restrictions with regards to the
* alignment and cache attributes of the atomic_t type.
*
* This routine can be used from both task and interrupt level.
*
* RETURNS: Contents of <target> before the atomic operation
*
* ERRNO: N/A
*
* atomicVal32_t atomicClear
*    (
*    atomic32_t *target	/@ memory location to clear @/
*    )
*/

FUNC_BEGIN(atomic32Clear)
	li	p1, 0
	/* fall through into atomic32Set */

/*******************************************************************************
*
* atomic32Set - atomically set a memory location
*
* This routine atomically sets the contents of <target> to <value> and returns
* the old value that was in <target>.
* Note that all CPU architectures supported by VxWorks can atomically write to
* a variable of size atomic32_t without the need to use this routine.  This
* routine is intended for software that needs to atomically fetch and replace
* the value of a memory location. Various CPU architectures may impose
* restrictions with regards to the alignment and cache attributes of the
* atomic32_t type.
*
* This routine can be used from both task and interrupt level.
*
* RETURNS: Contents of <target> before the atomic operation
*
* ERRNO: N/A
*
* atomicVal32_t atomic32Set
*    (
*    atomic32_t *target,	/@ memory location to set @/
*    atomicVal32_t value	/@ set with this value @/
*    )
*
*/

FUNC_BEGIN(atomic32Set)
	lwarx   p2, 0, p0      /* load and reserve */
	stwcx.  p1, 0, p0      /* store new value if still reserved */
	bne-    atomic32Set      /* loop if lost reservation */
	mr      p0, p2         /* return old value */
	blr
FUNC_END(atomic32Set)
FUNC_END(atomic32Clear)

/******************************************************************************
*
* atomic32Get - Get the value of a shared memory atomically
*
* This routine atomically retrieves the value in *target
*
* atomicVal32_t atomic32Get
*     (
*     atomic32_t * target    /@ address of atom to be retrieved @/
*     )
*
* RETURN: value read from address target.
*
*/

FUNC_BEGIN(atomic32Get)
	lwz	p0, 0(p0)	/* no need for reservation */
	blr
FUNC_END(atomic32Get)

/*******************************************************************************
*
* atomic32Add - atomically add a value to a memory location
*
* This routine atomically adds the contents of <target> and <value>, placing
* the result in <target>. The operation is done using signed integer arithmetic.
* Various CPU architectures may impose restrictions with regards to the
* alignment and cache attributes of the atomic32_t type.
*
* This routine can be used from both task and interrupt level.
*
* RETURNS: Contents of <target> before the atomic operation
*
* ERRNO: N/A
*
* atomicVal32_t atomic32Add
*    (
*    atomic32_t *target,	/@ memory location to add to @/
*    atomicVal32_t value	/@ value to add @/
*    )
*/

FUNC_BEGIN(atomic32Add)
	lwarx	p2, 0, p0      /* load and reserve */
	add	p3, p1, p2     /* add word */
	stwcx.	p3, 0, p0      /* store new value if still reserved */
	bne- 	atomic32Add      /* loop if lost reservation */
	mr      p0, p2         /* return old value */
	blr
FUNC_END(atomic32Add)

/*******************************************************************************
*
* atomic32Sub - atomically subtract a value from a memory location
*
* This routine atomically subtracts <value> from the contents of <target>,
* placing the result in <target>.  The operation is done using signed integer
* arithmetic. Various CPU architectures may impose restrictions with regards to
* the alignment and cache attributes of the atomic32_t type.
*
* This routine can be used from both task and interrupt level.
*
* RETURNS: Contents of <target> before the atomic operation
*
* ERRNO: N/A
*
* atomicVal32_t atomic32Sub
*    (
*    atomic32_t *target,	/@ memory location to subtract from @/
*    atomicVal32_t value	/@ value to subtract @/
*    )
*
*/

FUNC_BEGIN(atomic32Sub)
	lwarx	p2, 0, p0       /* load and reserve */
	subf	p3, p1, p2      /* subtract word */
	stwcx.	p3, 0, p0       /* store new value if still reserved */
	bne- 	atomic32Sub       /* loop if lost reservation */
	mr      p0, p2          /* return old value */
	blr
FUNC_END(atomic32Sub)

/*******************************************************************************
*
* atomic32And - atomically perform a bitwise AND on a memory location
*
* This routine atomically performs a bitwise AND operation of the contents of
* <target> and <value>, placing the result in <target>.
* Various CPU architectures may impose restrictions with regards to the
* alignment and cache attributes of the atomic32_t type.
*
* This routine can be used from both task and interrupt level.
*
* RETURNS: Contents of <target> before the atomic operation
*
* ERRNO: N/A
*
* atomicVal32_t atomic32And
*    (
*    atomic32_t *target,	/@ memory location to AND @/
*    atomicVal32_t value	/@ AND with this value @/
*    )
*
*/

FUNC_BEGIN(atomic32And)
	lwarx   p2, 0, p0       /* load and reserve */
	and     p3, p1, p2      /* AND word */
	stwcx.  p3, 0, p0       /* store new value if still reserved */
	bne-    atomic32And       /* loop if lost reservation */
	mr      p0, p2          /* return old value */
	blr
FUNC_END (atomic32And)

/*******************************************************************************
*
* atomic32Or - atomically perform a bitwise OR on memory location
*
* This routine atomically performs a bitwise OR operation of the contents of
* <target> and <value>, placing the result in <target>.
* Various CPU architectures may impose restrictions with regards to the
* alignment and cache attributes of the atomic32_t type.
*
* This routine can be used from both task and interrupt level.
*
* RETURNS: Contents of <target> before the atomic operation
*
* ERRNO: N/A
*
* atomicVal32_t atomic32Or
*    (
*    atomic32_t *target,	/@ memory location to OR @/
*    atomicVal32_t value	/@ OR with this value @/
*    )
*
*/

FUNC_BEGIN(atomic32Or)
	lwarx   p2, 0, p0       /* load and reserve */
	or      p3, p1, p2      /* OR word */
	stwcx.  p3, 0, p0       /* store new value if still reserved */
	bne-    atomic32Or        /* loop if lost reservation */
	mr      p0, p2          /* return old value */
	blr
FUNC_END (atomic32Or)

/*******************************************************************************
*
* atomic32Xor - atomically perform a bitwise XOR on a memory location
*
* This routine atomically performs a bitwise XOR operation of the contents of
* <target> and <value>, placing the result in <target>.
* Various CPU architectures may impose restrictions with regards to the
* alignment and cache attributes of the atomic32_t type.
*
* This routine can be used from both task and interrupt level.
*
* RETURNS: Contents of <target> before the atomic operation
*
* ERRNO: N/A
*
* atomicVal32_t atomic32Xor
*    (
*    atomic32_t *target,	/@ memory location to XOR @/
*    atomicVal32_t value	/@ XOR with this value @/
*    )
*
*/

FUNC_BEGIN(atomic32Xor)
	lwarx   p2, 0, p0       /* load and reserve */
	xor     p3, p1, p2      /* XOR word */
	stwcx.  p3, 0, p0       /* store new value if still reserved */
	bne-    atomic32Xor       /* loop if lost reservation */
	mr      p0, p2          /* return old value */
	blr
FUNC_END (atomic32Xor)

/*******************************************************************************
*
* atomic32Cas - atomically compare-and-swap the contents of a memory location
*
* This routine performs an atomic compare-and-swap. testing that the contents of
* <target> contains <oldValue>, and if it does, setting the value of <target>
* to <newValue>. Various CPU architectures may impose restrictions with regards
* to the alignment and cache attributes of the atomic32_t type.
*
* This routine can be used from both task and interrupt level.
*
* RETURNS: TRUE if the swap is actually executed, FALSE otherwise.
*
* ERRNO: N/A
*
* BOOL atomic32Cas
*    (
*    atomic32_t *target,	        /@ memory location to compare-and-swap @/
*    atomic32Val_t oldValue,	/@ compare to this value @/
*    atomic32Val_t newValue,	/@ swap with this value @/
*    )
*
*/

FUNC_BEGIN(atomic32Cas)
	lwarx	p3, 0, p0       /* load and reserve */
	cmpw	p3, p1          /* operands equal */
	bne-	atomicCasErr    /* skip if not */
	stwcx.	p2, 0, p0       /* store new value if still reserved */
	bne-	atomic32Cas     /* loop if lost reservation */
	li	p0, 1           	/* cmp and swap successful, return 1 */
	blr
atomicCasErr:
	li	p0, 0           /*cmp and swap failed, return 0 */
	blr
FUNC_END(atomic32Cas)
