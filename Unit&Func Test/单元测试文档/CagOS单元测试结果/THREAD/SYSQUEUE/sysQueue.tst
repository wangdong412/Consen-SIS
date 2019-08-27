-- VectorCAST 6.4d (02/29/16)
-- Test Case Script
-- 
-- Environment    : THREAD
-- Unit(s) Under Test: sysApi sysCfg sysMsg sysQueue sysTime
-- 
-- Script Features
TEST.SCRIPT_FEATURE:C_DIRECT_ARRAY_INDEXING
TEST.SCRIPT_FEATURE:CPP_CLASS_OBJECT_REVISION
TEST.SCRIPT_FEATURE:MULTIPLE_UUT_SUPPORT
TEST.SCRIPT_FEATURE:MIXED_CASE_NAMES
TEST.SCRIPT_FEATURE:STATIC_HEADER_FUNCS_IN_UUTS
--

-- Unit: sysQueue

-- Subprogram: OSQCreate

-- Test Case: OSQCreate1
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQCreate
TEST.NEW
TEST.NAME:OSQCreate1
TEST.BASIS_PATH:1 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysQueue.OSQCreate.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQCreate.p_name:<<malloc 1>>
TEST.VALUE:sysQueue.OSQCreate.max_qty:<<MIN>>
TEST.VALUE:sysQueue.OSQCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQCreate.p_err[0]:OS_ERR_CREATE_ISR
TEST.END

-- Test Case: OSQCreate2
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQCreate
TEST.NEW
TEST.NAME:OSQCreate2
TEST.BASIS_PATH:2 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQCreate.p_q:<<null>>
TEST.VALUE:sysQueue.OSQCreate.p_name:<<malloc 1>>
TEST.VALUE:sysQueue.OSQCreate.max_qty:<<MIN>>
TEST.VALUE:sysQueue.OSQCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQCreate.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.END

