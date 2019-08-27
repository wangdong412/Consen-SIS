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
 * 2015.10.14,created
 *
 * DESCRIPTION
 * This file contains file system shell library.
 */

#include "../driver/fs/fs_api.h"
#include "../driver/device/spi/spi.h"
#include <libc/shell.h>
#include <libc/string.h>
#include <libc/stdio.h>
#include <libc/rand.h>

extern UINT32 sysPirGet (void);
extern int atoi(const char *str);
extern int sf_erase_chip(void);

int test_fs(int argc, char *argv[]);

static int rename_file(int argc, char *argv[]);
static int list_file(int argc, char *argv[]);
static int mount_fs(int argc, char *argv[]);
static int du_file(int argc, char *argv[]);
static int format_fs(int argc, char *argv[]);
static int read_file(int argc, char *argv[]);
static int write_file(int argc, char *argv[]);
static int rm_file(int argc, char *argv[]);
static int df_file(int argc, char *argv[]);
static int nor_write(int argc, char *argv[]);
static int sf_ops(int argc, char *argv[]);


struct shell_cmd cmd_format= {
    .name = "fdisk",
    .help = "format flash",
    .usage = "format <type> ",
    .handler = format_fs
};

struct shell_cmd cmd_rename= {
    .name = "rename",
    .help = "rename file",
    .usage = "rename <old name> <new name> ",
    .handler = rename_file
};

struct shell_cmd cmd_fsdf= {
    .name = "df",
    .help = "get fs space unused",
    .usage = "df [-K][-M]",
    .handler = df_file
};

struct shell_cmd cmd_fsrm= {
    .name = "rm",
    .help = "rm file from flash",
    .usage = "rm <file name> ",
    .handler = rm_file
};

struct shell_cmd cmd_fsls = {
    .name = "ls",
    .help = "display files in flash",
    .usage = "ls",
    .handler = list_file
};

struct shell_cmd cmd_test= {
    .name = "test",
    .help = "test",
    .usage = "test ",
    .handler =test_fs
};
struct shell_cmd cmd_mount= {
    .name = "mount",
    .help = "mount sffs",
    .usage = "mount",
    .handler = mount_fs
};

struct shell_cmd cmd_fsread = {
    .name = "read",
    .help = "read file from flash",
    .usage = "read <file name> <len> <start offset>",
    .handler = read_file
};
struct shell_cmd cmd_fswrite= {
    .name = "write",
    .help = "write file to flash",
    .usage = "write <file name> <len>",
    .handler = write_file
};

struct shell_cmd cmd_norw= {
    .name = "nor_w",
    .help = "write nor flash",
    .usage = "nor_w <fname> <len>",
    .handler = nor_write
};

struct shell_cmd cmd_fsdu= {
    .name = "du",
    .help = "get fs space used",
    .usage = "du [-K][-M] [file name]",
    .handler = du_file
};

struct shell_cmd cmd_sf= {
    .name = "sf",
    .help = "spi flash",
    .usage = "usage: sf probe [[bus:]cs] [hz] [mode] - init flash device on given SPI bus and chip select \
                sf read addr offset len - read `len' bytes starting at `offset' to memory at `addr' \
                sf write addr offset len        - write `len' bytes from memory at `addr' to flash at `offset' \
                sf erase offset [+]len          - erase `len' bytes from `offset'`+len' round up `len' to block size \
                sf update addr offset len       - erase and write `len' bytes from memory at `addr' to flash at `offset'\n",
    .handler = sf_ops
};

static int list_file(int argc, char *argv[])
{
    int ret = 0;
    fs_ls();
    return ret;
}

static int write_file(int argc, char *argv[])
{
    int ret = 0;
    unsigned char buf[4096];
    int fd;
    int cnt;

    if (argc != 3) {
        printf("usage: write <file name> <len>\n");
        return -1;
    }
    cnt = atoi(argv[2]);

    fd = fs_open((INT8 *)argv[1],2,2);
    if (fd <0) {
        printf("file not exist\n");
        return -1;
    }
    memset(buf,0xde,sizeof(buf));
    ret = fs_write(fd,buf,cnt);
    if(ret <0) {
        printf("file write fail %d\n",ret);
    } else {
        printf("write retlen %d\n",ret);
    }
    ret = fs_close(fd);
    if (ret <0) {
        printf("close file %s fail\n",argv[1]);
        return -2;
    }
    return 0;
}


