-- VectorCAST 6.4d (02/29/16)
-- Test Case Script
-- 
-- Environment    : THREAD1
-- Unit(s) Under Test: sysApi sysCfg sysCore sysMsg sysMutex sysPrio sysQueue sysSem sysTime
-- 
-- Script Features
TEST.SCRIPT_FEATURE:C_DIRECT_ARRAY_INDEXING
TEST.SCRIPT_FEATURE:CPP_CLASS_OBJECT_REVISION
TEST.SCRIPT_FEATURE:MULTIPLE_UUT_SUPPORT
TEST.SCRIPT_FEATURE:MIXED_CASE_NAMES
TEST.SCRIPT_FEATURE:STATIC_HEADER_FUNCS_IN_UUTS
--

-- Unit: sysMutex

-- Subprogram: OSMutexCreate

-- Test Case: OSMUtexCreate2
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexCreate
TEST.NEW
TEST.NAME:OSMUtexCreate2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexCreate.p_mutex:<<null>>
TEST.VALUE:sysMutex.OSMutexCreate.p_name:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexCreate.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.END

-- Test Case: OSMUtexCreate3
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexCreate
TEST.NEW
TEST.NAME:OSMUtexCreate3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) while 0 ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSMutexQty:0
TEST.VALUE:sysMutex.OSMutexCreate.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexCreate.p_name:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.<<GLOBAL>>.OSMutexQty:1
TEST.EXPECTED:sysMutex.OSMutexCreate.p_mutex[0].Type:1297437784
TEST.EXPECTED:sysMutex.OSMutexCreate.p_mutex[0].PendList.HeadPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexCreate.p_mutex[0].PendList.TailPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexCreate.p_mutex[0].PendList.NbrEntries:0
TEST.EXPECTED:sysMutex.OSMutexCreate.p_mutex[0].MutexGrpNextPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexCreate.p_mutex[0].OwnerTCBPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexCreate.p_mutex[0].OwnerNestingCtr:0
TEST.EXPECTED:sysMutex.OSMutexCreate.p_mutex[0].TS:0
TEST.EXPECTED:sysMutex.OSMutexCreate.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSMutexCreate1
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexCreate
TEST.NEW
TEST.NAME:OSMutexCreate1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysMutex.OSMutexCreate.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexCreate.p_name:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexCreate.p_err[0]:OS_ERR_CREATE_ISR
TEST.END

-- Subprogram: OSMutexDel

-- Test Case: OSMutexDel1
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexDel
TEST.NEW
TEST.NAME:OSMutexDel1
TEST.BASIS_PATH:1 of 18
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.opt:<<MIN>>
TEST.VALUE:sysMutex.OSMutexDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_err[0]:OS_ERR_DEL_ISR
TEST.EXPECTED:sysMutex.OSMutexDel.return:0
TEST.END

-- Test Case: OSMutexDel2
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexDel
TEST.NEW
TEST.NAME:OSMutexDel2
TEST.BASIS_PATH:2 of 18
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex:<<null>>
TEST.VALUE:sysMutex.OSMutexDel.opt:<<MIN>>
TEST.VALUE:sysMutex.OSMutexDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.EXPECTED:sysMutex.OSMutexDel.return:0
TEST.END

-- Test Case: OSMutexDel3
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexDel
TEST.NEW
TEST.NAME:OSMutexDel3
TEST.BASIS_PATH:3 of 18
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (5) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.opt:2
TEST.VALUE:sysMutex.OSMutexDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_err[0]:OS_ERR_OPT_INVALID
TEST.EXPECTED:sysMutex.OSMutexDel.return:0
TEST.END

-- Test Case: OSMutexDel4
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexDel
TEST.NEW
TEST.NAME:OSMutexDel4
TEST.BASIS_PATH:4 of 18
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (18) case (opt) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Cannot set local variable p_pend_list in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexDel.opt 'equal to' and 'not equal to' same value in branches 3/7
      Only one possible value for condition in branch 19
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSMutexQty:0
TEST.VALUE:sysMutex.OSMutexDel.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].PendList.NbrEntries:0
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].MutexGrpNextPtr:<<null>>
TEST.VALUE:sysMutex.OSMutexDel.opt:0
TEST.VALUE:sysMutex.OSMutexDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.<<GLOBAL>>.OSMutexQty:65535
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].Type:1313820229
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.HeadPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.TailPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.NbrEntries:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].MutexGrpNextPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].OwnerTCBPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].OwnerNestingCtr:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].TS:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysMutex.OSMutexDel.return:0
TEST.END

-- Test Case: OSMutexDel5
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexDel
TEST.NEW
TEST.NAME:OSMutexDel5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (18) case (opt) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Cannot set local variable p_pend_list in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexDel.opt 'equal to' and 'not equal to' same value in branches 3/7
      Only one possible value for condition in branch 19
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSMutexQty:0
TEST.VALUE:sysMutex.OSMutexDel.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].PendList.NbrEntries:1
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].MutexGrpNextPtr:<<null>>
TEST.VALUE:sysMutex.OSMutexDel.opt:0
TEST.VALUE:sysMutex.OSMutexDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.HeadPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.TailPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].MutexGrpNextPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].OwnerTCBPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_err[0]:OS_ERR_TASK_WAITING
TEST.EXPECTED:sysMutex.OSMutexDel.return:1
TEST.END

