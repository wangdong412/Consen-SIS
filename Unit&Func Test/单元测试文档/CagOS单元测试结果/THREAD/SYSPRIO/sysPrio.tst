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

-- Unit: sysPrio

-- Subprogram: OS_PrioGetHighest

-- Test Case: OS_PrioGetHighest.001
TEST.UNIT:sysPrio
TEST.SUBPROGRAM:OS_PrioGetHighest
TEST.NEW
TEST.NAME:OS_PrioGetHighest.001
TEST.VALUE:uut_prototype_stubs.CPU_CntLeadZeros.return:2
TEST.VALUE:sysPrio.<<GLOBAL>>.OSPrioTbl[0]:0
TEST.EXPECTED:sysPrio.OS_PrioGetHighest.return:34
TEST.END

-- Test Case: OS_PrioGetHighest1
TEST.UNIT:sysPrio
TEST.SUBPROGRAM:OS_PrioGetHighest
TEST.NEW
TEST.NAME:OS_PrioGetHighest1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (*p_tbl == (CPU_DATA)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set p_tbl due to assignment
      Cannot set local variable p_tbl in branch 1
      Cannot set local variable p_tbl in branch 2
TEST.END_NOTES:
TEST.VALUE:uut_prototype_stubs.CPU_CntLeadZeros.return:1
TEST.VALUE:sysPrio.<<GLOBAL>>.OSPrioTbl[0]:1
TEST.EXPECTED:sysPrio.OS_PrioGetHighest.return:1
TEST.END

-- Subprogram: OS_PrioInit

-- Test Case: OS_PrioINIT
TEST.UNIT:sysPrio
TEST.SUBPROGRAM:OS_PrioInit
TEST.NEW
TEST.NAME:OS_PrioINIT
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for (i < (32U - 1U) / (4U * 8U) + 1U) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
TEST.END_NOTES:
TEST.EXPECTED:sysPrio.<<GLOBAL>>.OSPrioTbl[0]:0
TEST.END

-- Subprogram: OS_PrioInsert

-- Test Case: OS_PrioInsert
TEST.UNIT:sysPrio
TEST.SUBPROGRAM:OS_PrioInsert
TEST.NEW
TEST.NAME:OS_PrioInsert
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sysPrio.OS_PrioInsert.prio:<<MIN>>
TEST.EXPECTED:sysPrio.<<GLOBAL>>.OSPrioTbl[0]:2147483648
TEST.END

-- Subprogram: OS_PrioRemove

-- Test Case: OS_PrioRemove
TEST.UNIT:sysPrio
TEST.SUBPROGRAM:OS_PrioRemove
TEST.NEW
TEST.NAME:OS_PrioRemove
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sysPrio.<<GLOBAL>>.OSPrioTbl[0]:0
TEST.VALUE:sysPrio.OS_PrioRemove.prio:<<MIN>>
TEST.EXPECTED:sysPrio.<<GLOBAL>>.OSPrioTbl[0]:0
TEST.END
