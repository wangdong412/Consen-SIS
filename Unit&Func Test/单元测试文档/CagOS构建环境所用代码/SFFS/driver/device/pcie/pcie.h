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

#ifndef __PCIE_H__
#define __PCIE_H__

#include <libc/cagboot.h>
/*for pcie 1*/
/*bootloader defined*/
#define FPGA_PCIE1_BAR_BASE      0x80000000ul
/*fixed*/
#define FPGA_PCIE1_BAR0_OFFSET   0x400000ul
/*fixed*/
#define FPGA_PCIE1_BAR2_OFFSET   0x800000ul
#define FPGA_PCIE1_BAR0_BASE     (FPGA_PCIE1_BAR_BASE + FPGA_PCIE1_BAR0_OFFSET)
#define FPGA_PCIE1_BAR2_BASE     (FPGA_PCIE1_BAR_BASE + FPGA_PCIE1_BAR2_OFFSET)

/*fixed*/
#define FPGA_DMA1_REG_OFFSET     0x4000ul
/*fixed*/
#define FPGA_DMA1_RAM_OFFSET     0x5000ul
#define FPGA_DMA1_REG_ADDR       (FPGA_PCIE1_BAR2_BASE + FPGA_DMA1_REG_OFFSET)
#define FPGA_DMA1_RAM_ADDR       (FPGA_PCIE1_BAR2_BASE + FPGA_DMA1_RAM_OFFSET)

/*fixed , fpga bar0 size*/
#define FPGA_PCIE1_BAR0_SIZE     0x2000ul
/*fixed , ppc inbound size*/
#define PPC_PCIE1_BAR0_SIZE      0x200000ul
//inbound BAR//uboot defined
#define FPGA_INBOUND1_ADDR       0x0
//used for pcie 0 // inbound TAR
#define PPC_PCIE_DDR1_OFFSET     0x1fa00000ul  

#define PPC_PCIE_SEND1_RAM_ADDR  (PPC_PCIE_DDR1_OFFSET + PPC_PCIE1_BAR0_SIZE/2)

/*for pcie 2*/
/*bootloader defined*/
#define FPGA_PCIE2_BAR_BASE      0xa0000000ul   
/*fixed*/
#define FPGA_PCIE2_BAR0_OFFSET   0x400000ul    
/*fixed*/
#define FPGA_PCIE2_BAR2_OFFSET   0x800000ul    
#define FPGA_PCIE2_BAR0_BASE     (FPGA_PCIE2_BAR_BASE + FPGA_PCIE2_BAR0_OFFSET)
#define FPGA_PCIE2_BAR2_BASE     (FPGA_PCIE2_BAR_BASE + FPGA_PCIE2_BAR2_OFFSET)

/*fixed*/
#define FPGA_DMA2_REG_OFFSET     0x4000ul  
/*fixed*/
#define FPGA_DMA2_RAM_OFFSET     0x5000ul  
#define FPGA_DMA2_REG_ADDR       (FPGA_PCIE2_BAR2_BASE + FPGA_DMA2_REG_OFFSET)
#define FPGA_DMA2_RAM_ADDR       (FPGA_PCIE2_BAR2_BASE + FPGA_DMA2_RAM_OFFSET)

/*fixed  fpga bar0 size*/
#define FPGA_PCIE2_BAR0_SIZE     0x2000ul      
/*fixed ppc inbound size*/
#define PPC_PCIE2_BAR0_SIZE      0x200000ul    

/*inbound BAR,uboot defined*/
#define FPGA_INBOUND2_ADDR       0x300000ul 
/*used for pcie 1 ,inbound TAR*/
#define PPC_PCIE_DDR2_OFFSET     (PPC_PCIE_DDR1_OFFSET + PPC_PCIE1_BAR0_SIZE)
#define PPC_PCIE_SEND2_RAM_ADDR  (PPC_PCIE_DDR2_OFFSET + PPC_PCIE2_BAR0_SIZE/2)


int32_t pcie_send(int32_t id, uint32_t src , uint32_t dst,uint32_t cnt);
int32_t pcie_recv(int32_t id, uint32_t src , uint32_t dst,uint32_t cnt);
void pcie_read(uint8_t *dst, uint8_t *src, uint32_t len);
void pcie_write(uint8_t *dst, uint8_t *src, uint32_t len);
#endif
