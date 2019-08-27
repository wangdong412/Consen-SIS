

#include "../driver/device/spiflash/sf.h"
#include "../cagos.h"

void fsf_demo()
{
#ifdef _CAG_CONFIG_AMP0
    return 
#endif
    printf("spiflash demo 1\n");
    uint8_t buf[4096]={0};
    int xxx;
    int ret;
    printf("spi 0\n");

//    printf("erase all\n");
//    spiflash_erase(0,2048);
//    spiflash_erase(2048,2048);


    memset(buf,0x15,4096);

    if ((ret = spiflash_erase(0x14000/4096,1)) != 0)
        printf("erase faile1 %d %x %d\n",0x14000/4096,0x14000,ret);

    if ((ret=spiflash_write(0x14000,buf,256)) != 256){
        printf("wr\n");
        printf("erase faile1 %d %x %d\n",0x14000/4096,0x14000,ret);
    }

        if ((ret = spiflash_read(0x14000,buf,256))==256) {
            for(int i =0;i<256;i++)
                if(buf[i]!=0x15){
                    printf("11errrrr %x %x %d %d\n",buf[i],14000,14000,ret);
                    break;
                }

        }

    memset(buf,0x15,4096);
    if ((ret=spiflash_write(0x14000+256,buf,256)) != 256){
        printf("wr2 \n");
        printf("erase faile1 %d %x %d\n",0x14000/4096,0x14000+256,ret);
    }

        if ((ret = spiflash_read(0x14000+256,buf,256))==256) {
            for(int i =0;i<256;i++)
                if(buf[i]!=0x15){
                    printf("11errrrr %x %x %d %d\n",buf[i],14000+256,14000/4096,ret);
                    break;
                }

        }
return ;

    int ii=0,adr=0x0;

    while(1){

        if ((ret = spiflash_erase(ii,1)) != 0)
            printf("erase faile1 %d %x %d\n",ii,adr,ret);

        memset(buf,0xdd,4096);

        if ((ret=spiflash_write(adr,buf,256)) != 256){
            printf("write failed %x %d %d\n",adr,ii,ret);
            if ((ret = spiflash_read(adr,buf,256))==256) {
                for(int i =0;i<256;i++)
                    if(buf[i]!=0x15){
                        printf("11errrrr %x %x %d %d\n",buf[i],adr,ii,ret);
                        break;
                    }

            }
        }

        memset(buf,0,4096);
        if ((ret = spiflash_read(adr,buf,256))==256) {
            for(int i =0;i<256;i++)
                if(buf[i]!=0xdd){
                    printf("errrrr %x %x %d %d\n",buf[i],adr,ii,ret);
                    break;
                }
                else{
                    printf("%x ",buf[i]);
                }

        }
        ii++;
        adr+=4096;
        if(adr == 0x1000000)
            return;
    }

#if 1
    if (spiflash_erase(0,1) != 0)
        printf("erase faile1\n");
    if (spiflash_erase(32,1) != 0)
        printf("erase faile2\n");

    if (spiflash_erase(33,1) != 0)
        printf("erase faile2\n");
    for(int x = 0;x<4096;x++)
        buf[x]=x%255;
#endif

    memset(buf,0xab,4096);

    if (spiflash_write(0x00000,buf,256) != 256){
        printf("write failed\n");
    }

    memset(buf,0,4096);
    if (spiflash_read(0x00000,buf,256)==256) {
        for(int i =0;i<256;i++)
            printf("%x ",buf[i]);
    }
    printf("\n");


    /***************/

    memset(buf,0xcd,4096);
    if (spiflash_write(0x20000,buf,255) != 255){
        printf("write failed\n");
    }
    memset(buf,0,4096);

    if (spiflash_read(0x20000,buf,255)==255) {
        for(int i =0;i<255;i++)
            printf("%x ",buf[i]);
    }
    printf("\n222");

    memset(buf,0xcd,4096);
    if (spiflash_write(0x21000,buf,255) != 255){
        printf("write failed\n");
    }
    memset(buf,0,4096);

    if (spiflash_read(0x21000,buf,255)==255) {
        for(int i =0;i<255;i++)
            printf("%x ",buf[i]);
    }
    printf("\n");

    memset(buf,0,4096);
    if (spiflash_read(0x00000,buf,256)==256) {
        for(int i =0;i<256;i++)
            printf("%x ",buf[i]);
    }
    printf("\n");
    return 0;



    if (spiflash_erase(0,1) != 0)
        printf("erase faile\n");

    /***********************/

    if (spiflash_erase(32,1) != 0)
        printf("erase faile\n");
    memset(buf,0xef,4096);
    if (spiflash_write(0x20000,buf,255) != 255){
        printf("write failed\n");
    }
    memset(buf,0,4096);

    if (spiflash_read(0x20000,buf,255)==255) {
        for(int i =0;i<254;i++)
            printf("%x ",buf[i]);
    }
    printf("\n");
    if (spiflash_erase(32,1) != 0)
        printf("erase faile\n");

    return ;


    printf("spi 1\n");
#if 0
    if ((xxx = spiflash_id())<0){
        printf("get id fail : %x\n",xxx);
    }
#endif

    if ((xxx=spiflash_read(8*1024*1024,buf,100))==100) {
        for(int i =0;i<100;i++)
            printf("%x ",buf[i]);
    }
    else
        printf("read %d\n",xxx);
    printf("\n");

    memset(buf,0xcd,2048);
    if ((xxx=spiflash_write(8*1024*1024,buf,100)) != 100){
        printf("write failed\n");
    }
    else
        printf("write %d\n",xxx);

    if ((xxx = spiflash_read(8*1024*1024,buf,100))==100) {
        for(int i =0;i<100;i++)
            printf("%x ",buf[i]);
    }
    else 
        printf("read2 %d\n",xxx);
    printf("\n");

    if ((xxx=spiflash_erase(2048,1)) != 0)
        printf("erase faile %d\n",xxx);
    else
        printf("erase ok2\n");

    if ((xxx=spiflash_read(8*1024*1024,buf,100))==100) {
        for(int i =0;i<100;i++)
            printf("%x ",buf[i]);
    }
    else
        printf("read3 %d\n",xxx);
    printf("\n");

}
