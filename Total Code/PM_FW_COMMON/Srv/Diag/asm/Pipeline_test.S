#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      war_part
#  Description:     
#       set special value to register ,then execute mul and add operation,            
#       check the operation conclusion.
#  Author:
#  Used module:    None
#  Input Param:    InitVal_0-InitVal_4:register 
#       
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define war_part(InitVal_0, InitVal_1, InitVal_2, InitVal_3, InitVal_4) \
        li InitVal_0, 8 ;\
        li InitVal_2, 3 ;\
        li InitVal_3, 5 ;\
        li InitVal_4, 7 ;\
        mullw InitVal_1, InitVal_0, InitVal_2 ;\
        add InitVal_0, InitVal_3, InitVal_4 ;\
        cmpwi InitVal_1, 24 ;\
        bnel FailPipelineTest


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      waw_part
#  Description:     
#       set special value to register ,then execute mul and add operation,           
#       check the operation conclusion.
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
#define waw_part(InitVal_0, InitVal_1, InitVal_2, InitVal_3, InitVal_4) \
        li InitVal_1, 8 ;\
        li InitVal_2, 3 ;\
        li InitVal_3, 5 ;\
        li InitVal_4, 7 ;\
        mullw InitVal_0, InitVal_1, InitVal_2 ;\
        add InitVal_0, InitVal_3, InitVal_4 ;\
        cmpwi InitVal_0, 12 ;\
        bnel FailPipelineTest


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      raw_part
#  Description:     
#       set special value to register ,then execute mul and add operation,            
#       check the operation conclusion.
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
#define raw_part(InitVal_0, InitVal_1, InitVal_2, InitVal_3, InitVal_4) \
        li InitVal_0, 1 ;\
        li InitVal_2, 7 ;\
        li InitVal_3, 5 ;\
        li InitVal_4, 3 ;\
        mullw InitVal_0, InitVal_4, InitVal_3 ;\
        add InitVal_1, InitVal_0, InitVal_2 ;\
        cmpwi InitVal_1, 22 ;\
        bnel FailPipelineTest

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      war
#  Description:    
#       call war_part with different register bank
#  Author:
#  Used module:    None
#  Input Param:
#       GPR0-GPR31:Victim registers
#		GPR[v +/- 1,2,3,4]:Aggressor registers
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
#define wartest         war_part (0,1,2,3,4) ;\
        war_part (1,2,3,4,5) ;\
        war_part (2,3,4,5,6) ;\
        war_part (3,4,5,6,7) ;\
        war_part (4,5,6,7,8) ;\
        war_part (5,6,7,8,9) ;\
        war_part (6,7,8,9,10) ;\
        war_part (7,8,9,10,11) ;\
        war_part (8,9,10,11,12) ;\
        war_part (9,10,11,12,13) ;\
        war_part (10,11,12,13,14) ;\
        war_part (11,12,13,14,15) ;\
        war_part (12,13,14,15,16) ;\
        war_part (13,14,15,16,17) ;\
        war_part (14,15,16,17,18) ;\
        war_part (15,16,17,18,19) ;\
        war_part (16,17,18,19,20) ;\
        war_part (17,18,19,20,21) ;\
        war_part (18,19,20,21,22) ;\
        war_part (19,20,21,22,23) ;\
        war_part (20,21,22,23,24) ;\
        war_part (21,22,23,24,25) ;\
        war_part (22,23,24,25,26) ;\
        war_part (23,24,25,26,27) ;\
        war_part (24,25,26,27,28) ;\
        war_part (25,26,27,28,29) ;\
        war_part (26,27,28,29,30) ;\
        war_part (27,28,29,30,31) ;\
        war_part (28,27,26,25,24) ;\
        war_part (29,28,27,26,25) ;\
        war_part (30,29,28,27,26) ;\
        war_part (31,30,29,28,27) 
 
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      waw
#  Description:    
#       call waw_part with different register bank
#  Author:
#  Used module:    None
#  Input Param:
#       GPR0-GPR31:Victim registers
#		GPR[v +/- 12,13,14,15]:Aggressor registers
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
#define wawtest         waw_part (0,12,13,14,15) ;\
        waw_part (1,13,14,15,16) ;\
        waw_part (2,14,15,16,17) ;\
        waw_part (3,15,16,17,18) ;\
        waw_part (4,16,17,18,19) ;\
        waw_part (5,17,18,19,20) ;\
        waw_part (6,18,19,20,21) ;\
        waw_part (7,19,20,21,22) ;\
        waw_part (8,20,21,22,23) ;\
        waw_part (9,21,22,23,24) ;\
        waw_part (10,22,23,24,25) ;\
        waw_part (11,23,24,25,26) ;\
        waw_part (12,24,25,26,27) ;\
        waw_part (13,25,26,27,28) ;\
        waw_part (14,26,27,28,29) ;\
        waw_part (15,27,28,29,30) ;\
        waw_part (16,28,29,30,31) ;\
        waw_part (17,5,4,3,2) ;\
        waw_part (18,6,5,4,3) ;\
        waw_part (19,7,6,5,4) ;\
        waw_part (20,8,7,6,5) ;\
        waw_part (21,9,8,7,6) ;\
        waw_part (22,10,9,8,7) ;\
        waw_part (23,11,10,9,8) ;\
        waw_part (24,12,11,10,9) ;\
        waw_part (25,13,12,11,10) ;\
        waw_part (26,14,13,12,11) ;\
        waw_part (27,15,14,13,12) ;\
        waw_part (28,16,15,14,13) ;\
        waw_part (29,17,16,15,14) ;\
        waw_part (30,18,17,16,15) ;\
        waw_part (31,19,18,17,16) 

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      raw
#  Description:    call raw_part with different register bank
#       
#  Author:
#  Used module:    None
#  Input Param:
#       GPR0-GPR31:Victim registers
#		GPR[v +/- 5,6,7,8]:Aggressor registers
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
#define rawtest          raw_part (0,5,6,7,8) ;\
        raw_part (1,6,7,8,9) ;\
        raw_part (2,7,8,9,10) ;\
        raw_part (3,8,9,10,11) ;\
        raw_part (4,9,10,11,12) ;\
        raw_part (5,10,11,12,13) ;\
        raw_part (6,11,12,13,14) ;\
        raw_part (7,12,13,14,15) ;\
        raw_part (8,13,14,15,16) ;\
        raw_part (9,14,15,16,17) ;\
        raw_part (10,15,16,17,18) ;\
        raw_part (11,16,17,18,19) ;\
        raw_part (12,17,18,19,20) ;\
        raw_part (13,18,19,20,21) ;\
        raw_part (14,19,20,21,22) ;\
        raw_part (15,20,21,22,23) ;\
        raw_part (16,21,22,23,24) ;\
        raw_part (17,22,23,24,25) ;\
        raw_part (18,23,24,25,26) ;\
        raw_part (19,24,25,26,27) ;\
        raw_part (20,25,26,27,28) ;\
        raw_part (21,26,27,28,29) ;\
        raw_part (22,27,28,29,30) ;\
        raw_part (23,28,29,30,31) ;\
        raw_part (24,19,18,17,16) ;\
        raw_part (25,20,19,18,17) ;\
        raw_part (26,21,20,19,18) ;\
        raw_part (27,22,21,20,19) ;\
        raw_part (28,23,22,21,20) ;\
        raw_part (29,24,23,22,21) ;\
        raw_part (30,25,24,23,22) ;\
        raw_part (31,26,25,24,23) 

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      PipelineTest
#  Description:    
#       It carries out the pipeline test.
#  Author:
#  Used module:    None
#  Input Param:
#       GPR3:Test outcome vector
#		GPR4:Mask(OR) to be applied to check vector in case of exit without errors
#		GPR0-GPR31:Registers where the test shall be executed
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .globl PipelineTest

PipelineTest:
         PrologStackOp
         StackCheckBegin

         PushParam(3)
         PushParam(4)
         StackCheckEnd
         StoreGPRs2Stack
         StoreCtrXerCr2Stack
         mfspr 0, SPRG4
         PushReg(0)
         mfspr 0, SPRG5
         PushReg(0)
         stw 31, OFFSET_PUNT_STACK_CORR(1)
         TestCPUStart

         mtspr SPRG4, 1

         wartest
         wawtest
         rawtest

         mfspr 1, SPRG4
         TestConclusion
         b EndPipelineTest

FailPipelineTest:
         
         mtspr SPRG5, 1
         mfspr 1, SPRG4
         ErrorHandling
         mtspr SPRG4, 1
         mfspr 1, SPRG5

         mfspr 2, SPRG7
         subi 2, 2, 256
         stw 1, 132(2)
         mfspr 1, SPRG4

EndPipelineTest:

         StackEpilogBegin
         PopReg(0)
         mtspr SPRG5, 0
         PopReg(0)
         mtspr SPRG4, 0

         RestoreCtrXerCr
         StackEpilogEnd

     blr
