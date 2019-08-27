48 of 475 branches covered (10%)

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
               #include "e:\\fs\\sffs.h"
               #include "E:\\Visual Studio\\VC\\INCLUDE\\string.h"
               //#include "stdarg.h"
               //#include <libc/string1.h>
               int intLock(void)
               {
1 0     (T)    intLock
                return 0;
               }
               void intUnlock(int a)
               {
2 0     (T)    intUnlock
                return;
               }
               //two types of operation failure:
               //1. just return FS_FAIL and that's enough
               //2. return FS_FAIL and tell the failure
               //related sffs root node
               struct sffs os_sffs;
               //reserved
                INT32 current_fs=0;
               //static INT32 sffs_timeout = 0;
               //dfs_ops id
               INT32 flashDevid = -1;
               INT32 hdrsStat[(32)] = {0};
               INT32 fsStat = 0;
               INT32 sffs_erase_time[(240)]={0};//record relative sector erase time
               void NOLOG(INT8 *c,...)
               {
3 0     (T)    NOLOG
               }
               //spin lock, timeout
               // 0 ok,  <0 error
               INT32 sffs_lock(struct sffs *fs) 
               {
4 0     (T)    sffs_lock
                   INT32 ret = 0;
4 1     (T)(F)   if(fs->lock >0) {
                       ret = -1;
                       //////printf("locked %d\n",fs->lock);
                       //while(1);
               		return ret;
                   }
                   else {
                       fs->lock=1;
                       ret = 0;
                   }
                   return ret;
               }
               // 0 ok,<0 error
               INT32 sffs_unlock(struct sffs *fs) 
               {
5 0     (T)    sffs_unlock
                   INT32 ret = 0;
5 1     (T)(F)   if(fs->lock >0){
                       fs->lock=0;
                       ret = 0;
                   }
                   else {
                       ret = -1;
                       ////printf("unlocked %d\n",fs->lock);
                       //while(1);
               		return ret;
                   }
                   return ret;
               }
               //un-interruptable
               void sffs_protect(void)
               {
6 0     (T)    sffs_protect
               }
               void sffs_unprotect(void)
               {
7 0     (T)    sffs_unprotect
               }
               static INT32 sffs_get_errno(struct sffs *fs)
               {
8 0     (T)    sffs_get_errno
                   return fs->errno;
               }
               static void sffs_clear_errno(struct sffs *fs)
               {
9 0     (T)    sffs_clear_errno
                   fs->errno = 0;
               }
                void sffs_set_errno(struct sffs *fs,INT32 errno)
               {
10 0    (T)    sffs_set_errno
                   fs->errno=errno;
               }
                INT32 sffs_read_at(struct sffs *fs, UINT32 offset, void *data,UINT32 size)
               {
11 0    (T)    sffs_read_at
                   INT32 ret=-1 ;
11 1    (T)(F)   if (fs->read != ((void *)0))
               	{
               		ret = 3;// fs->read(flashDevid, offset, (UINT8 *)data, size);
                   }
                   return ret;
               }
                INT32 sffs_write_at(struct sffs *fs, UINT32 offset, const void *data, UINT32 size) 
               {
                   INT32 ret ;
12 0    (T)    sffs_write_at
                ret = 0; // fs->write(flashDevid, offset, (UINT8 *)data, size);
                   return ret;
               }
               //in S29GL512P ,this is a sector erase , and in N25Q64A will add or replaced by a sub_sector erase
                INT32 sffs_erase_at(struct sffs *fs, UINT32 no_from, UINT32 num) 
               {
                   INT32 ret ;
                   int i;
13 0    (T)    sffs_erase_at
                ret = 2; // fs->erase(flashDevid, no_from, num);
13 1    (T)(F)   for(i=0;i<num;i++) //record sector erase times
                       sffs_erase_time[no_from+i]++;
                   return ret;
               }
               //get free se and mark it in fs
               /*******************************************************************************
               *
               * sffs_get_se -  get a un-used sector 
               *
               *
               * RETURNS: FS_FAIL, if there's no unused sector
               *           <secotr no>, unused sector's no
               *
               */
                INT32 sffs_get_se(struct sffs *fs)
               {
                   INT16 i,j,k;
14 0    (T)    sffs_get_se
                   i = j = k = 0;
                   i=fs->free_se_cur;
14 1    (T)(F)   for( j=0; j<fs->device_size/fs->se_size; j++) 
               	{
                       k = get_bit_val(fs->free_se,i);
14 2    (T)(F)        if (k == 0) {
                           sffs_protect();
                           set_bit_val(fs->free_se,i);//mark
                           fs->free_se_cur = i;
                           sffs_unprotect();
                           return i;
                       }
                       i = (i+1)%(fs->device_size/fs->se_size);
                       i = (
14 3    (T)(F)        i==0)?1:i; //round
                   }
                   sffs_set_errno(fs,-3);
                   return -1;
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
                void sffs_set_sebit(struct sffs *fs, INT16 se_no)
               {
15 0    (T)    sffs_set_sebit
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
                void sffs_clear_sebit(struct sffs *fs,INT16 se_no)
               {
16 0    (T)    sffs_clear_sebit
                   clear_bit_val(fs->free_se,se_no);
               }
                UINT8 sffs_ckstat(struct sffs *fs,INT32 fd)
               {
17 0    (T)    sffs_ckstat
                   return fs->file[fd].stat;
               }
                INT32 sffs_ck_fd(struct sffs *fs, INT32 fd)
               {
                   //ok: 1. fd value;2. fd is opened
18 0    (T)    sffs_ck_fd
18 1    (T)(F)   if (0<= fd && fd<20 && fs->file[fd].size >=0 ) {
                       return 0;
                   }
                   else {
                       return -1;
                   }
               }
               INT32 sffs_check_fid(struct sffs *fs,INT32 fid) //just run mount
               {
19 0    (T)    sffs_check_fid
19 1    (T)(F)   for(INT32 i=0;i < fs->fileNum;i++)
                   {
19 2    (T)(F)        if (fs->file[i].fid == fid)
                           return i;
                   }
                   return -1;
               }
               /*******************************************************************************
               *
               * sffs_get_hdrss - get a empty file hdr in flash 
               *
               *
               * RETURNS: return the sub-sector No. of the empty file hdr
               *
               */
                INT32 sffs_get_hdrss(struct sffs *fs, INT32 fd)
               {
                   INT32 i,j,k;
20 0    ( )    sffs_get_hdrss
                   k=0;
20 1    ( )( )   if(fs->fileNum >= 20) {//hdr full
                       sffs_set_errno(fs,-5);
                       return -1;
                   }
                   //try to get an empyt hdr_ss
20 2    ( )( )   for(i=fs->free_ss_hdr_cur, j=0; j<(fs->se_size/fs->ss_size); j++) {
20 3    ( )( )        if(i == -1)
                           i=0;
20 4    ( )( )        if (fs->pageErase == 1) {
20 5    ( )( )            if (fs->free_ss_hdr[i] != 1 && fs->free_ss_hdr[i] != 3) {
                               fs->free_ss_hdr[i] = 2;
                               fs->free_ss_hdr_cur = i;
                               return i;
                           }
                       }
                       else {
                           //hdr ss is free and erased
20 6    ( )( )            if (hdrsStat[i] == 0 && fs->free_ss_hdr[i] == 0) {
                               hdrsStat[i] = 2;//occupy
                               fs->free_ss_hdr[i] = 2;
                               fs->free_ss_hdr_cur = i;
                               return i;
                           }
                       }
                       i=(i+1)%(fs->se_size/fs->ss_size);
                   }
                   //in S29GL512P or other flash that not support page erase ,will exist hdr_ss not used but not erased
20 7    ( )( )   if (fs->pageErase != 1) {
                       //printf("np\n");
                       //erase hdr sector
               TRY_TWICE:
                       //printf("sb %d\n",SFFS_SB_START);
20 8    ( )( )        if (sffs_erase_at(fs, 0, 1)<0) {
20 9    ( )( )            if (sffs_erase_at(fs, 0, 1)<0) {
                               sffs_set_errno(fs,-4);
                               return -1;
                           }
                       }
                       //printf("11\n");
                       //write old hdrs to flash
                       UINT32 ofs;
                       memset(hdrsStat,0,sizeof(hdrsStat));
                       memset(fs->free_ss_hdr,0,sizeof(fs->free_ss_hdr));
                       fs->free_ss_hdr_cur=0;
                       j=0;
                       //write hdrs in memory to flash
                       //check effective file list and write hdr to flash
20 10   ( )( )        for (i=0;i<20;i++) {
20 11   ( )( )            if (fs->file[i].deled == 0xff) /*file is not deleted */ {
20 12   ( )( )                if (fs->file[i].comt == 0xc0) /* file is committed */ {
                                   ofs = 0*fs->se_size + fs->file[i].hdr_ss * fs->ss_size;
                                   fs->free_ss_hdr[fs->file[i].hdr_ss] = 1;
                                   hdrsStat[fs->file[i].hdr_ss] = 1;
20 13   ( )( )                    if ( sffs_write_at(fs,ofs, &(fs->file[i]),
                                              (UINT32 )(&(fs->file[i].deled))-(UINT32)(&(fs->file[i])))
                                       < (UINT32 )(&(fs->file[i].deled))-(UINT32)(&(fs->file[i]))) {
20 14   ( )( )                        if (k<1)/* try to do twice*/ {
                                           NOLOG("erase hdr and re-write\n");
                                           k++;
                                           goto TRY_TWICE;
                                       }
                                       fs->free_ss_hdr[fs->file[i].hdr_ss] = 3;
                                       hdrsStat[fs->file[i].hdr_ss] = 3;
                                       NOLOG("write hdrss fail\n");
                                       //keep it as it is and change when actual hardware comes out
                                       fs->file[i].comt = 0xee;
                                       sffs_set_errno(fs,-10);
                                       fs->fileNum -- ;
                                       return -1; //it's better not to return  here
                                   }
                                   j++;
                               }
                               else 
20 15   ( )( )                    if (fs->file[i].stat !=0  && fs->file[i].hdr_ss != -1 ) {
                                   hdrsStat[fs->file[i].hdr_ss]=2;
                                   fs->free_ss_hdr[fs->file[i].hdr_ss]=2;
                                   j++;
                               }//end of FS_COMMITED
                           }//end of FS_EMPTY
                       }//end of for loop
20 16   ( )( )        if(j<20) {
20 17   ( )( )            for(i=0;i<(fs->se_size/fs->ss_size);i++) {
20 18   ( )( )                if (fs->free_ss_hdr[i] == 0 && hdrsStat[i]==0) {
                                   fs->free_ss_hdr_cur = i;
                                   hdrsStat[i]=2;
                                   fs->free_ss_hdr[i]=2;
                                   return fs->free_ss_hdr_cur;
                               }
                           }
                       }
                       else {
                           fs->free_ss_hdr_cur = -1;
                       }
                   }
                   sffs_set_errno(fs,-5);
                   return -1;
               }
               //temporary not used
               static INT32 sffs_set_free_hdrss(struct sffs *fs)
               {
                   INT32 i,j;
21 0    ( )    sffs_set_free_hdrss
21 1    ( )( )   if (fs->fileNum >= 20) {
                       //file full
                       sffs_set_errno(fs,-6);
                       return -1;
                   }
21 2    ( )( )   for(i=fs->free_ss_hdr_cur ,j=0; j<(fs->se_size/fs->ss_size); j++) {
21 3    ( )( )        if (fs->pageErase == 1) {
21 4    ( )( )            if ((INT8)fs->free_ss_hdr[i] == -1) {
                               fs->free_ss_hdr_cur = i;
                               return i;
                           }
                       }
                       else {
                           //hdr ss is free and erased
21 5    ( )( )            if (hdrsStat[i] == 0 && fs->free_ss_hdr[i] == -1) {
                               fs->free_ss_hdr_cur = i;
                               return i;
                           }
                       }
                       i=(i+1)%(fs->se_size/fs->ss_size);
                   }
                   //go here has two reason:1. no erased hdr ss;2. no free hdr ss. 
                   //sffs_get_hdrss will deal with these.
                   return -1;
               }
               //clear file hdr and relate var in struct sffs
                void sffs_reset_filehdr(struct sffs *fs, INT32 fd)
               {
                   INT32 i ,j;
22 0    ( )    sffs_reset_filehdr
                   i = fd;
                   fs->file[i].magic_num   = 0;
                   memset(fs->file[i].name,0x0,18);
                   fs->file[i].name_len    = 0;
                   fs->file[i].size        = -1;
                   fs->file[i].ofs         = -1;
                   //return SEs
22 1    ( )( )   for(j=0;j<16 && fs->file[i].se_no[j]!=-1;j++) {
                       sffs_clear_sebit(fs,fs->file[i].se_no[j]);
                       fs->file[i].se_no[j]=-1 ;
                   }
                   fs->file[i].hdr_ss  = -1;
                   fs->file[i].comt    = 0xff;
                   fs->file[i].unused  = 0xff;
                   fs->file[i].deled   = 0xff;
                   fs->file[i].fid     = 0;
                   fs->file[i].seq     = 0;
                   fs->file[i].stat    = 0;
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
23 0    ( )    sffs_find_filehdr
23 1    ( )( )   for(i=0;i<20;i++) {
                       //file not deleted and has same name
23 2    ( )( )        if (fs->file[i].deled != 0xde && 
                               fs->file[i].name_len> 0 && 
                               strlen((char *)fname) == fs->file[i].name_len && 
                               strncmp((char *)fname, (char *)fs->file[i].name, fs->file[i].name_len) == 0 ) {
                           return i;
                       }
                   }
                   sffs_set_errno(fs,-8);
                   return -1;
               }
               /******************************************************************************
               *
               * sffs_find_filehdr - get a unused file hdr
               *
               *
               * RETURNS: file hdr's number or FS_FAIL if no usable file hdr
               */
                INT32 sffs_get_filehdr(struct sffs *fs)
               {
                   INT32 i;
24 0    (T)    sffs_get_filehdr
24 1    (T)(F)   if (fs->fileNum >= 20) {
                       //file full
                       sffs_set_errno(fs,-6);
                       return -1;
                   }
24 2    (T)(F)   for(i=0;i<20;i++) {
24 3    (T)(F)        if (fs->file[i].hdr_ss < 0) {
                           return i;
                       }
                   }
                   sffs_set_errno(fs,-24);
                   return -1;
               }
               INT32 sffs_is_mounted(struct sffs *fs)
               {
25 0    (T)    sffs_is_mounted
                   return fs->mounted;
               }
               /* write data to flash. */
               /*only support sequence write and must start with SS' border*/
               /******************************************************************************
               *
               * sffs_write_4k - write data to flash
               * This routine will write data to flash. It can write at most 4k bytes one time. It should start from sub-sector's border and cannot across sub-sectors.
               *
               * RETURNS: actual wrote data length.or return FS_FAIL if the operation is failed.
               */
               INT32 sffs_write_4k(struct sffs *fs, INT32 fd, const void *data, INT32 size) 
               {
                   struct sffs_entry *file;
                   INT32 offset,se_in_file,ss_in_se, ss_no;
                   INT16 se_no ;
                   INT32 ret ;
26 0    ( )    sffs_write_4k
                   se_no = ss_no = -1;
                   //check fd/size is legal
26 1    ( )( )   if (sffs_ck_fd(fs, fd) < 0 || size > fs->ss_size || size < 0) {
               		sffs_set_errno(fs, -11);
               		return -1;
               	}
                   //file can be write only one time
26 2    ( )( )   if(fs->file[fd].comt == 0xc0) {
                       NOLOG("filen cannot be modify");
                       sffs_set_errno(fs,-12);
                       return -1;
                   }
26 3    ( )( )   if(size == 0)
                       return 0;
                   //get file hdr
                   file = &(fs->file[fd]);
26 4    ( )( )   if(file->size % fs->se_size == 0) {
                       se_no = sffs_get_se(fs);
26 5    ( )( )        if(se_no < 1) {
                           NOLOG("SFFS: no space left on device %d\n",se_no );
                           sffs_set_errno(fs,-3);
                           return -1;
                       }
                       file->se_no[file->size/fs->se_size] = se_no;
26 6    ( )( )        if (fs->pageErase != 1) {
                           //erase one whole sector when firstly programs in a new sector
                           ret = sffs_erase_at(fs,se_no,1);
26 7    ( )( )            if (ret <0) {
                               ret = sffs_erase_at(fs,se_no,1);
26 8    ( )( )                if (ret <0) {
                                   NOLOG("erase new sector fail ret %d no %d\n",ret,se_no);
                                   sffs_set_errno(fs,-13);
                                   return -1;
                               }
                           }
                       }
                       ss_no = se_no*(fs->se_size/fs->ss_size);
                   } 
                   else {
26 9    ( )( )        if(file->size%fs->ss_size != 0) {
                           NOLOG("write not aligined ss_size %d %d\n",file->size,size);
                           sffs_set_errno(fs,-14);
                           return -1;
                       }
                       se_in_file  = file->size / fs->se_size;
                       ss_in_se    = file->size % fs->se_size / fs->ss_size;
                       ss_no       = file->se_no[se_in_file] * (fs->se_size/fs->ss_size)+ ss_in_se;
                   }
                   offset = ss_no * fs->ss_size;
                   /* writing data */
26 10   ( )( )   if(fs->pageErase == 1)
                       sffs_erase_at(fs,ss_no,1);
                   ret = sffs_write_at(fs, offset, data, size);
26 11   ( )( )   if(ret == size){
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
               INT32 sffs_read_4k(struct sffs *fs, INT32 fd, void *data, size_t size) 
               {
                   struct sffs_entry *file;
                   INT32 offset, ss_no_in,se_no_in,ret;
27 0    ( )    sffs_read_4k
27 1    ( )( )   if (size == 0)
                       return 0;
                   //get file hdr
                   file = &(fs->file[fd]);
27 2    ( )( )   if(file->size ==0 || file->ofs == file->size-1)
                       return 0;
27 3    ( )( )   if(file->ofs + size > file->size)
                       size = file->size - file->ofs;
                   //ofs should link to the ss_no
                   offset=file->ofs; 
                   se_no_in = offset/fs->se_size;
                   ss_no_in = offset%(fs->se_size/fs->ss_size);
                   offset = offset%fs->se_size%fs->ss_size + file->se_no[se_no_in]*fs->se_size + ss_no_in*fs->ss_size;
27 4    ( )( )   if (offset % fs->ss_size + size > fs->ss_size) 
                       size = fs->ss_size - offset % fs->ss_size;
                   ret  = sffs_read_at(fs,offset,data, size);
27 5    ( )( )   if (ret < size) {
                       sffs_set_errno(fs,-11);
                       return ret;
                   }
                   file->ofs += size;
                   return ret;
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
               INT32 sffs_start(struct sffs *fs)
               {
                   INT32 i,j;
28 0    ( )    sffs_start
28 1    ( )( )   if((fs->se_size/fs->ss_size) != (32) ||
                           fs->ss_size != (4*1024) || 
                           (fs->device_size/fs->se_size) != (240)|| 
                           (32) < 20||
                           16*fs->se_size < (2*1024*1024)
                           ) {
                       sffs_set_errno(fs,-22);
                       NOLOG("sffs macro defined error\n");
                       return -1;
                   }
                   //file
28 2    ( )( )   for(i=0;i<20;i++) {
                       memset(fs->file[i].name,0x0,18);
                       fs->file[i].hdr_ss = -1;
                       fs->file[i].name_len = 0;
                       fs->file[i].size = -1;
                       fs->file[i].ofs = -1;
28 3    ( )( )        for(j=0;j<16;j++) {
                               fs->file[i].se_no[j]=-1;
                       }
                       fs->file[i].magic_num   = 0;
                       fs->file[i].comt        = 0xff;
                       fs->file[i].unused      = 0xff;
                       fs->file[i].deled       = 0xff;
                       fs->file[i].unused2[0]  = 0xff;
                       fs->file[i].unused2[1]  = 0xff;
                   }
                   //fs
                   memset(fs->free_se,0,sizeof(fs->free_se));
                   memset(fs->free_ss_hdr,2,sizeof(fs->free_ss_hdr));
                   memset(hdrsStat,0,sizeof(hdrsStat));
                   //give real value after mount
                   fs->free_ss_hdr_cur = -1;
                   fs->free_se_cur = 1;
                   fs->magic_num = 0xCA905001;
                   fs->fileNum = 0;
                   fs->fidcnt = 0;
                   fs->seqcnt= 0;
                   fs->lock = 0;
                   NOLOG("flash size %d\n",fs->device_size);
                   return 0;
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
29 0    ( )    sffs_open
29 1    ( )( )   if (fname == ((void *)0)){
                       sffs_set_errno(fs,-1);
                       return -1;
                   }
               	pos = 1;// sffs_find_filehdr(fs, fname);
                   //init new file hdr(mem) if not found file
29 2    ( )( )   if (pos < 0) {
                       //MUST get file hdr before get hdr ss
               		pos = 1;// sffs_get_filehdr(fs);
29 3    ( )( )        if (pos == -1 ) {
                           NOLOG("SFFS: get free file hdr fail!\n");
                           return -1;
                       }
29 4    ( )( )        if ( sffs_get_hdrss(fs,pos) == -1) {
                           NOLOG("SFFS: get flash hdr fail\n");
                           return -1;
                       }
                       file = &(fs->file[pos]);
29 5    ( )           if (1) //strlen((char *)fname) > FNAME_LEN) 
               		{
               			memset(file, 0, 2);
                           file->name_len = 18;
                           NOLOG("file name too long %s %d\n",fname,file->name_len);
                       } else {
               			file->name_len = 1;// strlen((char *)fname);
                       }
                       //strncpy((char *)file->name, (char *)fname, file->name_len);
                       file->name[file->name_len]='\0';
                       file->size = 0;
                       file->ofs = 0;
                       file->hdr_ss = fs->free_ss_hdr_cur;
                       fs->fileNum ++;
                       NOLOG("open file %s hdr %d\n",fname,fs->free_ss_hdr_cur);
                       //sffs_set_free_hdrss(fs); //???
                   } else {
                       file = &(fs->file[pos]);
                   }
29 6    ( )( )   if (mode == 1) {
                       //read only
                       file->stat  = 1;
                   } else 
29 7    ( )( )        if (mode == 2) {
                       //write only 
                       file->stat = 2;
                   } else {
                       //default as readonly
                       file->stat = 1;
                   }
                   NOLOG("open %s fd %d\n",fname,pos);
                   return pos;
               }
               /* file ops for user use*/
               //close and save file
               INT32 sffs_close(struct sffs *fs, INT32 fd)
               {
                   UINT32 ofs;
30 0    ( )    sffs_close
30 1    ( )( )   if (sffs_ck_fd(fs,fd) < 0) {
                       sffs_set_errno(fs,-9);
                       //printf("fd er %d\n",fd);
                       return -1;
                   }
30 2    ( )( )   if(fs->file[fd].comt == 0xc0) {//file has been stored in flash
                       fs->file[fd].ofs = 0;
                       fs->file[fd].stat = 0;// set file stat into zen
                       //printf("commited\n");
                       return 0;
                   }
                   fs->file[fd].magic_num = fs->magic_num;
                   fs->file[fd].ofs = 0;
                   fs->file[fd].comt= 0xc0;
                   ofs = 0 * fs->se_size +fs->file[fd].hdr_ss*fs->ss_size;
                   NOLOG("close hno %d ofs %d size %d\n",fs->file[fd].hdr_ss, ofs, sizeof(struct sffs_entry));
                   NOLOG("cf %d len %d stat %d\n",fd,fs->file[fd].size, sffs_ckstat(fs,fd));
                   //file hdr should be written in write mode
30 3    ( )( )   if (sffs_ckstat(fs,fd) == 2) {
30 4    ( )( )        if (fs->pageErase != 1) {
                           //first time to write hdr_ss to flash,so erase hdr se for safty
30 5    ( )( )            if (fsStat == 1) {
                               INT32 ret = 0;
                               ret = sffs_erase_at(fs,0,1);
30 6    ( )( )                if (ret <0) {
                                   ret = sffs_erase_at(fs,0,1);
30 7    ( )( )                    if (ret <0) {
                                       NOLOG("erase new sector in HDRSE fail ret %d \n",ret);
                                       sffs_set_errno(fs,-4);
                                       return -1;
                                   }
                               }
                               fsStat = 2;
                           }
                       }
                       fs->file[fd].fid = fs->fidcnt;
                       fs->file[fd].seq = fs->seqcnt;
                       //not write bit [deled] and [stat]
30 8    ( )( )        if ( sffs_write_at(fs, ofs, &(fs->file[fd]), (UINT32 )(&(fs->file[fd].deled))-(UINT32)(&(fs->file[fd]))) 
                               < (UINT32 )(&(fs->file[fd].deled))-(UINT32)(&(fs->file[fd]))) {
                           //only support page erase can do re-write
                           //erase page before write a page if page-erase supported
30 9    ( )( )            if (fs->pageErase == 1) {
                               sffs_erase_at(fs, 0*(fs->se_size / fs->ss_size)+ fs->file[fd].hdr_ss ,1);
30 10   ( )( )                if ( sffs_write_at(fs, ofs, &(fs->file[fd]), 
                                           (UINT32 )(&(fs->file[fd].deled))-(UINT32)(&(fs->file[fd])))
                                       < (UINT32 )(&(fs->file[fd].deled))-(UINT32)(&(fs->file[fd]))) {
                                   NOLOG("p close file %d fail name %s\n",fd,fs->file[fd].name);
                                   fs->free_ss_hdr[fs->file[fd].hdr_ss]=3;
                                   fs->file[fd].comt=0xee; //mark as not finished
                                   fs->file[fd].fid = 0;
                                   fs->file[fd].seq = 0;
                                   fs->fileNum --;
                                   sffs_set_errno(fs,-10);
                                   return -1;
                               }
                           }
                           else {
                               NOLOG("np close file %d fail name %s\n",fd,fs->file[fd].name);
                               hdrsStat[fs->file[fd].hdr_ss] =2;
                               fs->free_ss_hdr[fs->file[fd].hdr_ss]=2;
                               fs->file[fd].comt=0xee; //mark as not finished
                               fs->file[fd].fid = 0;
                               fs->file[fd].seq = 0;
                               fs->fileNum --;
                               sffs_set_errno(fs,-10);
                               return -1;
                           }
                       }
                   }
                               UINT8 hdd[(((((((((((((0+4)+18)+2)+ 4) + 4) + 4) + 4) + 16*2) + 2) +1) + 1) +1)+2)];
                               int i;
30 11   ( )( )   if ( sffs_read_at(fs, ofs, hdd, (UINT32 )(&(fs->file[fd].deled))-(UINT32)(&(fs->file[fd]))) 
                                       < (UINT32 )(&(fs->file[fd].deled))-(UINT32)(&(fs->file[fd]))) {
                                   //printf("wr fail\n");
                               }
                               //printf("usu\n");
30 12   ( )( )   for(i=0;i<(((((((((((((0+4)+18)+2)+ 4) + 4) + 4) + 4) + 16*2) + 2) +1) + 1) +1)+2);i++){
                                   //printf("%d:%x\n",i,hdd[i]);
                               }
                               //printf("\n");
30 13   ( )( )   if(fs->pageErase != 1) {
                       hdrsStat[fs->file[fd].hdr_ss] =1;
                       fs->free_ss_hdr[fs->file[fd].hdr_ss]=1;
                   }
                   fs->fidcnt ++;
                   fs->seqcnt ++;
                   fs->file[fd].stat = 0;// set file stat into zen
                   return 0;
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
                   UINT8 *temp,buf[(4*1024)];
                   INT32 size1,start;
                   struct sffs_entry *file;
31 0    ( )    sffs_write
                   INT32 ofsb=0,ofs;
                   temp = (UINT8 *)data;
                   //check fd is legal and size legal
31 1    ( )( )   if (sffs_ck_fd(fs,fd) < 0 || size <0 || size > (2*1024*1024)) {
                       sffs_set_errno(fs,-1);
                       return -1;
                   }
                   file = &(fs->file[fd]);
31 2    ( )( )   if (file->stat != 2) {
                       NOLOG("not in correct mode\n");
                       sffs_set_errno(fs,-19);
                       return -1;
                   }
31 3    ( )( )   if (size == 0)
                       return 0;
                   //file can be write only one time
31 4    ( )( )   if(file->comt == 0xc0) {
                       NOLOG("file cannot be modify");
                       sffs_set_errno(fs,-12);
                       return -1;
                   }
                   ofsb = file->ofs;
31 5    ( )( )   if(file->ofs + size >(2*1024*1024)) {
                       sffs_set_errno(fs,-16);
                       return -1;
                   }
                   //read-combin-write(concern for byte align)(can be optimized)
                   //concern for non-word align
                   size1 = fs->ss_size - file->ofs%fs->ss_size;
                   start = file->ofs/fs->ss_size*fs->ss_size;
                   ofs = ofsb % fs->ss_size;
31 6    ( )( )   if (size1 != fs->ss_size) {
31 7    ( )( )        if (size < size1) {
                           size1 =size;
                       }
31 8    ( )( )        if(sffs_read_at(fs,start,buf,ofs)<ofs) {
                           sffs_set_errno(fs,-11);
                           return -1;
                       }
                       //memcpy(buf,temp,size1);    
31 9    ( )( )        if (sffs_write_4k(fs,fd,buf,ofs+size1) < (ofs+size1)) {
31 10   ( )( )            if (fs->pageErase == 1) {
                               sffs_erase_at(fs,fs->file[fd].ofs/fs->ss_size,1);
31 11   ( )( )                if (sffs_write_4k(fs,fd,buf,ofs+size1) < (ofs+size1)) {
                                   sffs_set_errno(fs,-17);
                                   return file->ofs-ofsb;
                               }
                           }
                           else {
                               sffs_set_errno(fs,-17);
                               return file->ofs-ofsb;
                           }
                       }
                       temp += size1;
                       size -= size1;
                   }
                   size1 = fs->ss_size;
31 12   ( )( )   if(size > fs->ss_size) {
31 13   ( )( )        while(size >= fs->ss_size) {
31 14   ( )( )            if (sffs_write_4k(fs,fd,temp,size1)<size1) {
31 15   ( )( )                if (fs->pageErase == 1) {
                                   sffs_erase_at(fs,fs->file[fd].ofs/fs->ss_size,1);
31 16   ( )( )                    if (sffs_write_4k(fs,fd,temp,size1)<size1) {
                                       sffs_set_errno(fs,-17);
                                       return file->ofs-ofsb;
                                   }
                               }
                               else {
                                   sffs_set_errno(fs,-17);
                                   return file->ofs-ofsb;
                               }
                           }
                           size -= size1;
                           temp += size1;
                       }
                   }
31 17   ( )( )   if(size >0) {
31 18   ( )( )        if (sffs_write_4k(fs,fd,temp,size) < size) {
31 19   ( )( )            if (fs->pageErase == 1) {
                               sffs_erase_at(fs,fs->file[fd].ofs/fs->ss_size,1);
31 20   ( )( )                if (sffs_write_4k(fs,fd,temp,size) < size) {
                                   sffs_set_errno(fs,-17);
                                   return file->ofs-ofsb;
                               }
                           }
                           else {
                               sffs_set_errno(fs,-17);
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
32 0    ( )    sffs_read
                   ret = size;
                   size1 =size;
                   temp = (UINT8 *)data;
                   //check fd is legal
32 1    ( )( )   if (sffs_ck_fd(fs,fd) < 0 || size <0 || size >(2*1024*1024)) {
                       sffs_set_errno(fs,-1);
                       return -1;
                   }
32 2    ( )( )   if (size == 0)
                       return 0;
                   //divide data into three parts:not aligned start,aligned middle and not aligned end
                   file = &(fs->file[fd]);
32 3    ( )( )   if (file->stat != 1) {
                       NOLOG("not in read mode\n");
                       sffs_set_errno(fs,-18);
                       return -1;
                   }
                   ofsb = file->ofs;
32 4    ( )( )   if(file->ofs + size > file->size) {
                       size = file->size - file->ofs;
                       NOLOG("exceed file max len %d,can only read %d \n",(2*1024*1024),size);
                       sffs_set_errno(fs,-16);
                   }
32 5    ( )( )   if (file->ofs%fs->ss_size !=0) {
                       size1 = fs->ss_size - file->ofs%fs->ss_size;
32 6    ( )( )        if (size1 > size) {
                           size1 = size;
                       }
                       ret = sffs_read_4k(fs,fd,temp,size1); 
32 7    ( )( )        if (ret < size1) {
                           sffs_set_errno(fs,-11);
                           return ret;
                       }
                       temp += size1;
                       size -= size1;
                   }
32 8    ( )( )   if (size >= fs->ss_size) {
32 9    ( )( )        while(size >= fs->ss_size) {
32 10   ( )( )            if (sffs_read_4k(fs,fd,temp,fs->ss_size) < fs->ss_size) {
                               sffs_set_errno(fs,-11);
                               return file->ofs-ofsb;
                           }
                           size -= fs->ss_size;
                           temp += fs->ss_size;
                       }
                   }
32 11   ( )( )   if(size > 0) {
32 12   ( )( )        if (sffs_read_4k(fs,fd,temp,size) <size) {
                           sffs_set_errno(fs,-11);
                           return file->ofs-ofsb;
                       }
                       size = 0;
                   }
                   return file->ofs-ofsb;
               }
               INT32 sffs_rename(struct sffs *fs,const INT8 *old_name,const INT8 *new_name)
               {
                   INT32 pos1,pos2,nl,new_hs,old_hs,ofs_old,ofs_new;
                   struct sffs_entry *file;
33 0    ( )    sffs_rename
                   UINT8 data = 0xde;
33 1    ( )( )   if(old_name == ((void *)0) || new_name == ((void *)0)){
                       sffs_set_errno(fs,-1);
                       return -1;
                   }
               	else 
33 2    ( )           if (1) //(strlen((char *)new_name) == 1) strlen((char *)old_name) && strncmp((char*)old_name,(char *)new_name,strlen((char *)old_name))==0)
               	{
                       return 0;
                   }
                   //old_name should exist and new_name should should not 
               	pos1 = 1;// sffs_find_filehdr(fs, old_name);
33 3    ( )( )   if (pos1 <0){
                       sffs_set_errno(fs,-8);
                       return -1;
                   }
               	pos2 = 1;// sffs_find_filehdr(fs, new_name);
33 4    ( )( )   if (pos2 >= 0){
                       sffs_set_errno(fs,-26);
                       return -1;
                   }
                   //file has been written to flash then should rewrite flash,or just rename the file structure
                   file = &(fs->file[pos1]);
33 5    ( )      if(1)//(nl = strlen((char *)new_name))>FNAME_LEN)
               	{
                       nl=18;
                   }
33 6    ( )( )   if (file ->comt != 0xc0){
                       //strncpy((char *)file->name,(char *)new_name,nl);
                       file->name_len = nl;
                   }
                   else {
                       ofs_old=0*fs->se_size + file->hdr_ss*fs->ss_size ;//+ (UINT32 )(&(file->deled)) - (UINT32 )((file));
                       old_hs = file->hdr_ss;
33 7    ( )( )        if ( (new_hs = sffs_get_hdrss(fs,pos1)) == -1) {
                           NOLOG("SFFS: get flash hdr fail %d\n",new_hs);
                           return -1;
                       }
                       memset((char *)file->name,0,sizeof(file->name));
                       //strncpy((char *)file->name,(char *)new_name,nl);
                       file->name[nl]='\0';
                       file->name_len = nl;
                       file->hdr_ss=new_hs;
                       file->seq=fs->seqcnt;
                       ofs_new=file->hdr_ss*fs->ss_size + 0*fs->se_size;
                       char hdr1[(((((((((((((0+4)+18)+2)+ 4) + 4) + 4) + 4) + 16*2) + 2) +1) + 1) +1)+2)];
33 8    ( )( )        if( sffs_read_at(fs, ofs_new, hdr1, (UINT32 )(&(file->deled))-(UINT32)((file))) !=
                               (UINT32 )(&(file->deled))-(UINT32)(&(file)) ) {
                           //printf("rwread fail\n");
                       }
                       //printf("fd\n");
33 9    ( )( )        for(UINT32 i=0;i<((UINT32 )(&(file->deled))-(UINT32)(&(file)));i++) {
                           //printf("%d:%x\n",i,hdr1[i]);
                       }
                       //printf("\n");
                       //write new hdr
33 10   ( )( )        if( sffs_write_at(fs, ofs_new, file, (UINT32 )(&(file->deled))-(UINT32)((file))) !=
                               (UINT32 )(&(file->deled))-(UINT32)((file)) ) {
                           sffs_set_errno(fs,-27);
                           fs->free_ss_hdr[new_hs]=3;
33 11   ( )( )            if(fs->pageErase != 1)
                               hdrsStat[new_hs]=3;
                           file->seq=0;
                           return -1;
                       }
                       fs->free_ss_hdr[new_hs]=1;
33 12   ( )( )        if(fs->pageErase != 1)
                           hdrsStat[new_hs]=1;
                       //delete old hdr
                       ofs_old += (UINT32 )(&(file->deled)) - (UINT32 )((file));
33 13   ( )( )        if ( sffs_write_at(fs,ofs_old,&data, 1) != 1 ) {
                           sffs_set_errno(fs,-28);
                           fs->free_ss_hdr[old_hs]=3;
33 14   ( )( )            if(fs->pageErase != 1)
                               hdrsStat[old_hs]=3;
                           file->seq=0;
                           return -1;
                       }
                       fs->free_ss_hdr[old_hs]=2;
33 15   ( )( )        if(fs->pageErase != 1)
                           hdrsStat[old_hs]=2;
                       fs->seqcnt++;
                   }
                   return 0;
               }
               INT32 sffs_lseek(struct sffs *fs, INT32 fd, INT32 pos)
               {
34 0    ( )    sffs_lseek
34 1    ( )( )   if (sffs_ck_fd(fs,fd) < 0 || pos < 0) {
                       sffs_set_errno(fs,-1);
                       return -1;
                   }
                   //cannot seek in writing mode
34 2    ( )( )   if (sffs_ckstat(fs,fd) == 2) {
                       sffs_set_errno(fs,-19);
                       return -1;
                   }
34 3    ( )( )   if (fs->file[fd].ofs + pos > fs->file[fd].size)
                       fs->file[fd].ofs = fs->file[fd].size -1;
                   else {
                       fs->file[fd].ofs = fs->file[fd].ofs + pos;
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
                   INT32 ofs,size;
                   UINT8 data;
                   INT32 ret;
35 0    ( )    sffs_remove_byfd
35 1    ( )( )   if(fs->file[fd].comt != 0xc0) {
                       NOLOG("remove a opened file\n");
                       sffs_set_errno(fs,-20);
                       fs->free_ss_hdr[fs->file[fd].hdr_ss]=-1;
35 2    ( )( )        if (fs->pageErase != 1)
                           hdrsStat[fs->file[fd].hdr_ss] = 0;
                       sffs_reset_filehdr(fs,fd);
                       return 0;
                   }
                   data = 0xde;
                   size = 1;
                   ofs = 0*fs->se_size + fs->file[fd].hdr_ss * fs->ss_size ;
                   ofs += (UINT32 )(&(fs->file[fd].deled)) - (UINT32 )(&(fs->file[fd]));
                   NOLOG("rmove hdr size %d ,ofs %d hdr_ss %d\n",size,ofs,fs->file[fd].hdr_ss);
                   fs->free_ss_hdr[fs->file[fd].hdr_ss]=-1;
                   //mark file as deleted
                   ret = sffs_write_at(fs, ofs, &data,size);
35 3    ( )( )   if (ret != size) {
35 4    ( )( )        if (fs->pageErase == 1) {
                           //write error, so erase this sub-sector
35 5    ( )( )            if (sffs_erase_at(fs,fs->file[fd].hdr_ss,1)<0) {
                               NOLOG("delete fail %d\n",ret);
                           }
                           else {
                               sffs_reset_filehdr(fs, fd);
                               fs->fileNum -- ;
                               fs->free_ss_hdr[fs->file[fd].hdr_ss]=-1;
                               return 0;
                           }
                       }
                       else {
                           hdrsStat[fs->file[fd].hdr_ss]=2;
                       }
                       fs->free_ss_hdr[fs->file[fd].hdr_ss]=-1;
                       sffs_reset_filehdr(fs, fd); //???
                       sffs_set_errno(fs,-21);
                       fs->fileNum -- ;
                       return -1;
                   }
35 6    ( )( )   if (fs->pageErase == 1) 
                       hdrsStat[fs->file[fd].hdr_ss]=2;
                   fs->free_ss_hdr[fs->file[fd].hdr_ss]=2;
                   sffs_reset_filehdr(fs, fd);
                   fs->fileNum -- ;
                   return 0;
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
36 0    ( )    sffs_remove_byname
                pos = 1;// sffs_find_filehdr(fs, fname);
36 1    ( )( )   if (pos < 0) {        
                       NOLOG("no file %s\n",fname);
                       sffs_set_errno(fs,-8);
                       return -1;
                   } else {
                       return sffs_remove_byfd(fs, pos);
                   }
               }
               /* format flash with specific layout ? */
               INT32 sffs_format(struct sffs *fs, INT32 type) 
               {
                   INT32 ret;
37 0    ( )    sffs_format
                   switch (type) {
37 1    ( )        case 2:
                           break;
37 2    ( )        case 0:
                           //only erase hdrs
                           ret = sffs_erase_at(fs,0,1);
37 3    ( )( )       if(ret <0){
                               ret = sffs_erase_at(fs,0,1);
37 4    ( )( )                if(ret <0){
                                   sffs_set_errno(fs, -4);
                                   NOLOG("erase hdr faile\n");
                                   return -1;
                               }
                           }
                           sffs_start(fs);
                           break;
37 5    ( )        case 1:
                           //erase whole flash
                           sffs_start(fs); //reset after hdr flash sector erased
37 6    ( )( )       for(INT32 i=0;i<fs->device_size/fs->erase_unit;i++) {
                               ret = sffs_erase_at(fs,i,1);
37 7    ( )( )                if(ret <0) {
                                   ret = sffs_erase_at(fs,i,1);
37 8    ( )( )                    if(ret <0) {
                                       sffs_set_errno(fs, -13);
                                       NOLOG("erase whole flash faile\n");
                                       return -1;
                                   }
                               }
                           }
                           break;
37 9    ( )        default:
                           NOLOG("parameter err\n");
                           break;
                   }
                   return 0;
               }
               /* umount sffs */
               INT32 sffs_umount(struct sffs *fs) 
               {
38 0    (T)    sffs_umount
                   fs->mounted = 0;
                   sffs_start(fs);
                   return 0;
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
               //UINT8 header[SFFS_HEADER_SIZE+1];
                   UINT8 header[sizeof(struct sffs_entry)+1];
                   INT32 ret;//fd,fid;
39 0    ( )    sffs_mount
                   i = k = 0;
                   offset = 0*fs->se_size;
39 1    ( )( )   if( fs->mounted == 1)
                       return 0;
39 2    ( )( )   if (type == 1) {
                       //erase all
                       fs->mounted = 1;
                       return current_fs;
                   }
39 3    ( )( )   if (sffs_start(fs) <0) {
                       return -1;
                   }
39 4    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); return -1; };
                   NOLOG("device size 0x%x magic 0x%x sb %d\n",fs->device_size,fs->magic_num,0);
39 5    ( )( )   while(offset < fs->se_size+0*fs->se_size &&
                           sffs_read_at(fs, offset,header, sizeof(struct sffs_entry)) == sizeof(struct sffs_entry)) {
                       //deleted or not commited or broken hdr or empty files ommit
39 6    ( )( )        if (header[(((((((((((0+4)+18)+2)+ 4) + 4) + 4) + 4) + 16*2) + 2) +1) + 1)] != 0xff || 
                               header[(((((((((0+4)+18)+2)+ 4) + 4) + 4) + 4) + 16*2) + 2)] != 0xc0 ||
                               *(UINT32 *)(header) != 0xCA905001) {
                           //deleted or file hdr broken
39 7    ( )( )            if(fs->free_ss_hdr_cur < 0) {    //set the first free ss as free_ss_hdr_cur
                               fs->free_ss_hdr_cur = i;
                           }
39 8    ( )( )            if(fs->pageErase != 1)
                               hdrsStat[i]=2;//assume this is not a dirty sector and erased before using
                           i++;
                           offset=0*fs->se_size + i*fs->ss_size;
                           continue;
                       }
                       //got one file
                       //memcpy(&(fs->file[fs->fileNum]), header, sizeof(struct sffs_entry));
                       //check: if two file has the same fid,so delete the newer(bigger) one
                       ret = sffs_check_fid(fs, fs->file[fs->fileNum].fid);
39 9    ( )( )        if (ret != -1) {
                           INT32 fhdrs_t;
                           UINT8 data = 0xde;
                           INT32 ofs0 ;
                           //delete the newer one's hdr_ss, and continue 
39 10   ( )( )            if(fs->file[fs->fileNum].seq < fs->file[ret].seq)
               			{
                               memcpy(&(fs->file[ret]), &(fs->file[fs->fileNum]), sizeof(struct sffs_entry)); //the latter one should replace the former one
                               fhdrs_t = ret;
                           }
                           else
                               fhdrs_t = fs->fileNum;
                           ofs0 = (UINT32 )(&(fs->file[fhdrs_t].deled)) - (UINT32 )(&(fs->file[fhdrs_t])) + fs->file[fhdrs_t].hdr_ss*fs->ss_size;
39 11   ( )( )            if (sffs_write_at(fs,ofs0,&data,1) != 1) {
39 12   ( )( )                if (fs->pageErase == 1) {
39 13   ( )( )                    if (sffs_write_at(fs,ofs0,&data,1) != 1) {
                                       sffs_set_errno(fs,-10);
                                       sffs_unlock(fs);;
                                       return -1;
                                   }
                               }
                               else {
                                   sffs_set_errno(fs,-10);
                                   sffs_unlock(fs);;
                                   return -1;
                               }
                           }
                           fs->file[fhdrs_t].stat = 0;
                           i++;
                           offset = 0*fs->se_size + i*fs->ss_size;
39 14   ( )( )            if (fs->pageErase != 1)
                               hdrsStat[fs->file[fs->fileNum].hdr_ss]=1;//a used sector
                           continue;
                       }
                       fs->file[fs->fileNum].stat = 0;
39 15   ( )( )        if(fs->file[fs->fileNum].fid >fs->fidcnt)
                           fs->fidcnt = fs->file[fs->fileNum].fid; 
39 16   ( )( )        if(fs->file[fs->fileNum].seq > fs->seqcnt)
                           fs->seqcnt = fs->file[fs->fileNum].seq; 
                       fs->free_ss_hdr[fs->file[fs->fileNum].hdr_ss]=1;
39 17   ( )( )        if (fs->pageErase != 1) {
                               hdrsStat[fs->file[fs->fileNum].hdr_ss]=1;//a used sector
                       }
                       //modify free se no in fs
39 18   ( )( )        for(k=0;k<16 && fs->file[fs->fileNum].se_no[k]>0;k++) {
                           sffs_set_sebit(fs,fs->file[fs->fileNum].se_no[k]);
                       }
                       NOLOG("got file ss %d cmt 0x%x del 0x%x magic 0x%x\n",i,header[(((((((((0+4)+18)+2)+ 4) + 4) + 4) + 4) + 16*2) + 2)],header[(((((((((((0+4)+18)+2)+ 4) + 4) + 4) + 4) + 16*2) + 2) +1) + 1)]);
                       NOLOG("mn 0x%x name %s fid %d nl %d size %d ofs 0x%x hdr %d,cmt 0x%x, del %d\n",
                               fs->file[fs->fileNum].magic_num, fs->file[fs->fileNum].name, fs->file[fs->fileNum].fid, 
                               fs->file[fs->fileNum].name_len, fs->file[fs->fileNum].size, fs->file[fs->fileNum].ofs, 
                               fs->file[fs->fileNum].hdr_ss, fs->file[fs->fileNum].comt, fs->file[fs->fileNum].deled);
                       i++;
                       offset=0*fs->se_size + i*fs->ss_size;
                       fs->fileNum ++;
                   }
                   fs->fidcnt++;
39 19   ( )( )   if (fs->fileNum<1) {
                       NOLOG("empty \n");
39 20   ( )( )        if(fs->pageErase != 1)
                           fsStat = 1;//empty
                   }
39 21   ( )( )   if (type == 2) {
                       //erase empty se
                   }
                   fs->mounted = 1;
                   NOLOG("SFFS: mounted!\n");
                   sffs_unlock(fs);;
                   return current_fs;
               }
               /**********************************************************/
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
               UINT32 fs_get_err(void)
               {
40 0    ( )    fs_get_err
                   struct sffs *fs=&os_sffs; INT32 ret = -1; 
40 1    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); NOLOG("LOCK ERROR\n"); return -1; } 
40 2    ( )( )   if (!sffs_is_mounted(fs)) { sffs_set_errno(fs,-23); NOLOG("NOT mounted\n"); sffs_unlock(fs); return -1; };
                   ret = fs->errno;
                   sffs_unlock(fs); return ret;;
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
41 0    ( )    fs_mount
                   struct sffs *fs=&os_sffs;
                   INT32 ret = -1;
                   INT32 locker;
                   locker = intLock();
                   ret = sffs_mount(fs,type);
                   intUnlock(locker);
               	////printf("mounted %d\n", ret);
                   return ret;
               }
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
42 0    ( )    fs_umount
                   struct sffs *fs=&os_sffs; INT32 ret = -1; 
42 1    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); NOLOG("LOCK ERROR\n"); return -1; } 
42 2    ( )( )   if (!sffs_is_mounted(fs)) { sffs_set_errno(fs,-23); NOLOG("NOT mounted\n"); sffs_unlock(fs); return -1; };
                   ret = sffs_umount(fs);
                   sffs_unlock(fs); return ret;;
               }
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
43 0    ( )    fs_file_len
                   struct sffs *fs=&os_sffs; INT32 ret = -1; 
43 1    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); NOLOG("LOCK ERROR\n"); return -1; } 
43 2    ( )( )   if (!sffs_is_mounted(fs)) { sffs_set_errno(fs,-23); NOLOG("NOT mounted\n"); sffs_unlock(fs); return -1; };
43 3    ( )( )   if ((ret=1/*sffs_find_filehdr(fs, fname)*/) < 0) {
                       sffs_set_errno(fs,-8);
                       return -1;
                   }
                   ret = fs->file[ret].size;
                   sffs_unlock(fs); return ret;;
               }
               INT32 fs_rename(const INT8 *old_name, const INT8 *new_name)
               {
44 0    ( )    fs_rename
                   struct sffs *fs=&os_sffs; INT32 ret = -1; 
44 1    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); NOLOG("LOCK ERROR\n"); return -1; } 
44 2    ( )( )   if (!sffs_is_mounted(fs)) { sffs_set_errno(fs,-23); NOLOG("NOT mounted\n"); sffs_unlock(fs); return -1; };
                   ret = sffs_rename(fs,old_name,new_name);
                   sffs_unlock(fs); return ret;;
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
45 0    ( )    fs_lseek
                   struct sffs *fs=&os_sffs; INT32 ret = -1; 
