-- VectorCAST 6.4d (02/29/16)
-- Test Case Script
-- 
-- Environment    : THREAD1
-- Unit(s) Under Test: sysApi sysCfg sysCore sysMsg sysMutex sysPrio sysQueue sysSem sysStat sysTask sysTick sysTime
-- 
-- Script Features
TEST.SCRIPT_FEATURE:C_DIRECT_ARRAY_INDEXING
TEST.SCRIPT_FEATURE:CPP_CLASS_OBJECT_REVISION
TEST.SCRIPT_FEATURE:MULTIPLE_UUT_SUPPORT
TEST.SCRIPT_FEATURE:MIXED_CASE_NAMES
TEST.SCRIPT_FEATURE:STATIC_HEADER_FUNCS_IN_UUTS
--

-- Unit: sysApi

-- Subprogram: os_create_task

-- Test Case: createtask1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_create_task
TEST.NEW
TEST.NAME:createtask1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (prio >= 10) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_create_task.pname:<<malloc 1>>
TEST.VALUE:sysApi.os_create_task.ptask:VECTORCAST_INT1
TEST.VALUE:sysApi.os_create_task.parg:VECTORCAST_INT1
TEST.VALUE:sysApi.os_create_task.prio:<<MAX>>
TEST.VALUE:sysApi.os_create_task.tq:<<MIN>>
TEST.VALUE:sysApi.os_create_task.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_create_task.return:-2
TEST.END

-- Test Case: createtask2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_create_task
TEST.NEW
TEST.NAME:createtask2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (prio >= 10) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_create_task.pname:<<malloc 1>>
TEST.VALUE:sysApi.os_create_task.ptask:VECTORCAST_INT1
TEST.VALUE:sysApi.os_create_task.parg:VECTORCAST_INT1
TEST.VALUE:sysApi.os_create_task.prio:10
TEST.VALUE:sysApi.os_create_task.tq:<<MIN>>
TEST.VALUE:sysApi.os_create_task.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_create_task.return:-2
TEST.END

-- Test Case: createtask3
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_create_task
TEST.NEW
TEST.NAME:createtask3
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (prio >= 10) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_create_task.pname:<<malloc 1>>
TEST.VALUE:sysApi.os_create_task.ptask:VECTORCAST_INT1
TEST.VALUE:sysApi.os_create_task.parg:VECTORCAST_INT1
TEST.VALUE:sysApi.os_create_task.prio:<<MIN>>
TEST.VALUE:sysApi.os_create_task.tq:<<MIN>>
TEST.VALUE:sysApi.os_create_task.err:<<malloc 1>>
TEST.VALUE:sysApi.os_create_task.err[0]:1
TEST.EXPECTED:sysApi.os_create_task.return:-1
TEST.END

-- Test Case: createtask4
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_create_task
TEST.NEW
TEST.NAME:createtask4
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (prio >= 10) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_create_task.pname:<<malloc 1>>
TEST.VALUE:sysApi.os_create_task.ptask:VECTORCAST_INT1
TEST.VALUE:sysApi.os_create_task.parg:VECTORCAST_INT1
TEST.VALUE:sysApi.os_create_task.prio:<<MIN>>
TEST.VALUE:sysApi.os_create_task.tq:<<MIN>>
TEST.VALUE:sysApi.os_create_task.err:<<malloc 1>>
TEST.VALUE:sysApi.os_create_task.err[0]:0
TEST.EXPECTED:sysApi.os_create_task.return:0
TEST.END

-- Subprogram: os_del_task

-- Test Case: osdeltask1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_del_task
TEST.NEW
TEST.NAME:osdeltask1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (prio == 0xffff) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysTask.OSTaskDel
TEST.VALUE:sysApi.os_del_task.prio:<<MIN>>
TEST.VALUE:sysApi.os_del_task.err:<<malloc 1>>
TEST.VALUE:sysApi.os_del_task.err[0]:1
TEST.EXPECTED:sysApi.os_del_task.return:-1
TEST.END

-- Test Case: osdeltask2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_del_task
TEST.NEW
TEST.NAME:osdeltask2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (prio == 0xffff) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_del_task.prio:<<MIN>>
TEST.VALUE:sysApi.os_del_task.err:<<malloc 1>>
TEST.VALUE:sysApi.os_del_task.err[0]:0
TEST.EXPECTED:sysApi.os_del_task.return:0
TEST.END

-- Test Case: osdeltask3
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_del_task
TEST.NEW
TEST.NAME:osdeltask3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (prio == 0xffff) ==> TRUE
      (2) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysTask.OSTaskDel
TEST.VALUE:sysApi.os_del_task.prio:65535
TEST.VALUE:sysApi.os_del_task.err:<<malloc 1>>
TEST.VALUE:sysApi.os_del_task.err[0]:1
TEST.EXPECTED:sysApi.os_del_task.return:-1
TEST.END

-- Test Case: osdeltask4
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_del_task
TEST.NEW
TEST.NAME:osdeltask4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (prio == 0xffff) ==> TRUE
      (2) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysTask.OSTaskDel
TEST.VALUE:sysApi.os_del_task.prio:65535
TEST.VALUE:sysApi.os_del_task.err:<<malloc 1>>
TEST.VALUE:sysApi.os_del_task.err[0]:0
TEST.EXPECTED:sysApi.os_del_task.return:0
TEST.END

