#include <drv_config.h>
#include <libc/string.h>
#include <libc/rand.h>
#include <libc/rand.h>
#include "../driver/device/dfs/dfs.h"
#include <logh.h>

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


#ifdef SFFS_FPGA_SPI
//#include "device/unionflash/unionflash_drv.h"
#include "device/spiflash/sf.h"
//extern ufs_t ufs_ops[];
#endif

extern UINT32 fs_get_err(int fsid);

/*used for nor flash*/
extern int nor_flash_errno;
struct dfs_ops s29gl512pOps;
struct flash_struct s29gl512pSt;
#ifdef SFFS_MULTI_DEVICE
struct dfs_ops s29gl512pOps2;
struct flash_struct s29gl512pSt2;
#endif
int dfd= -1;
int dfd2= -1;
int sffs_idx; /*nor flash*/
int sffs_idx2;

/*used for fpga flash*/
extern int sf2_flash_errno;
struct flash_struct fpgaflashSt;
struct dfs_ops fpgaflashOps;
struct dfs_ops fpgaflashOps2;
int sffs_idx3; /*fpga flash*/
int dfd3= -1;
/*int dfd4= -1; zl 16.7.29*/


struct flash_struct flash_struct[3] = {
#ifdef CONFIG_NOR_FLASH
    {
        .seSize         = SE_SIZE,
        .seNum          = SE_NUM,
        .ssSize         = SS_SIZE,
        .ssNum          = SS_NUM,
        .totalSize      = DEVICE_SIZE,
    },
#else
    {
        .seSize         = 0,
        .seNum          = 0,
        .ssSize         = 0,
        .ssNum          = 0,
        .totalSize      = 0,
    },
#endif
#ifdef SFFS_FPGA_SPI
    {
        .seSize         = SE_SIZE,
        .seNum          = SE_NUM*2,
        .ssSize         = SS_SIZE,
        .ssNum          = SS_NUM,
        .totalSize      = DEVICE_SIZE,
    },
#else
    {
        .seSize         = 0,
        .seNum          = 0,
        .ssSize         = 0,
        .ssNum          = 0,
        .totalSize      = 0,
    },
#endif
};




