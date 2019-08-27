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

-- Unit: sysTask

-- Subprogram: OSTaskCreate

-- Test Case: OSTaskCreate1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskCreate
TEST.NEW
TEST.NAME:OSTaskCreate1
TEST.BASIS_PATH:1 of 18
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysTask.OSTaskCreate.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_name:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_arg:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.prio:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_stk_base:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.stk_limit:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.stk_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.q_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.time_quanta:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_ext:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.opt:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskCreate.p_err[0]:OS_ERR_TASK_CREATE_ISR
TEST.END

-- Test Case: OSTaskCreate10
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskCreate
TEST.NEW
TEST.NAME:OSTaskCreate10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 8
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_task == (OS_TASK_PTR)0) ==> FALSE
      (4) if (p_stk_base == (CPU_STK *)0) ==> FALSE
      (5) if (stk_size < OSCfg_StkSizeMin) ==> FALSE
      (6) if (stk_limit >= stk_size) ==> FALSE
      (7) if (prio >= 32U) ==> FALSE
      (8) if (prio == 32U - 1U) ==> FALSE
      (10) if ((opt & (OS_OPT)0x1U) != (OS_OPT)0) ==> FALSE
      (13) if (time_quanta == (OS_TICK)0) ==> FALSE
      (14) while 0 ==> FALSE
      (15) if (OSRunning != (OS_STATE)1U) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable p_task in branch 3 since it has a type which requires user code.
      Only one possible value for condition in branch 14
      Only one possible value for condition in branch 16
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListInsertTail
TEST.STUB:sysTask.OS_TaskInitTCB
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskQty:0
TEST.VALUE:sysTask.OSTaskCreate.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_name:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_task:OS_IdleTask
TEST.VALUE:sysTask.OSTaskCreate.p_arg:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.prio:2
TEST.VALUE:sysTask.OSTaskCreate.p_stk_base:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.stk_limit:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.stk_size:<<MAX>>
TEST.VALUE:sysTask.OSTaskCreate.q_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.time_quanta:0
TEST.VALUE:sysTask.OSTaskCreate.p_ext:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.opt:0
TEST.VALUE:sysTask.OSTaskCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskQty:1
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].Prio:2
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].BasePrio:2
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].StkSize:<<MAX>>
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].Opt:0
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].TimeQuanta:0
TEST.EXPECTED:sysTask.OSTaskCreate.p_err[0]:OS_ERR_NONE
TEST.EXPECTED_USER_CODE:sysTask.OSTaskCreate.p_tcb.p_tcb[0].TimeQuantaCtr
{{ <<sysTask.OSTaskCreate.p_tcb>>[0].TimeQuantaCtr == ( OSSchedRoundRobinDfltTimeQuanta ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: OSTaskCreate11
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskCreate
TEST.NEW
TEST.NAME:OSTaskCreate11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 8
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_task == (OS_TASK_PTR)0) ==> FALSE
      (4) if (p_stk_base == (CPU_STK *)0) ==> FALSE
      (5) if (stk_size < OSCfg_StkSizeMin) ==> FALSE
      (6) if (stk_limit >= stk_size) ==> FALSE
      (7) if (prio >= 32U) ==> FALSE
      (8) if (prio == 32U - 1U) ==> FALSE
      (10) if ((opt & (OS_OPT)0x1U) != (OS_OPT)0) ==> FALSE
      (13) if (time_quanta == (OS_TICK)0) ==> FALSE
      (14) while 0 ==> FALSE
      (15) if (OSRunning != (OS_STATE)1U) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable p_task in branch 3 since it has a type which requires user code.
      Only one possible value for condition in branch 14
      Only one possible value for condition in branch 16
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListInsertTail
TEST.STUB:sysTask.OS_TaskInitTCB
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskQty:0
TEST.VALUE:sysTask.OSTaskCreate.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_name:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_task:OS_IdleTask
TEST.VALUE:sysTask.OSTaskCreate.p_arg:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.prio:2
TEST.VALUE:sysTask.OSTaskCreate.p_stk_base:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.stk_limit:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.stk_size:<<MAX>>
TEST.VALUE:sysTask.OSTaskCreate.q_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.time_quanta:0
TEST.VALUE:sysTask.OSTaskCreate.p_ext:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.opt:1
TEST.VALUE:sysTask.OSTaskCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskQty:1
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].Prio:2
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].BasePrio:2
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].StkSize:<<MAX>>
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].Opt:1
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].TimeQuanta:0
TEST.EXPECTED:sysTask.OSTaskCreate.p_err[0]:OS_ERR_NONE
TEST.EXPECTED_USER_CODE:sysTask.OSTaskCreate.p_tcb.p_tcb[0].TimeQuantaCtr
{{ <<sysTask.OSTaskCreate.p_tcb>>[0].TimeQuantaCtr == ( OSSchedRoundRobinDfltTimeQuanta ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: OSTaskCreate12
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskCreate
TEST.NEW
TEST.NAME:OSTaskCreate12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 8
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_task == (OS_TASK_PTR)0) ==> FALSE
      (4) if (p_stk_base == (CPU_STK *)0) ==> FALSE
      (5) if (stk_size < OSCfg_StkSizeMin) ==> FALSE
      (6) if (stk_limit >= stk_size) ==> FALSE
      (7) if (prio >= 32U) ==> FALSE
      (8) if (prio == 32U - 1U) ==> FALSE
      (10) if ((opt & (OS_OPT)0x1U) != (OS_OPT)0) ==> FALSE
      (13) if (time_quanta == (OS_TICK)0) ==> FALSE
      (14) while 0 ==> FALSE
      (15) if (OSRunning != (OS_STATE)1U) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable p_task in branch 3 since it has a type which requires user code.
      Only one possible value for condition in branch 14
      Only one possible value for condition in branch 16
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListInsertTail
TEST.STUB:sysTask.OS_TaskInitTCB
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskQty:0
TEST.VALUE:sysTask.OSTaskCreate.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_name:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_task:OS_IdleTask
TEST.VALUE:sysTask.OSTaskCreate.p_arg:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.prio:2
TEST.VALUE:sysTask.OSTaskCreate.p_stk_base:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.stk_limit:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.stk_size:80
TEST.VALUE:sysTask.OSTaskCreate.q_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.time_quanta:0
TEST.VALUE:sysTask.OSTaskCreate.p_ext:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.opt:3
TEST.VALUE:sysTask.OSTaskCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskQty:1
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].Prio:2
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].BasePrio:2
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].StkSize:80
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].Opt:3
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].TimeQuanta:0
TEST.EXPECTED:sysTask.OSTaskCreate.p_err[0]:OS_ERR_NONE
TEST.EXPECTED_USER_CODE:sysTask.OSTaskCreate.p_tcb.p_tcb[0].TimeQuantaCtr
{{ <<sysTask.OSTaskCreate.p_tcb>>[0].TimeQuantaCtr == ( OSSchedRoundRobinDfltTimeQuanta ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: OSTaskCreate13
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskCreate
TEST.NEW
TEST.NAME:OSTaskCreate13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 8
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_task == (OS_TASK_PTR)0) ==> FALSE
      (4) if (p_stk_base == (CPU_STK *)0) ==> FALSE
      (5) if (stk_size < OSCfg_StkSizeMin) ==> FALSE
      (6) if (stk_limit >= stk_size) ==> FALSE
      (7) if (prio >= 32U) ==> FALSE
      (8) if (prio == 32U - 1U) ==> FALSE
      (10) if ((opt & (OS_OPT)0x1U) != (OS_OPT)0) ==> FALSE
      (13) if (time_quanta == (OS_TICK)0) ==> FALSE
      (14) while 0 ==> FALSE
      (15) if (OSRunning != (OS_STATE)1U) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable p_task in branch 3 since it has a type which requires user code.
      Only one possible value for condition in branch 14
      Only one possible value for condition in branch 16
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListInsertTail
TEST.STUB:sysTask.OS_TaskInitTCB
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskQty:0
TEST.VALUE:sysTask.OSTaskCreate.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_name:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_task:OS_IdleTask
TEST.VALUE:sysTask.OSTaskCreate.p_arg:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.prio:2
TEST.VALUE:sysTask.OSTaskCreate.p_stk_base:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.stk_limit:0
TEST.VALUE:sysTask.OSTaskCreate.stk_size:599
TEST.VALUE:sysTask.OSTaskCreate.q_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.time_quanta:0
TEST.VALUE:sysTask.OSTaskCreate.p_ext:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.opt:3
TEST.VALUE:sysTask.OSTaskCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskQty:1
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].Prio:2
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].BasePrio:2
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].Opt:3
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].TimeQuanta:0
TEST.EXPECTED:sysTask.OSTaskCreate.p_err[0]:OS_ERR_NONE
TEST.EXPECTED_USER_CODE:sysTask.OSTaskCreate.p_tcb.p_tcb[0].TimeQuantaCtr
{{ <<sysTask.OSTaskCreate.p_tcb>>[0].TimeQuantaCtr == ( OSSchedRoundRobinDfltTimeQuanta ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: OSTaskCreate2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskCreate
TEST.NEW
TEST.NAME:OSTaskCreate2
TEST.BASIS_PATH:2 of 18
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_tcb:<<null>>
TEST.VALUE:sysTask.OSTaskCreate.p_name:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_arg:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.prio:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_stk_base:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.stk_limit:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.stk_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.q_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.time_quanta:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_ext:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.opt:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskCreate.p_err[0]:OS_ERR_TCB_INVALID
TEST.END

-- Test Case: OSTaskCreate3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskCreate
TEST.NEW
TEST.NAME:OSTaskCreate3
TEST.BASIS_PATH:3 of 18
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_task == (OS_TASK_PTR)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable p_task in branch 3 since it has a type which requires user code.
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_name:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_task:<<null>>
TEST.VALUE:sysTask.OSTaskCreate.p_arg:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.prio:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_stk_base:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.stk_limit:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.stk_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.q_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.time_quanta:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_ext:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.opt:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskCreate.p_err[0]:OS_ERR_TASK_INVALID
TEST.END

-- Test Case: OSTaskCreate4
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskCreate
TEST.NEW
TEST.NAME:OSTaskCreate4
TEST.BASIS_PATH:4 of 18
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_task == (OS_TASK_PTR)0) ==> FALSE
      (4) if (p_stk_base == (CPU_STK *)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable p_task in branch 3 since it has a type which requires user code.
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_name:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_task:OS_IdleTask
TEST.VALUE:sysTask.OSTaskCreate.p_arg:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.prio:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_stk_base:<<null>>
TEST.VALUE:sysTask.OSTaskCreate.stk_limit:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.stk_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.q_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.time_quanta:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_ext:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.opt:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskCreate.p_err[0]:OS_ERR_STK_INVALID
TEST.END

-- Test Case: OSTaskCreate5
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskCreate
TEST.NEW
TEST.NAME:OSTaskCreate5
TEST.BASIS_PATH:5 of 18
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_task == (OS_TASK_PTR)0) ==> FALSE
      (4) if (p_stk_base == (CPU_STK *)0) ==> FALSE
      (5) if (stk_size < OSCfg_StkSizeMin) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable p_task in branch 3 since it has a type which requires user code.
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_name:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_task:OS_IdleTask
TEST.VALUE:sysTask.OSTaskCreate.p_arg:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.prio:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_stk_base:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_stk_base[0]:1
TEST.VALUE:sysTask.OSTaskCreate.stk_limit:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.stk_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.q_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.time_quanta:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_ext:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.opt:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskCreate.p_err[0]:OS_ERR_STK_SIZE_INVALID
TEST.END

-- Test Case: OSTaskCreate6
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskCreate
TEST.NEW
TEST.NAME:OSTaskCreate6
TEST.BASIS_PATH:6 of 18
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_task == (OS_TASK_PTR)0) ==> FALSE
      (4) if (p_stk_base == (CPU_STK *)0) ==> FALSE
      (5) if (stk_size < OSCfg_StkSizeMin) ==> FALSE
      (6) if (stk_limit >= stk_size) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable p_task in branch 3 since it has a type which requires user code.
      Cannot set const variable TEST.VALUE:sysTask.<<GLOBAL>>.OSCfg_StkSizeMin:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_name:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_task:OS_IdleTask
TEST.VALUE:sysTask.OSTaskCreate.p_arg:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.prio:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_stk_base:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.stk_limit:<<MAX>>
TEST.VALUE:sysTask.OSTaskCreate.stk_size:<<MAX>>
TEST.VALUE:sysTask.OSTaskCreate.q_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.time_quanta:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_ext:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.opt:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskCreate.p_err[0]:OS_ERR_STK_LIMIT_INVALID
TEST.END

-- Test Case: OSTaskCreate7
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskCreate
TEST.NEW
TEST.NAME:OSTaskCreate7
TEST.BASIS_PATH:7 of 18
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_task == (OS_TASK_PTR)0) ==> FALSE
      (4) if (p_stk_base == (CPU_STK *)0) ==> FALSE
      (5) if (stk_size < OSCfg_StkSizeMin) ==> FALSE
      (6) if (stk_limit >= stk_size) ==> FALSE
      (7) if (prio >= 32U) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable p_task in branch 3 since it has a type which requires user code.
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_name:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_task:OS_IdleTask
TEST.VALUE:sysTask.OSTaskCreate.p_arg:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.prio:<<MAX>>
TEST.VALUE:sysTask.OSTaskCreate.p_stk_base:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.stk_limit:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.stk_size:<<MAX>>
TEST.VALUE:sysTask.OSTaskCreate.q_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.time_quanta:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_ext:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.opt:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskCreate.p_err[0]:OS_ERR_PRIO_INVALID
TEST.END