-- Test Case: OSMutexDel6
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexDel
TEST.NEW
TEST.NAME:OSMutexDel6
TEST.BASIS_PATH:5 of 18
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (18) case (opt) ==> default
      (19) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Cannot set local variable p_pend_list in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexDel.opt 'equal to' and 'not equal to' same value in branches 3/7
      Only one possible value for condition in branch 19
TEST.END_NOTES:
TEST.STUB:sysCore.OS_PendObjDel
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSMutexQty:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSSchedLockNestingCtr:1
TEST.VALUE:sysMutex.OSMutexDel.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].PendList.NbrEntries:0
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].OwnerTCBPtr:<<null>>
TEST.VALUE:sysMutex.OSMutexDel.opt:1
TEST.VALUE:sysMutex.OSMutexDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.<<GLOBAL>>.OSMutexQty:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].Type:1313820229
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.HeadPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.TailPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.NbrEntries:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].MutexGrpNextPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].OwnerTCBPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].OwnerNestingCtr:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].TS:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysMutex.OSMutexDel.return:0
TEST.END

-- Test Case: OSMutexDel7
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexDel
TEST.NEW
TEST.NAME:OSMutexDel7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (18) case (opt) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Cannot set local variable p_pend_list in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexDel.opt 'equal to' and 'not equal to' same value in branches 3/7
      Only one possible value for condition in branch 19
TEST.END_NOTES:
TEST.STUB:sysMutex.OS_MutexGrpRemove
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSMutexQty:0
TEST.VALUE:sysMutex.OSMutexDel.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].PendList.NbrEntries:0
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].MutexGrpNextPtr:<<null>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.opt:0
TEST.VALUE:sysMutex.OSMutexDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.<<GLOBAL>>.OSMutexQty:65535
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].Type:1313820229
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.HeadPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.TailPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.NbrEntries:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].MutexGrpNextPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].OwnerTCBPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].OwnerNestingCtr:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].TS:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysMutex.OSMutexDel.return:0
TEST.END

-- Test Case: OSMutexDel8
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexDel
TEST.NEW
TEST.NAME:OSMutexDel8
TEST.BASIS_PATH:6 of 18
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (8) if (nbr_tasks == (OS_OBJ_QTY)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Cannot set local variable p_pend_list in branch 7
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 7
      Only one possible value for condition in branch 11
TEST.END_NOTES:
TEST.STUB:sysCore.OS_PendObjDel
TEST.STUB:sysMutex.OS_MutexGrpRemove
TEST.STUB:sysMutex.OS_MutexGrpPrioFindHighest
TEST.VALUE:sysMutex.<<GLOBAL>>.OS_AppTaskDelHookPtr:<<null>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSMutexQty:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSSchedLockNestingCtr:1
TEST.VALUE:sysMutex.OSMutexDel.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].PendList.NbrEntries:1
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].OwnerTCBPtr[0].Prio:1
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].OwnerTCBPtr[0].BasePrio:2
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexDel.opt:1
TEST.VALUE:sysMutex.OSMutexDel.p_err:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.return:1
TEST.EXPECTED:sysMutex.<<GLOBAL>>.OSMutexQty:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].Type:1313820229
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.HeadPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.TailPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.NbrEntries:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].MutexGrpNextPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].OwnerTCBPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].OwnerNestingCtr:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].TS:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysMutex.OSMutexDel.return:1
TEST.END

-- Test Case: OSMutexDel9
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexDel
TEST.NEW
TEST.NAME:OSMutexDel9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (8) if (nbr_tasks == (OS_OBJ_QTY)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Cannot set local variable p_pend_list in branch 7
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 7
      Only one possible value for condition in branch 11
TEST.END_NOTES:
TEST.STUB:sysCore.OS_PendObjDel
TEST.STUB:sysMutex.OS_MutexGrpRemove
TEST.STUB:sysMutex.OS_MutexGrpPrioFindHighest
TEST.VALUE:sysMutex.<<GLOBAL>>.OS_AppTaskDelHookPtr:<<null>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSMutexQty:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSSchedLockNestingCtr:1
TEST.VALUE:sysMutex.OSMutexDel.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].PendList.NbrEntries:1
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].OwnerTCBPtr[0].Prio:1
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].OwnerTCBPtr[0].BasePrio:2
TEST.VALUE:sysMutex.OSMutexDel.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexDel.opt:1
TEST.VALUE:sysMutex.OSMutexDel.p_err:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.return:10
TEST.EXPECTED:sysMutex.<<GLOBAL>>.OSMutexQty:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].Type:1313820229
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.HeadPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.TailPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].PendList.NbrEntries:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].MutexGrpNextPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].OwnerTCBPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].OwnerNestingCtr:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_mutex[0].TS:0
TEST.EXPECTED:sysMutex.OSMutexDel.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysMutex.OSMutexDel.return:1
TEST.END

-- Subprogram: OSMutexPend

-- Test Case: OSMutexPend1
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend1
TEST.BASIS_PATH:1 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_PEND_ISR
TEST.END

-- Test Case: OSMutexPend10
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (11) if (OSTCBCurPtr == p_mutex->OwnerTCBPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 13
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:0
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].Prio:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].TS:100
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr[0].Prio:<<MAX>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:0
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_ts[0]:100
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSMutexPend11
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (11) if (OSTCBCurPtr == p_mutex->OwnerTCBPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 13
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].Prio:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].TS:100
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr[0].Prio:<<MAX>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:0
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_ts[0]:100
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_PEND_ABORT
TEST.END

