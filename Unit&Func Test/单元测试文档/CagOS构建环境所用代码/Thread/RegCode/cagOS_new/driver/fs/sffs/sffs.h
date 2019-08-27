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
 * 2015.9.1, Ezio created
 *
 * DECRIPTION
 * SFFS(simple flash file system) header file.
 *
 */


#ifndef SFFS_MAIN_STRUCT_H__
#define SFFS_MAIN_STRUCT_H__


//not export
#include <libc/stdio.h>
#include <config.h>
#include <libc/stddef.h>
#include <drv_config.h>
#include <libc/cagboot.h>
#include "../bits.h"

//file hdr in flash status
#define FS_HDRS_EMPTY                   0
#define FS_HDRS_USED                    1
#define FS_HDRS_TMP                     2
#define FS_HDRS_BAD                     3

#define FS_S_UNUSED                     -1
#define FS_COMMITED                     0xc0
#define FS_DELETED                      0xde
#define FS_EMPTY                        0xff
#define FS_FERR                         0xee

//lock
#define FS_IS_LOCKED                    -1
#define FS_LOCK                         0

//values
#define SFFS_COMMITED                   0x1
//#define READONLY                      1
//#define WRITEONLY                     2

//log print
//#define LOG_STUBS

#ifdef LOG_STUBS
#define LOG_DEBUG       printf
#define LOG_INFO        printf
#define LOG_WARN        printf
#define LOG_ERROR       printf
#else
void NOLOG(const char*c,...);
#define LOG_DEBUG       NOLOG
#define LOG_INFO        NOLOG
#define LOG_WARN        NOLOG
#define LOG_ERROR       NOLOG
#endif
//flash file hdr
//magic num
#define FF_HDR_MAGIC            0
//file name
#define FF_HDR_NAME             (FF_HDR_MAGIC+4)
//file name len
#define FF_HDR_NAMELEN          (FF_HDR_NAME+FNAME_LEN+1)f
//file size
#define FF_HDR_SIZE             (FF_HDR_NAMELEN+2)
//file ofs
#define FF_HDR_OFS              (FF_HDR_SIZE+ 4)
#define FF_HDR_FID              (FF_HDR_OFS + 4)
#define FF_HDR_SEQ              (FF_HDR_FID + 4)
//block numbers
#define FF_HDR_BNO              (FF_HDR_SEQ + 4)
#define FF_HDR_HDRSS            (FF_HDR_BNO + SFFS_BNO_LIM*2)
//commit bit
#define FF_HDR_COMMIT           (FF_HDR_HDRSS + 2)
//unused bit
#define FF_HDR_UNUSED1           (FF_HDR_COMMIT +1)
//delete bit
#define FF_HDR_DELED            (FF_HDR_UNUSED1 + 1)
#define FF_HDR_STAT             (FF_HDR_DELED +1)
#define FF_HDR_UNUSED2          (FF_HDR_STAT + 1)
//file hdr size
#define SFFS_HEADER_SIZE        (FF_HDR_STAT+2)

#define MAX_SLOT 10

//ofs: flash offset,ptr: buf
typedef INT32 (*write_func)(INT32 did, UINT32 ofs, UINT8 *ptr, UINT32 size);
typedef INT32 (*read_func)(INT32 did, UINT32 ofs, UINT8 *ptr, UINT32 size);
typedef INT32 (*seek_func)(INT32 did, INT32 offset, INT32 whence);
typedef INT32 (*erase_func)(INT32 did, UINT32 no, UINT32 num);

//描述文件的数据结构
struct sffs_entry {
    UINT32                  magic_num;
    INT8                    name[FNAME_LEN+1]; //18
    INT16                   name_len;

    INT32                   size;
    INT32                   ofs;                        //文件读写指针
    UINT32                  fid;                        //文件编码
    UINT32                  seq;
    INT16                   se_no[SFFS_BNO_LIM];//16    //记录文件占用的sector no.
    INT16                   hdr_ss;                     //记录文件头在flash 的位置
    UINT8                   comt;                       //文件操作完成标志位
    UINT8                   unused;

    UINT8                   deled;                      //文件删除标志位
    //file is in writing or reading
    UINT8                   stat;
    UINT8                   unused2[2];
};

