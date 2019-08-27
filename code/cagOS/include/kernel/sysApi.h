#ifndef __OS_TASK_H__
#define __OS_TASK_H__
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

/*os api*/
char *os_version(void);

INT32 os_create_task(char *pname, void *ptask, void *parg, INT32 prio,UINT32 tq, UINT32 *err);
INT32 os_del_task(INT32 id, UINT32 *err);
INT32 os_resume_task(INT32 id, UINT32 *err);
INT32 os_suspend_task(INT32 id, UINT32 *err);
INT32 os_task_set_ts(INT32 id,UINT32 time_slice, UINT32 *err);
void os_delay_task(INT32 ticks, UINT32 *err);
void udelay(INT32 us);

#if 0
INT32 os_task_sem_pend(INT32 timeout, UINT16 opt,UINT32 *err);
INT32 os_task_sem_post(INT32 id, UINT16 opt, UINT32 *err);
INT32 os_task_sem_set(INT32 id,UINT32 cnt, UINT32 *err);
#endif /*no use*/

INT32 os_queue_create(char *qname,INT32 qsize, UINT32 *err);
INT32 os_queue_del(INT32 qnum,INT32 opt, UINT32 *err);
INT32 os_queue_post(INT32 qnum,char *buf,INT32 size,INT32 opt, UINT32 *err);
INT32 os_queue_get(INT32 qnum,char *buf,UINT16 *size,INT32 opt,INT32 timeout, UINT32 *err);

INT32 os_sem_create(char *sname, UINT32 cnt,UINT32 *err);
INT32 os_sem_del(INT32 sid,UINT16 opt,UINT32 *err);
UINT32 os_sem_pend(INT32 sid,UINT32 timeout,UINT16 opt,UINT32 *err);
UINT32 os_sem_post(INT32 sid, UINT16 opt, UINT32 *err);

INT32 os_mutex_create(char *mname,UINT32 *err);
INT32 os_mutex_del(INT32 mid, UINT16 opt,UINT32 *err);
INT32 os_mutex_pend(INT32 mid,UINT32 timeout,UINT16 opt,UINT32 *err);
INT32 os_mutex_post(INT32 mid,UINT16 opt,UINT32 *err);

void os_copy_data(UINT32 from, INT32 size);
void os_clear_data(UINT32 from, INT32 size);
#endif
