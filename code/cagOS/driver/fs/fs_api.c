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
 * 2016.5.4, Ezio created
 *
 * DECRIPTION
 * fs api for sffs.
 *
 */

#include "../device/dfs/dfs.h"
#include <libc/string.h>
#include "sffs/sffs.h"


extern int intLock(void);
extern void intUnlock(int);
extern struct sffs sffs_slot[];

static INT32 fs_flash_read(void *fs,UINT32 ofs,void *data,UINT32 cnt)
{
    return sffs_read_at((struct sffs *)fs,ofs,data,cnt);
}

/*******************************************************************************
*
* fs_get_err - get file system's last error number
*
* This routine will return last error number ,which will be over wrote.
* When your operation return FS_FAIL, you can use this to get the fail reason.
*
* RETURNS: last error number
*
*/
INT32 fs_get_err(INT32 fsid)
{
    return sffs_get_errno(fsid);;
}

/*******************************************************************************
*
* fs_mount - mount file sytem
*
*
* RETURNS: FS_FAIL, if mount failed
*           <fs_id>, if mount successfully
*
*/
INT32 fs_mount(INT32 type)
{
    struct sffs *fs=&sffs_slot[0];
    INT32 ret = FS_FAIL;
    INT32 locker;
    locker = intLock();
    ret = sffs_mount(fs,type);
    intUnlock(locker);
    return ret;
}

#ifdef SFFS_MULTI_DEVICE
INT32 fs_mount2(struct sffs sfs, INT32 type)
{
    int id = sffs_register(sfs);
    FS_CKFSID(id);

    struct sffs *fs = sffs_get_slot(id);
    if (fs == NULL) {
        LOG_WARN("fs mount2 fail\n");
        return FS_FAIL;
    }
    INT32 ret = FS_FAIL;
    /*    INT32 locker;*/
    /*avoid mount after main()*/
    /*    locker = intLock();*/
    ret = sffs_mount(fs,type);
    /*    intUnlock(locker);*/
    if (ret == FS_OK)
        return id;
    else
        return ret;
}
#endif

/*******************************************************************************
*
* fs_umount - umount file system
*
*
* RETURNS: FS_OK, if successfully
*           FS_FAIL, if failed
*
*/
INT32 fs_umount(INT32 fsid)
{
#ifdef SFFS_MULTI_DEVICE
    FS_DEFI2;
#else
    FS_DEFI;
#endif
    ret = sffs_umount(fs);
    FS_DEFO;
}

#ifdef SFFS_MULTI_DEVICE
INT32 fs_umount2(INT32 fsid)
{
    struct sffs *fs=sffs_get_slot(fsid);

    if (fs != NULL)
        return sffs_umount(fs);
    else
        return FS_FAIL;
}
#endif



/*******************************************************************************
*
* fs_file_len - get file len
*
*
* RETURNS: FS_FAIL, if file isnot exist
*           <file len>, if file exists
*
*/
INT32 fs_file_len(INT8 *fname)
{
    FS_DEFI;
    if ((ret=sffs_find_filehdr(fs, fname)) < 0) {
        sffs_set_errno(fs,SFFS_FILENAME_NOT_EXIST_ERR4);
        FS_DEFO;
    }
    ret = fs->file[ret].size;
    FS_DEFO;
}

#ifdef SFFS_MULTI_DEVICE
INT32 fs_file_len2(INT8 *fname,INT32 fsid)
{
    FS_DEFI2;
    if ((ret=sffs_find_filehdr(fs, fname)) < 0) {
        sffs_set_errno(fs,SFFS_FILENAME_NOT_EXIST_ERR5);
        FS_DEFO;
    }
    ret = fs->file[ret].size;
    FS_DEFO;
}
#endif


#ifdef SFFS_MULTI_DEVICE
INT32 fs_rename2(const INT8 *old_name, const INT8 *new_name, INT32 fsid)
{
    FS_DEFI2;

    ret = sffs_rename(fs,old_name,new_name);

    FS_DEFO;
}
#endif

INT32 fs_rename(const INT8 *old_name, const INT8 *new_name)
{
    FS_DEFI;

    ret = sffs_rename(fs,old_name,new_name);

    FS_DEFO;
}