struct sffs {
    //记录flash 的size
    INT32                   device_size;
    //记录所有的文件指针
    struct sffs_entry       file[SFFS_FILE_NUMBER];
    //保存empty space 信息
    UINT32                  free_se[SE_NUM/FREE_SIZEOF];
    INT8                    free_ss_hdr[SFFS_FHDR_NUM];
    //记录当前正在使用的se 的位置
    INT16                   free_se_cur;
    //记录当前empty ss for file hdr 的位置
    INT16                   free_ss_hdr_cur;
    //INT16                   sb_se;
    //file number count
    INT32                   fileNum;
    //mount stat
    INT32                   mounted;
    //last error number
    INT32                   errno;
    //current useable file id
    UINT32                  fidcnt;
    UINT32                  seqcnt;
    UINT32                  flashId;
    UINT32                  fsid;
    INT8                    lock;

    UINT8                   pageErase;
    INT8                    fsname[16];
    //these variable should assigned before mount
    UINT32                  magic_num;
    //sub-sector size
    INT32                   ss_size;
    //sector size
    INT32                   se_size;
    INT32                   erase_unit;
    //end

    //flash ops
    erase_func              erase;
    write_func              write;
    read_func               read;
    seek_func               seek;
};

//to be exported
//fs api
//sffs error number
#define SFFS_ERASE_HDRSS_ERR            -4
#define SFFS_WRITE_HDRSS_ERR            -10
#define SFFS_HDRSS_FULL_ERR             -7

#define SFFS_NO_FREEHDRSS_ERR           -5
#define SFFS_NO_FREEHDRSS_ERR1           -31

#define SFFS_FILE_NUM_FULL_ERR          -6

#define SFFS_PARA_ERR                   -1

#define SFFS_FILENAME_NOT_EXIST_ERR     -8
#define SFFS_FD_ERR                     -9
#define SFFS_WRONLY_ERR                 -15
#define SFFS_READMODE_ERROR             -18
#define SFFS_WRITEMODE_ERR              -19
#define SFFS_FILE_REWRITE_ERR           -12
#define SFFS_UNALIGN_WRITE_ERR          -14
#define SFFS_DEL_ERR                    -21
#define SFFS_FILE_LOCKED_ERR            -2
#define SFFS_FILE_NOT_COMMIT_ERR        -20
#define SFFS_FILE_SIZE_EXCED_ERR        -16
#define SFFS_INIT_ERR                   -22
#define SFFS_NOT_NOT_MOUNTED            -23
#define SFFS_MODE_ERR                   -29

#define SFFS_NO_FREE_SE                 -3
#define SFFS_READ_DATA_ERR              -11
#define SFFS_ERASE_DATA_ERR             -13
#define SFFS_WRITE_DATA_ERR             -17
#define SFFS_OFFSET_ERR                 -30

#define SFFS_GLOBAL_ERR                 -24 //global vars are inconsistent
#define SFFS_BUF_ERR                    -25
#define SFFS_FNAME_DUPLICATE_ERR        -26
#define SFFS_RENAME_FAIL1               -27
#define SFFS_RENAME_FAIL2               -28

//file status defines
#define FS_CLOSED                       0
#define FS_RDONLY                       1
#define FS_WRONLY                       2

//fs mounted
#define FS_UMOUNTED                     0
#define FS_MOUNTED                      1

//mode
#define FS_OPEN_NORMAL                  0 //if not exist return fail
#define FS_OPEN_TRU                     1 //if exist delete and create else create
#define FS_OPEN_CREATE                  2 //if exist open else create

#define FS_ID_POS(id)                   ((id==0)?0:(1<<(16-1+id)))
#define FS_ID_GET(id)                   (__builtin_ffs((id&0xffff0000)>>16))

//lseek type
#define FS_SEEK_SET                     0
#define FS_SEEK_TELL                    1

//return value defines
#define FS_OK                       0
#define FS_FAIL                     -1
//format type
#define FS_FORMAT_HDRS              0
#define FS_FORMAT_ALL               1
#define FS_FORMAT_DISK              2



extern void sffs_set_errno(struct sffs *fs,INT32 errno);
extern INT32 sffs_is_mounted(struct sffs *fs);
extern INT32 sffs_mount(struct sffs *fs, INT32 type);

#define ERASE_NUM_PER   1

//two types of operation failure:
//1. just return FS_FAIL and that's enough
//2. return FS_FAIL and tell the failure

#define FS_IS_MOUNTED(fs)  if (!sffs_is_mounted(fs)) {                     \
                                sffs_set_errno(fs,SFFS_NOT_NOT_MOUNTED);    \
                                LOG_DEBUG("NOT mountedn\n");                  \
                                sffs_unlock(fs);                            \
                                return FS_FAIL;                             \
                            }