45 1    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); NOLOG("LOCK ERROR\n"); return -1; } 
45 2    ( )( )   if (!sffs_is_mounted(fs)) { sffs_set_errno(fs,-23); NOLOG("NOT mounted\n"); sffs_unlock(fs); return -1; };
45 3    ( )( )   if (sffs_ck_fd(fs,fd) < 0) {
                       sffs_set_errno(fs,-9);
                       sffs_unlock(fs);;
                       return ret;
                   }
                   switch (cfg) {
45 4    ( )        case 1:    
                       //ftell
                           ret=fs->file[fd].ofs; 
                           break;
45 5    ( )        case 0:
                           ret=sffs_lseek(fs,fd,pos);
                           break;
45 6    ( )        default :
                           sffs_set_errno(fs,-1);
                           ret = -1;
                           break;
                   }
                   sffs_unlock(fs); return ret;;
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
               UINT32 fs_free(void) 
               {
46 0    ( )    fs_free
                   struct sffs *fs=&os_sffs; INT32 ret = -1; 
46 1    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); NOLOG("LOCK ERROR\n"); return -1; } 
46 2    ( )( )   if (!sffs_is_mounted(fs)) { sffs_set_errno(fs,-23); NOLOG("NOT mounted\n"); sffs_unlock(fs); return -1; };
                   INT32 i;
                   ret = 0;
