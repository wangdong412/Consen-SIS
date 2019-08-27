/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  shell cmd code for PPC85XX.
 * Copyright (c) 2013, 2014 CAG Systems, Inc.
 *------------------------------------------------------------
 *
 *    @@@@     @@@      @@@@@       ####      ######
 *  @@       @@  @@    @@         ##    ##   ##
 * @@       @@    @@  @@   @@@@  ##      ## ######
 * @@      @@@@@@@@@@ @@    @@    ##    ##      ##
 *  @@@@@ @@@      @@@  @@@@@@      ####  #######
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable CAG license agreement.
 *
 * 1 tab == 4 spaces!
 *------------------------------------------------------------
 * modification history
 * --------------------
 * 2016.02.16, Ezio created
 *
 * DESCRIPTION
 * task demo source file.
 */


#include <libc/cagboot.h>
#include <kernel/sysOs.h>
#include "../cagos.h"
static int id1,id2,id3;

typedef unsigned int uint32_t;
uint32_t s_uiRTStoUP1Sem;
uint32_t s_uiRTStoUP2Sem;
uint32_t s_uiUP1toRTSSem;
uint32_t s_uiUP2toRTSSem;

static void task_1(char *arg)
{
    unsigned int err;
    while(1) {
        printf("task %d\n",*(int *)arg);
        if ( 0 != os_resume_task(id3,&err)) {
            printf("resume task 3 fail %d\n",err);
        }
        os_delay_task(3,&err);
    }
}

static void task_2(char *arg)
{
    int todel=1;
    unsigned int err;
    while(1) {
        if(0 != os_del_task(todel,&err)) {
            if(0 != os_del_task(0xffff,&err)) {
                printf("delete tasks fail\n");
                while(1);
            }
        }
        os_delay_task(11,&err);
    }
}

static void task_3(char *arg)
{
    int dly=3;
    unsigned int err;
    while(1) {
        printf("task %d\n",*(int *)arg);
        if ( 0 != os_suspend_task(id3,&err)) {
            printf("suspend task 3 fail %d\n",err);
        }
        printf("task %d back\n",*(int *)arg);
    }
}

//test 1 : no tq, no same prio


//test 2 : add tq

static int val1=0,val2=0,val3=0,val4=0;

extern OS_TCB *OSTCBCurPtr;

static int cnt[2]={0};
static void  task_void(char *arg)
{
    uint32_t uiPend,uiErr;
    while(1){
        if(cnt[1] == 0){
            int x= getTblus();
            printf("tv0 %d ,tq: %d\n",x,OSTCBCurPtr->TimeQuantaCtr);
            cnt[1] = 1;
            cnt[0]=0;
        }
        uiPend = os_sem_pend(s_uiRTStoUP1Sem, 0, OS_OPT_PEND_BLOCKING, &uiErr);
        printf("pend r2u1\n");
        if(uiPend == 0) {
            os_sem_post(s_uiUP1toRTSSem, OS_OPT_POST_1|OS_OPT_POST_NO_SCHED, &uiErr);
            printf("post u12r\n");
        }
    }
}

static void  task_void1(char *arg)
{
    uint32_t uiPend,uiErr;
    while(1){
        if(cnt[0] == 0){
            int x= getTblus();
            printf("tv1 %d ,tq: %d\n",x,OSTCBCurPtr->TimeQuantaCtr);
            cnt[0] = 1;
            cnt[1]=0;
        }
        uiPend = os_sem_pend(s_uiUP1toRTSSem, 0, OS_OPT_PEND_NON_BLOCKING, &uiErr);
        printf("PEND u12r\n");
        if(uiPend == 0){
            os_sem_post(s_uiRTStoUP1Sem, OS_OPT_POST_1|OS_OPT_POST_NO_SCHED, &uiErr);
            printf("POST r2u1\n");
        }
    }
}
static void  task_void2(char *arg)
{
    int x = 0;
    int y=getTblus();
    while(1)
    {
        if (getTblus()-y >= 10000000){
            printf("%x %x\n",cnt[0],cnt[1]);
            y=getTblus();
        }
    }
}

static void task_1q(char *arg)
{
    int err;
    while(1) {
        val1++;
        if (val1 == 0x10000000) {
            printf("val1\n");
            os_resume_task(id2,&err);
            if(err != 0)
                printf("resume %d fail\n",id2);
            val1 =0;
        }
    }
}

static void task_2q(char *arg)
{
    int err;
    while(1) {
        val2++;
        if (val2 == 0x10000000) {
            printf("val2\n");
            os_suspend_task(id2,&err);
            if(err != 0)
                printf("suspend %d err\n",id2);
            val2 =0;
        }
    }
}

static void task_3q(char *arg)
{
    int err;
    while(1) {
        val3++;
        if (val3 == 0x10000000) {
            printf("val3\n");
            os_del_task(id3,&err);
            if(err != 0)
                printf("suspend %d err\n",id3);
            val3 =0;
        }
    }
}

//test 3 : multi task using one prio


void test_task_demo(void)
{
    printf("123\n");
    int a=1;
    unsigned int err;
#if 0
    id1 = os_create_task("task1",task_1,&a,1,0,&err);
    if(err != 0)
        printf("create task1 fail %d\n",err);
    a=2;
    os_create_task("task2",task_2,&a,2,0,&err);
    if(err != 0)
        printf("create task1 fail %d\n",err);
    a=3;
    os_create_task("task3",task_3,&a,3,0,&err);
    if(err != 0)
        printf("create task1 fail %d\n",err);
#endif

#if 0
    printf("123\n");
    id1 = os_create_task("task1",task_1q,&a,3,2,&err);
    if(err != 0)
        printf("create task1 fail %d\n",err);
    a=2;
    printf("123\n");
    id2 = os_create_task("task2",task_2q,&a,3,5,&err);
    if(err != 0)
        printf("create task1 fail %d\n",err);
    a=3;
    printf("123\n");
    id3 = os_create_task("task3",task_3q,&a,3,4,&err);
    if(err != 0)
        printf("create task1 fail %d\n",err);
    printf("ffff\n");
#endif

#if 1
    uint32_t uiErr=0;
    s_uiRTStoUP1Sem = os_sem_create("RTStoUP1", 1, &uiErr);
    s_uiRTStoUP2Sem = os_sem_create("RTStoUP2", 1, &uiErr);
    s_uiUP1toRTSSem = os_sem_create("UP1toRTS", 0, &uiErr);
    s_uiUP2toRTSSem = os_sem_create("UP2toRTS", 0, &uiErr);


    printf("abc\n");
    a=0;
    char aa = 0;
    id1 = os_create_task("task1",task_void,&aa,3,3000,&err);
    if(err != 0)
        printf("create task1 fail %d\n",err);
    a=1;
    printf("abc\n");
    char bb=1;
    id2 = os_create_task("task2",task_void1,&bb,3,5000,&err);
    if(err != 0)
        printf("create task1 fail %d\n",err);

    a=2;
    printf("abc\n");
    //id2 = os_create_task("task3",task_void2,&a,3,1000,&err);
    if(err != 0)
        printf("create task3 fail %d\n",err);

#endif
}
