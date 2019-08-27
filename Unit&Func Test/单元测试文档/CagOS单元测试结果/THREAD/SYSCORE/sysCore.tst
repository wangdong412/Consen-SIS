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

-- Unit: sysCore

-- Subprogram: OSInit

-- Test Case: OSInit1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSInit
TEST.NEW
TEST.NAME:OSInit1
TEST.BASIS_PATH:1 of 10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSCfg_ISRStkSize > (CPU_STK_SIZE)0) ==> FALSE
      (4) if (*p_err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:sysCore.<<GLOBAL>>.OSCfg_ISRStkSize: in branch 1
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgPoolInit
TEST.VALUE:sysCore.OSInit.p_err:<<malloc 1>>
TEST.VALUE:sysCore.OSInit.p_err[0]:OS_ERR_A
TEST.VALUE:sysMsg.OS_MsgPoolInit.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskCreateHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskDelHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskReturnHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppIdleTaskHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppStatTaskHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskSwHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTimeTickHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSIntNestingCtr:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSRunning:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioCur:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioHighRdy:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioSaved:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:100
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.EXPECTED:sysCore.OSInit.p_err[0]:OS_ERR_A
TEST.END

-- Test Case: OSInit1.001
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSInit
TEST.NEW
TEST.NAME:OSInit1.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSCfg_ISRStkSize > (CPU_STK_SIZE)0) ==> FALSE
      (4) if (*p_err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:sysCore.<<GLOBAL>>.OSCfg_ISRStkSize: in branch 1
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgPoolInit
TEST.STUB:sysMutex.OS_MutexInit
TEST.VALUE:sysCore.OSInit.p_err:<<malloc 1>>
TEST.VALUE:sysCore.OSInit.p_err[0]:OS_ERR_A
TEST.VALUE:sysMsg.OS_MsgPoolInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysMutex.OS_MutexInit.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskCreateHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskDelHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskReturnHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppIdleTaskHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppStatTaskHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskSwHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTimeTickHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSIntNestingCtr:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSRunning:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioCur:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioHighRdy:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioSaved:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:100
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.EXPECTED:sysCore.OSInit.p_err[0]:OS_ERR_A
TEST.END

-- Test Case: OSInit1.002
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSInit
TEST.NEW
TEST.NAME:OSInit1.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSCfg_ISRStkSize > (CPU_STK_SIZE)0) ==> FALSE
      (4) if (*p_err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:sysCore.<<GLOBAL>>.OSCfg_ISRStkSize: in branch 1
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgPoolInit
TEST.STUB:sysMutex.OS_MutexInit
TEST.STUB:sysQueue.OS_QInit
TEST.VALUE:sysCore.OSInit.p_err:<<malloc 1>>
TEST.VALUE:sysCore.OSInit.p_err[0]:OS_ERR_A
TEST.VALUE:sysMsg.OS_MsgPoolInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysMutex.OS_MutexInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysQueue.OS_QInit.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskCreateHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskDelHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskReturnHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppIdleTaskHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppStatTaskHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskSwHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTimeTickHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSIntNestingCtr:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSRunning:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioCur:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioHighRdy:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioSaved:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:100
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.EXPECTED:sysCore.OSInit.p_err[0]:OS_ERR_A
TEST.END

-- Test Case: OSInit1.003
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSInit
TEST.NEW
TEST.NAME:OSInit1.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSCfg_ISRStkSize > (CPU_STK_SIZE)0) ==> FALSE
      (4) if (*p_err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:sysCore.<<GLOBAL>>.OSCfg_ISRStkSize: in branch 1
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgPoolInit
TEST.STUB:sysMutex.OS_MutexInit
TEST.STUB:sysQueue.OS_QInit
TEST.STUB:sysSem.OS_SemInit
TEST.VALUE:sysCore.OSInit.p_err:<<malloc 1>>
TEST.VALUE:sysCore.OSInit.p_err[0]:OS_ERR_A
TEST.VALUE:sysMsg.OS_MsgPoolInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysMutex.OS_MutexInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysQueue.OS_QInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysSem.OS_SemInit.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskCreateHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskDelHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskReturnHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppIdleTaskHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppStatTaskHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskSwHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTimeTickHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSIntNestingCtr:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSRunning:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioCur:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioHighRdy:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioSaved:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:100
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.EXPECTED:sysCore.OSInit.p_err[0]:OS_ERR_A
TEST.END

-- Test Case: OSInit1.004
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSInit
TEST.NEW
TEST.NAME:OSInit1.004
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSCfg_ISRStkSize > (CPU_STK_SIZE)0) ==> FALSE
      (4) if (*p_err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:sysCore.<<GLOBAL>>.OSCfg_ISRStkSize: in branch 1
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgPoolInit
TEST.STUB:sysMutex.OS_MutexInit
TEST.STUB:sysQueue.OS_QInit
TEST.STUB:sysSem.OS_SemInit
TEST.STUB:sysTask.OS_TaskInit
TEST.VALUE:sysCore.OSInit.p_err:<<malloc 1>>
TEST.VALUE:sysCore.OSInit.p_err[0]:OS_ERR_A
TEST.VALUE:sysMsg.OS_MsgPoolInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysMutex.OS_MutexInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysQueue.OS_QInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysSem.OS_SemInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysTask.OS_TaskInit.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskCreateHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskDelHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskReturnHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppIdleTaskHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppStatTaskHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskSwHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTimeTickHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSIntNestingCtr:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSRunning:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioCur:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioHighRdy:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioSaved:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:100
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.EXPECTED:sysCore.OSInit.p_err[0]:OS_ERR_A
TEST.END

-- Test Case: OSInit1.005
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSInit
TEST.NEW
TEST.NAME:OSInit1.005
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSCfg_ISRStkSize > (CPU_STK_SIZE)0) ==> FALSE
      (4) if (*p_err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:sysCore.<<GLOBAL>>.OSCfg_ISRStkSize: in branch 1
TEST.END_NOTES:
TEST.STUB:sysCore.OS_IdleTaskInit
TEST.STUB:sysMsg.OS_MsgPoolInit
TEST.STUB:sysMutex.OS_MutexInit
TEST.STUB:sysQueue.OS_QInit
TEST.STUB:sysSem.OS_SemInit
TEST.STUB:sysTask.OS_TaskInit
TEST.VALUE:sysCore.OSInit.p_err:<<malloc 1>>
TEST.VALUE:sysCore.OSInit.p_err[0]:OS_ERR_A
TEST.VALUE:sysCore.OS_IdleTaskInit.p_err[0]:OS_ERR_A
TEST.VALUE:sysMsg.OS_MsgPoolInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysMutex.OS_MutexInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysQueue.OS_QInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysSem.OS_SemInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysTask.OS_TaskInit.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskCreateHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskDelHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskReturnHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppIdleTaskHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppStatTaskHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskSwHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTimeTickHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSIntNestingCtr:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSRunning:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioCur:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioHighRdy:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioSaved:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:100
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.EXPECTED:sysCore.OSInit.p_err[0]:OS_ERR_A
TEST.END

-- Test Case: OSInit1.006
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSInit
TEST.NEW
TEST.NAME:OSInit1.006
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSCfg_ISRStkSize > (CPU_STK_SIZE)0) ==> FALSE
      (4) if (*p_err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:sysCore.<<GLOBAL>>.OSCfg_ISRStkSize: in branch 1
TEST.END_NOTES:
TEST.STUB:sysCore.OS_IdleTaskInit
TEST.STUB:sysMsg.OS_MsgPoolInit
TEST.STUB:sysMutex.OS_MutexInit
TEST.STUB:sysQueue.OS_QInit
TEST.STUB:sysSem.OS_SemInit
TEST.STUB:sysStat.OS_StatTaskInit
TEST.STUB:sysTask.OS_TaskInit
TEST.VALUE:sysCore.OSInit.p_err:<<malloc 1>>
TEST.VALUE:sysCore.OSInit.p_err[0]:OS_ERR_A
TEST.VALUE:sysCore.OS_IdleTaskInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysMsg.OS_MsgPoolInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysMutex.OS_MutexInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysQueue.OS_QInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysSem.OS_SemInit.p_err[0]:OS_ERR_NONE
TEST.VALUE:sysTask.OS_TaskInit.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskCreateHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskDelHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskReturnHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppIdleTaskHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppStatTaskHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTaskSwHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OS_AppTimeTickHookPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSIntNestingCtr:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSRunning:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioCur:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioHighRdy:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioSaved:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:100
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.EXPECTED:sysCore.OSInit.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OSIntEnter

-- Test Case: OSIntEnter1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSIntEnter
TEST.NEW
TEST.NAME:OSIntEnter1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSRunning != (OS_STATE)1U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:<<MIN>>
TEST.END

-- Test Case: OSIntEnter2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSIntEnter
TEST.NEW
TEST.NAME:OSIntEnter2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSRunning != (OS_STATE)1U) ==> FALSE
      (2) if (OSIntNestingCtr >= (OS_NESTING_CTR)250U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:1
TEST.END

-- Test Case: OSIntEnter3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSIntEnter
TEST.NEW
TEST.NAME:OSIntEnter3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSRunning != (OS_STATE)1U) ==> FALSE
      (2) if (OSIntNestingCtr >= (OS_NESTING_CTR)250U) ==> FALSE
      (3) if ((OSTCBCurPtr->Opt & (OS_OPT)0x4U) != (OS_OPT)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSTCBCurPtr[0].Opt:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSIntNestingCtr:1
TEST.END

-- Test Case: OSIntEnter4
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSIntEnter
TEST.NEW
TEST.NAME:OSIntEnter4
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSRunning != (OS_STATE)1U) ==> FALSE
      (2) if (OSIntNestingCtr >= (OS_NESTING_CTR)250U) ==> FALSE
      (3) if ((OSTCBCurPtr->Opt & (OS_OPT)0x4U) != (OS_OPT)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSTCBCurPtr[0].Opt:65535
TEST.END

-- Subprogram: OSIntExit

-- Test Case: OSIntExit1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSIntExit
TEST.NEW
TEST.NAME:OSIntExit1
TEST.BASIS_PATH:1 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSRunning != (OS_STATE)1U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:<<MIN>>
TEST.END

-- Test Case: OSIntExit2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSIntExit
TEST.NEW
TEST.NAME:OSIntExit2
TEST.BASIS_PATH:2 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSRunning != (OS_STATE)1U) ==> FALSE
      (2) if (OSIntNestingCtr == (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:0
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:1
TEST.END

-- Test Case: OSIntExit3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSIntExit
TEST.NEW
TEST.NAME:OSIntExit3
TEST.BASIS_PATH:3 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSRunning != (OS_STATE)1U) ==> FALSE
      (2) if (OSIntNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (3) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSIntNestingCtr due to assignment
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:2
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:1
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSIntNestingCtr:1
TEST.END

-- Test Case: OSIntExit4
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSIntExit
TEST.NEW
TEST.NAME:OSIntExit4
TEST.BASIS_PATH:4 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSRunning != (OS_STATE)1U) ==> FALSE
      (2) if (OSIntNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (3) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (4) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSIntNestingCtr due to assignment
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MAX>>
TEST.END

-- Test Case: OSIntExit5
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSIntExit
TEST.NEW
TEST.NAME:OSIntExit5
TEST.BASIS_PATH:5 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSRunning != (OS_STATE)1U) ==> FALSE
      (2) if (OSIntNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (3) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (4) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (5) if (OSTCBHighRdyPtr == OSTCBCurPtr) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSIntNestingCtr due to assignment
TEST.END_NOTES:
TEST.STUB:sysPrio.OS_PrioGetHighest
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].HeadPtr:<<null>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSTaskCtxSwCtr:0
TEST.VALUE:sysCore.<<GLOBAL>>.OSTCBCurPtr:<<null>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSTCBHighRdyPtr:<<malloc 1>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSTCBHighRdyPtr[0].CtxSwCtr:0
TEST.VALUE:sysPrio.OS_PrioGetHighest.return:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioHighRdy:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSTaskCtxSwCtr:0
TEST.END

-- Test Case: OSIntExit6
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSIntExit
TEST.NEW
TEST.NAME:OSIntExit6
TEST.BASIS_PATH:6 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSRunning != (OS_STATE)1U) ==> FALSE
      (2) if (OSIntNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (3) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (4) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (5) if (OSTCBHighRdyPtr == OSTCBCurPtr) ==> FALSE
   Test Case Generation Notes:
      Cannot set OSIntNestingCtr due to assignment
TEST.END_NOTES:
TEST.STUB:sysPrio.OS_PrioGetHighest
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSTaskCtxSwCtr:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSTCBHighRdyPtr:<<malloc 1>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSTCBHighRdyPtr[0].CtxSwCtr:1
TEST.VALUE:sysPrio.OS_PrioGetHighest.return:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSTaskCtxSwCtr:2
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSTCBHighRdyPtr[0].CtxSwCtr:1
TEST.END

-- Subprogram: OSSched

-- Test Case: OSSched1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSched
TEST.NEW
TEST.NAME:OSSched1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.END

-- Test Case: OSSched2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSched
TEST.NEW
TEST.NAME:OSSched2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MAX>>
TEST.END

-- Test Case: OSSched3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSched
TEST.NEW
TEST.NAME:OSSched3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (3) if (OSTCBHighRdyPtr == OSTCBCurPtr) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysPrio.OS_PrioGetHighest
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].HeadPtr:<<null>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSTCBCurPtr:<<null>>
TEST.VALUE:sysPrio.OS_PrioGetHighest.return:0
TEST.END