static int rm_file(int argc, char *argv[])
{
    int ret=0;

    if (argc != 2) {
        printf("usage: rm <file name>\n");
        return -1;
    }
    printf("fname %s\n",argv[1]);
    ret = fs_remove((INT8 *)argv[1]);

    return ret;
}


static int df_file(int argc, char *argv[])
{
    int ret=0;
    int lv;

    if (argc > 2) {
        printf("usage: df [-K][-M]\n");
        return -1;
    }

    if (argc == 1)
        lv = 1;
    else {
        if (!strncmp((char *)argv[1] , "-M",2)) {
            lv = 1024*1024;
        } else if (!strncmp((char *)argv[1] , "-K",2)) {
            lv = 1024;
        }
    }


    ret = fs_free();
    printf("flash has %d\n",ret/lv);

    return 0;
}


static int rename_file(int argc, char *argv[])
{
    int ret = 0;
    if (argc != 3) {
        printf("usage: rename <old name > <new name>\n");
        return -1;
    }

    ret = fs_rename((INT8 *)argv[1],(INT8 *)argv[2]);

    return ret;
}

static int du_file(int argc, char *argv[])
{
    int ret=0;
    int lv;

    if (argc > 3) {
        printf("usage: df [-K][-M] [fiel name]\n");
        return -1;
    }

    if (argc == 1)
        lv = 1;
    else {
        if (!strncmp((char *)argv[1] , "-M",2)) {
            lv = 1024*1024;
        } else if (!strncmp((char *)argv[1] , "-K",2)) {
            lv = 1024;
        }
    }

    if (argc == 3) {
        ret = fs_file_len((INT8 *)argv[2]);
        printf("file %s has %d\n",argv[2],ret/lv);
    } else if (argc == 2 && lv ==1) {
        ret = fs_file_len((INT8 *)argv[1]);
        printf("file %s has %d\n",argv[1],ret/lv);
    } else {
        ret = fs_used();
        printf("flash has %d\n",ret/lv);
    }

    return 0;
}



static int format_fs(int argc, char *argv[])
{
    int ret=0;

    if (argc != 2) {
        printf("usage: fdisk <type>\n");
        return -1;
    }
    /*int type = strtol(argv[1]);*/
    ret = fs_format(0);/*(int)strtol(argv[1]));*/

    return ret;
}



static int rm0(INT8 *fname)
{
    int ret=0;

    printf("fname %s\n",fname);
    ret = fs_remove(fname);
    printf("deld\n");

    return ret;
}

static int rf0(INT8 *fname,int size)
{
    int ret = 0;
    unsigned char buf[4096];
    int fd;
    int cnt;

    cnt = size;
    if (cnt > 4096) {
        printf("read too many\n");
        cnt = 4096;
    }

    fd = fs_open(fname,0,1);
    if (fd <0) {
        printf("rfile not exist\n");
        return -1;
    }
    if (fs_file_len(fname) < cnt) {
        cnt = fs_file_len(fname);
    }
    ret = fs_read(fd,buf,cnt);
    printf("real read %d bytes file len %d cnt %d\n",ret,fs_file_len(fname),cnt);
    ret = fs_close(fd);
    if (ret <0) {
        printf("rclose file %s fail ret %d\n",fname,ret);
        return -2;
    }
    printf("readed\n");
    return 0;
}

static int wf0(INT8 *fname,int size )
{
    int ret = 0;
    unsigned char buf[4096];
    int fd;
    int cnt;

    cnt = size;

    fd = fs_open(fname,2,2);
    if (fd <0) {
        printf("wfile not exist\n");
        return -1;
    }
    memset(buf,0xde,sizeof(buf));
    ret = fs_write(fd,buf,cnt);
    if(ret <0) {
        printf("wfile write fail %d\n",ret);
    }
    printf("wred\n");
    ret = fs_close(fd);
    if (ret <0) {
        printf("wclose file %s fail ret %d\n",fname,ret);
        return -2;
    }
    printf("wrote\n");
    return 0;
}

static int rn0(INT8 *old, INT8 *new)
{

    INT32 ret=-1;
    printf("rename old %s new %s\n",old,new);
    ret = fs_rename(old,new);
    return ret;
}

