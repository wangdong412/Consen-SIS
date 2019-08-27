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

-- Unit: sysTick

-- Subprogram: OS_TickListInsert

-- Test Case: OS_TickListInsert1
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListInsert
TEST.NEW
TEST.NAME:OS_TickListInsert1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_list->TCB_Ptr == (OS_TCB *)0) ==> FALSE
      (2) while (p_tcb2 != (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Trying to set variable sysTick.OS_TickListInsert.p_list.TCB_Ptr 'equal to' and 'not equal to' same value in branches 1/2
TEST.END_NOTES:
TEST.VALUE:sysTick.OS_TickListInsert.p_list:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsert.p_list[0].TCB_Ptr:<<null>>
TEST.VALUE:sysTick.OS_TickListInsert.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsert.time:<<MIN>>
TEST.EXPECTED:sysTick.OS_TickListInsert.p_tcb[0].TickNextPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListInsert.p_tcb[0].TickPrevPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListInsert.p_tcb[0].TickRemain:0
TEST.END

-- Test Case: OS_TickListInsert2
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListInsert
TEST.NEW
TEST.NAME:OS_TickListInsert2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (p_list->TCB_Ptr == (OS_TCB *)0) ==> FALSE
      (2) while (p_tcb2 != (OS_TCB *)0) ==> TRUE
      (3) if (remain <= p_tcb2->TickRemain) ==> TRUE
      (4) if (p_tcb2->TickPrevPtr == (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTick.OS_TickListInsert.p_list:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsert.p_list[0].TCB_Ptr:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsert.p_list[0].TCB_Ptr[0].TickPrevPtr:<<null>>
TEST.VALUE:sysTick.OS_TickListInsert.p_list[0].TCB_Ptr[0].TickRemain:<<MAX>>
TEST.VALUE:sysTick.OS_TickListInsert.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsert.time:<<MIN>>
TEST.EXPECTED:sysTick.OS_TickListInsert.p_tcb[0].TickPrevPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListInsert.p_tcb[0].TickRemain:0
TEST.END

-- Test Case: OS_TickListInsert3
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListInsert
TEST.NEW
TEST.NAME:OS_TickListInsert3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_list->TCB_Ptr == (OS_TCB *)0) ==> FALSE
      (2) while (p_tcb2 != (OS_TCB *)0) ==> TRUE
      (3) if (remain <= p_tcb2->TickRemain) ==> TRUE
      (4) if (p_tcb2->TickPrevPtr == (OS_TCB *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTick.OS_TickListInsert.p_list:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsert.p_list[0].TCB_Ptr:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsert.p_list[0].TCB_Ptr[0].TickNextPtr:<<null>>
TEST.VALUE:sysTick.OS_TickListInsert.p_list[0].TCB_Ptr[0].TickPrevPtr:<<null>>
TEST.VALUE:sysTick.OS_TickListInsert.p_list[0].TCB_Ptr[0].TickRemain:0
TEST.VALUE:sysTick.OS_TickListInsert.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsert.time:19
TEST.EXPECTED:sysTick.OS_TickListInsert.p_tcb[0].TickNextPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListInsert.p_tcb[0].TickRemain:19
TEST.END

-- Test Case: OS_TickListInsert4
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListInsert
TEST.NEW
TEST.NAME:OS_TickListInsert4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (p_list->TCB_Ptr == (OS_TCB *)0) ==> FALSE
      (2) while (p_tcb2 != (OS_TCB *)0) ==> TRUE
      (3) if (remain <= p_tcb2->TickRemain) ==> TRUE
      (4) if (p_tcb2->TickPrevPtr == (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTick.OS_TickListInsert.p_list:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsert.p_list[0].TCB_Ptr:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsert.p_list[0].TCB_Ptr[0].TickPrevPtr:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsert.p_list[0].TCB_Ptr[0].TickRemain:<<MAX>>
TEST.VALUE:sysTick.OS_TickListInsert.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsert.time:<<MIN>>
TEST.EXPECTED:sysTick.OS_TickListInsert.p_tcb[0].TickRemain:0
TEST.END

-- Subprogram: OS_TickListInsertDly

-- Test Case: OS_TickListInsertDly1
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListInsertDly
TEST.NEW
TEST.NAME:OS_TickListInsertDly1
TEST.BASIS_PATH:1 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (opt == (OS_OPT)0x4U) ==> FALSE
      (3) if (opt == (OS_OPT)0x8U) ==> FALSE
      (6) if (time > (OS_TICK)0U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickCtr:500
TEST.VALUE:sysTick.OS_TickListInsertDly.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsertDly.time:500
TEST.VALUE:sysTick.OS_TickListInsertDly.opt:4
TEST.VALUE:sysTick.OS_TickListInsertDly.p_err:<<malloc 1>>
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_tcb[0].TickRemain:0
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_err[0]:OS_ERR_TIME_ZERO_DLY
TEST.END

-- Test Case: OS_TickListInsertDly2
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListInsertDly
TEST.NEW
TEST.NAME:OS_TickListInsertDly2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (opt == (OS_OPT)0x4U) ==> FALSE
      (3) if (opt == (OS_OPT)0x8U) ==> FALSE
      (6) if (time > (OS_TICK)0U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickCtr:0
TEST.VALUE:sysTick.OS_TickListInsertDly.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsertDly.time:<<MAX>>
TEST.VALUE:sysTick.OS_TickListInsertDly.opt:4
TEST.VALUE:sysTick.OS_TickListInsertDly.p_err:<<malloc 1>>
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_tcb[0].TickRemain:0
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_err[0]:OS_ERR_TIME_ZERO_DLY
TEST.END

-- Test Case: OS_TickListInsertDly3
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListInsertDly
TEST.NEW
TEST.NAME:OS_TickListInsertDly3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (opt == (OS_OPT)0x4U) ==> FALSE
      (3) if (opt == (OS_OPT)0x8U) ==> FALSE
      (6) if (time > (OS_TICK)0U) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysTick.OS_TickListInsert
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickCtr:0
TEST.VALUE:sysTick.OS_TickListInsertDly.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsertDly.time:500
TEST.VALUE:sysTick.OS_TickListInsertDly.opt:4
TEST.VALUE:sysTick.OS_TickListInsertDly.p_err:<<malloc 1>>
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_tcb[0].TaskState:1
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_tcb[0].TickRemain:0
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TickListInsertDly4
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListInsertDly
TEST.NEW
TEST.NAME:OS_TickListInsertDly4
TEST.BASIS_PATH:3 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (opt == (OS_OPT)0x4U) ==> FALSE
      (3) if (opt == (OS_OPT)0x8U) ==> TRUE
      (4) if (OSTickCtr - p_tcb->TickCtrPrev > time) ==> FALSE
      (5) if (remain > (OS_TICK)((((sizeof(OS_TICK) * 8U) / 2U >= 4U * 8U) ? 4294967295U : ((1U << (sizeof(OS_TICK) * 8U) / 2U) - 1UL)) << (sizeof(OS_TICK) * 8U) / 2U) || remain == (OS_TICK)0U) ==> TRUE
   Test Case Generation Notes:
      Cannot set remain due to assignment
TEST.END_NOTES:
TEST.STUB:sysTick.OS_TickListInsert
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickCtr:<<MAX>>
TEST.VALUE:sysTick.OS_TickListInsertDly.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsertDly.p_tcb[0].TickCtrPrev:1
TEST.VALUE:sysTick.OS_TickListInsertDly.time:0
TEST.VALUE:sysTick.OS_TickListInsertDly.opt:8
TEST.VALUE:sysTick.OS_TickListInsertDly.p_err:<<malloc 1>>
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_tcb[0].TaskState:1
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_tcb[0].TickCtrPrev:<<MAX>>
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TickListInsertDly5
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListInsertDly
TEST.NEW
TEST.NAME:OS_TickListInsertDly5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (opt == (OS_OPT)0x4U) ==> FALSE
      (3) if (opt == (OS_OPT)0x8U) ==> TRUE
      (4) if (OSTickCtr - p_tcb->TickCtrPrev > time) ==> FALSE
      (5) if (remain > (OS_TICK)((((sizeof(OS_TICK) * 8U) / 2U >= 4U * 8U) ? 4294967295U : ((1U << (sizeof(OS_TICK) * 8U) / 2U) - 1UL)) << (sizeof(OS_TICK) * 8U) / 2U) || remain == (OS_TICK)0U) ==> TRUE
   Test Case Generation Notes:
      Cannot set remain due to assignment
TEST.END_NOTES:
TEST.STUB:sysTick.OS_TickListInsert
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickCtr:1
TEST.VALUE:sysTick.OS_TickListInsertDly.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsertDly.p_tcb[0].TickCtrPrev:1
TEST.VALUE:sysTick.OS_TickListInsertDly.time:<<MAX>>
TEST.VALUE:sysTick.OS_TickListInsertDly.opt:8
TEST.VALUE:sysTick.OS_TickListInsertDly.p_err:<<malloc 1>>
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_tcb[0].TickRemain:0
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_tcb[0].TickCtrPrev:0
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_err[0]:OS_ERR_TIME_ZERO_DLY
TEST.END

-- Test Case: OS_TickListInsertDly6
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListInsertDly
TEST.NEW
TEST.NAME:OS_TickListInsertDly6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (opt == (OS_OPT)0x4U) ==> FALSE
      (3) if (opt == (OS_OPT)0x8U) ==> TRUE
      (4) if (OSTickCtr - p_tcb->TickCtrPrev > time) ==> FALSE
      (5) if (remain > (OS_TICK)((((sizeof(OS_TICK) * 8U) / 2U >= 4U * 8U) ? 4294967295U : ((1U << (sizeof(OS_TICK) * 8U) / 2U) - 1UL)) << (sizeof(OS_TICK) * 8U) / 2U) || remain == (OS_TICK)0U) ==> TRUE
   Test Case Generation Notes:
      Cannot set remain due to assignment
TEST.END_NOTES:
TEST.STUB:sysTick.OS_TickListInsert
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickCtr:1
TEST.VALUE:sysTick.OS_TickListInsertDly.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsertDly.p_tcb[0].TickCtrPrev:1
TEST.VALUE:sysTick.OS_TickListInsertDly.time:500
TEST.VALUE:sysTick.OS_TickListInsertDly.opt:8
TEST.VALUE:sysTick.OS_TickListInsertDly.p_err:<<malloc 1>>
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_tcb[0].TaskState:1
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_tcb[0].TickCtrPrev:501
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_err[0]:OS_ERR_NONE
TEST.END

-- Test Case: OS_TickListInsertDly7
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListInsertDly
TEST.NEW
TEST.NAME:OS_TickListInsertDly7
TEST.BASIS_PATH:7 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (opt == (OS_OPT)0x4U) ==> TRUE
      (2) if (remain > (OS_TICK)((((sizeof(OS_TICK) * 8U) / 2U >= 4U * 8U) ? 4294967295U : ((1U << (sizeof(OS_TICK) * 8U) / 2U) - 1UL)) << (sizeof(OS_TICK) * 8U) / 2U) || remain == (OS_TICK)0U) ==> FALSE
   Test Case Generation Notes:
      Cannot set remain due to assignment
TEST.END_NOTES:
TEST.VALUE:sysTick.OS_TickListInsertDly.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsertDly.time:<<MIN>>
TEST.VALUE:sysTick.OS_TickListInsertDly.opt:0
TEST.VALUE:sysTick.OS_TickListInsertDly.p_err:<<malloc 1>>
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_tcb[0].TickRemain:0
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_err[0]:OS_ERR_TIME_ZERO_DLY
TEST.END

-- Test Case: OS_TickListInsertDly8
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListInsertDly
TEST.NEW
TEST.NAME:OS_TickListInsertDly8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (opt == (OS_OPT)0x4U) ==> TRUE
      (2) if (remain > (OS_TICK)((((sizeof(OS_TICK) * 8U) / 2U >= 4U * 8U) ? 4294967295U : ((1U << (sizeof(OS_TICK) * 8U) / 2U) - 1UL)) << (sizeof(OS_TICK) * 8U) / 2U) || remain == (OS_TICK)0U) ==> FALSE
   Test Case Generation Notes:
      Cannot set remain due to assignment
TEST.END_NOTES:
TEST.STUB:sysTick.OS_TickListInsert
TEST.VALUE:sysTick.OS_TickListInsertDly.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListInsertDly.time:4
TEST.VALUE:sysTick.OS_TickListInsertDly.opt:0
TEST.VALUE:sysTick.OS_TickListInsertDly.p_err:<<malloc 1>>
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_tcb[0].TaskState:1
TEST.EXPECTED:sysTick.OS_TickListInsertDly.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OS_TickListRemove

-- Test Case: OS_TickListRemove1
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListRemove
TEST.NEW
TEST.NAME:OS_TickListRemove1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_tcb1 == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb2 != (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb[0].TickNextPtr:<<null>>
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb[0].TickPrevPtr:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb[0].TickListPtr:<<malloc 1>>
TEST.EXPECTED:sysTick.OS_TickListRemove.p_tcb[0].TickNextPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListRemove.p_tcb[0].TickPrevPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListRemove.p_tcb[0].TickListPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListRemove.p_tcb[0].TickRemain:0
TEST.END

-- Test Case: OS_TickListRemove2
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListRemove
TEST.NEW
TEST.NAME:OS_TickListRemove2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_tcb1 == (OS_TCB *)0) ==> FALSE
      (3) if (p_tcb2 != (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb[0].TickNextPtr:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb[0].TickPrevPtr:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb[0].TickListPtr:<<malloc 1>>
TEST.EXPECTED:sysTick.OS_TickListRemove.p_tcb[0].TickNextPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListRemove.p_tcb[0].TickPrevPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListRemove.p_tcb[0].TickListPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListRemove.p_tcb[0].TickRemain:0
TEST.END

-- Test Case: OS_TickListRemove3
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListRemove
TEST.NEW
TEST.NAME:OS_TickListRemove3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_tcb1 == (OS_TCB *)0) ==> TRUE
      (2) if (p_tcb2 == (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb[0].TickNextPtr:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb[0].TickPrevPtr:<<null>>
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb[0].TickListPtr:<<malloc 1>>
TEST.EXPECTED:sysTick.OS_TickListRemove.p_tcb[0].TickNextPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListRemove.p_tcb[0].TickListPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListRemove.p_tcb[0].TickRemain:0
TEST.END

-- Test Case: OS_TickListRemove4
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListRemove
TEST.NEW
TEST.NAME:OS_TickListRemove4
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (p_tcb1 == (OS_TCB *)0) ==> TRUE
      (2) if (p_tcb2 == (OS_TCB *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb:<<malloc 1>>
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb[0].TickNextPtr:<<null>>
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb[0].TickPrevPtr:<<null>>
TEST.VALUE:sysTick.OS_TickListRemove.p_tcb[0].TickListPtr:<<malloc 1>>
TEST.EXPECTED:sysTick.OS_TickListRemove.p_tcb[0].TickListPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListRemove.p_tcb[0].TickRemain:0
TEST.END

-- Subprogram: OS_TickListResetPeak

-- Test Case: OS_TickListResetPeak
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListResetPeak
TEST.NEW
TEST.NAME:OS_TickListResetPeak
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.END

-- Subprogram: OS_TickListUpdateDly

-- Test Case: OS_TickListUpdateDLy1
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListUpdateDly
TEST.NEW
TEST.NAME:OS_TickListUpdateDLy1
TEST.BASIS_PATH:1 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_tcb != (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 1
TEST.END_NOTES:
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListDly.TCB_Ptr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListUpdateDly.return:0
TEST.END

-- Test Case: OS_TickListUpdateDLy2
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListUpdateDly
TEST.NEW
TEST.NAME:OS_TickListUpdateDLy2
TEST.BASIS_PATH:2 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (p_tcb != (OS_TCB *)0) ==> TRUE
      (2) while (p_tcb->TickRemain == 0U) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set p_tcb->TickRemain due to assignment
      Cannot set p_list due to assignment
TEST.END_NOTES:
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListDly.TCB_Ptr:<<malloc 1>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListDly.TCB_Ptr[0].TickRemain:3
TEST.EXPECTED:sysTick.OS_TickListUpdateDly.return:0
TEST.END

-- Test Case: OS_TickListUpdateDLy3
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListUpdateDly
TEST.NEW
TEST.NAME:OS_TickListUpdateDLy3
TEST.BASIS_PATH:3 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_tcb != (OS_TCB *)0) ==> TRUE
      (2) while (p_tcb->TickRemain == 0U) ==> TRUE
      (3) if (p_tcb->TaskState == (OS_STATE)1U) ==> FALSE
      (4) if (p_tcb->TaskState == (OS_STATE)5U) ==> FALSE
      (5) if (p_tcb == (OS_TCB *)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set p_tcb->TickRemain due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 5
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 5
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 5
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
TEST.END_NOTES:
TEST.STUB:sysCore.OS_RdyListInsert
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListDly.TCB_Ptr:<<malloc 1>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListDly.TCB_Ptr[0].TickNextPtr:<<malloc 1>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListDly.TCB_Ptr[0].TaskState:1
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListDly.TCB_Ptr[0].TickRemain:1
TEST.EXPECTED:sysTick.<<GLOBAL>>.OSTickListDly.TCB_Ptr[0].TickNextPtr[0].TickPrevPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListUpdateDly.return:0
TEST.END

-- Test Case: OS_TickListUpdateDLy4
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListUpdateDly
TEST.NEW
TEST.NAME:OS_TickListUpdateDLy4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_tcb != (OS_TCB *)0) ==> TRUE
      (2) while (p_tcb->TickRemain == 0U) ==> TRUE
      (3) if (p_tcb->TaskState == (OS_STATE)1U) ==> FALSE
      (4) if (p_tcb->TaskState == (OS_STATE)5U) ==> FALSE
      (5) if (p_tcb == (OS_TCB *)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set p_tcb->TickRemain due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 5
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 5
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 5
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
TEST.END_NOTES:
TEST.STUB:sysCore.OS_RdyListInsert
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListDly.TCB_Ptr:<<malloc 1>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListDly.TCB_Ptr[0].TickNextPtr:<<malloc 1>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListDly.TCB_Ptr[0].TaskState:5
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListDly.TCB_Ptr[0].TickRemain:1
TEST.EXPECTED:sysTick.<<GLOBAL>>.OSTickListDly.TCB_Ptr[0].TickNextPtr[0].TickPrevPtr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListUpdateDly.return:0
TEST.END

-- Subprogram: OS_TickListUpdateTimeout

-- Test Case: OS_TickListUpdateTImeout1
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListUpdateTimeout
TEST.NEW
TEST.NAME:OS_TickListUpdateTImeout1
TEST.BASIS_PATH:1 of 11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (p_tcb != (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 1
TEST.END_NOTES:
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr:<<null>>
TEST.EXPECTED:sysTick.OS_TickListUpdateTimeout.return:0
TEST.END

-- Test Case: OS_TickListUpdateTimeout2
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListUpdateTimeout
TEST.NEW
TEST.NAME:OS_TickListUpdateTimeout2
TEST.BASIS_PATH:2 of 11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (p_tcb != (OS_TCB *)0) ==> TRUE
      (2) while (p_tcb->TickRemain == 0U) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set p_tcb->TickRemain due to assignment
      Cannot set p_list due to assignment
TEST.END_NOTES:
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr:<<malloc 1>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].TickRemain:10
TEST.EXPECTED:sysTick.OS_TickListUpdateTimeout.return:0
TEST.END

-- Test Case: OS_TickListUpdateTimeout3
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListUpdateTimeout
TEST.NEW
TEST.NAME:OS_TickListUpdateTimeout3
TEST.BASIS_PATH:3 of 11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_tcb != (OS_TCB *)0) ==> TRUE
      (2) while (p_tcb->TickRemain == 0U) ==> TRUE
      (3) if (p_tcb->PendOn == (OS_STATE)4U) ==> FALSE
      (4) if (p_tcb->TaskState == (OS_STATE)3U) ==> FALSE
      (5) if (p_tcb->TaskState == (OS_STATE)7U) ==> FALSE
      (6) if (p_tcb_owner != (OS_TCB *)0) ==> FALSE
      (10) if (p_tcb == (OS_TCB *)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set p_tcb->TickRemain due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 10
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 10
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 10
      Cannot set p_list due to assignment
TEST.END_NOTES:
TEST.STUB:sysCore.OS_PendListRemove
TEST.STUB:sysCore.OS_RdyListInsert
TEST.STUB:sysMutex.OS_MutexGrpPrioFindHighest
TEST.STUB:sysTask.OS_TaskChangePrio
TEST.VALUE:sysTick.<<GLOBAL>>.OSTCBCurPtr:<<malloc 1>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTCBCurPtr[0].Prio:1
TEST.VALUE:sysTick.<<GLOBAL>>.OSTCBCurPtr[0].BasePrio:2
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr:<<malloc 1>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].TickNextPtr:<<null>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].PendDataTblPtr:<<malloc 1>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].PendDataTblPtr[0].PrevPtr:<<null>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].PendDataTblPtr[0].NextPtr:<<null>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].PendDataTblPtr[0].TCBPtr:<<null>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].PendOn:4
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].TaskState:3
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].TickRemain:1
TEST.EXPECTED:sysTick.OS_TickListUpdateTimeout.return:0
TEST.VALUE_USER_CODE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr.TCB_Ptr[0].PendDataTblPtr.PendDataTblPtr[0].PendObjPtr
OS_TCB A;
OS_MUTEX *P,PP;
P= &PP;
P->OwnerTCBPtr=OSTCBCurPtr;
OS_PRIO B=1;
P->OwnerTCBPtr->Prio=B;
P->OwnerTCBPtr->BasePrio=B;
<<sysTick.<<GLOBAL>>.OSTickListTimeout>>.TCB_Ptr[0].PendDataTblPtr[0].PendObjPtr = (P);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OS_TickListUpdateTimeout3.001
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListUpdateTimeout
TEST.NEW
TEST.NAME:OS_TickListUpdateTimeout3.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (p_tcb != (OS_TCB *)0) ==> TRUE
      (2) while (p_tcb->TickRemain == 0U) ==> TRUE
      (3) if (p_tcb->PendOn == (OS_STATE)4U) ==> FALSE
      (4) if (p_tcb->TaskState == (OS_STATE)3U) ==> FALSE
      (5) if (p_tcb->TaskState == (OS_STATE)7U) ==> FALSE
      (6) if (p_tcb_owner != (OS_TCB *)0) ==> FALSE
      (10) if (p_tcb == (OS_TCB *)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set p_tcb->TickRemain due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 10
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 10
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 10
      Cannot set p_list due to assignment
TEST.END_NOTES:
TEST.STUB:sysCore.OS_PendListRemove
TEST.STUB:sysCore.OS_RdyListInsert
TEST.STUB:sysMutex.OS_MutexGrpPrioFindHighest
TEST.STUB:sysTask.OS_TaskChangePrio
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr:<<malloc 1>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].TickNextPtr:<<null>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].PendDataTblPtr:<<malloc 2>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].PendDataTblPtr[0].PrevPtr:<<null>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].PendDataTblPtr[0].NextPtr:<<null>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].PendDataTblPtr[0].TCBPtr:<<null>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].PendOn:4
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].TaskState:3
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].TickRemain:1
TEST.EXPECTED:sysTick.OS_TickListUpdateTimeout.return:0
TEST.VALUE_USER_CODE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr.TCB_Ptr[0].PendDataTblPtr.PendDataTblPtr[0].PendObjPtr
OS_MUTEX *P1,PP;
P1= &PP;
P1->OwnerTCBPtr = OSTCBCurPtr;


<<sysTick.<<GLOBAL>>.OSTickListTimeout>>.TCB_Ptr[0].PendDataTblPtr[0].PendObjPtr = (P1);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: OS_TickListUpdateTimeout4
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickListUpdateTimeout
TEST.NEW
TEST.NAME:OS_TickListUpdateTimeout4
TEST.BASIS_PATH:4 of 11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (p_tcb != (OS_TCB *)0) ==> TRUE
      (2) while (p_tcb->TickRemain == 0U) ==> TRUE
      (3) if (p_tcb->PendOn == (OS_STATE)4U) ==> FALSE
      (4) if (p_tcb->TaskState == (OS_STATE)3U) ==> FALSE
      (5) if (p_tcb->TaskState == (OS_STATE)7U) ==> FALSE
      (6) if (p_tcb_owner != (OS_TCB *)0) ==> FALSE
      (10) if (p_tcb == (OS_TCB *)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 1
      Cannot set p_list due to assignment
      Cannot set p_tcb->TickRemain due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_list due to assignment
      Cannot set p_tcb_owner due to assignment
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 10
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 10
      Cannot set p_list due to assignment
      Cannot set local variable p_list in branch 10
      Cannot set p_list due to assignment
TEST.END_NOTES:
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr:<<malloc 1>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].TickNextPtr:<<malloc 1>>
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].PendOn:0
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].TaskState:7
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickListTimeout.TCB_Ptr[0].TickRemain:1
TEST.EXPECTED:sysTick.OS_TickListUpdateTimeout.return:0
TEST.END