/*******************************************************************************
*
* fs_lseek - change file's file read/write pointer or get its current position.
* INT32 cfg: FS_SEEK_TELL, get file's r/w pointer
*            FS_SEEK_SET, set file's r/w pointer
* INT32 pos: used with FS_SEEK_SET, pos should not exceed file's length
*
* RETURNS: N/A
*
*/
INT32 fs_lseek(INT32 fd, INT32 pos, INT32 cfg)
{
#ifdef SFFS_MULTI_DEVICE
    INT32 fsid = (fd&0xffff0000)>>16 ;
    fd = sffs_get_fsfd(fd);
    FS_DEFI2;
#else
    FS_DEFI;
#endif

    if (sffs_ck_fd(fs,fd) < 0 || pos <0) {
        sffs_set_errno(fs,SFFS_FD_ERR2);
        LOG_WARN("seek para err\n");
        FS_DEFO;
    }

    switch (cfg) {
    case FS_SEEK_TELL:
        ret=fs->file[fd].ofs;
        break;
    case FS_SEEK_SET:
        ret=sffs_lseek(fs,fd,pos);
        break;
    default :
        sffs_set_errno(fs,SFFS_PARA_ERR6);
        ret = FS_FAIL;
        break;
    }
    FS_DEFO;
}
/*******************************************************************************
*
* fs_free - get storage's free space
*
* RETURNS: free space size
*
* NOTICE: the return value does not equal to total size minus all files' size beacause file ocupy a whole SECTOR even if it didnot use full of it.
*
*/
INT32 fs_free(void)
{
    FS_DEFI;
    INT32 i;
    ret = 0;
    for(i=0; i<sizeof(fs->free_se)/4; i++) {
        ret += get_all_one(fs->free_se[i]);
    }
    ret = fs->device_size - (ret +1)*fs->se_size;
    FS_DEFO;
}

#ifdef SFFS_MULTI_DEVICE
INT32 fs_free2(INT32 fsid)
{
    FS_DEFI2;
    INT32 i;
    ret = 0;
    for(i=0; i<sizeof(fs->free_se)/4; i++) {
        ret += get_all_one(fs->free_se[i]);
    }
    ret = fs->device_size - (ret +1)*fs->se_size;
    FS_DEFO;
}
#endif

/*******************************************************************************
*
* fs_used - get the size of all files
* NOTICE:
*
* RETURNS: all files' size
*
*/
INT32 fs_used(void)
{
    FS_DEFI;
    INT32 i;
    ret=0;
    for(i=0; i<SFFS_FILE_NUMBER; i++) {
        if (fs->file[i].comt == FS_COMMITED) {
            ret+=fs->file[i].size;
        }
    }
    FS_DEFO;
}

#ifdef SFFS_MULTI_DEVICE
INT32 fs_used2(INT32 fsid)
{
    FS_DEFI2;
    INT32 i;
    ret=0;
    for(i=0; i<SFFS_FILE_NUMBER; i++) {
        if (fs->file[i].comt == FS_COMMITED) {
            ret+=fs->file[i].size;
        }
    }
    FS_DEFO;
}
#endif
/*******************************************************************************
*
* fs_open - open file with specific flags and mode setting
*
* INT32 flags: FS_OPEN_NORMAL, return FS_FAIL if file does not exist
*              FS_OPEN_TRU, if file exists then deleted it and create a new file
*              FS_OPEN_CREATE, if file does not exist then create it
*
* INT32 mode:  FS_RDONLY,   open a file that cannot write
*              FS_WRONLY,   open a file that cannot read
*
* RETURNS: FS_FAIL, open file fail
*          <fd> , file's fd
*
*/
INT32 fs_open(const INT8 *fname, INT32 flags, INT32 mode)
{
#ifdef SFFS_MULTI_DEVICE
    int fsid = FS_ID_GET(mode);
    LOG_DEBUG("fsid %d\n",fsid);

    FS_GET_SFFS(fsid)

    FS_TRYLOCK(fs);
    FS_IS_MOUNTED(fs);

#else
    FS_DEFI;
#endif

    switch(flags) {
    case FS_OPEN_NORMAL:/*normal open :if file not exist then return fail*/
        if (sffs_find_filehdr(fs,fname) <0 ) {
            FS_DEFO;
        }
        break;
    case FS_OPEN_TRU:/*truncation*/
        sffs_remove_byname(fs,fname);
        break;
    case FS_OPEN_CREATE:/*if file not exist then create*/
        break;
    default:
        sffs_set_errno(fs,SFFS_PARA_ERR7);
        FS_DEFO;
    }
    mode = mode &0xffff;
    ret = sffs_open(fs,fname,mode);
    LOG_DEBUG("opend\n");

#ifdef SFFS_MULTI_DEVICE
    if (ret != FS_FAIL)
        ret = ret|(fsid<<16);
    LOG_DEBUG("open ret %d\n",ret);
#endif

    FS_DEFO;
}