-- Test Case: OSTaskCreate8
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskCreate
TEST.NEW
TEST.NAME:OSTaskCreate8
TEST.BASIS_PATH:8 of 18
TEST.NOTES:
This is an automatically generated test case.
   Test Path 8
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_task == (OS_TASK_PTR)0) ==> FALSE
      (4) if (p_stk_base == (CPU_STK *)0) ==> FALSE
      (5) if (stk_size < OSCfg_StkSizeMin) ==> FALSE
      (6) if (stk_limit >= stk_size) ==> FALSE
      (7) if (prio >= 32U) ==> FALSE
      (8) if (prio == 32U - 1U) ==> FALSE
      (10) if ((opt & (OS_OPT)0x1U) != (OS_OPT)0) ==> FALSE
      (13) if (time_quanta == (OS_TICK)0) ==> FALSE
      (14) while 0 ==> FALSE
      (15) if (OSRunning != (OS_STATE)1U) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable p_task in branch 3 since it has a type which requires user code.
      Only one possible value for condition in branch 14
      Only one possible value for condition in branch 16
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSRunning:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_name:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_task:OS_IdleTask
TEST.VALUE:sysTask.OSTaskCreate.p_arg:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.prio:31
TEST.VALUE:sysTask.OSTaskCreate.p_stk_base:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.stk_limit:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.stk_size:<<MAX>>
TEST.VALUE:sysTask.OSTaskCreate.q_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.time_quanta:1
TEST.VALUE:sysTask.OSTaskCreate.p_ext:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.opt:0
TEST.VALUE:sysTask.OSTaskCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskCreate.p_err[0]:OS_ERR_PRIO_INVALID
TEST.END

-- Test Case: OSTaskCreate9
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskCreate
TEST.NEW
TEST.NAME:OSTaskCreate9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 8
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_task == (OS_TASK_PTR)0) ==> FALSE
      (4) if (p_stk_base == (CPU_STK *)0) ==> FALSE
      (5) if (stk_size < OSCfg_StkSizeMin) ==> FALSE
      (6) if (stk_limit >= stk_size) ==> FALSE
      (7) if (prio >= 32U) ==> FALSE
      (8) if (prio == 32U - 1U) ==> FALSE
      (10) if ((opt & (OS_OPT)0x1U) != (OS_OPT)0) ==> FALSE
      (13) if (time_quanta == (OS_TICK)0) ==> FALSE
      (14) while 0 ==> FALSE
      (15) if (OSRunning != (OS_STATE)1U) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable p_task in branch 3 since it has a type which requires user code.
      Only one possible value for condition in branch 14
      Only one possible value for condition in branch 16
TEST.END_NOTES:
TEST.STUB:sysCore.OS_RdyListInsertTail
TEST.STUB:sysTask.OS_TaskInitTCB
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSRunning:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskQty:0
TEST.VALUE:sysTask.OSTaskCreate.p_name:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.p_task:OS_IdleTask
TEST.VALUE:sysTask.OSTaskCreate.p_arg:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.prio:31
TEST.VALUE:sysTask.OSTaskCreate.p_stk_base:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskCreate.stk_limit:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.stk_size:<<MAX>>
TEST.VALUE:sysTask.OSTaskCreate.q_size:<<MIN>>
TEST.VALUE:sysTask.OSTaskCreate.time_quanta:1
TEST.VALUE:sysTask.OSTaskCreate.p_ext:VECTORCAST_INT1
TEST.VALUE:sysTask.OSTaskCreate.opt:0
TEST.VALUE:sysTask.OSTaskCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskQty:1
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].Prio:31
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].BasePrio:31
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].StkSize:<<MAX>>
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].Opt:0
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].TimeQuanta:1
TEST.EXPECTED:sysTask.OSTaskCreate.p_tcb[0].TimeQuantaCtr:1
TEST.EXPECTED:sysTask.OSTaskCreate.p_err[0]:OS_ERR_NONE
TEST.VALUE_USER_CODE:sysTask.OSTaskCreate.p_tcb
<<sysTask.OSTaskCreate.p_tcb>> = ( &OSIdleTaskTCB );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: OSTaskDel

-- Test Case: BASIS-PATH-006-PARTIAL.005
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskDel
TEST.NEW
TEST.NAME:BASIS-PATH-006-PARTIAL.005
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (30) if (p_tcb->MutexGrpHeadPtr != (OS_MUTEX *)0) ==> FALSE
      (31) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 31
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_PendListRemove
TEST.STUB:sysMutex.OS_MutexGrpPostAll
TEST.STUB:sysTask.OS_TaskInitTCB
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskQty:4
TEST.VALUE:sysTask.OSTaskDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].PendOn:4
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].TaskState:2
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskQty:3
TEST.EXPECTED:sysTask.OSTaskDel.p_tcb[0].PendOn:0
TEST.EXPECTED:sysTask.OSTaskDel.p_tcb[0].TaskState:255
TEST.EXPECTED:sysTask.OSTaskDel.p_err[0]:OS_ERR_NONE
TEST.VALUE_USER_CODE:sysTask.OSTaskDel.p_tcb.p_tcb[0].PendDataTblPtr.PendDataTblPtr[0].PendObjPtr
OS_MUTEX *P,PP;
P=&PP;
P->OwnerTCBPtr=OSTCBCurPtr;
<<sysTask.OSTaskDel.p_tcb>>[0].PendDataTblPtr[0].PendObjPtr = ( P );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OSTaskDel1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskDel
TEST.NEW
TEST.NAME:OSTaskDel1
TEST.BASIS_PATH:1 of 30
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysTask.OSTaskDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskDel.p_err[0]:OS_ERR_TASK_DEL_ISR
TEST.END

-- Test Case: OSTaskDel10
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskDel
TEST.NEW
TEST.NAME:OSTaskDel10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (30) if (p_tcb->MutexGrpHeadPtr != (OS_MUTEX *)0) ==> FALSE
      (31) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 31
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_PendListRemove
TEST.STUB:sysMutex.OS_MutexGrpPostAll
TEST.STUB:sysTask.OS_TaskInitTCB
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskQty:4
TEST.VALUE:sysTask.OSTaskDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].PendOn:3
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].TaskState:3
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskQty:3
TEST.EXPECTED:sysTask.OSTaskDel.p_tcb[0].TaskState:255
TEST.EXPECTED:sysTask.OSTaskDel.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSTaskDel11
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskDel
TEST.NEW
TEST.NAME:OSTaskDel11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (30) if (p_tcb->MutexGrpHeadPtr != (OS_MUTEX *)0) ==> FALSE
      (31) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 31
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_PendListRemove
TEST.STUB:sysMutex.OS_MutexGrpPostAll
TEST.STUB:sysTask.OS_TaskInitTCB
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskQty:4
TEST.VALUE:sysTask.OSTaskDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].PendOn:8
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].TaskState:3
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskQty:3
TEST.EXPECTED:sysTask.OSTaskDel.p_tcb[0].TaskState:255
TEST.EXPECTED:sysTask.OSTaskDel.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSTaskDel2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskDel
TEST.NEW
TEST.NAME:OSTaskDel2
TEST.BASIS_PATH:2 of 30
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == &OSIdleTaskTCB) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable to unknown value in branch 2
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskDel.p_err[0]:OS_ERR_TASK_DEL_IDLE
TEST.VALUE_USER_CODE:sysTask.OSTaskDel.p_tcb
<<sysTask.OSTaskDel.p_tcb>> = (&OSIdleTaskTCB );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OSTaskDel3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskDel
TEST.NEW
TEST.NAME:OSTaskDel3
TEST.BASIS_PATH:3 of 30
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (28) case (p_tcb->TaskState) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 29
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListRemove
TEST.STUB:sysTask.OS_TaskInitTCB
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskQty:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TaskState:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].MutexGrpHeadPtr:<<null>>
TEST.VALUE:sysTask.OSTaskDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].TaskState:100
TEST.VALUE:sysTask.OSTaskDel.p_err:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskInitTCB.p_tcb[0].TaskState:1
TEST.EXPECTED:sysTask.OSTaskDel.p_err[0]:OS_ERR_STATE_INVALID
TEST.END

