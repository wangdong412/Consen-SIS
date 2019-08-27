-- VectorCAST 6.4d (02/29/16)
-- Test Case Script
-- 
-- Environment    : THREAD
-- Unit(s) Under Test: sysApi sysCfg sysCore sysMsg sysQueue sysSem sysTime
-- 
-- Script Features
TEST.SCRIPT_FEATURE:C_DIRECT_ARRAY_INDEXING
TEST.SCRIPT_FEATURE:CPP_CLASS_OBJECT_REVISION
TEST.SCRIPT_FEATURE:MULTIPLE_UUT_SUPPORT
TEST.SCRIPT_FEATURE:MIXED_CASE_NAMES
TEST.SCRIPT_FEATURE:STATIC_HEADER_FUNCS_IN_UUTS
--

-- Subprogram: OSSemCreate

-- Test Case: OSSemCreate1
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemCreate
TEST.NEW
TEST.NAME:OSSemCreate1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysSem.OSSemCreate.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemCreate.p_name:<<malloc 1>>
TEST.VALUE:sysSem.OSSemCreate.cnt:<<MIN>>
TEST.VALUE:sysSem.OSSemCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemCreate.p_err[0]:OS_ERR_CREATE_ISR
TEST.END

-- Test Case: OSSemCreate2
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemCreate
TEST.NEW
TEST.NAME:OSSemCreate2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemCreate.p_sem:<<null>>
TEST.VALUE:sysSem.OSSemCreate.p_name:<<malloc 1>>
TEST.VALUE:sysSem.OSSemCreate.cnt:<<MIN>>
TEST.VALUE:sysSem.OSSemCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemCreate.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.END

-- Test Case: OSSemCreate3
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemCreate
TEST.NEW
TEST.NAME:OSSemCreate3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) while 0 ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSSemQty:0
TEST.VALUE:sysSem.OSSemCreate.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemCreate.p_name:<<malloc 1>>
TEST.VALUE:sysSem.OSSemCreate.cnt:<<MIN>>
TEST.VALUE:sysSem.OSSemCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.<<GLOBAL>>.OSSemQty:1
TEST.EXPECTED:sysSem.OSSemCreate.p_sem[0].Type:1397050689
TEST.EXPECTED:sysSem.OSSemCreate.p_sem[0].PendList.HeadPtr:<<null>>
TEST.EXPECTED:sysSem.OSSemCreate.p_sem[0].PendList.TailPtr:<<null>>
TEST.EXPECTED:sysSem.OSSemCreate.p_sem[0].PendList.NbrEntries:0
TEST.EXPECTED:sysSem.OSSemCreate.p_sem[0].Ctr:0
TEST.EXPECTED:sysSem.OSSemCreate.p_sem[0].TS:0
TEST.EXPECTED:sysSem.OSSemCreate.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OSSemDel

-- Test Case: OSSemDel1
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemDel
TEST.NEW
TEST.NAME:OSSemDel1
TEST.BASIS_PATH:1 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysSem.OSSemDel.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemDel.opt:<<MIN>>
TEST.VALUE:sysSem.OSSemDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemDel.p_err[0]:OS_ERR_DEL_ISR
TEST.EXPECTED:sysSem.OSSemDel.return:0
TEST.END

-- Test Case: OSSemDel2
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemDel
TEST.NEW
TEST.NAME:OSSemDel2
TEST.BASIS_PATH:2 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemDel.p_sem:<<null>>
TEST.VALUE:sysSem.OSSemDel.opt:<<MIN>>
TEST.VALUE:sysSem.OSSemDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemDel.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.EXPECTED:sysSem.OSSemDel.return:0
TEST.END

-- Test Case: OSSemDel3
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemDel
TEST.NEW
TEST.NAME:OSSemDel3
TEST.BASIS_PATH:3 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (5) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemDel.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemDel.opt:2
TEST.VALUE:sysSem.OSSemDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemDel.p_err[0]:OS_ERR_OPT_INVALID
TEST.EXPECTED:sysSem.OSSemDel.return:0
TEST.END

