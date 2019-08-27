/*
 * Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Define _STDBOOL_H here to avoid macro expansion of true and false.
 * If the future code requires macro true or false, remove this define
 * and undef true and false before U_BOOT_CMD. This define and comment
 * shall be removed if change to U_BOOT_CMD is made to take string
 * instead of stringifying it.
 */
#define _STDBOOL_H

#include <common.h>
#include <command.h>
#include <fs.h>

#define OP_INVALID	0
#define OP_NOT		1
#define OP_OR		2
#define OP_AND		3
#define OP_STR_EMPTY	4
#define OP_STR_NEMPTY	5
#define OP_STR_EQ	6
#define OP_STR_NEQ	7
#define OP_STR_LT	8
#define OP_STR_GT	9
#define OP_INT_EQ	10
#define OP_INT_NEQ	11
#define OP_INT_LT	12
#define OP_INT_LE	13
#define OP_INT_GT	14
#define OP_INT_GE	15
#define OP_FILE_EXISTS	16

const struct {
    int arg;
    const char *str;
    int op;
    int adv;
} op_adv[] = {
    {1, "=", OP_STR_EQ, 3},
    {1, "!=", OP_STR_NEQ, 3},
    {1, "<", OP_STR_LT, 3},
    {1, ">", OP_STR_GT, 3},
    {1, "-eq", OP_INT_EQ, 3},
    {1, "-ne", OP_INT_NEQ, 3},
    {1, "-lt", OP_INT_LT, 3},
    {1, "-le", OP_INT_LE, 3},
    {1, "-gt", OP_INT_GT, 3},
    {1, "-ge", OP_INT_GE, 3},
    {0, "!", OP_NOT, 1},
    {0, "-o", OP_OR, 1},
    {0, "-a", OP_AND, 1},
    {0, "-z", OP_STR_EMPTY, 2},
    {0, "-n", OP_STR_NEMPTY, 2},
    {0, "-e", OP_FILE_EXISTS, 4},
};

static int do_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char * const *ap;
    int i, op, left, adv, expr, last_expr, last_unop, last_binop;

    /* args? */
    if (argc < 3)
        return 1;

#ifdef DEBUG
    {
        debug("test(%d):", argc);
        left = 1;
        while (argv[left])
            debug(" '%s'", argv[left++]);
    }
#endif

    left = argc - 1;
    ap = argv + 1;
    expr = 0;
    last_unop = OP_INVALID;
    last_binop = OP_INVALID;
    last_expr = -1;
    while (left > 0) {
        for (i = 0; i < ARRAY_SIZE(op_adv); i++) {
            if (left <= op_adv[i].arg)
                continue;
            if (!strcmp(ap[op_adv[i].arg], op_adv[i].str)) {
                op = op_adv[i].op;
                adv = op_adv[i].adv;
                break;
            }
        }
        if (i == ARRAY_SIZE(op_adv)) {
            expr = 1;
            break;
        }
        if (left < adv) {
            expr = 1;
            break;
        }

        switch (op) {
        case OP_STR_EMPTY:
            expr = strlen(ap[1]) == 0 ? 1 : 0;
            break;
        case OP_STR_NEMPTY:
            expr = strlen(ap[1]) == 0 ? 0 : 1;
            break;
        case OP_STR_EQ:
            expr = strcmp(ap[0], ap[2]) == 0;
            break;
        case OP_STR_NEQ:
            expr = strcmp(ap[0], ap[2]) != 0;
            break;
        case OP_STR_LT:
            expr = strcmp(ap[0], ap[2]) < 0;
            break;
        case OP_STR_GT:
            expr = strcmp(ap[0], ap[2]) > 0;
            break;
        case OP_INT_EQ:
            expr = simple_strtol(ap[0], NULL, 10) ==
                   simple_strtol(ap[2], NULL, 10);
            break;
        case OP_INT_NEQ:
            expr = simple_strtol(ap[0], NULL, 10) !=
                   simple_strtol(ap[2], NULL, 10);
            break;
        case OP_INT_LT:
            expr = simple_strtol(ap[0], NULL, 10) <
                   simple_strtol(ap[2], NULL, 10);
            break;
        case OP_INT_LE:
            expr = simple_strtol(ap[0], NULL, 10) <=
                   simple_strtol(ap[2], NULL, 10);
            break;
        case OP_INT_GT:
            expr = simple_strtol(ap[0], NULL, 10) >
                   simple_strtol(ap[2], NULL, 10);
            break;
        case OP_INT_GE:
            expr = simple_strtol(ap[0], NULL, 10) >=
                   simple_strtol(ap[2], NULL, 10);
            break;
        case OP_FILE_EXISTS:
            expr = file_exists(ap[1], ap[2], ap[3], FS_TYPE_ANY);
            break;
        }

        switch (op) {
        case OP_OR:
            last_expr = expr;
            last_binop = OP_OR;
            break;
        case OP_AND:
            last_expr = expr;
            last_binop = OP_AND;
            break;
        case OP_NOT:
            if (last_unop == OP_NOT)
                last_unop = OP_INVALID;
            else
                last_unop = OP_NOT;
            break;
        default:
            if (last_unop == OP_NOT) {
                expr = !expr;
                last_unop = OP_INVALID;
            }

            if (last_binop == OP_OR)
                expr = last_expr || expr;
            else if (last_binop == OP_AND)
                expr = last_expr && expr;
            last_binop = OP_INVALID;

            break;
        }

        ap += adv;
        left -= adv;
    }

    expr = !expr;

    debug (": returns %d\n", expr);

    return expr;
}