-- Test Case: OSTaskDel4
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskDel
TEST.NEW
TEST.NAME:OSTaskDel4
TEST.BASIS_PATH:4 of 30
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (28) case (p_tcb->TaskState) ==> default
      (29) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 29
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TaskState:100
TEST.VALUE:sysTask.OSTaskDel.p_tcb:<<null>>
TEST.VALUE:sysTask.OSTaskDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskDel.p_err[0]:OS_ERR_STATE_INVALID
TEST.END

-- Test Case: OSTaskDel5
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskDel
TEST.NEW
TEST.NAME:OSTaskDel5
TEST.BASIS_PATH:5 of 30
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (30) if (p_tcb->MutexGrpHeadPtr != (OS_MUTEX *)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 31
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListRemove
TEST.STUB:sysTask.OS_TaskInitTCB
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskQty:2
TEST.VALUE:sysTask.OSTaskDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].TaskState:0
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].MutexGrpHeadPtr:<<null>>
TEST.VALUE:sysTask.OSTaskDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskQty:1
TEST.EXPECTED:sysTask.OSTaskDel.p_tcb[0].TaskState:255
TEST.EXPECTED:sysTask.OSTaskDel.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSTaskDel6
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskDel
TEST.NEW
TEST.NAME:OSTaskDel6
TEST.BASIS_PATH:6 of 30
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (30) if (p_tcb->MutexGrpHeadPtr != (OS_MUTEX *)0) ==> FALSE
      (31) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 31
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysMutex.OS_MutexGrpPostAll
TEST.STUB:sysTask.OS_TaskInitTCB
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskQty:4
TEST.VALUE:sysTask.OSTaskDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].TaskState:4
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskQty:3
TEST.EXPECTED:sysTask.OSTaskDel.p_tcb[0].TaskState:255
TEST.EXPECTED:sysTask.OSTaskDel.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSTaskDel7
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskDel
TEST.NEW
TEST.NAME:OSTaskDel7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (30) if (p_tcb->MutexGrpHeadPtr != (OS_MUTEX *)0) ==> FALSE
      (31) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 31
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysMutex.OS_MutexGrpPostAll
TEST.STUB:sysTask.OS_TaskInitTCB
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskQty:4
TEST.VALUE:sysTask.OSTaskDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].TaskState:1
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskQty:3
TEST.EXPECTED:sysTask.OSTaskDel.p_tcb[0].TaskState:255
TEST.EXPECTED:sysTask.OSTaskDel.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSTaskDel8
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskDel
TEST.NEW
TEST.NAME:OSTaskDel8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (30) if (p_tcb->MutexGrpHeadPtr != (OS_MUTEX *)0) ==> FALSE
      (31) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 31
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysMutex.OS_MutexGrpPostAll
TEST.STUB:sysTask.OS_TaskInitTCB
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskQty:4
TEST.VALUE:sysTask.OSTaskDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].PendOn:0
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].TaskState:2
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskQty:3
TEST.EXPECTED:sysTask.OSTaskDel.p_tcb[0].TaskState:255
TEST.EXPECTED:sysTask.OSTaskDel.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSTaskDel9
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskDel
TEST.NEW
TEST.NAME:OSTaskDel9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (30) if (p_tcb->MutexGrpHeadPtr != (OS_MUTEX *)0) ==> FALSE
      (31) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 31
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_PendListRemove
TEST.STUB:sysMutex.OS_MutexGrpPostAll
TEST.STUB:sysTask.OS_TaskInitTCB
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskQty:4
TEST.VALUE:sysTask.OSTaskDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].PendOn:1
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].TaskState:2
TEST.VALUE:sysTask.OSTaskDel.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskQty:3
TEST.EXPECTED:sysTask.OSTaskDel.p_tcb[0].TaskState:255
TEST.EXPECTED:sysTask.OSTaskDel.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OSTaskRegGet

-- Test Case: OSTaskRegGet1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskRegGet
TEST.NEW
TEST.NAME:OSTaskRegGet1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (id >= 1U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.OSTaskRegGet.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskRegGet.id:<<MAX>>
TEST.VALUE:sysTask.OSTaskRegGet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskRegGet.p_err[0]:OS_ERR_REG_ID_INVALID
TEST.EXPECTED:sysTask.OSTaskRegGet.return:0
TEST.END

-- Test Case: OSTaskRegGet2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskRegGet
TEST.NEW
TEST.NAME:OSTaskRegGet2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (id >= 1U) ==> FALSE
      (2) while 0 ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 2
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysTask.OSTaskRegGet.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskRegGet.p_tcb[0].RegTbl[0]:1
TEST.VALUE:sysTask.OSTaskRegGet.id:<<MIN>>
TEST.VALUE:sysTask.OSTaskRegGet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskRegGet.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OSTaskRegGet.return:1
TEST.END

-- Test Case: OSTaskRegGet3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskRegGet
TEST.NEW
TEST.NAME:OSTaskRegGet3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (id >= 1U) ==> FALSE
      (2) while 0 ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (4) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 2
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].RegTbl[0]:<<MAX>>
TEST.VALUE:sysTask.OSTaskRegGet.p_tcb:<<null>>
TEST.VALUE:sysTask.OSTaskRegGet.id:<<MIN>>
TEST.VALUE:sysTask.OSTaskRegGet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskRegGet.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OSTaskRegGet.return:<<MAX>>
TEST.END

-- Subprogram: OSTaskRegGetID

-- Test Case: OSTaskRegGetID1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskRegGetID
TEST.NEW
TEST.NAME:OSTaskRegGetID1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while 0 ==> FALSE
      (2) if (OSTaskRegNextAvailID >= 1U) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 3
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskRegNextAvailID:<<MAX>>
TEST.VALUE:sysTask.OSTaskRegGetID.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskRegGetID.p_err[0]:OS_ERR_NO_MORE_ID_AVAIL
TEST.EXPECTED:sysTask.OSTaskRegGetID.return:1
TEST.END

-- Test Case: OSTaskRegGetID2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskRegGetID
TEST.NEW
TEST.NAME:OSTaskRegGetID2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while 0 ==> FALSE
      (2) if (OSTaskRegNextAvailID >= 1U) ==> TRUE
      (3) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 3
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSTaskRegNextAvailID:0
TEST.VALUE:sysTask.OSTaskRegGetID.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskRegNextAvailID:1
TEST.EXPECTED:sysTask.OSTaskRegGetID.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OSTaskRegGetID.return:0
TEST.END

-- Subprogram: OSTaskRegSet

-- Test Case: OSTaskRegSet1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskRegSet
TEST.NEW
TEST.NAME:OSTaskRegSet1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (id >= 1U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.OSTaskRegSet.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskRegSet.id:<<MAX>>
TEST.VALUE:sysTask.OSTaskRegSet.value:<<MIN>>
TEST.VALUE:sysTask.OSTaskRegSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskRegSet.p_err[0]:OS_ERR_REG_ID_INVALID
TEST.END

-- Test Case: OSTaskRegSet2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskRegSet
TEST.NEW
TEST.NAME:OSTaskRegSet2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (id >= 1U) ==> FALSE
      (2) while 0 ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 2
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysTask.OSTaskRegSet.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskRegSet.id:<<MIN>>
TEST.VALUE:sysTask.OSTaskRegSet.value:<<MIN>>
TEST.VALUE:sysTask.OSTaskRegSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskRegSet.p_tcb[0].RegTbl[0]:0
TEST.EXPECTED:sysTask.OSTaskRegSet.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSTaskRegSet3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskRegSet
TEST.NEW
TEST.NAME:OSTaskRegSet3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (id >= 1U) ==> FALSE
      (2) while 0 ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (4) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 2
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskRegSet.p_tcb:<<null>>
TEST.VALUE:sysTask.OSTaskRegSet.id:<<MIN>>
TEST.VALUE:sysTask.OSTaskRegSet.value:<<MIN>>
TEST.VALUE:sysTask.OSTaskRegSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskRegSet.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OSTaskResume

-- Test Case: OSTaskResume1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskResume
TEST.NEW
TEST.NAME:OSTaskResume1
TEST.BASIS_PATH:1 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysTask.OSTaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskResume.p_err[0]:OS_ERR_TASK_RESUME_ISR
TEST.END

-- Test Case: OSTaskResume2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskResume
TEST.NEW
TEST.NAME:OSTaskResume2
TEST.BASIS_PATH:2 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) while 0 ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0 || p_tcb == OSTCBCurPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 2
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskResume.p_tcb:<<null>>
TEST.VALUE:sysTask.OSTaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskResume.p_err[0]:OS_ERR_TASK_RESUME_SELF
TEST.END

-- Test Case: OSTaskResume3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskResume
TEST.NEW
TEST.NAME:OSTaskResume3
TEST.BASIS_PATH:3 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) while 0 ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0 || p_tcb == OSTCBCurPtr) ==> TRUE
      (4) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 2
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.STUB:sysTask.OS_TaskResume
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskResume.p_err:<<malloc 1>>
TEST.END

-- Test Case: OSTaskResume4
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskResume
TEST.NEW
TEST.NAME:OSTaskResume4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) while 0 ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0 || p_tcb == OSTCBCurPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 2
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskResume.p_err[0]:OS_ERR_TASK_RESUME_SELF
TEST.VALUE_USER_CODE:sysTask.OSTaskResume.p_tcb
<<sysTask.OSTaskResume.p_tcb>> = ( OSTCBCurPtr );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: OSTaskSemPend

-- Test Case: OSTaskSemPend1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend1
TEST.BASIS_PATH:1 of 24
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_PEND_ISR
TEST.EXPECTED:sysTask.OSTaskSemPend.return:0
TEST.END