-- Test Case: OSSemDel4
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemDel
TEST.NEW
TEST.NAME:OSSemDel4
TEST.BASIS_PATH:4 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (14) case (opt) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Cannot set local variable p_pend_list in branch 7
      Conflict: Trying to set variable sysSem.OSSemDel.opt 'equal to' and 'not equal to' same value in branches 3/7
      Only one possible value for condition in branch 15
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemDel.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemDel.p_sem[0].PendList.NbrEntries:1
TEST.VALUE:sysSem.OSSemDel.opt:0
TEST.VALUE:sysSem.OSSemDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemDel.p_err[0]:OS_ERR_TASK_WAITING
TEST.EXPECTED:sysSem.OSSemDel.return:1
TEST.END

-- Test Case: OSSemDel5
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemDel
TEST.NEW
TEST.NAME:OSSemDel5
TEST.BASIS_PATH:5 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (14) case (opt) ==> default
      (15) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Cannot set local variable p_pend_list in branch 7
      Conflict: Trying to set variable sysSem.OSSemDel.opt 'equal to' and 'not equal to' same value in branches 3/7
      Only one possible value for condition in branch 15
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSSemQty:0
TEST.VALUE:sysSem.OSSemDel.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemDel.p_sem[0].PendList.NbrEntries:0
TEST.VALUE:sysSem.OSSemDel.opt:0
TEST.VALUE:sysSem.OSSemDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.<<GLOBAL>>.OSSemQty:65535
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].Type:1313820229
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].PendList.HeadPtr:<<null>>
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].PendList.TailPtr:<<null>>
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].PendList.NbrEntries:0
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].Ctr:0
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].TS:0
TEST.EXPECTED:sysSem.OSSemDel.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysSem.OSSemDel.return:0
TEST.END

-- Test Case: OSSemDel6
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemDel
TEST.NEW
TEST.NAME:OSSemDel6
TEST.BASIS_PATH:13 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 13
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> TRUE
      (14) case (opt) ==> default
      (15) while 0 ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Cannot set local variable p_pend_list in branch 7
      Conflict: Trying to set variable sysSem.OSSemDel.opt 'equal to' and 'not equal to' same value in branches 3/7
      Only one possible value for condition in branch 15
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSSemQty:1
TEST.VALUE:sysSem.OSSemDel.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemDel.p_sem[0].PendList.NbrEntries:0
TEST.VALUE:sysSem.OSSemDel.opt:1
TEST.VALUE:sysSem.OSSemDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.<<GLOBAL>>.OSSemQty:0
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].Type:1313820229
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].PendList.HeadPtr:<<null>>
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].PendList.TailPtr:<<null>>
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].PendList.NbrEntries:0
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].Ctr:0
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].TS:0
TEST.EXPECTED:sysSem.OSSemDel.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysSem.OSSemDel.return:0
TEST.END

-- Test Case: OSSemDel7
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemDel
TEST.NEW
TEST.NAME:OSSemDel7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 13
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> TRUE
      (14) case (opt) ==> default
      (15) while 0 ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Cannot set local variable p_pend_list in branch 7
      Conflict: Trying to set variable sysSem.OSSemDel.opt 'equal to' and 'not equal to' same value in branches 3/7
      Only one possible value for condition in branch 15
TEST.END_NOTES:
TEST.STUB:sysCore.OS_PendObjDel
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSSemQty:1
TEST.VALUE:sysSem.OSSemDel.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemDel.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemDel.p_sem[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemDel.p_sem[0].PendList.NbrEntries:1
TEST.VALUE:sysSem.OSSemDel.opt:1
TEST.VALUE:sysSem.OSSemDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.<<GLOBAL>>.OSSemQty:0
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].Type:1313820229
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].PendList.HeadPtr:<<null>>
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].PendList.TailPtr:<<null>>
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].PendList.NbrEntries:0
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].Ctr:0
TEST.EXPECTED:sysSem.OSSemDel.p_sem[0].TS:0
TEST.EXPECTED:sysSem.OSSemDel.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysSem.OSSemDel.return:1
TEST.END

