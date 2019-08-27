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

-- Unit: sysMsg

-- Subprogram: OS_MsgPoolInit

-- Test Case: MsgPoolInit
TEST.UNIT:sysMsg
TEST.SUBPROGRAM:OS_MsgPoolInit
TEST.NEW
TEST.NAME:MsgPoolInit
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSCfg_MsgPoolBasePtr == (OS_MSG *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysCfg.<<GLOBAL>>.OSCfg_MsgPool[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysCfg.<<GLOBAL>>.OSCfg_MsgPool[0].MsgSize:1
TEST.VALUE:sysCfg.<<GLOBAL>>.OSCfg_MsgPool[0].MsgTS:1
TEST.VALUE:sysCfg.<<GLOBAL>>.OSCfg_MsgPool[1].NextPtr:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgPoolInit.p_err:<<malloc 1>>
TEST.EXPECTED:sysMsg.OS_MsgPoolInit.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OS_MsgQFreeAll

-- Test Case: MsgQFreeALL1
TEST.UNIT:sysMsg
TEST.SUBPROGRAM:OS_MsgQFreeAll
TEST.NEW
TEST.NAME:MsgQFreeALL1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_msg_q->NbrEntries > (OS_MSG_QTY)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMsg.OS_MsgQFreeAll.p_msg_q:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQFreeAll.p_msg_q[0].NbrEntries:<<MIN>>
TEST.EXPECTED:sysMsg.OS_MsgQFreeAll.return:0
TEST.END

-- Test Case: MsgQFreeAll2
TEST.UNIT:sysMsg
TEST.SUBPROGRAM:OS_MsgQFreeAll
TEST.NEW
TEST.NAME:MsgQFreeAll2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (p_msg_q->NbrEntries > (OS_MSG_QTY)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMsg.OS_MsgQFreeAll.p_msg_q:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQFreeAll.p_msg_q[0].InPtr:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQFreeAll.p_msg_q[0].NbrEntries:<<MAX>>
TEST.EXPECTED:sysMsg.OS_MsgQFreeAll.p_msg_q[0].NbrEntriesSize:0
TEST.EXPECTED:sysMsg.OS_MsgQFreeAll.return:<<MAX>>
TEST.END

-- Subprogram: OS_MsgQGet

-- Test Case: MsgQGet1
TEST.UNIT:sysMsg
TEST.SUBPROGRAM:OS_MsgQGet
TEST.NEW
TEST.NAME:MsgQGet1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_msg_q->NbrEntries == (OS_MSG_QTY)0) ==> TRUE
      (2) if (p_ts != (CPU_TS *)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].NbrEntries:0
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_size:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_ts:<<null>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_err:<<malloc 1>>
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_msg_size[0]:0
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_err[0]:OS_ERR_Q_EMPTY
TEST.END

-- Test Case: MsgQGet2
TEST.UNIT:sysMsg
TEST.SUBPROGRAM:OS_MsgQGet
TEST.NEW
TEST.NAME:MsgQGet2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (p_msg_q->NbrEntries == (OS_MSG_QTY)0) ==> TRUE
      (2) if (p_ts != (CPU_TS *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].NbrEntries:0
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_size:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_ts:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_err:<<malloc 1>>
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_msg_size[0]:0
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_ts[0]:0
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_err[0]:OS_ERR_Q_EMPTY
TEST.END

-- Test Case: MsgQGet3
TEST.UNIT:sysMsg
TEST.SUBPROGRAM:OS_MsgQGet
TEST.NEW
TEST.NAME:MsgQGet3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_msg_q->NbrEntries == (OS_MSG_QTY)0) ==> FALSE
      (3) if (p_ts != (CPU_TS *)0) ==> FALSE
      (4) if (p_msg_q->OutPtr == (OS_MSG *)0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Trying to set variable sysMsg.OS_MsgQGet.p_ts 'equal to' and 'not equal to' same value in branches 3/4
TEST.END_NOTES:
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NbrFree:1
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NbrUsed:1
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].OutPtr:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].OutPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].OutPtr[0].MsgSize:1
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].NbrEntries:1
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_size:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_ts:<<null>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_err:<<malloc 1>>
TEST.EXPECTED:sysMsg.<<GLOBAL>>.OSMsgPool.NbrFree:2
TEST.EXPECTED:sysMsg.<<GLOBAL>>.OSMsgPool.NbrUsed:0
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_msg_q[0].NbrEntries:0
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_msg_size[0]:1
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: MsgQGet4
TEST.UNIT:sysMsg
TEST.SUBPROGRAM:OS_MsgQGet
TEST.NEW
TEST.NAME:MsgQGet4
TEST.BASIS_PATH:4 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (p_msg_q->NbrEntries == (OS_MSG_QTY)0) ==> FALSE
      (3) if (p_ts != (CPU_TS *)0) ==> FALSE
      (4) if (p_msg_q->OutPtr == (OS_MSG *)0) ==> TRUE
   Test Case Generation Notes:
      Conflict: Trying to set variable sysMsg.OS_MsgQGet.p_ts 'equal to' and 'not equal to' same value in branches 3/4