-- Test Case: OSSched4
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSched
TEST.NEW
TEST.NAME:OSSched4
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (3) if (OSTCBHighRdyPtr == OSTCBCurPtr) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysPrio.OS_PrioGetHighest
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSTaskCtxSwCtr:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysPrio.OS_PrioGetHighest.return:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSTaskCtxSwCtr:2
TEST.END

-- Subprogram: OSSchedLock

-- Test Case: OSSchedLock1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedLock
TEST.NEW
TEST.NAME:OSSchedLock1
TEST.BASIS_PATH:1 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysCore.OSSchedLock.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.OSSchedLock.p_err[0]:OS_ERR_SCHED_LOCK_ISR
TEST.END

-- Test Case: OSSchedLock2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedLock
TEST.NEW
TEST.NAME:OSSchedLock2
TEST.BASIS_PATH:2 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSRunning != (OS_STATE)1U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:<<MIN>>
TEST.VALUE:sysCore.OSSchedLock.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.OSSchedLock.p_err[0]:OS_ERR_OS_NOT_RUNNING
TEST.END

-- Test Case: OSSchedLock3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedLock
TEST.NEW
TEST.NAME:OSSchedLock3
TEST.BASIS_PATH:3 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSRunning != (OS_STATE)1U) ==> FALSE
      (3) if (OSSchedLockNestingCtr >= (OS_NESTING_CTR)250U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MAX>>
TEST.VALUE:sysCore.OSSchedLock.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.OSSchedLock.p_err[0]:OS_ERR_LOCK_NESTING_OVF
TEST.END

-- Test Case: OSSchedLock4
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedLock
TEST.NEW
TEST.NAME:OSSchedLock4
TEST.BASIS_PATH:4 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSRunning != (OS_STATE)1U) ==> FALSE
      (3) if (OSSchedLockNestingCtr >= (OS_NESTING_CTR)250U) ==> FALSE
      (4) while 0 ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 4
      Only one possible value for condition in branch 5
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysCore.OSSchedLock.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:1
TEST.EXPECTED:sysCore.OSSchedLock.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OSSchedRoundRobinCfg

-- Test Case: OSSchedRoundRobinCfg1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedRoundRobinCfg
TEST.NEW
TEST.NAME:OSSchedRoundRobinCfg1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while 0 ==> FALSE
      (2) if (en != 1U) ==> FALSE
      (3) if (dflt_time_quanta > (OS_TICK)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.en:1
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.dflt_time_quanta:<<MIN>>
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:100
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.EXPECTED:sysCore.OSSchedRoundRobinCfg.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSSchedRoundRobinCfg2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedRoundRobinCfg
TEST.NEW
TEST.NAME:OSSchedRoundRobinCfg2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while 0 ==> FALSE
      (2) if (en != 1U) ==> FALSE
      (3) if (dflt_time_quanta > (OS_TICK)0) ==> FALSE
      (4) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.en:1
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.dflt_time_quanta:<<MAX>>
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:<<MAX>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.EXPECTED:sysCore.OSSchedRoundRobinCfg.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSSchedRoundRobinCfg3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedRoundRobinCfg
TEST.NEW
TEST.NAME:OSSchedRoundRobinCfg3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (en != 1U) ==> FALSE
      (3) if (dflt_time_quanta > (OS_TICK)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.en:0
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.dflt_time_quanta:<<MAX>>
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:<<MAX>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:0
TEST.EXPECTED:sysCore.OSSchedRoundRobinCfg.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSSchedRoundRobinCfg3.001
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedRoundRobinCfg
TEST.NEW
TEST.NAME:OSSchedRoundRobinCfg3.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (en != 1U) ==> FALSE
      (3) if (dflt_time_quanta > (OS_TICK)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.en:0
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.dflt_time_quanta:<<MIN>>
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:100
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:0
TEST.EXPECTED:sysCore.OSSchedRoundRobinCfg.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSSchedRoundRobinCfg3.002
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedRoundRobinCfg
TEST.NEW
TEST.NAME:OSSchedRoundRobinCfg3.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (en != 1U) ==> FALSE
      (3) if (dflt_time_quanta > (OS_TICK)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.en:<<MAX>>
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.dflt_time_quanta:<<MAX>>
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:<<MAX>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:0
TEST.EXPECTED:sysCore.OSSchedRoundRobinCfg.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSSchedRoundRobinCfg3.003
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedRoundRobinCfg
TEST.NEW
TEST.NAME:OSSchedRoundRobinCfg3.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (en != 1U) ==> FALSE
      (3) if (dflt_time_quanta > (OS_TICK)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.en:<<MAX>>
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.dflt_time_quanta:<<MIN>>
TEST.VALUE:sysCore.OSSchedRoundRobinCfg.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:100
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:0
TEST.EXPECTED:sysCore.OSSchedRoundRobinCfg.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OSSchedRoundRobinYield

-- Test Case: RoundRobinYield1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedRoundRobinYield
TEST.NEW
TEST.NAME:RoundRobinYield1
TEST.BASIS_PATH:1 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysCore.OSSchedRoundRobinYield.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.OSSchedRoundRobinYield.p_err[0]:OS_ERR_YIELD_ISR
TEST.END

-- Test Case: RoundRobinYield2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedRoundRobinYield
TEST.NEW
TEST.NAME:RoundRobinYield2
TEST.BASIS_PATH:2 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MAX>>
TEST.VALUE:sysCore.OSSchedRoundRobinYield.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.OSSchedRoundRobinYield.p_err[0]:OS_ERR_SCHED_LOCKED
TEST.END

-- Test Case: RoundRobinYield3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedRoundRobinYield
TEST.NEW
TEST.NAME:RoundRobinYield3
TEST.BASIS_PATH:3 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (3) if (OSSchedRoundRobinEn != 1U) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSSchedRoundRobinEn due to assignment
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:0
TEST.VALUE:sysCore.OSSchedRoundRobinYield.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.OSSchedRoundRobinYield.p_err[0]:OS_ERR_ROUND_ROBIN_DISABLED
TEST.END

-- Test Case: RoundRobinYield4
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedRoundRobinYield
TEST.NEW
TEST.NAME:RoundRobinYield4
TEST.BASIS_PATH:4 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (3) if (OSSchedRoundRobinEn != 1U) ==> FALSE
      (4) while 0 ==> FALSE
      (5) if (p_rdy_list->NbrEntries < (OS_OBJ_QTY)2) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSSchedRoundRobinEn due to assignment
      Only one possible value for condition in branch 4
      Cannot set p_rdy_list due to assignment
      Cannot set local variable p_rdy_list in branch 5
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSPrioCur:0
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].NbrEntries:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.VALUE:sysCore.OSSchedRoundRobinYield.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.OSSchedRoundRobinYield.p_err[0]:OS_ERR_ROUND_ROBIN_1
TEST.END

-- Test Case: RoundRobinYield5
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedRoundRobinYield
TEST.NEW
TEST.NAME:RoundRobinYield5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (3) if (OSSchedRoundRobinEn != 1U) ==> FALSE
      (4) while 0 ==> FALSE
      (5) if (p_rdy_list->NbrEntries < (OS_OBJ_QTY)2) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSSchedRoundRobinEn due to assignment
      Only one possible value for condition in branch 4
      Cannot set p_rdy_list due to assignment
      Cannot set local variable p_rdy_list in branch 5
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListMoveHeadToTail
TEST.VALUE:sysCfg.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSPrioCur:0
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].HeadPtr[0].TimeQuanta:0
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].NbrEntries:<<MAX>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.VALUE:sysCore.OSSchedRoundRobinYield.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSRdyList[0].HeadPtr[0].TimeQuantaCtr:1
TEST.EXPECTED:sysCore.OSSchedRoundRobinYield.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: RoundRobinYield6
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedRoundRobinYield
TEST.NEW
TEST.NAME:RoundRobinYield6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (3) if (OSSchedRoundRobinEn != 1U) ==> FALSE
      (4) while 0 ==> FALSE
      (5) if (p_rdy_list->NbrEntries < (OS_OBJ_QTY)2) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSSchedRoundRobinEn due to assignment
      Only one possible value for condition in branch 4
      Cannot set p_rdy_list due to assignment
      Cannot set local variable p_rdy_list in branch 5
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_RdyListMoveHeadToTail
TEST.VALUE:sysCfg.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSPrioCur:0
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].HeadPtr[0].TimeQuanta:<<MAX>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].NbrEntries:<<MAX>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.VALUE:sysCore.OSSchedRoundRobinYield.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSRdyList[0].HeadPtr[0].TimeQuantaCtr:<<MAX>>
TEST.EXPECTED:sysCore.OSSchedRoundRobinYield.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OSSchedUnlock

