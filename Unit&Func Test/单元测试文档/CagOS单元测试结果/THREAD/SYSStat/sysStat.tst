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

-- Unit: sysStat

-- Subprogram: OSStatReset

-- Test Case: OSStatReset1
TEST.UNIT:sysStat
TEST.SUBPROGRAM:OSStatReset
TEST.NEW
TEST.NAME:OSStatReset1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while 0 ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
      Only one possible value for condition in branch 2
TEST.END_NOTES:
TEST.VALUE:sysStat.OSStatReset.p_err:<<malloc 1>>
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCPUUsageMax:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskTimeMax:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSTickTaskTimeMax:0
TEST.EXPECTED:sysStat.OSStatReset.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OSStatTaskCPUUsageInit

-- Test Case: OSStatTaskCPUUsageInit1
TEST.UNIT:sysStat
TEST.SUBPROGRAM:OSStatTaskCPUUsageInit
TEST.NEW
TEST.NAME:OSStatTaskCPUUsageInit1
TEST.BASIS_PATH:1 of 8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (err != (OS_ERR_NONE)) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable err in branch 1
TEST.END_NOTES:
TEST.STUB:sysTime.OSTimeDly
TEST.VALUE:sysStat.OSStatTaskCPUUsageInit.p_err:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDly.p_err[0]:OS_ERR_B
TEST.EXPECTED:sysStat.OSStatTaskCPUUsageInit.p_err[0]:OS_ERR_B
TEST.END

-- Test Case: OSStatTaskCPUUsageInit2
TEST.UNIT:sysStat
TEST.SUBPROGRAM:OSStatTaskCPUUsageInit
TEST.NEW
TEST.NAME:OSStatTaskCPUUsageInit2
TEST.BASIS_PATH:2 of 8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (err != (OS_ERR_NONE)) ==> FALSE
      (2) while 0 ==> FALSE
      (4) if (OSCfg_TickRate_Hz > OSCfg_StatTaskRate_Hz) ==> FALSE
      (5) if (dly == (OS_TICK)0) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable err in branch 1
      Only one possible value for condition in branch 2
      Only one possible value for condition in branch 3
      Cannot set const variable TEST.VALUE:sysStat.<<GLOBAL>>.OSCfg_TickRate_Hz: in branch 4
      Cannot set const variable TEST.VALUE:sysStat.<<GLOBAL>>.OSCfg_StatTaskRate_Hz:
      Cannot set dly due to assignment
      Only one possible value for condition in branch 6
      Only one possible value for condition in branch 7
TEST.END_NOTES:
TEST.STUB:sysTime.OSTimeDly
TEST.VALUE:sysStat.OSStatTaskCPUUsageInit.p_err:<<malloc 1>>
TEST.VALUE:sysTime.OSTimeDly.p_err[0]:OS_ERR_NONE
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtr:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtrMax:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskRdy:1
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskTimeMax:0
TEST.EXPECTED:sysStat.OSStatTaskCPUUsageInit.p_err[0]:OS_ERR_NONE
TEST.END

-- Subprogram: OS_StatTask

-- Test Case: OS_StatTask1
TEST.UNIT:sysStat
TEST.SUBPROGRAM:OS_StatTask
TEST.NEW
TEST.NAME:OS_StatTask1
TEST.BASIS_PATH:1 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (OSStatTaskRdy != 1U) ==> FALSE
      (2) if (OSCfg_TickRate_Hz > OSCfg_StatTaskRate_Hz) ==> FALSE
      (3) if (dly == (OS_TICK)0) ==> FALSE
      (4) while 1U ==> FALSE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:sysStat.<<GLOBAL>>.OSCfg_TickRate_Hz: in branch 2
      Cannot set const variable TEST.VALUE:sysStat.<<GLOBAL>>.OSCfg_StatTaskRate_Hz:
      Cannot set dly due to assignment
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.STUB:sysStat.OSStatReset
TEST.STUB:sysTime.OSTimeDly
TEST.VALUE:uut_prototype_stubs.getTblms.return:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatResetFlag:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCPUUsage:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCPUUsageMax:12
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtr:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtrMax:100
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtrRun:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskRdy:1
TEST.VALUE:sysStat.OS_StatTask.p_arg:VECTORCAST_INT1
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatResetFlag:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtr:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtrRun:1
TEST.END