-- Test Case: OSMutexPend12
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (11) if (OSTCBCurPtr == p_mutex->OwnerTCBPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 13
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].Prio:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].TS:100
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr[0].Prio:<<MAX>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:0
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<null>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_ts:<<null>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_PEND_ABORT
TEST.END

-- Test Case: OSMutexPend13
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (11) if (OSTCBCurPtr == p_mutex->OwnerTCBPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 13
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:3
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].Prio:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].TS:100
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr[0].Prio:<<MAX>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:0
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<null>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_ts:<<null>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_TIMEOUT
TEST.END

-- Test Case: OSMutexPend14
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (11) if (OSTCBCurPtr == p_mutex->OwnerTCBPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 13
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:3
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].Prio:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].TS:100
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr[0].Prio:<<MAX>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:0
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_ts[0]:0
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_TIMEOUT
TEST.END

-- Test Case: OSMutexPend15
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (11) if (OSTCBCurPtr == p_mutex->OwnerTCBPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 13
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:2
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].Prio:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].TS:100
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr[0].Prio:<<MAX>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:0
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_ts[0]:100
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_OBJ_DEL
TEST.END

-- Test Case: OSMutexPend16
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend16
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (11) if (OSTCBCurPtr == p_mutex->OwnerTCBPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 13
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:2
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].Prio:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].TS:100
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr[0].Prio:<<MAX>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:0
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<null>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_ts:<<null>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_OBJ_DEL
TEST.END

-- Test Case: OSMutexPend17
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (11) if (OSTCBCurPtr == p_mutex->OwnerTCBPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 13
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:<<MAX>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].Prio:<<MAX>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].TS:100
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr[0].Prio:<<MAX>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:0
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<null>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_ts:<<null>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_STATUS_INVALID
TEST.END

-- Test Case: OSMutexPend18
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend18
TEST.BASIS_PATH:29 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 29
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0x8000U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:0
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBHighRdyPtr:<<malloc 4>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:32768
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<null>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_PEND_WOULD_BLOCK
TEST.END

-- Test Case: OSMutexPend2
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend2
TEST.BASIS_PATH:2 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<null>>
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.END

-- Test Case: OSMutexPend3
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend3
TEST.BASIS_PATH:3 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (5) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:1
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_OPT_INVALID
TEST.END

