
#include <libc/cagboot.h>
#include "../cagos.h"
static int sems[2]= {0xa};
static int pos=0;
static int sem0;

static void sem_idle(void *arg)
{
    int t1,t2;
    t1=sems[0];
    t2=sems[1];
    while(1) {
        if(t1 != sems[0] || t2 != sems[1]) {
            printf("sem idle %d %d\n",sems[0],sems[1]);
            t1=sems[0];
            t2=sems[1];
        }

    }
}

static void sem_task0(void *arg)
{
    unsigned int err=0;
    while(1) {
//        OSSemPend(&sem0,0,OS_OPT_PEND_BLOCKING,&cts,&err);
        os_sem_pend(sem0,0,OS_OPT_PEND_BLOCKING,&err);
        if(err != OS_ERR_NONE) {
            printf("tsk0 sem get fail\n");
        }
        pos=(pos+1)%2;
        sems[pos]=0;
        os_delay_task(200,&err);
//        OSSemPost(&sem0,OS_OPT_POST_1,&err);
        os_sem_post(sem0,OS_OPT_POST_1,&err);
        if(err != OS_ERR_NONE) {
            printf("tsk0 sem post fail\n");
        }
    }
}

static void sem_task1(void *arg)
{
    unsigned int err=0;
    while(1) {
        //OSSemPend(&sem0,0,OS_OPT_PEND_BLOCKING,&cts,&err);
        os_sem_pend(sem0,0,OS_OPT_PEND_BLOCKING,&err);
        if(err != OS_ERR_NONE) {
            printf("tsk1 sem get fail\n");
        }
        pos=(pos+1)%2;
        sems[pos]=1;
        os_delay_task(200,&err);
        //OSSemPost(&sem0,OS_OPT_POST_1,&err);
        os_sem_post(sem0,OS_OPT_POST_1,&err);
        if(err != OS_ERR_NONE) {
            printf("tsk1 sem post fail\n");
        }
    }

}

static void sem_task2(void *arg)
{
    unsigned int err=0;
    while(1) {
        //OSSemPend(&sem0,0,OS_OPT_PEND_BLOCKING,&cts,&err);
        os_sem_pend(sem0,0,OS_OPT_PEND_BLOCKING,&err);
        if(err != OS_ERR_NONE) {
            printf("tsk2 sem get fail\n");
        }
        pos=(pos+1)%2;
        sems[pos]=2;
        os_delay_task(200,&err);
        //OSSemPost(&sem0,OS_OPT_POST_1,&err);
        os_sem_post(sem0,OS_OPT_POST_1,&err);
        if(err != OS_ERR_NONE) {
            printf("tsk2 sem post fail\n");
        }
    }
}


void sem_test_demo()
{
    unsigned int err=0;

//    OSSemCreate(&sem0,"sem0",2,&err);
    sem0 = os_sem_create("sem0",2,&err);
    if( err != OS_ERR_NONE) {
        printf("sem create fail\n");
        return ;
    }

    os_create_task("p1",sem_task0,NULL,10,0,&err);
    os_create_task("p2",sem_task1,NULL,11,0,&err);
    os_create_task("p3",sem_task2,NULL,12,0,&err);
    os_create_task("idle",sem_idle,NULL,13,0,&err);

}
