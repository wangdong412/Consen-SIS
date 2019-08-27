#pragma once

/*
 * |主模块号（8bit）|错误等级 （2bit）                  |文件号（6bit） |错误号 （16bit） |
 * |0x06            | 0-致命   1-严重  2-一般   3-提示  |               |                 | 
 */

#define LOG_ERR_OS          (0x06<<24)

#define LOG_ERR_LEVEL_0     (0)
#define LOG_ERR_LEVEL_1     (1<<22)
#define LOG_ERR_LEVEL_2     (2<<22)
#define LOG_ERR_LEVEL_3     (3<<22)


#define LOG_ERR_ARCH        (0<<16|LOG_ERR_OS|LOG_ERR_LEVEL_1)
#define LOG_ERR_START       (1<<16|LOG_ERR_OS|LOG_ERR_LEVEL_1)
#define LOG_ERR_KERNEL      (2<<16|LOG_ERR_OS|LOG_ERR_LEVEL_1)
#define LOG_ERR_UTIL        (3<<16|LOG_ERR_OS|LOG_ERR_LEVEL_1)
#define LOG_ERR_BUS         (4<<16|LOG_ERR_OS|LOG_ERR_LEVEL_1)
#define LOG_ERR_FS          (5<<16|LOG_ERR_OS|LOG_ERR_LEVEL_1)
#define LOG_ERR_LIBC        (6<<16|LOG_ERR_OS|LOG_ERR_LEVEL_1)
#define LOG_ERR_DFS         (7<<16|LOG_ERR_OS|LOG_ERR_LEVEL_1)
#define LOG_ERR_FPGA        (8<<16|LOG_ERR_OS|LOG_ERR_LEVEL_1)
#define LOG_ERR_PCIE        (9<<16|LOG_ERR_OS|LOG_ERR_LEVEL_1)
#define LOG_ERR_NOR         (0xa<<16|LOG_ERR_OS|LOG_ERR_LEVEL_1)
#define LOG_ERR_SPIFLASH    (0xb<<16|LOG_ERR_OS|LOG_ERR_LEVEL_1)
#define LOG_ERR_HWI         (0xc<<16|LOG_ERR_OS|LOG_ERR_LEVEL_1)

/*log start*/

//libc
#define LOG_ERR_STR         (LOG_ERR_LIBC|(0x1<<12))
#define LOG_ERR_MEM         (LOG_ERR_LIBC|(0x2<<12))

#define STR_NULLP_ERR       (LOG_ERR_STR|0x1)


//bus
#define LOG_ERR_PMBUS       (LOG_ERR_BUS|(0x1<<12))
#define LOG_ERR_CMBUS       (LOG_ERR_BUS|(0x2<<12))
#define LOG_ERR_IPBUS       (LOG_ERR_BUS|(0x3<<12))

#define PMBUS_NREADY_ERR1    (LOG_ERR_PMBUS|0x1)
#define PMBUS_NREADY_ERR2    (LOG_ERR_PMBUS|0x2)
#define PMBUS_NREADY_ERR3    (LOG_ERR_PMBUS|0x3)
#define PMBUS_NREADY_ERR4    (LOG_ERR_PMBUS|0x4)

#define PMBUS_CNULLP_ERR    (LOG_ERR_PMBUS|0x5)
#define PMBUS_DATARECVFAIL_ERR1  (LOG_ERR_PMBUS|0x6)
#define PMBUS_DATARECVFAIL_ERR2  (LOG_ERR_PMBUS|0x7)
#define PMBUS_STATRECVFAIL_ERR1  (LOG_ERR_PMBUS|0x8)
#define PMBUS_STATRECVFAIL_ERR2  (LOG_ERR_PMBUS|0x9)
#define PMBUS_SENDFAIL_ERR1      (LOG_ERR_PMBUS|0xa)
#define PMBUS_SENDFAIL_ERR2      (LOG_ERR_PMBUS|0xa)

#define PMBUS_PARA_ERR1       (LOG_ERR_PMBUS|0x5)
#define PMBUS_PARA_ERR2       (LOG_ERR_PMBUS|0x5)
#define PMBUS_PARA_ERR3       (LOG_ERR_PMBUS|0x5)
#define PMBUS_PARA_ERR4       (LOG_ERR_PMBUS|0x5)
#define PMBUS_PARA_ERR5       (LOG_ERR_PMBUS|0x5)
#define PMBUS_PARA_ERR6       (LOG_ERR_PMBUS|0x5)
#define PMBUS_PARA_ERR7       (LOG_ERR_PMBUS|0x5)
#define PMBUS_PARA_ERR8       (LOG_ERR_PMBUS|0x5)

