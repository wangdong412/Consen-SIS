/*
 *------------------------------------------------------------
 * Project: cagOS
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
 * 2015.10.21, Ezio created
 *
 * DECRIPTION
 * spi driver specification.
 *
 */

#if !defined(__DEVICE_DEFS__)
#define __DEVICE_DEFS__

#define SPI_FLASH 1
#define S25FL128P 1

#define ESPIBASE	0xe0007000

#define BYTES_PER_SECTOR	0x00010000
#define BYTES_PER_PAGE		0x00000100

#define NUM_OF_PAGES	    256

#define MAX_SECTOR			256

#define GET_SECTOR_ADDRESS(x)	((x) << 16)
#define GET_SECTOR_NUMBER(x)	((x) >> 16)

#define BLOCKLOCKMASK	0x3c
#define ALL_SECTORS		(1 << 3)

#define MAX_WAIT_COMMAND	0x00200000  //1->2
#define MAX_ERASE_TIME		0x100
//#define MAX_ERASE_TIME		0x10000000
#define MAX_PROG_TIME		0x00010000

#define WREN	0x06	/*write enable*/
#define RDID	0x9F	/*read ID (only for products with X code)*/
#define RDSR	0x05	/*read status register*/
#define WRSR	0x01	/*write status register*/
#define READ	0x03	/*read data bytes*/
#define FREAD	0x0B	/*higher speed reed*/
#define PP		0x02	/*page program*/
#define SE		0xD8	/*sector erase*/
#define BE		0xC7	/*bulk erase*/
#define RES		0xAB	/*release from power down*/

#define WEL		(1 << 1)	/*Write Enable Latch*/
#define WIP		(1 << 0)	/*Write in Progressa*/


#define CMD(x)	((x) << 24)

#define NO_SECT_IN_U8		32		/*	256 - sectors (1bit per sector),
										NO_SECT_IN_U8 = 256 / 8bits
										1 - sector is protected, 0 - sector is not protected
									*/

#define GET_INDEX_AREA(x) 			(((x) & BLOCKLOCKMASK) >> 2)
#define BLANK_CHAR					0xff
#define MASK_3B						0x07

/*
 * size of diagnose buffer:
 * 5 words - Id
 * 8 words - sectors info
 * 8 words - protect info
 * 8 words - blank info	(extended diagnose)
 */
#define SIZE_OF_DIAGNOSE_BUFF		29
#define FULL_DIAGNOSE_OFFSET		21
#define PROTECTED_AREA_OFFSET		13

#define LOG8						3
#define INVALID			0xffffffff
#define MASK_16B		0x0000ffff
#define MASK_8B			0x000000ff

#define PROTECTED_AREA_NO	8
#define AREA(x)	((1) << (x))


#endif /* __S25FL032A_SPI_DEFS__ */
