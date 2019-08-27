
#include "./demo.h"
#include <config.h>
#include <libc/stdio.h>
#include <p1020rdb.h>
#include <kernel/sysPic.h>
#include <ppc/toolPpc.h>

//#include <kernel/sysOs.h>
#include <kernel/sysApi.h>

unsigned int * const flag1=0x1100000;
unsigned int * const flag2=0x2100000;
unsigned int * const flag3=0x3100000;
unsigned int * const flag4=0x5100000;
unsigned int * const flag5=0x6100000;
unsigned int * const flag6=0x7100000;


/****************************************/
void  jumpos()
{
    UINT32 t1,t2,t3,t4;
    printf("s %d\n",TASK_SPESTACK_HIGH-KERNEL_STACK_LOW);    

#ifdef _CAG_CONFIG_AMP0
    //t1= getTblus();/*get timestamp*/
    os_copy_data(0x5000000,0x00100000);
    printf("11\n");
    os_copy_stack();
    printf("22\n");
    //t2= getTblus();
    os_change_mmu();
    printf("D\n");
    //t3= getTblus();
    os_clear_data(0x5000000,0x00100000);
    os_clear_stack();
    os_update_mno();
    //t4= getTblus();
#else
    //t1= getTblus();/*get timestamp*/
    os_copy_data(0x1000000,0x00100000);
    printf("11\n");
    os_copy_stack();
    printf("22\n");
    //t2= getTblus();
    os_change_mmu();
    printf("D\n");
    //t3= getTblus();
    os_clear_data(0x1000000,0x00100000);
    os_clear_stack();
    os_update_mno();
    //t4= getTblus();
#endif

    //printf("%d %d %d %d\n",t1,t2,t3,t4);
}

void os_switch(void)
{
    powerpcIntDisable();
    printf("os_s\n");
    os_gap_check();
    jumpos();
    powerpcIntEnable();
}

char temp[140];

#ifdef _CAG_CONFIG_AMP0
#define CHANGE_STK 0x04100000
#else
#define CHANGE_STK 0x00100000
#endif

void others()
{
    int err;
    /*需要在0-0x4000000区域内创建任务，来执行切换地址映射的代码*/
//    char *temp=0x300000;

    OSTaskCreate(temp,
            "switchos",
            os_switch,
            NULL,
            1,
            CHANGE_STK,
            0x100,
            0x200,
            0,
            0,
            0x4200000,
            1|2,
            &err);
}

void task_void(char a[])
{
    int i=1;
    int err;
    while(1) {
        printf("ta %d\n",i++);
        powerpcIntDisable();
    os_gap_check();
        printf("T1 0x%x:%x 0x%x:%x 0x%x:%x ",flag1,*flag1,flag2,*flag2,flag3,*flag3);
        printf("T11 0x%x:%x 0x%x:%x 0x%x:%x\n",flag4,*flag4,flag5,*flag5,flag6,*flag6);
        powerpcIntEnable();
        printf("MU\n");

        if(i%1 == 0) {
            printf("os_switch\n");
            others();/*切换地址映射*/
//            powerpcIntDisable();
            //printf("T111 0x%x:%x 0x%x:%x 0x%x:%x ",flag1,*flag1,flag2,*flag2,flag3,*flag3);
            //printf("T111 0x%x:%x 0x%x:%x 0x%x:%x\n",flag4,*flag4,flag5,*flag5,flag6,*flag6);
        } else {
            os_delay_task(1,&err);
        }

    }
}

void task_void1(char a[])
{
    int i=0;
    int err;
    while(1) {
        printf("tb\n");
        powerpcIntDisable();
        printf("T2 0x%x:%x 0x%x:%x 0x%x:%x ",flag1,*flag1,flag2,*flag2,flag3,*flag3);
        printf("T2 0x%x:%x 0x%x:%x 0x%x:%x\n",flag4,*flag4,flag5,*flag5,flag6,*flag6);
        powerpcIntEnable();
        os_delay_task(1,&err);
    }
}

int id1,id2;

void dual_test(void)
{
    printf("maina\n");
    unsigned int err;
    char aa = 1;
    char bb=2;
    char c=3;

    *flag1=0x11111111;
    *flag2=0x22222222;
    *flag3=0x33333333;
    *flag4=0x44444444;
    *flag5=0x55555555;
    *flag6=0x66666666;

    printf("create tasks\n");
    id1 = os_create_task("task1",task_void,&aa,1,1,&err);
    if(err != 0)
        printf("create task1 fail %d\n",err);

    id2 = os_create_task("task2",task_void1,&bb,1,1,&err);
    if(err != 0)
        printf("create task1 fail %d\n",err);

    id2 = os_create_task("task3",task_void1,&c,1,1,&err);
    if(err != 0)
        printf("create task1 fail %d\n",err);
    os_gap_check();
}