-- Test Case: OSQCreate3
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQCreate
TEST.NEW
TEST.NAME:OSQCreate3
TEST.BASIS_PATH:3 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (max_qty == (OS_MSG_QTY)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQCreate.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQCreate.p_name:<<malloc 1>>
TEST.VALUE:sysQueue.OSQCreate.max_qty:0
TEST.VALUE:sysQueue.OSQCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQCreate.p_err[0]:OS_ERR_Q_SIZE
TEST.END

-- Test Case: OSQCreate4
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQCreate
TEST.NEW
TEST.NAME:OSQCreate4
TEST.BASIS_PATH:4 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (max_qty == (OS_MSG_QTY)0) ==> FALSE
      (4) while 0 ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 4
      Only one possible value for condition in branch 5
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSQQty:0
TEST.VALUE:sysQueue.OSQCreate.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQCreate.p_name:<<malloc 1>>
TEST.VALUE:sysQueue.OSQCreate.max_qty:1
TEST.VALUE:sysQueue.OSQCreate.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.<<GLOBAL>>.OSQQty:1
TEST.EXPECTED:sysQueue.OSQCreate.p_q[0].Type:1364542805
TEST.EXPECTED:sysQueue.OSQCreate.p_q[0].PendList.NbrEntries:0
TEST.EXPECTED:sysQueue.OSQCreate.p_q[0].MsgQ.NbrEntriesSize:1
TEST.EXPECTED:sysQueue.OSQCreate.p_q[0].MsgQ.NbrEntries:0
TEST.EXPECTED:sysQueue.OSQCreate.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OSQDel

-- Test Case: OSQDel1
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQDel
TEST.NEW
TEST.NAME:OSQDel1
TEST.BASIS_PATH:1 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysQueue.OSQDel.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQDel.opt:<<MIN>>
TEST.VALUE:sysQueue.OSQDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQDel.p_err[0]:OS_ERR_DEL_ISR
TEST.EXPECTED:sysQueue.OSQDel.return:0
TEST.END

-- Test Case: OSQDel2
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQDel
TEST.NEW
TEST.NAME:OSQDel2
TEST.BASIS_PATH:2 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQDel.p_q:<<null>>
TEST.VALUE:sysQueue.OSQDel.opt:<<MIN>>
TEST.VALUE:sysQueue.OSQDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQDel.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.EXPECTED:sysQueue.OSQDel.return:0
TEST.END

-- Test Case: OSQDel3
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQDel
TEST.NEW
TEST.NAME:OSQDel3
TEST.BASIS_PATH:3 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (5) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQDel.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQDel.opt:2
TEST.VALUE:sysQueue.OSQDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQDel.p_err[0]:OS_ERR_OPT_INVALID
TEST.EXPECTED:sysQueue.OSQDel.return:0
TEST.END

-- Test Case: OSQDel4
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQDel
TEST.NEW
TEST.NAME:OSQDel4
TEST.BASIS_PATH:4 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (14) case (opt) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Cannot set local variable p_pend_list in branch 7
      Conflict: Trying to set variable sysQueue.OSQDel.opt 'equal to' and 'not equal to' same value in branches 3/7
      Only one possible value for condition in branch 15
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSQQty:0
TEST.VALUE:sysQueue.OSQDel.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQDel.p_q[0].PendList.NbrEntries:0
TEST.VALUE:sysQueue.OSQDel.opt:0
TEST.VALUE:sysQueue.OSQDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.<<GLOBAL>>.OSQQty:65535
TEST.EXPECTED:sysQueue.OSQDel.p_q[0].Type:1313820229
TEST.EXPECTED:sysQueue.OSQDel.p_q[0].PendList.NbrEntries:0
TEST.EXPECTED:sysQueue.OSQDel.p_q[0].MsgQ.NbrEntriesSize:0
TEST.EXPECTED:sysQueue.OSQDel.p_q[0].MsgQ.NbrEntries:0
TEST.EXPECTED:sysQueue.OSQDel.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSQDel5
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQDel
TEST.NEW
TEST.NAME:OSQDel5
TEST.BASIS_PATH:5 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (14) case (opt) ==> default
      (15) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Cannot set local variable p_pend_list in branch 7
      Conflict: Trying to set variable sysQueue.OSQDel.opt 'equal to' and 'not equal to' same value in branches 3/7
      Only one possible value for condition in branch 15
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQDel.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQDel.p_q[0].PendList.NbrEntries:1
TEST.VALUE:sysQueue.OSQDel.opt:0
TEST.VALUE:sysQueue.OSQDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQDel.p_err[0]:OS_ERR_TASK_WAITING
TEST.END

-- Test Case: OSQDel6
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQDel
TEST.NEW
TEST.NAME:OSQDel6
TEST.BASIS_PATH:8 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 8
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (6) while 0 ==> FALSE
      (8) if (nbr_tasks == (OS_OBJ_QTY)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Cannot set local variable p_pend_list in branch 7
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 7
      Only one possible value for condition in branch 9
TEST.END_NOTES:
TEST.STUB:sysQueue.OS_QClr
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:3
TEST.VALUE:sysQueue.OSQDel.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQDel.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQDel.p_q[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQDel.p_q[0].PendList.NbrEntries:3
TEST.VALUE:sysQueue.OSQDel.p_q[0].MsgQ.NbrEntriesSize:3
TEST.VALUE:sysQueue.OSQDel.p_q[0].MsgQ.NbrEntries:0
TEST.VALUE:sysQueue.OSQDel.opt:1
TEST.VALUE:sysQueue.OSQDel.p_err:<<malloc 1>>
TEST.VALUE:sysQueue.OS_QClr.p_q[0].Type:1313820229
TEST.VALUE:sysQueue.OS_QClr.p_q[0].PendList.NbrEntries:0
TEST.VALUE:sysQueue.OS_QClr.p_q[0].MsgQ.NbrEntriesSize:0
TEST.VALUE:sysQueue.OS_QClr.p_q[0].MsgQ.NbrEntries:0
TEST.EXPECTED:sysQueue.OSQDel.p_q[0].PendList.NbrEntries:0
TEST.EXPECTED:sysQueue.OSQDel.p_q[0].MsgQ.NbrEntriesSize:0
TEST.EXPECTED:sysQueue.OSQDel.p_q[0].MsgQ.NbrEntries:0
TEST.EXPECTED:sysQueue.OSQDel.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysQueue.OSQDel.return:3
TEST.END

-- Test Case: PEND_LIST_HeadPtr_NULL_ERROR
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQDel
TEST.NEW
TEST.NAME:PEND_LIST_HeadPtr_NULL_ERROR
TEST.NOTES:
This is an automatically generated test case.
   Test Path 14
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0x1U
      (6) while 0 ==> FALSE
      (14) case (opt) ==> default
   Test Case Generation Notes:
      Only one possible value for condition in branch 6
      Cannot set local variable p_pend_list in branch 7
      Conflict: Trying to set variable sysQueue.OSQDel.opt 'equal to' and 'not equal to' same value in branches 4/7
      Only one possible value for condition in branch 15
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSQQty:3
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:12
TEST.VALUE:sysQueue.OSQDel.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQDel.p_q[0].PendList.NbrEntries:3
TEST.VALUE:sysQueue.OSQDel.p_q[0].MsgQ.NbrEntriesSize:3
TEST.VALUE:sysQueue.OSQDel.opt:1
TEST.VALUE:sysQueue.OSQDel.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.<<GLOBAL>>.OSQQty:2
TEST.EXPECTED:sysQueue.OSQDel.p_q[0].Type:1313820229
TEST.EXPECTED:sysQueue.OSQDel.p_q[0].PendList.NbrEntries:0
TEST.EXPECTED:sysQueue.OSQDel.p_q[0].MsgQ.NbrEntriesSize:0
TEST.EXPECTED:sysQueue.OSQDel.p_q[0].MsgQ.NbrEntries:0
TEST.EXPECTED:sysQueue.OSQDel.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysQueue.OSQDel.return:3
TEST.END

-- Subprogram: OSQFlush

-- Test Case: OSQFlush1
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQFlush
TEST.NEW
TEST.NAME:OSQFlush1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysQueue.OSQFlush.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQFlush.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQFlush.p_err[0]:OS_ERR_FLUSH_ISR
TEST.EXPECTED:sysQueue.OSQFlush.return:0
TEST.END

-- Test Case: OSQFlush2
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQFlush
TEST.NEW
TEST.NAME:OSQFlush2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQFlush.p_q:<<null>>
TEST.VALUE:sysQueue.OSQFlush.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQFlush.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.EXPECTED:sysQueue.OSQFlush.return:0
TEST.END

-- Test Case: OSQFlush3
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQFlush
TEST.NEW
TEST.NAME:OSQFlush3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) while 0 ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 3
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgQFreeAll
TEST.VALUE:sysMsg.OS_MsgQFreeAll.return:1
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQFlush.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQFlush.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQFlush.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysQueue.OSQFlush.return:1
TEST.END

-- Subprogram: OSQPend

-- Test Case: OSQPend1
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend1
TEST.BASIS_PATH:1 of 25
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_PEND_ISR
TEST.END

-- Test Case: OSQPend10
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 9
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U
      (7) if (p_ts != (CPU_TS *)0) ==> FALSE
      (8) while 0 ==> FALSE
      (9) if (*p_err == (OS_ERR_NONE)) ==> FALSE
      (11) if ((opt & (OS_OPT)0x8000U) != (OS_OPT)0) ==> FALSE
      (13) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Conflict: Trying to set variable sysQueue.OSQPend.p_ts 'equal to' and 'not equal to' same value in branches 7/8
      Only one possible value for condition in branch 14
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgQGet
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:0
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].TS:12
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgPtr:<<null>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgSize:11
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:0
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysQueue.OSQPend.p_msg_size[0]:11
TEST.EXPECTED:sysQueue.OSQPend.p_ts[0]:12
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSQPend11
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 9
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U
      (7) if (p_ts != (CPU_TS *)0) ==> FALSE
      (8) while 0 ==> FALSE
      (9) if (*p_err == (OS_ERR_NONE)) ==> FALSE
      (11) if ((opt & (OS_OPT)0x8000U) != (OS_OPT)0) ==> FALSE
      (13) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Conflict: Trying to set variable sysQueue.OSQPend.p_ts 'equal to' and 'not equal to' same value in branches 7/8
      Only one possible value for condition in branch 14
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgQGet
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:1
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].TS:12
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgPtr:<<null>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgSize:11
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:0
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysQueue.OSQPend.p_msg_size[0]:0
TEST.EXPECTED:sysQueue.OSQPend.p_ts[0]:12
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_PEND_ABORT
TEST.END

