#include "../driver/device/pcie/pmbus.h"

void pmbus_demo()
{
    printf("test send\n");
    int8_t s1 = set_pm_slot();
    uint8_t buf1[1024] ,buf2[1024],buf3[1024];
    int i = 1;
    memset(buf1,0,sizeof(buf1));

    s1 = get_pm_slot();
    printf("slot %x %x\n",get_pm_slot(),s1);
    if (s1 == 0){
        i = 10;
        while(1){
            memset(buf2,0xab,sizeof(buf2));
            memset(buf3,0x0,sizeof(buf3));

            //send data form pmA to pmC
            printf("copy to fpga \n");
            copy_to_pmbuf(2,buf2,i);

            printf("start send\n");
            enable_pm_send(2,1);

            printf("check send\n");
            while(check_pm_send(2)<0)
                ;

            printf("0 1 send\n");
            printf("recv \n");

            //recv data from pmC to pmA 
            int16_t res ;
            while(check_pm_recv(2,1)<0)
                ;

            res = copy_from_pmbuf(2,1,buf3);
            if(res >=0){
                printf("0 2 1 recv %x bytes\n",res);
                for(int j =0 ; j<res;j++)
                    if (buf3[j] != 0x12)
                        printf("recv falt %x\n",buf3[j]);
                printf("got : %x %x\n",buf3[0],buf3[1]);
            }
        }
    }
    else if (s1 ==1 ){
    }
    else if (s1 == 2) {
        i = 10;
        while(1){
            memset(buf2,0x12,sizeof(buf2));
            memset(buf3,0x0,sizeof(buf3));
            int16_t res ;

            printf("check recv\n");
            //recv data from pmC to pmA 
            while(check_pm_recv(1,1)<0)
                ;
            res = copy_from_pmbuf(1,1,buf3);
            if (res >=0){
                printf("0 2 1 recv %x bytes\n",res);
                for(int j =0 ; j<res;j++)
                    if (buf3[j] != 0xab)
                        printf("recv falt %x\n",buf3[j]);
                printf("got len %d : %x %x\n",res,buf3[0],buf3[1]);
            }

            //send data to pmA from pmC
            printf("copy to fpga slot %d\n",get_pm_slot());
            copy_to_pmbuf(1,buf2,i);
            printf("start send\n");
            enable_pm_send(1,1);
            printf("check send\n");
            while(check_pm_send(1)<0)
                ;

        }
    }

}