-- Subprogram: OSSemPend

-- Test Case: OSSemPend1
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend1
TEST.BASIS_PATH:1 of 26
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_PEND_ISR
TEST.EXPECTED:sysSem.OSSemPend.return:0
TEST.END

-- Test Case: OSSemPend10
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysSem.OSSemPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:1
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].Ctr:0
TEST.VALUE:sysSem.OSSemPend.p_sem[0].TS:1
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:0
TEST.VALUE:sysSem.OSSemPend.p_ts:<<null>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_ts:<<null>>
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_PEND_ABORT
TEST.EXPECTED:sysSem.OSSemPend.return:0
TEST.END

-- Test Case: OSSemPend11
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysSem.OSSemPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:1
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].Ctr:0
TEST.VALUE:sysSem.OSSemPend.p_sem[0].TS:1
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:0
TEST.VALUE:sysSem.OSSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_ts[0]:1
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_PEND_ABORT
TEST.EXPECTED:sysSem.OSSemPend.return:0
TEST.END

-- Test Case: OSSemPend12
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysSem.OSSemPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:3
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].Ctr:0
TEST.VALUE:sysSem.OSSemPend.p_sem[0].TS:1
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:0
TEST.VALUE:sysSem.OSSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_ts[0]:0
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_TIMEOUT
TEST.EXPECTED:sysSem.OSSemPend.return:0
TEST.END

-- Test Case: OSSemPend13
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysSem.OSSemPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:3
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].Ctr:0
TEST.VALUE:sysSem.OSSemPend.p_sem[0].TS:1
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:0
TEST.VALUE:sysSem.OSSemPend.p_ts:<<null>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_ts:<<null>>
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_TIMEOUT
TEST.EXPECTED:sysSem.OSSemPend.return:0
TEST.END

-- Test Case: OSSemPend14
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysSem.OSSemPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:2
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].Ctr:0
TEST.VALUE:sysSem.OSSemPend.p_sem[0].TS:1
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:0
TEST.VALUE:sysSem.OSSemPend.p_ts:<<null>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_ts:<<null>>
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_OBJ_DEL
TEST.EXPECTED:sysSem.OSSemPend.return:0
TEST.END

-- Test Case: OSSemPend15
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysSem.OSSemPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:2
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].Ctr:0
TEST.VALUE:sysSem.OSSemPend.p_sem[0].TS:1
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:0
TEST.VALUE:sysSem.OSSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_ts[0]:1
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_OBJ_DEL
TEST.EXPECTED:sysSem.OSSemPend.return:0
TEST.END

-- Test Case: OSSemPend16
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend16
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysSem.OSSemPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:<<MAX>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].Ctr:0
TEST.VALUE:sysSem.OSSemPend.p_sem[0].TS:1
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:0
TEST.VALUE:sysSem.OSSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_STATUS_INVALID
TEST.EXPECTED:sysSem.OSSemPend.return:0
TEST.END

-- Test Case: OSSemPend2
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend2
TEST.BASIS_PATH:2 of 26
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.p_sem:<<null>>
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.EXPECTED:sysSem.OSSemPend.return:0
TEST.END

-- Test Case: OSSemPend3
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend3
TEST.BASIS_PATH:3 of 26
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (5) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:1
TEST.VALUE:sysSem.OSSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_OPT_INVALID
TEST.EXPECTED:sysSem.OSSemPend.return:0
TEST.END

-- Test Case: OSSemPend4
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend4
TEST.BASIS_PATH:4 of 26
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysSem.OSSemPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].Ctr:<<MAX>>
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:0
TEST.VALUE:sysSem.OSSemPend.p_ts:<<null>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_sem[0].Ctr:4294967294
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysSem.OSSemPend.return:4294967294
TEST.END