-- Test Case: OSQPend12
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 9
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U
      (7) if (p_ts != (CPU_TS *)0) ==> FALSE
      (8) while 0 ==> FALSE
      (9) if (*p_err == (OS_ERR_NONE)) ==> FALSE
      (11) if ((opt & (OS_OPT)0x8000U) != (OS_OPT)0) ==> FALSE
      (13) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Conflict: Trying to set variable sysQueue.OSQPend.p_ts 'equal to' and 'not equal to' same value in branches 7/8
      Only one possible value for condition in branch 14
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgQGet
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:1
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].TS:12
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgPtr:<<null>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgSize:11
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:0
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<null>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysQueue.OSQPend.p_msg_size[0]:0
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_PEND_ABORT
TEST.END

-- Test Case: OSQPend13
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 9
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U
      (7) if (p_ts != (CPU_TS *)0) ==> FALSE
      (8) while 0 ==> FALSE
      (9) if (*p_err == (OS_ERR_NONE)) ==> FALSE
      (11) if ((opt & (OS_OPT)0x8000U) != (OS_OPT)0) ==> FALSE
      (13) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Conflict: Trying to set variable sysQueue.OSQPend.p_ts 'equal to' and 'not equal to' same value in branches 7/8
      Only one possible value for condition in branch 14
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgQGet
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:3
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].TS:12
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgPtr:<<null>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgSize:11
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:0
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<null>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysQueue.OSQPend.p_msg_size[0]:0
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_TIMEOUT
TEST.END

-- Test Case: OSQPend14
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 9
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U
      (7) if (p_ts != (CPU_TS *)0) ==> FALSE
      (8) while 0 ==> FALSE
      (9) if (*p_err == (OS_ERR_NONE)) ==> FALSE
      (11) if ((opt & (OS_OPT)0x8000U) != (OS_OPT)0) ==> FALSE
      (13) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Conflict: Trying to set variable sysQueue.OSQPend.p_ts 'equal to' and 'not equal to' same value in branches 7/8
      Only one possible value for condition in branch 14
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgQGet
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:3
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].TS:12
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgPtr:<<null>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgSize:11
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:0
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysQueue.OSQPend.p_msg_size[0]:0
TEST.EXPECTED:sysQueue.OSQPend.p_ts[0]:0
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_TIMEOUT
TEST.END

