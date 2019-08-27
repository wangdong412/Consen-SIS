#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      brcondstr_bc
#  Description:    
#       Instruction block implementing the conditional jump
#  Author:          
#  Used module:    None
#  Input Param:    
#       command:jump instruction 
#       bo:jump b0 field
#       bi:jump b1 field
#       lrflag);\:switch flag
#      
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define brcondstr_bc(command, bo, bi, lrflag) \
        .if lrflag ;\
            bl $+4 ;\
            mflr 8 ;\
            addi 8, 8, 20 ;\
            mtlr 8 ;\
            command bo, bi ;\
        .else ;\
            command bo, bi, $+8 ;\
        .endif ;\
        bl FailBPUConBranchTest


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      brcondstr_brctr
#  Description:    
#       Instruction block implementing the conditional jump to an address contained in CTR.
#  Author:          
#  Used module:    None
#  Input Param:    
#       command:jump instruction 
#       bo:jump b0 field
#       bi:jump b1 field
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define brcondstr_brctr(command, bo, bi) \
        bl $+4 ;\
        mflr 8 ;\
        addi 8, 8, 20 ;\
        mtctr 8 ;\
        command bo, bi ;\
        bl FailBPUConBranchTest


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      brcond
#  Description:    
#       Test of conditional jumps excluding those to an address contained in CTR
#  Author:          
#  Used module:    None
#  Input Param:    
#       command: jump instruction
#       lrflag:Conditional jump to address contained in LR 0.
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define brcond(command,lrflag) \
        lis 10, 0x1248 ;\
        ori 10, 10, 0x1248 ;\
        mtcr 10 ;\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 0, 0, lrflag);\
        brcondstr_bc(command, 1, 0, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 2, 0, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 3, 0, lrflag);\
        brcondstr_bc(command, 4, 0, lrflag);\
        brcondstr_bc(command, 5, 0, lrflag);\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 0, 5, lrflag);\
        brcondstr_bc(command, 1, 5, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 2, 5, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 3, 5, lrflag);\
        brcondstr_bc(command, 4, 5, lrflag);\
        brcondstr_bc(command, 5, 5, lrflag);\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 0, 10, lrflag);\
        brcondstr_bc(command, 1, 10, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 2, 10, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 3, 10, lrflag);\
        brcondstr_bc(command, 4, 10, lrflag);\
        brcondstr_bc(command, 5, 10, lrflag);\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 0, 15, lrflag);\
        brcondstr_bc(command, 1, 15, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 2, 15, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 3, 15, lrflag);\
        brcondstr_bc(command, 4, 15, lrflag);\
        brcondstr_bc(command, 5, 15, lrflag);\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 0, 16, lrflag);\
        brcondstr_bc(command, 1, 16, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 2, 16, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 3, 16, lrflag);\
        brcondstr_bc(command, 4, 16, lrflag);\
        brcondstr_bc(command, 5, 16, lrflag);\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 0, 21, lrflag);\
        brcondstr_bc(command, 1, 21, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 2, 21, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 3, 21, lrflag);\
        brcondstr_bc(command, 4, 21, lrflag);\
        brcondstr_bc(command, 5, 21, lrflag);\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 0, 31, lrflag);\
        brcondstr_bc(command, 1, 31, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 2, 31, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 3, 31, lrflag);\
        brcondstr_bc(command, 4, 31, lrflag);\
        brcondstr_bc(command, 5, 31, lrflag);\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 0, 18, lrflag);\
        brcondstr_bc(command, 1, 18, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 2, 18, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 3, 18, lrflag);\
        brcondstr_bc(command, 4, 18, lrflag);\
        brcondstr_bc(command, 5, 18, lrflag);\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 0, 20, lrflag);\
        brcondstr_bc(command, 1, 20, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 2, 20, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 3, 20, lrflag);\
        brcondstr_bc(command, 4, 20, lrflag);\
        brcondstr_bc(command, 5, 20, lrflag);\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 0, 26, lrflag);\
        brcondstr_bc(command, 1, 26, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 2, 26, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 3, 26, lrflag);\
        brcondstr_bc(command, 4, 26, lrflag);\
        brcondstr_bc(command, 5, 26, lrflag);\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 0, 1, lrflag);\
        brcondstr_bc(command, 1, 1, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 2, 1, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 3, 1, lrflag);\
        brcondstr_bc(command, 4, 1, lrflag);\
        brcondstr_bc(command, 5, 1, lrflag);\
        li 5, 3 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 8, 3, lrflag);\
        brcondstr_bc(command, 9, 3, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 10, 3, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 11, 3, lrflag);\
        brcondstr_bc(command, 12, 3, lrflag);\
        brcondstr_bc(command, 13, 3, lrflag);\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 16, 3, lrflag);\
        brcondstr_bc(command, 8, 6, lrflag);\
        brcondstr_bc(command, 9, 6, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 10, 6, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 11, 6, lrflag);\
        brcondstr_bc(command, 12, 6, lrflag);\
        brcondstr_bc(command, 13, 6, lrflag);\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 16, 6, lrflag);\
        brcondstr_bc(command, 8, 9, lrflag);\
        brcondstr_bc(command, 9, 9, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 10, 9, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 11, 9, lrflag);\
        brcondstr_bc(command, 12, 9, lrflag);\
        brcondstr_bc(command, 13, 9, lrflag);\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 16, 9, lrflag);\
        brcondstr_bc(command, 8, 12, lrflag);\
        brcondstr_bc(command, 9, 12, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 10, 12, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 11, 12, lrflag);\
        brcondstr_bc(command, 12, 12, lrflag);\
        brcondstr_bc(command, 13, 12, lrflag);\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 16, 12, lrflag);\
        brcondstr_bc(command, 8, 19, lrflag);\
        brcondstr_bc(command, 9, 19, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 10, 19, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 11, 19, lrflag);\
        brcondstr_bc(command, 12, 19, lrflag);\
        brcondstr_bc(command, 13, 19, lrflag);\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 16, 19, lrflag);\
        brcondstr_bc(command, 8, 22, lrflag);\
        brcondstr_bc(command, 9, 22, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 10, 22, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 11, 22, lrflag);\
        brcondstr_bc(command, 12, 22, lrflag);\
        brcondstr_bc(command, 13, 22, lrflag);\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 16, 22, lrflag);\
        brcondstr_bc(command, 8, 25, lrflag);\
        brcondstr_bc(command, 9, 25, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 10, 25, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 11, 25, lrflag);\
        brcondstr_bc(command, 12, 25, lrflag);\
        brcondstr_bc(command, 13, 25, lrflag);\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 16, 25, lrflag);\
        brcondstr_bc(command, 8, 28, lrflag);\
        brcondstr_bc(command, 9, 28, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 10, 28, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 11, 28, lrflag);\
        brcondstr_bc(command, 12, 28, lrflag);\
        brcondstr_bc(command, 13, 28, lrflag);\
        li 5, 4 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 16, 28, lrflag);\
        brcondstr_bc(command, 17, 0, lrflag);\
        li 5, 1 ;\
        mtctr 5 ;\
        brcondstr_bc(command, 18, 0, lrflag);\
        mtctr 5 ;\
        brcondstr_bc(command, 19, 0, lrflag);\
        brcondstr_bc(command, 20, 0, lrflag)

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      brctr
#  Description:    
#       Test of conditional jumps to an address contained in CTR
#  Author:          
#  Used module:    None
#  Input Param:    
#       command: jump instruction
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
#define brctr(command) \
        lis 10, 0x1248 ;\
        ori 10, 10, 0x1248 ;\
        mtcr 10 ;\
        brcondstr_brctr(command, 4, 2);\
        brcondstr_brctr(command, 5, 2);\
        brcondstr_brctr(command, 4, 4);\
        brcondstr_brctr(command, 5, 4);\
        brcondstr_brctr(command, 4, 7);\
        brcondstr_brctr(command, 5, 7);\
        brcondstr_brctr(command, 4, 8);\
        brcondstr_brctr(command, 5, 8);\
        brcondstr_brctr(command, 4, 11);\
        brcondstr_brctr(command, 5, 11);\
        brcondstr_brctr(command, 4, 13);\
        brcondstr_brctr(command, 5, 13);\
        brcondstr_brctr(command, 4, 14);\
        brcondstr_brctr(command, 5, 14);\
        brcondstr_brctr(command, 4, 23);\
        brcondstr_brctr(command, 5, 23);\
        brcondstr_brctr(command, 4, 27);\
        brcondstr_brctr(command, 5, 27);\
        brcondstr_brctr(command, 4, 17);\
        brcondstr_brctr(command, 5, 17);\
        brcondstr_brctr(command, 4, 24);\
        brcondstr_brctr(command, 5, 24);\
        brcondstr_brctr(command, 4, 29);\
        brcondstr_brctr(command, 5, 29);\
        brcondstr_brctr(command, 4, 30);\
        brcondstr_brctr(command, 5, 30);\
        brcondstr_brctr(command, 12, 3);\
        brcondstr_brctr(command, 13, 3);\
        brcondstr_brctr(command, 12, 6);\
        brcondstr_brctr(command, 13, 6);\
        brcondstr_brctr(command, 12, 9);\
        brcondstr_brctr(command, 13, 9);\
        brcondstr_brctr(command, 12, 12);\
        brcondstr_brctr(command, 13, 12);\
        brcondstr_brctr(command, 12, 19);\
        brcondstr_brctr(command, 13, 19);\
        brcondstr_brctr(command, 12, 22);\
        brcondstr_brctr(command, 13, 22);\
        brcondstr_brctr(command, 12, 25);\
        brcondstr_brctr(command, 13, 25);\
        brcondstr_brctr(command, 12, 28);\
        brcondstr_brctr(command, 13, 28);\
        brcondstr_brctr(command, 20, 0);


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      BPUConBranchTest
#  Description:    
#       It carries out the Branch Processing Unit (BPU) test.
#  Author:          
#  Used module:    None
#  Input Param:    
#        GPR3:Test outcome vector
#        GPR4:Mask (OR) to be applied to the check vector in case of exit without errors
#        GPR0:Address of calling function
#        GPR2:Address of error memory area
#        GPR12:Mask used for setting to 0 bit 29 of HID0
#        GPR15:Swap for modification of HID0
#  Output Param:   
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .globl BPUConBranchTest
BPUConBranchTest:
             
         PrologStackOp
         StackCheckBegin
         PushParam(3)
         PushParam(4)

         StackCheckEnd
         StoreGPRs2Stack
         StoreCtrXerCr2Stack

         mfspr 0, HID0
         PushReg(0)
         stw 31, OFFSET_PUNT_STACK_CORR(1)
         TestCPUStart

         lis 12, 0xFFFF
         ori 12, 12, 0xFFFB
         mfspr 15, HID0
         and 15, 15, 12
         mtspr HID0, 15
         isync
         
         brcond(bc,0)
         brcond(bcl,0)
         brcond(bclr,1)
         brcond(bclrl,1)
         brctr(bcctr)
         brctr(bcctrl)

         TestConclusion
         b EndBPUConBranchTest

FailBPUConBranchTest:
             
         ErrorHandling

EndBPUConBranchTest:

         StackEpilogBegin
         PopReg(0)
         sync
         mtspr HID0, 0
         sync
         isync
         RestoreCtrXerCr
         StackEpilogEnd
         blr
 .end
