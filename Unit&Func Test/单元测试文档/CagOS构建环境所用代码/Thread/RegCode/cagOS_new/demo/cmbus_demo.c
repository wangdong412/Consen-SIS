#include "../driver/device/pcie/cmbus.h"

int16_t cmbus_send(int32_t station, int8_t *buf, int32_t len,int32_t timeout)
{
    int16_t res = -1;
    if(copy_to_cmbuf(buf,len)<0){   //copy message to send buffer , and if cmbus is sending just return negative
        return -1;
    }

    if ( enable_cm_send(station) <0){   //enable send
        return -1;
    }
    while(1){
        res = check_cm_send(); //check if send ok 
        if (res >=0 || timeout-- == 0)
            break;
    }

    return res;
}
int16_t cmbus_recv(int8_t *buf, int32_t timeout)
{
    int16_t res = -1;
    while(1){
        res = check_cm_recv();//check whether recived ACK or data messsage
        if(res >=0 || timeout-- == 0)
            break;
    }
    if (res >0 ){
        res = copy_from_cmbuf(buf); //copy data from receive buffer and clear receive status register
    }
    return res;
}

void cmbus_demo()
{
    uint8_t buf1[4*1024],buf2[4*1024];
    printf("cmbus demo\n");
    set_pm_slot();

    while(1){
        if (cmbus_send(6,buf1,sizeof(buf1),0x20000)<0){
            printf("cmbus send fail\n");
        }
        else{
            printf("cmbus send ok %x\n",buf1[0]);
        }
        if( cmbus_recv(buf2,0x20000) <0){
            printf("cmbus recv fail\n");
        }
        else {
            printf("cmbuf recv ok %x %x\n",buf2[0],buf2[4096]);
        }
        sysUsDelay(2000*1000);
    }
}