-- Subprogram: os_delay_task

-- Test Case: delay_tsk1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_delay_task
TEST.NEW
TEST.NAME:delay_tsk1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:sysTime.OSTimeDly
TEST.VALUE:sysApi.os_delay_task.dly:<<MAX>>
TEST.VALUE:sysApi.os_delay_task.err:<<malloc 1>>
TEST.VALUE:sysApi.os_delay_task.err[0]:1
TEST.END

-- Subprogram: os_mutex_create

-- Test Case: mutex_Create3
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_create
TEST.NEW
TEST.NAME:mutex_Create3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) for (i < 20) ==> TRUE
      (2) if (((rts_sem)[i]).Type == (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)78) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)79) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)78) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)69))) ==> TRUE
      (3) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable i in branch 1
      Cannot set local variable i in branch 2
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[0].Type:1313820229
TEST.VALUE:sysApi.os_mutex_create.mname:<<malloc 1>>
TEST.VALUE:sysApi.os_mutex_create.err:<<malloc 1>>
TEST.VALUE:sysApi.os_mutex_create.err[0]:0
TEST.EXPECTED:sysApi.os_mutex_create.return:0
TEST.END

-- Test Case: mutex_create1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_create
TEST.NEW
TEST.NAME:mutex_create1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for (i < 20) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable i in branch 1
TEST.END_NOTES:
TEST.VALUE:sysApi.os_mutex_create.mname:<<malloc 1>>
TEST.VALUE:sysApi.os_mutex_create.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_mutex_create.return:-2
TEST.END

-- Test Case: mutex_create2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_create
TEST.NEW
TEST.NAME:mutex_create2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) for (i < 20) ==> TRUE
      (2) if (((rts_sem)[i]).Type == (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)78) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)79) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)78) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)69))) ==> TRUE
      (3) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable i in branch 1
      Cannot set local variable i in branch 2
TEST.END_NOTES:
TEST.STUB:sysMutex.OSMutexCreate
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[0].Type:1313820229
TEST.VALUE:sysApi.os_mutex_create.mname:<<malloc 1>>
TEST.VALUE:sysApi.os_mutex_create.err:<<malloc 1>>
TEST.VALUE:sysApi.os_mutex_create.err[0]:1
TEST.EXPECTED:sysApi.os_mutex_create.return:-1
TEST.END

-- Test Case: mutex_create4
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_create
TEST.NEW
TEST.NAME:mutex_create4
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) for (i < 20) ==> TRUE
      (2) if (((rts_sem)[i]).Type == (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)78) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)79) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)78) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)69))) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable i in branch 1
      Cannot set local variable i in branch 2
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[0].Type:<<MIN>>
TEST.VALUE:sysApi.os_mutex_create.mname:<<malloc 1>>
TEST.VALUE:sysApi.os_mutex_create.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_mutex_create.return:-2
TEST.END

-- Test Case: mutex_create5
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_create
TEST.NEW
TEST.NAME:mutex_create5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) for (i < 20) ==> TRUE
      (2) if (((rts_sem)[i]).Type == (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)78) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)79) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)78) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)69))) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable i in branch 1
      Cannot set local variable i in branch 2
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[0].Type:<<MIN>>
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[19].Type:1313820229
TEST.VALUE:sysApi.os_mutex_create.mname:<<malloc 1>>
TEST.VALUE:sysApi.os_mutex_create.err:<<malloc 1>>
TEST.VALUE:sysApi.os_mutex_create.err[0]:0
TEST.EXPECTED:sysApi.os_mutex_create.return:19
TEST.END

-- Subprogram: os_mutex_del

-- Test Case: mutex_del1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_del
TEST.NEW
TEST.NAME:mutex_del1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_mutex_del.mid:<<MAX>>
TEST.VALUE:sysApi.os_mutex_del.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_del.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_mutex_del.return:-2
TEST.END

-- Test Case: mutex_del1.001
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_del
TEST.NEW
TEST.NAME:mutex_del1.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_mutex_del.mid:<<MIN>>
TEST.VALUE:sysApi.os_mutex_del.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_del.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_mutex_del.return:-2
TEST.END

-- Test Case: mutex_del1.002
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_del
TEST.NEW
TEST.NAME:mutex_del1.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_mutex[2].Type:12
TEST.VALUE:sysApi.os_mutex_del.mid:2
TEST.VALUE:sysApi.os_mutex_del.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_del.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_mutex_del.return:-2
TEST.END

-- Test Case: mutex_del2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_del
TEST.NEW
TEST.NAME:mutex_del2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysMutex.OSMutexDel
TEST.VALUE:sysApi.<<GLOBAL>>.rts_mutex[0].Type:1297437784
TEST.VALUE:sysApi.os_mutex_del.mid:0
TEST.VALUE:sysApi.os_mutex_del.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_del.err:<<malloc 1>>
TEST.VALUE:sysApi.os_mutex_del.err[0]:1
TEST.EXPECTED:sysApi.os_mutex_del.return:-1
TEST.END