-- Test Case: OSSemPend5
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysSem.OSSemPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].Ctr:<<MAX>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].TS:1
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:0
TEST.VALUE:sysSem.OSSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_sem[0].Ctr:4294967294
TEST.EXPECTED:sysSem.OSSemPend.p_ts[0]:1
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysSem.OSSemPend.return:4294967294
TEST.END

-- Test Case: OSSemPend6
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysSem.OSSemPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSSchedLockNestingCtr:<<MAX>>
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].Ctr:0
TEST.VALUE:sysSem.OSSemPend.p_sem[0].TS:1
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:0
TEST.VALUE:sysSem.OSSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_SCHED_LOCKED
TEST.EXPECTED:sysSem.OSSemPend.return:0
TEST.END

-- Test Case: OSSemPend7
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend7
TEST.BASIS_PATH:26 of 26
TEST.NOTES:
This is an automatically generated test case.
   Test Path 26
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0x8000U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysSem.OSSemPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].Ctr:0
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:32768
TEST.VALUE:sysSem.OSSemPend.p_ts:<<null>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_PEND_WOULD_BLOCK
TEST.EXPECTED:sysSem.OSSemPend.return:0
TEST.END

-- Test Case: OSSemPend8
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysSem.OSSemPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:0
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].Ctr:0
TEST.VALUE:sysSem.OSSemPend.p_sem[0].TS:1
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:0
TEST.VALUE:sysSem.OSSemPend.p_ts:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_ts[0]:1
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysSem.OSSemPend.return:0
TEST.END

-- Test Case: OSSemPend9
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPend
TEST.NEW
TEST.NAME:OSSemPend9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) if (p_ts != (CPU_TS *)0) ==> FALSE
      (7) while 0 ==> FALSE
      (8) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 7
      Conflict: Trying to set variable sysSem.OSSemPend.p_ts 'equal to' and 'not equal to' same value in branches 6/7
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OSSched
TEST.STUB:sysCore.OS_Pend
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSSchedLockNestingCtr:0
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PrevPtr:<<malloc 1>>
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:0
TEST.VALUE:sysSem.<<GLOBAL>>.OSTCBCurPtr[0].TS:1
TEST.VALUE:sysSem.OSSemPend.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPend.p_sem[0].Ctr:0
TEST.VALUE:sysSem.OSSemPend.p_sem[0].TS:1
TEST.VALUE:sysSem.OSSemPend.timeout:<<MIN>>
TEST.VALUE:sysSem.OSSemPend.opt:0
TEST.VALUE:sysSem.OSSemPend.p_ts:<<null>>
TEST.VALUE:sysSem.OSSemPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPend.p_ts:<<null>>
TEST.EXPECTED:sysSem.OSSemPend.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysSem.OSSemPend.return:0
TEST.END

-- Subprogram: OSSemPendAbort

-- Test Case: OSSemPendAbort1
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPendAbort
TEST.NEW
TEST.NAME:OSSemPendAbort1
TEST.BASIS_PATH:1 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPendAbort.opt:<<MIN>>
TEST.VALUE:sysSem.OSSemPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPendAbort.p_err[0]:OS_ERR_PEND_ABORT_ISR
TEST.EXPECTED:sysSem.OSSemPendAbort.return:0
TEST.END

-- Test Case: OSSemPendAbort2
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPendAbort
TEST.NEW
TEST.NAME:OSSemPendAbort2
TEST.BASIS_PATH:2 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem:<<null>>
TEST.VALUE:sysSem.OSSemPendAbort.opt:<<MIN>>
TEST.VALUE:sysSem.OSSemPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPendAbort.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.EXPECTED:sysSem.OSSemPendAbort.return:0
TEST.END

-- Test Case: OSSemPendAbort3
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPendAbort
TEST.NEW
TEST.NAME:OSSemPendAbort3
TEST.BASIS_PATH:3 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (7) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPendAbort.opt:1
TEST.VALUE:sysSem.OSSemPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPendAbort.p_err[0]:OS_ERR_OPT_INVALID
TEST.EXPECTED:sysSem.OSSemPendAbort.return:0
TEST.END

