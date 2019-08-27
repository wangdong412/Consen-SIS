#include "demo.h"
#include <libc/stdio.h>
#include <libc/cagboot.h>
#include <cagos.h>
#include "../driver/device/pcie/pcie.h"


int pcie_memcpy(void)
{
    int src,dst,cnt;
    
    src=FPGA_PCIE1_BAR0_BASE;
    dst=PPC_PCIE_DDR1_OFFSET;
    cnt=100;

    printf("exchange data between cpu and fpga pcie via memcpy\n");
    printf("from 0x%x to 0x%x cnt %d\n",src,dst,cnt);
    printf("dst %x\n",*(uint32_t *)PPC_PCIE_DDR1_OFFSET);
    printf("src %x\n",*(uint32_t *)FPGA_PCIE1_BAR0_BASE);
    memcpy((uint32_t*)dst,(uint32_t*)src,cnt);
    printf("dst %x\n",*(uint32_t *)PPC_PCIE_DDR1_OFFSET);
    printf("src %x\n",*(uint32_t *)FPGA_PCIE1_BAR0_BASE);
    return 0;
}

int pcie_demo()
{
    int res;

    printf("pcie send : send from cpu to fpga pcie\n");
    printf("1 src %x\n",*(uint32_t *)(PPC_PCIE_DDR1_OFFSET+0x080000));
    printf("dst %x\n",*(uint32_t *)FPGA_PCIE1_BAR0_BASE);
    res = pcie_send(1,0x080000,0x0,0x64);
    if (res <0)
        printf("send fail\n");
    printf("2 src %x\n",*(uint32_t *)(PPC_PCIE_DDR1_OFFSET+0x080000));
    printf("dst %x\n",*(uint32_t *)FPGA_PCIE1_BAR0_BASE);

    printf("pcie recv : recv from fpga pcie to cpu pcie\n");
    memset((uint32_t*)(PPC_PCIE_DDR1_OFFSET+0x1000),0x0,0x100);
    printf("src %x\n",*(uint32_t *)FPGA_PCIE1_BAR0_BASE);
    printf("dst %x\n",*(uint32_t *)(PPC_PCIE_DDR1_OFFSET+0x1000));
    res = pcie_recv(1,0x0,0x1000,0x64);
    if (res <0)
        printf("recv fail\n");
    printf("src %x\n",*(uint32_t *)FPGA_PCIE1_BAR0_BASE);
    printf("dst %x\n",*(uint32_t *)FPGA_PCIE1_BAR0_BASE);
    printf("pcie1 done\n");

    /*************************************************/


    printf("pcie send\n");
    printf("2 src %x\n",*(uint32_t *)(PPC_PCIE_DDR2_OFFSET+0x100000));
    printf("dst %x\n",*(uint32_t *)FPGA_PCIE2_BAR0_BASE);
    res = pcie_send(2,0x100000,0x0,0x64);
    if (res <0)
        printf("send fail\n");
    printf("2 src %x\n",*(uint32_t *)(PPC_PCIE_DDR2_OFFSET+0x100000));
    printf("dst %x\n",*(uint32_t *)FPGA_PCIE2_BAR0_BASE);

    printf("pcie recv\n");
    memset((uint32_t*)(PPC_PCIE_DDR2_OFFSET +0x1000),0x0,0x100);
    printf("src %x\n",*(uint32_t *)FPGA_PCIE2_BAR0_BASE);
    printf("dst %x\n",*(uint32_t *)(PPC_PCIE_DDR2_OFFSET+0x1000));
    res = pcie_recv(2,0x0,0x1000,0x64);
    if (res <0)
        printf("recv fail\n");
    printf("src %x\n",*(uint32_t *)FPGA_PCIE2_BAR0_BASE);
    printf("dst %x\n",*(uint32_t *)FPGA_PCIE2_BAR0_BASE);
    printf("pcie2 done\n");
    return 0;
}