-- Test Case: mutex_del3
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_del
TEST.NEW
TEST.NAME:mutex_del3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_mutex[0].Type:1297437784
TEST.VALUE:sysApi.os_mutex_del.mid:0
TEST.VALUE:sysApi.os_mutex_del.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_del.err:<<malloc 1>>
TEST.VALUE:sysApi.os_mutex_del.err[0]:0
TEST.EXPECTED:sysApi.os_mutex_del.return:0
TEST.END

-- Subprogram: os_mutex_pend

-- Test Case: mutex_pend1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_pend
TEST.NEW
TEST.NAME:mutex_pend1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_mutex_pend.mid:<<MAX>>
TEST.VALUE:sysApi.os_mutex_pend.timeout:<<MIN>>
TEST.VALUE:sysApi.os_mutex_pend.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_pend.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_mutex_pend.return:-2
TEST.END

-- Test Case: mutex_pend1.001
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_pend
TEST.NEW
TEST.NAME:mutex_pend1.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_mutex_pend.mid:<<MIN>>
TEST.VALUE:sysApi.os_mutex_pend.timeout:<<MIN>>
TEST.VALUE:sysApi.os_mutex_pend.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_pend.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_mutex_pend.return:-2
TEST.END

-- Test Case: mutex_pend1.002
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_pend
TEST.NEW
TEST.NAME:mutex_pend1.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_mutex[3].Type:23
TEST.VALUE:sysApi.<<GLOBAL>>.rts_mutex[3].MutexGrpNextPtr:<<null>>
TEST.VALUE:sysApi.os_mutex_pend.mid:3
TEST.VALUE:sysApi.os_mutex_pend.timeout:<<MIN>>
TEST.VALUE:sysApi.os_mutex_pend.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_pend.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_mutex_pend.return:-2
TEST.END

-- Test Case: mutex_pend2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_pend
TEST.NEW
TEST.NAME:mutex_pend2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysMutex.OSMutexPend
TEST.VALUE:sysApi.<<GLOBAL>>.rts_mutex[0].Type:1297437784
TEST.VALUE:sysApi.os_mutex_pend.mid:0
TEST.VALUE:sysApi.os_mutex_pend.timeout:<<MIN>>
TEST.VALUE:sysApi.os_mutex_pend.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_pend.err:<<malloc 1>>
TEST.VALUE:sysApi.os_mutex_pend.err[0]:1
TEST.EXPECTED:sysApi.os_mutex_pend.return:-1
TEST.END

-- Test Case: mutex_pend3
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_pend
TEST.NEW
TEST.NAME:mutex_pend3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysMutex.OSMutexPend
TEST.VALUE:sysApi.<<GLOBAL>>.rts_mutex[0].Type:1297437784
TEST.VALUE:sysApi.os_mutex_pend.mid:0
TEST.VALUE:sysApi.os_mutex_pend.timeout:<<MIN>>
TEST.VALUE:sysApi.os_mutex_pend.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_pend.err:<<malloc 1>>
TEST.VALUE:sysApi.os_mutex_pend.err[0]:0
TEST.EXPECTED:sysApi.os_mutex_pend.return:0
TEST.END

-- Subprogram: os_mutex_post

-- Test Case: mutex_post1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_post
TEST.NEW
TEST.NAME:mutex_post1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_mutex_post.mid:<<MAX>>
TEST.VALUE:sysApi.os_mutex_post.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_post.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_mutex_post.return:-2
TEST.END

-- Test Case: mutex_post11
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_post
TEST.NEW
TEST.NAME:mutex_post11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_mutex_post.mid:<<MIN>>
TEST.VALUE:sysApi.os_mutex_post.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_post.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_mutex_post.return:-2
TEST.END

-- Test Case: mutex_post11.001
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_post
TEST.NEW
TEST.NAME:mutex_post11.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_mutex[19].Type:12
TEST.VALUE:sysApi.os_mutex_post.mid:19
TEST.VALUE:sysApi.os_mutex_post.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_post.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_mutex_post.return:-2
TEST.END

-- Test Case: mutex_post2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_post
TEST.NEW
TEST.NAME:mutex_post2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_mutex[0].Type:1297437784
TEST.VALUE:sysApi.os_mutex_post.mid:0
TEST.VALUE:sysApi.os_mutex_post.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_post.err:<<malloc 1>>
TEST.VALUE:sysApi.os_mutex_post.err[0]:1
TEST.EXPECTED:sysApi.os_mutex_post.return:-1
TEST.END

-- Test Case: mutex_post3
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_mutex_post
TEST.NEW
TEST.NAME:mutex_post3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((mid > 20 || mid < 0) || ((rts_mutex)[mid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)77) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)84) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)88))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysMutex.OSMutexPost
TEST.VALUE:sysApi.<<GLOBAL>>.rts_mutex[0].Type:1297437784
TEST.VALUE:sysApi.os_mutex_post.mid:0
TEST.VALUE:sysApi.os_mutex_post.opt:<<MIN>>
TEST.VALUE:sysApi.os_mutex_post.err:<<malloc 1>>
TEST.VALUE:sysApi.os_mutex_post.err[0]:0
TEST.EXPECTED:sysApi.os_mutex_post.return:0
TEST.END

-- Subprogram: os_queue_create