-- Test Case: OSTaskSemPend10
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) if (p_ts != (CPU_TS *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr->SemCtr > (OS_SEM_CTR)0) ==> TRUE
      (9) if (OSTCBCurPtr->SemPendTimeMax < OSTCBCurPtr->SemPendTime) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Conflict: Trying to set variable sysTask.OSTaskSemPend.p_ts 'equal to' and 'not equal to' same value in branches 5/6
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:uut_prototype_stubs.getTblms.return:3
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTimeMax:0
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:0
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<null>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPend.p_ts:<<null>>
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_PEND_ABORT
TEST.EXPECTED:sysTask.OSTaskSemPend.return:0
TEST.END

-- Test Case: OSTaskSemPend11
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) if (p_ts != (CPU_TS *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr->SemCtr > (OS_SEM_CTR)0) ==> TRUE
      (9) if (OSTCBCurPtr->SemPendTimeMax < OSTCBCurPtr->SemPendTime) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Conflict: Trying to set variable sysTask.OSTaskSemPend.p_ts 'equal to' and 'not equal to' same value in branches 5/6
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:uut_prototype_stubs.getTblms.return:3
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:3
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTimeMax:0
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:0
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<null>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPend.p_ts:<<null>>
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_TIMEOUT
TEST.EXPECTED:sysTask.OSTaskSemPend.return:0
TEST.END

-- Test Case: OSTaskSemPend12
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) if (p_ts != (CPU_TS *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr->SemCtr > (OS_SEM_CTR)0) ==> TRUE
      (9) if (OSTCBCurPtr->SemPendTimeMax < OSTCBCurPtr->SemPendTime) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Conflict: Trying to set variable sysTask.OSTaskSemPend.p_ts 'equal to' and 'not equal to' same value in branches 5/6
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:uut_prototype_stubs.getTblms.return:3
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:3
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTimeMax:0
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:0
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPend.p_ts[0]:0
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_TIMEOUT
TEST.EXPECTED:sysTask.OSTaskSemPend.return:0
TEST.END

-- Test Case: OSTaskSemPend13
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) if (p_ts != (CPU_TS *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr->SemCtr > (OS_SEM_CTR)0) ==> TRUE
      (9) if (OSTCBCurPtr->SemPendTimeMax < OSTCBCurPtr->SemPendTime) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Conflict: Trying to set variable sysTask.OSTaskSemPend.p_ts 'equal to' and 'not equal to' same value in branches 5/6
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:uut_prototype_stubs.getTblms.return:3
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:7
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTimeMax:0
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:0
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_STATUS_INVALID
TEST.EXPECTED:sysTask.OSTaskSemPend.return:0
TEST.END

-- Test Case: OSTaskSemPend14
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) if (p_ts != (CPU_TS *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr->SemCtr > (OS_SEM_CTR)0) ==> TRUE
      (9) if (OSTCBCurPtr->SemPendTimeMax < OSTCBCurPtr->SemPendTime) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Conflict: Trying to set variable sysTask.OSTaskSemPend.p_ts 'equal to' and 'not equal to' same value in branches 5/6
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.VALUE:uut_prototype_stubs.getTblms.return:3
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTimeMax:0
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:0
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:2
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTimeMax:2
TEST.EXPECTED:sysTask.OSTaskSemPend.p_ts[0]:1
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OSTaskSemPend.return:0
TEST.END

-- Test Case: OSTaskSemPend15
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend15
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSSchedLockNestingCtr:<<MAX>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:0
TEST.VALUE:sysTask.OSTaskSemPend.opt:0
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPend.p_ts[0]:0
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_SCHED_LOCKED
TEST.EXPECTED:sysTask.OSTaskSemPend.return:0
TEST.END

-- Test Case: OSTaskSemPend16
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend16
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) if (p_ts != (CPU_TS *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr->SemCtr > (OS_SEM_CTR)0) ==> TRUE
      (9) if (OSTCBCurPtr->SemPendTimeMax < OSTCBCurPtr->SemPendTime) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Conflict: Trying to set variable sysTask.OSTaskSemPend.p_ts 'equal to' and 'not equal to' same value in branches 5/6
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.VALUE:uut_prototype_stubs.getTblms.return:3
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTimeMax:<<MAX>>
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:0
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:2
TEST.EXPECTED:sysTask.OSTaskSemPend.p_ts[0]:1
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OSTaskSemPend.return:0
TEST.END

-- Test Case: OSTaskSemPend2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend2
TEST.BASIS_PATH:2 of 24
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (4) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:1
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_OPT_INVALID
TEST.EXPECTED:sysTask.OSTaskSemPend.return:0
TEST.END

-- Test Case: OSTaskSemPend3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend3
TEST.BASIS_PATH:3 of 24
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) if (p_ts != (CPU_TS *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr->SemCtr > (OS_SEM_CTR)0) ==> TRUE
      (9) if (OSTCBCurPtr->SemPendTimeMax < OSTCBCurPtr->SemPendTime) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Conflict: Trying to set variable sysTask.OSTaskSemPend.p_ts 'equal to' and 'not equal to' same value in branches 5/6
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:uut_prototype_stubs.getTblms.return:3
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:<<MAX>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTimeMax:<<MAX>>
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:0
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<null>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:4294967294
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:2
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OSTaskSemPend.return:4294967294
TEST.END

-- Test Case: OSTaskSemPend4
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) if (p_ts != (CPU_TS *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr->SemCtr > (OS_SEM_CTR)0) ==> TRUE
      (9) if (OSTCBCurPtr->SemPendTimeMax < OSTCBCurPtr->SemPendTime) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Conflict: Trying to set variable sysTask.OSTaskSemPend.p_ts 'equal to' and 'not equal to' same value in branches 5/6
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:uut_prototype_stubs.getTblms.return:3
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:<<MAX>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTimeMax:0
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:0
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<null>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:4294967294
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:2
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTimeMax:2
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OSTaskSemPend.return:4294967294
TEST.END

-- Test Case: OSTaskSemPend5
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend5
TEST.BASIS_PATH:10 of 24
TEST.NOTES:
This is an automatically generated test case.
   Test Path 10
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) if (p_ts != (CPU_TS *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr->SemCtr > (OS_SEM_CTR)0) ==> FALSE
      (11) if ((opt & (OS_OPT)0x8000U) != (OS_OPT)0) ==> FALSE
      (13) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
      (14) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Conflict: Trying to set variable sysTask.OSTaskSemPend.p_ts 'equal to' and 'not equal to' same value in branches 5/6
      Only one possible value for condition in branch 14
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSSchedLockNestingCtr:<<MAX>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:32768
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPend.p_ts[0]:0
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_PEND_WOULD_BLOCK
TEST.EXPECTED:sysTask.OSTaskSemPend.return:0
TEST.END

-- Test Case: OSTaskSemPend6
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) if (p_ts != (CPU_TS *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr->SemCtr > (OS_SEM_CTR)0) ==> TRUE
      (9) if (OSTCBCurPtr->SemPendTimeMax < OSTCBCurPtr->SemPendTime) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Conflict: Trying to set variable sysTask.OSTaskSemPend.p_ts 'equal to' and 'not equal to' same value in branches 5/6
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:uut_prototype_stubs.getTblms.return:3
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:<<MAX>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTimeMax:<<MAX>>
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:0
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:4294967294
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:2
TEST.EXPECTED:sysTask.OSTaskSemPend.p_ts[0]:1
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OSTaskSemPend.return:4294967294
TEST.END

-- Test Case: OSTaskSemPend7
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) if (p_ts != (CPU_TS *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr->SemCtr > (OS_SEM_CTR)0) ==> TRUE
      (9) if (OSTCBCurPtr->SemPendTimeMax < OSTCBCurPtr->SemPendTime) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Conflict: Trying to set variable sysTask.OSTaskSemPend.p_ts 'equal to' and 'not equal to' same value in branches 5/6
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:uut_prototype_stubs.getTblms.return:3
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTimeMax:0
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:0
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<null>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OSTaskSemPend.return:0
TEST.END

-- Test Case: OSTaskSemPend8
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) if (p_ts != (CPU_TS *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr->SemCtr > (OS_SEM_CTR)0) ==> TRUE
      (9) if (OSTCBCurPtr->SemPendTimeMax < OSTCBCurPtr->SemPendTime) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Conflict: Trying to set variable sysTask.OSTaskSemPend.p_ts 'equal to' and 'not equal to' same value in branches 5/6
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:uut_prototype_stubs.getTblms.return:3
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTimeMax:0
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:0
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPend.p_ts[0]:1
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OSTaskSemPend.return:0
TEST.END

-- Test Case: OSTaskSemPend9
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPend
TEST.NEW
TEST.NAME:OSTaskSemPend9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) if (p_ts != (CPU_TS *)0) ==> FALSE
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr->SemCtr > (OS_SEM_CTR)0) ==> TRUE
      (9) if (OSTCBCurPtr->SemPendTimeMax < OSTCBCurPtr->SemPendTime) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Conflict: Trying to set variable sysTask.OSTaskSemPend.p_ts 'equal to' and 'not equal to' same value in branches 5/6
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:uut_prototype_stubs.getTblms.return:3
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTime:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemPendTimeMax:0
TEST.VALUE:sysTask.OSTaskSemPend.timeout:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPend.opt:0
TEST.VALUE:sysTask.OSTaskSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPend.p_ts[0]:1
TEST.EXPECTED:sysTask.OSTaskSemPend.p_err[0]:OS_ERR_PEND_ABORT
TEST.EXPECTED:sysTask.OSTaskSemPend.return:0
TEST.END

-- Subprogram: OSTaskSemPendAbort

-- Test Case: OSTaskSemPendAbort1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPendAbort
TEST.NEW
TEST.NAME:OSTaskSemPendAbort1
TEST.BASIS_PATH:1 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPendAbort.opt:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPendAbort.p_err[0]:OS_ERR_PEND_ABORT_ISR
TEST.EXPECTED:sysTask.OSTaskSemPendAbort.return:0
TEST.END

-- Test Case: OSTaskSemPendAbort2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPendAbort
TEST.NEW
TEST.NAME:OSTaskSemPendAbort2
TEST.BASIS_PATH:2 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (4) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPendAbort.opt:1
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPendAbort.p_err[0]:OS_ERR_OPT_INVALID
TEST.EXPECTED:sysTask.OSTaskSemPendAbort.return:0
TEST.END

