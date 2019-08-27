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

-- Unit: sysTime

-- Subprogram: OSTimeDly

-- Test Case: OSTImeDly7
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDly
TEST.NEW
TEST.NAME:OSTImeDly7
TEST.BASIS_PATH:11 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 11
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (5) case (opt) ==> (OS_OPT)0x8U
      (6) if (dly == (OS_TICK)0U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysTime.OSTimeDly.dly:0
TEST.VALUE:sysTime.OSTimeDly.opt:8
TEST.VALUE:sysTime.OSTimeDly.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDly.p_err[0]:OS_ERR_TIME_ZERO_DLY
TEST.END

-- Test Case: OSTimeDly1
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDly
TEST.NEW
TEST.NAME:OSTimeDly1
TEST.BASIS_PATH:1 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysTime.OSTimeDly.dly:<<MIN>>
TEST.VALUE:sysTime.OSTimeDly.opt:<<MIN>>
TEST.VALUE:sysTime.OSTimeDly.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDly.p_err[0]:OS_ERR_TIME_DLY_ISR
TEST.END

-- Test Case: OSTimeDly2
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDly
TEST.NEW
TEST.NAME:OSTimeDly2
TEST.BASIS_PATH:2 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.<<GLOBAL>>.OSSchedLockNestingCtr:<<MAX>>
TEST.VALUE:sysTime.OSTimeDly.dly:<<MIN>>
TEST.VALUE:sysTime.OSTimeDly.opt:<<MIN>>
TEST.VALUE:sysTime.OSTimeDly.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDly.p_err[0]:OS_ERR_SCHED_LOCKED
TEST.END

-- Test Case: OSTimeDly3
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDly
TEST.NEW
TEST.NAME:OSTimeDly3
TEST.BASIS_PATH:3 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (8) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysTime.OSTimeDly.dly:<<MIN>>
TEST.VALUE:sysTime.OSTimeDly.opt:1
TEST.VALUE:sysTime.OSTimeDly.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDly.p_err[0]:OS_ERR_OPT_INVALID
TEST.END

-- Test Case: OSTimeDly4
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDly
TEST.NEW
TEST.NAME:OSTimeDly4
TEST.BASIS_PATH:4 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (3) case (opt) ==> 0U
      (6) if (dly == (OS_TICK)0U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysTime.OSTimeDly.dly:0
TEST.VALUE:sysTime.OSTimeDly.opt:0
TEST.VALUE:sysTime.OSTimeDly.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDly.p_err[0]:OS_ERR_TIME_ZERO_DLY
TEST.END

-- Test Case: OSTimeDly5
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDly
TEST.NEW
TEST.NAME:OSTimeDly5
TEST.BASIS_PATH:5 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (3) case (opt) ==> 0U
      (6) if (dly == (OS_TICK)0U) ==> FALSE
      (9) while 0 ==> FALSE
      (10) if (*p_err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 9
      Only one possible value for condition in branch 11
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListRemove
TEST.STUB:sysTick.OS_TickListInsertDly
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysTime.OSTimeDly.dly:1
TEST.VALUE:sysTime.OSTimeDly.opt:0
TEST.VALUE:sysTime.OSTimeDly.p_err:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDly.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysTime.OSTimeDly.p_err[0]:OS_ERR_A
TEST.END

-- Test Case: OSTimeDly6
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDly
TEST.NEW
TEST.NAME:OSTimeDly6
TEST.BASIS_PATH:10 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 10
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0x2U
      (6) if (dly == (OS_TICK)0U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysTime.OSTimeDly.dly:0
TEST.VALUE:sysTime.OSTimeDly.opt:2
TEST.VALUE:sysTime.OSTimeDly.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDly.p_err[0]:OS_ERR_TIME_ZERO_DLY
TEST.END

-- Test Case: OSTimeDly8
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDly
TEST.NEW
TEST.NAME:OSTimeDly8
TEST.BASIS_PATH:12 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 12
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (7) case (opt) ==> (OS_OPT)0x4U
      (9) while 0 ==> FALSE
      (10) if (*p_err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 9
      Only one possible value for condition in branch 11
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListRemove
TEST.STUB:sysTick.OS_TickListInsertDly
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysTime.OSTimeDly.dly:<<MIN>>
TEST.VALUE:sysTime.OSTimeDly.opt:4
TEST.VALUE:sysTime.OSTimeDly.p_err:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDly.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTime.<<GLOBAL>>.OSTCBCurPtr:<<null>>
TEST.EXPECTED:sysTime.OSTimeDly.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OSTimeDlyResume

-- Test Case: OSTimeDlyResume1
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDlyResume
TEST.NEW
TEST.NAME:OSTimeDlyResume1
TEST.BASIS_PATH:1 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDlyResume.p_err[0]:OS_ERR_TIME_DLY_RESUME_ISR
TEST.END

-- Test Case: OSTimeDlyResume10
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDlyResume
TEST.NEW
TEST.NAME:OSTimeDlyResume10
TEST.BASIS_PATH:16 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) while 0 ==> FALSE
      (4) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (14) case (p_tcb->TaskState) ==> (OS_STATE)4U
      (15) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Only one possible value for condition in branch 15
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb[0].TaskState:6
TEST.VALUE:sysTime.OSTimeDlyResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDlyResume.p_err[0]:OS_ERR_TASK_NOT_DLY
TEST.END

-- Test Case: OSTimeDlyResume11
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDlyResume
TEST.NEW
TEST.NAME:OSTimeDlyResume11
TEST.BASIS_PATH:17 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 17
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) while 0 ==> FALSE
      (4) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (16) case (p_tcb->TaskState) ==> (OS_STATE)5U
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Only one possible value for condition in branch 17
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb[0].TaskState:7
TEST.VALUE:sysTime.OSTimeDlyResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDlyResume.p_err[0]:OS_ERR_TASK_NOT_DLY
TEST.END

-- Test Case: OSTimeDlyResume12
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDlyResume
TEST.NEW
TEST.NAME:OSTimeDlyResume12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) while 0 ==> FALSE
      (4) if (p_tcb == OSTCBCurPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Only one possible value for condition in branch 5
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDlyResume.p_err[0]:OS_ERR_TASK_NOT_DLY
TEST.VALUE_USER_CODE:sysTime.OSTimeDlyResume.p_tcb
<<sysTime.OSTimeDlyResume.p_tcb>> = ( OSTCBCurPtr );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OSTimeDlyResume2
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDlyResume
TEST.NEW
TEST.NAME:OSTimeDlyResume2
TEST.BASIS_PATH:2 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb:<<null>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDlyResume.p_err[0]:OS_ERR_TASK_NOT_DLY
TEST.END

-- Test Case: OSTimeDlyResume3
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDlyResume
TEST.NEW
TEST.NAME:OSTimeDlyResume3
TEST.BASIS_PATH:3 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) while 0 ==> FALSE
      (4) if (p_tcb == OSTCBCurPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Only one possible value for condition in branch 5
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListInsert
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb[0].TaskState:0
TEST.VALUE:sysTime.OSTimeDlyResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDlyResume.p_err[0]:OS_ERR_TASK_NOT_DLY
TEST.END

-- Test Case: OSTimeDlyResume4
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDlyResume
TEST.NEW
TEST.NAME:OSTimeDlyResume4
TEST.BASIS_PATH:4 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) while 0 ==> FALSE
      (4) if (p_tcb == OSTCBCurPtr) ==> TRUE
      (5) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Only one possible value for condition in branch 5
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListInsert
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb[0].TaskState:1
TEST.VALUE:sysTime.OSTimeDlyResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDlyResume.p_tcb[0].TaskState:0
TEST.EXPECTED:sysTime.OSTimeDlyResume.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSTimeDlyResume5
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDlyResume
TEST.NEW
TEST.NAME:OSTimeDlyResume5
TEST.BASIS_PATH:5 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) while 0 ==> FALSE
      (4) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (22) case (p_tcb->TaskState) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Only one possible value for condition in branch 23
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb[0].TaskState:8
TEST.VALUE:sysTime.OSTimeDlyResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDlyResume.p_err[0]:OS_ERR_STATE_INVALID
TEST.END

-- Test Case: OSTimeDlyResume6
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDlyResume
TEST.NEW
TEST.NAME:OSTimeDlyResume6
TEST.BASIS_PATH:11 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 11
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) while 0 ==> FALSE
      (4) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (10) case (p_tcb->TaskState) ==> (OS_STATE)2U
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Only one possible value for condition in branch 11
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb[0].TaskState:2
TEST.VALUE:sysTime.OSTimeDlyResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDlyResume.p_err[0]:OS_ERR_TASK_NOT_DLY
TEST.END