-- Test Case: OSMutexPend4
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend4
TEST.BASIS_PATH:4 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysMutex.OS_MutexGrpAdd
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:0
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:0
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<null>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_NONE
TEST.EXPECTED_USER_CODE:sysMutex.OSMutexPend.p_mutex.p_mutex[0].OwnerTCBPtr
{{ <<sysMutex.OSMutexPend.p_mutex>>[0].OwnerTCBPtr == ( OSTCBCurPtr) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: OSMutexPend5
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend5
TEST.BASIS_PATH:5 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> TRUE
      (10) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysMutex.OS_MutexGrpAdd
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:0
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:0
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.EXPECTED:sysMutex.OSMutexPend.p_ts[0]:0
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_NONE
TEST.EXPECTED_USER_CODE:sysMutex.OSMutexPend.p_mutex.p_mutex[0].OwnerTCBPtr
{{ <<sysMutex.OSMutexPend.p_mutex>>[0].OwnerTCBPtr == ( OSTCBCurPtr ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: OSMutexPend6
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend6
TEST.BASIS_PATH:6 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> TRUE
      (9) if (p_ts != (CPU_TS *)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/9
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/10
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].TS:12
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:0
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:2
TEST.EXPECTED:sysMutex.OSMutexPend.p_ts[0]:12
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_MUTEX_OWNER
TEST.VALUE_USER_CODE:sysMutex.OSMutexPend.p_mutex.p_mutex[0].OwnerTCBPtr
<<sysMutex.OSMutexPend.p_mutex>>[0].OwnerTCBPtr = ( OSTCBCurPtr );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OSMutexPend7
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> TRUE
      (9) if (p_ts != (CPU_TS *)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/9
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/10
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].TS:12
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:0
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<null>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:2
TEST.EXPECTED:sysMutex.OSMutexPend.p_ts:<<null>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_MUTEX_OWNER
TEST.VALUE_USER_CODE:sysMutex.OSMutexPend.p_mutex.p_mutex[0].OwnerTCBPtr
<<sysMutex.OSMutexPend.p_mutex>>[0].OwnerTCBPtr = ( OSTCBCurPtr );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OSMutexPend8
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend8
TEST.BASIS_PATH:7 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (11) if (OSTCBCurPtr == p_mutex->OwnerTCBPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 13
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSSchedLockNestingCtr:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<null>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:0
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<null>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_SCHED_LOCKED
TEST.END

-- Test Case: OSMutexPend9
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPend
TEST.NEW
TEST.NAME:OSMutexPend9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) ==> FALSE
      (11) if (OSTCBCurPtr == p_mutex->OwnerTCBPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysMutex.OSMutexPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 13
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:0
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].Prio:1
TEST.VALUE:sysMutex.OSMutexPend.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerTCBPtr[0].Prio:<<MAX>>
TEST.VALUE:sysMutex.OSMutexPend.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPend.timeout:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPend.opt:0
TEST.VALUE:sysMutex.OSMutexPend.p_ts:<<null>>
TEST.VALUE:sysMutex.OSMutexPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPend.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OSMutexPendAbort

-- Test Case: OSMUtexPendAbort1
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPendAbort
TEST.NEW
TEST.NAME:OSMUtexPendAbort1
TEST.BASIS_PATH:1 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.opt:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPendAbort.p_err[0]:OS_ERR_PEND_ABORT_ISR
TEST.EXPECTED:sysMutex.OSMutexPendAbort.return:0
TEST.END

-- Test Case: OSMUtexPendAbort2
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPendAbort
TEST.NEW
TEST.NAME:OSMUtexPendAbort2
TEST.BASIS_PATH:2 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex:<<null>>
TEST.VALUE:sysMutex.OSMutexPendAbort.opt:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPendAbort.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.EXPECTED:sysMutex.OSMutexPendAbort.return:0
TEST.END

-- Test Case: OSMUtexPendAbort3
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPendAbort
TEST.NEW
TEST.NAME:OSMUtexPendAbort3
TEST.BASIS_PATH:3 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (7) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.opt:1
TEST.VALUE:sysMutex.OSMutexPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPendAbort.p_err[0]:OS_ERR_OPT_INVALID
TEST.EXPECTED:sysMutex.OSMutexPendAbort.return:0
TEST.END

-- Test Case: OSMUtexPendAbort4
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPendAbort
TEST.NEW
TEST.NAME:OSMUtexPendAbort4
TEST.BASIS_PATH:4 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (8) while 0 ==> FALSE
      (9) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0U) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 9
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.NbrEntries:0
TEST.VALUE:sysMutex.OSMutexPendAbort.opt:0
TEST.VALUE:sysMutex.OSMutexPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPendAbort.p_err[0]:OS_ERR_PEND_ABORT_NONE
TEST.EXPECTED:sysMutex.OSMutexPendAbort.return:0
TEST.END

-- Test Case: OSMUtexPendAbort9
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPendAbort
TEST.NEW
TEST.NAME:OSMUtexPendAbort9
TEST.BASIS_PATH:7 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (8) while 0 ==> FALSE
      (9) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0U) ==> FALSE
      (11) while (p_pend_list->NbrEntries > (OS_OBJ_QTY)0U) ==> FALSE
      (17) if ((opt & (OS_OPT)0x8000U) == (OS_OPT)0U) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 9
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 11
      Only one possible value for condition in branch 16
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_PendAbort
TEST.VALUE:uut_prototype_stubs.getTblms.return:0
TEST.VALUE:sysCore.OS_PendAbort.p_obj[0].PendList.NbrEntries:0
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.HeadPtr[0].TCBPtr[0].TaskState:0
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.NbrEntries:1
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr[0].Prio:1
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr[0].BasePrio:1
TEST.VALUE:sysMutex.OSMutexPendAbort.opt:256
TEST.VALUE:sysMutex.OSMutexPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPendAbort.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysMutex.OSMutexPendAbort.return:1
TEST.END

-- Test Case: OSMutexPendAbort5
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPendAbort
TEST.NEW
TEST.NAME:OSMutexPendAbort5
TEST.BASIS_PATH:5 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (8) while 0 ==> FALSE
      (9) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0U) ==> TRUE
      (10) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 9
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OS_PendAbort
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSSchedLockNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.NbrEntries:1
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr[0].Prio:0
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr[0].BasePrio:1
TEST.VALUE:sysMutex.OSMutexPendAbort.opt:0
TEST.VALUE:sysMutex.OSMutexPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPendAbort.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysMutex.OSMutexPendAbort.return:1
TEST.END

-- Test Case: OSMutexPendAbort6
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPendAbort
TEST.NEW
TEST.NAME:OSMutexPendAbort6
TEST.BASIS_PATH:6 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (8) while 0 ==> FALSE
      (9) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0U) ==> FALSE
      (11) while (p_pend_list->NbrEntries > (OS_OBJ_QTY)0U) ==> FALSE
      (17) if ((opt & (OS_OPT)0x8000U) == (OS_OPT)0U) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 9
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 11
      Only one possible value for condition in branch 16
      Conflict: Multiple equality operators with different values (sysMutex.OSMutexPendAbort.opt) in branches 3/17
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_PendAbort
TEST.STUB:sysMutex.OS_MutexGrpPrioFindHighest
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.HeadPtr[0].TCBPtr[0].Prio:1
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.NbrEntries:1
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr[0].Prio:1
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr[0].BasePrio:2
TEST.VALUE:sysMutex.OSMutexPendAbort.opt:0
TEST.VALUE:sysMutex.OSMutexPendAbort.p_err:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.return:1
TEST.EXPECTED:sysMutex.OSMutexPendAbort.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysMutex.OSMutexPendAbort.return:1
TEST.END

-- Test Case: OSMutexPendAbort7
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPendAbort
TEST.NEW
TEST.NAME:OSMutexPendAbort7
TEST.BASIS_PATH:11 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 11
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (8) while 0 ==> FALSE
      (9) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0U) ==> FALSE
      (11) while (p_pend_list->NbrEntries > (OS_OBJ_QTY)0U) ==> TRUE
      (12) if (p_tcb_owner->Prio != p_tcb_owner->BasePrio && p_tcb_owner->Prio == p_tcb->Prio) ==> FALSE
      (14) if (prio_new != p_tcb_owner->Prio) ==> TRUE
      (15) if (opt != (OS_OPT)0x100U) ==> TRUE
      (17) if ((opt & (OS_OPT)0x8000U) == (OS_OPT)0U) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 9
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 11
      Cannot set local variable p_pend_list in branch 12
      Cannot set p_pend_list due to assignment
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 12
      Only one possible value for condition in branch 16
      Conflict: Multiple equality operators with different values (sysMutex.OSMutexPendAbort.opt) in branches 3/17
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.HeadPtr[0].TCBPtr[0].Prio:1
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.NbrEntries:1
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr[0].Prio:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr[0].BasePrio:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPendAbort.opt:32768
TEST.VALUE:sysMutex.OSMutexPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexPendAbort.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysMutex.OSMutexPendAbort.return:1
TEST.END

