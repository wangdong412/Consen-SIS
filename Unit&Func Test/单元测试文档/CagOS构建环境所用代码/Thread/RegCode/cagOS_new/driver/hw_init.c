#include <drv_config.h>
#include <libc/string.h>
#include <libc/rand.h>
#include <libc/rand.h>
#include "../driver/device/dfs/dfs.h"

#ifdef CONFIG_NOR_FLASH
#include "../driver/device/norflash/lld.h"
#endif

#ifdef CONFIG_FS
#include "../driver/fs/fs_api.h"
extern struct sffs sffs_slot[];
#endif

/* only used in FSL board*/
#ifdef FSL_BOARD

#ifdef CONFIG_IIC
#include "../driver/device/iic/iic.h"
#endif

#ifdef CONFIG_SPI
#include "../driver/device/spi/device_defs.h"
#include "../driver/device/spi/spi.h"
#endif

extern void fsl_i2c_init(int id, int speed, int slaveadd);
extern void sf_id(void);


int sf_flash_errno;

/*i2c - rtc */
struct dfs_ops rtcOps = {
        .write = rtc_write,
        .read = rtc_read,
        .size = 32,
        .start = 0,
        .end = 31,
        .base_addr = 0,
    };

/* spi - flash*/
struct flash_struct s25fl128sSt = {
    .seSize      =   BYTES_PER_SECTOR,
    .seNum       =   256,
    .ssSize     =   BYTES_PER_PAGE,
    .ssNum      =   256,
    .totalSize      =   16*1024*1024,
};

struct dfs_ops s25fl128sOps = {
    .dname          =   (UINT8 *)"s25fl128s\n",
    .write          =   sf_write,
    .read           =   sf_read,
    .erase          =   sf_erase,
    .base_addr      =   0,
    .start          =   0,
    .size           =   16*1024*1024,
    .end            =   0 + 16*1024*1024,
    .data           =   (struct flash_struct *)&s25fl128sSt,
};


int sfDid = 0;
int rtcId;

/*
 * rtc format
 * 0 sec(00-59)
 * 1 min(00-59)
 * 2 hour(00-23 / 01-12)  bit 6: 12, /24
 * 3 Days of the week(01-07)
 * 4 Dates (01-31)
 * 5 Months (01-12)
 * 6 Years (00-99)
 * 7 ctrl
 */

#if defined(CONFIG_IIC)
void iic_init(void)
{
    fsl_i2c_init(0,CONFIG_SYS_FSL_I2C_SPEED,CONFIG_SYS_FSL_I2C_SLAVE);

    memset(&rtcOps,0,sizeof(rtcOps));

    rtcId = dfs_register(&rtcOps);

    return ;
}
#endif

#if defined(CONFIG_SPI)
void spi_init(void)
{
    memset(&s25fl128sOps,0,sizeof(s25fl128sOps));
    memset(&s25fl128sSt,0,sizeof(s25fl128sSt));

    sfDid = dfs_register(&s25fl128sOps);
    if (sfDid<0) {
        printf("sf ops register fail %d\n",sfDid);
        return ;
    }

    sf_id();
    return ;
}
#endif
#endif



extern UINT32 fs_get_err(int fsid);
extern int nor_flash_errno;

struct dfs_ops s29gl512pOps;     
struct flash_struct s29gl512pSt; 

#ifdef SFFS_MULTI_DEVICE 
struct dfs_ops s29gl512pOps2;     
struct flash_struct s29gl512pSt2; 
#endif

int dfd= 0;
int dfd2= 0;

int sffs_idx;
int sffs_idx2;

struct flash_struct flash_struct[3] = {
    {
        .seSize         = SE_SIZE,
        .seNum          = SE_NUM,
        .ssSize         = SS_SIZE,
        .ssNum          = SS_NUM,
        .totalSize      = DEVICE_SIZE,
    },
};

