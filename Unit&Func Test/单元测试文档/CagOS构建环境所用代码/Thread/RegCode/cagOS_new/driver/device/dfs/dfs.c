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
 * DFS(device file system) source file. Make a encapsulation of different device driver and provide a unified interface operating device.
 *
 */

#include "dfs.h"


#define DFS_CKDFD  	if (dfd <0 || dfd >=DFS_NUM || dfs_table[dfd]==NULL ) \
                                        return DFS_DFD_FAULT;
//dfs device id
INT32 i2c_dfd;
INT32 spi_dfd;
INT32 nor_dfd;

static struct dfs_ops *dfs_table[DFS_NUM];

void dfs_init(void)
{
    for (INT32 i=0; i<DFS_NUM; i++) {
        dfs_table[i]=NULL;
    }
}

INT32 dfs_register(struct dfs_ops *dops)
{
    if(dops != NULL)
        for(INT32 i=0; i<DFS_NUM; i++) {
            if (dfs_table[i] == NULL) {
                dfs_table[i] = dops;
                return i;
            }
        }
    return DFS_PARA_FAULT;
}

void dfs_unregister(INT32 dfd)
{
    for(INT32 i=0; i<DFS_NUM; i++) {
        if (dfs_table[i] != NULL) {
            dfs_table[i] =NULL;
        }
    }
}

struct dfs_ops *get_dfs(uint32_t id)
{
    if(id < DFS_NUM){
        return dfs_table[id];
    }
    else 
        return 0;
}



/*******************************************************************************
*
* dfs_write - write data
*
*
* RETURNS: return actual wrote data bytes
*
*/
INT32 dfs_write(INT32 dfd, UINT32 dst,UINT8 *src,UINT32 cnt)
{
    DFS_CKDFD;
    if (src == NULL ||dst >= dfs_table[dfd]->size || dfs_table[dfd]->write == NULL || (dst+cnt)>dfs_table[dfd]->size) { //exceed effective address or fd num
        LOG_DEBUG("dfs write error did %d\n",dfd);
        return DFS_PARA_FAULT;
    }
    return dfs_table[dfd]->write((UINT32)(dfs_table[dfd]->base_addr) + (UINT32)(dfs_table[dfd]->start)+dst ,src,cnt);

}
/*******************************************************************************
*
* dfs_read - read data
*
* RETURNS: return actual read data bytes
*
*/
INT32 dfs_read(INT32 dfd, UINT32 src, UINT8 *buf,UINT32 cnt)
{
    DFS_CKDFD;
    //exceed effective address or fd num
    if (buf == NULL ||src >= dfs_table[dfd]->size || dfs_table[dfd]->read == NULL || (src+cnt) > dfs_table[dfd]->size) {
        LOG_DEBUG("dfs read error did %d\n",dfd);
        LOG_DEBUG("size %d read %d cnt %d\n",dfs_table[dfd]->size,dfs_table[dfd]->read,cnt);
        return DFS_PARA_FAULT;
    }
    return dfs_table[dfd]->read(dfs_table[dfd]->base_addr + dfs_table[dfd]->start + src,buf,cnt);
}

/*******************************************************************************
*
* dfs_erase - erase flash
*
* erase flash from sector no and erase "num" secotrs, or if flash support page erase, the unit will be page.
*
* RETURNS:
*
*/
INT32 dfs_erase(INT32 dfd,UINT32 no,UINT32 num)
{
    DFS_CKDFD;
    if (no<0 || no*dfs_table[dfd]->eraseUnit >= dfs_table[dfd]->size || (no+num)*dfs_table[dfd]->eraseUnit >= dfs_table[dfd]->size || dfs_table[dfd]->erase == NULL)
        return DFS_PARA_FAULT;
    LOG_DEBUG("dfs erase %d %x\n",dfd,no + dfs_table[dfd]->start/dfs_table[dfd]->eraseUnit);
    return	dfs_table[dfd]->erase(no + dfs_table[dfd]->start/dfs_table[dfd]->eraseUnit,num);
}

/*******************************************************************************
*
* dfs_cap - get device's capability
*
* get specific dfs device's capabilities,such as can write,can read,etc
*
* INT32 caps: want to know weather the device can do `caps`
*
* RETURNS:
*
*/
INT32 dfs_cap(INT32 dfd,INT32 ops)
{
    DFS_CKDFD;
    switch(ops) {
    case DFS_WRITE:
        return !(dfs_table[dfd]->write == NULL);
    case DFS_READ:
        return !(dfs_table[dfd]->read == NULL);
    case DFS_ERASE:
        return !(dfs_table[dfd]->erase == NULL);
    case DFS_LENGTH:
        return !dfs_table[dfd]->size;
    case DFS_START:
        return !dfs_table[dfd]->start;
    case DFS_END:
        return !dfs_table[dfd]->end;
    default:
        return -1;
    }
}