#define CMBUS_PARA_ERR1         (LOG_ERR_CMBUS|0x1)
#define CMBUS_PARA_ERR2         (LOG_ERR_CMBUS|0x1)
#define CMBUS_PARA_ERR3         (LOG_ERR_CMBUS|0x1)
#define CMBUS_SENDNREADY_ERR    (LOG_ERR_CMBUS|0x2)
#define CMBUS_RECVFAIL_ERR      (LOG_ERR_CMBUS|0x3)

#define IPBUS_PARA_ERR1         (LOG_ERR_IPBUS|0x1)
#define IPBUS_PARA_ERR2         (LOG_ERR_IPBUS|0x1)
#define IPBUS_PARA_ERR3         (LOG_ERR_IPBUS|0x1)
#define IPBUS_PARA_ERR4         (LOG_ERR_IPBUS|0x1)
#define IPBUS_PARA_ERR5         (LOG_ERR_IPBUS|0x1)
#define IPBUS_PARA_ERR6         (LOG_ERR_IPBUS|0x1)

//pcie
#define PCIE_PARA_ERR1          (LOG_ERR_PCIE|0x1)
#define PCIE_PARA_ERR2          (LOG_ERR_PCIE|0x1)
#define PCIE_PARA_ERR3          (LOG_ERR_PCIE|0x1)
#define PCIE_PARA_ERR4          (LOG_ERR_PCIE|0x1)
#define PCIE_PARA_ERR5          (LOG_ERR_PCIE|0x1)
#define PCIE_PARA_ERR6          (LOG_ERR_PCIE|0x1)
#define PCIE_PARA_ERR7          (LOG_ERR_PCIE|0x1)
#define PCIE_PARA_ERR8          (LOG_ERR_PCIE|0x1)
#define PCIE_PARA_ERR9          (LOG_ERR_PCIE|0x1)

#define PCIE_SENDTIMEOUT_ERR    (LOG_ERR_PCIE|0x2)
#define PCIE_SENDFAIL_ERR       (LOG_ERR_PCIE|0x3)
#define PCIE_RECVTIMEOUT_ERR    (LOG_ERR_PCIE|0x4)
#define PCIE_RECVFAIL_ERR       (LOG_ERR_PCIE|0x5)

//spiflash
#define SF_PARA_ERR1             (LOG_ERR_SPIFLASH|0x1)
#define SF_PARA_ERR2             (LOG_ERR_SPIFLASH|0x1)
#define SF_PARA_ERR3             (LOG_ERR_SPIFLASH|0x1)
#define SF_PARA_ERR4             (LOG_ERR_SPIFLASH|0x1)
#define SF_PARA_ERR5             (LOG_ERR_SPIFLASH|0x1)
#define SF_PARA_ERR6             (LOG_ERR_SPIFLASH|0x1)
#define SF_PARA_ERR7             (LOG_ERR_SPIFLASH|0x1)
#define SF_PARA_ERR8             (LOG_ERR_SPIFLASH|0x1)

#define SF_READFAIL_ERR         (LOG_ERR_SPIFLASH|0x2)
#define SF_WRITEFAIL_ERR         (LOG_ERR_SPIFLASH|0x2)
#define SF_ERASEFAIL_ERR            (LOG_ERR_SPIFLASH|0x3)

//dfs
#define DFS_FULL_ERR                (LOG_ERR_DFS|0x1)
#define DFS_GETFAIL_ERR             (LOG_ERR_DFS|0x2)
#define DFS_PARA_ERR1               (LOG_ERR_DFS|0x3)
#define DFS_PARA_ERR2               (LOG_ERR_DFS|0x3)
#define DFS_PARA_ERR3               (LOG_ERR_DFS|0x3)
#define DFS_PARA_ERR4               (LOG_ERR_DFS|0x3)

