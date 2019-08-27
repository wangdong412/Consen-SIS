#include <libc/cagboot.h>
#include "../cagos.h"

static int times1 = 0,times2 = 0;
static int mt0;

//p
static void mt_task1(void *arg)
{
    unsigned int err=0;
    while(1) {
        //OSMutexPend(&mt0,0,OS_OPT_PEND_BLOCKING,&cts,&err);
        os_mutex_pend(mt0,0,OS_OPT_PEND_BLOCKING,&err);
        if(err != OS_ERR_NONE)
            printf("t1 get mutext fail\n");
        times1 ++;
        printf("t1 %d \n",times1);
        //OSMutexPost(&mt0,OS_OPT_POST_NONE,&err);
        os_mutex_post(mt0,OS_OPT_POST_NONE,&err);
        if(err != OS_ERR_NONE)
            printf("t1 get mutext fail\n");
        os_delay_task(1,&err);
    }
}

//v
static void mt_task2(void *arg)
{
    unsigned int err=0;
    while(1) {
//        OSMutexPend(&mt0,0,OS_OPT_PEND_BLOCKING,&cts,&err);
        os_mutex_pend(mt0,0,OS_OPT_PEND_BLOCKING,&err);
        if(err != OS_ERR_NONE)
            printf("t22 get mutext fail\n");
        times2 ++;
        printf("t2 %d \n",times2);
//        OSMutexPost(&mt0,OS_OPT_POST_NONE,&err);
        os_mutex_post(mt0,OS_OPT_POST_NONE,&err);
        if(err != OS_ERR_NONE)
            printf("t2 put mutext fail\n");
        os_delay_task(1,&err);
    }
}

static void mt_idle(void *arg)
{
    while(1) {
        printf("mt idle\n");
    }
}

void mutex_test_demo()
{
    unsigned int err=0;

//    OSMutexCreate(&mt0,"mt0",&err);
    mt0 = os_mutex_create("mt0",&err);
    if(err != OS_ERR_NONE) {
        printf("mutex create fail\n");
        return ;
    }
    os_create_task("p",mt_task1,NULL,10,0,&err);
    os_create_task("v",mt_task2,NULL,11,0,&err);
    os_create_task("idle",mt_idle,NULL,12,0,&err);
}
