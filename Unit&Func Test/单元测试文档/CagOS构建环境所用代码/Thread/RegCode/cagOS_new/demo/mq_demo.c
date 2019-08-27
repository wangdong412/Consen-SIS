
#include <libc/cagboot.h>
#include "../cagos.h"

int qnum1,qnum2;

static int sent1=0,sent2=0,recv1=0;
static int recv3=0,recv2=0;

void mq_task0(void *arg)
{
    int i=0;
    int err;

    while(1) {
        printf("tsk0 %d sent 1 %d sent 2 %d recv1 %d recv2 %d\n",i,sent1,sent2,recv1,recv2);
        i++;
        udelay(500000);
    }
}

//post and pend
void mq_task1(void *arg)
{
    int i=0;
    printf("tsk1 %d\n",i);
    i++;

#if 1
    UINT32 to=0;
    UINT8 *buf="I am task 1";
    UINT8 recv[100];
    UINT8 *precv;
    unsigned short size;
    unsigned int err;

    while(1) {
        precv = recv;
        size =0;
        os_queue_post(qnum2,buf,strlen(buf),OS_OPT_POST_FIFO,&err);
        if (err == OS_ERR_NONE) {
            sent1++;
        }
        os_queue_get(qnum1,recv,&size,OS_OPT_PEND_BLOCKING,to,&err);
        if (err == OS_ERR_NONE) {
            recv1++;
        } else
            printf("task 1 recv none\n");
        printf("\n");
        os_delay_task(1000,&err);
    }
#endif
}

//post and pend
void mq_task2(void *arg)
{
    int i=0;
    printf("tsk2 %d\n",i);
    i++;
#if 1
    UINT32 to=0;
    UINT8 *buf="I am task 2";
    UINT8 recv[100];
    UINT8 *precv;
    unsigned short size;
    unsigned int err;
    while(1) {
        precv = recv;
        size=0;
        os_queue_post(qnum1,buf,strlen(buf),OS_OPT_POST_FIFO,&err);
        if (err == OS_ERR_NONE) {
            sent2++;
        }
        os_queue_get(qnum2,recv,&size,OS_OPT_PEND_BLOCKING,to,&err);
        if (err == OS_ERR_NONE) {
            recv2++;
        } else
            printf("task 2 recv none\n");
        printf("\n");

        os_delay_task(1000,&err);
    }
#endif

}

//post fifo
void mq_task3(void *arg)
{
    int i=0;
    printf("tsk3 %d\n",i);
    i++;
#if 1
    UINT32 to=0;
    UINT8 *buf="I am task 3,I send you messages ,please receive it";
    UINT8 recv[100];
    UINT8 *precv;
    unsigned short size;
    unsigned int ts,err;
    i=1;
    while(1) {
        precv = recv;
        size=0;
        os_queue_post(qnum1,buf,strlen(buf),OS_OPT_POST_FIFO,&err);
        if (err != OS_ERR_NONE) {

        } else
            sent1++;

        if(i%20==0)
            os_delay_task(1,&err);
        i++;
    }
#endif

}

//pend block
void mq_task4(void *arg)
{
    int i=0;
    printf("tsk4 %d\n",i);
    i++;
#if 1
    UINT32 to=0;
    UINT8 *buf="I am task 4,I send you messages ,please receive it";
    UINT8 recv[100];
    UINT8 *precv;
    unsigned short size;
    unsigned int err;
    i=1;
    while(1) {
        precv = recv;
        size=0;
        os_queue_get(qnum1,recv,&size,OS_OPT_PEND_BLOCKING,to,&err);
        if (err == OS_ERR_NONE) {
            recv1++;
        } else {
            printf("recv none %d\n",err);
        }

        if(i%2==0) {
            os_delay_task(1,&err);
        }
        i++;

    }
#endif

}

//pend block
void mq_task5(void *arg)
{
    int i=0;
    printf("tsk5 %d\n",i);
    i++;
#if 1
    UINT32 to=0;
    UINT8 *buf="I am task 4,I send you messages ,please receive it";
    UINT8 recv[100];
    UINT8 *precv;
    unsigned short size;
    unsigned int err;
    i=1;
    while(1) {
        precv = recv;
        size=0;
        os_queue_get(qnum1,recv,&size,OS_OPT_PEND_BLOCKING,to,&err);
        if (err == OS_ERR_NONE) {
            recv2++;
        } else {
            printf("recv none %d\n",err);
        }

        if(i%2==0) {
            os_delay_task(1,&err);
        }
        i++;

    }
#endif
}

