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
 * 2015.9.18, Ezio created
 *
 * DECRIPTION
 * DFS(device file system) header file.
 *
 */

#ifndef __DFS_INC__
#define __DFS_INC__

//not export
#include <config.h>
#include "stddef.h"
#include "stdio.h"
#include <drv_config.h>
#include <libc/cagboot.h>
#include "../../fs/fs_api.h"

struct dfs_ops {
	INT32 (*write)(UINT32 dst,UINT8 *src,UINT32 cnt );
	INT32 (*read)(UINT32 src, UINT8 *buf, UINT32 cnt);
	INT32 (*erase)(INT32 no,INT32 num);
    INT32 (*lseek)(INT32 offset, INT32 whence);
	INT32 (*ioctl)(INT32 cmd, void *args);
    INT32 base_addr; //record flash's base address ,but may not use from this offset
	INT32 start;
	INT32 end;
    INT32 size;
    INT32 eraseUnit;
	INT32 *errno;
	UINT8 *dname;
    void *data;
};


struct flash_struct {
    INT32 seSize;
    INT32 seNum;
    INT32 ssSize;
    INT32 ssNum;
    INT32 totalSize;
};

void dfs_init(void);
INT32 dfs_cap(INT32 dfd, INT32 ops);

//to be exported
//dfs api
#define DFS_DFD_FAULT   -1
#define DFS_PARA_FAULT  -2

//dfs device capability
#define DFS_WRITE       0
#define DFS_READ        1
#define DFS_ERASE       2
#define DFS_LENGTH      3
#define DFS_START       4
#define DFS_END         5
INT32 dfs_register(struct dfs_ops *dops);
void dfs_unregister(INT32 dfd);
struct dfs_ops *get_dfs(UINT32 id);
INT32 dfs_write(INT32 dfd, UINT32 dst,UINT8 *src,UINT32 cnt);
INT32 dfs_read(INT32 dfd, UINT32 src, UINT8 *buf,UINT32 cnt);
INT32 dfs_erase(INT32 did,UINT32 no,UINT32 num);
//end exported
#endif