-- Test Case: OSTaskSemPendAbort3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPendAbort
TEST.NEW
TEST.NAME:OSTaskSemPendAbort3
TEST.BASIS_PATH:3 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) while 0 ==> FALSE
      (6) if (p_tcb == (OS_TCB *)0 || p_tcb == OSTCBCurPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 5
      Only one possible value for condition in branch 7
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_tcb:<<null>>
TEST.VALUE:sysTask.OSTaskSemPendAbort.opt:0
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPendAbort.p_err[0]:OS_ERR_PEND_ABORT_SELF
TEST.EXPECTED:sysTask.OSTaskSemPendAbort.return:0
TEST.END

-- Test Case: OSTaskSemPendAbort4
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPendAbort
TEST.NEW
TEST.NAME:OSTaskSemPendAbort4
TEST.BASIS_PATH:5 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) while 0 ==> FALSE
      (6) if (p_tcb == (OS_TCB *)0 || p_tcb == OSTCBCurPtr) ==> FALSE
      (8) if (p_tcb->PendOn != (OS_STATE)7U) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 5
      Only one possible value for condition in branch 9
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_tcb[0].PendOn:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPendAbort.opt:0
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPendAbort.p_err[0]:OS_ERR_PEND_ABORT_NONE
TEST.EXPECTED:sysTask.OSTaskSemPendAbort.return:0
TEST.END

-- Test Case: OSTaskSemPendAbort5
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPendAbort
TEST.NEW
TEST.NAME:OSTaskSemPendAbort5
TEST.BASIS_PATH:6 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
      (5) while 0 ==> FALSE
      (6) if (p_tcb == (OS_TCB *)0 || p_tcb == OSTCBCurPtr) ==> FALSE
      (8) if (p_tcb->PendOn != (OS_STATE)7U) ==> TRUE
      (9) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 5
      Only one possible value for condition in branch 9
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_PendAbort
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_tcb[0].PendOn:7
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_tcb[0].Opt:0
TEST.VALUE:sysTask.OSTaskSemPendAbort.opt:0
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPendAbort.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OSTaskSemPendAbort.return:1
TEST.END

-- Test Case: OSTaskSemPendAbort6
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPendAbort
TEST.NEW
TEST.NAME:OSTaskSemPendAbort6
TEST.BASIS_PATH:13 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 13
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0x8000U
      (5) while 0 ==> FALSE
      (6) if (p_tcb == (OS_TCB *)0 || p_tcb == OSTCBCurPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 5
      Only one possible value for condition in branch 7
TEST.END_NOTES:
TEST.STUB:sysCore.OS_PendAbort
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_tcb[0].PendOn:7
TEST.VALUE:sysTask.OSTaskSemPendAbort.opt:32768
TEST.VALUE:sysTask.OSTaskSemPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPendAbort.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OSTaskSemPendAbort.return:1
TEST.END

-- Subprogram: OSTaskSemPost

-- Test Case: OSTaskSemPost1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPost
TEST.NEW
TEST.NAME:OSTaskSemPost1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (3) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.OSTaskSemPost.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPost.opt:1
TEST.VALUE:sysTask.OSTaskSemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemPost.p_err[0]:OS_ERR_OPT_INVALID
TEST.EXPECTED:sysTask.OSTaskSemPost.return:0
TEST.END

-- Test Case: OSTaskSemPost2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPost
TEST.NEW
TEST.NAME:OSTaskSemPost2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) case (opt) ==> (OS_OPT)0U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysTask.OS_TaskSemPost
TEST.VALUE:sysTask.OSTaskSemPost.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPost.opt:0
TEST.VALUE:sysTask.OSTaskSemPost.p_err:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSemPost.return:1
TEST.EXPECTED:sysTask.OSTaskSemPost.return:1
TEST.END

-- Test Case: OSTaskSemPost3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemPost
TEST.NEW
TEST.NAME:OSTaskSemPost3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (2) case (opt) ==> (OS_OPT)0x8000U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysTask.OS_TaskSemPost
TEST.VALUE:sysTask.OSTaskSemPost.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemPost.opt:32768
TEST.VALUE:sysTask.OSTaskSemPost.p_err:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSemPost.return:3
TEST.EXPECTED:sysTask.OSTaskSemPost.return:3
TEST.END

-- Subprogram: OSTaskSemSet

-- Test Case: OSTaskSemSet1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemSet
TEST.NEW
TEST.NAME:OSTaskSemSet1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysTask.OSTaskSemSet.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemSet.cnt:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemSet.p_err[0]:OS_ERR_SET_ISR
TEST.EXPECTED:sysTask.OSTaskSemSet.return:0
TEST.END

-- Test Case: OSTaskSemSet2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemSet
TEST.NEW
TEST.NAME:OSTaskSemSet2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) while 0 ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 2
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemSet.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSemSet.p_tcb[0].SemCtr:12
TEST.VALUE:sysTask.OSTaskSemSet.cnt:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemSet.p_tcb[0].SemCtr:0
TEST.EXPECTED:sysTask.OSTaskSemSet.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OSTaskSemSet.return:12
TEST.END

-- Test Case: OSTaskSemSet3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSemSet
TEST.NEW
TEST.NAME:OSTaskSemSet3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) while 0 ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (4) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 2
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].SemCtr:<<MAX>>
TEST.VALUE:sysTask.OSTaskSemSet.p_tcb:<<null>>
TEST.VALUE:sysTask.OSTaskSemSet.cnt:<<MIN>>
TEST.VALUE:sysTask.OSTaskSemSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSemSet.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OSTaskSemSet.return:<<MAX>>
TEST.END

-- Subprogram: OSTaskStkChk

-- Test Case: OSTaskStkChk1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskStkChk
TEST.NEW
TEST.NAME:OSTaskStkChk1
TEST.BASIS_PATH:1 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_free:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_used:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskStkChk.p_err[0]:OS_ERR_TASK_STK_CHK_ISR
TEST.END

-- Test Case: OSTaskStkChk2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskStkChk
TEST.NEW
TEST.NAME:OSTaskStkChk2
TEST.BASIS_PATH:2 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_free == (CPU_STK_SIZE *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_free:<<null>>
TEST.VALUE:sysTask.OSTaskStkChk.p_used:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskStkChk.p_err[0]:OS_ERR_PTR_INVALID
TEST.END

-- Test Case: OSTaskStkChk3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskStkChk
TEST.NEW
TEST.NAME:OSTaskStkChk3
TEST.BASIS_PATH:3 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_free == (CPU_STK_SIZE *)0) ==> FALSE
      (3) if (p_used == (CPU_STK_SIZE *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_free:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_used:<<null>>
TEST.VALUE:sysTask.OSTaskStkChk.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskStkChk.p_err[0]:OS_ERR_PTR_INVALID
TEST.END

-- Test Case: OSTaskStkChk4
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskStkChk
TEST.NEW
TEST.NAME:OSTaskStkChk4
TEST.BASIS_PATH:4 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_free == (CPU_STK_SIZE *)0) ==> FALSE
      (3) if (p_used == (CPU_STK_SIZE *)0) ==> FALSE
      (4) while 0 ==> FALSE
      (5) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) if (p_tcb->StkPtr == (CPU_STK *)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 4
      Only one possible value for condition in branch 7
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].StkPtr:<<null>>
TEST.VALUE:sysTask.OSTaskStkChk.p_free:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_used:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskStkChk.p_free[0]:0
TEST.EXPECTED:sysTask.OSTaskStkChk.p_used[0]:0
TEST.EXPECTED:sysTask.OSTaskStkChk.p_err[0]:OS_ERR_TASK_NOT_EXIST
TEST.END

-- Test Case: OSTaskStkChk5
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskStkChk
TEST.NEW
TEST.NAME:OSTaskStkChk5
TEST.BASIS_PATH:6 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_free == (CPU_STK_SIZE *)0) ==> FALSE
      (3) if (p_used == (CPU_STK_SIZE *)0) ==> FALSE
      (4) while 0 ==> FALSE
      (5) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) if (p_tcb->StkPtr == (CPU_STK *)0) ==> FALSE
      (8) if ((p_tcb->Opt & (OS_OPT)0x1U) == (OS_OPT)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 4
      Only one possible value for condition in branch 9
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].StkPtr:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].Opt:0
TEST.VALUE:sysTask.OSTaskStkChk.p_free:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_used:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskStkChk.p_free[0]:0
TEST.EXPECTED:sysTask.OSTaskStkChk.p_used[0]:0
TEST.EXPECTED:sysTask.OSTaskStkChk.p_err[0]:OS_ERR_TASK_OPT
TEST.END

-- Test Case: OSTaskStkChk6
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskStkChk
TEST.NEW
TEST.NAME:OSTaskStkChk6
TEST.BASIS_PATH:8 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 8
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_free == (CPU_STK_SIZE *)0) ==> FALSE
      (3) if (p_used == (CPU_STK_SIZE *)0) ==> FALSE
      (4) while 0 ==> FALSE
      (5) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) if (p_tcb->StkPtr == (CPU_STK *)0) ==> FALSE
      (8) if ((p_tcb->Opt & (OS_OPT)0x1U) == (OS_OPT)0) ==> FALSE
      (11) while (*p_stk == (CPU_STK)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 4
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].StkPtr:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].StkBasePtr:<<malloc 2>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].StkBasePtr[0]:<<MIN>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].StkBasePtr[1]:1
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].StkSize:2
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].Opt:1
TEST.VALUE:sysTask.OSTaskStkChk.p_free:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_used:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskStkChk.p_free[0]:1
TEST.EXPECTED:sysTask.OSTaskStkChk.p_used[0]:1
TEST.EXPECTED:sysTask.OSTaskStkChk.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSTaskStkChk7
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskStkChk
TEST.NEW
TEST.NAME:OSTaskStkChk7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_free == (CPU_STK_SIZE *)0) ==> FALSE
      (3) if (p_used == (CPU_STK_SIZE *)0) ==> FALSE
      (4) while 0 ==> FALSE
      (5) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) if (p_tcb->StkPtr == (CPU_STK *)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 4
      Only one possible value for condition in branch 7
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].StkPtr:<<null>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb:<<null>>
TEST.VALUE:sysTask.OSTaskStkChk.p_free:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_used:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskStkChk.p_free[0]:0
TEST.EXPECTED:sysTask.OSTaskStkChk.p_used[0]:0
TEST.EXPECTED:sysTask.OSTaskStkChk.p_err[0]:OS_ERR_TASK_NOT_EXIST
TEST.END