-- Test Case: OSSchedUnlock1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedUnlock
TEST.NEW
TEST.NAME:OSSchedUnlock1
TEST.BASIS_PATH:1 of 8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:<<OPTIONS>>.REFERENCED_GLOBALS:FALSE
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysCore.OSSchedUnlock.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.OSSchedUnlock.p_err[0]:OS_ERR_SCHED_UNLOCK_ISR
TEST.END

-- Test Case: OSSchedUnlock2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedUnlock
TEST.NEW
TEST.NAME:OSSchedUnlock2
TEST.BASIS_PATH:2 of 8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSRunning != (OS_STATE)1U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:<<MIN>>
TEST.VALUE:sysCore.OSSchedUnlock.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.OSSchedUnlock.p_err[0]:OS_ERR_OS_NOT_RUNNING
TEST.END

-- Test Case: OSSchedUnlock3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedUnlock
TEST.NEW
TEST.NAME:OSSchedUnlock3
TEST.BASIS_PATH:3 of 8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSRunning != (OS_STATE)1U) ==> FALSE
      (3) if (OSSchedLockNestingCtr == (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysCore.OSSchedUnlock.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.OSSchedUnlock.p_err[0]:OS_ERR_SCHED_NOT_LOCKED
TEST.END

-- Test Case: OSSchedUnlokc4
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedUnlock
TEST.NEW
TEST.NAME:OSSchedUnlokc4
TEST.BASIS_PATH:4 of 8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSRunning != (OS_STATE)1U) ==> FALSE
      (3) if (OSSchedLockNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (4) while 0 ==> FALSE
      (5) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 4
      Cannot set OSSchedLockNestingCtr due to assignment
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:1
TEST.VALUE:sysCore.OSSchedUnlock.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.EXPECTED:sysCore.OSSchedUnlock.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSSchedUnlokc5
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSSchedUnlock
TEST.NEW
TEST.NAME:OSSchedUnlokc5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (OSRunning != (OS_STATE)1U) ==> FALSE
      (3) if (OSSchedLockNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (4) while 0 ==> FALSE
      (5) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 4
      Cannot set OSSchedLockNestingCtr due to assignment
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:2
TEST.VALUE:sysCore.OSSchedUnlock.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:1
TEST.EXPECTED:sysCore.OSSchedUnlock.p_err[0]:OS_ERR_SCHED_LOCKED
TEST.END

-- Subprogram: OSStart

-- Test Case: OSStart1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSStart
TEST.NEW
TEST.NAME:OSStart1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSRunning == (OS_STATE)0U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:1
TEST.VALUE:sysCore.OSStart.p_err:<<malloc 1>>
TEST.EXPECTED:sysCore.OSStart.p_err[0]:OS_ERR_OS_RUNNING
TEST.END

-- Test Case: OSStart2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OSStart
TEST.NEW
TEST.NAME:OSStart2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSRunning == (OS_STATE)0U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysPrio.OS_PrioGetHighest
TEST.VALUE:sysCore.<<GLOBAL>>.OSRunning:0
TEST.VALUE:sysCore.OSStart.p_err:<<malloc 1>>
TEST.VALUE:sysPrio.OS_PrioGetHighest.return:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSRunning:1
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioCur:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSPrioHighRdy:0
TEST.EXPECTED:sysCore.OSStart.p_err[0]:OS_ERR_FATAL_RETURN
TEST.END

-- Subprogram: OS_IdleTask

-- Test Case: BASIS-PATH-001-PARTIAL
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_IdleTask
TEST.NEW
TEST.NAME:BASIS-PATH-001-PARTIAL
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while 1U ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSIdleTaskCtr:0
TEST.VALUE:sysCore.OS_IdleTask.p_arg:VECTORCAST_INT1
TEST.END

-- Subprogram: OS_IdleTaskInit

-- Test Case: IdleTaskInit1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_IdleTaskInit
TEST.NEW
TEST.NAME:IdleTaskInit1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:sysTask.OSTaskCreate
TEST.VALUE:sysCore.OS_IdleTaskInit.p_err:<<malloc 1>>
TEST.VALUE:sysCore.OS_IdleTaskInit.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSIdleTaskCtr:0
TEST.END

-- Subprogram: OS_Pend

-- Test Case: OS_Pend1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Pend
TEST.NEW
TEST.NAME:OS_Pend1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysCore.OS_TaskBlock
TEST.VALUE:sysCore.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_Pend.p_pend_data:<<malloc 1>>
TEST.VALUE:sysCore.OS_Pend.p_obj:<<null>>
TEST.VALUE:sysCore.OS_Pend.pending_on:<<MIN>>
TEST.VALUE:sysCore.OS_Pend.timeout:<<MIN>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSTCBCurPtr[0].PendDataTblPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSTCBCurPtr[0].PendOn:<<MIN>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:0
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSTCBCurPtr[0].PendDataTblEntries:0
TEST.END

-- Test Case: OS_Pend2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Pend
TEST.NEW
TEST.NAME:OS_Pend2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysCore.OS_PendDataInit
TEST.STUB:sysCore.OS_PendListInsertPrio
TEST.STUB:sysCore.OS_TaskBlock
TEST.VALUE:sysCore.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_Pend.p_pend_data:<<malloc 1>>
TEST.VALUE:sysCore.OS_Pend.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_Pend.pending_on:<<MIN>>
TEST.VALUE:sysCore.OS_Pend.timeout:<<MIN>>
TEST.VALUE:sysCore.OS_PendDataInit.tbl_size:1
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSTCBCurPtr[0].PendDataTblPtr:<<null>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSTCBCurPtr[0].PendOn:<<MIN>>
TEST.EXPECTED:sysCore.<<GLOBAL>>.OSTCBCurPtr[0].PendDataTblEntries:0
TEST.END

-- Subprogram: OS_PendAbort

-- Test Case: PendAbort1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort
TEST.NEW
TEST.NAME:PendAbort1
TEST.BASIS_PATH:1 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (15) case (p_tcb->TaskState) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendAbort.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].TaskState:8
TEST.VALUE:sysCore.OS_PendAbort.ts:<<MIN>>
TEST.END

-- Test Case: PendAbort10
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort
TEST.NEW
TEST.NAME:PendAbort10
TEST.BASIS_PATH:11 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 11
      (10) case (p_tcb->TaskState) ==> (OS_STATE)6U
      (12) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (13) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
      (14) if (p_tcb->TaskState == (OS_STATE)7U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendAbort.p_obj:<<null>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].PendOn:<<MIN>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].TaskState:6
TEST.VALUE:sysCore.OS_PendAbort.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendStatus:1
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TaskState:4
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TS:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].MsgSize:0
TEST.END

-- Test Case: PendAbort11
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort
TEST.NEW
TEST.NAME:PendAbort11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 11
      (10) case (p_tcb->TaskState) ==> (OS_STATE)6U
      (12) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (13) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
      (14) if (p_tcb->TaskState == (OS_STATE)7U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendAbort.p_obj:<<null>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].PendOn:3
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].TaskState:6
TEST.VALUE:sysCore.OS_PendAbort.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendStatus:1
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TaskState:4
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TS:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].MsgSize:0
TEST.END

-- Test Case: PendAbort12
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort
TEST.NEW
TEST.NAME:PendAbort12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 11
      (10) case (p_tcb->TaskState) ==> (OS_STATE)6U
      (12) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (13) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
      (14) if (p_tcb->TaskState == (OS_STATE)7U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendAbort.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].PendOn:3
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].TaskState:6
TEST.VALUE:sysCore.OS_PendAbort.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendStatus:1
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TaskState:4
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TS:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].MsgSize:0
TEST.END

-- Test Case: PendAbort13
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort
TEST.NEW
TEST.NAME:PendAbort13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 11
      (10) case (p_tcb->TaskState) ==> (OS_STATE)6U
      (12) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (13) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
      (14) if (p_tcb->TaskState == (OS_STATE)7U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysCore.OS_PendAbort.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].PendOn:3
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].TaskState:7
TEST.VALUE:sysCore.OS_PendAbort.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendStatus:1
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TaskState:4
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TS:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].MsgSize:0
TEST.END

-- Test Case: PendAbort2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort
TEST.NEW
TEST.NAME:PendAbort2
TEST.BASIS_PATH:2 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendAbort.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].TaskState:0
TEST.VALUE:sysCore.OS_PendAbort.ts:<<MIN>>
TEST.END

