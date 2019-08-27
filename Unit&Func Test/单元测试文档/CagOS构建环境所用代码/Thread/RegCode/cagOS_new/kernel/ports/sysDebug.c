#include <kernel/sysOs.h>
#include <kernel/sysMap.h>
#include <libc/stdio.h>

void debugKernelC ()
{
    printf ("Jeff C debug here\n");
    while (1) {
        printf ("Jeff C debug here\n");
    }
}

void debugKernelS ()
{
    printf ("Jeff S debug here\n");
    while (1) {
    }
}

OS_TCB        AppTaskTCB[4];

static void AppTask1 (void *p_arg)
{
    OS_ERR             os_err;
    UINT32 time3;
    UINT32 time3Bak;

    printf("%s,%d\n",__FUNCTION__,__LINE__);
    while (DEF_ON) {
        powerpcIntDisable();
        //whetstone_test ();
        powerpcIntEnable();

#if 0
        time3 = getTblms ();
        if (!((time3-time3Bak) < 5))
            printf("%s tick:%d ms\n",__FUNCTION__,(time3 - time3Bak));
        time3Bak = time3;

        OSTimeDly((OS_TICK )5000,                                  /* Synchronize with clock tick                            */
                  (OS_OPT  )OS_OPT_TIME_DLY,
                  (OS_ERR *)&os_err);
#endif
    }
}

static void AppTask2 (void *p_arg)
{
    OS_ERR             os_err;
    UINT32 time3;
    UINT32 time3Bak;

    while (DEF_ON) {
        powerpcIntDisable();
        //whetstone_test ();
        powerpcIntEnable();

#if 0
        time3 = getTblms ();
        if (!((time3-time3Bak) < 5))
            printf("%s tick:%d ms\n",__FUNCTION__,(time3 - time3Bak));
        time3Bak = time3;

        OSTimeDly((OS_TICK )5000,                                  /* Synchronize with clock tick                            */
                  (OS_OPT  )OS_OPT_TIME_DLY,
                  (OS_ERR *)&os_err);
#endif
    }
}

static void AppTask3 (void *p_arg)
{
    OS_ERR             os_err;
    UINT32 time3;
    UINT32 time3Bak;

    while (DEF_ON) {
        powerpcIntDisable();
        //whetstone_test ();
        powerpcIntEnable();

#if 0
        time3 = getTblms ();
        if (!((time3-time3Bak) < 5))
            printf("%s tick:%d ms\n",__FUNCTION__,(time3 - time3Bak));
        time3Bak = time3;

        OSTimeDly((OS_TICK )5000,                                  /* Synchronize with clock tick                            */
                  (OS_OPT  )OS_OPT_TIME_DLY,
                  (OS_ERR *)&os_err);
#endif
    }
}

static void AppTask4 (void *p_arg)
{
    OS_ERR             os_err;
    UINT32 time3;
    UINT32 time3Bak;

    while (DEF_ON) {
#if 0
        time3 = getTblms ();

        if (!((time3-time3Bak) < 5))
            printf("%s tick:%d ms\n",__FUNCTION__,(time3 - time3Bak));
        time3Bak = time3;
        OSTimeDly((OS_TICK )5000,                                  /* Synchronize with clock tick                            */
                  (OS_OPT  )OS_OPT_TIME_DLY,
                  (OS_ERR *)&os_err);
#endif
    }
}

void taskDemo ()
{
    OS_ERR  os_err;

    printf("%s,%d\n",__FUNCTION__,__LINE__);


    OSTaskCreate(&AppTaskTCB[0],                              /* Create the Start Task                                */
                 "App Task 1",
                 AppTask1,
                 0u,
                 6,
                 (CPU_STK*)TASK_STACK_6,
                 (TASK_STACK_SIZE / 10u),
                 TASK_STACK_SIZE,
                 0u,
                 2,
                 (void *) TASK_SPESTACK_6,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);

#if 0
    OSTaskCreate(&AppTaskTCB[1],                              /* Create the Start Task                                */
                 "App Task 2",
                 AppTask2,
                 0u,
                 6,
                 (CPU_STK*)TASK_STACK_7,
                 (TASK_STACK_SIZE / 10u),
                 TASK_STACK_SIZE,
                 0u,
                 2,
                 (void *) TASK_SPESTACK_7,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);


    OSTaskCreate(&AppTaskTCB[2],                              /* Create the Start Task                                */
                 "App Task 3",
                 AppTask3,
                 0u,
                 6,
                 (CPU_STK*)TASK_STACK_8,
                 (TASK_STACK_SIZE / 10u),
                 TASK_STACK_SIZE,
                 0u,
                 2,
                 (void *) TASK_SPESTACK_8,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);


    OSTaskCreate(&AppTaskTCB[3],                              /* Create the Start Task                                */
                 "App Task 4",
                 AppTask4,
                 0u,
                 6,
                 (CPU_STK*)TASK_STACK_9,
                 (TASK_STACK_SIZE / 10u),
                 TASK_STACK_SIZE,
                 0u,
                 2,
                 (void *) TASK_SPESTACK_9,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);
#endif
}

