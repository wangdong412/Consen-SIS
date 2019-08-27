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
 * SFFS(simple flash file system) source file.
 *
 */

#include <libc/string.h>
#include "sffs.h"
#include "../../device/dfs/dfs.h"

extern int intLock(void);
extern void intUnlock(int);

/*related sffs root node*/
struct sffs sffs_slot[MAX_SLOT];

#ifdef SFFS_MULTI_DEVICE
INT32 hdrsStat[MAX_SLOT][FS_SS_NUM] = {{0}};
INT32 fsStat[MAX_SLOT] = {0};
INT32 sffs_erase_time[MAX_SLOT][FS_SE_NUM]= {{0}}; /*record relative sector erase time*/
#else
INT32 hdrsStat[FS_SS_NUM] = {0};
INT32 fsStat = 0;
INT32 sffs_erase_time[FS_SE_NUM]= {0}; /*record relative sector erase time*/
#endif

void NOLOG(const char *c,...)
{
}

/*spin lock, timeout*/
/*return 0 ok, >0 has locked, <0 error*/
INT32 sffs_lock(struct sffs *fs)
{
    INT32 ret = 0;
    if(fs->lock >0) {
        ret = FS_FAIL;
        LOG_ERROR("locked %d\n",fs->lock);
        while(1);
    } else {
        fs->lock=1;
        ret = FS_OK;
    }
    return ret;
}

/* 0 ok,<0 error*/
INT32 sffs_unlock(struct sffs *fs)
{
    INT32 ret = 0;
    if(fs->lock >0) {
        fs->lock=0;
        ret = FS_OK;
    } else {
        ret = FS_FAIL;
        LOG_ERROR("unlocked %d\n",fs->lock);
        while(1);
    }
    return ret;
}

/*un-interruptable*/
void sffs_protect(void)
{
}

void sffs_unprotect(void)
{
}

#ifdef SFFS_MULTI_DEVICE
INT32 sffs_get_fsfd(INT32 fd)
{
    return fd&0xffff;
}
#endif

#if 0
static int sffs_get_fsid(char *fname)
{
    int id = 0;

    return id;
}
#endif

void sffs_set_errno(struct sffs *fs,INT32 errno)
{
    fs->errno=errno;
    LOG_DEBUG("err %d \n",errno);
}

INT32 sffs_get_errno(INT32 fsid)
{
#ifdef SFFS_MULTI_DEVICE
    FS_DEFI2;
#else
    FS_DEFI;
#endif
    ret = fs->errno;
    FS_DEFO;
}

inline INT32 sffs_read_at(struct sffs *fs, UINT32 offset, void *data,UINT32 size)
{
    INT32 ret = FS_FAIL;
    if(fs->read != NULL) {
        ret = fs->read(fs->flashId, offset, (UINT8 *)data, size);
    }
    return ret;
}

static inline INT32 sffs_write_at(struct sffs *fs, UINT32 offset, const void *data, UINT32 size)
{
    INT32 ret = FS_FAIL;
    if(fs->write != NULL) {
        ret = fs->write(fs->flashId, offset, (UINT8 *)data, size);
    }
    return ret;
}

/*in S29GL512P ,this is a sector erase , and in N25Q64A will add or replaced by a sub_sector erase*/
static inline INT32 sffs_erase_at(struct sffs *fs, UINT32 no_from, UINT32 num)
{
    INT32 ret = FS_FAIL;
    int i;
    if(fs->erase != NULL && no_from <FS_SE_NUM ) {
        ret = fs->erase(fs->flashId, no_from,num);
        for(i=0; i<num; i++) /*record sector erase times*/
#ifdef SFFS_MULTI_DEVICE
            sffs_erase_time[fs->fsid][no_from+i]++;
#else
            sffs_erase_time[no_from+i]++;
#endif
    }
    return ret;
}

/*get free se and mark it in fs*/
/*******************************************************************************
*
* sffs_get_se -  get a un-used sector
*
*
* RETURNS: FS_FAIL, if there's no unused sector
*           <secotr no>, unused sector's no
*
*/
static INT32 sffs_get_se(struct sffs *fs)
{
    INT16 i;
    INT32 j,k;
    i = 0;
    j = k = 0;
    /*检查一遍flash*/
    /*从记录的当前free se 开始 for WL*/
    i=fs->free_se_cur;
    for( j=0; j<fs->device_size/fs->se_size; j++) {
        k = get_bit_val(fs->free_se,i);
        if (k == 0) {
            sffs_protect();
            set_bit_val(fs->free_se,i);/*mark*/
            fs->free_se_cur = i;
            sffs_unprotect();
            return i;
        }
        i = (i+1)%(INT16)(fs->device_size/fs->se_size);
        i = (i==0)?1:i; /*round*/
    }
    sffs_set_errno(fs,SFFS_NO_FREE_SE);
    return FS_FAIL;
}

/*******************************************************************************
*
* sffs_set_sebit - mark one sector is used
*
* set sector's respective bit in struct sffs
*
*
* RETURNS: N/A
*
*/
static void sffs_set_sebit(struct sffs *fs, INT16 se_no)
{
    set_bit_val(fs->free_se, se_no);
}

/*******************************************************************************
*
* sffs_clear_sebit - mark one sector as unused
*
* clear sector's respective bit in struct sffs
*
*
* RETURNS: N/A
*
*/
static void sffs_clear_sebit(struct sffs *fs,INT16 se_no)
{
    clear_bit_val(fs->free_se,se_no);
}

static UINT8 sffs_ckstat(struct sffs *fs,INT32 fd)
{
    return fs->file[fd].stat;
}

INT32 sffs_ck_fd(struct sffs *fs, INT32 fd)
{
    /*ok: 1. fd value;2. fd is opened*/
    if (0<= fd && fd<SFFS_FILE_NUMBER && fs->file[fd].size >=0 ) {
        return FS_OK;
    } else {
        return FS_FAIL;
    }
}

/*has same fid ,return i, else return FS_FAIL*/
static INT32 sffs_check_fid(struct sffs *fs,INT32 fid) /*just run mount*/
{
    for(INT32 i=0; i < fs->fileNum; i++) {
        if (fs->file[i].fid == fid)
            return i;
    }

    return FS_FAIL;
}