-- Test Case: queue_create1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_create
TEST.NEW
TEST.NAME:queue_create1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for (i < 20) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable i in branch 1
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_msgq[1].Type:0
TEST.VALUE:sysApi.os_queue_create.qname:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_create.qsize:<<MIN>>
TEST.VALUE:sysApi.os_queue_create.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_queue_create.return:-1
TEST.END

-- Test Case: queue_create2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_create
TEST.NEW
TEST.NAME:queue_create2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) for (i < 20) ==> TRUE
      (2) if (((rts_msgq)[i]).Type == (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)78) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)79) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)78) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)69))) ==> TRUE
      (3) if (*err == (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable i in branch 1
      Cannot set local variable i in branch 2
TEST.END_NOTES:
TEST.STUB:sysQueue.OSQCreate
TEST.VALUE:sysApi.<<GLOBAL>>.rts_msgq[0].Type:1313820229
TEST.VALUE:sysApi.os_queue_create.qname:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_create.qsize:<<MIN>>
TEST.VALUE:sysApi.os_queue_create.err:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_create.err[0]:0
TEST.EXPECTED:sysApi.os_queue_create.return:0
TEST.END

-- Test Case: queue_create3
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_create
TEST.NEW
TEST.NAME:queue_create3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) for (i < 20) ==> TRUE
      (2) if (((rts_msgq)[i]).Type == (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)78) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)79) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)78) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)69))) ==> TRUE
      (3) if (*err == (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable i in branch 1
      Cannot set local variable i in branch 2
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_msgq[0].Type:1313820229
TEST.VALUE:sysApi.os_queue_create.qname:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_create.qsize:<<MIN>>
TEST.VALUE:sysApi.os_queue_create.err:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_create.err[0]:1
TEST.EXPECTED:sysApi.os_queue_create.return:-2
TEST.END

-- Test Case: queue_create4
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_create
TEST.NEW
TEST.NAME:queue_create4
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) for (i < 20) ==> TRUE
      (2) if (((rts_msgq)[i]).Type == (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)78) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)79) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)78) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)69))) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable i in branch 1
      Cannot set local variable i in branch 2
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_msgq[0].Type:<<MIN>>
TEST.VALUE:sysApi.os_queue_create.qname:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_create.qsize:<<MIN>>
TEST.VALUE:sysApi.os_queue_create.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_queue_create.return:-1
TEST.END

-- Subprogram: os_queue_del

-- Test Case: queue_del1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_del
TEST.NEW
TEST.NAME:queue_del1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((rts_msgq)[qnum]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)81) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)69) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)85))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_msgq[0].Type:<<MIN>>
TEST.VALUE:sysApi.os_queue_del.qnum:0
TEST.VALUE:sysApi.os_queue_del.opt:<<MIN>>
TEST.VALUE:sysApi.os_queue_del.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_queue_del.return:-1
TEST.END

-- Test Case: queue_del2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_del
TEST.NEW
TEST.NAME:queue_del2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (((rts_msgq)[qnum]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)81) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)69) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)85))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysQueue.OSQDel
TEST.VALUE:sysApi.<<GLOBAL>>.rts_msgq[0].Type:1364542805
TEST.VALUE:sysApi.os_queue_del.qnum:0
TEST.VALUE:sysApi.os_queue_del.opt:<<MIN>>
TEST.VALUE:sysApi.os_queue_del.err:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_del.err[0]:1
TEST.EXPECTED:sysApi.os_queue_del.return:-2
TEST.END

-- Test Case: queue_del3
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_del
TEST.NEW
TEST.NAME:queue_del3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (((rts_msgq)[qnum]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)81) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)69) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)85))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_msgq[0].Type:1364542805
TEST.VALUE:sysApi.os_queue_del.qnum:0
TEST.VALUE:sysApi.os_queue_del.opt:<<MIN>>
TEST.VALUE:sysApi.os_queue_del.err:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_del.err[0]:0
TEST.EXPECTED:sysApi.os_queue_del.return:0
TEST.END

-- Subprogram: os_queue_get

-- Test Case: queue_get1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_get
TEST.NEW
TEST.NAME:queue_get1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((qnum < 0 || qnum > 20) || ((rts_msgq)[qnum]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)81) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)69) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)85))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_queue_get.qnum:<<MIN>>
TEST.VALUE:sysApi.os_queue_get.buf:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_get.size:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_get.opt:<<MIN>>
TEST.VALUE:sysApi.os_queue_get.timeout:<<MIN>>
TEST.VALUE:sysApi.os_queue_get.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_queue_get.return:-1
TEST.END

-- Test Case: queue_get1.001
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_get
TEST.NEW
TEST.NAME:queue_get1.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((qnum < 0 || qnum > 20) || ((rts_msgq)[qnum]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)81) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)69) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)85))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_queue_get.qnum:20
TEST.VALUE:sysApi.os_queue_get.buf:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_get.size:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_get.opt:<<MIN>>
TEST.VALUE:sysApi.os_queue_get.timeout:<<MIN>>
TEST.VALUE:sysApi.os_queue_get.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_queue_get.return:-1
TEST.END

