#include "../inc/common_macro.s"
#include "../inc/common_def.h"


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      Bpu_branch_bca_code
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
 .section Bpu_branch_bca_code, "ax"
          mfspr 26, 276
          mtlr 26
          blr
_endBpu_branch_bca_code:

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      brcondstr_bca
#  Description:    
#       Instruction block implementing the conditional jump
#  Author:
#  Used module:    None
#  Input Param:    
#       command:Instruction on conditional jump to be tested         
#       bo:Jump BO field           
#       bi:Jump BI field          
#       lr_flag: Conditional jump to the address contained in LR 0	Other types of jump
#  Output Param:   
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define brcondstr_bca(command, bo, bi, lr_flag) \
        .if lr_flag ;\
            bl $+4 ;\
            mflr 8 ;\
            addi 8, 8, 20 ;\
            mtlr 8 ;\
            command bo, bi ;\
        .else ;\
            bl $+4 ;\
            mflr 25 ;\
            addi 25, 25, 20 ;\
            mtspr 276, 25 ;\
            command bo, bi, START_ADDRESS_BCA_TEST ;\
        .endif ;\
         bl FailBUPConBrandBcaTest

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      brcond
#  Description:    
#       test absolutly conditional jump Branch instruction
#  Author:
#  Used module:    None
#  Input Param: 
#		command:Instruction on conditional jump to be tested
#       lr_flag:Conditional jump to the address contained in LR 0
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define brcond(command, lr_flag) \
        lis 10, 0x1248 ;\
        ori 10, 10, 0x1248 ;\
        mtcr 10 ;\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 0, 0, lr_flag) ;\
        brcondstr_bca(command, 1, 0, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 2, 0, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 3, 0, lr_flag) ;\
        brcondstr_bca(command, 4, 0, lr_flag) ;\
        brcondstr_bca(command, 5, 0, lr_flag) ;\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 0, 5, lr_flag) ;\
        brcondstr_bca(command, 1, 5, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 2, 5, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 3, 5, lr_flag) ;\
        brcondstr_bca(command, 4, 5, lr_flag) ;\
        brcondstr_bca(command, 5, 5, lr_flag) ;\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 0, 10, lr_flag) ;\
        brcondstr_bca(command, 1, 10, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 2, 10, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 3, 10, lr_flag) ;\
        brcondstr_bca(command, 4, 10, lr_flag) ;\
        brcondstr_bca(command, 5, 10, lr_flag) ;\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 0, 15, lr_flag) ;\
        brcondstr_bca(command, 1, 15, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 2, 15, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 3, 15, lr_flag) ;\
        brcondstr_bca(command, 4, 15, lr_flag) ;\
        brcondstr_bca(command, 5, 15, lr_flag) ;\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 0, 16, lr_flag) ;\
        brcondstr_bca(command, 1, 16, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 2, 16, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 3, 16, lr_flag) ;\
        brcondstr_bca(command, 4, 16, lr_flag) ;\
        brcondstr_bca(command, 5, 16, lr_flag) ;\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 0, 21, lr_flag) ;\
        brcondstr_bca(command, 1, 21, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 2, 21, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 3, 21, lr_flag) ;\
        brcondstr_bca(command, 4, 21, lr_flag) ;\
        brcondstr_bca(command, 5, 21, lr_flag) ;\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 0, 31, lr_flag) ;\
        brcondstr_bca(command, 1, 31, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 2, 31, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 3, 31, lr_flag) ;\
        brcondstr_bca(command, 4, 31, lr_flag) ;\
        brcondstr_bca(command, 5, 31, lr_flag) ;\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 0, 18, lr_flag) ;\
        brcondstr_bca(command, 1, 18, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 2, 18, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 3, 18, lr_flag) ;\
        brcondstr_bca(command, 4, 18, lr_flag) ;\
        brcondstr_bca(command, 5, 18, lr_flag) ;\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 0, 20, lr_flag) ;\
        brcondstr_bca(command, 1, 20, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 2, 20, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 3, 20, lr_flag) ;\
        brcondstr_bca(command, 4, 20, lr_flag) ;\
        brcondstr_bca(command, 5, 20, lr_flag) ;\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 0, 26, lr_flag) ;\
        brcondstr_bca(command, 1, 26, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 2, 26, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 3, 26, lr_flag) ;\
        brcondstr_bca(command, 4, 26, lr_flag) ;\
        brcondstr_bca(command, 5, 26, lr_flag) ;\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 0, 1, lr_flag) ;\
        brcondstr_bca(command, 1, 1, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 2, 1, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 3, 1, lr_flag) ;\
        brcondstr_bca(command, 4, 1, lr_flag) ;\
        brcondstr_bca(command, 5, 1, lr_flag) ;\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 8, 3, lr_flag) ;\
        brcondstr_bca(command, 9, 3, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 10, 3, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 11, 3, lr_flag) ;\
        brcondstr_bca(command, 12, 3, lr_flag) ;\
        brcondstr_bca(command, 13, 3, lr_flag) ;\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 16, 3, lr_flag) ;\
        brcondstr_bca(command, 8, 6, lr_flag) ;\
        brcondstr_bca(command, 9, 6, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 10, 6, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 11, 6, lr_flag) ;\
        brcondstr_bca(command, 12, 6, lr_flag) ;\
        brcondstr_bca(command, 13, 6, lr_flag) ;\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 16, 6, lr_flag) ;\
        brcondstr_bca(command, 8, 9, lr_flag) ;\
        brcondstr_bca(command, 9, 9, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 10, 9, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 11, 9, lr_flag) ;\
        brcondstr_bca(command, 12, 9, lr_flag) ;\
        brcondstr_bca(command, 13, 9, lr_flag) ;\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 16, 9, lr_flag) ;\
        brcondstr_bca(command, 8, 12, lr_flag) ;\
        brcondstr_bca(command, 9, 12, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 10, 12, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 11, 12, lr_flag) ;\
        brcondstr_bca(command, 12, 12, lr_flag) ;\
        brcondstr_bca(command, 13, 12, lr_flag) ;\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 16, 12, lr_flag) ;\
        brcondstr_bca(command, 8, 19, lr_flag) ;\
        brcondstr_bca(command, 9, 19, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 10, 19, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 11, 19, lr_flag) ;\
        brcondstr_bca(command, 12, 19, lr_flag) ;\
        brcondstr_bca(command, 13, 19, lr_flag) ;\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 16, 19, lr_flag) ;\
        brcondstr_bca(command, 8, 22, lr_flag) ;\
        brcondstr_bca(command, 9, 22, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 10, 22, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 11, 22, lr_flag) ;\
        brcondstr_bca(command, 12, 22, lr_flag) ;\
        brcondstr_bca(command, 13, 22, lr_flag) ;\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 16, 22, lr_flag) ;\
        brcondstr_bca(command, 8, 25, lr_flag) ;\
        brcondstr_bca(command, 9, 25, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 10, 25, lr_flag) ;\
        mtctr 5;\
        brcondstr_bca(command, 11, 25, lr_flag) ;\
        brcondstr_bca(command, 12, 25, lr_flag) ;\
        brcondstr_bca(command, 13, 25, lr_flag) ;\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 16, 25, lr_flag) ;\
        brcondstr_bca(command, 8, 28, lr_flag) ;\
        brcondstr_bca(command, 9, 28, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 10, 28, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 11, 28, lr_flag) ;\
        brcondstr_bca(command, 12, 28, lr_flag) ;\
        brcondstr_bca(command, 13, 28, lr_flag) ;\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 16, 28, lr_flag) ;\
        brcondstr_bca(command, 17, 0, lr_flag) ;\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bca(command, 18, 0, lr_flag) ;\
        mtctr 5 ;\
        brcondstr_bca(command, 19, 0, lr_flag) ;\
        brcondstr_bca(command, 20, 0, lr_flag)
 
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      BPUConBranchBcaTest
#  Description:    
#       external interface function ,It carries out the Branch Processing Unit (BPU) test.
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
 .section Bpu_branch_bca_code_ext, "ax"
 .global BPUConBranchBcaTest
BPUConBranchBcaTest:
             
         PrologStackOp
         StackCheckBegin
         PushParam(3)
         PushParam(4)

         StackCheckEnd
         StoreGPRs2Stack
         StoreCtrXerCr2Stack

         mfspr 0, HID0
         PushReg(0)
         mfspr 0, SPRG4
         PushReg(0)

         stw 31, OFFSET_PUNT_STACK_CORR(1)
         TestCPUStart

         lis 12, 0xFFFF
         ori 12, 12, 0xFFFB
         mfspr 15, HID0
         and 15, 15, 12
         mtspr HID0, 15
         isync
         
         brcond(bca, 0)

         TestConclusion
         b EndBUPConBrandBcaTest

FailBUPConBrandBcaTest:
             
         ErrorHandling

EndBUPConBrandBcaTest:

         StackEpilogBegin
         PopReg(0)
         mtspr SPRG4, 0
         PopReg(0)
         sync
         mtspr HID0, 0
         sync
         isync
         RestoreCtrXerCr
         StackEpilogEnd
         blr
 .end