-- Test Case: OSQPend15
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 9
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U
      (7) if (p_ts != (CPU_TS *)0) ==> FALSE
      (8) while 0 ==> FALSE
      (9) if (*p_err == (OS_ERR_NONE)) ==> FALSE
      (11) if ((opt & (OS_OPT)0x8000U) != (OS_OPT)0) ==> FALSE
      (13) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Conflict: Trying to set variable sysQueue.OSQPend.p_ts 'equal to' and 'not equal to' same value in branches 7/8
      Only one possible value for condition in branch 14
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgQGet
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:2
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].TS:12
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgPtr:<<null>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgSize:11
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:0
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysQueue.OSQPend.p_msg_size[0]:0
TEST.EXPECTED:sysQueue.OSQPend.p_ts[0]:12
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_OBJ_DEL
TEST.END

-- Test Case: OSQPend16
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend16
TEST.NOTES:
This is an automatically generated test case.
   Test Path 9
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U
      (7) if (p_ts != (CPU_TS *)0) ==> FALSE
      (8) while 0 ==> FALSE
      (9) if (*p_err == (OS_ERR_NONE)) ==> FALSE
      (11) if ((opt & (OS_OPT)0x8000U) != (OS_OPT)0) ==> FALSE
      (13) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Conflict: Trying to set variable sysQueue.OSQPend.p_ts 'equal to' and 'not equal to' same value in branches 7/8
      Only one possible value for condition in branch 14
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgQGet
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:2
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].TS:12
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgPtr:<<null>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgSize:11
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:0
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<null>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysQueue.OSQPend.p_msg_size[0]:0
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_OBJ_DEL
TEST.END

-- Test Case: OSQPend17
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend17
TEST.NOTES:
This is an automatically generated test case.
   Test Path 9
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U
      (7) if (p_ts != (CPU_TS *)0) ==> FALSE
      (8) while 0 ==> FALSE
      (9) if (*p_err == (OS_ERR_NONE)) ==> FALSE
      (11) if ((opt & (OS_OPT)0x8000U) != (OS_OPT)0) ==> FALSE
      (13) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Conflict: Trying to set variable sysQueue.OSQPend.p_ts 'equal to' and 'not equal to' same value in branches 7/8
      Only one possible value for condition in branch 14
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgQGet
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:<<MAX>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].TS:12
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgPtr:<<null>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgSize:11
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:0
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<null>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysQueue.OSQPend.p_msg_size[0]:0
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_STATUS_INVALID
TEST.END

-- Test Case: OSQPend2
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend2
TEST.BASIS_PATH:2 of 25
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.p_q:<<null>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.END

-- Test Case: OSQPend3
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend3
TEST.BASIS_PATH:3 of 25
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<null>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_PTR_INVALID
TEST.END

-- Test Case: OSQPend4
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend4
TEST.BASIS_PATH:4 of 25
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> FALSE
      (6) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:1
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_OPT_INVALID
TEST.END