-- Test Case: queue_get1.002
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_get
TEST.NEW
TEST.NAME:queue_get1.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((qnum < 0 || qnum > 20) || ((rts_msgq)[qnum]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)81) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)69) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)85))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_queue_get.qnum:<<MAX>>
TEST.VALUE:sysApi.os_queue_get.buf:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_get.size:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_get.opt:<<MIN>>
TEST.VALUE:sysApi.os_queue_get.timeout:<<MIN>>
TEST.VALUE:sysApi.os_queue_get.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_queue_get.return:-1
TEST.END

-- Test Case: queue_get1.003
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_get
TEST.NEW
TEST.NAME:queue_get1.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((qnum < 0 || qnum > 20) || ((rts_msgq)[qnum]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)81) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)69) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)85))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_msgq[1].Type:1
TEST.VALUE:sysApi.os_queue_get.qnum:1
TEST.VALUE:sysApi.os_queue_get.buf:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_get.size:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_get.opt:<<MIN>>
TEST.VALUE:sysApi.os_queue_get.timeout:<<MIN>>
TEST.VALUE:sysApi.os_queue_get.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_queue_get.return:-1
TEST.END

-- Test Case: queue_get2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_get
TEST.NEW
TEST.NAME:queue_get2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((qnum < 0 || qnum > 20) || ((rts_msgq)[qnum]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)81) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)69) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)85))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_msgq[0].Type:1364542805
TEST.VALUE:sysApi.os_queue_get.qnum:0
TEST.VALUE:sysApi.os_queue_get.buf:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_get.size:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_get.opt:<<MIN>>
TEST.VALUE:sysApi.os_queue_get.timeout:<<MIN>>
TEST.VALUE:sysApi.os_queue_get.err:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_get.err[0]:1
TEST.EXPECTED:sysApi.os_queue_get.return:-2
TEST.END

-- Test Case: queue_get3
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_get
TEST.NEW
TEST.NAME:queue_get3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((qnum < 0 || qnum > 20) || ((rts_msgq)[qnum]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)81) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)69) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)85))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_msgq[0].Type:1364542805
TEST.VALUE:sysApi.os_queue_get.qnum:0
TEST.VALUE:sysApi.os_queue_get.buf:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_get.size:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_get.opt:<<MIN>>
TEST.VALUE:sysApi.os_queue_get.timeout:<<MIN>>
TEST.VALUE:sysApi.os_queue_get.err:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_get.err[0]:0
TEST.EXPECTED:sysApi.os_queue_get.return:0
TEST.END

-- Subprogram: os_queue_post

-- Test Case: queue_post1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_post
TEST.NEW
TEST.NAME:queue_post1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((qnum < 0 || qnum > 20) || ((rts_msgq)[qnum]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)81) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)69) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)85))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_queue_post.qnum:<<MIN>>
TEST.VALUE:sysApi.os_queue_post.buf:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_post.size:<<MIN>>
TEST.VALUE:sysApi.os_queue_post.opt:<<MIN>>
TEST.VALUE:sysApi.os_queue_post.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_queue_post.return:-1
TEST.END

-- Test Case: queue_post2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_post
TEST.NEW
TEST.NAME:queue_post2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((qnum < 0 || qnum > 20) || ((rts_msgq)[qnum]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)81) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)69) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)85))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_queue_post.qnum:20
TEST.VALUE:sysApi.os_queue_post.buf:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_post.size:<<MIN>>
TEST.VALUE:sysApi.os_queue_post.opt:<<MIN>>
TEST.VALUE:sysApi.os_queue_post.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_queue_post.return:-1
TEST.END

-- Test Case: queue_post3
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_post
TEST.NEW
TEST.NAME:queue_post3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((qnum < 0 || qnum > 20) || ((rts_msgq)[qnum]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)81) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)69) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)85))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_msgq[1].Type:1
TEST.VALUE:sysApi.os_queue_post.qnum:1
TEST.VALUE:sysApi.os_queue_post.buf:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_post.size:<<MIN>>
TEST.VALUE:sysApi.os_queue_post.opt:<<MIN>>
TEST.VALUE:sysApi.os_queue_post.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_queue_post.return:-1
TEST.END

-- Test Case: queue_post4
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_post
TEST.NEW
TEST.NAME:queue_post4
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((qnum < 0 || qnum > 20) || ((rts_msgq)[qnum]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)81) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)69) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)85))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_msgq[0].Type:1364542805
TEST.VALUE:sysApi.os_queue_post.qnum:0
TEST.VALUE:sysApi.os_queue_post.buf:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_post.size:<<MIN>>
TEST.VALUE:sysApi.os_queue_post.opt:<<MIN>>
TEST.VALUE:sysApi.os_queue_post.err:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_post.err[0]:1
TEST.EXPECTED:sysApi.os_queue_post.return:-2
TEST.END

-- Test Case: queue_post5
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_queue_post
TEST.NEW
TEST.NAME:queue_post5
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((qnum < 0 || qnum > 20) || ((rts_msgq)[qnum]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)81) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)85) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)69) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)85))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysQueue.OSQPost
TEST.VALUE:sysApi.<<GLOBAL>>.rts_msgq[0].Type:1364542805
TEST.VALUE:sysApi.os_queue_post.qnum:0
TEST.VALUE:sysApi.os_queue_post.buf:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_post.size:<<MIN>>
TEST.VALUE:sysApi.os_queue_post.opt:<<MIN>>
TEST.VALUE:sysApi.os_queue_post.err:<<malloc 1>>
TEST.VALUE:sysApi.os_queue_post.err[0]:0
TEST.EXPECTED:sysApi.os_queue_post.return:0
TEST.END

