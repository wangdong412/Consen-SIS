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
 * 2015.11.4, Ezio created
 *
 * DECRIPTION
 * This file contains the driver componment configuration
 *
 */


#ifndef __FLASH_SPEC_H__
#define __FLASH_SPEC_H__
#include <config.h>

/* module setting */

//i2c config
#if defined(CONFIG_IIC)
#define CONFIG_IIC_0
//#define CONFIG_IIC_1
#endif

//flash config
#if defined(SFFS_NOR) || defined(CONFIG_NOR_FLASH)
//sub sector size
#define SS_SIZE                 (4*1024)
//ss number per sector
#define SS_NUM                  (32)
//sector size
#define SE_SIZE                 (SS_SIZE * SS_NUM)
//sector number
#define SE_NUM                  (512)
//flash total size
#define DEVICE_SIZE             (SE_SIZE * SE_NUM)

#elif defined(SFFS_SPI) || defined(CONFIG_SPI_FLASH)
//sub sector size
#define SS_SIZE                 (256)
//ss number per sector
#define SS_NUM                  (256)
//sector size
#define SE_SIZE                 (SS_SIZE * SS_NUM)
//sector number
#define SE_NUM                  (256)
//flash total size
#define DEVICE_SIZE             (SE_SIZE * SE_NUM)

#elif defined (SFFS_FPGA_SPI) || defined(CONFIG_FPGA_FLASH)
//sub sector size
#define SS_SIZE                 (4*1024)
//ss number per sector
#define SS_NUM                  (32)
//sector size
#define SE_SIZE                 (SS_SIZE * SS_NUM)
//sector number
#define SE_NUM                  (128)
//flash total size
#define DEVICE_SIZE             (SE_SIZE * SE_NUM)
#endif

//dfs config
#define DFS_NUM 8
#if 0
#if PAGE_ERASE == 1
#define FLASH_ERASE_UNIT        SS_SIZE
#elif PAGE_ERASE == 0
#define FLASH_ERASE_UNIT        SE_SIZE
#endif
#endif

//sffs config

#define SFFS_PAGE_ERASE         1
#define FNAME_LEN               17
#define SFFS_MAGIC              0xCA905001
#define SFFS_SB_START           0   //superblock position // force to 0
#define FREE_SIZEOF             32
#define FS_SE_NUM               (240)
#define FS_SS_NUM               SS_NUM
#define FS_SS_SIZE              SS_SIZE

//must align with n*SE_SIZE //real size is SFFS_MAX_FILES_SIZE-SFFS_HEADER_SIZE
#define SFFS_MAX_FILES_SIZE     (2*1024*1024)
//one file has N se
#define SFFS_BNO_LIM            16
#define SFFS_FILE_NUMBER        20
#define SFFS_FHDR_NUM           SS_NUM

#endif
