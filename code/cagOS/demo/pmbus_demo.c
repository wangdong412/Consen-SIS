#include "../driver/bus/pmbus.h"

void pmbus_demo()
{
    printf("test send\n");
    int8_t s1 = set_pm_slot();
    uint8_t buf1[1024] ,buf2[1024],buf3[1024];
    int i = 1;
    int x = 0;
    memset(buf1,0,sizeof(buf1));

    s1 = get_pm_slot();
    printf("slot %x %x\n",get_pm_slot(),s1);
    if (s1 == 0){

        i = 100;
        while(1){
        printf("0 cycle %d\n",x++);
            memset(buf2,x%256,sizeof(buf2));
            memset(buf3,0x0,sizeof(buf3));

            //send data form pmA to pmC
            printf("send data to slot 2\n");
            copy_to_pmbuf(2,buf2,i);

            printf("start send data from slot 0 to 2\n");
            enable_pm_send(2,1);

            printf("check send\n");
            while(check_pm_send(2)<0)
                ;

            printf("send ok\n");

            printf("try to recv data \n");

            //recv data from pmC to pmA 
            int16_t res ;
            printf("check recv\n");
            while(check_pm_recv(2,1)<0)
                ;

            printf("recv data from slot 2 to 0 \n");
            res = copy_from_pmbuf(2,1,buf3);
            if(res >=0){
                printf("recv %d bytes ,data are :\n",res);
                for(int j =0 ; j<res;j++){
                    printf("%x ",buf3[j]);
                    if (buf3[j] != (x%256))
                        printf("recv falt %x\n",buf3[j]);
                }
                printf("\n");
            }
        }
    }
    else if (s1 ==1 ){

        i = 100;
        while(1){
        printf("1 cycle %d\n",x++);
            memset(buf2,x%256,sizeof(buf2));
            memset(buf3,0x0,sizeof(buf3));

            //send data form pmA to pmC
            printf("send data to slot 2\n");
            copy_to_pmbuf(2,buf2,i);

            printf("start send data from slot 0 to 2\n");
            enable_pm_send(2,1);

            printf("check send\n");
            while(check_pm_send(2)<0)
                ;

            printf("send ok\n");

            printf("try to recv data \n");

            //recv data from pmC to pmA 
            int16_t res ;
            printf("check recv\n");
            while(check_pm_recv(2,1)<0)
                ;

            printf("recv data from slot 2 to 0 \n");
            res = copy_from_pmbuf(2,1,buf3);
            if(res >=0){
                printf("recv %d bytes ,data are :\n",res);
                for(int j =0 ; j<res;j++){
                    printf("%x ",buf3[j]);
                    if (buf3[j] != (x%256))
                        printf("recv falt %x\n",buf3[j]);
                }
                printf("\n");
            }
        }
    }
    else if (s1 == 2) {

        i = 100;
        while(1){
        printf("2 cycle %d\n",x++);
            memset(buf2,x%256,sizeof(buf2));
            memset(buf3,0x0,sizeof(buf3));
            int16_t res ;

            printf("try to recv data \n");
            printf("check recv\n");
            //recv data from pmC to pmA 
            while(check_pm_recv(1,1)<0)
                ;
            printf("recv data from slot 2 to 0 \n");
            res = copy_from_pmbuf(1,1,buf3);
            if (res >=0){
                printf("recv %d bytes , data are :\n",res);
                for(int j =0 ; j<res;j++){
                    printf("%x ",buf3[j]);
                    if (buf3[j] != (x%256))
                        printf("recv falt %x\n",buf3[j]);
                }
                printf("\n");
            }

            //send data to pmA from pmC
            printf("send data to slot 0\n");
            copy_to_pmbuf(1,buf2,i);
            printf("start send\n");
            enable_pm_send(1,1);
            printf("check send\n");
            while(check_pm_send(1)<0)
                ;
            printf("send ok\n");

        }
    }

}