46 3    ( )( )   for(i=0;i<sizeof(fs->free_se)/4;i++) {
                       ret += get_all_one(fs->free_se[i]);
                   }
                   ret = fs->device_size - (ret +1)*fs->se_size;
                   sffs_unlock(fs); return ret;;
               }
               /*******************************************************************************
               *
               * fs_used - get the size of all files
               * NOTICE:
               *
               * RETURNS: all files' size
               *
               */
               UINT32 fs_used(void)
               {
47 0    ( )    fs_used
                   struct sffs *fs=&os_sffs; INT32 ret = -1; 
47 1    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); NOLOG("LOCK ERROR\n"); return -1; } 
47 2    ( )( )   if (!sffs_is_mounted(fs)) { sffs_set_errno(fs,-23); NOLOG("NOT mounted\n"); sffs_unlock(fs); return -1; };
                   INT32 i;
                   ret=0;
47 3    ( )( )   for(i=0;i<20;i++) {
47 4    ( )( )        if (fs->file[i].comt == 0xc0) {
                           ret+=fs->file[i].size;
                       }
                   }
                   sffs_unlock(fs); return ret;;
               }
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
48 0    ( )    fs_open
                   struct sffs *fs=&os_sffs; INT32 ret = -1; 
48 1    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); NOLOG("LOCK ERROR\n"); return -1; } 
48 2    ( )( )   if (!sffs_is_mounted(fs)) { sffs_set_errno(fs,-23); NOLOG("NOT mounted\n"); sffs_unlock(fs); return -1; };
                   switch(flags) {
48 3    ( )        case 0://normal open :if file not exist then return fail
48 4    ( )          if (1/*sffs_find_filehdr(fs,fname)*/ <0 ) {
                               sffs_unlock(fs); return ret;;
                               return -1;
                           }
                           break;
48 5    ( )        case 1://truncation
                           sffs_remove_byname(fs,fname);
                           break;
48 6    ( )        case 2://if file not exist then create
                           break;
48 7    ( )        default:
                           sffs_set_errno(fs,-1);
                           sffs_unlock(fs); return ret;;
                   }
                   ret = sffs_open(fs,fname,mode);
                   sffs_unlock(fs); return ret;;
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
               INT32 fs_remove(const INT8 *fname)
               {
49 0    ( )    fs_remove
                   struct sffs *fs=&os_sffs; INT32 ret = -1; 
49 1    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); NOLOG("LOCK ERROR\n"); return -1; } 
49 2    ( )( )   if (!sffs_is_mounted(fs)) { sffs_set_errno(fs,-23); NOLOG("NOT mounted\n"); sffs_unlock(fs); return -1; };
                   ret = sffs_remove_byname(fs, fname);
                   sffs_unlock(fs); return ret;;
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
50 0    ( )    fs_read
                   struct sffs *fs=&os_sffs; INT32 ret = -1; 
