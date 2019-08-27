
#pragma once
#include <libc/cagboot.h>
#include "sffs/sffs.h"
INT32 fs_rename(const INT8 *old_name,const INT8 *new_name);
INT32 fs_format(INT32 type);
INT32 fs_umount(INT32 fsid);
INT32 fs_mount2(struct sffs sfs, INT32 type);
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