//norflash
#define NOR_LOCKFAIL_ERR            (LOG_ERR_NOR|0x1)
#define NOR_UNLOCKFAIL_ERR            (LOG_ERR_NOR|0x1)
#define NOR_POLL_ERR1                   (LOG_ERR_NOR|0x3)
#define NOR_POLL_ERR2                   (LOG_ERR_NOR|0x3)
#define NOR_POLL_ERR3                   (LOG_ERR_NOR|0x3)
#define NOR_POLL_ERR4                   (LOG_ERR_NOR|0x3)
/*#define NOR_STATUSFAIL_ERR1             (LOG_ERR_NOR|0x4)*/
/*#define NOR_STATUSTIMEOUT_ERR           (LOG_ERR_NOR|0x5)*/
#define NOR_WRITEFAIL_ERR1              (LOG_ERR_NOR|0x3)
#define NOR_WRITEFAIL_ERR2              (LOG_ERR_NOR|0x3)
#define NOR_WRITEFAIL_ERR3              (LOG_ERR_NOR|0x3)
#define NOR_MEMCPY_ERR1                 (LOG_ERR_NOR|0x3)
#define NOR_MEMCPY_ERR2                 (LOG_ERR_NOR|0x3)
#define NOR_ERASEFAIL_ERR1              (LOG_ERR_NOR|0x3)
#define NOR_ERASEFAIL_ERR2              (LOG_ERR_NOR|0x3)
#define NOR_PARA_ERR1                   (LOG_ERR_NOR|0x3)

//hwinit
#define HWI_PARA_ERR1                   (LOG_ERR_HWI|0x1)
#define HWI_GETDFS_ERR                  (LOG_ERR_HWI|0x2)
#define HWI_REGDFS_ERR1                 (LOG_ERR_HWI|0x3)
#define HWI_REGDFS_ERR2                 (LOG_ERR_HWI|0x3)
#define HWI_REGDFS_ERR3                 (LOG_ERR_HWI|0x3)
#define HWI_INITFAIL_ERR1                (LOG_ERR_HWI|0x1)
#define HWI_INITFAIL_ERR2                (LOG_ERR_HWI|0x1)


//fs
#define LOG_ERR_SFFS        (LOG_ERR_FS|(0x1<<12))

//sffs error number //-42
#define SFFS_ERASE_HDRSS_ERR1            (LOG_ERR_SFFS|0x1)//-4
#define SFFS_ERASE_HDRSS_ERR2            (LOG_ERR_SFFS|0x2)//-4
#define SFFS_ERASE_HDRSS_ERR3            (LOG_ERR_SFFS|0x3)//-4
#define SFFS_WRITE_HDRSS_ERR1             (LOG_ERR_SFFS|0x4)//-10
#define SFFS_WRITE_HDRSS_ERR2             (LOG_ERR_SFFS|0x5)//-10
#define SFFS_WRITE_HDRSS_ERR3             (LOG_ERR_SFFS|0x6)//-10
#define SFFS_WRITE_HDRSS_ERR4             (LOG_ERR_SFFS|0x7)//-10
#define SFFS_WRITE_HDRSS_ERR5             (LOG_ERR_SFFS|0x8)//-10
//#define SFFS_HDRSS_FULL_ERR             -7

#define SFFS_NO_FREEHDRSS_ERR           (LOG_ERR_SFFS|0x36)//-5
//#define SFFS_NO_FREEHDRSS_ERR1           -31

#define SFFS_FILE_NUM_FULL_ERR          (LOG_ERR_SFFS|0x9)//-6

#define SFFS_PARA_ERR                   (LOG_ERR_SFFS|0x35)//-1
#define SFFS_PARA_ERR1                   (LOG_ERR_SFFS|0xa)//-1
#define SFFS_PARA_ERR2                   (LOG_ERR_SFFS|0xb)//-1
#define SFFS_PARA_ERR3                   (LOG_ERR_SFFS|0xc)//-1
#define SFFS_PARA_ERR4                   (LOG_ERR_SFFS|0xd)//-1
#define SFFS_PARA_ERR5                   (LOG_ERR_SFFS|0xe)//-1
#define SFFS_PARA_ERR6                   (LOG_ERR_SFFS|0x41)//-1
#define SFFS_PARA_ERR7                   (LOG_ERR_SFFS|0x42)//-1

#define SFFS_FILENAME_NOT_EXIST_ERR1     (LOG_ERR_SFFS|0xf)//-38
#define SFFS_FILENAME_NOT_EXIST_ERR2     (LOG_ERR_SFFS|0x10)//-39
#define SFFS_FILENAME_NOT_EXIST_ERR3     (LOG_ERR_SFFS|0x11)//-40
#define SFFS_FILENAME_NOT_EXIST_ERR4     (LOG_ERR_SFFS|0x12)//-41
#define SFFS_FILENAME_NOT_EXIST_ERR5     (LOG_ERR_SFFS|0x13)//-42