#define FS_UNLOCK(fs)        sffs_unlock(fs);

#define FS_TRYLOCK(fs)  if (sffs_lock(fs)!=FS_OK) {     \
                            sffs_set_errno(fs, SFFS_FILE_LOCKED_ERR); \
                            LOG_DEBUG("LOCK ERRORn\n");                   \
                            return FS_FAIL; \
                        }

#define FS_DEFI2    struct sffs *fs; \
                    INT32 ret = FS_FAIL;            \
                    FS_CKFSID(fsid);  \
                    fs = &sffs_slot[fsid]; \
                    FS_TRYLOCK(fs);                                  \
                    FS_IS_MOUNTED(fs);
#define FS_DEFI     struct sffs *fs=&sffs_slot[0];                  \
                    INT32 ret = FS_FAIL;                            \
                    FS_TRYLOCK(fs);                                  \
                    FS_IS_MOUNTED(fs);

#define FS_DEFO    FS_UNLOCK(fs);                                    \
                   return ret;

#define FS_CKADDR(buf)  if( buf == NULL) {   \
                            sffs_set_errno(fs,SFFS_BUF_ERR);    \
                            LOG_DEBUG("EMPTY POINTER\n");                 \
                            while(1);   \
                            FS_DEFO;        \
                        }

#define FS_CKFSID(fsid)     if(fsid < 0 || fsid >= MAX_SLOT || sffs_slot[fsid].flashId == -1) { \
                                sffs_set_errno(&sffs_slot[MAX_SLOT-1], SFFS_PARA_ERR);  \
                                LOG_WARN("fs id err\n"); \
                                return FS_FAIL;         \
                            }

#define FS_GET_SFFS(fsid)     struct sffs *fs;        \
                            int ret = FS_FAIL;      \
                            FS_CKFSID(fsid);        \
                            fs =&sffs_slot[fsid];



INT32 sffs_lock(struct sffs *fs);
INT32 sffs_unlock(struct sffs *fs);
struct sffs *sffs_get_slot(int fsid);
INT32 sffs_register(struct sffs fs);
void sffs_set_errno(struct sffs *fs,INT32 errno);
INT32 sffs_umount(struct sffs *fs);
INT32 sffs_find_filehdr(struct sffs *fs,const INT8 *fname);
INT32 sffs_rename(struct sffs *fs,const INT8 *old_name,const INT8 *new_name);
INT32 sffs_get_fsfd(INT32 fd);
INT32 sffs_ck_fd(struct sffs *fs, INT32 fd);
INT32 sffs_lseek(struct sffs *fs, INT32 fd, INT32 pos);
INT32 sffs_remove_byname(struct sffs *fs, const INT8 *fname);
INT32 sffs_open(struct sffs *fs,const INT8 *fname,INT32 mode);
INT32 sffs_close(struct sffs *fs, INT32 fd);
INT32 sffs_write(struct sffs *fs, INT32 fd, const void *data, INT32 size);
INT32 sffs_read(struct sffs *fs, INT32 fd, void *data, INT32 size);
INT32 sffs_format(struct sffs *fs, INT32 type);
INT32 sffs_get_errno(INT32 fsid) ;


static __inline__ INT32 sffs_read_at(struct sffs *fs, UINT32 offset, void *data,UINT32 size)
{
    INT32 ret = FS_FAIL;
    if(fs->read != NULL) {
        ret = fs->read(fs->flashId, offset, (UINT8 *)data, size);
    }
    return ret;
}

#if 0
INT32 fs_rename(const INT8 *old_name,const INT8 *new_name);
INT32 fs_format(INT32 type);
INT32 fs_umount(INT32 fsid);
INT32 fs_mount(INT32 type);
INT32 fs_open(const INT8 *fname, INT32 flags, INT32 mode);
INT32 fs_remove(const INT8 *fname);
INT32 fs_close(INT32 fd);
INT32 fs_write(INT32 fd, const void *buf, UINT32 count);
INT32 fs_read(INT32 fd, void *buf, UINT32 count);
INT32 fs_lseek(INT32 fd, INT32 pos, INT32 cfg);
INT32 fs_file_len(INT8 *fname);
INT32 fs_ls(void);
INT32 fs_used(void);
INT32 fs_free(void);
#endif
//end exported
#endif