-- Test Case: PendAbort3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort
TEST.NEW
TEST.NAME:PendAbort3
TEST.BASIS_PATH:3 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (2) case (p_tcb->TaskState) ==> (OS_STATE)1U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendAbort.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].TaskState:1
TEST.VALUE:sysCore.OS_PendAbort.ts:<<MIN>>
TEST.END

-- Test Case: PendAbort4
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort
TEST.NEW
TEST.NAME:PendAbort4
TEST.BASIS_PATH:4 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (3) case (p_tcb->TaskState) ==> (OS_STATE)4U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendAbort.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].TaskState:4
TEST.VALUE:sysCore.OS_PendAbort.ts:<<MIN>>
TEST.END

-- Test Case: PendAbort5
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort
TEST.NEW
TEST.NAME:PendAbort5
TEST.BASIS_PATH:5 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (4) case (p_tcb->TaskState) ==> (OS_STATE)5U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendAbort.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].TaskState:5
TEST.VALUE:sysCore.OS_PendAbort.ts:<<MIN>>
TEST.END

-- Test Case: PendAbort6
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort
TEST.NEW
TEST.NAME:PendAbort6
TEST.BASIS_PATH:6 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (5) case (p_tcb->TaskState) ==> (OS_STATE)2U
      (7) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (8) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
      (9) if (p_tcb->TaskState == (OS_STATE)3U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendAbort.p_obj:<<null>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].PendOn:<<MIN>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].TaskState:2
TEST.VALUE:sysCore.OS_PendAbort.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendStatus:1
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TaskState:0
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TS:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].MsgSize:0
TEST.END

-- Test Case: PendAbort7
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort
TEST.NEW
TEST.NAME:PendAbort7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (5) case (p_tcb->TaskState) ==> (OS_STATE)2U
      (7) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (8) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
      (9) if (p_tcb->TaskState == (OS_STATE)3U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendAbort.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].PendOn:<<MIN>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].TaskState:2
TEST.VALUE:sysCore.OS_PendAbort.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendStatus:1
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TaskState:0
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TS:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].MsgSize:0
TEST.END

-- Test Case: PendAbort8
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort
TEST.NEW
TEST.NAME:PendAbort8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (5) case (p_tcb->TaskState) ==> (OS_STATE)2U
      (7) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (8) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
      (9) if (p_tcb->TaskState == (OS_STATE)3U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendAbort.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].PendOn:3
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].TaskState:2
TEST.VALUE:sysCore.OS_PendAbort.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendStatus:1
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TaskState:0
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TS:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].MsgSize:0
TEST.END

-- Test Case: PendAbort9
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort
TEST.NEW
TEST.NAME:PendAbort9
TEST.BASIS_PATH:10 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 10
      (6) case (p_tcb->TaskState) ==> (OS_STATE)3U
      (7) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (8) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
      (9) if (p_tcb->TaskState == (OS_STATE)3U) ==> FALSE
   Test Case Generation Notes:
      Conflict: Trying to set variable sysCore.OS_PendAbort.p_tcb.TaskState 'equal to' and 'not equal to' same value in branches 6/9
TEST.END_NOTES:
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysCore.OS_PendAbort.p_obj:<<null>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].PendOn:<<MIN>>
TEST.VALUE:sysCore.OS_PendAbort.p_tcb[0].TaskState:3
TEST.VALUE:sysCore.OS_PendAbort.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].PendStatus:1
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TaskState:0
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].TS:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort.p_tcb[0].MsgSize:0
TEST.END

-- Subprogram: OS_PendAbort1

-- Test Case: OS_PendAbort1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort1
TEST.NEW
TEST.NAME:OS_PendAbort1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (n_pend_list > (OS_OBJ_QTY)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendAbort1.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort1.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort1.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort1.p_tcb[0].PendDataTblEntries:<<MIN>>
TEST.VALUE:sysCore.OS_PendAbort1.ts:<<MIN>>
TEST.END

-- Test Case: OS_PendAbort2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort1
TEST.NEW
TEST.NAME:OS_PendAbort2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (n_pend_list > (OS_OBJ_QTY)0) ==> TRUE
      (2) if (p_obj == p_pend_data->PendObjPtr) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendAbort1.p_obj:<<null>>
TEST.VALUE:sysCore.OS_PendAbort1.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort1.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort1.p_tcb[0].PendDataTblPtr[0].PendObjPtr:<<null>>
TEST.VALUE:sysCore.OS_PendAbort1.p_tcb[0].PendDataTblEntries:1
TEST.VALUE:sysCore.OS_PendAbort1.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendAbort1.p_tcb[0].PendDataTblPtr[0].RdyTS:<<MIN>>
TEST.END

-- Test Case: OS_PendAbort3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendAbort1
TEST.NEW
TEST.NAME:OS_PendAbort3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (n_pend_list > (OS_OBJ_QTY)0) ==> TRUE
      (2) if (p_obj == p_pend_data->PendObjPtr) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendAbort1.p_obj:<<null>>
TEST.VALUE:sysCore.OS_PendAbort1.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort1.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort1.p_tcb[0].PendDataTblPtr[0].PendObjPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendAbort1.p_tcb[0].PendDataTblEntries:<<MAX>>
TEST.VALUE:sysCore.OS_PendAbort1.ts:<<MIN>>
TEST.END

-- Subprogram: OS_PendDataInit

-- Test Case: PendData1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendDataInit
TEST.NEW
TEST.NAME:PendData1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for (i < tbl_size) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendDataInit.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendDataInit.p_pend_data_tbl:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendDataInit.tbl_size:0
TEST.EXPECTED:sysCore.OS_PendDataInit.p_tcb[0].PendDataTblEntries:0
TEST.END

-- Test Case: PendData2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendDataInit
TEST.NEW
TEST.NAME:PendData2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) for (i < tbl_size) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendDataInit.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendDataInit.p_pend_data_tbl:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendDataInit.tbl_size:1
TEST.EXPECTED:sysCore.OS_PendDataInit.p_tcb[0].PendDataTblEntries:1
TEST.EXPECTED:sysCore.OS_PendDataInit.p_pend_data_tbl[0].RdyMsgSize:0
TEST.EXPECTED:sysCore.OS_PendDataInit.p_pend_data_tbl[0].RdyTS:0
TEST.END

-- Subprogram: OS_PendListChangePrio

-- Test Case: BASIS-PATH-002-PARTIAL
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListChangePrio
TEST.NEW
TEST.NAME:BASIS-PATH-002-PARTIAL
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (n_pend_list > 0U) ==> TRUE
      (2) if (p_pend_list->NbrEntries > 1U) ==> FALSE
   Test Case Generation Notes:
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 2
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].PendObjPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].PendObjPtr[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].PendObjPtr[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].PendObjPtr[0].PendList.NbrEntries:0
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].RdyMsgPtr:<<null>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblEntries:1
TEST.END

-- Test Case: BASIS-PATH-003-PARTIAL
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListChangePrio
TEST.NEW
TEST.NAME:BASIS-PATH-003-PARTIAL
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (n_pend_list > 0U) ==> TRUE
      (2) if (p_pend_list->NbrEntries > 1U) ==> TRUE
   Test Case Generation Notes:
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 2
TEST.END_NOTES:
TEST.STUB:sysCore.OS_PendListInsertPrio
TEST.STUB:sysCore.OS_PendListRemove1
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr:<<malloc 2>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].PrevPtr:<<malloc 2>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].NextPtr:<<malloc 2>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].PendObjPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].PendObjPtr[0].PendList.HeadPtr:<<malloc 2>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].PendObjPtr[0].PendList.HeadPtr[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].PendObjPtr[0].PendList.HeadPtr[0].NextPtr:<<malloc 3>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].PendObjPtr[0].PendList.HeadPtr[1].PrevPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].PendObjPtr[0].PendList.HeadPtr[1].NextPtr:<<malloc 2>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].PendObjPtr[0].PendList.TailPtr:<<malloc 3>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].PendObjPtr[0].PendList.NbrEntries:2
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[1].PrevPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[1].NextPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[1].PendObjPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[1].PendObjPtr[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[1].PendObjPtr[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[1].PendObjPtr[0].PendList.NbrEntries:0
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblEntries:2
TEST.EXPECTED:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr[0].RdyObjPtr:<<null>>
TEST.END

-- Test Case: PendListChangePrio1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListChangePrio
TEST.NEW
TEST.NAME:PendListChangePrio1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (n_pend_list > 0U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListChangePrio.p_tcb[0].PendDataTblEntries:<<MIN>>
TEST.END

-- Subprogram: OS_PendListInit

-- Test Case: OS_PendListInit
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListInit
TEST.NEW
TEST.NAME:OS_PendListInit
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListInit.p_pend_list:<<malloc 1>>
TEST.EXPECTED:sysCore.OS_PendListInit.p_pend_list[0].HeadPtr:<<null>>
TEST.EXPECTED:sysCore.OS_PendListInit.p_pend_list[0].TailPtr:<<null>>
TEST.EXPECTED:sysCore.OS_PendListInit.p_pend_list[0].NbrEntries:0
TEST.END

-- Subprogram: OS_PendListInsertHead

-- Test Case: OS_PendListInsertHead1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListInsertHead
TEST.NEW
TEST.NAME:OS_PendListInsertHead1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_pend_data_next != (OS_PEND_DATA *)0) ==> FALSE
      (2) if (p_pend_list->NbrEntries == 1U) ==> FALSE
   Test Case Generation Notes:
      Cannot set p_pend_list->NbrEntries due to assignment
      Conflict: Trying to set variable sysCore.OS_PendListInsertHead.p_pend_list.HeadPtr 'equal to' and 'not equal to' same value in branches 1/2
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListInsertHead.p_pend_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertHead.p_pend_list[0].HeadPtr:<<null>>
TEST.VALUE:sysCore.OS_PendListInsertHead.p_pend_list[0].NbrEntries:0
TEST.VALUE:sysCore.OS_PendListInsertHead.p_pend_data:<<malloc 1>>
TEST.EXPECTED:sysCore.OS_PendListInsertHead.p_pend_list[0].NbrEntries:1
TEST.EXPECTED:sysCore.OS_PendListInsertHead.p_pend_data[0].PrevPtr:<<null>>
TEST.END

-- Test Case: OS_PendListInsertHead2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListInsertHead
TEST.NEW
TEST.NAME:OS_PendListInsertHead2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (p_pend_data_next != (OS_PEND_DATA *)0) ==> FALSE
      (2) if (p_pend_list->NbrEntries == 1U) ==> TRUE
   Test Case Generation Notes:
      Cannot set p_pend_list->NbrEntries due to assignment
      Conflict: Trying to set variable sysCore.OS_PendListInsertHead.p_pend_list.HeadPtr 'equal to' and 'not equal to' same value in branches 1/2
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListInsertHead.p_pend_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertHead.p_pend_list[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertHead.p_pend_list[0].NbrEntries:1
TEST.VALUE:sysCore.OS_PendListInsertHead.p_pend_data:<<malloc 1>>
TEST.EXPECTED:sysCore.OS_PendListInsertHead.p_pend_list[0].NbrEntries:2
TEST.EXPECTED:sysCore.OS_PendListInsertHead.p_pend_data[0].PrevPtr:<<null>>
TEST.END

-- Subprogram: OS_PendListInsertPrio

-- Test Case: OS_PendListInsertPrio1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListInsertPrio
TEST.NEW
TEST.NAME:OS_PendListInsertPrio1
TEST.BASIS_PATH:6 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].NbrEntries:0
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].TCBPtr:<<malloc 1>>
TEST.EXPECTED:sysCore.OS_PendListInsertPrio.p_pend_list[0].NbrEntries:1
TEST.EXPECTED:sysCore.OS_PendListInsertPrio.p_pend_data[0].PrevPtr:<<null>>
TEST.EXPECTED:sysCore.OS_PendListInsertPrio.p_pend_data[0].NextPtr:<<null>>
TEST.END

-- Test Case: OS_PendListInsertPrio2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListInsertPrio
TEST.NEW
TEST.NAME:OS_PendListInsertPrio2
TEST.BASIS_PATH:3 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
      (2) while (p_pend_data_next != (OS_PEND_DATA *)0) ==> FALSE
      (4) if (p_pend_data_next == (OS_PEND_DATA *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr:<<null>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].TailPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].NbrEntries:1
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].TCBPtr[0].Prio:1
TEST.EXPECTED:sysCore.OS_PendListInsertPrio.p_pend_list[0].NbrEntries:2
TEST.EXPECTED:sysCore.OS_PendListInsertPrio.p_pend_data[0].NextPtr:<<null>>
TEST.END

