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
extern int sffs_idx3;

int *sffs_test_data =0x10000000;
int *sffs_test_data2=0x13000000;

void make_test_data(int len_w,int seed)
{
    if(len_w >16*1024*1024)
        len_w=2*1024*1024;

    for(int i =0; i<len_w; i++) {
        *(sffs_test_data+i) = seed+i;
    }
}




void fs_multi_demo(void)
{
    int8_t *fname1="test",*fname2="test2",*fname3="deled";


    int32_t fd1,fd2;
    int32_t cnt;
    uint8_t buf[4096],buf1[4096];
    int32_t ret = FS_FAIL;
    int rd=0,rd2,ii = 0;


const char *lname[]= {
        [0]="tes1\0",
        [1]="TES2\0",
        [2]="exp3\0",
        [3]="EXP4\0",
        [4]="exp5\0",
        [5]="eXp6\0",
        [6]="EXp7\0",
        [7]="ExP8\0",
        [8]="Exp9\0",
        [9]="ex10\0",
        [10]="Ex11\0",
        [11]="Te12\0",
        [12]="tE13\0",
        [13]="hf14\0",
        [14]="fa15\0",
        [15]="io16\0",
        [16]="mb17\0",
        [17]="gh18\0",
        [18]="ty19\0",
        [19]="zc20\0",
        [20]="kl21\0",
    };
            for(int kk = 0;kk<21;kk++){
                printf("%s\n",lname[kk]);
            }
    memset(buf,0x5a,sizeof(buf));
    memset(buf1,0,sizeof(buf1));

    printf("test in fs 3 %d\n",sffs_idx3);
    sffs_idx = sffs_idx3;

    fs_format(0);

    fs_ls();

    while(1) {
        printf("test %d\n",ii);

        rd = rand()%20;
        printf("rd %d,%s\n",rd,lname[rd]);

        //fd1 = fs_open(lname[rd],FS_OPEN_CREATE,FS_WRONLY|FS_ID_POS(sffs_idx3));
        fd1 = fs_open(lname[rd],FS_OPEN_CREATE,FS_WRONLY);
        if (fd1 <0) {
            printf("1open file create %s fail in fs %d ret %d\n",lname[rd],FS_ID_POS(sffs_idx),fd1);
            return;
        }

        printf("rd1 %s %x \n",lname[rd],rd);
//        printf("write 4M bytes\n");
        cnt = 131072;
        printf("rd1.0 %s %x \n",lname[rd],rd);
        make_test_data(cnt/4,9);
        printf("rd1.1 %s %x \n",lname[rd],rd);
        //printf("%d\n",*(int *)sffs_test_data);

        for (int i =0 ; i<cnt; i+=4096) {
//            volatile unsigned short *time=(unsigned short*)0xff700140;
//            volatile unsigned long t1,t2;
//            t1=getTblus();
            ret = fs_write(fd1,sffs_test_data,4096);
            printf("rd1.2 %s %x %d\n",lname[rd],rd,i);
//            t2=getTblus();

//            printf("wt %x\n",t2-t1);
            if(ret < 4096) {
                if (sffs_get_errno(0) == SFFS_NO_FREE_SE ) {
                    printf("full\n");
                    fs_ls();
                    goto cls;
                    break;
                }
                else if (sffs_get_errno(0) == SFFS_FILE_REWRITE_ERR) {
                    goto cls;
                }
                else {
                    printf("1write file %s fail ret %d err %d\n",lname[rd],ret,sffs_get_errno(0));
                    return ;
                }
            }
            printf("rd1.2.1 %s %x %d\n",lname[rd],rd,i);
//          printf("write %x\n",*(int *)sffs_test_data);
        }
        printf("rd2 %s %x \n",lname[rd],rd);

cls:    ret = fs_close(fd1);
        if(ret <0) {
            printf("1close file %s fail ret %d err %d \n",lname[rd],ret,sffs_get_errno(0));
            return ;
        }

//        printf("read data \n");

        printf("rd3 %s %x \n",lname[rd],rd);
        //fd1 = fs_open(lname[rd],FS_OPEN_NORMAL,FS_RDONLY|FS_ID_POS(sffs_idx3));
        fd1 = fs_open(lname[rd],FS_OPEN_NORMAL,FS_RDONLY);
        if (fd1 <0) {
            printf("1open file normal  %s fail in fs %d ret %d err %d\n",lname[rd],FS_ID_POS(sffs_idx),fd1,sffs_get_errno(0));
            for(int kk = 0;kk<21;kk++){
                printf("%s\n",lname[kk]);
            }

            return;
        }

        ret = fs_read(fd1,sffs_test_data2,cnt);
        if(ret < 0) {
            printf("1read fail ret %d\n",ret);
            return ;
        }

        if (ret == 0 )
            printf("file read 0\n");

        if (!memcmp(sffs_test_data,sffs_test_data2,cnt) && !memcmp(sffs_test_data,sffs_test_data2,ret)) {
            printf("read or write fail %d\n",ret);
            printf("read 1:\n");
            for(int i =0; i<ret; i++) {
                printf("%x ",sffs_test_data2[i]);
            }
            printf("\n");
        }
        else {
            printf("readt \n");
            for(int i =0; i<100; i++) {
                printf("%x ",sffs_test_data2[i]);
            }
            printf("\n");
            printf("\n");
        }


        ret = fs_close(fd1);
        if(ret <0) {
            printf("1close file %s fail ret %d err %d\n",lname[rd],ret,sffs_get_errno(0));
            return ;
        }

        rd2=rand()%20;

        ret = fs_rename(lname[rd],lname[rd2]);
        if(ret < 0) {
            if (sffs_get_errno(0) != SFFS_FILENAME_NOT_EXIST_ERR2 &&
                    sffs_get_errno(0) != SFFS_FNAME_DUPLICATE_ERR ){
                printf("1rename fail ret %d err %d\n",ret,sffs_get_errno(0));
                return ;
            }
        }
        else{
            printf("rename %s to %s\n",lname[rd],lname[rd2]);
        }

#if 0
        printf("1 %s 2 %s\n",lname[rd],lname[rd2]);
        fs_ls();
        fd1 = fs_open(lname[rd],FS_OPEN_NORMAL,FS_RDONLY);
        if (fd1 <0) {
            printf("1open file normal  %s fail in fs %d ret %d err %d\n",lname[rd],FS_ID_POS(sffs_idx),fd1,sffs_get_errno(0));
            return;
        }
        
        return ;
#endif

rmf :   rd2=rand()%20;
        if (ii % 4 == 0){
            ret = fs_remove(lname[rd2]);
            printf("remove %s\n",lname[rd2]);
            if(ret < 0) {
                if (sffs_get_errno(0) != SFFS_FILENAME_NOT_EXIST_ERR3){
                    printf("1remove fail ret %d err %d\n",ret,sffs_get_errno(0));
                    return ;
                }
                
            }
            else {
                printf("rm file %s\n",lname[rd2]);
            }
        }

xxx:    ii++;
        //if (ii%10==0)
            fs_ls();
    }

    printf("sffs demo done\n");
}