U_BOOT_CMD(
    test,	CONFIG_SYS_MAXARGS,	1,	do_test,
    "minimal test like /bin/sh",
    "[args..]"
);

static int do_false(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    return 1;
}

U_BOOT_CMD(
    false,	CONFIG_SYS_MAXARGS,	1,	do_false,
    "do nothing, unsuccessfully",
    NULL
);

static int do_true(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    return 0;
}

U_BOOT_CMD(
    true,	CONFIG_SYS_MAXARGS,	1,	do_true,
    "do nothing, successfully",
    NULL
);

#define FLAG_ADDR 0xec000000

#define FLAG_A 1
#define FLAG_B 2

#define FPGA_FLAG_ADDR  0xec020000

#define FPGA_A_LEN_ADDR   0xeca40000
#define FPGA_B_LEN_ADDR   0xece40000

#define FPGA_A_DATA_ADDR    0xeca40004
#define FPGA_B_DATA_ADDR    0xece40004

#if 0
#define RTS0_A_ADDR 0xefc40000
#define RTS1_A_ADDR 0xefa40000

#define RTS0_B_ADDR 0xec560000
#define RTS1_B_ADDR 0xec760000

#else
#define RTS0_A_ADDR 0xec040000
#define RTS1_A_ADDR 0xec240000

#define RTS0_B_ADDR 0xec540000
#define RTS1_B_ADDR 0xec740000
#endif

#define RTS_CRC_C_ADDR
#define RTS_CRC_A_ADDR  0x11

#define RTS_LEN_C_ADDR
#define RTS_LEN_A_ADDR  0x19

#define RTS_HDR_SIZE 0x20000

#define RTS0_CR_LEN (0x1000000-0xeaf010)
#define RTS1_CR_LEN (0x5000000-0x5eaf010)

#define RTS0RAM     0x04eaf080
#define RTS1RAM     0x00eaf080

unsigned char HexToChar(unsigned char bChar)
{
    if((bChar>=0x30)&&(bChar<=0x39))
        bChar -= 0x30;
    else if((bChar>=0x41)&&(bChar<=0x46))//大写字母
        bChar -= 0x37;
    else if((bChar>=0x61)&&(bChar<=0x66))//小写字母
        bChar -= 0x57;
    else bChar = 0xff;
    return bChar;
}


unsigned char CharToHex(unsigned char bHex)
{
    if((bHex>=0)&&(bHex<=9))
        bHex += 0x30;
    else if((bHex>=10)&&(bHex<=15))//大写字母
        bHex += 0x37;
    else bHex = 0xff;
    return bHex;
}