-- Test Case: OS_PendListInsertPrio3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListInsertPrio
TEST.NEW
TEST.NAME:OS_PendListInsertPrio3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
      (2) while (p_pend_data_next != (OS_PEND_DATA *)0) ==> FALSE
      (4) if (p_pend_data_next == (OS_PEND_DATA *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr[0].PrevPtr:<<null>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr[0].TCBPtr[0].Prio:2
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].TailPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].NbrEntries:1
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].TCBPtr[0].Prio:1
TEST.EXPECTED:sysCore.OS_PendListInsertPrio.p_pend_data[0].PrevPtr:<<null>>
TEST.END

-- Test Case: OS_PendListInsertPrio4
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListInsertPrio
TEST.NEW
TEST.NAME:OS_PendListInsertPrio4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
      (2) while (p_pend_data_next != (OS_PEND_DATA *)0) ==> FALSE
      (4) if (p_pend_data_next == (OS_PEND_DATA *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr[0].PrevPtr:<<null>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr[0].NextPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr[0].NextPtr[0].TCBPtr[0].Prio:30
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr[0].TCBPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr[0].TCBPtr[0].NextPtr[0].Prio:30
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr[0].TCBPtr[0].Prio:2
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].TailPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].NbrEntries:1
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].TCBPtr[0].Prio:10
TEST.END

-- Test Case: OS_PendListInsertPrio4.001
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListInsertPrio
TEST.NEW
TEST.NAME:OS_PendListInsertPrio4.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
      (2) while (p_pend_data_next != (OS_PEND_DATA *)0) ==> FALSE
      (4) if (p_pend_data_next == (OS_PEND_DATA *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].HeadPtr[0].TCBPtr[0].Prio:<<MAX>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].TailPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_list[0].NbrEntries:1
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListInsertPrio.p_pend_data[0].TCBPtr[0].Prio:10
TEST.EXPECTED:sysCore.OS_PendListInsertPrio.p_pend_list[0].NbrEntries:2
TEST.END

-- Subprogram: OS_PendListRemove

-- Test Case: BASIS-PATH-002
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListRemove
TEST.NEW
TEST.NAME:BASIS-PATH-002
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (n_pend_list > (OS_OBJ_QTY)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysCore.OS_PendListRemove1
TEST.VALUE:sysCore.OS_PendListRemove.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove.p_tcb[0].PendDataTblPtr[0].PendObjPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove.p_tcb[0].PendDataTblPtr[0].PendObjPtr[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove.p_tcb[0].PendDataTblPtr[0].PendObjPtr[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove.p_tcb[0].PendDataTblEntries:1
TEST.EXPECTED:sysCore.OS_PendListRemove.p_tcb[0].PendDataTblPtr:<<null>>
TEST.EXPECTED:sysCore.OS_PendListRemove.p_tcb[0].PendDataTblEntries:0
TEST.END

-- Test Case: PendListRemove1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListRemove
TEST.NEW
TEST.NAME:PendListRemove1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (n_pend_list > (OS_OBJ_QTY)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListRemove.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove.p_tcb[0].PendDataTblEntries:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendListRemove.p_tcb[0].PendDataTblPtr:<<null>>
TEST.EXPECTED:sysCore.OS_PendListRemove.p_tcb[0].PendDataTblEntries:0
TEST.END

-- Subprogram: OS_PendListRemove1

-- Test Case: BASIS-PATH-003
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListRemove1
TEST.NEW
TEST.NAME:BASIS-PATH-003
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_pend_list->NbrEntries == 1U) ==> FALSE
      (2) if (p_pend_data->PrevPtr == (OS_PEND_DATA *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_list[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_list[0].NbrEntries:<<MIN>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_data:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_data[0].PrevPtr:<<null>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_data[0].NextPtr:<<malloc 1>>
TEST.END

-- Test Case: BASIS-PATH-004
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListRemove1
TEST.NEW
TEST.NAME:BASIS-PATH-004
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (p_pend_list->NbrEntries == 1U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_list[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_list[0].TailPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_list[0].NbrEntries:1
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_data:<<malloc 1>>
TEST.EXPECTED:sysCore.OS_PendListRemove1.p_pend_list[0].HeadPtr:<<null>>
TEST.EXPECTED:sysCore.OS_PendListRemove1.p_pend_list[0].TailPtr:<<null>>
TEST.END

-- Test Case: PendListRemove1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListRemove1
TEST.NEW
TEST.NAME:PendListRemove1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_pend_list->NbrEntries == 1U) ==> FALSE
      (2) if (p_pend_data->PrevPtr == (OS_PEND_DATA *)0) ==> FALSE
      (3) if (p_pend_data->NextPtr == (OS_PEND_DATA *)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p_next in branch 3
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_list[0].NbrEntries:<<MIN>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_data:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_data[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_data[0].NextPtr:<<malloc 1>>
TEST.EXPECTED:sysCore.OS_PendListRemove1.p_pend_list[0].NbrEntries:65535
TEST.EXPECTED:sysCore.OS_PendListRemove1.p_pend_data[0].PrevPtr:<<null>>
TEST.EXPECTED:sysCore.OS_PendListRemove1.p_pend_data[0].NextPtr:<<null>>
TEST.END

-- Test Case: PendListRemove2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendListRemove1
TEST.NEW
TEST.NAME:PendListRemove2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (p_pend_list->NbrEntries == 1U) ==> FALSE
      (2) if (p_pend_data->PrevPtr == (OS_PEND_DATA *)0) ==> FALSE
      (3) if (p_pend_data->NextPtr == (OS_PEND_DATA *)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable p_next in branch 3
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_list[0].NbrEntries:<<MIN>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_data:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_data[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendListRemove1.p_pend_data[0].NextPtr:<<null>>
TEST.END

-- Subprogram: OS_PendObjDel

-- Test Case: PendObjDel1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendObjDel
TEST.NEW
TEST.NAME:PendObjDel1
TEST.BASIS_PATH:1 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (13) case (p_tcb->TaskState) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendObjDel.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].TaskState:8
TEST.VALUE:sysCore.OS_PendObjDel.ts:<<MIN>>
TEST.END

-- Test Case: PendObjDel10
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendObjDel
TEST.NEW
TEST.NAME:PendObjDel10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 10
      (9) case (p_tcb->TaskState) ==> (OS_STATE)6U
      (11) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (12) if (p_tcb->TaskState == (OS_STATE)7U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysCore.OS_PendObjDel.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].PendOn:3
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].TaskState:7
TEST.VALUE:sysCore.OS_PendObjDel.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].PendStatus:2
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].TaskState:4
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].TS:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].MsgSize:0
TEST.END

-- Test Case: PendObjDel11
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendObjDel
TEST.NEW
TEST.NAME:PendObjDel11
TEST.BASIS_PATH:10 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 10
      (9) case (p_tcb->TaskState) ==> (OS_STATE)6U
      (11) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (12) if (p_tcb->TaskState == (OS_STATE)7U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendObjDel.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].PendOn:<<MIN>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].TaskState:6
TEST.VALUE:sysCore.OS_PendObjDel.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].PendStatus:2
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].TaskState:4
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].TS:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].MsgSize:0
TEST.END

-- Test Case: PendObjDel2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendObjDel
TEST.NEW
TEST.NAME:PendObjDel2
TEST.BASIS_PATH:2 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendObjDel.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].TaskState:0
TEST.VALUE:sysCore.OS_PendObjDel.ts:<<MIN>>
TEST.END

-- Test Case: PendObjDel3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendObjDel
TEST.NEW
TEST.NAME:PendObjDel3
TEST.BASIS_PATH:3 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (2) case (p_tcb->TaskState) ==> (OS_STATE)1U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendObjDel.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].TaskState:1
TEST.VALUE:sysCore.OS_PendObjDel.ts:<<MIN>>
TEST.END