-- Subprogram: os_resume_task

-- Test Case: resumetask1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_resume_task
TEST.NEW
TEST.NAME:resumetask1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_resume_task.prio:<<MIN>>
TEST.VALUE:sysApi.os_resume_task.err:<<malloc 1>>
TEST.VALUE:sysApi.os_resume_task.err[0]:1
TEST.EXPECTED:sysApi.os_resume_task.return:-1
TEST.END

-- Test Case: resumetask2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_resume_task
TEST.NEW
TEST.NAME:resumetask2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysTask.OSTaskResume
TEST.VALUE:sysApi.os_resume_task.prio:<<MIN>>
TEST.VALUE:sysApi.os_resume_task.err:<<malloc 1>>
TEST.VALUE:sysApi.os_resume_task.err[0]:0
TEST.EXPECTED:sysApi.os_resume_task.return:0
TEST.END

-- Subprogram: os_sem_create

-- Test Case: sem_create1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_create
TEST.NEW
TEST.NAME:sem_create1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for (i < 20) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable i in branch 1
TEST.END_NOTES:
TEST.VALUE:sysApi.os_sem_create.sname:<<malloc 9>>
TEST.VALUE:sysApi.os_sem_create.sname:<<null>>
TEST.VALUE:sysApi.os_sem_create.cnt:<<MIN>>
TEST.VALUE:sysApi.os_sem_create.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_sem_create.return:-2
TEST.END

-- Test Case: sem_create2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_create
TEST.NEW
TEST.NAME:sem_create2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) for (i < 20) ==> TRUE
      (2) if (((rts_sem)[i]).Type == (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)78) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)79) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)78) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)69))) ==> TRUE
      (3) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable i in branch 1
      Cannot set local variable i in branch 2
TEST.END_NOTES:
TEST.STUB:sysSem.OSSemCreate
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[0].Type:1313820229
TEST.VALUE:sysApi.os_sem_create.sname:<<malloc 1>>
TEST.VALUE:sysApi.os_sem_create.cnt:<<MIN>>
TEST.VALUE:sysApi.os_sem_create.err:<<malloc 1>>
TEST.VALUE:sysApi.os_sem_create.err[0]:1
TEST.EXPECTED:sysApi.os_sem_create.return:-1
TEST.END

-- Test Case: sem_create3
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_create
TEST.NEW
TEST.NAME:sem_create3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) for (i < 20) ==> TRUE
      (2) if (((rts_sem)[i]).Type == (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)78) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)79) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)78) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)69))) ==> TRUE
      (3) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable i in branch 1
      Cannot set local variable i in branch 2
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[0].Type:1313820229
TEST.VALUE:sysApi.os_sem_create.sname:<<malloc 1>>
TEST.VALUE:sysApi.os_sem_create.cnt:<<MIN>>
TEST.VALUE:sysApi.os_sem_create.err:<<malloc 1>>
TEST.VALUE:sysApi.os_sem_create.err[0]:0
TEST.EXPECTED:sysApi.os_sem_create.return:0
TEST.END

-- Test Case: sem_create4
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_create
TEST.NEW
TEST.NAME:sem_create4
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) for (i < 20) ==> TRUE
      (2) if (((rts_sem)[i]).Type == (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)78) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)79) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)78) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)69))) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable i in branch 1
      Cannot set local variable i in branch 2
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[0].Type:<<MIN>>
TEST.VALUE:sysApi.os_sem_create.sname:<<malloc 1>>
TEST.VALUE:sysApi.os_sem_create.cnt:<<MIN>>
TEST.VALUE:sysApi.os_sem_create.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_sem_create.return:-2
TEST.END

-- Test Case: sem_create5
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_create
TEST.NEW
TEST.NAME:sem_create5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) for (i < 20) ==> TRUE
      (2) if (((rts_sem)[i]).Type == (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)78) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)79) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)78) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)69))) ==> TRUE
      (3) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable i in branch 1
      Cannot set local variable i in branch 2
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[0].Type:1313820228
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[19].Type:1313820229
TEST.VALUE:sysApi.os_sem_create.sname:<<malloc 1>>
TEST.VALUE:sysApi.os_sem_create.cnt:<<MIN>>
TEST.VALUE:sysApi.os_sem_create.err:<<malloc 1>>
TEST.VALUE:sysApi.os_sem_create.err[0]:0
TEST.EXPECTED:sysApi.os_sem_create.return:19
TEST.END

-- Subprogram: os_sem_del

-- Test Case: sem_del1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_del
TEST.NEW
TEST.NAME:sem_del1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((sid > 20 || sid < 0) || ((rts_sem)[sid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)83) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)69) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)77) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)65))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_sem_del.sid:<<MAX>>
TEST.VALUE:sysApi.os_sem_del.opt:<<MIN>>
TEST.VALUE:sysApi.os_sem_del.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_sem_del.return:-2
TEST.END