//post all
void mq_task6(void *arg)
{
    int i=0;
    printf("tsk6 %d\n",i);
    i++;
#if 1
    UINT32 to=0;
    UINT8 *buf="I am task 4,I send you messages ,please receive it";
    UINT8 recv[100];
    UINT8 *precv;
    unsigned short size;
    unsigned int err;
    i=1;
    while(1) {
        precv = recv;
        size=0;
        os_queue_post(qnum1,buf,strlen(buf),OS_OPT_POST_ALL,&err);
        if (err != OS_ERR_NONE) {
            printf("post fail %d\n",err);
        } else
            sent2++;
        if(i%13==0) {
            os_delay_task(1000,&err);
        }
        i++;
    }
#endif
}

//pend non block
void mq_task7(void *arg)
{
    int i=0;
    printf("tsk7 %d\n",i);
    i++;
#if 1
    UINT32 to=0;
    UINT8 *buf="I am task 4,I send you messages ,please receive it";
    UINT8 recv[100];
    UINT8 *precv;
    unsigned short size;
    unsigned int err;
    i=1;
    while(1) {
        precv = recv;
        size=0;
        os_queue_get(qnum1,recv,&size,OS_OPT_PEND_BLOCKING,to,&err);
        if (err == OS_ERR_NONE) {
            recv2++;
        } else {
        }
        if(i%2==0) {
            os_delay_task(10,&err);
        }
        i++;
    }
#endif
}

void test_msg_demo(void)
{
    int a=1;
    int err;

    UINT8    *qname="msgq1";
    OS_MSG_QTY  q1Max = 100;
    OS_ERR      q1err;
    UINT8    *qname2="msgq2";
    OS_MSG_QTY  q2Max = 100;
    OS_ERR      q2err;

    qnum1 = os_queue_create(qname,q1Max,&q1err);
    if (q1err != OS_ERR_NONE) {
        printf("osq 1 create faile %d\n",q1err);
        return ;
    }

    qnum2 = os_queue_create(qname2,q2Max,&q2err);
    if (q1err != OS_ERR_NONE) {
        printf("osq 2 create faile %d\n",q1err);
        os_queue_del(qnum2,OS_OPT_DEL_ALWAYS,&q2err);
        if (q2err != OS_ERR_NONE ) {
            printf("osq 1 delete fail %d\n",q2err);
        }
        return ;
    }

    UINT8 *mq_task0Name="mq_task0";
    UINT8 *mq_task1Name="mq_task1";
    UINT8 *mq_task2Name="mq_task2";
    UINT8 *mq_task3Name="mq_task3";
    UINT8 *mq_task4Name="mq_task4";
    UINT8 *mq_task5Name="mq_task5";

//#define _RECV_MORE_
#ifdef _SEND_MORE_
    os_create_task(mq_task2Name,mq_task3,NULL,0,0,&err);
    os_create_task(mq_task1Name,mq_task4,NULL,1,0,&err);
    os_create_task(mq_task0Name,mq_task0,NULL,2,0,&err);
#elif defined(_RECV_MORE_)
    os_create_task(mq_task2Name,mq_task5,NULL,0,0,&err);
    os_create_task(mq_task1Name,mq_task3,NULL,1,0,&err);
    os_create_task(mq_task0Name,mq_task0,NULL,err);
#elif defined(_BROADCAST_)
    os_create_task(mq_task2Name,mq_task6,NULL,0,0,&err);
    os_create_task(mq_task3Name,mq_task4,NULL,1,0,&err);
    os_create_task(mq_task1Name,mq_task5,NULL,2,0,&err);
    os_create_task(mq_task0Name,mq_task0,NULL,3,0,&err);
#elif defined(_PEND_NON_BLOCK_)
    os_create_task(mq_task2Name,mq_task7,NULL,0,0,&err);
    os_create_task(mq_task1Name,mq_task3,NULL,1,0,&err);
    os_create_task(mq_task0Name,mq_task0,NULL,2,0,&err);
#else //pend and post
    os_create_task(mq_task2Name,mq_task1,NULL,0,0,&err);
    os_create_task(mq_task1Name,mq_task2,NULL,1,0,&err);
    os_create_task(mq_task0Name,mq_task0,NULL,2,0,&err);
#endif
}