-- Test Case: PendObjDel4
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendObjDel
TEST.NEW
TEST.NAME:PendObjDel4
TEST.BASIS_PATH:4 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (3) case (p_tcb->TaskState) ==> (OS_STATE)4U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendObjDel.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].TaskState:4
TEST.VALUE:sysCore.OS_PendObjDel.ts:<<MIN>>
TEST.END

-- Test Case: PendObjDel5
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendObjDel
TEST.NEW
TEST.NAME:PendObjDel5
TEST.BASIS_PATH:5 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (4) case (p_tcb->TaskState) ==> (OS_STATE)5U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendObjDel.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].TaskState:5
TEST.VALUE:sysCore.OS_PendObjDel.ts:<<MIN>>
TEST.END

-- Test Case: PendObjDel6
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendObjDel
TEST.NEW
TEST.NAME:PendObjDel6
TEST.BASIS_PATH:6 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (5) case (p_tcb->TaskState) ==> (OS_STATE)2U
      (7) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (8) if (p_tcb->TaskState == (OS_STATE)3U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendObjDel.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].PendOn:<<MIN>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].TaskState:2
TEST.VALUE:sysCore.OS_PendObjDel.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].PendStatus:2
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].TaskState:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].TS:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].MsgSize:0
TEST.END

-- Test Case: PendObjDel7
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendObjDel
TEST.NEW
TEST.NAME:PendObjDel7
TEST.BASIS_PATH:7 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (5) case (p_tcb->TaskState) ==> (OS_STATE)2U
      (7) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (8) if (p_tcb->TaskState == (OS_STATE)3U) ==> TRUE
   Test Case Generation Notes:
      Conflict: Multiple equality operators with different values (sysCore.OS_PendObjDel.p_tcb.TaskState) in branches 5/8
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendObjDel.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].PendOn:3
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].TaskState:2
TEST.VALUE:sysCore.OS_PendObjDel.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].PendStatus:2
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].TaskState:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].TS:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].MsgSize:0
TEST.END

-- Test Case: PendObjDel8
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendObjDel
TEST.NEW
TEST.NAME:PendObjDel8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (5) case (p_tcb->TaskState) ==> (OS_STATE)2U
      (7) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (8) if (p_tcb->TaskState == (OS_STATE)3U) ==> TRUE
   Test Case Generation Notes:
      Conflict: Multiple equality operators with different values (sysCore.OS_PendObjDel.p_tcb.TaskState) in branches 5/8
TEST.END_NOTES:
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysCore.OS_PendObjDel.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].PendOn:3
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].TaskState:3
TEST.VALUE:sysCore.OS_PendObjDel.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].PendStatus:2
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].TaskState:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].TS:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].MsgSize:0
TEST.END

-- Test Case: PendObjDel9
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendObjDel
TEST.NEW
TEST.NAME:PendObjDel9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 10
      (9) case (p_tcb->TaskState) ==> (OS_STATE)6U
      (11) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (12) if (p_tcb->TaskState == (OS_STATE)7U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendObjDel.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].PendOn:3
TEST.VALUE:sysCore.OS_PendObjDel.p_tcb[0].TaskState:6
TEST.VALUE:sysCore.OS_PendObjDel.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].PendStatus:2
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].TaskState:4
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].TS:0
TEST.EXPECTED:sysCore.OS_PendObjDel.p_tcb[0].MsgSize:0
TEST.END

-- Subprogram: OS_PendObjDel1

-- Test Case: PendObjDel11
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendObjDel1
TEST.NEW
TEST.NAME:PendObjDel11
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (n_pend_list > (OS_OBJ_QTY)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendObjDel1.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel1.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel1.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel1.p_tcb[0].PendDataTblEntries:<<MIN>>
TEST.VALUE:sysCore.OS_PendObjDel1.ts:<<MIN>>
TEST.END

-- Test Case: PendObjDel12
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendObjDel1
TEST.NEW
TEST.NAME:PendObjDel12
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (n_pend_list > (OS_OBJ_QTY)0) ==> TRUE
      (2) if (p_obj == p_pend_data->PendObjPtr) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendObjDel1.p_obj:<<null>>
TEST.VALUE:sysCore.OS_PendObjDel1.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel1.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel1.p_tcb[0].PendDataTblPtr[0].PendObjPtr:<<null>>
TEST.VALUE:sysCore.OS_PendObjDel1.p_tcb[0].PendDataTblEntries:<<MAX>>
TEST.VALUE:sysCore.OS_PendObjDel1.ts:<<MIN>>
TEST.END

-- Test Case: PendObjDel13
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_PendObjDel1
TEST.NEW
TEST.NAME:PendObjDel13
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (n_pend_list > (OS_OBJ_QTY)0) ==> TRUE
      (2) if (p_obj == p_pend_data->PendObjPtr) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_PendObjDel1.p_obj:<<null>>
TEST.VALUE:sysCore.OS_PendObjDel1.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel1.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel1.p_tcb[0].PendDataTblPtr[0].PendObjPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_PendObjDel1.p_tcb[0].PendDataTblEntries:<<MAX>>
TEST.VALUE:sysCore.OS_PendObjDel1.ts:<<MIN>>
TEST.END

-- Subprogram: OS_Post

-- Test Case: OS_Post1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Post
TEST.NEW
TEST.NAME:OS_Post1
TEST.BASIS_PATH:1 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (15) case (p_tcb->TaskState) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_Post.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb[0].TaskState:8
TEST.VALUE:sysCore.OS_Post.p_void:VECTORCAST_INT1
TEST.VALUE:sysCore.OS_Post.msg_size:<<MIN>>
TEST.VALUE:sysCore.OS_Post.ts:<<MIN>>
TEST.END

-- Test Case: OS_Post10
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Post
TEST.NEW
TEST.NAME:OS_Post10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 11
      (10) case (p_tcb->TaskState) ==> (OS_STATE)6U
      (12) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (13) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
      (14) if (p_tcb->TaskState == (OS_STATE)7U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysCore.OS_Post.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb[0].PendOn:3
TEST.VALUE:sysCore.OS_Post.p_tcb[0].TaskState:7
TEST.VALUE:sysCore.OS_Post.p_void:VECTORCAST_INT1
TEST.VALUE:sysCore.OS_Post.msg_size:<<MIN>>
TEST.VALUE:sysCore.OS_Post.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].PendStatus:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].TaskState:4
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].TS:0
TEST.END

-- Test Case: OS_Post11
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Post
TEST.NEW
TEST.NAME:OS_Post11
TEST.BASIS_PATH:11 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 11
      (10) case (p_tcb->TaskState) ==> (OS_STATE)6U
      (12) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (13) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
      (14) if (p_tcb->TaskState == (OS_STATE)7U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_Post.p_obj:<<null>>
TEST.VALUE:sysCore.OS_Post.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb[0].PendOn:<<MIN>>
TEST.VALUE:sysCore.OS_Post.p_tcb[0].TaskState:6
TEST.VALUE:sysCore.OS_Post.p_void:VECTORCAST_INT1
TEST.VALUE:sysCore.OS_Post.msg_size:<<MIN>>
TEST.VALUE:sysCore.OS_Post.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].PendStatus:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].TaskState:4
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].TS:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].MsgSize:0
TEST.END

-- Test Case: OS_Post2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Post
TEST.NEW
TEST.NAME:OS_Post2
TEST.BASIS_PATH:2 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) case (p_tcb->TaskState) ==> (OS_STATE)0U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_Post.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb[0].TaskState:0
TEST.VALUE:sysCore.OS_Post.p_void:VECTORCAST_INT1
TEST.VALUE:sysCore.OS_Post.msg_size:<<MIN>>
TEST.VALUE:sysCore.OS_Post.ts:<<MIN>>
TEST.END

-- Test Case: OS_Post3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Post
TEST.NEW
TEST.NAME:OS_Post3
TEST.BASIS_PATH:3 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (2) case (p_tcb->TaskState) ==> (OS_STATE)1U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_Post.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb[0].TaskState:1
TEST.VALUE:sysCore.OS_Post.p_void:VECTORCAST_INT1
TEST.VALUE:sysCore.OS_Post.msg_size:<<MIN>>
TEST.VALUE:sysCore.OS_Post.ts:<<MIN>>
TEST.END

-- Test Case: OS_Post4
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Post
TEST.NEW
TEST.NAME:OS_Post4
TEST.BASIS_PATH:4 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (3) case (p_tcb->TaskState) ==> (OS_STATE)4U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_Post.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb[0].TaskState:4
TEST.VALUE:sysCore.OS_Post.p_void:VECTORCAST_INT1
TEST.VALUE:sysCore.OS_Post.msg_size:<<MIN>>
TEST.VALUE:sysCore.OS_Post.ts:<<MIN>>
TEST.END

-- Test Case: OS_Post5
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Post
TEST.NEW
TEST.NAME:OS_Post5
TEST.BASIS_PATH:5 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (4) case (p_tcb->TaskState) ==> (OS_STATE)5U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_Post.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb[0].TaskState:5
TEST.VALUE:sysCore.OS_Post.p_void:VECTORCAST_INT1
TEST.VALUE:sysCore.OS_Post.msg_size:<<MIN>>
TEST.VALUE:sysCore.OS_Post.ts:<<MIN>>
TEST.END

-- Test Case: OS_Post6
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Post
TEST.NEW
TEST.NAME:OS_Post6
TEST.BASIS_PATH:6 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (5) case (p_tcb->TaskState) ==> (OS_STATE)2U
      (7) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (8) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
      (9) if (p_tcb->TaskState == (OS_STATE)3U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_Post.p_obj:<<null>>
TEST.VALUE:sysCore.OS_Post.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb[0].PendOn:<<MIN>>
TEST.VALUE:sysCore.OS_Post.p_tcb[0].TaskState:2
TEST.VALUE:sysCore.OS_Post.p_void:VECTORCAST_INT1
TEST.VALUE:sysCore.OS_Post.msg_size:<<MIN>>
TEST.VALUE:sysCore.OS_Post.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].PendStatus:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].TaskState:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].TS:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].MsgSize:0
TEST.END