-- Test Case: sem_del11
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_del
TEST.NEW
TEST.NAME:sem_del11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((sid > 20 || sid < 0) || ((rts_sem)[sid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)83) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)69) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)77) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)65))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_sem_del.sid:<<MIN>>
TEST.VALUE:sysApi.os_sem_del.opt:<<MIN>>
TEST.VALUE:sysApi.os_sem_del.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_sem_del.return:-2
TEST.END

-- Test Case: sem_del12
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_del
TEST.NEW
TEST.NAME:sem_del12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((sid > 20 || sid < 0) || ((rts_sem)[sid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)83) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)69) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)77) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)65))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[1].Type:1
TEST.VALUE:sysApi.os_sem_del.sid:1
TEST.VALUE:sysApi.os_sem_del.opt:<<MIN>>
TEST.VALUE:sysApi.os_sem_del.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_sem_del.return:-2
TEST.END

-- Test Case: sem_del2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_del
TEST.NEW
TEST.NAME:sem_del2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((sid > 20 || sid < 0) || ((rts_sem)[sid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)83) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)69) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)77) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)65))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysSem.OSSemDel
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[0].Type:1397050689
TEST.VALUE:sysApi.os_sem_del.sid:0
TEST.VALUE:sysApi.os_sem_del.opt:<<MIN>>
TEST.VALUE:sysApi.os_sem_del.err:<<malloc 1>>
TEST.VALUE:sysApi.os_sem_del.err[0]:1
TEST.EXPECTED:sysApi.os_sem_del.return:-1
TEST.END

-- Test Case: sem_del3
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_del
TEST.NEW
TEST.NAME:sem_del3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sid > 20 || sid < 0) || ((rts_sem)[sid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)83) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)69) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)77) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)65))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[0].Type:1397050689
TEST.VALUE:sysApi.os_sem_del.sid:0
TEST.VALUE:sysApi.os_sem_del.opt:<<MIN>>
TEST.VALUE:sysApi.os_sem_del.err:<<malloc 1>>
TEST.VALUE:sysApi.os_sem_del.err[0]:0
TEST.EXPECTED:sysApi.os_sem_del.return:0
TEST.END

-- Subprogram: os_sem_pend

-- Test Case: sem_pend1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_pend
TEST.NEW
TEST.NAME:sem_pend1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((sid > 20 || sid < 0) || ((rts_sem)[sid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)83) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)69) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)77) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)65))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_sem_pend.sid:<<MAX>>
TEST.VALUE:sysApi.os_sem_pend.timeout:<<MIN>>
TEST.VALUE:sysApi.os_sem_pend.opt:<<MIN>>
TEST.VALUE:sysApi.os_sem_pend.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_sem_pend.return:-2
TEST.END

-- Test Case: sem_pend2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_pend
TEST.NEW
TEST.NAME:sem_pend2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((sid > 20 || sid < 0) || ((rts_sem)[sid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)83) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)69) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)77) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)65))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysSem.OSSemPend
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[0].Type:1397050689
TEST.VALUE:sysApi.os_sem_pend.sid:0
TEST.VALUE:sysApi.os_sem_pend.timeout:<<MIN>>
TEST.VALUE:sysApi.os_sem_pend.opt:<<MIN>>
TEST.VALUE:sysApi.os_sem_pend.err:<<malloc 1>>
TEST.VALUE:sysApi.os_sem_pend.err[0]:1
TEST.EXPECTED:sysApi.os_sem_pend.return:-1
TEST.END

-- Test Case: sem_pend3
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_pend
TEST.NEW
TEST.NAME:sem_pend3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sid > 20 || sid < 0) || ((rts_sem)[sid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)83) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)69) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)77) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)65))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysSem.OSSemPend
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[0].Type:1397050689
TEST.VALUE:sysApi.os_sem_pend.sid:0
TEST.VALUE:sysApi.os_sem_pend.timeout:<<MIN>>
TEST.VALUE:sysApi.os_sem_pend.opt:<<MIN>>
TEST.VALUE:sysApi.os_sem_pend.err:<<malloc 1>>
TEST.VALUE:sysApi.os_sem_pend.err[0]:0
TEST.EXPECTED:sysApi.os_sem_pend.return:0
TEST.END

-- Test Case: sem_pend4
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_pend
TEST.NEW
TEST.NAME:sem_pend4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((sid > 20 || sid < 0) || ((rts_sem)[sid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)83) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)69) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)77) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)65))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_sem_pend.sid:<<MIN>>
TEST.VALUE:sysApi.os_sem_pend.timeout:<<MIN>>
TEST.VALUE:sysApi.os_sem_pend.opt:<<MIN>>
TEST.VALUE:sysApi.os_sem_pend.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_sem_pend.return:-2
TEST.END

-- Test Case: sem_pend5
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_pend
TEST.NEW
TEST.NAME:sem_pend5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((sid > 20 || sid < 0) || ((rts_sem)[sid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)83) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)69) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)77) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)65))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[3].Type:45
TEST.VALUE:sysApi.os_sem_pend.sid:3
TEST.VALUE:sysApi.os_sem_pend.timeout:<<MIN>>
TEST.VALUE:sysApi.os_sem_pend.opt:<<MIN>>
TEST.VALUE:sysApi.os_sem_pend.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_sem_pend.return:-2
TEST.END