-- Test Case: OSTaskStkChk8
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskStkChk
TEST.NEW
TEST.NAME:OSTaskStkChk8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_free == (CPU_STK_SIZE *)0) ==> FALSE
      (3) if (p_used == (CPU_STK_SIZE *)0) ==> FALSE
      (4) while 0 ==> FALSE
      (5) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) if (p_tcb->StkPtr == (CPU_STK *)0) ==> FALSE
      (8) if ((p_tcb->Opt & (OS_OPT)0x1U) == (OS_OPT)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 4
      Only one possible value for condition in branch 9
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].StkPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].Opt:0
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb:<<null>>
TEST.VALUE:sysTask.OSTaskStkChk.p_free:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_used:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskStkChk.p_free[0]:0
TEST.EXPECTED:sysTask.OSTaskStkChk.p_used[0]:0
TEST.EXPECTED:sysTask.OSTaskStkChk.p_err[0]:OS_ERR_TASK_OPT
TEST.END

-- Test Case: OSTaskStkChk9
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskStkChk
TEST.NEW
TEST.NAME:OSTaskStkChk9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 8
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_free == (CPU_STK_SIZE *)0) ==> FALSE
      (3) if (p_used == (CPU_STK_SIZE *)0) ==> FALSE
      (4) while 0 ==> FALSE
      (5) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (6) if (p_tcb->StkPtr == (CPU_STK *)0) ==> FALSE
      (8) if ((p_tcb->Opt & (OS_OPT)0x1U) == (OS_OPT)0) ==> FALSE
      (11) while (*p_stk == (CPU_STK)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 4
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].StkPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].StkBasePtr:<<malloc 2>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].StkBasePtr[0]:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].StkBasePtr[1]:1
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].Opt:1
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].StkPtr:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].StkBasePtr:<<malloc 2>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].StkBasePtr[0]:<<MIN>>
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].StkBasePtr[1]:1
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].StkSize:2
TEST.VALUE:sysTask.OSTaskStkChk.p_tcb[0].Opt:1
TEST.VALUE:sysTask.OSTaskStkChk.p_free:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_used:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskStkChk.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskStkChk.p_free[0]:1
TEST.EXPECTED:sysTask.OSTaskStkChk.p_used[0]:1
TEST.EXPECTED:sysTask.OSTaskStkChk.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OSTaskSuspend

-- Test Case: OSTaskSuspend1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSuspend
TEST.NEW
TEST.NAME:OSTaskSuspend1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysTask.OSTaskSuspend.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSuspend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSuspend.p_err[0]:OS_ERR_TASK_SUSPEND_ISR
TEST.END

-- Test Case: OSTaskSuspend2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSuspend
TEST.NEW
TEST.NAME:OSTaskSuspend2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == &OSIdleTaskTCB) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable to unknown value in branch 2
TEST.END_NOTES:
TEST.STUB:sysTask.OS_TaskSuspend
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskSuspend.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskSuspend.p_err:<<malloc 1>>
TEST.END

-- Test Case: OSTaskSuspend3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskSuspend
TEST.NEW
TEST.NAME:OSTaskSuspend3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskSuspend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskSuspend.p_err[0]:OS_ERR_TASK_SUSPEND_IDLE
TEST.VALUE_USER_CODE:sysTask.OSTaskSuspend.p_tcb
<<sysTask.OSTaskSuspend.p_tcb>> = ( &OSIdleTaskTCB );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: OSTaskTimeQuantaSet

-- Test Case: OSTaskTImeQuantaSet1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskTimeQuantaSet
TEST.NEW
TEST.NAME:OSTaskTImeQuantaSet1
TEST.BASIS_PATH:1 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.time_quanta:<<MIN>>
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskTimeQuantaSet.p_err[0]:OS_ERR_SET_ISR
TEST.END

-- Test Case: OSTaskTImeQuantaSet2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskTimeQuantaSet
TEST.NEW
TEST.NAME:OSTaskTImeQuantaSet2
TEST.BASIS_PATH:2 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) while 0 ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (4) if (time_quanta == 0U) ==> FALSE
      (5) if (p_tcb->TimeQuanta > p_tcb->TimeQuantaCtr) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 2
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:1
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.p_tcb[0].TimeQuantaCtr:<<MAX>>
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.time_quanta:<<MIN>>
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskTimeQuantaSet.p_tcb[0].TimeQuanta:1
TEST.EXPECTED:sysTask.OSTaskTimeQuantaSet.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSTaskTImeQuantaSet3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskTimeQuantaSet
TEST.NEW
TEST.NAME:OSTaskTImeQuantaSet3
TEST.BASIS_PATH:3 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) while 0 ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (4) if (time_quanta == 0U) ==> FALSE
      (5) if (p_tcb->TimeQuanta > p_tcb->TimeQuantaCtr) ==> FALSE
      (6) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 2
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:1
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.p_tcb[0].TimeQuantaCtr:0
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.time_quanta:<<MIN>>
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskTimeQuantaSet.p_tcb[0].TimeQuanta:1
TEST.EXPECTED:sysTask.OSTaskTimeQuantaSet.p_tcb[0].TimeQuantaCtr:1
TEST.EXPECTED:sysTask.OSTaskTimeQuantaSet.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSTaskTImeQuantaSet4
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskTimeQuantaSet
TEST.NEW
TEST.NAME:OSTaskTImeQuantaSet4
TEST.BASIS_PATH:4 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) while 0 ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (4) if (time_quanta == 0U) ==> FALSE
      (5) if (p_tcb->TimeQuanta > p_tcb->TimeQuantaCtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 2
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.p_tcb[0].TimeQuantaCtr:<<MIN>>
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.time_quanta:<<MAX>>
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskTimeQuantaSet.p_tcb[0].TimeQuanta:<<MAX>>
TEST.EXPECTED:sysTask.OSTaskTimeQuantaSet.p_tcb[0].TimeQuantaCtr:<<MAX>>
TEST.EXPECTED:sysTask.OSTaskTimeQuantaSet.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSTaskTImeQuantaSet5
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OSTaskTimeQuantaSet
TEST.NEW
TEST.NAME:OSTaskTImeQuantaSet5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) while 0 ==> FALSE
      (3) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (4) if (time_quanta == 0U) ==> FALSE
      (5) if (p_tcb->TimeQuanta > p_tcb->TimeQuantaCtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 2
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TimeQuantaCtr:1
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.p_tcb:<<null>>
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.time_quanta:<<MAX>>
TEST.VALUE:sysTask.OSTaskTimeQuantaSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OSTaskTimeQuantaSet.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OS_TaskChangePrio

-- Test Case: OS_TaskChangePrio1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskChangePrio
TEST.NEW
TEST.NAME:OS_TaskChangePrio1
TEST.BASIS_PATH:1 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (23) case (p_tcb->TaskState) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb[0].TaskState:8
TEST.VALUE:sysTask.OS_TaskChangePrio.prio_new:<<MIN>>
TEST.END

-- Test Case: OS_TaskChangePrio2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskChangePrio
TEST.NEW
TEST.NAME:OS_TaskChangePrio2
TEST.BASIS_PATH:2 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (2) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (24) while (p_tcb != (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
TEST.END_NOTES:
TEST.STUB:sysCore.OS_RdyListInsertTail
TEST.STUB:sysCore.OS_RdyListRemove
TEST.STUB:sysPrio.OS_PrioInsert
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb[0].TaskState:0
TEST.VALUE:sysTask.OS_TaskChangePrio.prio_new:<<MIN>>
TEST.EXPECTED:sysTask.OS_TaskChangePrio.p_tcb[0].TaskState:0
TEST.END

-- Test Case: OS_TaskChangePrio3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskChangePrio
TEST.NEW
TEST.NAME:OS_TaskChangePrio3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (2) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (24) while (p_tcb != (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
TEST.END_NOTES:
TEST.STUB:sysCore.OS_RdyListInsertTail
TEST.STUB:sysCore.OS_RdyListRemove
TEST.STUB:sysPrio.OS_PrioInsert
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TaskState:0
TEST.VALUE:sysTask.OS_TaskChangePrio.prio_new:<<MIN>>
TEST.VALUE_USER_CODE:sysTask.OS_TaskChangePrio.p_tcb
<<sysTask.OS_TaskChangePrio.p_tcb>> = ( OSTCBCurPtr );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OS_TaskChangePrio4
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskChangePrio
TEST.NEW
TEST.NAME:OS_TaskChangePrio4
TEST.BASIS_PATH:3 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (2) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (24) while (p_tcb != (OS_TCB *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb[0].TaskState:1
TEST.VALUE:sysTask.OS_TaskChangePrio.prio_new:<<MIN>>
TEST.EXPECTED:sysTask.OS_TaskChangePrio.p_tcb[0].Prio:0
TEST.END

-- Test Case: OS_TaskChangePrio5
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskChangePrio
TEST.NEW
TEST.NAME:OS_TaskChangePrio5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (2) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (24) while (p_tcb != (OS_TCB *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb[0].TaskState:4
TEST.VALUE:sysTask.OS_TaskChangePrio.prio_new:<<MIN>>
TEST.EXPECTED:sysTask.OS_TaskChangePrio.p_tcb[0].Prio:0
TEST.END

-- Test Case: OS_TaskChangePrio6
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskChangePrio
TEST.NEW
TEST.NAME:OS_TaskChangePrio6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (2) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (24) while (p_tcb != (OS_TCB *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb[0].TaskState:5
TEST.VALUE:sysTask.OS_TaskChangePrio.prio_new:<<MIN>>
TEST.EXPECTED:sysTask.OS_TaskChangePrio.p_tcb[0].Prio:0
TEST.END

-- Test Case: OS_TaskChangePrio7
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskChangePrio
TEST.NEW
TEST.NAME:OS_TaskChangePrio7
TEST.BASIS_PATH:4 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (2) if (p_tcb == OSTCBCurPtr) ==> TRUE
      (24) while (p_tcb != (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb[0].PendOn:1
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb[0].TaskState:2
TEST.VALUE:sysTask.OS_TaskChangePrio.prio_new:<<MIN>>
TEST.EXPECTED:sysTask.OS_TaskChangePrio.p_tcb[0].Prio:0
TEST.END

-- Test Case: OS_TaskChangePrio8
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskChangePrio
TEST.NEW
TEST.NAME:OS_TaskChangePrio8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (2) if (p_tcb == OSTCBCurPtr) ==> TRUE
      (24) while (p_tcb != (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb[0].PendOn:2
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb[0].TaskState:2
TEST.VALUE:sysTask.OS_TaskChangePrio.prio_new:<<MIN>>
TEST.EXPECTED:sysTask.OS_TaskChangePrio.p_tcb[0].Prio:0
TEST.END

-- Test Case: OS_TaskChangePrio9
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskChangePrio
TEST.NEW
TEST.NAME:OS_TaskChangePrio9
TEST.BASIS_PATH:5 of 23
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (3) case (p_tcb->TaskState) ==> (OS_STATE)1U
      (24) while (p_tcb != (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_tcb_owner due to assignment
TEST.END_NOTES:
TEST.STUB:sysCore.OS_PendListChangePrio
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb[0].PendOn:4
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb[0].TaskState:7
TEST.VALUE:sysTask.OS_TaskChangePrio.p_tcb[0].Prio:1
TEST.VALUE:sysTask.OS_TaskChangePrio.prio_new:<<MAX>>
TEST.EXPECTED:sysTask.OS_TaskChangePrio.p_tcb[0].Prio:<<MAX>>
TEST.VALUE_USER_CODE:sysTask.OS_TaskChangePrio.p_tcb.p_tcb[0].PendDataTblPtr.PendDataTblPtr[0].PendObjPtr
OS_MUTEX *P,PP;
P=&PP;

<<sysTask.OS_TaskChangePrio.p_tcb>>[0].PendDataTblPtr[0].PendObjPtr = (P);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: OS_TaskInit

-- Test Case: OS_TaskInit
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskInit
TEST.NEW
TEST.NAME:OS_TaskInit
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskInit.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskCtxSwCtr:0
TEST.EXPECTED:sysTask.<<GLOBAL>>.OSTaskQty:0
TEST.EXPECTED:sysTask.OS_TaskInit.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OS_TaskInitTCB

-- Test Case: OS_TaskInitTCB
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskInitTCB
TEST.NEW
TEST.NAME:OS_TaskInitTCB
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskInitTCB.p_tcb:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].StkPtr:<<null>>
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].StkLimitPtr:<<null>>
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].NextPtr:<<null>>
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].PrevPtr:<<null>>
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].TickNextPtr:<<null>>
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].TickPrevPtr:<<null>>
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].TickListPtr:<<null>>
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].StkBasePtr:<<null>>
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].PendOn:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].PendStatus:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].TaskState:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].Prio:32
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].BasePrio:32
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].StkSize:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].Opt:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].TS:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].SemCtr:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].TickRemain:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].TickCtrPrev:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].TimeQuanta:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].TimeQuantaCtr:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].MsgSize:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].SuspendCtr:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].CPUUsage:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].CPUUsageMax:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].CtxSwCtr:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].CyclesDelta:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].CyclesStart:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].CyclesTotal:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].SemPendTime:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].SemPendTimeMax:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].StkUsed:0
TEST.EXPECTED:sysTask.OS_TaskInitTCB.p_tcb[0].StkFree:0
TEST.END

