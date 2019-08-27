#include "../driver/device/dma/fsl_dma.h"


void dma_demo()
{
    dma_init();
    uint8_t buf1[1024*1024],buf2[1024*1024];
    int time1,time2;
    int ret;
    time1=get_ticks();
    memset(buf1,0xab,sizeof(buf1));
    time2=get_ticks();
    printf("memset 1M %d ticks \n",time2-time1);

    for(;;){
        time1=getTblus();
        ret = dma_memcpy(buf2,buf1,sizeof(buf1));
        time2=getTblus();
        if(ret <0)
            printf("dma fail\n");
        printf("dmacpy 1M %d ticks \n",time2-time1);

        time1=getTblus();
        ret = memcpy(buf2,buf1,sizeof(buf1));
        time2=getTblus();
        printf("memcpy 1M %d ticks \n",time2-time1);
    }
}