50 1    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); NOLOG("LOCK ERROR\n"); return -1; } 
50 2    ( )( )   if (!sffs_is_mounted(fs)) { sffs_set_errno(fs,-23); NOLOG("NOT mounted\n"); sffs_unlock(fs); return -1; };
50 3    ( )( )   if(buf == ((void *)0)) { sffs_set_errno(fs,-25); NOLOG("EMPTY POINTER\n"); 
50 4    ( )           while(1); sffs_unlock(fs); return ret;; };
                   ret = sffs_read(fs,fd, buf,count);    
                   sffs_unlock(fs); return ret;;
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
51 0    ( )    fs_write
                   struct sffs *fs=&os_sffs; INT32 ret = -1; 
51 1    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); NOLOG("LOCK ERROR\n"); return -1; } 
51 2    ( )( )   if (!sffs_is_mounted(fs)) { sffs_set_errno(fs,-23); NOLOG("NOT mounted\n"); sffs_unlock(fs); return -1; };
51 3    ( )( )   if(buf == ((void *)0)) { sffs_set_errno(fs,-25); NOLOG("EMPTY POINTER\n"); 
51 4    ( )           while(1); sffs_unlock(fs); return ret;; };
                   ret = sffs_write(fs, fd, buf, count);
                   sffs_unlock(fs); return ret;;
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
52 0    ( )    fs_close
                   struct sffs *fs=&os_sffs; INT32 ret = -1; 
