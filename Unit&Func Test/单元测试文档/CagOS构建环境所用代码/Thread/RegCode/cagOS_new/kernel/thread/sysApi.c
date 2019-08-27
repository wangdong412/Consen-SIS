/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  OS configure code.
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
 * 01,Feb,2016,Ezio created
 *
 * DESCRIPTION
 * This file contains OS apis
 */

/*
 *
 * prio: 任务优先级，存在相同优先级的不同任务，应用程序看到的 prio 不是真正的prio
 * id: 任务的唯一标识，供应用使用区分任务
 *
 * stack_pos: 记录那些 tcb 和 stack 已经被使用
 * pos: 标识应用任务占用的tcb
 * tcb: os 用来标识任务，位置由 pos 来决定
 * stack: 任务栈，位置由 pos 决定,
 *
 * prio_real=prio_rts +prio_base
 * id=pos+prio_rts
 * tcb=tcb[pos]
 * stack=stack[pos]
 * stack_pos: 1=not used,0=used(为了使用 gcc 的内置函数）
 *
 */

#define  MICRIUM_SOURCE
#include <kernel/sysOs.h>
#include <kernel/sysMap.h>
#include <ppc/ppc85xx.h>
#include <libc/stdio.h>

extern void sysUsDelay(int delay);

#define RTS_NUM         10
#define RTS_PRIO_BASE   5
#define QUEUS_NUM       20
#define SEM_NUM         20
#define MUT_NUM         20

OS_TCB      rts_tcbs[RTS_NUM]={0};
OS_Q        rts_msgq[QUEUS_NUM]={0};
OS_SEM      rts_sem[SEM_NUM]={0};
OS_MUTEX    rts_mutex[MUT_NUM]={0};

static unsigned long stack_pos=0xffffffff; //初始值需要体现出 os 自己的任务 or 将 rts 任务单独管理

void rts_var_init(void)
{
    int i=0;
    for(i=0;i<QUEUS_NUM;i++) {
        rts_msgq[i].Type = OS_OBJ_TYPE_NONE;
    }
    for(i=0;i<SEM_NUM;i++) {
        rts_sem[i].Type = OS_OBJ_TYPE_NONE;
    }
    for(i=0;i<MUT_NUM;i++) {
        rts_mutex[i].Type = OS_OBJ_TYPE_NONE;
    }
}
int os_get_prio(int id)
{
    return id&0xffff;
}

int os_get_pos(int id)
{
    return (id&0xffff0000)>>16;
}

/*
 * 创建任务
 *
 * Description: 创建一个新任务执行用户函数
 *
 * Arguments:
 *      [in]pname   任务名
 *      [in]ptask   任务函数
 *      [in]parg    函数参数
 *      [in]prio    任务优先级（可用范围1~9）
 *      [in]tq      时间片个数
 *      [out]err     返回操作错误码
 * Returns:
 *      创建成功返回任务id，失败返回-1/-2
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_create_task(char *pname, void *ptask, void *parg, int prio,unsigned int tq, unsigned int *err)
{
    if(prio >= RTS_NUM || prio <0)
        return -2;
    int pos = __builtin_ffs(stack_pos);
    if (pos <1)//full
        return -2;
    pos-=1;


    OSTaskCreate(&rts_tcbs[pos],
                 pname,
                 ptask,
                 parg,
                 prio+RTS_PRIO_BASE,
                 (CPU_STK*)(TASK_STACK_HIGH-(( pos +RTS_PRIO_BASE)*TASK_STACK_SIZE)),/*gcc*/
                 (TASK_STACK_SIZE/10u),
                 TASK_STACK_SIZE,
                 0,/*maximun number of messages*/
                 tq,
                 (TASK_SPESTACK_HIGH-((pos+RTS_PRIO_BASE)*TASK_SPESTACK_SIZE)),
                 (OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP),
                 err);

    if (*err != OS_ERR_NONE)
        return -1;
    else {
        stack_pos &= ~(1<<pos);
        return (pos<<16)|prio;
    }
}