-- Subprogram: OS_TaskResume

-- Test Case: OS_TaskResume1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskResume
TEST.NEW
TEST.NAME:OS_TaskResume1
TEST.BASIS_PATH:1 of 20
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while 0 ==> FALSE
      (19) case (p_tcb->TaskState) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 20
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].TaskState:8
TEST.VALUE:sysTask.OS_TaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskResume.p_err[0]:OS_ERR_STATE_INVALID
TEST.END

-- Test Case: OS_TaskResume10
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskResume
TEST.NEW
TEST.NAME:OS_TaskResume10
TEST.BASIS_PATH:17 of 20
TEST.NOTES:
This is an automatically generated test case.
   Test Path 17
      (1) while 0 ==> FALSE
      (16) case (p_tcb->TaskState) ==> (OS_STATE)7U
      (17) if (p_tcb->SuspendCtr == (OS_NESTING_CTR)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_tcb->SuspendCtr due to assignment
      Only one possible value for condition in branch 18
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].TaskState:7
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:1
TEST.VALUE:sysTask.OS_TaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskResume.p_tcb[0].TaskState:3
TEST.EXPECTED:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:0
TEST.EXPECTED:sysTask.OS_TaskResume.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TaskResume11
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskResume
TEST.NEW
TEST.NAME:OS_TaskResume11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 17
      (1) while 0 ==> FALSE
      (16) case (p_tcb->TaskState) ==> (OS_STATE)7U
      (17) if (p_tcb->SuspendCtr == (OS_NESTING_CTR)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_tcb->SuspendCtr due to assignment
      Only one possible value for condition in branch 18
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].TaskState:7
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:4
TEST.VALUE:sysTask.OS_TaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:3
TEST.EXPECTED:sysTask.OS_TaskResume.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TaskResume12
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskResume
TEST.NEW
TEST.NAME:OS_TaskResume12
TEST.BASIS_PATH:14 of 20
TEST.NOTES:
This is an automatically generated test case.
   Test Path 14
      (1) while 0 ==> FALSE
      (13) case (p_tcb->TaskState) ==> (OS_STATE)6U
      (14) if (p_tcb->SuspendCtr == (OS_NESTING_CTR)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_tcb->SuspendCtr due to assignment
      Only one possible value for condition in branch 15
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].TaskState:6
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:3
TEST.VALUE:sysTask.OS_TaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:2
TEST.EXPECTED:sysTask.OS_TaskResume.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TaskResume13
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskResume
TEST.NEW
TEST.NAME:OS_TaskResume13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 14
      (1) while 0 ==> FALSE
      (13) case (p_tcb->TaskState) ==> (OS_STATE)6U
      (14) if (p_tcb->SuspendCtr == (OS_NESTING_CTR)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_tcb->SuspendCtr due to assignment
      Only one possible value for condition in branch 15
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].TaskState:6
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:1
TEST.VALUE:sysTask.OS_TaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskResume.p_tcb[0].TaskState:2
TEST.EXPECTED:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:0
TEST.EXPECTED:sysTask.OS_TaskResume.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TaskResume2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskResume
TEST.NEW
TEST.NAME:OS_TaskResume2
TEST.BASIS_PATH:3 of 20
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.VALUE:sysTask.OS_TaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].TaskState:0
TEST.VALUE:sysTask.OS_TaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskResume.p_err[0]:OS_ERR_TASK_NOT_SUSPENDED
TEST.END

-- Test Case: OS_TaskResume3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskResume
TEST.NEW
TEST.NAME:OS_TaskResume3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.VALUE:sysTask.OS_TaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].TaskState:1
TEST.VALUE:sysTask.OS_TaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskResume.p_err[0]:OS_ERR_TASK_NOT_SUSPENDED
TEST.END

-- Test Case: OS_TaskResume4
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskResume
TEST.NEW
TEST.NAME:OS_TaskResume4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.VALUE:sysTask.OS_TaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].TaskState:2
TEST.VALUE:sysTask.OS_TaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskResume.p_err[0]:OS_ERR_TASK_NOT_SUSPENDED
TEST.END

-- Test Case: OS_TaskResume5
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskResume
TEST.NEW
TEST.NAME:OS_TaskResume5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.VALUE:sysTask.OS_TaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].TaskState:3
TEST.VALUE:sysTask.OS_TaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskResume.p_err[0]:OS_ERR_TASK_NOT_SUSPENDED
TEST.END

-- Test Case: OS_TaskResume6
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskResume
TEST.NEW
TEST.NAME:OS_TaskResume6
TEST.BASIS_PATH:5 of 20
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) while 0 ==> FALSE
      (3) case (p_tcb->TaskState) ==> (OS_STATE)1U
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListInsert
TEST.VALUE:sysTask.OS_TaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].TaskState:4
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:1
TEST.VALUE:sysTask.OS_TaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskResume.p_tcb[0].TaskState:0
TEST.EXPECTED:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:0
TEST.EXPECTED:sysTask.OS_TaskResume.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TaskResume7
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskResume
TEST.NEW
TEST.NAME:OS_TaskResume7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) while 0 ==> FALSE
      (3) case (p_tcb->TaskState) ==> (OS_STATE)1U
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListInsert
TEST.VALUE:sysTask.OS_TaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].TaskState:4
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:2
TEST.VALUE:sysTask.OS_TaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:1
TEST.EXPECTED:sysTask.OS_TaskResume.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TaskResume8
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskResume
TEST.NEW
TEST.NAME:OS_TaskResume8
TEST.BASIS_PATH:9 of 20
TEST.NOTES:
This is an automatically generated test case.
   Test Path 9
      (1) while 0 ==> FALSE
      (7) case (p_tcb->TaskState) ==> (OS_STATE)4U
      (8) if (p_tcb->SuspendCtr == (OS_NESTING_CTR)0) ==> FALSE
      (9) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_tcb->SuspendCtr due to assignment
      Only one possible value for condition in branch 9
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].TaskState:5
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:1
TEST.VALUE:sysTask.OS_TaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskResume.p_tcb[0].TaskState:1
TEST.EXPECTED:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:0
TEST.EXPECTED:sysTask.OS_TaskResume.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TaskResume9
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskResume
TEST.NEW
TEST.NAME:OS_TaskResume9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 9
      (1) while 0 ==> FALSE
      (7) case (p_tcb->TaskState) ==> (OS_STATE)4U
      (8) if (p_tcb->SuspendCtr == (OS_NESTING_CTR)0) ==> FALSE
      (9) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_tcb->SuspendCtr due to assignment
      Only one possible value for condition in branch 9
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskResume.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].TaskState:5
TEST.VALUE:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:255
TEST.VALUE:sysTask.OS_TaskResume.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskResume.p_tcb[0].SuspendCtr:254
TEST.EXPECTED:sysTask.OS_TaskResume.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OS_TaskReturn

-- Test Case: OS_TaskReturn
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskReturn
TEST.NEW
TEST.NAME:OS_TaskReturn
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:sysTask.OSTaskDel
TEST.END

-- Subprogram: OS_TaskSemPost

-- Test Case: OS_TaskSemPost1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSemPost
TEST.NEW
TEST.NAME:OS_TaskSemPost1
TEST.BASIS_PATH:1 of 35
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (36) case (p_tcb->TaskState) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 37
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].TaskState:8
TEST.VALUE:sysTask.OS_TaskSemPost.opt:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.ts:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_tcb[0].TS:0
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_err[0]:OS_ERR_STATE_INVALID
TEST.EXPECTED:sysTask.OS_TaskSemPost.return:0
TEST.END

-- Test Case: OS_TaskSemPost10
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSemPost
TEST.NEW
TEST.NAME:OS_TaskSemPost10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (16) case sizeof(OS_SEM_CTR) ==> default
      (17) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 7
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 10
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 13
      Only one possible value for condition in branch 17
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Post
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].PendOn:5
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].TaskState:2
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].SemCtr:12
TEST.VALUE:sysTask.OS_TaskSemPost.opt:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.ts:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_tcb[0].TS:0
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_tcb[0].SemCtr:13
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OS_TaskSemPost.return:13
TEST.END

