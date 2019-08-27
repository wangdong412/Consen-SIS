#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      BPUBranchUnconTest
#  Description:    
#       It carries out the unconditional jump test on the Branch Processing Unit它
#  Author:
#  Used module:    None
#  Input Param:
#       GPR3:Test outcome vector
#		GPR4:Mask (OR) to be applied to the check vector in case of exit without errors
#		GPR0:Address of calling function
#		GPR2:Address of error memory area
#		GPR5:Counter
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl BPUBranchUnconTest
 
BPUBranchUnconTest:
        
      PrologStackOp
      StackCheckBegin
      
      PushParam(3)
      PushParam(4)
      StackCheckEnd
      StoreGPRs2Stack
      StoreCtrXerCr2Stack

      stw 31, OFFSET_PUNT_STACK_CORR(1)
      TestCPUStart

      li 5, 1
      b ok_b
      bl FailBPUBranchUnconTest
ok_b:
      addi 5, 5, 3
      bl ok_bl
      bl FailBPUBranchUnconTest
ok_bl:
      cmplwi 5, 0x0004
      bnel FailBPUBranchUnconTest

      TestConclusion
      b EndBPUBranchUnconTest

FailBPUBranchUnconTest:

      ErrorHandling

EndBPUBranchUnconTest:

      StackEpilogBegin
      RestoreCtrXerCr
      StackEpilogEnd

      blr
 .end
