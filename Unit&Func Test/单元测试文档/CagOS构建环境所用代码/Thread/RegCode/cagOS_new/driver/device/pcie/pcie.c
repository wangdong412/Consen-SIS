/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal: PCIE configure
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
 * 03jun15,ezio created
 *
 * DESCRIPTION
 * This file contains the board PCIE component
 */

#include <libc/cagboot.h>
#include <libc/stddef.h>
#include <libc/string.h>
#include <ppc/io.h>
#include "../dfs/dfs.h"
#include "pcie.h"

/* set using channel 1 */
#define DMA_DESC_CTRL_CHANNEL   (1<<27)     
/* bit OWNED_BY_HW, When this bit is set to 1, the core can USE this descriptor */
#define DMA_DESC_CTRL_OBH       (1<<31)     


#if 0
uint32_t fpga_dma_descriptor[8]= {0};
//construct sgdma desciptor
static inline void construct_dma_desc(uint32_t source, uint32_t dst, uint16_t cnt,uint32_t nd)
{
    uint32_t *desc = &fpga_dma_descriptor[0];
    desc[0] = source;                                   //source address
    desc[2] = dst;                                      //destination address
    desc[6] = (uint32_t)cnt;
    desc[7] = DMA_DESC_CTRL_CHANNEL|DMA_DESC_CTRL_OBH;  //set DESC_CONTROL in dma descriptor
}

//construct sgdma desciptor for write
static inline int32_t make_w_desc(uint32_t source, uint32_t dst, uint16_t cnt,uint32_t nd)
{
    construct_dma_desc(source+PPC_PCIE_DDR1_OFFSET,dst+FPGA_PCIE1_BAR0_OFFSET,cnt,nd);
}

//construct sgdma desciptor for read
static inline void make_r_desc(uint32_t source, uint32_t dst, uint16_t cnt,uint32_t nd)
{
    construct_dma_desc(source+FPGA_PCIE1_BAR0_OFFSET,dst+PPC_PCIE_DDR1_OFFSET,cnt,nd);
}

//tell sgdma controller to start dma transmit
static inline void start_dma_transmit(int32_t id)
{
    uint32_t i,temp;
    uint32_t *ocm_base;

    uint32_t *dma_base;
    uint32_t dma_ram_ofs;

    uint32_t *desc = &fpga_dma_descriptor[0];
    ptrdiff_t stat = 0,ctrl = 4, ndp = 8;

    if (id == 1) {
        ocm_base = (uint32_t *)FPGA_DMA1_RAM_ADDR;
        dma_base = (uint32_t *)FPGA_DMA1_REG_ADDR;
        dma_ram_ofs = FPGA_DMA1_RAM_OFFSET;
    } else if (id ==2) {
        ocm_base = (uint32_t *)FPGA_DMA2_RAM_ADDR;
        dma_base = (uint32_t *)FPGA_DMA2_REG_ADDR;
        dma_ram_ofs = FPGA_DMA2_RAM_OFFSET;
        desc[2]+= 0x300000;
    } else {
        return -2;
    }

    for(i=0; i<=7; i++) {
        out_le32(ocm_base+i,desc[i]);
    }
    //clear run
    out_le32(dma_base + ctrl , in_le32(dma_base + ctrl)&~0x20);

    //clear status
    out_le32(dma_base + stat,0xff);

    //and ndp
    out_le32(dma_base + ndp , dma_ram_ofs);

    //configure ctrl reg and start RUN
    temp = in_le32(dma_base +ctrl);
    temp |= 0x20;
    temp |= 0x40;
    //start
    out_le32(dma_base + ctrl , temp);
}

//wait dma transmit done or failed
static inline uint32_t wait_dma_done(uint32_t id)
{
    uint32_t res = -1;
    ptrdiff_t  ctrl = 4,stat = 0;
    uint32_t *dma_base;
    uint32_t timeout = 0x20000;

    if (id == 1) {
        dma_base = (uint32_t *)FPGA_DMA1_REG_ADDR;
    } else if (id ==2 ) {
        dma_base = (uint32_t *)FPGA_DMA2_REG_ADDR;
    } else {
        return -2;
    }

    //wait finish and check err/status
    while(in_le32(dma_base + stat) & 0x10 ) {
        timeout -- ;
        if(timeout == 0) {
            return res;
        }
    }

    //get stat
    res = in_le32(dma_base + stat);

    //clear status
    out_le32(dma_base + stat,0xff);

    //clear run
    out_le32(dma_base + ctrl, in_le32(dma_base + ctrl)&~0x20);
    return res;
}
#endif