-- Test Case: OS_StatTask2
TEST.UNIT:sysStat
TEST.SUBPROGRAM:OS_StatTask
TEST.NEW
TEST.NAME:OS_StatTask2
TEST.BASIS_PATH:2 of 15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (OSStatTaskRdy != 1U) ==> FALSE
      (2) if (OSCfg_TickRate_Hz > OSCfg_StatTaskRate_Hz) ==> FALSE
      (3) if (dly == (OS_TICK)0) ==> FALSE
      (4) while 1U ==> TRUE
      (5) while 0 ==> FALSE
      (7) if (OSStatTaskCtrMax > OSStatTaskCtrRun) ==> FALSE
      (13) if (OSStatResetFlag == 1U) ==> FALSE
      (14) if (OSStatTaskTimeMax < ts_end) ==> FALSE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:sysStat.<<GLOBAL>>.OSCfg_TickRate_Hz: in branch 2
      Cannot set const variable TEST.VALUE:sysStat.<<GLOBAL>>.OSCfg_StatTaskRate_Hz:
      Cannot set dly due to assignment
      Only one possible value for condition in branch 4
      Only one possible value for condition in branch 5
      Only one possible value for condition in branch 6
      Cannot set OSStatTaskTimeMax due to assignment
      Cannot set ts_end due to assignment
TEST.END_NOTES:
TEST.STUB:sysStat.OSStatReset
TEST.STUB:sysTime.OSTimeDly
TEST.VALUE:uut_prototype_stubs.getTblms.return:2
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatResetFlag:<<MIN>>
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtr:<<MAX>>
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtrMax:<<MIN>>
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtrRun:12
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskRdy:1
TEST.VALUE:sysStat.OS_StatTask.p_arg:VECTORCAST_INT1
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCPUUsage:10000
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtr:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtrRun:<<MAX>>
TEST.END