struct dfs_ops flash_ops[] = {
#ifdef CONFIG_NOR_FLASH
    {
        .dname      = (unsigned char *)"nor-flash\0",
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
        .dname      = (unsigned char *)"nor-flash-2\0",
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
#else
    {
        .dname      = 0,
        .write      = 0,
        .read       = 0,
        .erase      = 0,
        .base_addr  = 0,
        .size       = 0,
        .start      = 0,
        .end        = 0,
        .errno      = 0,
        .data       = 0,
        .eraseUnit  = 0,
    },
    {
        .dname      = 0,
        .write      = 0,
        .read       = 0,
        .erase      = 0,
        .base_addr  = 0,
        .size       = 0,
        .start      = 0,
        .end        = 0,
        .errno      = 0,
        .data       = 0,
        .eraseUnit  = 0,
    },
#endif
#if defined(FSL_BOARD) && defined(CONFIG_SPI)
    {
        .dname      = (unsigned char *)"sf-flash\0",
        .write      = sf_write,
        .read       = sf_read,
        .erase      = sf_erase,
        .base_addr  = 0,
        .size       = 0,
        .start      = 0,
        .end        = 0,
        .errno      = &sf_flash_errno,
        .data       = NULL,/*to define*/
        .eraseUnit  = SS_SIZE,
    },
#else
    {
        .dname      = 0,
        .write      = 0,
        .read       = 0,
        .erase      = 0,
        .base_addr  = 0,
        .size       = 0,
        .start      = 0,
        .end        = 0,
        .errno      = 0,
        .data       = 0,
        .eraseUnit  = 0,
    },
#endif
#ifdef SFFS_FPGA_SPI
    {
        .dname      = (unsigned char *)"spifpgaflash\0",//-0", //zl 16.7.29
        .write      = spiflash_write,
        .read       = spiflash_read,
        .erase      = spiflash_erase,
        .base_addr  = 0,
        .size       = 0x1000000,
        .start      = 0,
        .end        = 0x1000000,
        .errno      = &sf2_flash_errno,
        .data       = &flash_struct[1],
        .eraseUnit  = SS_SIZE,

    },
#if 0
    {
        .dname      = (unsigned char *)"spi-fpga-flash-0",
        .write      = lld_write,/*sf2_write,*/
        .read       = lld_read,/*sf2_read,*/
        .erase      = lld_erase,/*sf2_erase,*/
        .base_addr  = FLASH_BASE_ADDR,
        .size       = 0x800000,
        .start      = 0,
        .end        = 0x800000,
        .errno      = &sf2_flash_errno,
        .data       = &flash_struct[1],
        .eraseUnit  = SE_SIZE,

    },
    {
        .dname      = (unsigned char *)"spi-fpga-flash-1",
        .write      = lld_write,
        .read       = lld_read,
        .erase      = lld_erase,
        .base_addr  = FLASH_BASE_ADDR,
        .size       = 0x800000,
        .start      = 0x800000,
        .end        = 0x1000000,
        .errno      = &sf2_flash_errno,
        .data       = &flash_struct[1],
        .eraseUnit  = SE_SIZE,

    },
#endif
#else
    {
        .dname      = 0,
        .write      = 0,
        .read       = 0,
        .erase      = 0,
        .base_addr  = 0,
        .size       = 0,
        .start      = 0,
        .end        = 0,
        .errno      = 0,
        .data       = 0,
        .eraseUnit  = 0,
    },
    {
        .dname      = 0,
        .write      = 0,
        .read       = 0,
        .erase      = 0,
        .base_addr  = 0,
        .size       = 0,
        .start      = 0,
        .end        = 0,
        .errno      = 0,
        .data       = 0,
        .eraseUnit  = 0,
    },
#endif
};





#if defined(CONFIG_FS)

#ifdef SFFS_MULTI_DEVICE
extern struct sffs sffs_slot[];
#endif


#define NOR_FLASH_ID    0
#define SF_FLASH_ID     1
extern void sffs_slot_clear(void);

#define hwi_log(log)
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
    struct sffs temp;

    /*check flash parameters*/
    switch(flash) {
    case NOR_FLASH_ID:
        pageerase = 0;
        break;
    case SF_FLASH_ID:
        pageerase = 1;
        break;
    default:
        ret = -3;
        hwi_log(HWI_PARA_ERR1);
        break;
    }

    if (ret <0)
        return ret;

    /*for sffs*/
    if (fs == 0) {

        struct dfs_ops *dev ;

        dev = get_dfs(dfsid);
        if(dev == NULL) {
            printf("dfs id error(no dfs)\n");
            hwi_log(HWI_GETDFS_ERR);
            return -1;
        }

//zl 16.7.29
#if 0 //def SFFS_FPGA_SPI

        if ( !strncmp((char *)dev->dname,(char *)"spi-fpga-flash",strlen("spi-fpga-flash"))) {
            printf("fpga flash\n");
            temp.write      = union_write;
            temp.read       = union_read;
            temp.seek       = NULL;
            temp.erase      = union_erase;
            temp.magic_num  = SFFS_MAGIC;
            temp.ss_size    = ((struct flash_struct *)(dev->data))->ssSize;
            temp.se_size    = ((struct flash_struct *)(dev->data))->seSize;
            temp.device_size= dev->size;
            temp.flashId    = dfsid; /*uselsee for ufs*/
            temp.pageErase  = pageerase;
            temp.erase_unit = dev->eraseUnit;
        } else
#endif
        {
            temp.write      = dfs_write;
            temp.read       = dfs_read;
            temp.seek       = NULL;
            temp.erase      = dfs_erase;
            temp.magic_num  = SFFS_MAGIC;
            temp.ss_size    = ((struct flash_struct *)(dev->data))->ssSize;
            temp.se_size    = ((struct flash_struct *)(dev->data))->seSize;
            temp.device_size= dev->size;
            temp.flashId    = dfsid;
            temp.pageErase  = pageerase;
            temp.erase_unit = dev->eraseUnit;
        }
        strncpy((char *)temp.fsname,(char *)(dev->dname),(unsigned int )(strlen((char *)(dev->dname))+1));

#ifdef SFFS_MULTI_DEVICE
        if (finited ==0 ) {
            sffs_slot_clear();
            finited = 1;
        }

        cur = fs_mount2(temp,0);
        //printf("mount2 %x\n",cur);
#else
        cur = fs_mount(0);
#endif

        if (cur == -1) {
            printf("sffs init fail: %x\n",cur);
            hwi_log(HWI_INITFAIL_ERR1);
        }
        /*else
            printf("sffs mount %x\n",cur);*/

    } else {
        printf("unsupported filesystem\n");
        ret = -3;
        hwi_log(HWI_INITFAIL_ERR2);
    }
    return ret ;
}