-- Test Case: OS_Post7
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Post
TEST.NEW
TEST.NAME:OS_Post7
TEST.BASIS_PATH:7 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (5) case (p_tcb->TaskState) ==> (OS_STATE)2U
      (7) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (8) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
      (9) if (p_tcb->TaskState == (OS_STATE)3U) ==> TRUE
   Test Case Generation Notes:
      Conflict: Multiple equality operators with different values (sysCore.OS_Post.p_tcb.TaskState) in branches 5/9
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_Post.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb[0].PendOn:3
TEST.VALUE:sysCore.OS_Post.p_tcb[0].TaskState:2
TEST.VALUE:sysCore.OS_Post.p_void:VECTORCAST_INT1
TEST.VALUE:sysCore.OS_Post.msg_size:<<MIN>>
TEST.VALUE:sysCore.OS_Post.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].PendStatus:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].TaskState:0
TEST.END

-- Test Case: OS_Post8
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Post
TEST.NEW
TEST.NAME:OS_Post8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (5) case (p_tcb->TaskState) ==> (OS_STATE)2U
      (7) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (8) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
      (9) if (p_tcb->TaskState == (OS_STATE)3U) ==> TRUE
   Test Case Generation Notes:
      Conflict: Multiple equality operators with different values (sysCore.OS_Post.p_tcb.TaskState) in branches 5/9
TEST.END_NOTES:
TEST.STUB:sysTick.OS_TickListRemove
TEST.VALUE:sysCore.OS_Post.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb[0].PendOn:3
TEST.VALUE:sysCore.OS_Post.p_tcb[0].TaskState:3
TEST.VALUE:sysCore.OS_Post.p_void:VECTORCAST_INT1
TEST.VALUE:sysCore.OS_Post.msg_size:<<MIN>>
TEST.VALUE:sysCore.OS_Post.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].PendStatus:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].TaskState:0
TEST.END

-- Test Case: OS_Post9
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Post
TEST.NEW
TEST.NAME:OS_Post9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 11
      (10) case (p_tcb->TaskState) ==> (OS_STATE)6U
      (12) if (p_tcb->PendOn == (OS_STATE)3U) ==> FALSE
      (13) if (p_obj != (OS_PEND_OBJ *)0) ==> FALSE
      (14) if (p_tcb->TaskState == (OS_STATE)7U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_Post.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post.p_tcb[0].PendOn:3
TEST.VALUE:sysCore.OS_Post.p_tcb[0].TaskState:6
TEST.VALUE:sysCore.OS_Post.p_void:VECTORCAST_INT1
TEST.VALUE:sysCore.OS_Post.msg_size:<<MIN>>
TEST.VALUE:sysCore.OS_Post.ts:<<MIN>>
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].PendOn:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].PendStatus:0
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].TaskState:4
TEST.EXPECTED:sysCore.OS_Post.p_tcb[0].TS:0
TEST.END

-- Subprogram: OS_Post1

-- Test Case: OS_Post1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Post1
TEST.NEW
TEST.NAME:OS_Post1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (n_pend_list > (OS_OBJ_QTY)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_Post1.p_obj:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post1.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post1.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post1.p_tcb[0].PendDataTblEntries:<<MIN>>
TEST.VALUE:sysCore.OS_Post1.p_void:VECTORCAST_INT1
TEST.VALUE:sysCore.OS_Post1.msg_size:<<MIN>>
TEST.VALUE:sysCore.OS_Post1.ts:<<MIN>>
TEST.END

-- Test Case: OS_Post2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Post1
TEST.NEW
TEST.NAME:OS_Post2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (n_pend_list > (OS_OBJ_QTY)0) ==> TRUE
      (2) if (p_obj == p_pend_data->PendObjPtr) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_Post1.p_obj:<<null>>
TEST.VALUE:sysCore.OS_Post1.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post1.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post1.p_tcb[0].PendDataTblPtr[0].PendObjPtr:<<null>>
TEST.VALUE:sysCore.OS_Post1.p_tcb[0].PendDataTblEntries:<<MAX>>
TEST.VALUE:sysCore.OS_Post1.p_void:VECTORCAST_INT1
TEST.VALUE:sysCore.OS_Post1.msg_size:<<MIN>>
TEST.VALUE:sysCore.OS_Post1.ts:<<MIN>>
TEST.END

-- Test Case: OS_Post3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_Post1
TEST.NEW
TEST.NAME:OS_Post3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (n_pend_list > (OS_OBJ_QTY)0) ==> TRUE
      (2) if (p_obj == p_pend_data->PendObjPtr) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_Post1.p_obj:<<null>>
TEST.VALUE:sysCore.OS_Post1.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post1.p_tcb[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post1.p_tcb[0].PendDataTblPtr[0].PendObjPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_Post1.p_tcb[0].PendDataTblEntries:1
TEST.VALUE:sysCore.OS_Post1.p_void:VECTORCAST_INT1
TEST.VALUE:sysCore.OS_Post1.msg_size:<<MIN>>
TEST.VALUE:sysCore.OS_Post1.ts:<<MIN>>
TEST.END

-- Subprogram: OS_RdyListInit

-- Test Case: RdyListInit1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListInit
TEST.NEW
TEST.NAME:RdyListInit1
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) for (i < 32U) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Cannot set local variable p_rdy_list in branch 2
      Cannot set local variable p_rdy_list in branch 2
      Cannot set local variable p_rdy_list in branch 2
TEST.END_NOTES:
TEST.END

-- Subprogram: OS_RdyListInsert

-- Test Case: RdyListInsert1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListInsert
TEST.NEW
TEST.NAME:RdyListInsert1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_tcb->Prio == OSPrioCur) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSPrioCur:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].NbrEntries:0
TEST.VALUE:sysCore.OS_RdyListInsert.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListInsert.p_tcb[0].Prio:<<MIN>>
TEST.EXPECTED:sysCore.OS_RdyListInsert.p_tcb[0].NextPtr:<<null>>
TEST.EXPECTED:sysCore.OS_RdyListInsert.p_tcb[0].PrevPtr:<<null>>
TEST.END

-- Test Case: RdyListInsert2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListInsert
TEST.NEW
TEST.NAME:RdyListInsert2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (p_tcb->Prio == OSPrioCur) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSPrioCur:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].NbrEntries:0
TEST.VALUE:sysCore.OS_RdyListInsert.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListInsert.p_tcb[0].Prio:<<MIN>>
TEST.EXPECTED:sysCore.OS_RdyListInsert.p_tcb[0].NextPtr:<<null>>
TEST.EXPECTED:sysCore.OS_RdyListInsert.p_tcb[0].PrevPtr:<<null>>
TEST.END

-- Subprogram: OS_RdyListInsertHead

-- Test Case: RdyLIstInsertHead1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListInsertHead
TEST.NEW
TEST.NAME:RdyLIstInsertHead1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_rdy_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set p_rdy_list due to assignment
      Cannot set local variable p_rdy_list in branch 1
      Cannot set local variable p_rdy_list in branch 2
      Cannot set local variable p_rdy_list in branch 2
      Cannot set local variable p_rdy_list in branch 2
      Cannot set p_rdy_list due to assignment
      Cannot set local variable p_rdy_list in branch 2
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].NbrEntries:0
TEST.VALUE:sysCore.OS_RdyListInsertHead.p_tcb:<<malloc 1>>
TEST.EXPECTED:sysCore.OS_RdyListInsertHead.p_tcb[0].NextPtr:<<null>>
TEST.EXPECTED:sysCore.OS_RdyListInsertHead.p_tcb[0].PrevPtr:<<null>>
TEST.END

-- Test Case: RdyLIstInsertHead2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListInsertHead
TEST.NEW
TEST.NAME:RdyLIstInsertHead2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (p_rdy_list->NbrEntries == (OS_OBJ_QTY)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set p_rdy_list due to assignment
      Cannot set local variable p_rdy_list in branch 1
      Cannot set local variable p_rdy_list in branch 2
      Cannot set local variable p_rdy_list in branch 2
      Cannot set local variable p_rdy_list in branch 2
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].NbrEntries:1
TEST.VALUE:sysCore.OS_RdyListInsertHead.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListInsertHead.p_tcb[0].NextPtr:<<malloc 1>>
TEST.EXPECTED:sysCore.OS_RdyListInsertHead.p_tcb[0].PrevPtr:<<null>>
TEST.END

-- Subprogram: OS_RdyListInsertTail

-- Test Case: RdyListInsetTail1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListInsertTail
TEST.NEW
TEST.NAME:RdyListInsetTail1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_rdy_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set p_rdy_list due to assignment
      Cannot set local variable p_rdy_list in branch 1
      Cannot set local variable p_rdy_list in branch 2
      Cannot set local variable p_rdy_list in branch 2
      Cannot set p_rdy_list due to assignment
      Cannot set local variable p_rdy_list in branch 2
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[0].NbrEntries:0
TEST.VALUE:sysCore.OS_RdyListInsertTail.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListInsertTail.p_tcb[0].Prio:0
TEST.EXPECTED:sysCore.OS_RdyListInsertTail.p_tcb[0].NextPtr:<<null>>
TEST.EXPECTED:sysCore.OS_RdyListInsertTail.p_tcb[0].PrevPtr:<<null>>
TEST.END