//check src and dst address used for pcie send
static inline int32_t pcie_send_check(uint32_t id,uint32_t src,uint32_t dst,int32_t cnt)
{
    uint32_t ret = TRUE;
    if (id == 1 ) {
        if ( (src + cnt)>PPC_PCIE1_BAR0_SIZE || (dst+cnt)>FPGA_PCIE1_BAR0_SIZE || cnt ==0) {
            ret = FALSE;
        }
    } else if (id == 2) {
        if ( (src + cnt)>PPC_PCIE2_BAR0_SIZE || (dst+cnt)>FPGA_PCIE2_BAR0_SIZE || cnt ==0) {
            ret = FALSE;
        }
    } else {
        ret = FALSE;
    }

    return ret ;

}

/* check src and dst address used for pcie receive */
static inline int32_t pcie_recv_check(uint32_t id,uint32_t src,uint32_t dst,int32_t cnt)
{
    uint32_t ret = TRUE;
    if (id == 1 ) {
        if ( (dst + cnt)>PPC_PCIE1_BAR0_SIZE || (src + cnt)>FPGA_PCIE1_BAR0_SIZE || cnt ==0) {
            ret = FALSE;
        }
    } else if (id == 2) {
        if ( (dst + cnt)>PPC_PCIE2_BAR0_SIZE || (src + cnt)>FPGA_PCIE2_BAR0_SIZE || cnt ==0) {
            ret = FALSE;
        }
    } else {
        ret = FALSE;
    }

    return ret ;

}

int32_t pcie_send(int32_t id, uint32_t src, uint32_t dst, uint32_t cnt)
{
    uint32_t dma_reg,dma_ram,psrc,pdst;
    uint32_t desc[8]={0};
    uint32_t temp;
    ptrdiff_t stat = 0,ctrl = 4*4, ndp = 8*4;
    uint32_t timeout = 0x200000;

    /* set register address for each pcie controller */
    if (id == 1) {
        dma_reg = FPGA_DMA1_REG_ADDR ;
        dma_ram = FPGA_DMA1_RAM_ADDR ;
        if (!pcie_send_check(id,src,dst,cnt))
            return -1;
        psrc = PPC_PCIE_SEND1_RAM_ADDR + src;
        pdst = FPGA_PCIE1_BAR0_OFFSET + dst;
    } else if (id ==2 ) {
        dma_reg = FPGA_DMA2_REG_ADDR ;
        dma_ram = FPGA_DMA2_RAM_ADDR ;
        if (!pcie_send_check(id,src,dst,cnt))
            return -2;
        psrc = PPC_PCIE_SEND2_RAM_ADDR + src;
        pdst = FPGA_PCIE2_BAR0_OFFSET + dst;
    }

    /* configure sgdma descriptor*/
    /* source address */
    desc[0] = psrc;             
    /* destination address */
    desc[2] = pdst;
    desc[6] = cnt;
    /* set DESC_CONTROL in dma descriptor */
    desc[7] = DMA_DESC_CTRL_CHANNEL|DMA_DESC_CTRL_OBH;  

    for(int i=0; i<=7; i++) {
        out_le32((uint32_t *)(dma_ram+i*4),desc[i]);
    }

    /* clear run */
    out_le32((uint32_t *)(dma_reg + ctrl) , in_le32((uint32_t *)(dma_reg + ctrl))&~0x20);

    /*clear status */
    out_le32((uint32_t *)(dma_reg + stat),0xff);

    /*and ndp */
    out_le32((uint32_t *)(dma_reg + ndp) , dma_ram);

    /*configure ctrl reg and start RUN */
    temp = in_le32((uint32_t *)(dma_reg +ctrl));
    temp |= 0x20;
    temp |= 0x40;
    /* start */
    out_le32((uint32_t *)(dma_reg + ctrl) , temp);

    /* wait finish and check err/status */
    while(in_le32((uint32_t *)(dma_reg + stat)) & 0x10 ) {
        timeout -- ;
        if(timeout == 0) {
            return -4;
        }
    }

    /* get stat */
    temp = in_le32((uint32_t *)(dma_reg + stat));

    /* clear status */
    out_le32((uint32_t *)(dma_reg + stat),0xff);

    /* clear run */
    out_le32((uint32_t *)(dma_reg + ctrl), in_le32((uint32_t *)(dma_reg + ctrl))&~0x20);

    /* check result */
    if ((temp & 0x10)!=0)
        return -5;

    return cnt;
}