/*
 *
 * 删除任务
 *
 * Description: 删除指定优先级的任务
 *
 * Arguments:
 *      [in]id    任务 id 号
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功返回0，失败返回-1
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_del_task(int id,unsigned int *err)
{
    //delete self
    if(id== 0xffff) {
        OSTaskDel(OSTCBCurPtr, err);
    } else {
        int pos= os_get_pos(id);
        if(pos>= RTS_NUM || pos<0)
            return -2;
        OSTaskDel(&rts_tcbs[pos], err);
    }

    if (*err != OS_ERR_NONE)
        return -1;
    else {
        return 0;
    }
}

/*
 *
 * 唤醒任务
 *
 * Description: 唤醒一个被挂起的任务
 *
 * Arguments:
 *      [in]id    任务 id 号
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功返回0，失败返回-1
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_resume_task(int id,unsigned int *err)
{
    int pos = os_get_pos(id);
    if(pos >= RTS_NUM || pos<0)
        return -2;
    OSTaskResume(&rts_tcbs[pos],err);
    if (*err != OS_ERR_NONE)
        return -1;
    else
        return 0;
}

/*
 *
 * 挂起任务
 *
 * Description: 挂起一个任务
 *
 * Arguments:
 *      [in]pos    任务 id 号
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功返回0，失败返回-1
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_suspend_task(int id, unsigned int *err)
{
    int pos = os_get_pos(id);
    if(pos >= RTS_NUM || pos <0)
        return -2;
    OSTaskSuspend(&rts_tcbs[pos],err);
    if (*err != OS_ERR_NONE)
        return -1;
    else
        return 0;
}

/*
 *
 * 向任务发送信号
 *
 * Description: 向一个任务发送信号
 *
 * Arguments:
 *      [in]pos    任务 id 号
 *      [in]opt     选项
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      返回任务当前的信号值
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
unsigned int os_task_sem_post(int id, unsigned short opt, unsigned int *err)
{
    int pos = os_get_pos(id);
    if(pos >= RTS_NUM || pos <0)
        return -2;
    return OSTaskSemPost(&rts_tcbs[pos],opt,err);
}

/*
 *
 * 等待信号
 *
 * Description: 等待其它任务给自己发信号
 *
 * Arguments:
 *      [in]timeout     等待信号的超时时间(为0 则一直等待)
 *      [in]opt         选项
 *      [out]timestamp  收到信号的时间戳
 *      [out]err        返回操作错误码
 *
 * Returns:
 *      返回任务收到的信号值
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
unsigned int os_task_sem_pend(int timeout, unsigned short opt, unsigned int *err)
{
    return OSTaskSemPend(timeout,opt,NULL,err);
}

/*
 *
 * 修改信号值
 *
 * Description: 修改一个任务的信号值
 *
 * Arguments:
 *      [in]id      任务 id 号
 *      [in]cnt     新的信号值
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      新的信号值
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
unsigned int os_task_sem_set(int id,unsigned int cnt, unsigned int *err)
{
    int pos = os_get_pos(id);
    if(pos >= RTS_NUM || pos <0)
        return -2;
    return OSTaskSemSet(&rts_tcbs[pos],cnt,err);
}

/*
 *
 * 任务延时
 *
 * Description: 让当前任务让权等待一段时间
 *
 * Arguments:
 *      [in]dly     延时的 tick 数
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      新的信号值
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
void os_delay_task(int dly, unsigned int *err)
{
    OSTimeDly(dly, OS_OPT_TIME_DLY,err);
}

/*
 *
 * 等待
 *
 * Description: 让任务忙等待一段时间
 *
 * Arguments:
 *      [in]us      忙等待的微秒数
 *
 * Returns: None
 *
 *
 */
void udelay(int us)
{
    sysUsDelay(us);
}