-- Test Case: RdyListInsetTail2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListInsertTail
TEST.NEW
TEST.NAME:RdyListInsetTail2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (p_rdy_list->NbrEntries == (OS_OBJ_QTY)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set p_rdy_list due to assignment
      Cannot set local variable p_rdy_list in branch 1
      Cannot set local variable p_rdy_list in branch 2
      Cannot set local variable p_rdy_list in branch 2
      Cannot set local variable p_rdy_list in branch 2
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[1].TailPtr:<<malloc 1>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSRdyList[1].NbrEntries:<<MAX>>
TEST.VALUE:sysCore.OS_RdyListInsertTail.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListInsertTail.p_tcb[0].Prio:1
TEST.EXPECTED:sysCore.OS_RdyListInsertTail.p_tcb[0].NextPtr:<<null>>
TEST.END

-- Subprogram: OS_RdyListMoveHeadToTail

-- Test Case: MoveHeadToTail1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListMoveHeadToTail
TEST.NEW
TEST.NAME:MoveHeadToTail1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (4) case (p_rdy_list->NbrEntries) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_RdyListMoveHeadToTail.p_rdy_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListMoveHeadToTail.p_rdy_list[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListMoveHeadToTail.p_rdy_list[0].HeadPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListMoveHeadToTail.p_rdy_list[0].TailPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListMoveHeadToTail.p_rdy_list[0].NbrEntries:3
TEST.END

-- Test Case: MoveHeadToTail2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListMoveHeadToTail
TEST.NEW
TEST.NAME:MoveHeadToTail2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) case (p_rdy_list->NbrEntries) ==> 0
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_RdyListMoveHeadToTail.p_rdy_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListMoveHeadToTail.p_rdy_list[0].NbrEntries:0
TEST.END

-- Test Case: MoveHeadToTail3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListMoveHeadToTail
TEST.NEW
TEST.NAME:MoveHeadToTail3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (2) case (p_rdy_list->NbrEntries) ==> 1
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_RdyListMoveHeadToTail.p_rdy_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListMoveHeadToTail.p_rdy_list[0].NbrEntries:1
TEST.END

-- Test Case: MoveHeadToTail4
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListMoveHeadToTail
TEST.NEW
TEST.NAME:MoveHeadToTail4
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (3) case (p_rdy_list->NbrEntries) ==> 2
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_RdyListMoveHeadToTail.p_rdy_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListMoveHeadToTail.p_rdy_list[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListMoveHeadToTail.p_rdy_list[0].TailPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListMoveHeadToTail.p_rdy_list[0].NbrEntries:2
TEST.END

-- Subprogram: OS_RdyListRemove

-- Test Case: RdyListRemove1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListRemove
TEST.NEW
TEST.NAME:RdyListRemove1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_tcb1 == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb2 == (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p_rdy_list in branch 3
      Cannot set local variable p_rdy_list in branch 3
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb[0].PrevPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb[0].Prio:0
TEST.EXPECTED:sysCore.OS_RdyListRemove.p_tcb[0].NextPtr:<<null>>
TEST.EXPECTED:sysCore.OS_RdyListRemove.p_tcb[0].PrevPtr:<<null>>
TEST.END

-- Test Case: RdyListRemove2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListRemove
TEST.NEW
TEST.NAME:RdyListRemove2
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (p_tcb1 == (OS_TCB *)0) ==> TRUE
      (2) if (p_tcb2 == (OS_TCB *)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable p_rdy_list in branch 3
      Cannot set local variable p_rdy_list in branch 3
      Cannot set local variable p_rdy_list in branch 3
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb[0].NextPtr:<<null>>
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb[0].PrevPtr:<<null>>
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb[0].Prio:0
TEST.EXPECTED:sysCore.OS_RdyListRemove.p_tcb[0].NextPtr:<<null>>
TEST.EXPECTED:sysCore.OS_RdyListRemove.p_tcb[0].PrevPtr:<<null>>
TEST.END

-- Test Case: RdyListRemove3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListRemove
TEST.NEW
TEST.NAME:RdyListRemove3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_tcb1 == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb2 == (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p_rdy_list in branch 3
      Cannot set local variable p_rdy_list in branch 3
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb[0].PrevPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb[0].Prio:0
TEST.EXPECTED:sysCore.OS_RdyListRemove.p_tcb[0].NextPtr:<<null>>
TEST.EXPECTED:sysCore.OS_RdyListRemove.p_tcb[0].PrevPtr:<<null>>
TEST.END

-- Test Case: RdyListRemove4
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_RdyListRemove
TEST.NEW
TEST.NAME:RdyListRemove4
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_tcb1 == (OS_TCB *)0) ==> TRUE
      (2) if (p_tcb2 == (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p_rdy_list in branch 3
      Cannot set local variable p_rdy_list in branch 3
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb[0].NextPtr[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_RdyListRemove.p_tcb[0].PrevPtr:<<null>>
TEST.EXPECTED:sysCore.OS_RdyListRemove.p_tcb[0].NextPtr:<<null>>
TEST.EXPECTED:sysCore.OS_RdyListRemove.p_tcb[0].NextPtr[0].PrevPtr:<<null>>
TEST.EXPECTED:sysCore.OS_RdyListRemove.p_tcb[0].PrevPtr:<<null>>
TEST.END

-- Subprogram: OS_SchedRoundRobin

-- Test Case: SchedRoundRobin1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_SchedRoundRobin
TEST.NEW
TEST.NAME:SchedRoundRobin1
TEST.BASIS_PATH:1 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSSchedRoundRobinEn != 1U) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSSchedRoundRobinEn due to assignment
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:0
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list:<<malloc 1>>
TEST.END

-- Test Case: SchedRoundRobin2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_SchedRoundRobin
TEST.NEW
TEST.NAME:SchedRoundRobin2
TEST.BASIS_PATH:2 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSSchedRoundRobinEn != 1U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSSchedRoundRobinEn due to assignment
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr:<<null>>
TEST.END

-- Test Case: SchedRoundRobin3
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_SchedRoundRobin
TEST.NEW
TEST.NAME:SchedRoundRobin3
TEST.BASIS_PATH:3 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSSchedRoundRobinEn != 1U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == &OSIdleTaskTCB) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSSchedRoundRobinEn due to assignment
      Cannot set variable to unknown value in branch 3
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list:<<malloc 1>>
TEST.VALUE_USER_CODE:sysCore.OS_SchedRoundRobin.p_rdy_list.p_rdy_list[0].HeadPtr
<<sysCore.OS_SchedRoundRobin.p_rdy_list>>[0].HeadPtr = ( &OSIdleTaskTCB );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: SchedRoundRobin4
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_SchedRoundRobin
TEST.NEW
TEST.NAME:SchedRoundRobin4
TEST.BASIS_PATH:4 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSSchedRoundRobinEn != 1U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (4) if (p_tcb->TimeQuantaCtr > (OS_TICK)0) ==> FALSE
      (5) if (p_tcb->TimeQuantaCtr > (OS_TICK)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSSchedRoundRobinEn due to assignment
      Conflict: Trying to set variable sysCore.OS_SchedRoundRobin.p_rdy_list.HeadPtr.TimeQuantaCtr 'less than' and 'greater than' same value in branches 4/5
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr[0].TimeQuantaCtr:2
TEST.EXPECTED:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr[0].TimeQuantaCtr:1
TEST.END

-- Test Case: SchedRoundRobin5
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_SchedRoundRobin
TEST.NEW
TEST.NAME:SchedRoundRobin5
TEST.BASIS_PATH:5 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSSchedRoundRobinEn != 1U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (4) if (p_tcb->TimeQuantaCtr > (OS_TICK)0) ==> FALSE
      (6) if (p_rdy_list->NbrEntries < (OS_OBJ_QTY)2) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSSchedRoundRobinEn due to assignment
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr[0].TimeQuantaCtr:1
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].NbrEntries:<<MIN>>
TEST.EXPECTED:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr[0].TimeQuantaCtr:0
TEST.END

-- Test Case: SchedRoundRobin6
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_SchedRoundRobin
TEST.NEW
TEST.NAME:SchedRoundRobin6
TEST.BASIS_PATH:6 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSSchedRoundRobinEn != 1U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (4) if (p_tcb->TimeQuantaCtr > (OS_TICK)0) ==> FALSE
      (6) if (p_rdy_list->NbrEntries < (OS_OBJ_QTY)2) ==> FALSE
      (7) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSSchedRoundRobinEn due to assignment
TEST.END_NOTES:
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MAX>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr[0].TimeQuantaCtr:<<MIN>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].NbrEntries:<<MAX>>
TEST.END

-- Test Case: SchedRoundRobin7
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_SchedRoundRobin
TEST.NEW
TEST.NAME:SchedRoundRobin7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSSchedRoundRobinEn != 1U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (4) if (p_tcb->TimeQuantaCtr > (OS_TICK)0) ==> FALSE
      (6) if (p_rdy_list->NbrEntries < (OS_OBJ_QTY)2) ==> FALSE
      (7) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSSchedRoundRobinEn due to assignment
TEST.END_NOTES:
TEST.STUB:sysCore.OS_RdyListMoveHeadToTail
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr[0].TimeQuanta:0
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr[0].TimeQuantaCtr:<<MIN>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].NbrEntries:<<MAX>>
TEST.EXPECTED:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr[0].TimeQuantaCtr:1
TEST.END

-- Test Case: SchedRoundRobin8
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_SchedRoundRobin
TEST.NEW
TEST.NAME:SchedRoundRobin8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSSchedRoundRobinEn != 1U) ==> FALSE
      (2) if (p_tcb == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb == &OSIdleTaskTCB) ==> FALSE
      (4) if (p_tcb->TimeQuantaCtr > (OS_TICK)0) ==> FALSE
      (6) if (p_rdy_list->NbrEntries < (OS_OBJ_QTY)2) ==> FALSE
      (7) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set OSSchedRoundRobinEn due to assignment
TEST.END_NOTES:
TEST.STUB:sysCore.OS_RdyListMoveHeadToTail
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedRoundRobinDfltTimeQuanta:1
TEST.VALUE:sysCore.<<GLOBAL>>.OSSchedRoundRobinEn:1
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list:<<malloc 1>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr:<<malloc 1>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr[0].TimeQuanta:<<MAX>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr[0].TimeQuantaCtr:<<MIN>>
TEST.VALUE:sysCore.OS_SchedRoundRobin.p_rdy_list[0].NbrEntries:<<MAX>>
TEST.EXPECTED:sysCore.OS_SchedRoundRobin.p_rdy_list[0].HeadPtr[0].TimeQuantaCtr:<<MAX>>
TEST.END

-- Subprogram: OS_TaskBlock

-- Test Case: TaskBlock1
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_TaskBlock
TEST.NEW
TEST.NAME:TaskBlock1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (timeout > (OS_TICK)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_TaskBlock.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_TaskBlock.timeout:<<MIN>>
TEST.EXPECTED:sysCore.OS_TaskBlock.p_tcb[0].TaskState:2
TEST.END

-- Test Case: TaskBlock2
TEST.UNIT:sysCore
TEST.SUBPROGRAM:OS_TaskBlock
TEST.NEW
TEST.NAME:TaskBlock2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (timeout > (OS_TICK)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCore.OS_TaskBlock.p_tcb:<<malloc 1>>
TEST.VALUE:sysCore.OS_TaskBlock.timeout:<<MAX>>
TEST.EXPECTED:sysCore.OS_TaskBlock.p_tcb[0].TaskState:3
TEST.END
