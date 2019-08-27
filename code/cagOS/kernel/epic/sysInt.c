/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  platform IRQ code for p1020rdb.
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
 * This file contains the board IRQ component
 */
#include <config.h>
#include <p1020rdb.h>
#include <libc/cagboot.h>
#include <kernel/sysPic.h>
#include <kernel/sysOs.h>
#include <ppc/toolPpc.h>
#include <libc/stddef.h>
#include <libc/stdio.h>
#include <libc/string.h>

typedef void (*irqHandler)(UINT32, void *);
IMPORT UINT32 * sysEpicIntAckReg;		/* int ack reg adrs */
IMPORT UINT16 sysEpicSpuriousVector;		/* Superious Vector Number */

#define IRQ_MAX_NUM  (EPIC_EX_IRQ_NUM + EPIC_IN_IRQ_NUM)

typedef struct irqDescriptor {
    irqHandler handler;
    UINT32 irqnum;           //中断号==中断向量
    UINT32 flags;
    UINT32 count;
    void * pData;
    char name[32];
} irqDesNode;

irqDesNode irqDeseTable[IRQ_MAX_NUM];

void irqInit()
{
    memset(irqDeseTable,0,sizeof(irqDeseTable));
}

int irqUnregister(unsigned int irqNum)
{
    signed int ret=0;

    if(irqNum>=IRQ_MAX_NUM) {
        ret=-1;
        printf("irqNum is beyond the max num :%s\n",__FUNCTION__);

    } else {
        OS_CRITICAL_ENTER();
        if(irqDeseTable[irqNum].handler!=NULL) {
            irqDeseTable[irqNum].handler=(irqHandler)NULL;
            irqDeseTable[irqNum].pData=NULL;
            memset(irqDeseTable[irqNum].name,sizeof(irqDeseTable[irqNum].name),0);
            irqDeseTable[irqNum].flags=0;
            irqDeseTable[irqNum].irqnum=0xffffffff;
        }
        OS_CRITICAL_EXIT();
    }
    return ret;
}

int irqRregister(unsigned int irqNum,irqHandler handler,void * private_data,const char * name)
{
    signed int ret=0;

    if(name == NULL) {
        ret = -1;
        printf("the int handler name is not defined,%s\n",__FUNCTION__);
        return ret;
    }

    if(irqNum>=IRQ_MAX_NUM) {
        ret=-1;
        printf("irqNum is beyond the max num :%s\n",__FUNCTION__);
    } else {
        OS_CRITICAL_ENTER();
        if(NULL==irqDeseTable[irqNum].handler) {
            irqDeseTable[irqNum].handler=handler;
            irqDeseTable[irqNum].pData=private_data;
            strncpy(irqDeseTable[irqNum].name,name,32);
            irqDeseTable[irqNum].flags=0;
            irqDeseTable[irqNum].irqnum=irqNum;
        } else {
            ret=-1;
        }
        OS_CRITICAL_EXIT();
    }
    return ret;
}

/*******************************************************************************
*
* OSIntHandler - handles the EPIC interrupts to the CPU
*
* This routine handles interrupts originating from the embedded interrupt
* controller on the MPC8540 PowerPC processor.
* This handler is entered from the 0x500 exception.
*
* This routine is entered with CPU external interrupts enables.
*
* Since the EPIC is the primary interrupt controller this driver
* first initiates an Epic acknowledge call and reads the vector
* put out by the EPIC. Subsequent vectors have to be obtained if
* an external interrupt controller is connected to one of the
* sysEpic handlers.
*
* This routine then processes the interrupt by calling all the interrupt
* service routines chained to the vector.
*
* Finally, this routine re-arms the interrupt at the PIC by performing an
* PIC EOI for both the EPIC and the 8259s.
*
* RETURNS:  N/A
*
* ERRNO: N/A
*
* \INTERNAL
* Changes to this routine should be mirrored in sysSysEpicIntHandlerSV(), which
* should be almost identical apart from the System Viewer instrumentation.
*
*/

void OSIntHandler (void)
{
    UINT32	vector = sysEpicSpuriousVector;
    UINT32  sysEpicIntVecCheck = sysEpicSpuriousVector;

    /* loop till all interrupts are cleared */

    while (TRUE) {
        /* clear int, return the vec for highest IRQ */

        vector = EPIC_REG_NO_OFFSET_READ(sysEpicIntAckReg) & 0xffff;
        if (vector == sysEpicIntVecCheck)
            break;

        ++irqDeseTable[vector].count;
        if(irqDeseTable[vector].handler != NULL) {
            (irqDeseTable[vector].handler)(vector,irqDeseTable[vector].pData);
        }

        EPIC_REG_NO_OFFSET_WRITE((UINT32)sysEpicIntAckReg +
                                 (EPIC_EOI_REG - EPIC_INT_ACK_REG), 0x0);
    } /* while */
}
