
#include <libc/cagboot.h>
#include "../cagos.h"

static int sems[2]= {0xa};
static int pos=0;
static int id1,id2,id3;

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
    os_task_sem_set(0,0,&err);
    if(err != OS_ERR_NONE)
        printf("set semphore fail\n");
    while(1) {
        os_task_sem_pend(1,OS_OPT_PEND_BLOCKING,err);
        if(err != OS_ERR_NONE) {
            printf("tsk0 sem get fail\n");
        }
        pos=(pos+1)%2;
        sems[pos]=0;
        os_delay_task(200,&err);
        os_task_sem_post(id2,OS_OPT_POST_NONE,&err);
        if(err != OS_ERR_NONE) {
            printf("tsk0 sem post fail\n");
        } else
            printf("post2\n");
    }
}

static void sem_task1(void *arg)
{
    unsigned int err=0;
    os_task_sem_set(1,0,&err);
    if(err != OS_ERR_NONE)
        printf("set semphore fail\n");

    while(1) {
        os_task_sem_pend(1,OS_OPT_PEND_BLOCKING,&err);
        if(err != OS_ERR_NONE) {
            printf("tsk1 sem get fail\n");
        }
        pos=(pos+1)%2;
        sems[pos]=1;
        os_delay_task(200,&err);
        os_task_sem_post(id1,OS_OPT_POST_NONE,&err);
        if(err != OS_ERR_NONE) {
            printf("tsk1 sem post fail\n");
        } else
            printf("post1\n");
    }

}

void task_sem_test_demo()
{
    unsigned int err=0;

    id1 = os_create_task("p1",sem_task0,NULL,0,0,&err);
    if(err !=0)
        printf("task 1 error\n");
    id2 = os_create_task("p2",sem_task1,NULL,1,0,&err);
    if(err !=0)
        printf("task 2 error\n");
    id3 = os_create_task("idle",sem_idle,NULL,3,0,&err);
    if(err !=0)
        printf("task 3 error\n");

}