int32_t pcie_recv(int32_t id, uint32_t src, uint32_t dst, uint32_t cnt)
{
    uint32_t dma_reg,dma_ram,psrc,pdst;
    uint32_t desc[8]={0};
    uint32_t temp;
    ptrdiff_t stat = 0,ctrl = 4*4, ndp = 8*4;
    uint32_t timeout = 0x2000000;

    /*set register address for each pcie controller*/
    if (id == 1) {
        dma_reg = FPGA_DMA1_REG_ADDR ;
        dma_ram = FPGA_DMA1_RAM_ADDR ;
        if (!pcie_send_check(id,src,dst,cnt))
            return -1;
        pdst = PPC_PCIE_SEND1_RAM_ADDR + dst ;
        psrc = FPGA_PCIE1_BAR0_OFFSET + src ;
    } else if (id ==2 ) {
        dma_reg = FPGA_DMA2_REG_ADDR ;
        dma_ram = FPGA_DMA2_RAM_ADDR ;
        if (!pcie_send_check(id,src,dst,cnt))
            return -2;
        pdst = PPC_PCIE_SEND2_RAM_ADDR + dst ;
        psrc = FPGA_PCIE2_BAR0_OFFSET + src;
    }

    /*configure sgdma descriptor*/
    /*source address*/
    desc[0] = psrc;
    /*destination address*/
    desc[2] = pdst;
    desc[6] = cnt;
    /*set DESC_CONTROL in dma descriptor*/
    desc[7] = DMA_DESC_CTRL_CHANNEL|DMA_DESC_CTRL_OBH;  

    for(int i=0; i<=7; i++) {
        out_le32((uint32_t *)(dma_ram+i*4),desc[i]);
    }

    /*clear run*/
    out_le32((uint32_t *)(dma_reg + ctrl) , in_le32((uint32_t *)(dma_reg + ctrl))&~0x20);

    /*clear status*/
    out_le32((uint32_t *)(dma_reg + stat),0xff);

    /*and ndp*/
    out_le32((uint32_t *)(dma_reg + ndp) , dma_ram);

    /*configure ctrl reg and start RUN*/
    temp = in_le32((uint32_t *)(dma_reg +ctrl));
    temp |= 0x20;
    temp |= 0x40;
    /*start*/
    out_le32((uint32_t *)(dma_reg + ctrl) , temp);

    /*wait finish and check err/status*/
    while(in_le32((uint32_t *)(dma_reg + stat)) & 0x10 ) {
        timeout -- ;
        if(timeout == 0) {
            return -4;
        }
    }

    /*get stat*/
    temp = in_le32((uint32_t *)(dma_reg + stat));

    /*clear status*/
    out_le32((uint32_t *)(dma_reg + stat),0xff);

    /*clear run*/
    out_le32((uint32_t *)(dma_reg + ctrl), in_le32((uint32_t *)(dma_reg + ctrl))&~0x20);

    /*check result*/
    if ((temp & 0x10)!=0)
        return -5;

    return cnt;
}

/*src: offset in pcie ram*/
void pcie_read(uint8_t *dst, uint8_t *src, uint32_t len)
{
    memcpy(dst,src+FPGA_PCIE1_BAR0_BASE,len);
}

/*dst: offset in pcie ram*/
void pcie_write(uint8_t *dst, uint8_t *src, uint32_t len)
{
    memcpy(dst+FPGA_PCIE1_BAR0_BASE,src,len);
}