/*
 *
 * 创建消息队列
 *
 * Description: 创建一个消息队列
 *
 * Arguments:
 *      [in]qname   消息队列名称
 *      [in]qsize   消息队列的长度（byte）
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功则返回消息队列的 id ， 失败返回-1/-2
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_queue_create(char *qname,int qsize, unsigned int *err)
{
    for (int i=0; i<QUEUS_NUM; i++) {
        if(rts_msgq[i].Type == OS_OBJ_TYPE_NONE) {
            OSQCreate(&rts_msgq[i],qname,qsize,err);
            if (*err == OS_ERR_NONE)
                return i;
            else
                return -2;
        }
    }
    return -1;
}

/*
 *
 * 删除消息队列
 *
 * Description: 删除一个消息队列
 *
 * Arguments:
 *      [in]qnum    消息队列号
 *      [in]opt     选项
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功则返回0 ， 失败返回-1/-2
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_queue_del(int qnum,int opt, unsigned int *err)
{
    int ret=0 ;
    if(rts_msgq[qnum].Type != OS_OBJ_TYPE_Q) {
        return -1;
    }

    OSQDel(&rts_msgq[qnum],opt,err);

    if(*err != OS_ERR_NONE)
        return -2;
    else
        return ret;
}

/*
 *
 * 发送消息
 *
 * Description: 向消息队列发送消息
 *
 * Arguments:
 *      [in]qnum    消息队列号
 *      [in]buf     要发送的消息
 *      [in]size    消息长度
 *      [in]opt     选项
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功则返回0 ， 失败返回-1/-2
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_queue_post(int qnum,char *buf,int size,int opt, unsigned int *err)
{
    if(qnum <0||qnum >QUEUS_NUM ||rts_msgq[qnum].Type != OS_OBJ_TYPE_Q) {
        return -1;
    }

    OSQPost(&rts_msgq[qnum],buf,size,opt,err);
    if (*err != OS_ERR_NONE)
        return -2;
    else
        return 0;
}

/*
 *
 * 接收消息
 *
 * Description: 从消息队列接收消息
 *
 * Arguments:
 *      [in]qnum    消息队列号
 *      [out]buf    要发送的消息
 *      [out]ts     收到消息的时间戳
 *      [out]size   收到的消息长度
 *      [in]opt     选项
 *      [in]timeout 接收消息的超时时间(为0 则一直等待)
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功则返回0 ， 失败返回-1/-2
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_queue_get(int qnum,char *buf,int *size,int opt,int timeout, unsigned int *err)
{
    if(qnum <0||qnum >QUEUS_NUM ||rts_msgq[qnum].Type != OS_OBJ_TYPE_Q) {
        return -1;
    }

    buf=OSQPend(&rts_msgq[qnum],timeout,opt,size,NULL,err);
    if(*err != OS_ERR_NONE)
        return -2;
    else
        return 0;
}

/*
 *
 * 创建信号量
 *
 * Description: 创建一个指定大小的信号量
 *
 * Arguments:
 *      [in]sname   信号量名
 *      [in]cnt     信号量初始值
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功则返回信号量id ， 失败返回-1/-2
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_sem_create(char *sname, unsigned int cnt,unsigned int *err)
{
    for (int i=0; i<SEM_NUM; i++) {
        if(rts_sem[i].Type == OS_OBJ_TYPE_NONE) {
            OSSemCreate(&rts_sem[i],sname,cnt,err);
            if (*err != OS_ERR_NONE)
                return -1;
            else
                return i;
        }
    }
    return -2;
}

/*
 *
 * 删除信号量
 *
 * Description: 删除一个信号量
 *
 * Arguments:
 *      [in]sid     信号量 id
 *      [in]opt     选项
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功则返回0 ， 失败返回-1/-2
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_sem_del(int sid,unsigned short opt,unsigned int *err)
{
    if(sid>SEM_NUM || sid <0 || rts_sem[sid].Type != OS_OBJ_TYPE_SEM)
        return -2;

    OSSemDel(&rts_sem[sid],opt,err);
    if (*err != OS_ERR_NONE)
        return -1;
    else
        return 0;
}

/*
 *
 * 等待信号
 *
 * Description: 等待信号量可用
 *
 * Arguments:
 *      [in]sid     信号量 id
 *      [in]timtout 等待超时时间
 *      [in]opt     选项
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功则返回0 ， 失败返回-1/-2
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_sem_pend(int sid,unsigned int timeout,unsigned short opt,unsigned int *err)
{
    if(sid>SEM_NUM || sid <0 || rts_sem[sid].Type != OS_OBJ_TYPE_SEM)
        return -2;
    OSSemPend(&rts_sem[sid],timeout,opt,NULL,err);

    if (*err != OS_ERR_NONE)
	{
        return -1;
	}
    else
        return 0;
}

/*
 *
 * 发送信号
 *
 * Description: 发送一个信号量
 *
 * Arguments:
 *      [in]sid     信号量 id
 *      [in]opt     选项
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功则返回0 ， 失败返回-1/-2
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_sem_post(int sid, unsigned short opt, unsigned int *err)
{
    if(sid>SEM_NUM || sid <0 || rts_sem[sid].Type != OS_OBJ_TYPE_SEM)
        return -2;
    OSSemPost(&rts_sem[sid],opt,err);
    if (*err != OS_ERR_NONE)
	{
        return -1;
	}
    else
        return 0;
}

/*
 *
 * 创建互斥量
 *
 * Description: 创建一个互斥量
 *
 * Arguments:
 *      [in]mname   互斥量名
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功则返回互斥量 id ， 失败返回-1/-2
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_mutex_create(char *mname,unsigned int *err)
{
    for (int i=0; i<MUT_NUM; i++) {
        if(rts_sem[i].Type == OS_OBJ_TYPE_NONE) {
            OSMutexCreate(&rts_mutex[i],mname,err);
            if (*err != OS_ERR_NONE)
                return -1;
            else
                return i;
        }
    }
    return -2;
}

/*
 *
 * 删除互斥量
 *
 * Description: 删除一个互斥量
 *
 * Arguments:
 *      [in]mid     互斥量id
 *      [in]opt     选项
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功则返回0 ， 失败返回-1/-2
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_mutex_del(int mid, unsigned short opt,unsigned int *err)
{
    if(mid>MUT_NUM || mid <0 || rts_mutex[mid].Type != OS_OBJ_TYPE_MUTEX)
        return -2;
    OSMutexDel(&rts_mutex[mid],opt,err);
    if (*err != OS_ERR_NONE)
        return -1;
    else
        return 0;
}

/*
 *
 * 申请互斥量
 *
 * Description: 申请互斥量
 *
 * Arguments:
 *      [in]mid     互斥量id
 *      [in]timeout 等待超时时间
 *      [in]opt     选项
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功则返回0 ， 失败返回-1/-2
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_mutex_pend(int mid,unsigned int timeout,unsigned short opt,unsigned int *err)
{
    if(mid>MUT_NUM || mid <0 || rts_mutex[mid].Type != OS_OBJ_TYPE_MUTEX)
        return -2;
    OSMutexPend(&rts_mutex[mid],timeout,opt,NULL,err);
    if (*err != OS_ERR_NONE)
        return -1;
    else
        return 0;
}

/*
 *
 * 释放互斥量
 *
 * Description: 释放互斥量
 *
 * Arguments:
 *      [in]mid     互斥量id
 *      [in]opt     选项
 *      [out]err    返回操作错误码
 *
 * Returns:
 *      成功则返回0 ， 失败返回-1/-2
 *
 * Note(s):
 *      出错时可以根据 err 的值查找失败原因
 *
 */
int os_mutex_post(int mid,unsigned short opt,unsigned int *err)
{
    if(mid>MUT_NUM || mid <0 || rts_mutex[mid].Type != OS_OBJ_TYPE_MUTEX)
        return -2;
    OSMutexPost(&rts_mutex[mid],opt,err);
    if (*err != OS_ERR_NONE)
        return -1;
    else
        return 0;
}