#if 0
void ufs_demo(void)
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

    fd1 = fs_open(fname1,FS_OPEN_CREATE,FS_WRONLY|FS_ID_POS(sffs_idx3));
    if (fd1 <0) {
        printf("1open file create|tru %s fail in fs %d ret %d\n",fname1,FS_ID_POS(sffs_idx3),fd1);
        return;
    }
    ret = fs_write(fd1,buf,cnt);
    if(ret < cnt) {
        printf("1write file %s fail ret %d\n",fname1,ret);
        return ;
    }

    ret = fs_close(fd1);
    if(ret <0) {
        printf("1close file %s fail ret %d\n",fname1,ret);
        return ;
    }
    fd1 = fs_open(fname1,FS_OPEN_NORMAL,FS_RDONLY|FS_ID_POS(sffs_idx3));
    if (fd1 <0) {
        printf("1open file normal  %s fail in fs %d ret %d\n",fname1,FS_ID_POS(sffs_idx3),fd1);
        return;
    }
    ret = fs_read(fd1,buf1,cnt);
    if(ret < 0) {
        printf("1read fail ret %d\n",ret);
        return ;
    }
    printf("read:\n");
    for(int i =0; i<ret; i++) {
        printf("%x ",buf1[i]);
    }
    printf("\n");
    ret = fs_close(fd1);
    if(ret <0) {
        printf("1close file %s fail ret %d\n",fname1,ret);
        return ;
    }
    ret = fs_rename(fname1,fname3);
    if(ret < 0) {
        printf("1rename fail ret %d\n",ret);
        return ;
    }
    ret = fs_remove(fname3);
    if(ret < 0) {
        printf("1remove fail ret %d\n",ret);
    }

    printf("sffs ufs demo done\n");
}
#endif