-- Test Case: OSTimeDlyResume7
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDlyResume
TEST.NEW
TEST.NAME:OSTimeDlyResume7
TEST.BASIS_PATH:13 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 13
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) while 0 ==> FALSE
      (4) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (12) case (p_tcb->TaskState) ==> (OS_STATE)3U
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Only one possible value for condition in branch 13
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb[0].TaskState:3
TEST.VALUE:sysTime.OSTimeDlyResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDlyResume.p_err[0]:OS_ERR_TASK_NOT_DLY
TEST.END

-- Test Case: OSTimeDlyResume8
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDlyResume
TEST.NEW
TEST.NAME:OSTimeDlyResume8
TEST.BASIS_PATH:14 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 14
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) while 0 ==> FALSE
      (4) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (12) case (p_tcb->TaskState) ==> (OS_STATE)3U
      (13) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Only one possible value for condition in branch 13
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb[0].TaskState:4
TEST.VALUE:sysTime.OSTimeDlyResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDlyResume.p_err[0]:OS_ERR_TASK_NOT_DLY
TEST.END

-- Test Case: OSTimeDlyResume9
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeDlyResume
TEST.NEW
TEST.NAME:OSTimeDlyResume9
TEST.BASIS_PATH:15 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 15
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) while 0 ==> FALSE
      (4) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (14) case (p_tcb->TaskState) ==> (OS_STATE)4U
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Only one possible value for condition in branch 15
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysTime.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDlyResume.p_tcb[0].TaskState:5
TEST.VALUE:sysTime.OSTimeDlyResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeDlyResume.p_tcb[0].TaskState:4
TEST.EXPECTED:sysTime.OSTimeDlyResume.p_err[0]:OS_ERR_TASK_SUSPENDED
TEST.END