-- Test Case: OSSemPendAbort4
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPendAbort
TEST.NEW
TEST.NAME:OSSemPendAbort4
TEST.BASIS_PATH:4 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (8) while 0 ==> FALSE
      (9) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0U) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 9
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem[0].PendList.NbrEntries:0
TEST.VALUE:sysSem.OSSemPendAbort.opt:0
TEST.VALUE:sysSem.OSSemPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPendAbort.p_err[0]:OS_ERR_PEND_ABORT_NONE
TEST.EXPECTED:sysSem.OSSemPendAbort.return:0
TEST.END

-- Test Case: OSSemPendAbort5
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPendAbort
TEST.NEW
TEST.NAME:OSSemPendAbort5
TEST.BASIS_PATH:5 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
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
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem[0].PendList.NbrEntries:<<MAX>>
TEST.VALUE:sysSem.OSSemPendAbort.opt:0
TEST.VALUE:sysSem.OSSemPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPendAbort.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysSem.OSSemPendAbort.return:1
TEST.END

-- Test Case: OSSemPendAbort6
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPendAbort
TEST.NEW
TEST.NAME:OSSemPendAbort6
TEST.BASIS_PATH:12 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 12
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0x100U
      (8) while 0 ==> FALSE
      (9) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0U) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 9
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysCore.OS_PendAbort
TEST.VALUE:sysCore.OS_PendAbort.p_obj[0].PendList.NbrEntries:0
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem[0].PendList.NbrEntries:1
TEST.VALUE:sysSem.OSSemPendAbort.opt:256
TEST.VALUE:sysSem.OSSemPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPendAbort.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysSem.OSSemPendAbort.return:1
TEST.END

-- Test Case: OSSemPendAbort7
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPendAbort
TEST.NEW
TEST.NAME:OSSemPendAbort7
TEST.BASIS_PATH:13 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 13
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (5) case (opt) ==> (OS_OPT)0U | (OS_OPT)0x8000U
      (8) while 0 ==> FALSE
      (9) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0U) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 9
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPendAbort.p_sem[0].PendList.NbrEntries:1
TEST.VALUE:sysSem.OSSemPendAbort.opt:32768
TEST.VALUE:sysSem.OSSemPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPendAbort.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysSem.OSSemPendAbort.return:1
TEST.END

-- Subprogram: OSSemPost