-- Subprogram: OS_TickUpdate

-- Test Case: OS_TickUpdate1
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickUpdate
TEST.NEW
TEST.NAME:OS_TickUpdate1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSTickTaskTimeMax < ts_delta) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysTick.OS_TickListUpdateDly
TEST.STUB:sysTick.OS_TickListUpdateTimeout
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickCtr:0
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickTaskTimeMax:<<MAX>>
TEST.VALUE:sysTick.OS_TickListUpdateDly.return:1
TEST.VALUE:sysTick.OS_TickListUpdateTimeout.return:1
TEST.EXPECTED:sysTick.<<GLOBAL>>.OSTickCtr:1
TEST.END

-- Test Case: OS_TickUpdate2
TEST.UNIT:sysTick
TEST.SUBPROGRAM:OS_TickUpdate
TEST.NEW
TEST.NAME:OS_TickUpdate2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (OSTickTaskTimeMax < ts_delta) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sysTick.OS_TickListUpdateDly
TEST.STUB:sysTick.OS_TickListUpdateTimeout
TEST.VALUE:sysTick.<<GLOBAL>>.OSTickTaskTimeMax:<<MIN>>
TEST.VALUE:sysTick.OS_TickListUpdateDly.return:1
TEST.VALUE:sysTick.OS_TickListUpdateTimeout.return:1
TEST.EXPECTED:sysTick.<<GLOBAL>>.OSTickTaskTimeMax:2
TEST.END
