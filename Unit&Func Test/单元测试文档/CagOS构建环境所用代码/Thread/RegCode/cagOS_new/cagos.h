#ifndef __ExPORT_HEADER_H__ 
#define __ExPORT_HEADER_H__ 

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  OS configure code.
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
 * 01,Feb,2016,Ezio created
 *
 * DESCRIPTION
 * This file contains OS apis
 */

#define NULL 0 



extern int dfd;             //nor flash's dfs id             
extern int sffs_idx;        //sffs's id

#define OS_ERR_NONE 0 

typedef unsigned int UINT32;
typedef signed int INT32;
typedef unsigned short UINT16;
typedef signed short INT16;
typedef unsigned char UINT8;
typedef signed char INT8;
typedef unsigned long long UINT64;
typedef signed long long INT64;
typedef int BOOL;


/*
------------------------------------------------------------------------------------------------------------------------
*                                                    DELETE OPTIONS
------------------------------------------------------------------------------------------------------------------------
*/

//common
#define  OS_OPT_DEL_NO_PEND                  (UINT16)(0x0000u)
#define  OS_OPT_DEL_ALWAYS                   (UINT16)(0x0001u)

/*
------------------------------------------------------------------------------------------------------------------------
*                                                     PEND OPTIONS
------------------------------------------------------------------------------------------------------------------------
*/

//common
#define  OS_OPT_PEND_BLOCKING                (UINT16)(0x0000u)
#define  OS_OPT_PEND_NON_BLOCKING            (UINT16)(0x8000u)

/*
------------------------------------------------------------------------------------------------------------------------
*                                                     POST OPTIONS
------------------------------------------------------------------------------------------------------------------------
*/

//sem
#define  OS_OPT_POST_1                       (UINT16)(0x0000u)  /* Post message to highest priority task waiting      */

//sem and msgq
#define  OS_OPT_POST_ALL                     (UINT16)(0x0200u)  /* Broadcast message to ALL tasks waiting             */

// task_sem and mutex
#define  OS_OPT_POST_NONE                    (UINT16)(0x0000u)  /* no special option selected*/

//msgq
#define  OS_OPT_POST_FIFO                    (UINT16)(0x0000u)  /* Default is to post FIFO                            */
#define  OS_OPT_POST_LIFO                    (UINT16)(0x0010u)  /* Post to highest priority task waiting              */

//common
#define  OS_OPT_POST_NO_SCHED                (UINT16)(0x8000u)  /* Do not call the scheduler if this is selected      */


//os api

INT32 os_create_task(char *pname, void *ptask, void *parg, INT32 prio,UINT32 tq, UINT32 *err);
INT32 os_del_task(INT32 id, UINT32 *err);
INT32 os_resume_task(INT32 id, UINT32 *err);
INT32 os_suspend_task(INT32 id, UINT32 *err);
void os_delay_task(INT32 ticks, UINT32 *err);
void udelay(INT32 us);

UINT32 os_task_sem_pend(INT32 timeout, UINT16 opt,UINT32 *err);
UINT32 os_task_sem_post(INT32 id, UINT16 opt, UINT32 *err);
UINT32 os_task_sem_set(INT32 id,UINT32 cnt, UINT32 *err);

INT32 os_queue_create(char *qname,INT32 qsize, UINT32 *err);
INT32 os_queue_del(INT32 qnum,INT32 opt, UINT32 *err);
INT32 os_queue_post(INT32 qnum,char *buf,INT32 size,INT32 opt, UINT32 *err);
INT32 os_queue_get(INT32 qnum,char *buf,UINT16 *size,INT32 opt,INT32 timeout, UINT32 *err);

INT32 os_sem_create(char *sname, UINT32 cnt,UINT32 *err);
INT32 os_sem_del(INT32 sid,UINT16 opt,UINT32 *err);
UINT32 os_sem_pend(INT32 sid,UINT32 timeout,UINT16 opt,UINT32 *err);
UINT32 os_sem_post(INT32 sid, UINT16 opt, UINT32 *err);

INT32 os_mutex_create(char *mname,UINT32 *err);
INT32 os_mutex_del(INT32 mid, UINT16 opt,UINT32 *err);
INT32 os_mutex_pend(INT32 mid,UINT32 timeout,UINT16 opt,UINT32 *err);
INT32 os_mutex_post(INT32 mid,UINT16 opt,UINT32 *err);


//dfs api
#define DFS_DFD_FAULT   -1
#define DFS_PARA_FAULT  -2

//dfs device capability
#define DFS_WRITE       0
#define DFS_READ        1
#define DFS_ERASE       2
#define DFS_LENGTH      3
#define DFS_START       4
#define DFS_END         5
INT32 dfs_register(struct dfs_ops *dops);
void dfs_unregister(INT32 dfd);
INT32 dfs_write(INT32 dfd, UINT32 dst,UINT8 *src,UINT32 cnt);
INT32 dfs_read(INT32 dfd, UINT32 src, UINT8 *buf,UINT32 cnt);
INT32 dfs_erase(INT32 did,UINT32 no,UINT32 num);