-- Test Case: OSMutexPendAbort8
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPendAbort
TEST.NEW
TEST.NAME:OSMutexPendAbort8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (8) while 0 ==> FALSE
      (9) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0U) ==> FALSE
      (11) while (p_pend_list->NbrEntries > (OS_OBJ_QTY)0U) ==> FALSE
      (17) if ((opt & (OS_OPT)0x8000U) == (OS_OPT)0U) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 9
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 11
      Only one possible value for condition in branch 16
      Conflict: Multiple equality operators with different values (sysMutex.OSMutexPendAbort.opt) in branches 3/17
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_PendAbort
TEST.STUB:sysMutex.OS_MutexGrpPrioFindHighest
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].PendList.NbrEntries:1
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr[0].Prio:1
TEST.VALUE:sysMutex.OSMutexPendAbort.p_mutex[0].OwnerTCBPtr[0].BasePrio:1
TEST.VALUE:sysMutex.OSMutexPendAbort.opt:0
TEST.VALUE:sysMutex.OSMutexPendAbort.p_err:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.return:<<MAX>>
TEST.EXPECTED:sysMutex.OSMutexPendAbort.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysMutex.OSMutexPendAbort.return:1
TEST.END

-- Subprogram: OSMutexPost

-- Test Case: OSMutexPost
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPost
TEST.NEW
TEST.NAME:OSMutexPost
TEST.BASIS_PATH:1 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.opt:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPost.p_err[0]:OS_ERR_POST_ISR
TEST.END

-- Test Case: OSMutexPost10
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPost
TEST.NEW
TEST.NAME:OSMutexPost10
TEST.BASIS_PATH:17 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 17
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0x8000U
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr != p_mutex->OwnerTCBPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 8
TEST.END_NOTES:
TEST.STUB:sysCore.OS_Post
TEST.STUB:sysMutex.OS_MutexGrpAdd
TEST.STUB:sysMutex.OS_MutexGrpRemove
TEST.VALUE:uut_prototype_stubs.getTblms.return:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].Prio:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].BasePrio:1
TEST.VALUE:sysMutex.OSMutexPost.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].PendList.NbrEntries:1
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPost.opt:32768
TEST.VALUE:sysMutex.OSMutexPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPost.p_mutex[0].OwnerNestingCtr:1
TEST.EXPECTED:sysMutex.OSMutexPost.p_mutex[0].TS:1
TEST.EXPECTED:sysMutex.OSMutexPost.p_err[0]:OS_ERR_NONE
TEST.VALUE_USER_CODE:sysMutex.OSMutexPost.p_mutex.p_mutex[0].OwnerTCBPtr
<<sysMutex.OSMutexPost.p_mutex>>[0].OwnerTCBPtr = ( OSTCBCurPtr );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OSMutexPost2
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPost
TEST.NEW
TEST.NAME:OSMutexPost2
TEST.BASIS_PATH:2 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex:<<null>>
TEST.VALUE:sysMutex.OSMutexPost.opt:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPost.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.END

-- Test Case: OSMutexPost3
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPost
TEST.NEW
TEST.NAME:OSMutexPost3
TEST.BASIS_PATH:3 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (5) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.opt:1
TEST.VALUE:sysMutex.OSMutexPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPost.p_err[0]:OS_ERR_OPT_INVALID
TEST.END

-- Test Case: OSMutexPost4
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPost
TEST.NEW
TEST.NAME:OSMutexPost4
TEST.BASIS_PATH:4 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr != p_mutex->OwnerTCBPtr) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 8
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<null>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].OwnerTCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.opt:0
TEST.VALUE:sysMutex.OSMutexPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPost.p_err[0]:OS_ERR_MUTEX_NOT_OWNER
TEST.END

-- Test Case: OSMutexPost5
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPost
TEST.NEW
TEST.NAME:OSMutexPost5
TEST.BASIS_PATH:5 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr != p_mutex->OwnerTCBPtr) ==> TRUE
      (8) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 8