/*******************************************************************************
*
* fs_remove - delete a file
*
*
* RETURNS: FS_FAIL, delete failed
*          FS_OK,  delete successfully
*
*/

#ifdef SFFS_MULTI_DEVICE
INT32 fs_remove2(const INT8 *fname ,int fsid)
{
    FS_GET_SFFS(fsid);
    FS_TRYLOCK(fs);
    FS_IS_MOUNTED(fs);
    ret = sffs_remove_byname(fs,fname);
    FS_DEFO;
}
#endif
INT32 fs_remove(const INT8 *fname)
{
    FS_DEFI;
    ret = sffs_remove_byname(fs, fname);
    FS_DEFO;
}

/*******************************************************************************
*
* fs_read - read file
* NOTICE: should only used in files that opened FS_RDONLY
*
* RETURNS: FS_FAIL, read failed
*           <len>, actual read bytes, if <len> does not equal to count, there may be some errors
*
*/
INT32 fs_read(INT32 fd, void *buf, UINT32 count)
{
#ifdef SFFS_MULTI_DEVICE
    INT32 fsid = (fd&0xffff0000)>>16 ;
    fd = sffs_get_fsfd(fd);
    FS_DEFI2;
#else
    FS_DEFI;
#endif
    FS_CKADDR(buf);
    ret = sffs_read(fs,fd, buf,count);
    FS_DEFO;
}

/*******************************************************************************
*
* fs_write - write file
* NOTICE: should only used in files that opened FS_WRONLY
*
* RETURNS: FS_FAIL, write failed
*           <len>, actual wrote bytes, if <len> does not equal to count, there may be some errors
*
*/
INT32 fs_write(INT32 fd, const void *buf, UINT32 count)
{
#ifdef SFFS_MULTI_DEVICE
    INT32 fsid = (fd&0xffff0000)>>16 ;
    fd = sffs_get_fsfd(fd);
    FS_DEFI2;
#else
    FS_DEFI;
#endif

    FS_CKADDR(buf);

    ret = sffs_write(fs, fd, buf, count);
    FS_DEFO;
}

/*******************************************************************************
*
* fs_close - close one file
*
*
* RETURNS: FS_FAIL, close file failed
*          FS_OK,   close file successfully
*
*/
INT32 fs_close(INT32 fd)
{
#ifdef SFFS_MULTI_DEVICE
    INT32 fsid = (fd&0xffff0000)>>16 ;
    fd = sffs_get_fsfd(fd);
    FS_DEFI2;
#else
    FS_DEFI;
#endif

    ret = sffs_close(fs,fd);
    FS_DEFO;
}

/*******************************************************************************
*
* fs_format - format file system
* INT32 type: FS_FORMAT_HDRS, only erase file headers
*             FS_FORMAT_ALL, erase all flash
*
* RETURNS: FS_FAIL, format failed
*          FS_OK, format successfully
*
*/
INT32 fs_format(INT32 type)
{
    FS_DEFI;
    LOG_DEBUG("format %d\n",type);
    ret = sffs_format(fs,type);
    return ret;
}

#ifdef SFFS_MULTI_DEVICE
INT32 fs_format2(INT32 fsid,INT32 type)
{
    FS_DEFI2;
    ret = sffs_format(fs,type);
    return ret;
}
#endif
/*******************************************************************************
*
* fs_ls - list files on flash
*
* This routine will show files include that deleted but not file header exists in flash, and opened files.
*
* RETURNS: FS_FAIL, operation failed
*          FS_OK,  operation successful
*
*/