/*******************************************************************************
*
* sffs_get_hdrss - get a empty file hdr in flash
*
*
* RETURNS: return the sub-sector No. of the empty file hdr
*
*/
static INT32 sffs_get_hdrss(struct sffs *fs, INT32 fd)
{
    INT32 i,j,k;

    k=0;
    if(fs->fileNum >= SFFS_FILE_NUMBER) {/*hdr full*/
        sffs_set_errno(fs,SFFS_NO_FREEHDRSS_ERR);
        return FS_FAIL;
    }

    /*try to get an empyt hdr_ss*/
    for(i=fs->free_ss_hdr_cur, j=0; j<(fs->se_size/fs->ss_size); j++) {
        if (i == -1)
            i=0;

        if (fs->pageErase == SFFS_PAGE_ERASE) {
            if (fs->free_ss_hdr[i] != FS_HDRS_USED && fs->free_ss_hdr[i] != FS_HDRS_BAD) {
                fs->free_ss_hdr[i] = FS_HDRS_TMP;
                fs->free_ss_hdr_cur = i;
                return i;
            }
        } else {
            /*hdr ss is free and erased*/
#ifdef SFFS_MULTI_DEVICE
            if (hdrsStat[fs->fsid][i] == FS_HDRS_EMPTY && fs->free_ss_hdr[i] == FS_HDRS_EMPTY) {
                hdrsStat[fs->fsid][i] = FS_HDRS_TMP;/*occupy*/
#else
            if (hdrsStat[i] == FS_HDRS_EMPTY && fs->free_ss_hdr[i] == FS_HDRS_EMPTY) {
                hdrsStat[i] = FS_HDRS_TMP;/*occupy*/
#endif
                fs->free_ss_hdr[i] = FS_HDRS_TMP;
                fs->free_ss_hdr_cur = i;
                return i;
            }
        }
        i=(i+1)%(fs->se_size/fs->ss_size);
    } /*end for*/

    /*in S29GL512P or other flash that not support page erase ,will exist hdr_ss not used but not erased*/
    if (fs->pageErase != SFFS_PAGE_ERASE) {
        /*erase hdr sector*/
TRY_TWICE:
        if (sffs_erase_at(fs, SFFS_SB_START, ERASE_NUM_PER)<0) {
            if (sffs_erase_at(fs, SFFS_SB_START, ERASE_NUM_PER)<0) {
                sffs_set_errno(fs,SFFS_ERASE_HDRSS_ERR);
                return FS_FAIL-1;
            }
        }
        /*write old hdrs to flash*/
        UINT32 ofs;

#ifdef SFFS_MULTI_DEVICE
        memset(&hdrsStat[fs->fsid],FS_HDRS_EMPTY,FS_SS_NUM);
#else
        memset(hdrsStat,FS_HDRS_EMPTY,sizeof(hdrsStat));
#endif
        memset(fs->free_ss_hdr,FS_HDRS_EMPTY,sizeof(fs->free_ss_hdr));

        fs->free_ss_hdr_cur=0;
        j=0;
        /*write hdrs in memory to flash*/
        /*check effective file list and write hdr to flash*/
        for (i=0; i<SFFS_FILE_NUMBER; i++) {
            if (fs->file[i].deled == FS_EMPTY) { /*file is not deleted */
                if (fs->file[i].comt == FS_COMMITED) { /* file is committed */

                    ofs = SFFS_SB_START*fs->se_size + fs->file[i].hdr_ss * fs->ss_size;

                    fs->free_ss_hdr[fs->file[i].hdr_ss] = FS_HDRS_USED;
#ifdef SFFS_MULTI_DEVICE
                    hdrsStat[fs->fsid][fs->file[i].hdr_ss] = FS_HDRS_USED;
#else
                    hdrsStat[fs->file[i].hdr_ss] = FS_HDRS_USED;
#endif

                    if ( sffs_write_at(fs,ofs, &(fs->file[i]),
                                       (UINT32 )(&(fs->file[i].deled))-(UINT32)(&(fs->file[i])))
                         < (UINT32 )(&(fs->file[i].deled))-(UINT32)(&(fs->file[i]))) {
                        if (k<1) { /* try to do twice*/
                            LOG_WARN("erase hdr and re-write\n");
                            k++;
                            goto TRY_TWICE;
                        }
                        fs->free_ss_hdr[fs->file[i].hdr_ss] = FS_HDRS_BAD;
#ifdef SFFS_MULTI_DEVICE
                        hdrsStat[fs->fsid][fs->file[i].hdr_ss] = FS_HDRS_BAD;
#else
                        hdrsStat[fs->file[i].hdr_ss] = FS_HDRS_BAD;
#endif
                        LOG_ERROR("write hdrss fail\n");
                        /*keep it as it is and change when actual hardware comes out*/
                        fs->file[i].comt = FS_FERR;
                        sffs_set_errno(fs,SFFS_WRITE_HDRSS_ERR);
                        fs->fileNum -- ;
                        return FS_FAIL-2; /*it's better not to return  here*/
                    }
                    j++;
                } else if (fs->file[i].stat !=FS_CLOSED  && fs->file[i].hdr_ss != FS_S_UNUSED ) {
#ifdef SFFS_MULTI_DEVICE
                    hdrsStat[fs->fsid][fs->file[i].hdr_ss]=FS_HDRS_TMP;
#else
                    hdrsStat[fs->file[i].hdr_ss]=FS_HDRS_TMP;
#endif
                    fs->free_ss_hdr[fs->file[i].hdr_ss]=FS_HDRS_TMP;
                    j++;
                }/*end of FS_COMMITED*/
            }/*end of FS_EMPTY*/
        }/*end of for loop*/

        if(j<SFFS_FILE_NUMBER) {
            for(i=0; i<(fs->se_size/fs->ss_size); i++) {
                LOG_DEBUG("ii %d %d %d\n",i, fs->free_ss_hdr[i] , hdrsStat[fs->fsid][i]);
#ifdef SFFS_MULTI_DEVICE
                if (fs->free_ss_hdr[i] == FS_HDRS_EMPTY && hdrsStat[fs->fsid][i]==FS_HDRS_EMPTY) {
                    fs->free_ss_hdr_cur = i;
                    hdrsStat[fs->fsid][i]=FS_HDRS_TMP;
#else
                if (fs->free_ss_hdr[i] == FS_HDRS_EMPTY && hdrsStat[i]==FS_HDRS_EMPTY) {
                    fs->free_ss_hdr_cur = i;
                    hdrsStat[i]=FS_HDRS_TMP;
#endif
                    fs->free_ss_hdr[i]=FS_HDRS_TMP;
                    return fs->free_ss_hdr_cur;
                }
            }
        }
        else {
            fs->free_ss_hdr_cur = -1;
        }
    }/*end for if*/

/*    sffs_set_errno(fs,SFFS_NO_FREEHDRSS_ERR1);*/
    LOG_DEBUG("jj %d\n",j);
    return FS_FAIL-3;
}

/*temporary not used*/
/*clear file hdr and relate var in struct sffs*/
static inline void sffs_reset_filehdr(struct sffs *fs, INT32 fd)
{
    INT32 i ,j;
    i = fd;

    fs->file[i].magic_num   = 0;
    memset(fs->file[i].name,0x0,FNAME_LEN);
    fs->file[i].name_len    = 0;
    fs->file[i].size        = -1;
    fs->file[i].ofs         = -1;

    /*return SEs*/
    for(j=0; j<SFFS_BNO_LIM && fs->file[i].se_no[j]!=FS_S_UNUSED; j++) {
        sffs_clear_sebit(fs,fs->file[i].se_no[j]);
        fs->file[i].se_no[j]=FS_S_UNUSED ;
    }

    fs->file[i].hdr_ss  = FS_S_UNUSED;
    fs->file[i].comt    = FS_EMPTY;
    fs->file[i].unused  = FS_EMPTY;
    fs->file[i].deled   = FS_EMPTY;
    fs->file[i].fid     = 0;
    fs->file[i].seq     = 0;
    fs->file[i].stat    = FS_CLOSED;
}

/*******************************************************************************
*
* sffs_find_filehdr - find the specifil filehdr in memory with the given file name
*
*
* RETURNS: file hdr's number or FS_FAIL if file not exists
*
*/
INT32 sffs_find_filehdr(struct sffs *fs,const INT8 *fname)
{
    INT32 i;

    for(i=0; i<SFFS_FILE_NUMBER; i++) {
        /*file not deleted and has same name*/
        if (fs->file[i].deled != FS_DELETED &&
            fs->file[i].name_len> 0 &&
            strlen((char *)fname) == fs->file[i].name_len &&

            strncmp((char *)fname, (char *)fs->file[i].name, fs->file[i].name_len) == 0 ) {
            return i;
        }
    }
/*    sffs_set_errno(fs,SFFS_FILENAME_NOT_EXIST_ERR);*/
    return FS_FAIL;
}

/*******************************************************************************
*
* sffs_find_filehdr - get a unused file hdr
*
*
* RETURNS: file hdr's number or FS_FAIL if no usable file hdr
*
*/
static INT32 sffs_get_filehdr(struct sffs *fs)
{
    INT32 i;
    if (fs->fileNum >= SFFS_FILE_NUMBER) {
        /*file full*/
        sffs_set_errno(fs,SFFS_FILE_NUM_FULL_ERR);
        return FS_FAIL;
    }

    for(i=0; i<SFFS_FILE_NUMBER; i++) {
        if (fs->file[i].hdr_ss < 0) {
            return i;
        }
    }
    sffs_set_errno(fs,SFFS_GLOBAL_ERR);
    return FS_FAIL;
}

INT32 sffs_is_mounted(struct sffs *fs)
{
    return fs->mounted;
}

/* write data to flash. */
/*only support sequence write and must start with SS' border*/

/*******************************************************************************
*
* sffs_write_4k - write data to flash
* This routine will write data to flash. It can write at most 4k bytes one time. It should start from sub-sector's border and cannot across sub-sectors.
*
* RETURNS: actual wrote data length.or return FS_FAIL if the operation is failed.
*
*/
static INT32 sffs_write_4k(struct sffs *fs, INT32 fd, const void *data, INT32 size)
{
    struct sffs_entry *file;
    UINT32 offset;
    INT32 se_in_file,ss_in_se, ss_no;
    INT16 se_no ;
    INT32 ret ;

    se_no = ss_no = -1;
    if(size == 0)
        return 0;

    /*get file hdr*/
    file = &(fs->file[fd]);

    /*除了最后一次write，其它每次write 都必须是ss_size byte，*/
    /*每次写都申请一个new ss*/
    /*先占一个se，每次写的时候在用一个ss*/
    if(file->size % fs->se_size == 0) {
        se_no = sffs_get_se(fs);
        if(se_no < 1) {
            LOG_WARN("SFFS: no space left on device %d\n",se_no );
            sffs_set_errno(fs,SFFS_NO_FREE_SE);
            return FS_FAIL;
        }
        LOG_DEBUG("write got se %d\n",se_no);
        file->se_no[file->size/fs->se_size] = se_no;
        /*不支持页擦除，则每次申请sector 时擦除整块*/
        if (fs->pageErase != SFFS_PAGE_ERASE) {
            /*erase one whole sector when firstly programs in a new sector*/
            ret = sffs_erase_at(fs,(UINT32)se_no,ERASE_NUM_PER);
            if (ret <0) {
                ret = sffs_erase_at(fs,se_no,ERASE_NUM_PER);
                if (ret <0) {
                    LOG_WARN("erase new sector fail ret %d no %d\n",ret,se_no);
                    sffs_set_errno(fs,SFFS_ERASE_DATA_ERR);
                    return FS_FAIL;
                }
            }
        }
        ss_no = se_no*(fs->se_size/fs->ss_size);
        LOG_DEBUG("write ss %d\n",ss_no);
    } else {
        if(file->size%fs->ss_size != 0) {
            LOG_INFO("write not aligined ss_size %d %d\n",file->size,size);
            sffs_set_errno(fs,SFFS_UNALIGN_WRITE_ERR);
            return FS_FAIL;
        }
        se_in_file  = file->size / fs->se_size;
        ss_in_se    = file->size % fs->se_size / fs->ss_size;
        ss_no       = file->se_no[se_in_file] * (fs->se_size/fs->ss_size)+ ss_in_se;
        LOG_DEBUG("write in ss %d\n",ss_no);
    }
    offset = ss_no * fs->ss_size;

    /* writing data */
    if(fs->pageErase == SFFS_PAGE_ERASE)
        sffs_erase_at(fs,ss_no,ERASE_NUM_PER);

    ret = sffs_write_at(fs, offset, data, size);
    if(ret == size) {
        file->size += ret;
        file->ofs += ret;
    }
    return ret;
}

/* read data from flash */
/*******************************************************************************
*
* sffs_read_4k - read data from flash
* This routine will read data from flash. It can read at most 4k bytes one time. It should start from sub-sector's border and cannot across sub-sectors.
*
* RETURNS: actual read data length.or return FS_FAIL if the operation is failed.
*
*/
static INT32 sffs_read_4k(struct sffs *fs, INT32 fd, void *data, INT32 size)
{
    struct sffs_entry *file;
    INT32 offset, ss_no_in,se_no_in,ret;

    if (size == 0)
        return 0;

    /*get file hdr*/
    file = &(fs->file[fd]);

    if(file->size ==0 || file->ofs == file->size-1)
        return 0;

    if(file->ofs + size > file->size)
        size = file->size - file->ofs;

    /*ofs should link to the ss_no*/
    offset   = file->ofs;
    /*got se no in se_no[]*/
    se_no_in = offset/fs->se_size;
    /*calcuate ss no in a se*/
    ss_no_in = offset%fs->se_size/fs->ss_size;
    /*calcuate the address in flash : se*se_size + ss_in_se*ss_size + offset in ss*/
    offset   = offset%fs->se_size%fs->ss_size + file->se_no[se_no_in]*fs->se_size + ss_no_in*fs->ss_size;

    LOG_DEBUG("we read se %d ss %d ofs %d\n",file->se_no[se_no_in],ss_no_in,offset);

    if (offset % fs->ss_size + size > fs->ss_size) {
        size = fs->ss_size - offset % fs->ss_size;
    }

    ret  = sffs_read_at(fs,offset,data, size);

    if (ret <0){
        return ret;
    }

    if (ret < size) {
        file->ofs += ret;
        sffs_set_errno(fs,SFFS_READ_DATA_ERR);
        return ret;
    }
    file->ofs += size;
    return ret;
}

void sffs_slot_clear(void)
{
    for(int jj=0; jj<MAX_SLOT; jj++) {
        sffs_slot[jj].flashId = -1;
        sffs_slot[jj].fsid = -1;
    }
}

/*******************************************************************************
*
* sffs_start- init sffs' data structure
*
* NOTICE: struct sffs' operation and flash specific parameters are init by hand.
*
* RETURNS: FS_FAIL, if mount failed
*           <fs_id>, if mount successfully
*
*/
/* init memory structure */
static INT32 sffs_start(struct sffs *fs)
{
    INT32 i,j;
    /*file*/
    for(i=0; i<SFFS_FILE_NUMBER; i++) {
        memset(fs->file[i].name,0x0,FNAME_LEN);
        fs->file[i].hdr_ss = FS_S_UNUSED;
        fs->file[i].name_len = 0;
        fs->file[i].size = -1;
        fs->file[i].ofs = -1;
        for(j=0; j<SFFS_BNO_LIM; j++) {
            fs->file[i].se_no[j]=FS_S_UNUSED;
        }
        fs->file[i].magic_num   = 0;
        fs->file[i].comt        = FS_EMPTY;
        fs->file[i].unused      = FS_EMPTY;
        fs->file[i].deled       = FS_EMPTY;
        fs->file[i].unused2[0]  = FS_EMPTY;
        fs->file[i].unused2[1]  = FS_EMPTY;
    }
    /*fs*/
    memset(fs->free_se,0,sizeof(fs->free_se));
    memset(fs->free_ss_hdr,FS_HDRS_TMP,sizeof(fs->free_ss_hdr));

#ifdef SFFS_MULTI_DEVICE
    memset(&hdrsStat[fs->fsid],0,FS_SS_NUM);
#else
    memset(hdrsStat,0,sizeof(hdrsStat));
#endif

    /*give real value after mount*/
    fs->free_ss_hdr_cur = FS_S_UNUSED;
    fs->free_se_cur = 1;
    fs->magic_num = SFFS_MAGIC;
    fs->fileNum = 0;
    fs->fidcnt = 0;
    fs->seqcnt= 0;
    fs->lock = 0;

    LOG_DEBUG("flash size %d\n",fs->device_size);
    return FS_OK;
}

/*******************************************************************************
*
* sffs_open - open file by name
*
* NOTICE: open file by name ,if file is not exist then create it
*
* RETURNS: FS_FAIL, if mount failed
*           <fs_id>, if mount successfully
*
*/
INT32 sffs_open(struct sffs *fs,const INT8 *fname,INT32 mode)
{
    INT32 pos ;
    struct sffs_entry *file;

    if (fname == NULL) {
        sffs_set_errno(fs,SFFS_PARA_ERR);
        return FS_FAIL;
    }

    pos = sffs_find_filehdr(fs,fname);
    /*init new file hdr(mem) if not found file*/
    if (pos < 0) {
        /*MUST get file hdr before get hdr ss*/
        pos = sffs_get_filehdr(fs);
        if (pos == FS_FAIL ) {
            LOG_ERROR("SFFS: get free file hdr fail!\n");
            return FS_FAIL;
        }
        if ( sffs_get_hdrss(fs,pos) == FS_FAIL) {
            LOG_ERROR("SFFS: get flash hdr fail\n");
            return FS_FAIL;
        }

        file = &(fs->file[pos]);

        if (strlen((char *)fname) > FNAME_LEN) {
            file->name_len = FNAME_LEN;
            LOG_WARN("file name too long %s %d\n",fname,file->name_len);
        } else {
            file->name_len = strlen((char *)fname);
        }

        strncpy((char *)file->name, (char *)fname, file->name_len);
        file->name[file->name_len]='\0';
        file->size = 0;
        file->ofs = 0;
        file->hdr_ss = fs->free_ss_hdr_cur;
        fs->fileNum ++;

        LOG_DEBUG("open file %s hdr %d\n",fname,fs->free_ss_hdr_cur);
        /*sffs_set_free_hdrss(fs); */

    } else {
        file = &(fs->file[pos]);
    }

    if (mode == FS_RDONLY) {
        /*read only*/
        file->stat  = FS_RDONLY;
    } else if (mode == FS_WRONLY) {
        /*write only*/
        file->stat = FS_WRONLY;
    } else {
        /*default as readonly*/
        file->stat = FS_RDONLY;
    }
    LOG_DEBUG("open %s fd %d\n",fname,pos);
    return pos;
}

/* file ops for user use*/
/*close and save file*/
INT32 sffs_close(struct sffs *fs, INT32 fd)
{
    UINT32 ofs;

    if (sffs_ck_fd(fs,fd) < 0) {
        sffs_set_errno(fs,SFFS_FD_ERR);
        LOG_WARN("fd er %d\n",fd);
        return FS_FAIL;
    }

    if(fs->file[fd].comt == FS_COMMITED) {/*file has been stored in flash*/
        fs->file[fd].ofs = 0;
        fs->file[fd].stat = FS_CLOSED;/* set file stat into zen*/
        LOG_INFO("commited\n");
        return FS_OK;
    }

    fs->file[fd].magic_num = fs->magic_num;
    fs->file[fd].ofs = 0;
    fs->file[fd].comt= FS_COMMITED;

    ofs = SFFS_SB_START * fs->se_size +fs->file[fd].hdr_ss*fs->ss_size;

    LOG_DEBUG("close hno %d ofs %d size %d\n",fs->file[fd].hdr_ss, ofs, sizeof(struct sffs_entry));
    LOG_DEBUG("cf %d len %d stat %d\n",fd,fs->file[fd].size, sffs_ckstat(fs,fd));

    /*file hdr should be written in write mode*/
	if (sffs_ckstat(fs, fd) == FS_WRONLY)
	{

		if (fs->pageErase != SFFS_PAGE_ERASE) {
			/*first time to write hdr_ss to flash,so erase hdr se for safty*/
#ifdef SFFS_MULTI_DEVICE
			if (fsStat[fs->fsid] == 1) {
#else
			if (fsStat == 1) {
#endif
				INT32 ret = 0;
				ret = sffs_erase_at(fs, SFFS_SB_START, ERASE_NUM_PER);
				if (ret < 0) {
					ret = sffs_erase_at(fs, SFFS_SB_START, ERASE_NUM_PER);
					if (ret < 0) {
						LOG_WARN("erase new sector in HDRSE fail ret %d \n", ret);
						sffs_set_errno(fs, SFFS_ERASE_HDRSS_ERR);
						return FS_FAIL;
					}
				}
#ifdef SFFS_MULTI_DEVICE
				fsStat[fs->fsid] = 2;
#else
				fsStat = 2;
#endif
			}
			}

		fs->file[fd].fid = fs->fidcnt;
		fs->file[fd].seq = fs->seqcnt;

		/*not write bit [deled] and [stat]*/
		if (sffs_write_at(fs, ofs, &(fs->file[fd]), (UINT32)(&(fs->file[fd].deled)) - (UINT32)(&(fs->file[fd])))
			< (INT32)((UINT32)(&(fs->file[fd].deled)) - (UINT32)(&(fs->file[fd])))) {
			/*only support page erase can do re-write*/
			/*erase page before write a page if page-erase supported*/
			if (fs->pageErase == SFFS_PAGE_ERASE) {
				/*no need to check erase ok or fail*/
				sffs_erase_at(fs, SFFS_SB_START*(fs->se_size / fs->ss_size) + fs->file[fd].hdr_ss, ERASE_NUM_PER);
				if (sffs_write_at(fs, ofs, &(fs->file[fd]),
					(UINT32)(&(fs->file[fd].deled)) - (UINT32)(&(fs->file[fd])))
					< (INT32)((UINT32)(&(fs->file[fd].deled)) - (UINT32)(&(fs->file[fd])))) {

					LOG_WARN("p close file %d fail name %s\n", fd, fs->file[fd].name);

					fs->free_ss_hdr[fs->file[fd].hdr_ss] = FS_HDRS_BAD;

					fs->file[fd].comt = FS_FERR; /*mark as not finished*/
					fs->file[fd].fid = 0;
					fs->file[fd].seq = 0;
					fs->fileNum--;
					sffs_set_errno(fs, SFFS_WRITE_HDRSS_ERR);
					return FS_FAIL;
				}
			}
			else {
				LOG_DEBUG("np close file %d fail name %s\n", fd, fs->file[fd].name);

#ifdef SFFS_MULTI_DEVICE
				hdrsStat[fs->fsid][fs->file[fd].hdr_ss] = FS_HDRS_TMP;
#else
				hdrsStat[fs->file[fd].hdr_ss] = FS_HDRS_TMP;
#endif
				fs->free_ss_hdr[fs->file[fd].hdr_ss] = FS_HDRS_TMP;

				fs->file[fd].comt = FS_FERR; /*mark as not finished*/
				fs->file[fd].fid = 0;
				fs->file[fd].seq = 0;
				fs->fileNum--;
				sffs_set_errno(fs, SFFS_WRITE_HDRSS_ERR);
				return FS_FAIL;
			}
		}
		}
    if(fs->pageErase != SFFS_PAGE_ERASE) {
#ifdef SFFS_MULTI_DEVICE
        hdrsStat[fs->fsid][fs->file[fd].hdr_ss] =FS_HDRS_USED;
#else
        hdrsStat[fs->file[fd].hdr_ss] =FS_HDRS_USED;
#endif
        fs->free_ss_hdr[fs->file[fd].hdr_ss]=FS_HDRS_USED;
    }
    fs->fidcnt ++;
    fs->seqcnt ++;
    fs->file[fd].stat = FS_CLOSED;/* set file stat into zen*/
    return FS_OK;
}

/*******************************************************************************
*
* sffs_write - write data to flash
* This routine will write data to flash. It will split buf into several piece and call sffs_write_4k write these data to flash
*
* RETURNS: actual wrote data length.or return FS_FAIL if the operation is failed.
*
*/
INT32 sffs_write(struct sffs *fs, INT32 fd, const void *data, INT32 size)
{
    UINT8 *temp;
    INT32 size1;
    struct sffs_entry *file;
    INT32 ofsb=0;

    temp = (UINT8 *)data;
    /*check fd is legal and size legal*/
    if (sffs_ck_fd(fs,fd) < 0 || size <0 || size > SFFS_MAX_FILES_SIZE) {
        sffs_set_errno(fs,SFFS_PARA_ERR);
        return FS_FAIL;
    }

    file = &(fs->file[fd]);
    if (file->stat != FS_WRONLY) {
        LOG_WARN("not in correct mode\n");
        sffs_set_errno(fs,SFFS_WRITEMODE_ERR);
        return FS_FAIL;
    }
    if (size == 0)
        return 0;

    /*file can be write only one time*/
    if(file->comt == FS_COMMITED) {
        LOG_INFO("file cannot be modify");
        sffs_set_errno(fs,SFFS_FILE_REWRITE_ERR);
        return FS_FAIL;
    }

    ofsb = file->ofs;
    if(file->ofs + size > SFFS_MAX_FILES_SIZE) {
        sffs_set_errno(fs,SFFS_FILE_SIZE_EXCED_ERR);
        return FS_FAIL;
    }

    /*read-combin-write(concern for byte align)(can be optimized)*/
    /*concern for non-word align*/
    size1 = fs->ss_size - file->ofs%fs->ss_size;
/*    ofs = ofsb % fs->ss_size;*/
    if (size1 != fs->ss_size) {
        if (size < size1) {
            size1 =size;
        }
        if (sffs_write_4k(fs,fd,temp,size1) < (size1)) {
#if 0
            if (fs->pageErase == SFFS_PAGE_ERASE) {

                sffs_erase_at(fs,fs->file[fd].ofs/fs->ss_size,ERASE_NUM_PER);

                if (sffs_write_4k(fs,fd,buf,ofs+size1) < (ofs+size1)) {
                    sffs_set_errno(fs,SFFS_WRITE_DATA_ERR);
                    return file->ofs-ofsb;
                }
            } else
#endif
            {
                sffs_set_errno(fs,SFFS_WRITE_DATA_ERR);
                return file->ofs - ofsb;
            }
        }
        temp += size1;
        size -= size1;
    }

    size1 = fs->ss_size;
    if(size > fs->ss_size) {

        while(size >= fs->ss_size) {
            if (sffs_write_4k(fs,fd,temp,size1)<size1) {

                if (fs->pageErase == SFFS_PAGE_ERASE) {
                    sffs_erase_at(fs,fs->file[fd].ofs/fs->ss_size,ERASE_NUM_PER);
                    if (sffs_write_4k(fs,fd,temp,size1)<size1) {
                        sffs_set_errno(fs,SFFS_WRITE_DATA_ERR);
                        return file->ofs-ofsb;
                    }

                } else {
                    sffs_set_errno(fs,SFFS_WRITE_DATA_ERR);
                    return file->ofs-ofsb;
                }

            }
            size -= size1;
            temp += size1;
        }
    }

    if(size >0) {

        if (sffs_write_4k(fs,fd,temp,size) < size) {

            if (fs->pageErase == SFFS_PAGE_ERASE) {
                sffs_erase_at(fs,fs->file[fd].ofs/fs->ss_size,ERASE_NUM_PER);
                if (sffs_write_4k(fs,fd,temp,size) < size) {
                    sffs_set_errno(fs,SFFS_WRITE_DATA_ERR);
                    return file->ofs-ofsb;
                }
            } else {
                sffs_set_errno(fs,SFFS_WRITE_DATA_ERR);
                return file->ofs-ofsb;
            }
        }

        size = 0;
    }
    return file->ofs-ofsb;
}

/*******************************************************************************
*
* sffs_read - read data from flash
* This routine will read data from flash. It will split buf into several piece and call sffs_read_4k read these data from flash
*
* RETURNS: actual wrote data length.or return FS_FAIL if the operation is failed.
*
*/
INT32 sffs_read(struct sffs *fs, INT32 fd, void *data, INT32 size)
{
    struct sffs_entry *file;
    INT32 size1,ret;
    UINT8 *temp;
    INT32 ofsb;

    ret = size;
    size1 =size;
    temp = (UINT8 *)data;
    /*check fd is legal*/
    if (sffs_ck_fd(fs,fd) < 0 || size < 0 || size >SFFS_MAX_FILES_SIZE) {
        sffs_set_errno(fs,SFFS_PARA_ERR);
        return FS_FAIL;
    }

    if (size == 0)
        return 0;

    /*divide data into three parts:not aligned start,aligned middle and not aligned end*/
    file = &(fs->file[fd]);
    if (file->stat != FS_RDONLY) {
        LOG_WARN("not in read mode\n");
        sffs_set_errno(fs,SFFS_READMODE_ERROR);
        return FS_FAIL;
    }
    ofsb = file->ofs;

    if(file->ofs + size > file->size) {
        size = file->size - file->ofs;
        if (size<0) { /* no need ?*/
            LOG_WARN("ofs exceed %d \n",file->ofs);
            sffs_set_errno(fs,SFFS_OFFSET_ERR);
            return size;
        }

        LOG_WARN("exceed file max len %d,can only read %d \n",SFFS_MAX_FILES_SIZE,size);
        sffs_set_errno(fs,SFFS_FILE_SIZE_EXCED_ERR);
    }

    if (file->ofs%fs->ss_size !=0) {
        size1 = fs->ss_size - file->ofs%fs->ss_size;
        if (size1 > size) {
            size1 = size;
        }
        ret = sffs_read_4k(fs,fd,temp,size1);
        if (ret < size1) {
            sffs_set_errno(fs,SFFS_READ_DATA_ERR);
            return ret;
        }

        temp += size1;
        size -= size1;

    }

    if (size >= fs->ss_size) {
        while(size >= fs->ss_size) {
            if (sffs_read_4k(fs,fd,temp,fs->ss_size) < fs->ss_size) {
                sffs_set_errno(fs,SFFS_READ_DATA_ERR);
                return file->ofs-ofsb;
            }

            size -= fs->ss_size;
            temp += fs->ss_size;
        }
    }

    if(size > 0) {
        if (sffs_read_4k(fs,fd,temp,size) < size) {
            sffs_set_errno(fs,SFFS_READ_DATA_ERR);
            return file->ofs-ofsb;
        }
        size = 0;
    }
    return file->ofs-ofsb;
}

INT32 sffs_rename(struct sffs *fs,const INT8 *old_name,const INT8 *new_name)
{
    INT32 pos1,pos2,nl,new_hs,old_hs;
    UINT32 ofs_new,ofs_old;
    struct sffs_entry *file;
    UINT8 data = FS_DELETED;

    if(old_name == NULL || new_name == NULL) {
        sffs_set_errno(fs,SFFS_PARA_ERR);
        return FS_FAIL;
    } else if (strlen((char *)new_name) == strlen((char *)old_name) &&
               strncmp((char*)old_name,(char *)new_name,strlen((char *)old_name))==0) {
        return FS_OK;
    }

    /*old_name should exist and new_name should should not exist*/
    pos1 = sffs_find_filehdr(fs,old_name);
    if (pos1 <0) {
        sffs_set_errno(fs,SFFS_FILENAME_NOT_EXIST_ERR);
        return FS_FAIL;
    }

    pos2 = sffs_find_filehdr(fs,new_name);
    if (pos2 >= 0) {
        sffs_set_errno(fs,SFFS_FNAME_DUPLICATE_ERR);
        return FS_FAIL;
    }

    /*file has been written to flash then should rewrite flash,or just rename the file structure*/
    file = &(fs->file[pos1]);

    if((nl = strlen((char *)new_name))>FNAME_LEN) {
        nl = FNAME_LEN;
    }
    if (file ->comt != FS_COMMITED) {
        strncpy((char *)file->name,(char *)new_name,nl);
        file->name_len = nl;
    } else {
        ofs_old=SFFS_SB_START*fs->se_size + (INT32)(file->hdr_ss)*fs->ss_size ;/*+ (UINT32 )(&(file->deled)) - (UINT32 )((file));*/
        old_hs = file->hdr_ss;

        if ( (new_hs = sffs_get_hdrss(fs,pos1)) < FS_OK) {
            LOG_ERROR("SFFS: 2 get flash hdr fail %d\n",new_hs);
            return FS_FAIL;
        }

        memset((char *)file->name,0,sizeof(file->name));
        strncpy((char *)file->name,(char *)new_name,nl);
        file->name[nl]  = '\0';
        file->name_len  = (INT16)nl;
        file->hdr_ss    = (INT16)new_hs;
        file->seq       = fs->seqcnt;

        ofs_new         = file->hdr_ss*fs->ss_size + SFFS_SB_START*fs->se_size;

        /*write new hdr*/
        if( sffs_write_at(fs, ofs_new, file, (UINT32 )(&(file->deled))-(UINT32)((file))) !=
            (INT32)((UINT32 )(&(file->deled))-(UINT32)((file))) ) {

            sffs_set_errno(fs,SFFS_RENAME_FAIL1);
            fs->free_ss_hdr[new_hs]=FS_HDRS_BAD;
            if(fs->pageErase != SFFS_PAGE_ERASE)
#ifdef SFFS_MULTI_DEVICE
                hdrsStat[fs->fsid][new_hs]=FS_HDRS_BAD;
#else
                hdrsStat[new_hs]=FS_HDRS_BAD;
#endif
            file->seq=0;
            return FS_FAIL;
        }
        fs->free_ss_hdr[new_hs]=FS_HDRS_USED;
        if(fs->pageErase != SFFS_PAGE_ERASE)
#ifdef SFFS_MULTI_DEVICE
            hdrsStat[fs->fsid][new_hs]=FS_HDRS_USED;
#else
            hdrsStat[new_hs]=FS_HDRS_USED;
#endif

        /*delete old hdr*/
        ofs_old += (UINT32 )(&(file->deled)) - (UINT32 )((file));
        if ( sffs_write_at(fs,ofs_old,&data, 1) != 1 ) {
            sffs_set_errno(fs,SFFS_RENAME_FAIL2);
            fs->free_ss_hdr[old_hs]=FS_HDRS_BAD;
            if(fs->pageErase != SFFS_PAGE_ERASE)
#ifdef SFFS_MULTI_DEVICE
                hdrsStat[fs->fsid][old_hs]=FS_HDRS_BAD;
#else
                hdrsStat[old_hs]=FS_HDRS_BAD;
#endif
            file->seq=0;
            return FS_FAIL;
        }
        fs->free_ss_hdr[old_hs]=FS_HDRS_TMP;
        if(fs->pageErase != SFFS_PAGE_ERASE)
#ifdef SFFS_MULTI_DEVICE
            hdrsStat[fs->fsid][old_hs]=FS_HDRS_TMP;
#else
            hdrsStat[old_hs]=FS_HDRS_TMP;
#endif
        fs->seqcnt++;
    }
    return FS_OK;

}

INT32 sffs_lseek(struct sffs *fs, INT32 fd, INT32 pos)
{
    if (sffs_ck_fd(fs,fd) < 0 || pos < 0) {
        sffs_set_errno(fs,SFFS_PARA_ERR);
        return FS_FAIL;
    }
    /*cannot seek in writing mode*/
    if (sffs_ckstat(fs,fd) == FS_WRONLY) {
        sffs_set_errno(fs,SFFS_WRITEMODE_ERR);
        return FS_FAIL;
    }

    if (pos > fs->file[fd].size )
        fs->file[fd].ofs = fs->file[fd].size -1;
    else {
        fs->file[fd].ofs = pos;
    }

    return fs->file[fd].ofs;
}

/* delete file */
/*******************************************************************************
*
* sffs_remove_byfd - delete file by fd
*
* RETURNS: FS_FAIL, if delete failed
*          FS_OK, if delete successfully
*
*/
INT32 sffs_remove_byfd(struct sffs *fs, INT32 fd)
{
    UINT32 ofs;
    INT32 size;
    UINT8 data;
    INT32 ret;

    if(fs->file[fd].comt != FS_COMMITED) {
        LOG_INFO("remove a opened file\n");
        sffs_set_errno(fs,SFFS_FILE_NOT_COMMIT_ERR);

        fs->free_ss_hdr[fs->file[fd].hdr_ss]=FS_S_UNUSED;
        if (fs->pageErase != SFFS_PAGE_ERASE)
#ifdef SFFS_MULTI_DEVICE
            hdrsStat[fs->fsid][fs->file[fd].hdr_ss] = FS_HDRS_EMPTY;
#else
            hdrsStat[fs->file[fd].hdr_ss] = FS_HDRS_EMPTY;
#endif
        sffs_reset_filehdr(fs,fd);
        return FS_OK;
    }

    data = FS_DELETED;

    size    = 1;
    ofs     = (UINT32)(SFFS_SB_START*fs->se_size + fs->file[fd].hdr_ss * fs->ss_size) ;
    ofs    += (UINT32 )(&(fs->file[fd].deled)) - (UINT32 )(&(fs->file[fd]));

    LOG_INFO("rmove hdr size %d ,ofs %d hdr_ss %d\n",size,ofs,fs->file[fd].hdr_ss);

    fs->free_ss_hdr[fs->file[fd].hdr_ss]=FS_S_UNUSED;
    /*mark file as deleted*/
    ret = sffs_write_at(fs, ofs, &data,size);
    if (ret != size) {
        if (fs->pageErase == SFFS_PAGE_ERASE) {
            /*write error, so erase this sub-sector*/
            if (sffs_erase_at(fs,fs->file[fd].hdr_ss,ERASE_NUM_PER)<0) {
                LOG_WARN("delete fail %d\n",ret);
            } else {
                sffs_reset_filehdr(fs, fd);
                fs->fileNum -- ;
                fs->free_ss_hdr[fs->file[fd].hdr_ss]=FS_DELETED;/*FS_S_UNUSED;*/
                return FS_OK;
            }

        } else {
#ifdef SFFS_MULTI_DEVICE
            hdrsStat[fs->fsid][fs->file[fd].hdr_ss]=FS_HDRS_TMP;
#else
            hdrsStat[fs->file[fd].hdr_ss]=FS_HDRS_TMP;
#endif
        }
        fs->free_ss_hdr[fs->file[fd].hdr_ss]=FS_S_UNUSED;
        sffs_reset_filehdr(fs, fd); 
        sffs_set_errno(fs,SFFS_DEL_ERR);
        fs->fileNum -- ;
        return FS_FAIL;
    }
    if (fs->pageErase == SFFS_PAGE_ERASE)
#ifdef SFFS_MULTI_DEVICE
        hdrsStat[fs->fsid][fs->file[fd].hdr_ss]=FS_HDRS_TMP;
#else
        hdrsStat[fs->file[fd].hdr_ss]=FS_HDRS_TMP;
#endif
    fs->free_ss_hdr[fs->file[fd].hdr_ss]=FS_HDRS_TMP;
    sffs_reset_filehdr(fs, fd);
    fs->fileNum -- ;
    return FS_OK;
}

/*******************************************************************************
*
* sffs_remove_byname - delete file by file name
*
* Actually this routine only transfer file name to fd and call sffs_remove_byfd to delete a file
*
* RETURNS: FS_FAIL, if delete failed
*          FS_OK, if delete successfully
*
*/
INT32 sffs_remove_byname(struct sffs *fs, const INT8 *fname)
{
    INT32 pos ;

    pos = sffs_find_filehdr(fs,fname);
    if (pos < 0) {
        LOG_INFO("no file %s\n",fname);
        sffs_set_errno(fs,SFFS_FILENAME_NOT_EXIST_ERR);
        return FS_FAIL;
    } else {
        return sffs_remove_byfd(fs, pos);
    }
}

/* format flash with specific layout ? */
INT32 sffs_format(struct sffs *fs, INT32 type)
{
    INT32 ret;
    switch (type) {
    case FS_FORMAT_DISK:
        break;
    case FS_FORMAT_HDRS:
        /*only erase hdrs*/
        ret = sffs_erase_at(fs,SFFS_SB_START,ERASE_NUM_PER);
        if(ret <0) {
            ret = sffs_erase_at(fs,SFFS_SB_START,ERASE_NUM_PER);
            if(ret <0) {
                sffs_set_errno(fs, SFFS_ERASE_HDRSS_ERR);
                LOG_INFO("erase hdr faile\n");
                sffs_start(fs);
                return FS_FAIL;
            }
        }
        sffs_start(fs);
        break;
    case FS_FORMAT_ALL:
        /*erase whole flash*/
        for(INT32 i=0; i<fs->device_size/fs->erase_unit; i++) {
            ret = sffs_erase_at(fs,i,ERASE_NUM_PER);
            if(ret <0) {
                ret = sffs_erase_at(fs,i,ERASE_NUM_PER);
                if(ret <0) {
                    sffs_set_errno(fs, SFFS_ERASE_DATA_ERR);
                    LOG_INFO("erase whole flash faile %d\n",i);
                    sffs_start(fs);
                    return FS_FAIL;
                }
            }
        }
        sffs_start(fs); /*reset after hdr flash sector erased*/
        break;
    default:
        LOG_INFO("parameter err\n");
        break;
    }
    return FS_OK;
}


/* umount sffs */
INT32 sffs_umount(struct sffs *fs)
{
    fs->mounted = FS_UMOUNTED;
    sffs_start(fs);
    return FS_OK;
}

static inline INT32 sffs_check_sffs(struct sffs fs)
{
    INT32 ret = 0;
    if (fs.device_size == 0 || fs.ss_size ==0 
            || fs.se_size == 0 ){
        ret = -1;
    }
    return ret;
}


INT32 sffs_register(struct sffs fs)
{
    if (sffs_check_sffs(fs))
        return -2;

    for (int i=0; i<MAX_SLOT-1; i++) {
        if(sffs_slot[i].flashId == -1) {
            memcpy(&sffs_slot[i],&fs,sizeof(struct sffs));
            sffs_slot[i].fsid=i;
            return i;
        }
    }
    return -1;
}

struct sffs *sffs_get_slot(int fsid)
{
    return (fsid>=0 && fsid <MAX_SLOT-1)?&sffs_slot[fsid]:0;
}
/*******************************************************************************
*
* sffs_mount - mount sffs
*
* RETURNS: FS_FAIL, if mount failed
*           <fs_id>, if mount successfully
*
*/
INT32 sffs_mount(struct sffs *fs, INT32 type)
{
    UINT32 offset;
    INT16 i,k;
/*UINT8 header[SFFS_HEADER_SIZE+1];*/
    UINT8 header[sizeof(struct sffs_entry)+1];
    INT32 ret;/*fd,fid;*/

    i = k = 0;
    offset = SFFS_SB_START*fs->se_size; /*SFFS_SB_START is an relative offset*/

    if( fs->mounted == FS_MOUNTED)
        return FS_OK;

    if (type == 1) {
        /*erase all*/
        fs->mounted = FS_MOUNTED;
        return FS_OK;
    }

    if (sffs_start(fs) <0) {
        return FS_FAIL;
    }
    FS_TRYLOCK(fs);
    LOG_DEBUG("device size 0x%x magic 0x%x sb %d\n",fs->device_size,fs->magic_num,SFFS_SB_START);

    while(offset < fs->se_size+SFFS_SB_START*fs->se_size &&
          sffs_read_at(fs, offset,header, sizeof(struct sffs_entry)) == sizeof(struct sffs_entry)) {

        /*deleted or not commited or broken hdr or empty files ommit*/
        if (header[FF_HDR_DELED] != FS_EMPTY ||
            header[FF_HDR_COMMIT] != FS_COMMITED ||
            *(UINT32 *)(header) != SFFS_MAGIC) {

            /*deleted or file hdr broken*/
            if(fs->free_ss_hdr_cur < 0) {    /*set the first free ss as free_ss_hdr_cur*/
                fs->free_ss_hdr_cur = i;
            }
            if(fs->pageErase != SFFS_PAGE_ERASE)
#ifdef SFFS_MULTI_DEVICE
                hdrsStat[fs->fsid][i]= FS_HDRS_TMP;/*assume this is not a dirty sector and erased before using*/
#else
                hdrsStat[i] = FS_HDRS_TMP;/*assume this is not a dirty sector and erased before using*/
#endif
            i++;
            offset=SFFS_SB_START*fs->se_size + i*fs->ss_size;
            continue;
        }

        /*got one file*/
        memcpy(&(fs->file[fs->fileNum]), header, sizeof(struct sffs_entry));

        /*check: if two file has the same fid,so delete the newer(bigger) one*/
        ret = sffs_check_fid(fs, fs->file[fs->fileNum].fid);
        /*has same fid*/
        if (ret != FS_FAIL) {
            INT32 fhdrs_t;
            UINT8 data = FS_DELETED;
            UINT32 ofs0 ;
            /*delete the newer one's hdr_ss, and continue*/
            if(fs->file[fs->fileNum].seq < fs->file[ret].seq) {
                memcpy(&(fs->file[ret]), &(fs->file[fs->fileNum]), sizeof(struct sffs_entry)); /*the latter one should replace the former one*/
                fhdrs_t = ret;
            } else
                fhdrs_t = fs->fileNum;

            ofs0 = (UINT32 )(&(fs->file[fhdrs_t].deled)) - (UINT32 )(&(fs->file[fhdrs_t])) +
                   (UINT32)(fs->file[fhdrs_t].hdr_ss*fs->ss_size);

            /*mark as deleted*/
            if (sffs_write_at(fs,ofs0,&data,1) != 1) {
                if (fs->pageErase == SFFS_PAGE_ERASE) {
                    if (sffs_write_at(fs,ofs0,&data,1) != 1) {
                        sffs_set_errno(fs,SFFS_WRITE_HDRSS_ERR);
                        FS_UNLOCK(fs);
                        return FS_FAIL;
                    }
                } else {
                    sffs_set_errno(fs,SFFS_WRITE_HDRSS_ERR);
                    FS_UNLOCK(fs);
                    return FS_FAIL;
                }
            }
            fs->file[fhdrs_t].stat = FS_CLOSED;
            i++;
            offset = SFFS_SB_START*fs->se_size + i*fs->ss_size;
            if (fs->pageErase != SFFS_PAGE_ERASE)
#ifdef SFFS_MULTI_DEVICE
                hdrsStat[fs->fsid][fs->file[fs->fileNum].hdr_ss]=FS_HDRS_USED;/*a used sector*/
#else
                hdrsStat[fs->file[fs->fileNum].hdr_ss]=FS_HDRS_USED;/*a used sector*/
#endif
            continue;
        }

        fs->file[fs->fileNum].stat = FS_CLOSED;

        if(fs->file[fs->fileNum].fid >fs->fidcnt)
            fs->fidcnt = fs->file[fs->fileNum].fid;

        if(fs->file[fs->fileNum].seq > fs->seqcnt)
            fs->seqcnt = fs->file[fs->fileNum].seq;

        fs->free_ss_hdr[fs->file[fs->fileNum].hdr_ss]=FS_HDRS_USED;

        if (fs->pageErase != SFFS_PAGE_ERASE) {
#ifdef SFFS_MULTI_DEVICE
            hdrsStat[fs->fsid][fs->file[fs->fileNum].hdr_ss]=FS_HDRS_USED;/*a used sector*/
#else
            hdrsStat[fs->file[fs->fileNum].hdr_ss]=FS_HDRS_USED;/*a used sector*/
#endif
        }

        /*modify free se no in fs*/
        for(k=0; k<SFFS_BNO_LIM && fs->file[fs->fileNum].se_no[k]>0; k++) {
            sffs_set_sebit(fs,fs->file[fs->fileNum].se_no[k]);
        }
        LOG_DEBUG("got file ss %d cmt 0x%x del 0x%x magic 0x%x\n",i,header[FF_HDR_COMMIT],header[FF_HDR_DELED]);
        LOG_DEBUG("mn 0x%x name %s fid %d nl %d size %d ofs 0x%x hdr %d,cmt 0x%x, del %d\n",
                  fs->file[fs->fileNum].magic_num, fs->file[fs->fileNum].name, fs->file[fs->fileNum].fid,
                  fs->file[fs->fileNum].name_len, fs->file[fs->fileNum].size, fs->file[fs->fileNum].ofs,
                  fs->file[fs->fileNum].hdr_ss, fs->file[fs->fileNum].comt, fs->file[fs->fileNum].deled);

        i++;
        offset=SFFS_SB_START*fs->se_size + i*fs->ss_size;
        fs->fileNum ++;
    } /*end for while*/
    fs->fidcnt++;

    if (fs->fileNum<1) {
        LOG_INFO("empty \n");
        if(fs->pageErase != SFFS_PAGE_ERASE)
#ifdef SFFS_MULTI_DEVICE
            fsStat[fs->fsid] = 1;/*empty*/
#else
            fsStat = 1;/*empty*/
#endif
    }

    if (type == 2) {
        /*erase empty se*/
    }
    fs->mounted = FS_MOUNTED;
    LOG_DEBUG("SFFS: mounted!\n");
    FS_UNLOCK(fs);
    return FS_OK;
}

