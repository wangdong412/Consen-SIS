#include "../inc/common_macro.s"
#include "../inc/common_def.h"
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      Bpu_branch_ba_code
#  Description:    
#       Place code which needs be executed for test ba instruction in specified address 
#  Author:
#  Used module:    None
#  Input Param:    
#       
#  Output Param:   
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .section .text, "ax"
  .globl BPUBranchUnconBaTest
BPUBranchUnconBaTest:
    addi 5, 5, 3
    mfspr 26, 276
    mtlr 26         # move r26 to lr
    blr
 _endBpu_branch_ba_code:
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      Bpu_branch_bla_code
#  Description:    
#       test bla instruction
#  Author:
#  Used module:    None
#  Input Param:    
#      
#  Output Param:   
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .section .text, "ax"
  .globl BPUBranchUnconBlaTest
BPUBranchUnconBlaTest:
    addi 5, 5, 4
    blr
 _endBpu_branch_bla_code:
 
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      BPUBranchUnconBaBlaTest
#  Description:    
#       It carries out the unconditional jump test on the Branch Processing Unit
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
 .section .text, "ax"
 .globl BPUBranchUnconBaBlaTest
BPUBranchUnconBaBlaTest:
     PrologStackOp  #create stack
     StackCheckBegin
     PushParam (3)
     PushParam (4)
     StackCheckEnd
     StoreGPRs2Stack
     StoreCtrXerCr2Stack
     
     mfspr 0, SPRG4
     PushReg (0)
     
     stw 31, OFFSET_PUNT_STACK_CORR(1)
     TestCPUStart
     
     li 5, 1 #load immediate r5 = 1
     bl $+4 
     mflr 25
     addi 25, 25, 16
     mtspr 276, 25
     ba START_ADDRESS_BA_TEST   
     addi 5, 5, 7
     bla START_ADDRESS_BLA_TEST
     addi 5, 5, 9
     cmplwi 5, 24
     bnel FailBPUBranchUnconBaBlaTest
	
     TestConclusion
     b EndBPUBranchUnconBaBlaTest

FailBPUBranchUnconBaBlaTest:
     
     ErrorHandling
     
EndBPUBranchUnconBaBlaTest:

     StackEpilogBegin
     PopReg (0)
     mtspr SPRG4, 0
     RestoreCtrXerCr
     StackEpilogEnd
     
     blr
 .end
                     