-- Subprogram: OSTimeGet

-- Test Case: OSTimeGet1
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeGet
TEST.NEW
TEST.NAME:OSTimeGet1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while 0 ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 2
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSTickCtr:0
TEST.VALUE:sysTime.OSTimeGet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeGet.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTime.OSTimeGet.return:0
TEST.END

-- Test Case: OSTimeGet2
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeGet
TEST.NEW
TEST.NAME:OSTimeGet2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while 0 ==> FALSE
      (2) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 2
TEST.END_NOTES:
TEST.VALUE:sysTime.<<GLOBAL>>.OSTickCtr:<<MAX>>
TEST.VALUE:sysTime.OSTimeGet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.OSTimeGet.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTime.OSTimeGet.return:<<MAX>>
TEST.END

-- Subprogram: OSTimeSet

-- Test Case: OSTImeSet2
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeSet
TEST.NEW
TEST.NAME:OSTImeSet2
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> TRUE
      (2) while 0 ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 2
TEST.END_NOTES:
TEST.VALUE:sysTime.OSTimeSet.ticks:<<MAX>>
TEST.VALUE:sysTime.OSTimeSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.<<GLOBAL>>.OSTickCtr:<<MAX>>
TEST.EXPECTED:sysTime.OSTimeSet.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSTimeSet1
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeSet
TEST.NEW
TEST.NAME:OSTimeSet1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while 0 ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 2
TEST.END_NOTES:
TEST.VALUE:sysTime.OSTimeSet.ticks:<<MIN>>
TEST.VALUE:sysTime.OSTimeSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTime.<<GLOBAL>>.OSTickCtr:0
TEST.EXPECTED:sysTime.OSTimeSet.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OSTimeTick

-- Test Case: OSTimeTick
TEST.UNIT:sysTime
TEST.SUBPROGRAM:OSTimeTick
TEST.NEW
TEST.NAME:OSTimeTick
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.END