struct dfs_ops flash_ops[3] ={
    {
        .dname      = (unsigned char *)"nor-flash",
        .write      = lld_write,
        .read       = lld_read,
        .erase      = lld_erase,
        .base_addr  = FLASH_BASE_ADDR,
        .size       = 21*1024*1024,
        .start      = 30*1024*1024,
        .end        = 30*1024*1024+21*1024*1024,
        .errno      = &nor_flash_errno,
        .data       = &flash_struct[0],
        .eraseUnit  = SE_SIZE,
    },
    {
        .dname      = (unsigned char *)"nor-flash-2",
        .write      = lld_write,
        .read       = lld_read,
        .erase      = lld_erase,
        .base_addr  = FLASH_BASE_ADDR,
        .size       = 20*1024*1024,
        .start      = 10*1024*1024,
        .end        = 20*1024*1024+10*1024*1024,
        .errno      = &nor_flash_errno,
        .data       = &flash_struct[0],
        .eraseUnit  = SE_SIZE,
    },
#ifdef FSL_BOARD
    {
            .dname      = (unsigned char *)"sf-flash",
            .write      = sf_write,
            .read       = sf_read,
            .erase      = sf_erase,
            .base_addr  = 0,
            .size       = 0,
            .start      = 0,
            .end        = 0,
            .errno      = &sf_flash_errno,
            .data       = NULL,
            .eraseUnit  = SS_SIZE,
    },
#else
#endif
};



#if defined(CONFIG_FS)

#ifdef SFFS_MULTI_DEVICE
extern struct sffs sffs_slot[];
#endif

#define NOR_FLASH_ID    0
#define SF_FLASH_ID     1
extern void sffs_slot_clear(void);

/*
 * flash    : 0 nor, 1 spi flash(fpga)
 * dfsid    : dfs_ops id
 * fs       : 0 sffs , 
 */
static uint8_t finited = 0;
int32_t fs_init(int32_t flash, int32_t dfsid, int32_t fs)
{
    int32_t ret = 0, cur;
    int32_t pageerase ;

    printf("dfsid %d\n",dfsid);

    //check flash parameters
    switch(flash){
        case NOR_FLASH_ID:
            pageerase = 0;
            printf("nor falsh\n");
            break;
        case SF_FLASH_ID:
            pageerase = 1;
            break;
        default:
            ret = -3;
            break;
    }

    if (ret <0)
        return ret;

    /*for sffs*/
    if (fs == 0) { 
        struct dfs_ops *dev ;
        
        dev = get_dfs(dfsid);
        if(dev == NULL){
            printf("dfs id error\n");
            return -1;
        }

        struct sffs temp = {
            .write      = dfs_write,
            .read       = dfs_read,
            .seek       = NULL,
            .erase      = dfs_erase,
            .magic_num  = SFFS_MAGIC,
            .ss_size    = ((struct flash_struct *)(dev->data))->ssSize,
            .se_size    = ((struct flash_struct *)(dev->data))->seSize,
            .device_size= dev->size,
            .flashId    = dfsid,
            .pageErase  = pageerase,
            .erase_unit = dev->eraseUnit,
        };
        strncpy((char *)temp.fsname,(char *)(dev->dname),sizeof(dev->dname));

#ifdef SFFS_MULTI_DEVICE
        if (finited ==0 ){
            sffs_slot_clear();
            finited = 1;
        }
        cur = fs_mount2(temp,0);
        printf("mount2 %x\n",cur);
#else
        cur = fs_mount(0);
#endif

        if (cur == -1)
            printf("sffs init fail: %x\n",cur);
        else
            printf("sffs mount %x\n",cur);

    }
    else {
        printf("unsupported filesystem\n");
        ret = -3;
    }
    return ret ;
}

#endif

#if defined(CONFIG_NOR_FLASH)
void nor_init(void)
{
    printf("hello nor flash\n");
    unsigned char vers[100]= {};
    unsigned int stat = 0;
    unsigned char wbuf[1024*1024];

    memset(wbuf,0xca,sizeof(wbuf));

    lld_InitCmd((UINT16 *)0xec000000);
    lld_GetVersion(vers);
    vers[99]='\0';
    stat = lld_ReadConfigRegCmd((UINT16 *)_CAG_CONFIG_FLASH_BASE_ADRS,0);
    printf("cfg 0x%x\n",stat);
    printf("vers %s\n",vers);


    s29gl512pSt = flash_struct[0];

    s29gl512pOps = flash_ops[0];

    dfd = dfs_register(&s29gl512pOps);
    if (dfd <0 ) {
        printf("nor flash ops register fail %d \n",dfd);
    }
    printf("did %d\n",dfd);

    /**********************************************************/

    s29gl512pSt2 = flash_struct[0];

    s29gl512pOps2 = flash_ops[1];

    dfd2 = dfs_register(&s29gl512pOps2);
    if (dfd2 <0 ) {
        printf("nor flash ops register fail %d \n",dfd2);
    }
    printf("did %d\n",dfd2);
}
#endif