TEST.END_NOTES:
TEST.VALUE:uut_prototype_stubs.getTblms.return:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<null>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].OwnerNestingCtr:<<MAX>>
TEST.VALUE:sysMutex.OSMutexPost.opt:0
TEST.VALUE:sysMutex.OSMutexPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPost.p_mutex[0].OwnerNestingCtr:254
TEST.EXPECTED:sysMutex.OSMutexPost.p_mutex[0].TS:1
TEST.EXPECTED:sysMutex.OSMutexPost.p_err[0]:OS_ERR_MUTEX_NESTING
TEST.VALUE_USER_CODE:sysMutex.OSMutexPost.p_mutex.p_mutex[0].OwnerTCBPtr
<<sysMutex.OSMutexPost.p_mutex>>[0].OwnerTCBPtr = ( OSTCBCurPtr );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OSMutexPost6
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPost
TEST.NEW
TEST.NAME:OSMutexPost6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr != p_mutex->OwnerTCBPtr) ==> TRUE
      (8) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 8
TEST.END_NOTES:
TEST.STUB:sysMutex.OS_MutexGrpRemove
TEST.VALUE:uut_prototype_stubs.getTblms.return:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<null>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].PendList.NbrEntries:0
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPost.opt:0
TEST.VALUE:sysMutex.OSMutexPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPost.p_mutex[0].OwnerTCBPtr:<<null>>
TEST.EXPECTED:sysMutex.OSMutexPost.p_mutex[0].OwnerNestingCtr:0
TEST.EXPECTED:sysMutex.OSMutexPost.p_mutex[0].TS:1
TEST.EXPECTED:sysMutex.OSMutexPost.p_err[0]:OS_ERR_NONE
TEST.VALUE_USER_CODE:sysMutex.OSMutexPost.p_mutex.p_mutex[0].OwnerTCBPtr
<<sysMutex.OSMutexPost.p_mutex>>[0].OwnerTCBPtr = ( OSTCBCurPtr );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OSMutexPost7
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPost
TEST.NEW
TEST.NAME:OSMutexPost7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr != p_mutex->OwnerTCBPtr) ==> TRUE
      (8) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 8
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Post
TEST.STUB:sysMutex.OS_MutexGrpAdd
TEST.STUB:sysMutex.OS_MutexGrpRemove
TEST.VALUE:uut_prototype_stubs.getTblms.return:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].Prio:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].BasePrio:2
TEST.VALUE:sysMutex.OSMutexPost.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].PendList.NbrEntries:1
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPost.opt:0
TEST.VALUE:sysMutex.OSMutexPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPost.p_mutex[0].OwnerNestingCtr:1
TEST.EXPECTED:sysMutex.OSMutexPost.p_err[0]:OS_ERR_NONE
TEST.VALUE_USER_CODE:sysMutex.OSMutexPost.p_mutex.p_mutex[0].OwnerTCBPtr
<<sysMutex.OSMutexPost.p_mutex>>[0].OwnerTCBPtr = ( OSTCBCurPtr );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OSMutexPost8
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPost
TEST.NEW
TEST.NAME:OSMutexPost8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr != p_mutex->OwnerTCBPtr) ==> TRUE
      (8) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 8
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Post
TEST.STUB:sysMutex.OS_MutexGrpAdd
TEST.STUB:sysMutex.OS_MutexGrpRemove
TEST.STUB:sysMutex.OS_MutexGrpPrioFindHighest
TEST.VALUE:uut_prototype_stubs.getTblms.return:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].Prio:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].BasePrio:<<MAX>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].PendList.NbrEntries:1
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPost.opt:0
TEST.VALUE:sysMutex.OSMutexPost.p_err:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.return:0
TEST.EXPECTED:sysMutex.OSMutexPost.p_mutex[0].OwnerNestingCtr:1
TEST.EXPECTED:sysMutex.OSMutexPost.p_err[0]:OS_ERR_NONE
TEST.VALUE_USER_CODE:sysMutex.OSMutexPost.p_mutex.p_mutex[0].OwnerTCBPtr
<<sysMutex.OSMutexPost.p_mutex>>[0].OwnerTCBPtr = ( OSTCBCurPtr );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OSMutexPost9
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OSMutexPost
TEST.NEW
TEST.NAME:OSMutexPost9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_mutex == (OS_MUTEX *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (7) if (OSTCBCurPtr != p_mutex->OwnerTCBPtr) ==> TRUE
      (8) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 8
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Post
TEST.STUB:sysMutex.OS_MutexGrpAdd
TEST.STUB:sysMutex.OS_MutexGrpRemove
TEST.VALUE:uut_prototype_stubs.getTblms.return:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].Prio:1
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].BasePrio:1
TEST.VALUE:sysMutex.OSMutexPost.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].PendList.NbrEntries:1
TEST.VALUE:sysMutex.OSMutexPost.p_mutex[0].OwnerNestingCtr:1
TEST.VALUE:sysMutex.OSMutexPost.opt:0
TEST.VALUE:sysMutex.OSMutexPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.OSMutexPost.p_mutex[0].OwnerNestingCtr:1
TEST.EXPECTED:sysMutex.OSMutexPost.p_err[0]:OS_ERR_NONE
TEST.VALUE_USER_CODE:sysMutex.OSMutexPost.p_mutex.p_mutex[0].OwnerTCBPtr
<<sysMutex.OSMutexPost.p_mutex>>[0].OwnerTCBPtr = ( OSTCBCurPtr );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: OS_MutexClr