int test_fs(int argc, char *argv[])
{
    int ret=0,i,j;
    INT8 namel[SFFS_FILE_NUMBER][2];
    printf("0x%x\n",sysPirGet());
    if(sysPirGet() == 1) {
        printf("run in core 1\n");
        int *bb = (int *)0xec000000;
        printf("%x\n",*bb);
    } else {
        printf("core 0\n");
    }

    for(i=0; i<SFFS_FILE_NUMBER; i++) {
        namel[i][0]=i+65;
        namel[i][1]='\0';
    }

    if (argc != 1) {
        printf("usage: testd\n");
        return -1;
    }
    for(i=0; i<20; i++) {
        printf("times %d|\n",i);
        j=rand()%4;
        switch (j) {
        case 0:
            printf("write\n");
            wf0(&namel[rand()%SFFS_FILE_NUMBER][0], rand()%(SFFS_MAX_FILES_SIZE));
            break;
        case 1:
            printf("remove\n");
            rm0(&namel[rand()%SFFS_FILE_NUMBER][0]);
            break;
        case 2:
            printf("read\n");
            rf0(&namel[rand()%SFFS_FILE_NUMBER][0],rand()%1024);
            break;
        case 3:
            printf("rename\n");
            rn0(&namel[rand()%SFFS_FILE_NUMBER][0],&namel[(rand()%SFFS_FILE_NUMBER+1)%SFFS_FILE_NUMBER][0]);
            break;
        default:
            printf("error option\n");
        }
        fs_ls();
    }
    return ret;
}

extern int sffs_init(void);
static int mount_fs(int argc, char *argv[])
{
    int ret=0;

    if (argc != 1) {
        printf("usage: mount\n");
        return -1;
    }
    ret = sffs_init();
    ret = fs_mount(0);
    return ret;
}

//nor cmd
static int nor_write(int argc, char *argv[])
{
    int ret=0;

    if (argc != 3) {
        printf("usage: nor_w <fname> <len>\n");
        return -1;
    }
    return ret;
}

static int sf_ops(int argc, char *argv[])
{
    int ret=0;

    if (argc < 4) {
        printf("usage: sf probe [[bus:]cs] [hz] [mode] - init flash device on given SPI bus and chip select \n\
                sf read addr offset len - read `len' bytes starting at `offset' to memory at `addr'\n\
                sf write addr offset len        - write `len' bytes from memory at `addr' to flash at `offset'\n\
                sf erase offset [+]len          - erase `len' bytes from `offset'`+len' round up `len' to block size\n\
                sf update addr offset len       - erase and write `len' bytes from memory at `addr' to flash at `offset'\n");

        return -1;
    }
    if(strncmp((char *)argv[1] ,"eraseall",8) ==0 ) {
        sf_erase_chip();
    } else {
        printf("sf command not implement all\n");
    }
    return ret;
}

void fsCmdReg(void)
{
    shellCmdRegister(&cmd_fsls);
    shellCmdRegister(&cmd_fsread);
    shellCmdRegister(&cmd_fswrite);
    shellCmdRegister(&cmd_fsrm);
    shellCmdRegister(&cmd_fsdf);
    shellCmdRegister(&cmd_fsdu);
    shellCmdRegister(&cmd_format);
    shellCmdRegister(&cmd_test);
    shellCmdRegister(&cmd_mount);
    shellCmdRegister(&cmd_sf);
    shellCmdRegister(&cmd_rename);
}

static int read_file(int argc, char *argv[])
{
    int ret = 0;
    unsigned char buf[4096];
    int fd;
    int cnt;
    int ofs;

    if (argc != 4) {
        printf("usage : read <file name>  <len> <start offset>\n");
        return -1;
    }
    cnt = atoi(argv[2]);
    printf("cnt %d\n",cnt);
    if (cnt > 4096) {
        printf("read too many\n");
        cnt = 4096;
    }

    fd = fs_open((INT8 *)argv[1],0,1);
    if (fd <0) {
        printf("file not exist\n");
        return -1;
    }
    ofs = atoi(argv[3]);
    if (ofs >= fs_file_len((INT8 *)argv[1]) ) {
        cnt = 0;
    } else if ( fs_file_len((INT8 *)argv[1])  <= (cnt+ofs)) {
        cnt = fs_file_len((INT8 *)argv[1]) - ofs;
    }
    if (cnt != 0) {
        ret = fs_lseek(fd,10,0);
        ret = fs_read(fd,buf,cnt);
        printf("real read %d bytes file len %d\n",ret,fs_file_len((INT8 *)argv[1]));
        printf("\n\n");
        for(int i=0; i<ret; i++) {
            printf("%x ",buf[i]);
        }
        printf("\n\n");
    } else {
        printf("para err ,read 0 byte\n");
    }
    ret = fs_close(fd);
    if (ret <0) {
        printf("close file %s fail\n",argv[1]);
        return -2;
    }
    return 0;
}