INT32 fs_ls(void)
{
    FS_DEFI;
    struct sffs_entry files[FS_SS_NUM];
    INT32 i,j,k;
    INT8 tname[FNAME_LEN+1];
#if 1
    j=k=0;
    LOG_INFO("flash: %s\nfname    | size    | hdr_ss  | comt | deled |  magic    |   ofs     | valid  | fid \n",fs->fsname);
    for(i=0; i<(fs->se_size/fs->ss_size); i++) {
//        fs_flash_read(fs,(UINT32)(SFFS_SB_START*fs->se_size+i*fs->ss_size) ,&files[i],sizeof(struct sffs_entry));
        sffs_read_at(fs,(UINT32)(SFFS_SB_START*fs->se_size+i*fs->ss_size) ,&files[i],sizeof(struct sffs_entry));

        if(files[i].magic_num == SFFS_MAGIC) {
            if (files[i].name_len <=FNAME_LEN ) {
                memcpy(tname,files[i].name,files[i].name_len);
                tname[files[i].name_len]='\0';
                printf("%8s |%8d |   %3d   | 0x%2x | 0x%2x  |0x%8x |0x%08x |   %2d | 0x%08x \n",
                       tname, files[i].size, files[i].hdr_ss, files[i].comt, files[i].deled,
                       files[i].magic_num, files[i].ofs, files[i].deled==FS_EMPTY?1:0,files[i].fid);
                if(files[i].deled == FS_EMPTY)
                    k++;
                j++;
            }
        }
    }
    printf("flash has %d hdr ,%d file \n\n",j,k);
#endif
    j=0;
    LOG_INFO("memory:\nfname    | size    | hdr_ss  | comt | deled |  magic    | ofs     | stat | fid \n");
    for(i=0; i<SFFS_FILE_NUMBER; i++) {
        if (fs->file[i].name_len && fs->file[i].stat != 0 ) {
            printf("%8s |%8d |   %3d   |  %2d  |  %2d   |0x%8x |0x%08x |  %2d  | 0x%08x \n",
                   fs->file[i].name, fs->file[i].size, fs->file[i].hdr_ss, fs->file[i].comt==FS_COMMITED?1:0,
                   fs->file[i].deled==FS_EMPTY?0:1, fs->file[i].magic_num, fs->file[i].ofs, fs->file[i].stat,fs->file[i].fid);
            j++;
        }
    }
    printf("opened %d files.\n",j);
    FS_DEFO;
}

#ifdef SFFS_MULTI_DEVICE
INT32 fs_ls2(INT32 fsid)
{
    FS_DEFI2;
    struct sffs_entry files[FS_SS_NUM];
    INT32 i,j,k;
    INT8 tname[FNAME_LEN+1];
#if 1
    j=k=0;
    LOG_INFO("flash: %s\nfname    | size    | hdr_ss  | comt | deled |  magic    |   ofs     | valid  | fid \n",fs->fsname);
    for(i=0; i<(fs->se_size/fs->ss_size); i++) {
        fs_flash_read(fs,(UINT32)(SFFS_SB_START*fs->se_size+i*fs->ss_size) ,&files[i],sizeof(struct sffs_entry));
        if(files[i].magic_num == SFFS_MAGIC) {
            if (files[i].name_len <=FNAME_LEN ) {
                memcpy(tname,files[i].name,files[i].name_len);
                tname[files[i].name_len]='\0';
                LOG_INFO("%8s |%8d |   %3d   | 0x%2x | 0x%2x  |0x%8x |0x%08x |   %2d | 0x%08x \n",
                         tname, files[i].size, files[i].hdr_ss, files[i].comt, files[i].deled,
                         files[i].magic_num, files[i].ofs, files[i].deled==FS_EMPTY?1:0,files[i].fid);
                if(files[i].deled == FS_EMPTY)
                    k++;
                j++;
            }
        }
    }
    LOG_INFO("flash has %d hdr ,%d file \n\n",j,k);
#endif
    j=0;
    LOG_INFO("memory:\nfname    | size    | hdr_ss  | comt | deled |  magic    | ofs     | stat | fid \n");
    for(i=0; i<SFFS_FILE_NUMBER; i++) {
        if (fs->file[i].name_len && fs->file[i].stat != 0 ) {
            LOG_INFO("%8s |%8d |   %3d   |  %2d  |  %2d   |0x%8x |0x%08x |  %2d  | 0x%08x \n",
                     fs->file[i].name, fs->file[i].size, fs->file[i].hdr_ss, fs->file[i].comt==FS_COMMITED?1:0,
                     fs->file[i].deled==FS_EMPTY?0:1, fs->file[i].magic_num, fs->file[i].ofs, fs->file[i].stat,fs->file[i].fid);
            j++;
        }
    }
    LOG_INFO("opened %d files.\n",j);
    FS_DEFO;
}
#endif