-- Test Case: OS_MutexClr1
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OS_MutexClr
TEST.NEW
TEST.NAME:OS_MutexClr1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sysMutex.OS_MutexClr.p_mutex:<<malloc 1>>
TEST.EXPECTED:sysMutex.OS_MutexClr.p_mutex[0].Type:1313820229
TEST.EXPECTED:sysMutex.OS_MutexClr.p_mutex[0].PendList.HeadPtr:<<null>>
TEST.EXPECTED:sysMutex.OS_MutexClr.p_mutex[0].PendList.TailPtr:<<null>>
TEST.EXPECTED:sysMutex.OS_MutexClr.p_mutex[0].PendList.NbrEntries:0
TEST.EXPECTED:sysMutex.OS_MutexClr.p_mutex[0].MutexGrpNextPtr:<<null>>
TEST.EXPECTED:sysMutex.OS_MutexClr.p_mutex[0].OwnerTCBPtr:<<null>>
TEST.EXPECTED:sysMutex.OS_MutexClr.p_mutex[0].OwnerNestingCtr:0
TEST.EXPECTED:sysMutex.OS_MutexClr.p_mutex[0].TS:0
TEST.END

-- Subprogram: OS_MutexGrpAdd

-- Test Case: OS_MutexGrpADD
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OS_MutexGrpAdd
TEST.NEW
TEST.NAME:OS_MutexGrpADD
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sysMutex.OS_MutexGrpAdd.p_tcb:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpAdd.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpAdd.p_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpAdd.p_mutex[0].MutexGrpNextPtr:<<malloc 1>>
TEST.END

-- Subprogram: OS_MutexGrpPostAll