//fs api
//sffs error number
#define SFFS_ERASE_HDRSS_ERR            -4
#define SFFS_WRITE_HDRSS_ERR            -10
#define SFFS_HDRSS_FULL_ERR             -7
#define SFFS_NO_FREEHDRSS_ERR           -5
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

//atomic operation

#define TRUE 1
#define FALSE 0

/* atomically clear a memory location */
UINT32 atomicClear
   (
   UINT32 *target /* memory location to clear */
   );

/* atomically set a memory location */
   //return the old value of target
UINT32 atomic32Set
   (
   UINT32 *target,    /* memory location to set */
   UINT32 value    /* set with this value */
   );

   /* Get the value of a shared memory atomically */
UINT32 atomic32Get
    (
    UINT32 * target    /* address of atom to be retrieved */
    );


    /* atomically add a value to a memory location */
UINT32 atomic32Add
   (
   UINT32 *target,    /* memory location to add to */
   UINT32 value    /* value to add */
   );

   /* atomically subtract a value from a memory location */
UINT32 atomic32Sub
   (
   UINT32 *target,    /* memory location to subtract from */
   UINT32 value    /* value to subtract */
   );

   /* atomically perform a bitwise AND on a memory location */
UINT32 atomic32And
   (
   UINT32 *target,    /* memory location to AND */
   UINT32 value    /* AND with this value */
   );

   /* atomically perform a bitwise OR on memory location */
UINT32 atomic32Or
   (
   UINT32 *target,    /* memory location to OR */
   UINT32 value    /* OR with this value */
   );

   /* atomically perform a bitwise XOR on a memory location */
UINT32 atomic32Xor
   (
   UINT32 *target,    /* memory location to XOR */
   UINT32 value    /* XOR with this value */
   );

   /* atomically compare-and-swap the contents of a memory location */
BOOL atomic32Cas
    (
    UINT32 *target,	        /* memory location to compare-and-swap */
    UINT32 ldValue,	/* compare to this value */
    UINT32 newValue	/* swap with this value */
    );

#endif


//fpga operation
//reg_num : fpga register's relative offset
//return : value of the given register
uint16_t fpga_read(uint16_t reg_num);
//value : write to fpga
void fpga_write(uint16_t reg_num,uint16_t value);

//pcie 

//使用 pcie dma 发送数据
//id : 0 - pcie 控制器 0 ， 1 - pcie 控制器 1
int32_t pcie_send(int32_t id, uint32_t src , uint32_t dst,uint32_t cnt);
//使用 pcie dma 接收数据
int32_t pcie_recv(int32_t id, uint32_t src , uint32_t dst,uint32_t cnt);

//在 cpu 和 fpga 之间直接接收数据
void pcie_read(uint8_t *dst, uint8_t *src, uint32_t len);
//在 cpu 和 fpga 之间直接发送数据
void pcie_write(uint8_t *dst, uint8_t *src, uint32_t len);


//cmbus & pmbus
#define FUNC_OK     -1      // ok

//pmbus
//插槽正确返回 FUNC_OK ， 其他返回i<0
int8_t set_pm_slot(void);
//返回插槽号,失败返回负数
int8_t get_pm_slot(void);

/*
bus_no: pmbus id， 1 - pmbus1 ，2 - pmbus2
buf_no: pm recv buffer type, 1 - data buffer , 2 - state buffer 
*/
//成功返回 len ， 失败返回 <0
int16_t copy_to_pmbuf(int8_t bus_no, uint8_t *data, int32_t len);
//成功返回 FUNC_OK ， 失败返回 <0
int16_t enable_pm_send(int bus_no,int buf_no);
//发送完成返回 FUNC_OK ，其它返回 <0
int16_t check_pm_send(int32_t bus_no);


//成功返回数据长度， 失败返回 <0
int16_t copy_from_pmbuf(int8_t bus_no, int8_t buf_no, uint8_t *buf);
//收到数据返回 FUNC_OK ，其它返回 <0
int16_t check_pm_recv(int32_t bus_no,int32_t buf_no);

//返回接收时间
int16_t get_pmrecv_time(int8_t bus_no, int8_t bufno);

//cmbus
/*
station: cmbus target station id (3~15)
*/
//成功返回 len ， 失败返回 <0
int16_t copy_to_cmbuf(uint8_t *data, int32_t len);
//成功返回 FUNC_OK ， 失败返回 <0
int16_t enable_cm_send(int station);
//发送完成返回 FUNC_OK ，其它返回 <0
int16_t check_cm_send(void);

//收到数据返回 1(数据)/2（应答） ，其它返回 <0
int16_t check_cm_recv(void);
//成功返回数据长度， 失败返回 <0
int16_t copy_from_cmbuf(uint8_t *buf);

//返回接收时间
int16_t get_cmrecv_time(void);