void os_gap_fill()
{
    memset(GAP_0,0x00,GAP_SIZE);
    memset(GAP_1,0x00,GAP_SIZE);
    memset(GAP_2,0x00,GAP_SIZE);
    memset(GAP_3,0x00,GAP_SIZE);
    memset(GAP_4,0x00,GAP_SIZE);
    memset(GAP_5,0x00,GAP_SIZE);
    memset(GAP_6,0x00,GAP_SIZE);
    memset(GAP_7,0x00,GAP_SIZE);

#ifdef _CAG_CONFIG_AMP0
    for(int i=1;i<3;i++){
#else
    for(int i=1;i<4;i++){
#endif
        memset(TASK_STACK_HIGH- i *(GAP_SIZE+TASK_STACK_SIZE),0x00,GAP_SIZE);
    }
    for(int i=1;i<7;i++){
        memset(TASK_SPESTACK_HIGH- i *(GAP_SIZE+TASK_SPESTACK_SIZE),0x00,GAP_SIZE);
    }
}

void os_gap_check()
{
    int8_t broken=0;
    uint8_t buf[GAP_SIZE]={0x00};
    memset(buf,0x00,GAP_SIZE);
    int ret=0;
    printf("check\n");
    ret=memcmp(GAP_0,buf,GAP_SIZE);
    if(ret !=0){
        powerpcIntDisable();
        printf("%s %d\n",__FUNCTION__,__LINE__);
        for(int i=0;i<GAP_SIZE;i++){
            printf("%x ",*(uint8_t *)(GAP_0+i));
        }
        broken=1;
        printf("\n");
    }
    ret=memcmp(GAP_1,buf,GAP_SIZE);
    if(ret !=0){
        powerpcIntDisable();
        printf("%s %d\n",__FUNCTION__,__LINE__);
        for(int i=0;i<GAP_SIZE;i++){
            printf("%x ",*(uint8_t *)(GAP_1+i));
        }
        broken=1;
        printf("\n");
    }
    ret=memcmp(GAP_2,buf,GAP_SIZE);
    if(ret !=0){
        powerpcIntDisable();
        printf("%s %d\n",__FUNCTION__,__LINE__);
        printf("%x\n",GAP_2);
        for(int i=0;i<GAP_SIZE;i++){
            printf("%x ",*(uint8_t *)(GAP_2+i));
        }
        broken=1;
        printf("\n");
    }
    ret=memcmp(GAP_3,buf,GAP_SIZE);
    if(ret !=0){
        powerpcIntDisable();
        printf("%s %d\n",__FUNCTION__,__LINE__);
        printf("%x\n",GAP_3);
        for(int i=0;i<GAP_SIZE;i++){
            printf("%x ",*(uint8_t *)(GAP_3+i));
        }
        broken=1;
        printf("\n");
    }
    ret=memcmp(GAP_4,buf,GAP_SIZE);
    if(ret !=0){
        powerpcIntDisable();
        printf("%s %d\n",__FUNCTION__,__LINE__);
        for(int i=0;i<GAP_SIZE;i++){
            printf("%x ",*(uint8_t *)(GAP_4+i));
        }
        broken=1;
        printf("\n");
    }
    ret=memcmp(GAP_5,buf,GAP_SIZE);
    if(ret !=0){
        powerpcIntDisable();
        printf("%s %d\n",__FUNCTION__,__LINE__);
        for(int i=0;i<GAP_SIZE;i++){
            printf("%x ",*(uint8_t *)(GAP_5+i));
        }
        broken=1;
        printf("\n");
    }
    ret=memcmp(GAP_6,buf,GAP_SIZE);
    if(ret !=0){
        powerpcIntDisable();
        printf("%s %d\n",__FUNCTION__,__LINE__);
        for(int i=0;i<GAP_SIZE;i++){
            printf("%x ",*(uint8_t *)(GAP_6+i));
        }
        broken=1;
        printf("\n");
    }
    ret=memcmp(GAP_7,buf,GAP_SIZE);
    if(ret !=0){
        powerpcIntDisable();
        printf("%s %d\n",__FUNCTION__,__LINE__);
        printf("%x\n",GAP_7);
        for(int i=0;i<GAP_SIZE;i++){
            printf("%x ",*(uint8_t *)(GAP_7+i));
        }
        broken=1;
        printf("\n");
    }

#ifdef _CAG_CONFIG_AMP0
    for(int i=1;i<3;i++){
#else
    for(int i=1;i<4;i++){
#endif
        ret=memcmp(TASK_STACK_HIGH- i *(GAP_SIZE+TASK_STACK_SIZE),buf,GAP_SIZE);
        if(ret !=0){
            powerpcIntDisable();
            printf("%s %d\n",__FUNCTION__,__LINE__);
            for(int j=0;j<GAP_SIZE;j++){
                printf("%x ",*(uint8_t *)(TASK_STACK_HIGH- i *(GAP_SIZE+TASK_STACK_SIZE)+j));
            }
            broken=1;
        printf("\n");
        }
    }
    for(int i=1;i<7;i++){
        ret=memcmp(TASK_SPESTACK_HIGH- i *(GAP_SIZE+TASK_SPESTACK_SIZE),buf,GAP_SIZE);
        if(ret !=0){
            powerpcIntDisable();
            printf("%s %d\n",__FUNCTION__,__LINE__);
            for(int j=0;j<GAP_SIZE;j++){
                printf("%x ",*(uint8_t *)(TASK_SPESTACK_HIGH- i *(GAP_SIZE+TASK_SPESTACK_SIZE)+j));
            }
            broken=1;
        printf("\n");
        }
    }
    if(broken==1){
        while(1);
    }
}
