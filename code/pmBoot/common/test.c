#include <common.h>
#include <command.h>
#include <pci.h>
#include <asm/fsl_dma.h>
#include "../include/mtd/cfi_flash.h"

static int do_blink(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint32_t *pgpio = (uint32_t *)CAG_GPIO_BASE; 
    uint32_t a1 = *pgpio;
    *pgpio = a1|0x30000000;

    while(1){
        a1 = *(pgpio+2);
        *(pgpio+2) = a1|0x30000000;
        udelay(200000);
        a1 = *(pgpio+2);
        *(pgpio+2) = a1&(~0x30000000);
        udelay(200000);
        if(ctrlc()){
            return 0;
        }
    }

	return 0;
}

U_BOOT_CMD(blink, 3, 0, do_blink,
	"blink gpio",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");

static int do_sram(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint16_t *sram= (uint16_t *)CAG_SYS_SRAM_BASE1; 

    int32_t i;


    printf("read 0-7 byte\n");
    for(i=0;i<4;i++){
        printf("%x ",*(sram+i));
    }
    printf("\n");

    printf("read 512k+ 0-7 byte\n");
    for(i=0;i<4;i++){
        printf("%x ",*(sram+i +512*1024/2));
    }
    printf("\n");

    
    printf("write 0-7 byte\n");
    for(i=0;i<4;i++){
        *(sram+i) = 0x1234;
    }
    printf("write 512k+ 0-7 byte\n");
    for(i=0;i<4;i++){
        *(sram+i+512*1024/2) = 0x1234;
    }
    printf("\n");

    printf("read2 0-7 byte\n");
    for(i=0;i<4;i++){
        printf("%x ",*(sram+i));
    }
    printf("\n");

    printf("read2 512k+ 0-7 byte\n");
    for(i=0;i<4;i++){
        printf("%x ",*(sram+i +512*1024/2));
    }
    printf("\n");

	return 0;
}

U_BOOT_CMD(sram , 3, 0, do_sram,
	"sram read",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");

static int do_fpga(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    uint16_t *fpga = (uint16_t *)CAG_SYS_FPGA_BASE1;
    printf("read %x %x\n",*fpga,*(fpga+1));
    *(fpga+2)=0x25;
    *(fpga+3)=0x2;
    return 0;
}

U_BOOT_CMD(fpga, 3, 0, do_fpga,
	"fpga blink",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");

int16_t yyy;
static int do_fpga_read(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    uint16_t *fpga = (uint16_t *)CAG_SYS_FPGA_BASE1;
    uint16_t xx;
    while(1){
        yyy=*fpga;
    }
    return 0;
}

U_BOOT_CMD(fpga_read, 3, 0, do_fpga_read,
	"fpga blink",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");

static int do_fpga_write(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    uint16_t *fpga = (uint16_t *)(CAG_SYS_FPGA_BASE1+0x6c);
    uint16_t xx;
    yyy = 0xffff;
    while(1){
        *fpga = yyy;
    }
    return 0;
}

U_BOOT_CMD(fpga_write, 3, 0, do_fpga_write,
	"fpga blink",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");


static int do_pcie1(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    unsigned int *pcie1 = 0x80200000;
    uint32_t pl[1024]={0};
    int i=0,j;

    if (argc <2)
        return 0;

    j=simple_strtoul(argv[1],NULL,10);

    for(i=0;i<j;i++) {
        pl[i] = *(pcie1+i);
    }

    for(i=0;i<j;i++){
        printf("%x \n",pl[i]);
    }
    printf("\n");

    return 0;
}

U_BOOT_CMD(pcie1, 3, 0, do_pcie1,
	"pcie 1 test",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");

static int do_pcie2(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint32_t *pcie1 = 0x80000000;
    uint32_t pl[1024]={0};
    int i=0,j;

    //pcie dma operation
    uint32_t *dma_base = 0x80405000;
    uint32_t *ocm_base = 0x80404000;
    ptrdiff_t stat = 0, ver = 1, ctrl = 4, ndp = 8;
    uint32_t desc[7]={0};
    uint32_t temp;

    if (argc < 4){
        printf("argc=%d\n",argc);
        return 0;
    }

    desc[0]=simple_strtoul(argv[1],NULL,10); //src
    desc[2]=simple_strtoul(argv[2],NULL,10); //dst
    desc[6]=simple_strtoul(argv[3],NULL,10); //cnt

    printf("version %x\n",in_le32(dma_base + ver)); //pcie is little-endian

    //test 1 
    //construct desc

 //   desc[0]=0x200000;
 //   desc[2]=0x200800;
 //   desc[6]=0x20;
 
    desc[4] = 0x4100;
    desc[7] = 0x88<<24;;

    if (in_le32(dma_base+stat) & 0x10){
        printf("dma busy\n");
        return -1;
    }


    //write desc to dma ram 
    for(i=0;i<=7;i++){
        out_le32(ocm_base+i,desc[i]);
    }
    memset(desc,0,sizeof(desc));
    for(i=0;i<7;i++){
        printf("%x\n",desc[i]);
    }
    printf("\n");
    for(i=0;i<=7;i++){
        desc[i] = in_le32(ocm_base+i);
    }
    for(i=0;i<7;i++){
        printf("%x\n",desc[i]);
    }

    //clear run
    out_le32(dma_base + ctrl , in_le32(dma_base + ctrl)&~0x20);

    //clear status
    out_le32(dma_base + stat,0xff);

    //and ndp
    out_le32(dma_base + ndp , 0x5000);

    //configure ctrl reg and start RUN
    temp = in_le32(dma_base +ctrl);
    temp |= 0x20;
    temp |= 0x40;
//    temp &= ~0x10;
    out_le32(dma_base + ctrl , temp);



    //wait finish and check err/status
    while(1){
        if(in_le32(dma_base + stat) ==  0xc ){
            printf("dma done %d %x\n",i,in_le32(dma_base+stat));
            break;
        }
        if(ctrlc())
            break;
    }


    printf("dma AA %d %x\n",i,in_le32(dma_base+stat));
//    //clear status
//    out_le32(dma_base + stat,0xff);

    //clear run
    out_le32(dma_base + ctrl, in_le32(dma_base + ctrl)&~0x20);
    printf("dma bb %d %x\n",i,in_le32(dma_base+stat));

//    for(i=0;i<0x20/4;i++){
  //      printf("%x\n",in_le32(desc[2]+ i*4));
   // }

    return 0;
}

U_BOOT_CMD(pcie2, 4, 0, do_pcie2,
	"pcie 1 test",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");

static void do_dma1(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]) 
{
    uint32_t src,dst,cnt;
    int32_t res = -1;

    if (argc < 4){
        printf("argc=%d\n",argc);
        return 0;
    }

    src=simple_strtoul(argv[1],NULL,10);
    dst=simple_strtoul(argv[2],NULL,10);
    cnt=simple_strtoul(argv[3],NULL,10);

    dma_init();
    res = dmacpy(dst,src,cnt);
    if(res <0){
        printf("dma failed\n");
    }
    return res;
}

U_BOOT_CMD(dma, 4, 0, do_dma1,
	"pcie 1 test",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");


#define FPGA_PCIE_BAR_BASE      0x80000000
#define FPGA_PCIE_BAR0_OFFSET   0x400000
#define FPGA_PCIE_BAR2_OFFSET   0x800000
#define FPGA_PCIE_BAR0_BASE     (FPGA_PCIE_BAR_BASE + FPGA_PCIE_BAR0_OFFSET)
#define FPGA_PCIE_BAR2_BASE     (FPGA_PCIE_BAR_BASE + FPGA_PCIE_BAR2_OFFSET)

#define FPGA_DMA_OFFSET         0x4000
#define FPGA_DMA_RAM_OFFSET     0x5000
#define FPGA_DMA_ADDR           (FPGA_PCIE_BAR2_BASE + FPGA_DMA_OFFSET)
#define FPGA_DMA_RAM_ADDR       (FPGA_PCIE_BAR2_BASE + FPGA_DMA_RAM_OFFSET)
#define FPGA_PCIE_BASE          FPGA_PCIE_BAR0_BASE

#define PPC_PCIE_DDR0_OFFSET    0x1fa00000

#define FPGA_PCIE_BAR0_SIZE     0x2000
#define PPC_PCIE_BAR0_SIZE      0x200000

uint32_t fpga_dma_descriptor[7]={0};

static void construct_dma_desc(uint32_t source, uint32_t dst, uint16_t cnt,uint32_t nd)
{
    uint32_t *desc = &fpga_dma_descriptor[0];
    desc[0] = source;
    desc[2] = dst;
    //desc[4] = nd;
    desc[6] = (uint32_t)cnt;
    desc[7] = 0x88<<24;//channel setting 1?
}

static int32_t make_w_desc(uint32_t source, uint32_t dst, uint16_t cnt,uint32_t nd)
{
    if ( (source+cnt)>PPC_PCIE_BAR0_SIZE || (dst+cnt)>FPGA_PCIE_BAR0_SIZE || cnt ==0) {
        return -1;
    }
    construct_dma_desc(source+PPC_PCIE_DDR0_OFFSET,dst+FPGA_PCIE_BAR0_OFFSET,cnt,nd);

}

static void make_r_desc(uint32_t source, uint32_t dst, uint16_t cnt,uint32_t nd)
{
    if ( (dst+cnt)>PPC_PCIE_BAR0_SIZE || (source +cnt)>FPGA_PCIE_BAR0_SIZE || cnt == 0) {
        return -1;
    }
    construct_dma_desc(source+FPGA_PCIE_BAR0_OFFSET,dst+PPC_PCIE_DDR0_OFFSET,cnt,nd);
}

static void start_dma_transmit(int32_t n)
{
    uint32_t i,j,temp;
    uint32_t *ocm_base = FPGA_DMA_RAM_ADDR;
    uint32_t *dma_base = FPGA_DMA_ADDR;
    uint32_t *desc = &fpga_dma_descriptor[0];
    ptrdiff_t stat = 0,ctrl = 4, ndp = 8;

    for(i=0;i<=7;i++){
        out_le32(ocm_base+i,desc[i]);
    }
    //clear run
    out_le32(dma_base + ctrl , in_le32(dma_base + ctrl)&~0x20);

    //clear status
    out_le32(dma_base + stat,0xff);

    //and ndp
    out_le32(dma_base + ndp , FPGA_DMA_RAM_OFFSET);

    //configure ctrl reg and start RUN
    temp = in_le32(dma_base +ctrl);
    temp |= 0x20;
    temp |= 0x40;
    //start
    out_le32(dma_base + ctrl , temp);
}

static uint32_t wait_dma_done(void)
{
    uint32_t res = -1;
    ptrdiff_t  ctrl = 4,stat = 0;
    uint32_t *dma_base = FPGA_DMA_ADDR;

    //wait finish and check err/status
    while(in_le32(dma_base + stat) & 0x10){
        if(ctrlc())
            break;
    }

    //get stat
    res = in_le32(dma_base + stat);

    //clear status
    out_le32(dma_base + stat,0xff);

    //clear run
    out_le32(dma_base + ctrl, in_le32(dma_base + ctrl)&~0x20);
    return res;
}


static int32_t pcie_dma_trans(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint32_t res;

    uint32_t src,dst,cnt;

    if (argc < 5){
        printf("parameter err,example: pd r|w src dst\n");
        return 0;
    }

    src=simple_strtoul(argv[2],NULL,10); //src
    dst=simple_strtoul(argv[3],NULL,10); //dst
    cnt=simple_strtoul(argv[4],NULL,10); //cnt

    construct_dma_desc(src,dst,cnt,0);
    if(strncmp(argv[1],"r",1) == 0) {
        make_r_desc(src,dst,cnt,0);
    }
    else if(strncmp(argv[1],"w",1) == 0) {
        make_w_desc(src,dst,cnt,0);
    }
    else {
        printf("parameter err\n");
        return 0;
    }
    start_dma_transmit(0); 
    res = wait_dma_done();
    printf("stat %x\n",res);
    return 0;
}

U_BOOT_CMD(pd1, 5, 0, pcie_dma_trans,
	"pcie dma transmit",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");

static int pcie_write(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int src,dst,cnt;
    int i=0;
    
    dst = 0x80402000;
    src = 0x1fa00000;
    cnt = 0x2000;

    printf("from 0x%x to 0x%x cnt %d\n",src,dst,cnt);
    for( i=0;i<cnt;i++){
        *((unsigned char *)src+i)=rand()%0xff;
    }
    printf("%x %x %x\n",*((unsigned char *)src+10),*((unsigned char *)src+1000),*((unsigned char *)src+2000));

    do {
        memcpy(dst,src,cnt);
        if(ctrlc())
            break;
    }while(1);

    return 0;
}

U_BOOT_CMD(pcie_write, 1, 0, pcie_write,
	"pcie memcpy",
	"pmp src dst\n"
	"\n"
	"");

extern unsigned int rand(void);
static int pcie_read(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int src,dst,cnt;
    int i=0;
    
    src = 0x80402000;
    dst = 0x1fa00000;
    cnt = 0x2000;

    printf("from 0x%x to 0x%x cnt %d\n",src,dst,cnt);
    do {
        memcpy(dst,src,cnt);
        if(ctrlc())
            break;
    }while(1);

    return 0;
}

U_BOOT_CMD(pcie_read, 1, 0, pcie_read,
	"pcie memcpy",
	"pmp src dst\n"
	"\n"
	"");

static int pcie_memcpy(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int src,dst,cnt;
    
    if (argc < 4){
        printf("parameter err,example: pd r|w src dst\n");
        return 0;
    }

    src=simple_strtoul(argv[1],NULL,10); //src
    dst=simple_strtoul(argv[2],NULL,10); //dst
    cnt=simple_strtoul(argv[3],NULL,10); //cnt

    printf("from 0x%x to 0x%x cnt %d\n",src,dst,cnt);
    memcpy(dst,src,cnt);

    return 0;
}

U_BOOT_CMD(pmp, 4, 0, pcie_memcpy,
	"pcie memcpy",
	"pmp src dst\n"
	"\n"
	"");

static int do_pcie4(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint32_t pl[1024]={0};
    int i=0,j;

    //pcie dma operation
    uint32_t *dma_base = 0xa0804000;
    uint32_t *ocm_base = 0xa0805000;
    ptrdiff_t stat = 0, ver = 1, ctrl = 4, ndp = 8;
    uint32_t desc[7]={0};
    uint32_t temp;

    if (argc < 4){
        printf("argc=%d\n",argc);
        return 0;
    }

    desc[0]=simple_strtoul(argv[1],NULL,10); //src
    desc[2]=simple_strtoul(argv[2],NULL,10); //dst
    desc[6]=simple_strtoul(argv[3],NULL,10); //cnt

    printf("version %x\n",in_le32(dma_base + ver)); //pcie is little-endian

    //test 1 
    //construct desc

 //   desc[0]=0x200000;
 //   desc[2]=0x200800;
 //   desc[6]=0x20;
 
    desc[4] = 0x4100;
    desc[7] = 0x88<<24;;

    if (in_le32(dma_base+stat) & 0x10){
        printf("dma busy\n");
        return -1;
    }


    //write desc to dma ram 
    for(i=0;i<=7;i++){
        out_le32(ocm_base+i,desc[i]);
    }
    memset(desc,0,sizeof(desc));
    for(i=0;i<7;i++){
        printf("%x\n",desc[i]);
    }
    printf("\n");
    for(i=0;i<=7;i++){
        desc[i] = in_le32(ocm_base+i);
    }
    for(i=0;i<7;i++){
        printf("%x\n",desc[i]);
    }

    //clear run
    out_le32(dma_base + ctrl , in_le32(dma_base + ctrl)&~0x20);

    //clear status
    out_le32(dma_base + stat,0xff);

    //and ndp
    out_le32(dma_base + ndp , 0x5000);

    //configure ctrl reg and start RUN
    temp = in_le32(dma_base +ctrl);
    temp |= 0x20;
    temp |= 0x40;
//    temp &= ~0x10;
    out_le32(dma_base + ctrl , temp);

    //wait finish and check err/status
    while(in_le32(dma_base + stat) & 0x10){
        //if(!(in_le32(dma_base + stat) & 0x10) ){
        //    printf("dma done %d %x\n",i,in_le32(dma_base+stat));
        //    break;
        //}
        if(ctrlc())
            break;
    }


    printf("dma AA %d %x\n",i,in_le32(dma_base+stat));
//    //clear status
//    out_le32(dma_base + stat,0xff);

    //clear run
    out_le32(dma_base + ctrl, in_le32(dma_base + ctrl)&~0x20);
    printf("dma bb %d %x\n",i,in_le32(dma_base+stat));

//    for(i=0;i<0x20/4;i++){
  //      printf("%x\n",in_le32(desc[2]+ i*4));
    //}

    return 0;
}

U_BOOT_CMD(pcie4, 4, 0, do_pcie4,
	"pcie dma  test",
	"\n"
	"\n"
	"");


static int do_sram_r(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint16_t *sram= (uint16_t *)CAG_SYS_SRAM_BASE1; 

    int32_t i;
    uint16_t rr;

    for(i=0;i<1024*1024/2;i++){
        rr = *(sram +i);
        if (rr == 0x12345678)
            break;
    }
    printf("read donw %d\n",rr);

	return 0;
}

U_BOOT_CMD(read_sram , 3, 0, do_sram_r,
	"sram read long",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");

static int do_sram_w(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint16_t *sram= (uint16_t *)CAG_SYS_SRAM_BASE1; 

    int32_t i;
    for(i=0;i<1024*1024/2;i++){
        *(sram +i)=0x55aa;
    }


	return 0;
}

U_BOOT_CMD(write_sram, 3, 0, do_sram_w,
	"sram write long",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");

#include <flash.h>                                            
#include <mtd/cfi_flash.h>                                    
extern flash_info_t flash_info[];   

static int do_flash_r(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint16_t *sram= (uint16_t *)0xec000000; 

    int32_t i;
    uint16_t rr;

    for(i=0;i<1024*1024;i++){
        rr = *(sram +i);
        if (rr == 0x12345678)
            break;
    }
    printf("read donw %d\n",rr);


	return 0;
}

U_BOOT_CMD(flash_read, 3, 0, do_flash_r,
	"flash read long",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");

static int do_flash_w(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint16_t *sram= (uint16_t *)0xec000000; 

    int32_t i,ret;

    i = flash_sect_erase(0xec000000,0xec200000-1);
    if(i != 0){
        printf("erase failed\n");
        return 1;
    }

    printf("erase done\n");

    mdelay(5000);



    uint8_t *ff = 0x10000000;
    memset(ff,0x5a,1024*1024);

    for(i=0;i<1024;i++){
        ret = flash_write(ff,i*1024 + 0xec000000,1024);
        if(ret != 0)
            printf("flash write failed. addr 0x%x\n",i*1024+0xec000000);
    }

	return 0;
}

U_BOOT_CMD(flash_write, 3, 0, do_flash_w,
	"flash write long",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");

static int do_loopf_w(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    uint16_t *sram= (uint16_t *)0xec000000; 

    int32_t i,ret;

    uint16_t *ff = 0xeeaaaaaa ;
    uint16_t *yy = 0xed555555 ;
    uint16_t a= 0x5555;
    uint16_t b= 0xaaaa;


    for(i=0;;i++){
        flash_write(&a,0xeeaaaaaa,2);
        flash_write(&b,0xeeaaaaaa,2);
//        flash_write(&b,0xed555555,2);
//        ret = flash_write(ff,i*1024 + 0xec000000,1024);
//        if(ret != 0)
//            printf("flash write failed. addr 0x%x\n",i*1024+0xec000000);
//        *ff = 0x5555;
//        *yy = 0xaaaa;        
    }

	return 0;
}

U_BOOT_CMD(flash_loop_w, 3, 0, do_loopf_w,
	"flash write long",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");


static int do_loopf_r(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int32_t i,ret;

    uint16_t *ff = 0xeeaaaaaa ;
    uint16_t *yy = 0xed555555 ;
    uint16_t t;


    for(i=0;;i++){
        t = *ff ;
    }

	return 0;
}

U_BOOT_CMD(flash_loop_r, 1, 0, do_loopf_r,
	"flash write loop",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");



uint8_t *addrt = 0x10000000;
uint8_t *addrt2 = 0x10000000;
static int do_mem_loopr(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int8_t i,ret;

    i=0;
    int32_t j = 0;

    while(1){
        *(addrt+i) = 0x55;
        if(i == 0x100000){
            if (j == 0x1000000){
                printf("%x %x\n",*(addrt+i-1),*(addrt2+i-1));
                j=0;
            }
            i=0;
            j++;
        }
        *(addrt2+i) = 0xaa;
        if(i == 0x100000){
            if (j == 0x1000000){
                printf("%x %x\n",*(addrt+i-1),*(addrt2+i-1));
                j=0;
            }
            i=0;
            j++;
        }
    }

	return 0;
}

U_BOOT_CMD(mem_loop, 1, 0, do_mem_loopr,
	"mem loop",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");


static int do_mem_loopwrite(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int8_t i,ret;

    i=0;
    int32_t j = 0;

    while(1){
        memset(addrt,0xac,1024*1024);
    }

	return 0;
}

U_BOOT_CMD(mem_write, 1, 0, do_mem_loopwrite,
	"mem loop",
	"<input|set|clear|toggle> <pin>\n"
	"    - input/set/clear/toggle the specified pin\n"
	"gpio status [<bank> | <pin>]");


