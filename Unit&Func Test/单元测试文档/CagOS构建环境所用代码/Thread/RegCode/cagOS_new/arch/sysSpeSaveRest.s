/* sysSpeSaveRest.s - Power PC save and restore EABI functions for SPE APU */

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

#define _ASMLANGUAGE
#include "vxWorks.h"
#include "asm.h"

		.data
		.globl __speSaveRest_o
__speSaveRest_o:
		.long 0x0

	FUNC_EXPORT(_save64gpr_14)
	FUNC_EXPORT(_save64gpr_15)
	FUNC_EXPORT(_save64gpr_16)
	FUNC_EXPORT(_save64gpr_17)
	FUNC_EXPORT(_save64gpr_18)
	FUNC_EXPORT(_save64gpr_19)
	FUNC_EXPORT(_save64gpr_20)
	FUNC_EXPORT(_save64gpr_21)
	FUNC_EXPORT(_save64gpr_22)
	FUNC_EXPORT(_save64gpr_23)
	FUNC_EXPORT(_save64gpr_24)
	FUNC_EXPORT(_save64gpr_25)
	FUNC_EXPORT(_save64gpr_26)
	FUNC_EXPORT(_save64gpr_27)
	FUNC_EXPORT(_save64gpr_28)
	FUNC_EXPORT(_save64gpr_29)
	FUNC_EXPORT(_save64gpr_30)
	FUNC_EXPORT(_save64gpr_31)
	FUNC_EXPORT(_rest64gpr_14)
	FUNC_EXPORT(_rest64gpr_15)
	FUNC_EXPORT(_rest64gpr_16)
	FUNC_EXPORT(_rest64gpr_17)
	FUNC_EXPORT(_rest64gpr_18)
	FUNC_EXPORT(_rest64gpr_19)
	FUNC_EXPORT(_rest64gpr_20)
	FUNC_EXPORT(_rest64gpr_21)
	FUNC_EXPORT(_rest64gpr_22)
	FUNC_EXPORT(_rest64gpr_23)
	FUNC_EXPORT(_rest64gpr_24)
	FUNC_EXPORT(_rest64gpr_25)
	FUNC_EXPORT(_rest64gpr_26)
	FUNC_EXPORT(_rest64gpr_27)
	FUNC_EXPORT(_rest64gpr_28)
	FUNC_EXPORT(_rest64gpr_29)
	FUNC_EXPORT(_rest64gpr_30)
	FUNC_EXPORT(_rest64gpr_31)
	FUNC_EXPORT(_save64gpr_14_l)
	FUNC_EXPORT(_save64gpr_15_l)
	FUNC_EXPORT(_save64gpr_16_l)
	FUNC_EXPORT(_save64gpr_17_l)
	FUNC_EXPORT(_save64gpr_18_l)
	FUNC_EXPORT(_save64gpr_19_l)
	FUNC_EXPORT(_save64gpr_20_l)
	FUNC_EXPORT(_save64gpr_21_l)
	FUNC_EXPORT(_save64gpr_22_l)
	FUNC_EXPORT(_save64gpr_23_l)
	FUNC_EXPORT(_save64gpr_24_l)
	FUNC_EXPORT(_save64gpr_25_l)
	FUNC_EXPORT(_save64gpr_26_l)
	FUNC_EXPORT(_save64gpr_27_l)
	FUNC_EXPORT(_save64gpr_28_l)
	FUNC_EXPORT(_save64gpr_29_l)
	FUNC_EXPORT(_save64gpr_30_l)
	FUNC_EXPORT(_save64gpr_31_l)
	FUNC_EXPORT(_rest64gpr_14_l)
	FUNC_EXPORT(_rest64gpr_15_l)
	FUNC_EXPORT(_rest64gpr_16_l)
	FUNC_EXPORT(_rest64gpr_17_l)
	FUNC_EXPORT(_rest64gpr_18_l)
	FUNC_EXPORT(_rest64gpr_19_l)
	FUNC_EXPORT(_rest64gpr_20_l)
	FUNC_EXPORT(_rest64gpr_21_l)
	FUNC_EXPORT(_rest64gpr_22_l)
	FUNC_EXPORT(_rest64gpr_23_l)
	FUNC_EXPORT(_rest64gpr_24_l)
	FUNC_EXPORT(_rest64gpr_25_l)
	FUNC_EXPORT(_rest64gpr_26_l)
	FUNC_EXPORT(_rest64gpr_27_l)
	FUNC_EXPORT(_rest64gpr_28_l)
	FUNC_EXPORT(_rest64gpr_29_l)
	FUNC_EXPORT(_rest64gpr_30_l)
	FUNC_EXPORT(_rest64gpr_31_l)

	_WRS_TEXT_SEG_START