-- Test Case: OSSemPost1
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPost
TEST.NEW
TEST.NAME:OSSemPost1
TEST.BASIS_PATH:1 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_sem == (OS_SEM *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.OSSemPost.p_sem:<<null>>
TEST.VALUE:sysSem.OSSemPost.opt:<<MIN>>
TEST.VALUE:sysSem.OSSemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPost.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.EXPECTED:sysSem.OSSemPost.return:0
TEST.END

-- Test Case: OSSemPost2
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPost
TEST.NEW
TEST.NAME:OSSemPost2
TEST.BASIS_PATH:2 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (p_sem == (OS_SEM *)0) ==> FALSE
      (6) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.OSSemPost.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPost.opt:1
TEST.VALUE:sysSem.OSSemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemPost.p_err[0]:OS_ERR_OPT_INVALID
TEST.EXPECTED:sysSem.OSSemPost.return:0
TEST.END

-- Test Case: OSSemPost3
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPost
TEST.NEW
TEST.NAME:OSSemPost3
TEST.BASIS_PATH:3 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_sem == (OS_SEM *)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysSem.OS_SemPost
TEST.VALUE:sysSem.OSSemPost.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPost.opt:0
TEST.VALUE:sysSem.OSSemPost.p_err:<<malloc 1>>
TEST.VALUE:sysSem.OS_SemPost.return:0
TEST.EXPECTED:sysSem.OSSemPost.return:0
TEST.END

-- Test Case: OSSemPost4
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPost
TEST.NEW
TEST.NAME:OSSemPost4
TEST.BASIS_PATH:4 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0x200U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysSem.OS_SemPost
TEST.VALUE:sysSem.OSSemPost.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPost.opt:512
TEST.VALUE:sysSem.OSSemPost.p_err:<<malloc 1>>
TEST.VALUE:sysSem.OS_SemPost.return:<<MAX>>
TEST.EXPECTED:sysSem.OSSemPost.return:<<MAX>>
TEST.END

-- Test Case: OSSemPost5
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPost
TEST.NEW
TEST.NAME:OSSemPost5
TEST.BASIS_PATH:5 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (p_sem == (OS_SEM *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U | (OS_OPT)0x8000U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysSem.OS_SemPost
TEST.VALUE:sysSem.OSSemPost.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPost.opt:32768
TEST.VALUE:sysSem.OSSemPost.p_err:<<malloc 1>>
TEST.VALUE:sysSem.OS_SemPost.return:1
TEST.EXPECTED:sysSem.OSSemPost.return:1
TEST.END

-- Test Case: OSSemPost6
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemPost
TEST.NEW
TEST.NAME:OSSemPost6
TEST.BASIS_PATH:6 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (p_sem == (OS_SEM *)0) ==> FALSE
      (5) case (opt) ==> (OS_OPT)0x200U | (OS_OPT)0x8000U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysSem.OS_SemPost
TEST.VALUE:sysSem.OSSemPost.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemPost.opt:33280
TEST.VALUE:sysSem.OSSemPost.p_err:<<malloc 1>>
TEST.VALUE:sysSem.OS_SemPost.return:4
TEST.EXPECTED:sysSem.OSSemPost.return:4
TEST.END

-- Subprogram: OSSemSet

-- Test Case: OSSemSet1
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemSet
TEST.NEW
TEST.NAME:OSSemSet1
TEST.BASIS_PATH:1 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysSem.OSSemSet.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemSet.cnt:<<MIN>>
TEST.VALUE:sysSem.OSSemSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemSet.p_err[0]:OS_ERR_SET_ISR
TEST.END

-- Test Case: OSSemSet2
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemSet
TEST.NEW
TEST.NAME:OSSemSet2
TEST.BASIS_PATH:2 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemSet.p_sem:<<null>>
TEST.VALUE:sysSem.OSSemSet.cnt:<<MIN>>
TEST.VALUE:sysSem.OSSemSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemSet.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.END

-- Test Case: OSSemSet3
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemSet
TEST.NEW
TEST.NAME:OSSemSet3
TEST.BASIS_PATH:3 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) while 0 ==> FALSE
      (4) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> FALSE
      (5) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 5
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemSet.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemSet.p_sem[0].Ctr:<<MAX>>
TEST.VALUE:sysSem.OSSemSet.cnt:<<MIN>>
TEST.VALUE:sysSem.OSSemSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemSet.p_sem[0].Ctr:0
TEST.EXPECTED:sysSem.OSSemSet.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSSemSet4
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemSet
TEST.NEW
TEST.NAME:OSSemSet4
TEST.BASIS_PATH:4 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) while 0 ==> FALSE
      (4) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> FALSE
      (5) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
      (6) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 5
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemSet.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemSet.p_sem[0].PendList.NbrEntries:0
TEST.VALUE:sysSem.OSSemSet.p_sem[0].Ctr:<<MIN>>
TEST.VALUE:sysSem.OSSemSet.cnt:<<MIN>>
TEST.VALUE:sysSem.OSSemSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemSet.p_sem[0].Ctr:0
TEST.END

-- Test Case: OSSemSet5
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OSSemSet
TEST.NEW
TEST.NAME:OSSemSet5
TEST.BASIS_PATH:5 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_sem == (OS_SEM *)0) ==> FALSE
      (3) while 0 ==> FALSE
      (4) if (p_sem->Ctr > (OS_SEM_CTR)0) ==> FALSE
      (5) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 5
      Only one possible value for condition in branch 6
