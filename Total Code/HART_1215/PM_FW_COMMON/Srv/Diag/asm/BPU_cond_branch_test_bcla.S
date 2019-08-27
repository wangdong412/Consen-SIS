#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      Bpu_branch_bcla_code
#  Description:    
#       Place code which needs be called in specified address 
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
 .section Bpu_branch_bcla_code, "ax"
      mflr 26
      addi 26, 26, 4
      mtlr 26
      blr
 _endBpu_branch_bcla_code:


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      brcondstr_bcla
#  Description:      
#       Instruction block implementing the conditional jump
#  Author:
#  Used module:    None
#  Input Param:    
#       command:Instruction on conditional jump to be tested           
#       bo:Jump BO field          
#       bi:Jump BI field           
#       lr_flag:Conditional jump to the address contained in LR 0
#  Output Param:       
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define brcondstr_bcla(command, bo, bi, lrflag) \
    .if lrflag ;\
        bl $+4 ;\
        mflr 8 ;\
        addi 8, 8, 20 ;\
        mtlr 8 ;\
        command bo, bi ;\
    .else ;\
        command bo, bi, START_ADDRESS_BCLA_TEST ;\
    .endif ;\
    bl FailBPUConBranchBclaTest


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      brcond_part1
#  Description:    
#       integrated diagnosing absolutly conditional jump Branch instruction which              
#       will place the address following this instruction into the LR
#  Author:
#  Used module:    None
#  Input Param:    
#       command:conditional jump  instruction            
#       bo:jump region R0            
#       bi:jump region R1            
#       lr_flag: LR flag
#  Output Param:     
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define brcond_part1(command, bi, lrflag) \
    li 5, 3 ;\
    mtctr 5 ;\
    brcondstr_bcla(command, 0, bi, lrflag) ;\
    brcondstr_bcla(command, 1, bi, lrflag) ;\
    li 5, 1 ;\
    mtctr 5 ;\
    brcondstr_bcla(command, 2, bi, lrflag) ;\
    mtctr 5 ;\
    brcondstr_bcla(command, 3, bi, lrflag) ;\
    brcondstr_bcla(command, 4, bi, lrflag) ;\
    brcondstr_bcla(command, 5, bi, lrflag)


 #*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      brcond_part2
#  Description:     
#        integrated diagnosing absolutly conditional jump Branch instruction which           
#        will place the address following this instruction into the LR 
#  Author:
#  Used module:    None
#  Input Param:    
#        command:conditional jump  instruction          
#        bo:jump region R0          
#        bi:jump region R1         
#        lr_flag: LR flag
#  Output Param:   
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define brcond_part2(command, bi, lrflag) \
    brcondstr_bcla(command, 8, bi, lrflag) ;\
    brcondstr_bcla(command, 9, bi, lrflag) ;\
    li 5, 1 ;\
    mtctr 5 ;\
    brcondstr_bcla(command, 10, bi, lrflag) ;\
    mtctr 5 ;\
    brcondstr_bcla(command, 11, bi, lrflag) ;\
    brcondstr_bcla(command, 12, bi, lrflag) ;\
    brcondstr_bcla(command, 13, bi, lrflag) ;\
    li 5, 4 ;\
    mtctr 5 ;\
    brcondstr_bcla(command, 16, bi, lrflag)

 
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      brcond
#  Description:    
#       test absolutly conditional jump Branch instruction which             
#       will place the address following this instruction into the LR 
#  Author:
#  Used module:    None
#  Input Param:    
#       command:Instruction on conditional jump to be tested         
#       bo:Jump BO field           
#       bi:Jump BI field           
#       lr_flag:Conditional jump to the address contained in LR 0
#  Output Param:     
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define brcond(command, lrflag) \
    lis 10, 0x1248 ;\
    ori 10, 10, 0x1248 ;\
    mtcr 10 ;\
    brcond_part1(command, 0,  lrflag) ;\
    brcond_part1(command, 5,  lrflag) ;\
    brcond_part1(command, 10, lrflag) ;\
    brcond_part1(command, 15, lrflag) ;\
    brcond_part1(command, 16, lrflag) ;\
    brcond_part1(command, 21, lrflag) ;\
    brcond_part1(command, 31, lrflag) ;\
    brcond_part1(command, 18, lrflag) ;\
    brcond_part1(command, 20, lrflag) ;\
    brcond_part1(command, 26, lrflag) ;\
    brcond_part1(command, 1,  lrflag) ;\
    li 5, 3 ;\
    mtctr 5 ;\
    brcond_part2(command, 3, lrflag) ;\
    brcond_part2(command, 6, lrflag) ;\
    brcond_part2(command, 9, lrflag) ;\
    brcond_part2(command, 12, lrflag) ;\
    brcond_part2(command, 19, lrflag) ;\
    brcond_part2(command, 22, lrflag) ;\
    brcond_part2(command, 25, lrflag) ;\
    brcond_part2(command, 28, lrflag) ;\
    brcondstr_bcla(command, 17, 0, lrflag) ;\
    li 5, 1 ;\
    mtctr 5 ;\
    brcondstr_bcla(command, 18, 0, lrflag) ;\
    mtctr 5 ;\
    brcondstr_bcla(command, 19, 0, lrflag) ;\
    brcondstr_bcla(command, 20, 0, lrflag)

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      BPUConBranchBclaTest
#  Description:     
#       It carries out the Branch Processing Unit (BPU) test
#  Author:
#  Used module:    None
#  Input Param:
#		GPR3:Test outcome vector
#		GPR4:Mask (OR) to be applied to the check vector in case of exit without errors
#		GPR0:Address of calling function
#		GPR2:Address of error memory area
#		GPR12:Mask used for setting to 0 bit 29 of HID0
#		GPR15:Swap for modification of HID0      
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .section Bpu_branch_bcla_code_ext, "ax"
 .globl BPUConBranchBclaTest
BPUConBranchBclaTest:

     PrologStackOp
     StackCheckBegin
     PushParam (3)
     PushParam (4)
     StackCheckEnd
     StoreGPRs2Stack
     StoreCtrXerCr2Stack
     mfspr 0, HID0
     PushReg (0)
     stw 31, OFFSET_PUNT_STACK_CORR(1)
     TestCPUStart

     lis 12, 0xFFFF
     ori 12, 12, 0xFFFB
     mfspr 15, HID0
     and 15, 15, 12
     mtspr HID0, 15
     isync

     brcond(bcla, 0)
     TestConclusion
     b EndBPUConBranchBclaTest

FailBPUConBranchBclaTest:
     ErrorHandling
EndBPUConBranchBclaTest:

     StackEpilogBegin

     PopReg (0)
     sync
     mtspr HID0, 0
     sync
     isync
     RestoreCtrXerCr
     StackEpilogEnd
     blr
 .end