_save64gpr_14:	evstdd	r14,0(r11)
_save64gpr_15:	evstdd	r15,8(r11)
_save64gpr_16:	evstdd	r16,16(r11)
_save64gpr_17:	evstdd	r17,24(r11)
_save64gpr_18:	evstdd	r18,32(r11)
_save64gpr_19:	evstdd	r19,40(r11)
_save64gpr_20:	evstdd	r20,48(r11)
_save64gpr_21:	evstdd	r21,56(r11)
_save64gpr_22:	evstdd	r22,64(r11)
_save64gpr_23:	evstdd	r23,72(r11)
_save64gpr_24:	evstdd	r24,80(r11)
_save64gpr_25:	evstdd	r25,88(r11)
_save64gpr_26:	evstdd	r26,96(r11)
_save64gpr_27:	evstdd	r27,104(r11)
_save64gpr_28:	evstdd	r28,112(r11)
_save64gpr_29:	evstdd	r29,120(r11)
_save64gpr_30:	evstdd	r30,128(r11)
_save64gpr_31:	evstdd	r31,136(r11)
	blr

_save64gpr_14_l:	evstdd	r14,0(r11)
_save64gpr_15_l:	evstdd	r15,8(r11)
_save64gpr_16_l:	evstdd	r16,16(r11)
_save64gpr_17_l:	evstdd	r17,24(r11)
_save64gpr_18_l:	evstdd	r18,32(r11)
_save64gpr_19_l:	evstdd	r19,40(r11)
_save64gpr_20_l:	evstdd	r20,48(r11)
_save64gpr_21_l:	evstdd	r21,56(r11)
_save64gpr_22_l:	evstdd	r22,64(r11)
_save64gpr_23_l:	evstdd	r23,72(r11)
_save64gpr_24_l:	evstdd	r24,80(r11)
_save64gpr_25_l:	evstdd	r25,88(r11)
_save64gpr_26_l:	evstdd	r26,96(r11)
_save64gpr_27_l:	evstdd	r27,104(r11)
_save64gpr_28_l:	evstdd	r28,112(r11)
_save64gpr_29_l:	evstdd	r29,120(r11)
_save64gpr_30_l:	evstdd	r30,128(r11)
_save64gpr_31_l:	evstdd	r31,136(r11)
	stw	r0,148(r11)
	blr

_rest64gpr_14:	evldd	r14,0(r11)
_rest64gpr_15:	evldd	r15,8(r11)
_rest64gpr_16:	evldd	r16,16(r11)
_rest64gpr_17:	evldd	r17,24(r11)
_rest64gpr_18:	evldd	r18,32(r11)
_rest64gpr_19:	evldd	r19,40(r11)
_rest64gpr_20:	evldd	r20,48(r11)
_rest64gpr_21:	evldd	r21,56(r11)
_rest64gpr_22:	evldd	r22,64(r11)
_rest64gpr_23:	evldd	r23,72(r11)
_rest64gpr_24:	evldd	r24,80(r11)
_rest64gpr_25:	evldd	r25,88(r11)
_rest64gpr_26:	evldd	r26,96(r11)
_rest64gpr_27:	evldd	r27,104(r11)
_rest64gpr_28:	evldd	r28,112(r11)
_rest64gpr_29:	evldd	r29,120(r11)
_rest64gpr_30:	evldd	r30,128(r11)
_rest64gpr_31:	evldd	r31,136(r11)
	blr

_rest64gpr_14_l:	evldd	r14,0(r11)
_rest64gpr_15_l:	evldd	r15,8(r11)
_rest64gpr_16_l:	evldd	r16,16(r11)
_rest64gpr_17_l:	evldd	r17,24(r11)
_rest64gpr_18_l:	evldd	r18,32(r11)
_rest64gpr_19_l:	evldd	r19,40(r11)
_rest64gpr_20_l:	evldd	r20,48(r11)
_rest64gpr_21_l:	evldd	r21,56(r11)
_rest64gpr_22_l:	evldd	r22,64(r11)
_rest64gpr_23_l:	evldd	r23,72(r11)
_rest64gpr_24_l:	evldd	r24,80(r11)
_rest64gpr_25_l:	evldd	r25,88(r11)
_rest64gpr_26_l:	evldd	r26,96(r11)
_rest64gpr_27_l:	evldd	r27,104(r11)
_rest64gpr_28_l:	evldd	r28,112(r11)
_rest64gpr_29_l:	evldd	r29,120(r11)
_rest64gpr_30_l:	evldd	r30,128(r11)
_rest64gpr_31_l:	evldd	r31,136(r11)
    lwz     r0,148(r11)
    mtspr   8,r0
    addi	r1,r11,144
    blr