-- Test Case: OS_TaskSemPost11
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSemPost
TEST.NEW
TEST.NAME:OS_TaskSemPost11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (16) case sizeof(OS_SEM_CTR) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 7
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 10
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 13
      Only one possible value for condition in branch 17
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].TaskState:1
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].SemCtr:0
TEST.VALUE:sysTask.OS_TaskSemPost.opt:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.ts:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OS_TaskSemPost.return:1
TEST.END

-- Test Case: OS_TaskSemPost2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSemPost
TEST.NEW
TEST.NAME:OS_TaskSemPost2
TEST.BASIS_PATH:2 of 35
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (36) case (p_tcb->TaskState) ==> default
      (37) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 37
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TaskState:8
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb:<<null>>
TEST.VALUE:sysTask.OS_TaskSemPost.opt:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.ts:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_err[0]:OS_ERR_STATE_INVALID
TEST.EXPECTED:sysTask.OS_TaskSemPost.return:0
TEST.END

-- Test Case: OS_TaskSemPost3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSemPost
TEST.NEW
TEST.NAME:OS_TaskSemPost3
TEST.BASIS_PATH:3 of 35
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (16) case sizeof(OS_SEM_CTR) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 7
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 10
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 13
      Only one possible value for condition in branch 17
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].TaskState:0
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].SemCtr:4294967295
TEST.VALUE:sysTask.OS_TaskSemPost.opt:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.ts:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_err[0]:OS_ERR_SEM_OVF
TEST.EXPECTED:sysTask.OS_TaskSemPost.return:0
TEST.END

-- Test Case: OS_TaskSemPost4
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSemPost
TEST.NEW
TEST.NAME:OS_TaskSemPost4
TEST.BASIS_PATH:4 of 35
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (16) case sizeof(OS_SEM_CTR) ==> default
      (17) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 7
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 10
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 13
      Only one possible value for condition in branch 17
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Post
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].PendOn:7
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].TaskState:2
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].SemCtr:12
TEST.VALUE:sysTask.OS_TaskSemPost.opt:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.ts:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_tcb[0].TS:0
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OS_TaskSemPost.return:12
TEST.END

-- Test Case: OS_TaskSemPost5
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSemPost
TEST.NEW
TEST.NAME:OS_TaskSemPost5
TEST.BASIS_PATH:5 of 35
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (7) case sizeof(OS_SEM_CTR) ==> 1U
      (8) if (p_tcb->SemCtr == 255U) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 7
      Only one possible value for condition in branch 9
TEST.END_NOTES:
TEST.STUB:sysCore.OS_Post
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].PendOn:7
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].TaskState:7
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].Opt:65535
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].SemCtr:255
TEST.VALUE:sysTask.OS_TaskSemPost.opt:<<MAX>>
TEST.VALUE:sysTask.OS_TaskSemPost.ts:<<MAX>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_tcb[0].TS:<<MAX>>
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_tcb[0].SemCtr:255
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysTask.OS_TaskSemPost.return:255
TEST.END

-- Test Case: OS_TaskSemPost6
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSemPost
TEST.NEW
TEST.NAME:OS_TaskSemPost6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (16) case sizeof(OS_SEM_CTR) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 7
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 10
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 13
      Only one possible value for condition in branch 17
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].TaskState:1
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].SemCtr:4294967295
TEST.VALUE:sysTask.OS_TaskSemPost.opt:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.ts:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_err[0]:OS_ERR_SEM_OVF
TEST.EXPECTED:sysTask.OS_TaskSemPost.return:0
TEST.END

-- Test Case: OS_TaskSemPost7
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSemPost
TEST.NEW
TEST.NAME:OS_TaskSemPost7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (16) case sizeof(OS_SEM_CTR) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 7
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 10
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 13
      Only one possible value for condition in branch 17
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].TaskState:4
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].SemCtr:4294967295
TEST.VALUE:sysTask.OS_TaskSemPost.opt:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.ts:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_err[0]:OS_ERR_SEM_OVF
TEST.EXPECTED:sysTask.OS_TaskSemPost.return:0
TEST.END

-- Test Case: OS_TaskSemPost8
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSemPost
TEST.NEW
TEST.NAME:OS_TaskSemPost8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (16) case sizeof(OS_SEM_CTR) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 7
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 10
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 13
      Only one possible value for condition in branch 17
TEST.END_NOTES:
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].TaskState:5
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].SemCtr:4294967295
TEST.VALUE:sysTask.OS_TaskSemPost.opt:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.ts:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_err[0]:OS_ERR_SEM_OVF
TEST.EXPECTED:sysTask.OS_TaskSemPost.return:0
TEST.END

-- Test Case: OS_TaskSemPost9
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSemPost
TEST.NEW
TEST.NAME:OS_TaskSemPost9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) case (p_tcb->TaskState) ==> (OS_STATE)0U
      (16) case sizeof(OS_SEM_CTR) ==> default
      (17) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 7
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 10
      Cannot set switch condition sizeof(OS_SEM_CTR) in branch 13
      Only one possible value for condition in branch 17
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Post
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].PendOn:5
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].TaskState:2
TEST.VALUE:sysTask.OS_TaskSemPost.p_tcb[0].SemCtr:4294967295
TEST.VALUE:sysTask.OS_TaskSemPost.opt:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.ts:<<MIN>>
TEST.VALUE:sysTask.OS_TaskSemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_tcb[0].TS:0
TEST.EXPECTED:sysTask.OS_TaskSemPost.p_err[0]:OS_ERR_SEM_OVF
TEST.EXPECTED:sysTask.OS_TaskSemPost.return:0
TEST.END

-- Subprogram: OS_TaskSuspend

-- Test Case: OS_TaskSuspend1
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSuspend
TEST.NEW
TEST.NAME:OS_TaskSuspend1
TEST.BASIS_PATH:1 of 20
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (19) case (p_tcb->TaskState) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 20
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSSchedLockNestingCtr:<<MAX>>
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb:<<null>>
TEST.VALUE:sysTask.OS_TaskSuspend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_err[0]:OS_ERR_SCHED_LOCKED
TEST.END

-- Test Case: OS_TaskSuspend10
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSuspend
TEST.NEW
TEST.NAME:OS_TaskSuspend10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (6) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 7
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListRemove
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb[0].TaskState:5
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb[0].SuspendCtr:1
TEST.VALUE:sysTask.OS_TaskSuspend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_tcb[0].SuspendCtr:2
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TaskSuspend11
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSuspend
TEST.NEW
TEST.NAME:OS_TaskSuspend11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (6) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 7
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListRemove
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb[0].TaskState:6
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb[0].SuspendCtr:1
TEST.VALUE:sysTask.OS_TaskSuspend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_tcb[0].SuspendCtr:2
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TaskSuspend12
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSuspend
TEST.NEW
TEST.NAME:OS_TaskSuspend12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (6) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 7
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListRemove
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb[0].TaskState:7
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb[0].SuspendCtr:1
TEST.VALUE:sysTask.OS_TaskSuspend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_tcb[0].SuspendCtr:2
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TaskSuspend2
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSuspend
TEST.NEW
TEST.NAME:OS_TaskSuspend2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (19) case (p_tcb->TaskState) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 20
TEST.END_NOTES:
TEST.VALUE:sysTask.<<GLOBAL>>.OSSchedLockNestingCtr:<<MAX>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSuspend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_err[0]:OS_ERR_SCHED_LOCKED
TEST.VALUE_USER_CODE:sysTask.OS_TaskSuspend.p_tcb
<<sysTask.OS_TaskSuspend.p_tcb>> = ( OSTCBCurPtr );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OS_TaskSuspend3
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSuspend
TEST.NEW
TEST.NAME:OS_TaskSuspend3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (19) case (p_tcb->TaskState) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 20
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.VALUE:sysTask.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTask.<<GLOBAL>>.OSTCBCurPtr[0].TaskState:8
TEST.VALUE:sysTask.OS_TaskSuspend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_err[0]:OS_ERR_STATE_INVALID
TEST.VALUE_USER_CODE:sysTask.OS_TaskSuspend.p_tcb
<<sysTask.OS_TaskSuspend.p_tcb>> = ( OSTCBCurPtr );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OS_TaskSuspend4
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSuspend
TEST.NEW
TEST.NAME:OS_TaskSuspend4
TEST.BASIS_PATH:2 of 20
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (19) case (p_tcb->TaskState) ==> default
      (20) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 20
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb[0].TaskState:8
TEST.VALUE:sysTask.OS_TaskSuspend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_err[0]:OS_ERR_STATE_INVALID
TEST.END

-- Test Case: OS_TaskSuspend5
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSuspend
TEST.NEW
TEST.NAME:OS_TaskSuspend5
TEST.BASIS_PATH:3 of 20
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (6) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 7
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListRemove
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb[0].TaskState:0
TEST.VALUE:sysTask.OS_TaskSuspend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_tcb[0].TaskState:4
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_tcb[0].SuspendCtr:1
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TaskSuspend6
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSuspend
TEST.NEW
TEST.NAME:OS_TaskSuspend6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (6) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 7
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListRemove
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb[0].TaskState:1
TEST.VALUE:sysTask.OS_TaskSuspend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_tcb[0].TaskState:5
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_tcb[0].SuspendCtr:1
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TaskSuspend7
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSuspend
TEST.NEW
TEST.NAME:OS_TaskSuspend7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (6) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 7
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListRemove
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb[0].TaskState:2
TEST.VALUE:sysTask.OS_TaskSuspend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_tcb[0].TaskState:6
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_tcb[0].SuspendCtr:1
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TaskSuspend8
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSuspend
TEST.NEW
TEST.NAME:OS_TaskSuspend8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (6) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 7
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListRemove
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb[0].TaskState:3
TEST.VALUE:sysTask.OS_TaskSuspend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_tcb[0].TaskState:7
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_tcb[0].SuspendCtr:1
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TaskSuspend9
TEST.UNIT:sysTask
TEST.SUBPROGRAM:OS_TaskSuspend
TEST.NEW
TEST.NAME:OS_TaskSuspend9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == OSTCBCurPtr) ==> FALSE
      (6) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 7
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListRemove
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb:<<malloc 1>>
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb[0].TaskState:4
TEST.VALUE:sysTask.OS_TaskSuspend.p_tcb[0].SuspendCtr:1
TEST.VALUE:sysTask.OS_TaskSuspend.p_err:<<malloc 1>>
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_tcb[0].SuspendCtr:2
TEST.EXPECTED:sysTask.OS_TaskSuspend.p_err[0]:OS_ERR_NONE
TEST.END