52 1    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); NOLOG("LOCK ERROR\n"); return -1; } 
52 2    ( )( )   if (!sffs_is_mounted(fs)) { sffs_set_errno(fs,-23); NOLOG("NOT mounted\n"); sffs_unlock(fs); return -1; };
                   ret = sffs_close(fs,fd);
                   sffs_unlock(fs); return ret;;
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
53 0    ( )    fs_format
                   struct sffs *fs=&os_sffs; INT32 ret = -1; 
53 1    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); NOLOG("LOCK ERROR\n"); return -1; } 
53 2    ( )( )   if (!sffs_is_mounted(fs)) { sffs_set_errno(fs,-23); NOLOG("NOT mounted\n"); sffs_unlock(fs); return -1; };
                   ret = sffs_format(fs,type);
                   return ret; 
               }
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
54 0    ( )    fs_ls
                   struct sffs *fs=&os_sffs; INT32 ret = -1; 
54 1    ( )( )   if (sffs_lock(fs)!=0) { sffs_set_errno(fs, -2); NOLOG("LOCK ERROR\n"); return -1; } 
54 2    ( )( )   if (!sffs_is_mounted(fs)) { sffs_set_errno(fs,-23); NOLOG("NOT mounted\n"); sffs_unlock(fs); return -1; };
                   struct sffs_entry files[(32)];
                   INT32 i,j,k;
                   INT8 tname[18+1];
                   j=k=0;
                   NOLOG("flash: %s\nfname    | size    | hdr_ss  | comt | deled |  magic    |   ofs     | valid  | fid \n",fs->fsname);