TEST.END_NOTES:
TEST.VALUE:sysSem.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysSem.OSSemSet.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OSSemSet.p_sem[0].PendList.NbrEntries:1
TEST.VALUE:sysSem.OSSemSet.p_sem[0].Ctr:<<MIN>>
TEST.VALUE:sysSem.OSSemSet.cnt:<<MIN>>
TEST.VALUE:sysSem.OSSemSet.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OSSemSet.p_err[0]:OS_ERR_TASK_WAITING
TEST.END

-- Subprogram: OS_SemClr

-- Test Case: SemClr1
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OS_SemClr
TEST.NEW
TEST.NAME:SemClr1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sysSem.OS_SemClr.p_sem:<<malloc 1>>
TEST.EXPECTED:sysSem.OS_SemClr.p_sem[0].Type:1313820229
TEST.EXPECTED:sysSem.OS_SemClr.p_sem[0].PendList.HeadPtr:<<null>>
TEST.EXPECTED:sysSem.OS_SemClr.p_sem[0].PendList.TailPtr:<<null>>
TEST.EXPECTED:sysSem.OS_SemClr.p_sem[0].PendList.NbrEntries:0
TEST.EXPECTED:sysSem.OS_SemClr.p_sem[0].Ctr:0
TEST.EXPECTED:sysSem.OS_SemClr.p_sem[0].TS:0
TEST.END

-- Subprogram: OS_SemInit

-- Test Case: SemInit1
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OS_SemInit
TEST.NEW
TEST.NAME:SemInit1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sysSem.OS_SemInit.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.<<GLOBAL>>.OSSemQty:0
TEST.EXPECTED:sysSem.OS_SemInit.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OS_SemPost

-- Test Case: SemPost1
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OS_SemPost
TEST.NEW
TEST.NAME:SemPost1
TEST.BASIS_PATH:1 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while 0 ==> FALSE
      (2) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
      (14) if ((opt & (OS_OPT)0x200U) != (OS_OPT)0) ==> FALSE
      (15) while (cnt > 0U) ==> FALSE
      (17) if ((opt & (OS_OPT)0x8000U) == (OS_OPT)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 2
      Cannot set local variable p_pend_list in branch 15
      Cannot set cnt due to assignment
      Cannot set local variable p_pend_list in branch 15
      Only one possible value for condition in branch 16
      Conflict: Multiple equality operators with different values (sysSem.OS_SemPost.opt) in branches 14/17
TEST.END_NOTES:
TEST.VALUE:sysSem.OS_SemPost.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].PendList.NbrEntries:0
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].Ctr:4294967295
TEST.VALUE:sysSem.OS_SemPost.opt:0
TEST.VALUE:sysSem.OS_SemPost.ts:<<MIN>>
TEST.VALUE:sysSem.OS_SemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OS_SemPost.p_err[0]:OS_ERR_SEM_OVF
TEST.EXPECTED:sysSem.OS_SemPost.return:0
TEST.END

-- Test Case: SemPost2
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OS_SemPost
TEST.NEW
TEST.NAME:SemPost2
TEST.BASIS_PATH:2 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while 0 ==> FALSE
      (2) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
      (14) if ((opt & (OS_OPT)0x200U) != (OS_OPT)0) ==> FALSE
      (15) while (cnt > 0U) ==> FALSE
      (17) if ((opt & (OS_OPT)0x8000U) == (OS_OPT)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 2
      Cannot set local variable p_pend_list in branch 15
      Cannot set cnt due to assignment
      Cannot set local variable p_pend_list in branch 15
      Only one possible value for condition in branch 16
TEST.END_NOTES:
TEST.STUB:sysCore.OS_Post
TEST.VALUE:sysSem.OS_SemPost.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].PendList.NbrEntries:1
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].Ctr:0
TEST.VALUE:sysSem.OS_SemPost.opt:0
TEST.VALUE:sysSem.OS_SemPost.ts:<<MIN>>
TEST.VALUE:sysSem.OS_SemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OS_SemPost.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysSem.OS_SemPost.return:0
TEST.END