unsigned int opow(unsigned int x,unsigned int y)
{
    int i;
    unsigned int ret=1;
    if(y == 0) {
        return 1;
    }

    for(i=0; i<y; i++) {
        ret = ret *x;

    }
    return ret;
}


unsigned int get_len(unsigned char *alen)
{
    int i;
    unsigned char str1[8];
    unsigned int lens=0;

    for(i=1; i<8; i++) {
        str1[i-1] = HexToChar(*(alen+i));
        lens+=str1[i-1]*opow(10,8-i-1);
    }
    return lens;
}

unsigned int get_crc(unsigned char *acrc)
{
    int i;
    unsigned int crcs=0;
    unsigned char str1[8];
    for(i=0; i<8; i++) {
        str1[i] = HexToChar(*(acrc+i));
        crcs|=str1[i]<<((8-i-1)*4);
    }
    return crcs;
}

static int do_boot_rts_c(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{

    //read A/B flag
    uint32_t *flag_addr=(uint32_t *)FLAG_ADDR;

    //cp bin to mem
    uint8_t *rts0_addr;
    uint8_t *rts1_addr;
    unsigned char *bin0len,*bin1len;
    uint32_t bin0lenc,bin1lenc;
    unsigned char *crc0,*crc1;
    uint32_t crc0c,crc1c;
    uint32_t crc00,crc11;

    uint32_t crcok;
    uint32_t boot_sec=0;

    if(*flag_addr == FLAG_A) {
        rts0_addr = RTS0_A_ADDR;
        rts1_addr = RTS1_A_ADDR;
        boot_sec=1;
    } else {
        rts0_addr = RTS0_B_ADDR;
        rts1_addr = RTS1_B_ADDR;
        boot_sec=2;
    }

    //get crc and length

    bin0len = rts0_addr+RTS_LEN_A_ADDR;
    bin1len = rts1_addr+RTS_LEN_A_ADDR;
    crc0    = rts0_addr+RTS_CRC_A_ADDR;
    crc1    = rts1_addr+RTS_CRC_A_ADDR;

    crc0c = get_crc(crc0);
    crc1c = get_crc(crc1);
    bin0lenc = get_len(bin0len);
    bin1lenc = get_len(bin1len);
    if (bin0lenc>0x300000 || bin1lenc >0x300000){
        printf("wrong len %x %x\n",bin0lenc,bin1lenc);
        goto runnext;
    }


    printf("read len %d %d \ncrc %x %x\n",bin0lenc,bin1lenc,crc0c,crc1c);


    //calcuate crc and compare

    crcok = 1;

    crc00 = crc32(0,rts0_addr+RTS_HDR_SIZE,bin0lenc);
    crc11 = crc32(0,rts1_addr+RTS_HDR_SIZE,bin1lenc);
#if 0
    crc00 = crc32(0,RTS0RAM+RTS_HDR_SIZE,bin0lenc);
    crc11 = crc32(0,RTS1RAM+RTS_HDR_SIZE,bin1lenc);
#endif
    if (crc00 != crc0c && crc11 != crc1c)
        crcok=-1;

    //if ok then boot else calcuate the other
    //ready to boot?
    if (crcok == 1) {
        //ok -> boot
    } else {
        //or check other one
runnext:        
        crcok =1;
        if(boot_sec == FLAG_A) {
            rts0_addr = RTS0_B_ADDR;
            rts1_addr = RTS1_B_ADDR;
            boot_sec=2;
        } else {
            rts0_addr = RTS0_A_ADDR;
            rts1_addr = RTS1_A_ADDR;
            boot_sec=1;
        }

        //get crc and length
        bin0len = rts0_addr+RTS_LEN_A_ADDR;
        bin1len = rts1_addr+RTS_LEN_A_ADDR;
        crc0    = rts0_addr+RTS_CRC_A_ADDR;
        crc1    = rts1_addr+RTS_CRC_A_ADDR;

        crc0c = get_crc(crc0);
        crc1c = get_crc(crc1);

        bin0lenc = get_len(bin0len);
        bin1lenc = get_len(bin1len);

        if (bin0lenc>0x300000 || bin1lenc >0x300000){
            printf("wrong len %x %x\n",bin0lenc,bin1lenc);
            goto fail;
        }
        printf("ed read len %d %d \ncrc %x %x\n",bin0lenc,bin1lenc,crc0c,crc1c);
        //calcuate crc and compare again

        crc00 = crc32(0,rts0_addr+RTS_HDR_SIZE,bin0lenc);
        crc11 = crc32(0,rts1_addr+RTS_HDR_SIZE,bin1lenc);

        if (crc0c != crc00 || crc1c != crc11) {
            //bad -> wait here
fail:            printf("no correct RTS ,so hang here !\n");
            while(1);
        }

    }
    printf("ok crc 0 %x %x 1 %x %x len 0 0x%x  1 0x%x \n",crc0,crc0c,crc1,crc1c,bin0lenc,bin1lenc);
    printf("boot from %d\n",boot_sec);

    memcpy(RTS0RAM,rts0_addr+RTS_HDR_SIZE,bin0lenc);
    memcpy(RTS1RAM,rts1_addr+RTS_HDR_SIZE,bin1lenc);
    return 0;
}

U_BOOT_CMD(
    ckrts,	CONFIG_SYS_MAXARGS,	1,	do_boot_rts_c,
    "boot rts with crc check,A/B copy",
    NULL
);

#include <common.h>
#include <command.h>
#include <asm/errno.h>
int strtou32(const char *str, unsigned int base, u32 *result)
{
    char *ep;

    *result = simple_strtoul(str, &ep, base);
    if (ep == str || *ep != '\0')
        return -EINVAL;

    return 0;
}
static int do_cksum(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
    if (argc <2) {
        printf("pa fai\n");
        return -1;
    }

    uint32_t start = 0;
    strtou32(argv[1],16,&start);
    uint32_t len = 0;
    strtou32(argv[2],10,&len);

    printf("start %x len %x\n",start,len);

    uint32_t crc = cksum_crc32(start,len);

    printf("crc %d %x\n",crc,crc);

    return 0;
}

U_BOOT_CMD(
    cksum,	CONFIG_SYS_MAXARGS,	1,	do_cksum,
    "boot rts with crc check,A/B copy",
    NULL
);

#define MASK_ADDRESSLINE                    (0x10000000U)
#define TEST_ADDRLINE_27_P                  (0x03FFFFF0U)   /* 地址线测试地址，第27根地址线取0  */
#define TEST_ADDRLINE_27_N                  (0x07FFFFF0U)   /* 地址线测试地址，第27根地址线取1  */
#define DIAG_ADDRLINE_ADDR                  (0x04000000U)   /* 地址线测试基地址                 */
#define CPU_TEST_DB_0                  (0x55U)            /* 地址线测试数据背景 */
#define CPU_TEST_DB_1                  (0xAAU)            /* 地址线测试数据背景 */
#define TEST_ADDRLINE_NUM              (26U)              /* 地址线只检前26根*/
#define MAX_LINE_SQ                    (2U)
#define DCACHE_EN                       (1)
#define DCACHE_DIS                      (0)
#define TRUE                            (1)
#define FALSE                           (0)

typedef enum {
    TESTLR = 0U,                        /*LR寄存器自检                      */
    TESTCR,                             /*CR寄存器自检                      */
    TESTGPR_INTERWORD,                  /*通用寄存器字间错误自检            */
    TESTGPR_INTRAWORD,                  /*通用寄存器字内错误自检            */
    TESTSPR_INTERWORD,                  /*特殊功能寄存器字间错误自检        */
    TESTSPR_INTRAWORD,                  /*特殊功能寄存器字内错误自检        */
    TESTFPR_INTERWORD,                  /*浮点型寄存器字内错误自检          */
    TESTFPR_INTRAWORD,                  /*浮点型寄存器字间错误自检          */
    TESTSPRG,                           /*SPRG寄存器自检                    */
    TESTFPSCR,                          /*FPSCR寄存器自检                   */
    TESTIU,                             /*指令单元自检                      */
    TESTCONBRANCHINST,                  /*条件分支跳转指令自检              */
    TESTUNCONBRANCHINST,                /*无条件分支跳转指令自检            */
    TESTFPU,                            /*浮点型指令集自检                  */
    TESTPIPELINE,                       /*流水线自检                        */
    TESTLSU,                            /*Load/Store单元自检                */
    TESTCRISPR,                         /*Critical SPR寄存器自检            */
    TESTINSTCACHE,                      /*指令Cache自检                     */
    TESTDATACACHE,                      /*Data Cache自检                    */
    MAX_CPU,                            /*自检序列号尾                      */
    TESTDATALINE,                       /*数据线自检                        */
    TESTADDRLINE,                       /*地址线自检                        */
    TESTSTACK                           /*栈空间溢出自检                    */
} CPUDiagSequence;



typedef uint8_t * puint8_t;

void cacheE500DisableD(void)
{
    flush_dcache();
    dcache_disable();

}

void cacheE500EnableD(void)
{
    invalidate_dcache();
    dcache_enable();
}

//void CpuTestDDRAddressLine(uint32_t *puiVectorTest)
static int do_ddrline_test(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
    uint32_t uiAddress = 0U;                                /*测试地址*/
    uint8_t  uiReadValue = 0U;                              /*测试数值*/
    uint32_t uiCount = 0U;                                  /*计数值*/
    uint32_t uiDCacheFlag = FALSE;                          /*Cache使能标识*/


    /* 执行顺序自检 */
    /* 如果此时data cache 使能，关闭，否则数据存在cache中，未通过地址线 */
    if(DCACHE_EN == dcache_status()) {
        /* 关闭data cache */
        printf("disable cache 1\n");
        cacheE500DisableD();
        uiDCacheFlag = TRUE;
    }

    if(DCACHE_DIS == dcache_status()) {
        /* 首先检测是否第27根地址线正常，其后测试基于第27根地址线 */
        uiAddress = TEST_ADDRLINE_27_P;
        /* 对测试地址写0x55 */
        *((puint8_t)uiAddress) = CPU_TEST_DB_0;
        /* 第27根地址线取反 */
        uiAddress = TEST_ADDRLINE_27_N;
        /* 对测试地址写0xAA */
        *((puint8_t)uiAddress) = CPU_TEST_DB_1;
        uiAddress = TEST_ADDRLINE_27_P;
        uiReadValue = *((puint8_t)uiAddress);

        /* 检查是否读出值正确，如果该地址线故障,此时读出值应为0xAA */
        if(CPU_TEST_DB_0 != uiReadValue) {
            printf("check err 0\n");
            return -1;
        }

        uiAddress = TEST_ADDRLINE_27_N;
        uiReadValue = *((puint8_t)uiAddress);

        /* 检查是否读出值正确，如果该地址线故障,此时读出值应为0xAA */
        if(CPU_TEST_DB_1 != uiReadValue) {
            printf("check err 1\n");
            return -1;
        }

        /*设置第27根地址线为1，检测其他地址线，这样避免改写ram 有效数据*/
        for(uiCount = 0U; uiCount < TEST_ADDRLINE_NUM; ++uiCount) {
            /*每次测试翻转一位，其他位除基准位外均为0*/
            uiAddress = DIAG_ADDRLINE_ADDR;
            /*对测试地址1写入0x55*/
            *((puint8_t)uiAddress) = CPU_TEST_DB_0;
            /*翻转测试地址线*/
            uiAddress += (1U<<uiCount);
            /*对测试地址2写入0xAA*/
            *((puint8_t)uiAddress) = CPU_TEST_DB_1;
            /* 恢复基地址 */
            uiAddress = DIAG_ADDRLINE_ADDR;
            /*从测试地址1读入数据*/
            uiReadValue = *((puint8_t)uiAddress);

            /*与写入值进行比较*/
            if(CPU_TEST_DB_0 != uiReadValue) {
                printf("check err 2\n");
                return -1;
            }

            uiAddress += (1U<<uiCount);
            /* 从测试地址2读入数据 */
            uiReadValue = *((puint8_t)uiAddress);

            /* 与写入值进行比较 */
            if(CPU_TEST_DB_1 != uiReadValue) {
                /* 不一致，记录错误 */
                printf("check err 3\n");
                return -1;
            }

        }
        /* 如果之前关闭data cache，恢复设置 */
        if(TRUE == uiDCacheFlag) {
            printf("enable cache 1\n");
            cacheE500EnableD();
        }
    }

    return 0;
}


U_BOOT_CMD(
    linetest,	CONFIG_SYS_MAXARGS,	1,	do_ddrline_test,
    "line test",
    NULL
);



#if 0
static int do_fpga_pcie(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
    pci_init();
    return 0;
}

U_BOOT_CMD(
    initfpga,	CONFIG_SYS_MAXARGS,	1,do_fpga_pcie,
    "line test",
    NULL
);
#endif

static int do_ddrline_test2(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
    uint32_t uiAddress = 0U;                                /*测试地址*/
    uint8_t  uiReadValue = 0U;                              /*测试数值*/
    uint32_t uiCount = 0U;                                  /*计数值*/
    uint32_t uiDCacheFlag = FALSE;                          /*Cache使能标识*/


    /* 执行顺序自检 */
    /* 如果此时data cache 使能，关闭，否则数据存在cache中，未通过地址线 */
    if(DCACHE_EN == dcache_status()) {
        /* 关闭data cache */
        printf("disable cache 1\n");
        cacheE500DisableD();
        uiDCacheFlag = TRUE;
    }

    if(DCACHE_DIS == dcache_status()) {
        /* 首先检测是否第27根地址线正常，其后测试基于第27根地址线 */
        uiAddress = TEST_ADDRLINE_27_P;
        /* 对测试地址写0x55 */
        *((puint8_t)uiAddress) = CPU_TEST_DB_0;
        /* 第27根地址线取反 */
        uiAddress = TEST_ADDRLINE_27_N;
        /* 对测试地址写0xAA */
        *((puint8_t)uiAddress) = CPU_TEST_DB_1;
        uiAddress = TEST_ADDRLINE_27_P;
        uiReadValue = *((puint8_t)uiAddress);

        /* 检查是否读出值正确，如果该地址线故障,此时读出值应为0xAA */
        if(CPU_TEST_DB_0 != uiReadValue) {
            printf("check err 0\n");
            while(1);
            return -1;
        }

        uiAddress = TEST_ADDRLINE_27_N;
        uiReadValue = *((puint8_t)uiAddress);

        /* 检查是否读出值正确，如果该地址线故障,此时读出值应为0xAA */
        if(CPU_TEST_DB_0 != uiReadValue) {
            printf("check err 1\n");
            while(1);
            return -1;
        }

        /*设置第27根地址线为1，检测其他地址线，这样避免改写ram 有效数据*/
        for(uiCount = 0U; uiCount < TEST_ADDRLINE_NUM; ++uiCount) {
            /*每次测试翻转一位，其他位除基准位外均为0*/
            uiAddress = DIAG_ADDRLINE_ADDR;
            /*对测试地址1写入0x55*/
            *((puint8_t)uiAddress) = CPU_TEST_DB_0;
            /*翻转测试地址线*/
            uiAddress += (1U<<uiCount);
            /*对测试地址2写入0xAA*/
            *((puint8_t)uiAddress) = CPU_TEST_DB_1;
            /* 恢复基地址 */
            uiAddress = DIAG_ADDRLINE_ADDR;
            /*从测试地址1读入数据*/
            uiReadValue = *((puint8_t)uiAddress);

            /*与写入值进行比较*/
            if(CPU_TEST_DB_0 != uiReadValue) {
                printf("check err 2\n");
            while(1);
                return -1;
            }

            uiAddress += (1U<<uiCount);
            /* 从测试地址2读入数据 */
            uiReadValue = *((puint8_t)uiAddress);

            /* 与写入值进行比较 */
            if(CPU_TEST_DB_1 != uiReadValue) {
                /* 不一致，记录错误 */
                printf("check err 3\n");
            while(1);
                return -1;
            }

        }
        /* 如果之前关闭data cache，恢复设置 */
        if(TRUE == uiDCacheFlag) {
            printf("enable cache 1\n");
            cacheE500EnableD();
        }
    }

    return 0;
}


U_BOOT_CMD(
    linetest2,	CONFIG_SYS_MAXARGS,	1,	do_ddrline_test2,
    "line test",
    NULL
);