-- Test Case: OS_StatTask3
TEST.UNIT:sysStat
TEST.SUBPROGRAM:OS_StatTask
TEST.NEW
TEST.NAME:OS_StatTask3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (OSStatTaskRdy != 1U) ==> FALSE
      (2) if (OSCfg_TickRate_Hz > OSCfg_StatTaskRate_Hz) ==> FALSE
      (3) if (dly == (OS_TICK)0) ==> FALSE
      (4) while 1U ==> FALSE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:sysStat.<<GLOBAL>>.OSCfg_TickRate_Hz: in branch 2
      Cannot set const variable TEST.VALUE:sysStat.<<GLOBAL>>.OSCfg_StatTaskRate_Hz:
      Cannot set dly due to assignment
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.STUB:sysStat.OSStatReset
TEST.STUB:sysTime.OSTimeDly
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatResetFlag:2
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCPUUsage:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCPUUsageMax:12
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtr:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtrMax:400000
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtrRun:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskRdy:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskTimeMax:0
TEST.VALUE:sysStat.OS_StatTask.p_arg:VECTORCAST_INT1
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatResetFlag:2
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCPUUsageMax:10000
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtr:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtrRun:1
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskTimeMax:1
TEST.STUB_VAL_USER_CODE:uut_prototype_stubs.getTblms.return
static int a=1;
a=a+1;
<<uut_prototype_stubs.getTblms.return>> = ( a );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: OS_StatTask4
TEST.UNIT:sysStat
TEST.SUBPROGRAM:OS_StatTask
TEST.NEW
TEST.NAME:OS_StatTask4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (OSStatTaskRdy != 1U) ==> FALSE
      (2) if (OSCfg_TickRate_Hz > OSCfg_StatTaskRate_Hz) ==> FALSE
      (3) if (dly == (OS_TICK)0) ==> FALSE
      (4) while 1U ==> FALSE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:sysStat.<<GLOBAL>>.OSCfg_TickRate_Hz: in branch 2
      Cannot set const variable TEST.VALUE:sysStat.<<GLOBAL>>.OSCfg_StatTaskRate_Hz:
      Cannot set dly due to assignment
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.STUB:sysStat.OSStatReset
TEST.STUB:sysTime.OSTimeDly
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatResetFlag:2
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCPUUsage:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCPUUsageMax:12
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtr:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtrMax:4000000
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtrRun:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskRdy:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskTimeMax:0
TEST.VALUE:sysStat.OS_StatTask.p_arg:VECTORCAST_INT1
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatResetFlag:2
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCPUUsageMax:10000
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtr:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtrRun:1
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskTimeMax:1
TEST.STUB_VAL_USER_CODE:uut_prototype_stubs.getTblms.return
static int a=1;
a=a+1;
<<uut_prototype_stubs.getTblms.return>> = ( a );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: OS_StatTask5
TEST.UNIT:sysStat
TEST.SUBPROGRAM:OS_StatTask
TEST.NEW
TEST.NAME:OS_StatTask5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (OSStatTaskRdy != 1U) ==> FALSE
      (2) if (OSCfg_TickRate_Hz > OSCfg_StatTaskRate_Hz) ==> FALSE
      (3) if (dly == (OS_TICK)0) ==> FALSE
      (4) while 1U ==> FALSE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:sysStat.<<GLOBAL>>.OSCfg_TickRate_Hz: in branch 2
      Cannot set const variable TEST.VALUE:sysStat.<<GLOBAL>>.OSCfg_StatTaskRate_Hz:
      Cannot set dly due to assignment
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.STUB:sysStat.OSStatReset
TEST.STUB:sysTime.OSTimeDly
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatResetFlag:2
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCPUUsage:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCPUUsageMax:12
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtr:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtrMax:40000000
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtrRun:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskRdy:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskTimeMax:0
TEST.VALUE:sysStat.OS_StatTask.p_arg:VECTORCAST_INT1
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatResetFlag:2
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCPUUsageMax:10000
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtr:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtrRun:1
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskTimeMax:1
TEST.STUB_VAL_USER_CODE:uut_prototype_stubs.getTblms.return
static int a=1;
a=a+1;
<<uut_prototype_stubs.getTblms.return>> = ( a );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: OS_StatTask6
TEST.UNIT:sysStat
TEST.SUBPROGRAM:OS_StatTask
TEST.NEW
TEST.NAME:OS_StatTask6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (OSStatTaskRdy != 1U) ==> FALSE
      (2) if (OSCfg_TickRate_Hz > OSCfg_StatTaskRate_Hz) ==> FALSE
      (3) if (dly == (OS_TICK)0) ==> FALSE
      (4) while 1U ==> FALSE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:sysStat.<<GLOBAL>>.OSCfg_TickRate_Hz: in branch 2
      Cannot set const variable TEST.VALUE:sysStat.<<GLOBAL>>.OSCfg_StatTaskRate_Hz:
      Cannot set dly due to assignment
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.STUB:sysStat.OSStatReset
TEST.STUB:sysTime.OSTimeDly
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatResetFlag:2
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCPUUsage:<<MAX>>
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCPUUsageMax:<<MAX>>
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtr:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtrMax:4000000000
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskCtrRun:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskRdy:1
TEST.VALUE:sysStat.<<GLOBAL>>.OSStatTaskTimeMax:0
TEST.VALUE:sysStat.OS_StatTask.p_arg:VECTORCAST_INT1
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatResetFlag:2
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCPUUsage:10000
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtr:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtrRun:1
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskTimeMax:1
TEST.STUB_VAL_USER_CODE:uut_prototype_stubs.getTblms.return
static int a=1;
a=a+1;
<<uut_prototype_stubs.getTblms.return>> = ( a );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Subprogram: OS_StatTaskInit

-- Test Case: OS_StatTaskInit1
TEST.UNIT:sysStat
TEST.SUBPROGRAM:OS_StatTaskInit
TEST.NEW
TEST.NAME:OS_StatTaskInit1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (OSCfg_StatTaskStkBasePtr == (CPU_STK *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sysStat.OS_StatTaskInit.p_err:<<malloc 1>>
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatResetFlag:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtr:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtrMax:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskCtrRun:0
TEST.EXPECTED:sysStat.<<GLOBAL>>.OSStatTaskRdy:0
TEST.EXPECTED:sysStat.OS_StatTaskInit.p_err[0]:OS_ERR_STAT_PRIO_INVALID
TEST.END