#define SFFS_FD_ERR                     (LOG_ERR_SFFS|0x14)//-9
#define SFFS_FD_ERR2                     (LOG_ERR_SFFS|0x40)//-9
//#define SFFS_WRONLY_ERR                 -15
#define SFFS_READMODE_ERROR             (LOG_ERR_SFFS|0x15)//-18
#define SFFS_WRITEMODE_ERR1              (LOG_ERR_SFFS|0x16)//-19
#define SFFS_WRITEMODE_ERR2              (LOG_ERR_SFFS|0x17)//-19
#define SFFS_FILE_REWRITE_ERR           (LOG_ERR_SFFS|0x18)//-12
#define SFFS_UNALIGN_WRITE_ERR          (LOG_ERR_SFFS|0x19)//-14
#define SFFS_DEL_ERR                    (LOG_ERR_SFFS|0x1a)//-21
#define SFFS_FILE_LOCKED_ERR            (LOG_ERR_SFFS|0x37)//-2
#define SFFS_FILE_NOT_COMMIT_ERR        (LOG_ERR_SFFS|0x1b)//-20
#define SFFS_FILE_SIZE_EXCED_ERR1        (LOG_ERR_SFFS|0x1c)//-16
#define SFFS_FILE_SIZE_EXCED_ERR2        (LOG_ERR_SFFS|0x1d)//-16
//#define SFFS_INIT_ERR                   -22
#define SFFS_NOT_NOT_MOUNTED            (LOG_ERR_SFFS|0x38)//-23
//#define SFFS_MODE_ERR                   -29

#define SFFS_NO_FREE_SE1                 (LOG_ERR_SFFS|0x1e)//-3
#define SFFS_NO_FREE_SE2                 (LOG_ERR_SFFS|0x1f)//-3
#define SFFS_NO_FREE_SE3                 (LOG_ERR_SFFS|0x20)//-3
#define SFFS_NO_FREE_SE4                 (LOG_ERR_SFFS|0x21)//-3
#define SFFS_NO_FREE_SE5                 (LOG_ERR_SFFS|0x22)//-3
#define SFFS_NO_FREE_SE6                 (LOG_ERR_SFFS|0x23)//-3
#define SFFS_NO_FREE_SE7                 (LOG_ERR_SFFS|0x24)//-3

#define SFFS_READ_DATA_ERR1              (LOG_ERR_SFFS|0x25)//-11
#define SFFS_READ_DATA_ERR2              (LOG_ERR_SFFS|0x26)//-11
#define SFFS_READ_DATA_ERR3              (LOG_ERR_SFFS|0x27)//-11
#define SFFS_READ_DATA_ERR4              (LOG_ERR_SFFS|0x28)//-11

#define SFFS_ERASE_DATA_ERR1             (LOG_ERR_SFFS|0x29)//-13
#define SFFS_ERASE_DATA_ERR2             (LOG_ERR_SFFS|0x2a)//-13

#define SFFS_WRITE_DATA_ERR1             (LOG_ERR_SFFS|0x2b)//-32
#define SFFS_WRITE_DATA_ERR2             (LOG_ERR_SFFS|0x2c)//-33
#define SFFS_WRITE_DATA_ERR3             (LOG_ERR_SFFS|0x2d)//-34
#define SFFS_WRITE_DATA_ERR4             (LOG_ERR_SFFS|0x2e)//-35
#define SFFS_WRITE_DATA_ERR5             (LOG_ERR_SFFS|0x2f)//-36
//#define SFFS_WRITE_DATA_ERR6             -37

#define SFFS_OFFSET_ERR                 (LOG_ERR_SFFS|0x30)//-30

#define SFFS_GLOBAL_ERR                 (LOG_ERR_SFFS|0x31)//-24 //global vars are inconsistent
#define SFFS_BUF_ERR                    (LOG_ERR_SFFS|0x39)//-25
#define SFFS_FNAME_DUPLICATE_ERR        (LOG_ERR_SFFS|0x32)//-26
#define SFFS_RENAME_FAIL1               (LOG_ERR_SFFS|0x33)//-27
#define SFFS_RENAME_FAIL2               (LOG_ERR_SFFS|0x34)//-28