TEST.END_NOTES:
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NbrFree:9
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NbrUsed:9
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].OutPtr:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].OutPtr[0].NextPtr:<<null>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].OutPtr[0].MsgSize:0
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].NbrEntries:1
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_size:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_ts:<<null>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_err:<<malloc 1>>
TEST.EXPECTED:sysMsg.<<GLOBAL>>.OSMsgPool.NbrFree:10
TEST.EXPECTED:sysMsg.<<GLOBAL>>.OSMsgPool.NbrUsed:8
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_msg_q[0].NbrEntries:0
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_msg_size[0]:0
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: MsgQGet5
TEST.UNIT:sysMsg
TEST.SUBPROGRAM:OS_MsgQGet
TEST.NEW
TEST.NAME:MsgQGet5
TEST.BASIS_PATH:5 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (p_msg_q->NbrEntries == (OS_MSG_QTY)0) ==> FALSE
      (3) if (p_ts != (CPU_TS *)0) ==> TRUE
      (4) if (p_msg_q->OutPtr == (OS_MSG *)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NbrFree:1
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NbrUsed:1
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].OutPtr:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].OutPtr[0].NextPtr:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].OutPtr[0].MsgSize:65535
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].OutPtr[0].MsgTS:1
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_q[0].NbrEntries:1
TEST.VALUE:sysMsg.OS_MsgQGet.p_msg_size:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_ts:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQGet.p_err:<<malloc 1>>
TEST.EXPECTED:sysMsg.<<GLOBAL>>.OSMsgPool.NbrFree:2
TEST.EXPECTED:sysMsg.<<GLOBAL>>.OSMsgPool.NbrUsed:0
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_msg_q[0].NbrEntries:0
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_msg_size[0]:65535
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_ts[0]:1
TEST.EXPECTED:sysMsg.OS_MsgQGet.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OS_MsgQInit

-- Test Case: MsgQInit1
TEST.UNIT:sysMsg
TEST.SUBPROGRAM:OS_MsgQInit
TEST.NEW
TEST.NAME:MsgQInit1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sysMsg.OS_MsgQInit.p_msg_q:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQInit.size:<<MIN>>
TEST.EXPECTED:sysMsg.OS_MsgQInit.p_msg_q[0].NbrEntriesSize:0
TEST.EXPECTED:sysMsg.OS_MsgQInit.p_msg_q[0].NbrEntries:0
TEST.END

-- Subprogram: OS_MsgQPut

-- Test Case: MsgQput1
TEST.UNIT:sysMsg
TEST.SUBPROGRAM:OS_MsgQPut
TEST.NEW
TEST.NAME:MsgQput1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_msg_q->NbrEntries >= p_msg_q->NbrEntriesSize) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q[0].NbrEntriesSize:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q[0].NbrEntries:<<MAX>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_void:VECTORCAST_INT1
TEST.VALUE:sysMsg.OS_MsgQPut.msg_size:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.opt:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.ts:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_err:<<malloc 1>>
TEST.EXPECTED:sysMsg.OS_MsgQPut.p_err[0]:OS_ERR_Q_MAX
TEST.END

