/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  shell cmd code for PPC85XX.
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
 * 2015.10.10, Ezio created
 *
 * DESCRIPTION
 * sffs demo source file.
 */

#include <libc/cagboot.h>
#include "../cagos.h"
//#include "../driver/fs/fs_api.h"
//#include "../driver/device/dfs/dfs.h"
#include "./demo.h"
#include <drv_config.h>

extern int sffs_idx;
extern int sffs_idx2;

void fs_multi_demo(void)
{
    int8_t *fname1="test",*fname2="test2",*fname3="deled";
    int32_t fd1,fd2;
    int32_t cnt;
    uint8_t buf[4096],buf1[4096];
    int32_t ret = FS_FAIL;

    cnt = 2345;
    memset(buf,0x5a,sizeof(buf));
    memset(buf1,0,sizeof(buf1));

    printf("test in fs 1\n");

    fd1 = fs_open(fname1,FS_OPEN_CREATE,FS_WRONLY|FS_ID_POS(sffs_idx));
    if (fd1 <0){
        printf("1open file create|tru %s fail in fs %d ret %d\n",fname1,FS_ID_POS(sffs_idx),fd1);
        return;
    }
    ret = fs_write(fd1,buf,cnt);
    if(ret < cnt){
        printf("1write file %s fail ret %d\n",fname1,ret);
        return ;
    }

    ret = fs_close(fd1);
    if(ret <0){
        printf("1close file %s fail ret %d\n",fname1,ret);
        return ;
    }
    fd1 = fs_open(fname1,FS_OPEN_NORMAL,FS_RDONLY|FS_ID_POS(sffs_idx));
    if (fd1 <0){
        printf("1open file normal  %s fail in fs %d ret %d\n",fname1,FS_ID_POS(sffs_idx),fd1);
        return;
    }
    ret = fs_read(fd1,buf1,cnt);
    if(ret < 0){
        printf("1read fail ret %d\n",ret);
        return ;
    }
    printf("read:\n");
    for(int i =0;i<ret;i++){
        printf("%x ",buf1[i]);
    }
    printf("\n");
    ret = fs_close(fd1);
    if(ret <0){
        printf("1close file %s fail ret %d\n",fname1,ret);
        return ;
    }
    ret = fs_rename(fname1,fname3);
    if(ret < 0){
        printf("1rename fail ret %d\n",ret);
        return ;
    }
    ret = fs_remove(fname3);
    if(ret < 0){
        printf("1remove fail ret %d\n",ret);
    }

    /***************************/

#ifdef SFFS_MULTI_DEVICE

    fd2 = fs_open(fname2,FS_OPEN_CREATE,FS_WRONLY|FS_ID_POS(sffs_idx2));
    if (fd2 <0){
        printf("open file cre|tru %s fail in fs %d ret %d\n",fname2,FS_ID_POS(sffs_idx),fd2);
        return;
    }
    ret = fs_write(fd2,buf,cnt);
    if(ret < cnt){
        printf("write file %s fail ret %d\n",fname2,ret);
        return ;
    }

    ret = fs_close(fd2);
    if(ret <0){
        printf("close file %s fail ret %d\n",fname2,ret);
        return ;
    }
    fd2 = fs_open(fname2,FS_OPEN_NORMAL,FS_RDONLY|FS_ID_POS(sffs_idx2));
    if (fd2 <0){
        printf("open file normal %s fail in fs %d ret %d\n",fname2,FS_ID_POS(sffs_idx2),fd2);
        return;
    }
    ret = fs_read(fd2,buf1,cnt);
    if(ret < 0){
        printf("read fail ret %d\n",ret);
        return ;
    }
    printf("read:\n");
    ret = fs_close(fd2);
    if(ret <0){
        printf("close file %s fail ret %d\n",fname2,ret);
        return ;
    }
    ret = fs_rename(fname2,fname3);
    if(ret < 0){
        printf("rename fail ret %d\n",ret);
        return ;
    }
    ret = fs_remove2(fname3,sffs_idx2);
    if(ret < 0){
        printf("remove fail ret %d\n",ret);
        return ;
    }
#endif
    printf("sffs demo done\n");
}