-- Test Case: OS_MutexGrpPostAll1
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OS_MutexGrpPostAll
TEST.NEW
TEST.NAME:OS_MutexGrpPostAll1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (p_mutex != (OS_MUTEX *)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr:<<null>>
TEST.END

-- Test Case: OS_MutexGrpPostAll2
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OS_MutexGrpPostAll
TEST.NEW
TEST.NAME:OS_MutexGrpPostAll2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (p_mutex != (OS_MUTEX *)0) ==> TRUE
      (2) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 2
      Cannot set local variable p_pend_list in branch 3
      Cannot set p_pend_list due to assignment
TEST.END_NOTES:
TEST.STUB:sysMutex.OS_MutexGrpRemove
TEST.VALUE:uut_prototype_stubs.getTblms.return:9
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr[0].PendList.NbrEntries:0
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr[0].MutexGrpNextPtr:<<malloc 1>>
TEST.EXPECTED:sysMutex.OS_MutexGrpPostAll.p_tcb[0].TickPrevPtr:<<null>>
TEST.EXPECTED:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr[0].OwnerTCBPtr:<<null>>
TEST.EXPECTED:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr[0].OwnerNestingCtr:0
TEST.EXPECTED:sysMutex.OS_MutexGrpPostAll.p_tcb[0].TS:0
TEST.END

-- Test Case: OS_MutexGrpPostAll3
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OS_MutexGrpPostAll
TEST.NEW
TEST.NAME:OS_MutexGrpPostAll3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (p_mutex != (OS_MUTEX *)0) ==> TRUE
      (2) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 2
TEST.END_NOTES:
TEST.STUB:sysCore.OS_Post
TEST.STUB:sysMutex.OS_MutexGrpAdd
TEST.VALUE:uut_prototype_stubs.getTblms.return:1
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr[0].PendList.NbrEntries:1
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr[0].MutexGrpNextPtr:<<malloc 1>>
TEST.EXPECTED:sysMutex.OS_MutexGrpPostAll.p_tcb[0].TS:0
TEST.END

-- Test Case: OS_MutexGrpPostAll4
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OS_MutexGrpPostAll
TEST.NEW
TEST.NAME:OS_MutexGrpPostAll4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (p_mutex != (OS_MUTEX *)0) ==> TRUE
      (2) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 2
TEST.END_NOTES:
TEST.STUB:sysCore.OS_Post
TEST.STUB:sysMutex.OS_MutexGrpAdd
TEST.VALUE:uut_prototype_stubs.getTblms.return:1
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr[0].PendList.NbrEntries:1
TEST.VALUE:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr[0].MutexGrpNextPtr:<<null>>
TEST.EXPECTED:sysMutex.OS_MutexGrpPostAll.p_tcb[0].MutexGrpHeadPtr:<<null>>
TEST.EXPECTED:sysMutex.OS_MutexGrpPostAll.p_tcb[0].TS:0
TEST.END

-- Subprogram: OS_MutexGrpPrioFindHighest

-- Test Case: OS_MutexGrpPrioFindHighest1
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OS_MutexGrpPrioFindHighest
TEST.NEW
TEST.NAME:OS_MutexGrpPrioFindHighest1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (*pp_mutex != (OS_MUTEX *)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set pp_mutex due to assignment
      Cannot set local variable pp_mutex in branch 1
TEST.END_NOTES:
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb[0].MutexGrpHeadPtr:<<null>>
TEST.EXPECTED:sysMutex.OS_MutexGrpPrioFindHighest.return:31
TEST.END

-- Test Case: OS_MutexGrpPrioFindHighest2
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OS_MutexGrpPrioFindHighest
TEST.NEW
TEST.NAME:OS_MutexGrpPrioFindHighest2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (*pp_mutex != (OS_MUTEX *)0) ==> TRUE
      (2) if (p_head != (OS_PEND_DATA *)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set pp_mutex due to assignment
      Cannot set local variable pp_mutex in branch 1
      Cannot set pp_mutex due to assignment
      Cannot set pp_mutex due to assignment
      Cannot set local variable pp_mutex in branch 2
TEST.END_NOTES:
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb[0].MutexGrpHeadPtr[0].PendList.HeadPtr:<<null>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb[0].MutexGrpHeadPtr[0].MutexGrpNextPtr:<<null>>
TEST.EXPECTED:sysMutex.OS_MutexGrpPrioFindHighest.return:31
TEST.END

-- Test Case: OS_MutexGrpPrioFindHighest3
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OS_MutexGrpPrioFindHighest
TEST.NEW
TEST.NAME:OS_MutexGrpPrioFindHighest3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (*pp_mutex != (OS_MUTEX *)0) ==> TRUE
      (2) if (p_head != (OS_PEND_DATA *)0) ==> TRUE
      (3) if (prio < highest_prio) ==> FALSE
   Test Case Generation Notes:
      Cannot set pp_mutex due to assignment
      Cannot set local variable pp_mutex in branch 1
      Cannot set pp_mutex due to assignment
      Cannot set pp_mutex due to assignment
      Cannot set local variable pp_mutex in branch 2
      Cannot set pp_mutex due to assignment
      Cannot set pp_mutex due to assignment
      Cannot set pp_mutex due to assignment
      Cannot set highest_prio due to assignment
      Cannot set pp_mutex due to assignment
      Cannot set pp_mutex due to assignment
TEST.END_NOTES:
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb[0].MutexGrpHeadPtr[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb[0].MutexGrpHeadPtr[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb[0].MutexGrpHeadPtr[0].PendList.HeadPtr[0].TCBPtr[0].Prio:0
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb[0].MutexGrpHeadPtr[0].MutexGrpNextPtr:<<null>>
TEST.EXPECTED:sysMutex.OS_MutexGrpPrioFindHighest.return:0
TEST.END

-- Test Case: OS_MutexGrpPrioFindHighest4
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OS_MutexGrpPrioFindHighest
TEST.NEW
TEST.NAME:OS_MutexGrpPrioFindHighest4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (*pp_mutex != (OS_MUTEX *)0) ==> TRUE
      (2) if (p_head != (OS_PEND_DATA *)0) ==> TRUE
      (3) if (prio < highest_prio) ==> FALSE
   Test Case Generation Notes:
      Cannot set pp_mutex due to assignment
      Cannot set local variable pp_mutex in branch 1
      Cannot set pp_mutex due to assignment
      Cannot set pp_mutex due to assignment
      Cannot set local variable pp_mutex in branch 2
      Cannot set pp_mutex due to assignment
      Cannot set pp_mutex due to assignment
      Cannot set pp_mutex due to assignment
      Cannot set highest_prio due to assignment
      Cannot set pp_mutex due to assignment
      Cannot set pp_mutex due to assignment
TEST.END_NOTES:
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb[0].MutexGrpHeadPtr[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb[0].MutexGrpHeadPtr[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb[0].MutexGrpHeadPtr[0].PendList.HeadPtr[0].TCBPtr[0].Prio:255
TEST.VALUE:sysMutex.OS_MutexGrpPrioFindHighest.p_tcb[0].MutexGrpHeadPtr[0].MutexGrpNextPtr:<<null>>
TEST.EXPECTED:sysMutex.OS_MutexGrpPrioFindHighest.return:31
TEST.END

-- Subprogram: OS_MutexGrpRemove

-- Test Case: BASIS-PATH-001-PARTIAL
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OS_MutexGrpRemove
TEST.NEW
TEST.NAME:BASIS-PATH-001-PARTIAL
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (*pp_mutex != p_mutex) ==> FALSE
   Test Case Generation Notes:
      Cannot set pp_mutex due to assignment
      Cannot set local variable pp_mutex in branch 1
      Cannot set local variable pp_mutex in branch 2
TEST.END_NOTES:
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.OSTCBCurPtr[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE:sysMutex.<<GLOBAL>>.h_mutex:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpRemove.p_tcb:<<malloc 1>>
TEST.VALUE:sysMutex.OS_MutexGrpRemove.p_tcb[0].MutexGrpHeadPtr:<<malloc 1>>
TEST.VALUE_USER_CODE:sysMutex.OS_MutexGrpRemove.p_tcb.p_tcb[0].MutexGrpHeadPtr.MutexGrpHeadPtr[0].MutexGrpNextPtr
<<sysMutex.OS_MutexGrpRemove.p_tcb>>[0].MutexGrpHeadPtr[0].MutexGrpNextPtr = ( h_mutex );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:sysMutex.OS_MutexGrpRemove.p_mutex
<<sysMutex.OS_MutexGrpRemove.p_mutex>> = ( h_mutex );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: OS_MutexInit

-- Test Case: OS_MutexInit
TEST.UNIT:sysMutex
TEST.SUBPROGRAM:OS_MutexInit
TEST.NEW
TEST.NAME:OS_MutexInit
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sysMutex.OS_MutexInit.p_err:<<malloc 1>>
TEST.EXPECTED:sysMutex.<<GLOBAL>>.OSMutexQty:0
TEST.EXPECTED:sysMutex.OS_MutexInit.p_err[0]:OS_ERR_NONE
TEST.END