-- Test Case: MsgQput2
TEST.UNIT:sysMsg
TEST.SUBPROGRAM:OS_MsgQPut
TEST.NEW
TEST.NAME:MsgQput2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (p_msg_q->NbrEntries >= p_msg_q->NbrEntriesSize) ==> FALSE
      (2) if (OSMsgPool.NbrFree == (OS_MSG_QTY)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NbrFree:0
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q[0].NbrEntriesSize:<<MAX>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q[0].NbrEntries:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_void:VECTORCAST_INT1
TEST.VALUE:sysMsg.OS_MsgQPut.msg_size:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.opt:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.ts:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_err:<<malloc 1>>
TEST.EXPECTED:sysMsg.OS_MsgQPut.p_err[0]:OS_ERR_MSG_POOL_EMPTY
TEST.END

-- Test Case: MsgQput3
TEST.UNIT:sysMsg
TEST.SUBPROGRAM:OS_MsgQPut
TEST.NEW
TEST.NAME:MsgQput3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_msg_q->NbrEntries >= p_msg_q->NbrEntriesSize) ==> FALSE
      (2) if (OSMsgPool.NbrFree == (OS_MSG_QTY)0) ==> FALSE
      (3) if (p_msg_q->NbrEntries == (OS_MSG_QTY)0) ==> FALSE
      (4) if ((opt & (OS_OPT)0x10U) == (OS_OPT)0U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NextPtr:<<malloc 1>>
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NbrFree:1
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NbrUsed:1
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q[0].NbrEntriesSize:1
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q[0].NbrEntries:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_void:VECTORCAST_INT1
TEST.VALUE:sysMsg.OS_MsgQPut.msg_size:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.opt:16
TEST.VALUE:sysMsg.OS_MsgQPut.ts:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_err:<<malloc 1>>
TEST.EXPECTED:sysMsg.<<GLOBAL>>.OSMsgPool.NbrFree:0
TEST.EXPECTED:sysMsg.<<GLOBAL>>.OSMsgPool.NbrUsed:2
TEST.EXPECTED:sysMsg.OS_MsgQPut.p_msg_q[0].NbrEntriesSize:1
TEST.EXPECTED:sysMsg.OS_MsgQPut.p_msg_q[0].NbrEntries:1
TEST.EXPECTED:sysMsg.OS_MsgQPut.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: MsgQput4
TEST.UNIT:sysMsg
TEST.SUBPROGRAM:OS_MsgQPut
TEST.NEW
TEST.NAME:MsgQput4
TEST.BASIS_PATH:5 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (p_msg_q->NbrEntries >= p_msg_q->NbrEntriesSize) ==> FALSE
      (2) if (OSMsgPool.NbrFree == (OS_MSG_QTY)0) ==> FALSE
      (3) if (p_msg_q->NbrEntries == (OS_MSG_QTY)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NextPtr:<<malloc 1>>
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NbrFree:1
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NbrUsed:1
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q[0].InPtr:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q[0].OutPtr:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q[0].NbrEntriesSize:<<MAX>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q[0].NbrEntries:1
TEST.VALUE:sysMsg.OS_MsgQPut.p_void:VECTORCAST_INT1
TEST.VALUE:sysMsg.OS_MsgQPut.msg_size:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.opt:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.ts:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_err:<<malloc 1>>
TEST.EXPECTED:sysMsg.<<GLOBAL>>.OSMsgPool.NbrFree:0
TEST.EXPECTED:sysMsg.<<GLOBAL>>.OSMsgPool.NbrUsed:2
TEST.EXPECTED:sysMsg.OS_MsgQPut.p_msg_q[0].NbrEntries:2
TEST.EXPECTED:sysMsg.OS_MsgQPut.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: MsgQput5
TEST.UNIT:sysMsg
TEST.SUBPROGRAM:OS_MsgQPut
TEST.NEW
TEST.NAME:MsgQput5
TEST.BASIS_PATH:4 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (p_msg_q->NbrEntries >= p_msg_q->NbrEntriesSize) ==> FALSE
      (2) if (OSMsgPool.NbrFree == (OS_MSG_QTY)0) ==> FALSE
      (3) if (p_msg_q->NbrEntries == (OS_MSG_QTY)0) ==> FALSE
      (4) if ((opt & (OS_OPT)0x10U) == (OS_OPT)0U) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NextPtr:<<malloc 1>>
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NbrFree:1
TEST.VALUE:sysMsg.<<GLOBAL>>.OSMsgPool.NbrUsed:0
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q[0].InPtr:<<malloc 1>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q[0].NbrEntriesSize:12
TEST.VALUE:sysMsg.OS_MsgQPut.p_msg_q[0].NbrEntries:1
TEST.VALUE:sysMsg.OS_MsgQPut.p_void:VECTORCAST_INT1
TEST.VALUE:sysMsg.OS_MsgQPut.msg_size:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.opt:65535
TEST.VALUE:sysMsg.OS_MsgQPut.ts:<<MIN>>
TEST.VALUE:sysMsg.OS_MsgQPut.p_err:<<malloc 1>>
TEST.EXPECTED:sysMsg.<<GLOBAL>>.OSMsgPool.NbrFree:0
TEST.EXPECTED:sysMsg.<<GLOBAL>>.OSMsgPool.NbrUsed:1
TEST.EXPECTED:sysMsg.OS_MsgQPut.p_msg_q[0].NbrEntries:2
TEST.EXPECTED:sysMsg.OS_MsgQPut.p_err[0]:OS_ERR_NONE
TEST.END