-- Test Case: SemPost3
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OS_SemPost
TEST.NEW
TEST.NAME:SemPost3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while 0 ==> FALSE
      (2) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
      (14) if ((opt & (OS_OPT)0x200U) != (OS_OPT)0) ==> FALSE
      (15) while (cnt > 0U) ==> FALSE
      (17) if ((opt & (OS_OPT)0x8000U) == (OS_OPT)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 2
      Cannot set local variable p_pend_list in branch 15
      Cannot set cnt due to assignment
      Cannot set local variable p_pend_list in branch 15
      Only one possible value for condition in branch 16
      Conflict: Multiple equality operators with different values (sysSem.OS_SemPost.opt) in branches 14/17
TEST.END_NOTES:
TEST.VALUE:sysSem.OS_SemPost.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].PendList.NbrEntries:0
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].Ctr:0
TEST.VALUE:sysSem.OS_SemPost.opt:0
TEST.VALUE:sysSem.OS_SemPost.ts:<<MIN>>
TEST.VALUE:sysSem.OS_SemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OS_SemPost.p_sem[0].Ctr:1
TEST.EXPECTED:sysSem.OS_SemPost.p_sem[0].TS:0
TEST.EXPECTED:sysSem.OS_SemPost.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysSem.OS_SemPost.return:1
TEST.END

-- Test Case: SemPost4
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OS_SemPost
TEST.NEW
TEST.NAME:SemPost4
TEST.BASIS_PATH:3 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
      (14) if ((opt & (OS_OPT)0x200U) != (OS_OPT)0) ==> FALSE
      (15) while (cnt > 0U) ==> FALSE
      (16) while 0 ==> TRUE
      (17) if ((opt & (OS_OPT)0x8000U) == (OS_OPT)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 2
      Cannot set local variable p_pend_list in branch 15
      Cannot set cnt due to assignment
      Cannot set local variable p_pend_list in branch 15
      Only one possible value for condition in branch 16
      Conflict: Multiple equality operators with different values (sysSem.OS_SemPost.opt) in branches 14/17
TEST.END_NOTES:
TEST.STUB:sysCore.OS_Post
TEST.VALUE:sysSem.OS_SemPost.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].PendList.HeadPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].PendList.NbrEntries:1
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].Ctr:0
TEST.VALUE:sysSem.OS_SemPost.opt:512
TEST.VALUE:sysSem.OS_SemPost.ts:<<MIN>>
TEST.VALUE:sysSem.OS_SemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OS_SemPost.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysSem.OS_SemPost.return:0
TEST.END

-- Test Case: SemPost5
TEST.UNIT:sysSem
TEST.SUBPROGRAM:OS_SemPost
TEST.NEW
TEST.NAME:SemPost5
TEST.BASIS_PATH:4 of 17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while 0 ==> FALSE
      (2) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
      (14) if ((opt & (OS_OPT)0x200U) != (OS_OPT)0) ==> FALSE
      (15) while (cnt > 0U) ==> TRUE
      (17) if ((opt & (OS_OPT)0x8000U) == (OS_OPT)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 2
      Cannot set local variable p_pend_list in branch 15
      Cannot set cnt due to assignment
      Cannot set local variable p_pend_list in branch 15
      Cannot set p_pend_list due to assignment
      Cannot set p_pend_list due to assignment
      Only one possible value for condition in branch 16
      Conflict: Multiple equality operators with different values (sysSem.OS_SemPost.opt) in branches 14/17
TEST.END_NOTES:
TEST.STUB:sysCore.OS_Post
TEST.VALUE:sysSem.OS_SemPost.p_sem:<<malloc 1>>
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].PendList.NbrEntries:<<MAX>>
TEST.VALUE:sysSem.OS_SemPost.p_sem[0].Ctr:1
TEST.VALUE:sysSem.OS_SemPost.opt:32768
TEST.VALUE:sysSem.OS_SemPost.ts:<<MIN>>
TEST.VALUE:sysSem.OS_SemPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysSem.OS_SemPost.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysSem.OS_SemPost.return:1
TEST.END