-- Subprogram: os_sem_post

-- Test Case: sem_post1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_post
TEST.NEW
TEST.NAME:sem_post1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((sid > 20 || sid < 0) || ((rts_sem)[sid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)83) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)69) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)77) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)65))) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_sem_post.sid:<<MAX>>
TEST.VALUE:sysApi.os_sem_post.opt:<<MIN>>
TEST.VALUE:sysApi.os_sem_post.err:<<malloc 1>>
TEST.EXPECTED:sysApi.os_sem_post.return:-2
TEST.END

-- Test Case: sem_post2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_post
TEST.NEW
TEST.NAME:sem_post2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((sid > 20 || sid < 0) || ((rts_sem)[sid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)83) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)69) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)77) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)65))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysSem.OSSemPost
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[0].Type:1397050689
TEST.VALUE:sysApi.os_sem_post.sid:0
TEST.VALUE:sysApi.os_sem_post.opt:<<MIN>>
TEST.VALUE:sysApi.os_sem_post.err:<<malloc 1>>
TEST.VALUE:sysApi.os_sem_post.err[0]:1
TEST.EXPECTED:sysApi.os_sem_post.return:-1
TEST.END

-- Test Case: sem_post3
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_sem_post
TEST.NEW
TEST.NAME:sem_post3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sid > 20 || sid < 0) || ((rts_sem)[sid]).Type != (OS_OBJ_TYPE)((((CPU_INT32U)((CPU_INT08U)83) << 3U * 8U | (CPU_INT32U)((CPU_INT08U)69) << 2U * 8U) | (CPU_INT32U)((CPU_INT08U)77) << 1U * 8U) | (CPU_INT32U)((CPU_INT08U)65))) ==> FALSE
      (2) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.<<GLOBAL>>.rts_sem[0].Type:1397050689
TEST.VALUE:sysApi.os_sem_post.sid:0
TEST.VALUE:sysApi.os_sem_post.opt:<<MIN>>
TEST.VALUE:sysApi.os_sem_post.err:<<malloc 1>>
TEST.VALUE:sysApi.os_sem_post.err[0]:0
TEST.EXPECTED:sysApi.os_sem_post.return:0
TEST.END

-- Subprogram: os_suspend_task

-- Test Case: suspendtask1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_suspend_task
TEST.NEW
TEST.NAME:suspendtask1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (*err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysTask.OSTaskSuspend
TEST.VALUE:sysApi.os_suspend_task.prio:<<MIN>>
TEST.VALUE:sysApi.os_suspend_task.err:<<malloc 1>>
TEST.VALUE:sysApi.os_suspend_task.err[0]:1
TEST.EXPECTED:sysApi.os_suspend_task.return:-1
TEST.END

-- Test Case: suspendtask2
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_suspend_task
TEST.NEW
TEST.NAME:suspendtask2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (*err != (OS_ERR_NONE)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysApi.os_suspend_task.prio:<<MIN>>
TEST.VALUE:sysApi.os_suspend_task.err:<<malloc 1>>
TEST.VALUE:sysApi.os_suspend_task.err[0]:0
TEST.EXPECTED:sysApi.os_suspend_task.return:0
TEST.END

-- Subprogram: os_task_sem_pend

-- Test Case: task_sem_pend1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_task_sem_pend
TEST.NEW
TEST.NAME:task_sem_pend1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:sysTask.OSTaskSemPend
TEST.VALUE:sysApi.os_task_sem_pend.timeout:<<MIN>>
TEST.VALUE:sysApi.os_task_sem_pend.opt:<<MIN>>
TEST.VALUE:sysApi.os_task_sem_pend.err:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPend.return:12
TEST.EXPECTED:sysApi.os_task_sem_pend.return:12
TEST.END

-- Subprogram: os_task_sem_post

-- Test Case: task_sem1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_task_sem_post
TEST.NEW
TEST.NAME:task_sem1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:sysTask.OSTaskSemPost
TEST.VALUE:sysApi.os_task_sem_post.prio:<<MIN>>
TEST.VALUE:sysApi.os_task_sem_post.opt:<<MIN>>
TEST.VALUE:sysApi.os_task_sem_post.err:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPost.return:1
TEST.EXPECTED:sysApi.os_task_sem_post.return:1
TEST.END

-- Subprogram: os_task_sem_set

-- Test Case: sem_set1
TEST.UNIT:sysApi
TEST.SUBPROGRAM:os_task_sem_set
TEST.NEW
TEST.NAME:sem_set1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:sysTask.OSTaskSemSet
TEST.VALUE:sysApi.os_task_sem_set.prio:<<MIN>>
TEST.VALUE:sysApi.os_task_sem_set.cnt:<<MIN>>
TEST.VALUE:sysApi.os_task_sem_set.err:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemSet.return:11
TEST.EXPECTED:sysApi.os_task_sem_set.return:11
TEST.END

-- Subprogram: udelay

-- Test Case: BASIS-PATH-001
TEST.UNIT:sysApi
TEST.SUBPROGRAM:udelay
TEST.NEW
TEST.NAME:BASIS-PATH-001
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sysApi.udelay.us:<<MAX>>
TEST.END