#endif

#if defined(CONFIG_NOR_FLASH)
void nor_init(void)
{
    printf("hello nor flash\n");

    /*    unsigned int stat = 0;*/
    unsigned char wbuf[1024*1024];

    memset(wbuf,0xca,sizeof(wbuf));

    lld_InitCmd((UINT16 *)0xec000000);
#if 0
    unsigned char vers[100]= {};
    lld_GetVersion(vers);
    vers[99]='\0';
    stat = lld_ReadConfigRegCmd((UINT16 *)_CAG_CONFIG_FLASH_BASE_ADRS,0);
    printf("cfg 0x%x\n",stat);
    printf("vers %s\n",vers);
#endif


    s29gl512pSt = flash_struct[0];

    s29gl512pOps = flash_ops[0];

    dfd = dfs_register(&s29gl512pOps);
    if (dfd <0 ) {
        printf("nor flash ops register fail %d \n",dfd);
        hwi_log(HWI_REGDFS_ERR1);
    }
    printf("did %d\n",dfd);

    /**********************************************************/

#ifdef SFFS_MULTI_DEVICE
    s29gl512pSt2 = flash_struct[0];

    s29gl512pOps2 = flash_ops[1];

    dfd2 = dfs_register(&s29gl512pOps2);
    if (dfd2 <0 ) {
        printf("nor flash ops register fail %d \n",dfd2);
        hwi_log(HWI_REGDFS_ERR2);
    }
    printf("did %d\n",dfd2);
#endif
}
#endif

#if defined(CONFIG_FPGA_FLASH)
void fpga_flash_init(void)
{
    /*fpga flash device init*/
    printf("fpga spi flash init\n");

    /*register fpga flash to dfs*/

    fpgaflashSt  = flash_struct[1];

    fpgaflashOps = flash_ops[3];

    dfd3 = dfs_register(&fpgaflashOps);
    if(dfd3 <0) {
        printf("fpga flash ops register fail %d\n",dfd3);
        hwi_log(HWI_REGDFS_ERR3);
    }
    printf("did %d\n",dfd3);
    //ufs_register(dfs_write,dfs_read,dfs_erase,dfd3);

    //16.7.29
#if 0

    fpgaflashOps2 = flash_ops[4];

    dfd4 = dfs_register(&fpgaflashOps2);
    if(dfd4 <0) {
        printf("fpga flash ops register fail %d\n",dfd4);
    }
    printf("did %d\n",dfd4);
    ufs_register(dfs_write,dfs_read,dfs_erase,dfd4);
#endif
}
#endif


UINT32 log_index=0;

void record_log(UINT32 logn)
{
    *((UINT32 *)_CAG_CONFIG_SRAM_ADDR +log_index)=logn;
    log_index++;
    if(log_index == 0x10000)
        log_index=0;
}

UINT32 echo_log(UINT32 index)
{
    return *((UINT32 *)_CAG_CONFIG_SRAM_ADDR+index);
}