-- Test Case: OSQPend5
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend5
TEST.BASIS_PATH:5 of 25
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U
      (7) if (p_ts != (CPU_TS *)0) ==> FALSE
      (8) while 0 ==> FALSE
      (9) if (*p_err == (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Conflict: Trying to set variable sysQueue.OSQPend.p_ts 'equal to' and 'not equal to' same value in branches 7/8
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgQGet
TEST.VALUE:sysMsg.OS_MsgQGet.return:VECTORCAST_INT1
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:0
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<null>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSQPend6
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend6
TEST.BASIS_PATH:6 of 25
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U
      (7) if (p_ts != (CPU_TS *)0) ==> FALSE
      (8) while 0 ==> FALSE
      (9) if (*p_err == (OS_ERR_NONE)) ==> TRUE
      (10) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Conflict: Trying to set variable sysQueue.OSQPend.p_ts 'equal to' and 'not equal to' same value in branches 7/8
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgQGet
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:0
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSQPend7
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend7
TEST.BASIS_PATH:7 of 25
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U
      (7) if (p_ts != (CPU_TS *)0) ==> FALSE
      (8) while 0 ==> FALSE
      (9) if (*p_err == (OS_ERR_NONE)) ==> FALSE
      (11) if ((opt & (OS_OPT)0x8000U) != (OS_OPT)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Conflict: Trying to set variable sysQueue.OSQPend.p_ts 'equal to' and 'not equal to' same value in branches 7/8
      Conflict: Multiple equality operators with different values (sysQueue.OSQPend.opt) in branches 4/11
      Only one possible value for condition in branch 12
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgQGet
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:<<MAX>>
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:0
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<null>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_SCHED_LOCKED
TEST.END

-- Test Case: OSQPend8
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend8
TEST.BASIS_PATH:8 of 25
TEST.NOTES:
This is an automatically generated test case.
   Test Path 8
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U
      (7) if (p_ts != (CPU_TS *)0) ==> FALSE
      (8) while 0 ==> FALSE
      (9) if (*p_err == (OS_ERR_NONE)) ==> FALSE
      (11) if ((opt & (OS_OPT)0x8000U) != (OS_OPT)0) ==> TRUE
      (12) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Conflict: Trying to set variable sysQueue.OSQPend.p_ts 'equal to' and 'not equal to' same value in branches 7/8
      Conflict: Multiple equality operators with different values (sysQueue.OSQPend.opt) in branches 4/11
      Only one possible value for condition in branch 12
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgQGet
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:8
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:32768
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<null>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_PEND_WOULD_BLOCK
TEST.END

-- Test Case: OSQPend9
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPend
TEST.NEW
TEST.NAME:OSQPend9
TEST.BASIS_PATH:9 of 25
TEST.NOTES:
This is an automatically generated test case.
   Test Path 9
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) if (p_msg_size == (OS_MSG_SIZE *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U
      (7) if (p_ts != (CPU_TS *)0) ==> FALSE
      (8) while 0 ==> FALSE
      (9) if (*p_err == (OS_ERR_NONE)) ==> FALSE
      (11) if ((opt & (OS_OPT)0x8000U) != (OS_OPT)0) ==> FALSE
      (13) if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Conflict: Trying to set variable sysQueue.OSQPend.p_ts 'equal to' and 'not equal to' same value in branches 7/8
      Only one possible value for condition in branch 14
TEST.END_NOTES:
TEST.STUB:sysMsg.OS_MsgQGet
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].PendStatus:0
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgPtr:<<null>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSTCBCurPtr[0].MsgSize:11
TEST.VALUE:sysQueue.OSQPend.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.timeout:<<MIN>>
TEST.VALUE:sysQueue.OSQPend.opt:0
TEST.VALUE:sysQueue.OSQPend.p_msg_size:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_ts:<<null>>
TEST.VALUE:sysQueue.OSQPend.p_err:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPend.p_err[0]:OS_ERR_A
TEST.EXPECTED:sysQueue.OSQPend.p_msg_size[0]:11
TEST.EXPECTED:sysQueue.OSQPend.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OSQPendAbort

-- Test Case: OSQPendAbort1
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPendAbort
TEST.NEW
TEST.NAME:OSQPendAbort1
TEST.BASIS_PATH:1 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MAX>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPendAbort.opt:<<MIN>>
TEST.VALUE:sysQueue.OSQPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPendAbort.p_err[0]:OS_ERR_PEND_ABORT_ISR
TEST.EXPECTED:sysQueue.OSQPendAbort.return:0
TEST.END

-- Test Case: OSQPendAbort2
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPendAbort
TEST.NEW
TEST.NAME:OSQPendAbort2
TEST.BASIS_PATH:2 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q:<<null>>
TEST.VALUE:sysQueue.OSQPendAbort.opt:<<MIN>>
TEST.VALUE:sysQueue.OSQPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPendAbort.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.EXPECTED:sysQueue.OSQPendAbort.return:0
TEST.END

-- Test Case: OSQPendAbort3
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPendAbort
TEST.NEW
TEST.NAME:OSQPendAbort3
TEST.BASIS_PATH:3 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (7) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPendAbort.opt:1
TEST.VALUE:sysQueue.OSQPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPendAbort.p_err[0]:OS_ERR_OPT_INVALID
TEST.EXPECTED:sysQueue.OSQPendAbort.return:0
TEST.END

-- Test Case: OSQPendAbort4
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPendAbort
TEST.NEW
TEST.NAME:OSQPendAbort4
TEST.BASIS_PATH:4 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0U
      (8) while 0 ==> FALSE
      (9) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0U) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 8
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 9
      Only one possible value for condition in branch 10
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q[0].PendList.NbrEntries:0
TEST.VALUE:sysQueue.OSQPendAbort.opt:0
TEST.VALUE:sysQueue.OSQPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPendAbort.p_err[0]:OS_ERR_PEND_ABORT_NONE
TEST.END

-- Test Case: OSQPendAbort5
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPendAbort
TEST.NEW
TEST.NAME:OSQPendAbort5
TEST.BASIS_PATH:5 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
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
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:1
TEST.VALUE:sysQueue.OSQPendAbort.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q[0].PendList.NbrEntries:1
TEST.VALUE:sysQueue.OSQPendAbort.opt:0
TEST.VALUE:sysQueue.OSQPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPendAbort.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysQueue.OSQPendAbort.return:1
TEST.END

-- Test Case: OSQPendAbort6
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPendAbort
TEST.NEW
TEST.NAME:OSQPendAbort6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
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
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:1
TEST.VALUE:sysQueue.OSQPendAbort.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q[0].PendList.HeadPtr[0].TCBPtr[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q[0].PendList.HeadPtr[0].TCBPtr[0].PendDataTblPtr[0].PendObjPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q[0].PendList.HeadPtr[0].TCBPtr[0].PendDataTblPtr[0].PendObjPtr[0].PendList.NbrEntries:0
TEST.VALUE:sysQueue.OSQPendAbort.p_q[0].PendList.HeadPtr[0].TCBPtr[0].TaskState:3
TEST.VALUE:sysQueue.OSQPendAbort.p_q[0].PendList.NbrEntries:1
TEST.VALUE:sysQueue.OSQPendAbort.opt:32768
TEST.VALUE:sysQueue.OSQPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPendAbort.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysQueue.OSQPendAbort.return:1
TEST.END

-- Test Case: OSQPendAbort7
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPendAbort
TEST.NEW
TEST.NAME:OSQPendAbort7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (OSIntNestingCtr > (OS_NESTING_CTR)0U) ==> FALSE
      (2) if (p_q == (OS_Q *)0) ==> FALSE
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
TEST.VALUE:uut_prototype_stubs.OS_PendAbort.p_obj[0].PendList.NbrEntries:0
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:<<MIN>>
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:1
TEST.VALUE:sysQueue.OSQPendAbort.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q[0].PendList.HeadPtr[0].TCBPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPendAbort.p_q[0].PendList.NbrEntries:1
TEST.VALUE:sysQueue.OSQPendAbort.opt:256
TEST.VALUE:sysQueue.OSQPendAbort.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPendAbort.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysQueue.OSQPendAbort.return:1
TEST.END

-- Subprogram: OSQPost

-- Test Case: OSQPost1
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPost
TEST.NEW
TEST.NAME:OSQPost1
TEST.BASIS_PATH:1 of 10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_q == (OS_Q *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.OSQPost.p_q:<<null>>
TEST.VALUE:sysQueue.OSQPost.p_void:VECTORCAST_INT1
TEST.VALUE:sysQueue.OSQPost.msg_size:<<MIN>>
TEST.VALUE:sysQueue.OSQPost.opt:<<MIN>>
TEST.VALUE:sysQueue.OSQPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPost.p_err[0]:OS_ERR_OBJ_PTR_NULL
TEST.END

-- Test Case: OSQPost10
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPost
TEST.NEW
TEST.NAME:OSQPost10
TEST.BASIS_PATH:10 of 10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 10
      (1) if (p_q == (OS_Q *)0) ==> FALSE
      (9) case (opt) ==> ((OS_OPT)0x10U | (OS_OPT)0x200U) | (OS_OPT)0x8000U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.OSQPost.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.NbrEntries:1
TEST.VALUE:sysQueue.OSQPost.p_void:VECTORCAST_INT1
TEST.VALUE:sysQueue.OSQPost.msg_size:<<MIN>>
TEST.VALUE:sysQueue.OSQPost.opt:33296
TEST.VALUE:sysQueue.OSQPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPost.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSQPost2
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPost
TEST.NEW
TEST.NAME:OSQPost2
TEST.BASIS_PATH:2 of 10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (p_q == (OS_Q *)0) ==> FALSE
      (10) case (opt) ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.OSQPost.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_void:VECTORCAST_INT1
TEST.VALUE:sysQueue.OSQPost.msg_size:<<MIN>>
TEST.VALUE:sysQueue.OSQPost.opt:1
TEST.VALUE:sysQueue.OSQPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPost.p_err[0]:OS_ERR_OPT_INVALID
TEST.END

-- Test Case: OSQPost3
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPost
TEST.NEW
TEST.NAME:OSQPost3
TEST.BASIS_PATH:3 of 10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_q == (OS_Q *)0) ==> FALSE
      (2) case (opt) ==> (OS_OPT)0U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSSchedLockNestingCtr:1
TEST.VALUE:sysQueue.OSQPost.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.NbrEntries:1
TEST.VALUE:sysQueue.OSQPost.p_void:VECTORCAST_INT1
TEST.VALUE:sysQueue.OSQPost.msg_size:<<MIN>>
TEST.VALUE:sysQueue.OSQPost.opt:0
TEST.VALUE:sysQueue.OSQPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPost.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSQPost4
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPost
TEST.NEW
TEST.NAME:OSQPost4
TEST.BASIS_PATH:4 of 10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (p_q == (OS_Q *)0) ==> FALSE
      (3) case (opt) ==> (OS_OPT)0x10U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.OSQPost.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.NbrEntries:1
TEST.VALUE:sysQueue.OSQPost.p_void:VECTORCAST_INT1
TEST.VALUE:sysQueue.OSQPost.msg_size:<<MIN>>
TEST.VALUE:sysQueue.OSQPost.opt:16
TEST.VALUE:sysQueue.OSQPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPost.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSQPost5
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPost
TEST.NEW
TEST.NAME:OSQPost5
TEST.BASIS_PATH:5 of 10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (p_q == (OS_Q *)0) ==> FALSE
      (4) case (opt) ==> (OS_OPT)0U | (OS_OPT)0x200U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.OSQPost.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.NbrEntries:1
TEST.VALUE:sysQueue.OSQPost.p_void:VECTORCAST_INT1
TEST.VALUE:sysQueue.OSQPost.msg_size:<<MIN>>
TEST.VALUE:sysQueue.OSQPost.opt:512
TEST.VALUE:sysQueue.OSQPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPost.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSQPost6
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPost
TEST.NEW
TEST.NAME:OSQPost6
TEST.BASIS_PATH:6 of 10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (p_q == (OS_Q *)0) ==> FALSE
      (5) case (opt) ==> (OS_OPT)0x10U | (OS_OPT)0x200U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.OSQPost.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.NbrEntries:1
TEST.VALUE:sysQueue.OSQPost.p_void:VECTORCAST_INT1
TEST.VALUE:sysQueue.OSQPost.msg_size:<<MIN>>
TEST.VALUE:sysQueue.OSQPost.opt:528
TEST.VALUE:sysQueue.OSQPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPost.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSQPost7
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPost
TEST.NEW
TEST.NAME:OSQPost7
TEST.BASIS_PATH:7 of 10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (p_q == (OS_Q *)0) ==> FALSE
      (6) case (opt) ==> (OS_OPT)0U | (OS_OPT)0x8000U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.OSQPost.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.NbrEntries:1
TEST.VALUE:sysQueue.OSQPost.p_void:VECTORCAST_INT1
TEST.VALUE:sysQueue.OSQPost.msg_size:<<MIN>>
TEST.VALUE:sysQueue.OSQPost.opt:32768
TEST.VALUE:sysQueue.OSQPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPost.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSQPost8
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPost
TEST.NEW
TEST.NAME:OSQPost8
TEST.BASIS_PATH:8 of 10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 8
      (1) if (p_q == (OS_Q *)0) ==> FALSE
      (7) case (opt) ==> (OS_OPT)0x10U | (OS_OPT)0x8000U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.OSQPost.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.NbrEntries:1
TEST.VALUE:sysQueue.OSQPost.p_void:VECTORCAST_INT1
TEST.VALUE:sysQueue.OSQPost.msg_size:<<MIN>>
TEST.VALUE:sysQueue.OSQPost.opt:32784
TEST.VALUE:sysQueue.OSQPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPost.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OSQPost9
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OSQPost
TEST.NEW
TEST.NAME:OSQPost9
TEST.BASIS_PATH:9 of 10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 9
      (1) if (p_q == (OS_Q *)0) ==> FALSE
      (8) case (opt) ==> ((OS_OPT)0U | (OS_OPT)0x200U) | (OS_OPT)0x8000U
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysQueue.OSQPost.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OSQPost.p_q[0].PendList.NbrEntries:1
TEST.VALUE:sysQueue.OSQPost.p_void:VECTORCAST_INT1
TEST.VALUE:sysQueue.OSQPost.msg_size:<<MIN>>
TEST.VALUE:sysQueue.OSQPost.opt:33280
TEST.VALUE:sysQueue.OSQPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OSQPost.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OS_QClr

-- Test Case: OS_QClr1
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OS_QClr
TEST.NEW
TEST.NAME:OS_QClr1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSMsgPool.NbrFree:1
TEST.VALUE:sysQueue.<<GLOBAL>>.OSMsgPool.NbrUsed:1
TEST.VALUE:sysQueue.OS_QClr.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OS_QClr.p_q[0].MsgQ.InPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OS_QClr.p_q[0].MsgQ.OutPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OS_QClr.p_q[0].MsgQ.NbrEntries:1
TEST.EXPECTED:sysQueue.<<GLOBAL>>.OSMsgPool.NbrFree:2
TEST.EXPECTED:sysQueue.<<GLOBAL>>.OSMsgPool.NbrUsed:0
TEST.EXPECTED:sysQueue.OS_QClr.p_q[0].Type:1313820229
TEST.EXPECTED:sysQueue.OS_QClr.p_q[0].MsgQ.NbrEntriesSize:0
TEST.EXPECTED:sysQueue.OS_QClr.p_q[0].MsgQ.NbrEntries:0
TEST.END

-- Subprogram: OS_QInit

-- Test Case: OS_QInit
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OS_QInit
TEST.NEW
TEST.NAME:OS_QInit
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sysQueue.OS_QInit.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.<<GLOBAL>>.OSQQty:0
TEST.EXPECTED:sysQueue.OS_QInit.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OS_QPost

-- Test Case: OS_QPost1
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OS_QPost
TEST.NEW
TEST.NAME:OS_QPost1
TEST.BASIS_PATH:1 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while 0 ==> FALSE
      (2) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> TRUE
      (3) if ((opt & (OS_OPT)0x10U) == (OS_OPT)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 2
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysQueue.OS_QPost.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OS_QPost.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OS_QPost.p_q[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OS_QPost.p_q[0].PendList.NbrEntries:0
TEST.VALUE:sysQueue.OS_QPost.p_q[0].MsgQ.NbrEntriesSize:1
TEST.VALUE:sysQueue.OS_QPost.p_q[0].MsgQ.NbrEntries:23
TEST.VALUE:sysQueue.OS_QPost.p_void:VECTORCAST_INT1
TEST.VALUE:sysQueue.OS_QPost.msg_size:<<MIN>>
TEST.VALUE:sysQueue.OS_QPost.opt:16
TEST.VALUE:sysQueue.OS_QPost.ts:<<MIN>>
TEST.VALUE:sysQueue.OS_QPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OS_QPost.p_err[0]:OS_ERR_Q_MAX
TEST.END

-- Test Case: OS_QPost2
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OS_QPost
TEST.NEW
TEST.NAME:OS_QPost2
TEST.BASIS_PATH:2 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while 0 ==> FALSE
      (2) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> TRUE
      (3) if ((opt & (OS_OPT)0x10U) == (OS_OPT)0) ==> FALSE
      (4) while 0 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 2
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysQueue.OS_QPost.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OS_QPost.p_q[0].PendList.NbrEntries:0
TEST.VALUE:sysQueue.OS_QPost.p_q[0].MsgQ.NbrEntriesSize:0
TEST.VALUE:sysQueue.OS_QPost.p_q[0].MsgQ.NbrEntries:1
TEST.VALUE:sysQueue.OS_QPost.p_void:VECTORCAST_INT1
TEST.VALUE:sysQueue.OS_QPost.msg_size:<<MIN>>
TEST.VALUE:sysQueue.OS_QPost.opt:0
TEST.VALUE:sysQueue.OS_QPost.ts:<<MIN>>
TEST.VALUE:sysQueue.OS_QPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OS_QPost.p_err[0]:OS_ERR_Q_MAX
TEST.END

-- Test Case: OS_QPost3
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OS_QPost
TEST.NEW
TEST.NAME:OS_QPost3
TEST.BASIS_PATH:3 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while 0 ==> FALSE
      (2) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> TRUE
      (3) if ((opt & (OS_OPT)0x10U) == (OS_OPT)0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 2
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.VALUE:sysQueue.<<GLOBAL>>.OSIntNestingCtr:1
TEST.VALUE:sysQueue.OS_QPost.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OS_QPost.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OS_QPost.p_q[0].PendList.NbrEntries:1
TEST.VALUE:sysQueue.OS_QPost.p_void:VECTORCAST_INT1
TEST.VALUE:sysQueue.OS_QPost.msg_size:<<MIN>>
TEST.VALUE:sysQueue.OS_QPost.opt:0
TEST.VALUE:sysQueue.OS_QPost.ts:<<MIN>>
TEST.VALUE:sysQueue.OS_QPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OS_QPost.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_QPost4
TEST.UNIT:sysQueue
TEST.SUBPROGRAM:OS_QPost
TEST.NEW
TEST.NAME:OS_QPost4
TEST.BASIS_PATH:4 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while 0 ==> FALSE
      (2) if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) ==> FALSE
      (5) if ((opt & (OS_OPT)0x200U) != (OS_OPT)0) ==> FALSE
      (6) while (cnt > 0U) ==> FALSE
      (8) if ((opt & (OS_OPT)0x8000U) == (OS_OPT)0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Cannot set p_pend_list due to assignment
      Cannot set local variable p_pend_list in branch 2
      Cannot set local variable p_pend_list in branch 6
      Cannot set cnt due to assignment
      Cannot set local variable p_pend_list in branch 6
      Only one possible value for condition in branch 7
      Conflict: Multiple equality operators with different values (sysQueue.OS_QPost.opt) in branches 5/8
TEST.END_NOTES:
TEST.VALUE:sysQueue.OS_QPost.p_q:<<malloc 1>>
TEST.VALUE:sysQueue.OS_QPost.p_q[0].PendList.HeadPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OS_QPost.p_q[0].PendList.TailPtr:<<malloc 1>>
TEST.VALUE:sysQueue.OS_QPost.p_q[0].PendList.NbrEntries:1
TEST.VALUE:sysQueue.OS_QPost.p_void:VECTORCAST_INT1
TEST.VALUE:sysQueue.OS_QPost.msg_size:<<MIN>>
TEST.VALUE:sysQueue.OS_QPost.opt:65535
TEST.VALUE:sysQueue.OS_QPost.ts:<<MIN>>
TEST.VALUE:sysQueue.OS_QPost.p_err:<<malloc 1>>
TEST.EXPECTED:sysQueue.OS_QPost.p_err[0]:OS_ERR_NONE
TEST.END