54 3    ( )( )   for(i=0;i<(fs->se_size/fs->ss_size);i++) {
                       sffs_read_at(fs,0*fs->se_size+i*fs->ss_size ,&files[i],sizeof(struct sffs_entry));
54 4    ( )( )        if(files[i].magic_num == 0xCA905001) {
54 5    ( )( )            if (files[i].name_len <=18 ) {
                               memcpy(tname,files[i].name,files[i].name_len);
                               tname[files[i].name_len]='\0';
                               NOLOG("%8s |%8d |   %3d   | 0x%2x | 0x%2x  |0x%8x |0x%08x |   %2d | 0x%08x \n",
                                       tname, files[i].size, files[i].hdr_ss, files[i].comt, files[i].deled, files[i].magic_num, files[i].ofs, 
54 6    ( )( )                files[i].deled==0xff?1:0,files[i].fid);
54 7    ( )( )                if(files[i].deled == 0xff)
                                   k++;
                               j++;
                           }
                       }
                   }
                   NOLOG("flash has %d hdr ,%d file \n\n",j,k);
                   j=0;
                   NOLOG("memory:\nfname    | size    | hdr_ss  | comt | deled |  magic    | ofs     | stat | fid \n");
54 8    ( )( )   for(i=0;i<20;i++) {
54 9    ( )( )        if (fs->file[i].name_len && fs->file[i].stat != 0 ) {
                           NOLOG("%8s |%8d |   %3d   |  %2d  |  %2d   |0x%8x |0x%08x |  %2d  | 0x%08x \n",
                                   fs->file[i].name, fs->file[i].size, fs->file[i].hdr_ss, 
54 10   ( )( )            fs->file[i].comt==0xc0?1:0, 
54 11   ( )( )            fs->file[i].deled==0xff?0:1, fs->file[i].magic_num, fs->file[i].ofs, fs->file[i].stat,fs->file[i].fid);
                           j++;
                       }
                   }
                   NOLOG("opened %d files.\n",j);
                   sffs_unlock(fs); return ret;;
               }


48 of 475 branches covered (10%)