typedef unsigned int uint32_t;
uint32_t s_uiRTStoUP1Sem;
uint32_t s_uiRTStoUP2Sem;
uint32_t s_uiUP1toRTSSem;
uint32_t s_uiUP2toRTSSem;

static int cnt[2]= {0};
static void  task_void(char *arg)
{
    uint32_t uiPend,uiErr,uiPost;
    UINT32 time3 = getTblms();
    UINT32 time3Bak = getTblms();
    printf("%s %d \n",__FUNCTION__,__LINE__);
    while(1) {
        uiPend = os_sem_pend(s_uiRTStoUP1Sem, 0, OS_OPT_PEND_NON_BLOCKING, &uiErr);
        time3 = getTblms ();

        if (!((time3-time3Bak) < 5000)) {
            printf("%s tick:%d ms\n",__FUNCTION__,(time3 - time3Bak));
            time3Bak = time3;
        }
        //printf("uiPend:%d\n",uiPend);

        if(uiPend == 0) {
            uiPost = os_sem_post(s_uiUP1toRTSSem, OS_OPT_POST_1, &uiErr);
            printf("uiPost: %d\n",uiPost);
            printf("-----------------\n");
        }
    }
}

static void  task_void1(char *arg)
{
    uint32_t uiPend,uiErr,uiPost;
    UINT32 time3 = getTblms();
    UINT32 time3Bak = getTblms();
    printf("%s %d \n",__FUNCTION__,__LINE__);

    while(1) {
        uiPend = os_sem_pend(s_uiUP1toRTSSem, 0, OS_OPT_PEND_NON_BLOCKING, &uiErr);
        time3 = getTblms ();

        if (!((time3-time3Bak) < 3000)) {
            printf("%s tick:%d ms\n",__FUNCTION__,(time3 - time3Bak));
            time3Bak = time3;
        }

        //printf("uiPend:%d\n",uiPend);

        if(uiPend == 0) {
            uiPost = os_sem_post(s_uiRTStoUP1Sem, OS_OPT_POST_1, &uiErr);
            printf("uiPost: %d\n",uiPost);
            printf("+++++++++++++++++\n");
        }
    }
}

static void  task_void2(char *arg)
{
    int x = 0;
    int y=getTblus();
    while(1) {
        if (getTblus()-y >= 1000000000) {
            printf("%x %x\n",cnt[0],cnt[1]);
            y=getTblus();
        }
    }
}



#define RTS_NUM         10
#define RTS_PRIO_BASE   5
#define QUEUS_NUM       20
#define SEM_NUM         20
#define MUT_NUM         20

OS_TCB      rts_tcbs[RTS_NUM];
OS_Q        rts_msgq[QUEUS_NUM];
OS_SEM      rts_sem[SEM_NUM];
OS_MUTEX    rts_mutex[MUT_NUM];

void rts_var_init(void)
{
    int i=0;
    for(i=0; i<QUEUS_NUM; i++) {
        rts_msgq[i].Type = OS_OBJ_TYPE_NONE;
    }
    for(i=0; i<SEM_NUM; i++) {
        rts_sem[i].Type = OS_OBJ_TYPE_NONE;
    }
    for(i=0; i<MUT_NUM; i++) {
        rts_mutex[i].Type = OS_OBJ_TYPE_NONE;
    }
}


static int id1,id2,id3;
void rts_main()
{

    rts_var_init();
    int a=1;
    unsigned int err;
    uint32_t uiErr=0;
    s_uiRTStoUP1Sem = os_sem_create("RTStoUP1", 1, &uiErr);
//s_uiRTStoUP2Sem = os_sem_create("RTStoUP2", 1, &uiErr);
    s_uiUP1toRTSSem = os_sem_create("UP1toRTS", 0, &uiErr);
//s_uiUP2toRTSSem = os_sem_create("UP2toRTS", 0, &uiErr);


    printf("abc\n");
    a=0;
    char aa = 0;
    id1 = os_create_task("task1",task_void,&aa,3,5000,&err);
    if(err != 0)
        printf("create task1 fail %d\n",err);
    a=1;
    printf("abc\n");
    char bb=1;
    id2 = os_create_task("task2",task_void1,&bb,3,3000,&err);
    if(err != 0)
        printf("create task1 fail %d\n",err);

    a=2;
    printf("abc\n");
    id2 = os_create_task("task3",task_void2,&a,3,1000,&err);
    if(err != 0)
        printf("create task3 fail %d\n",err);
}




