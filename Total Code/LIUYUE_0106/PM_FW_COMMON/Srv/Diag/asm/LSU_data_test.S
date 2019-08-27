#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      cppattern
#  Description: 
#       Copy of patterns contained in GPR30 and GPR31 on registers GPR10 and GPR11.
#  Author:
#  Used module:    None
#  Input Param:
#       GPR10-11:Pattern
#		GPR30-31:Destination registers where patterns are copied
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro cppattern
         mr 11, 31
         mr 10, 30
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      storeRegister
#  Description: 
#       Store data to sdram
#  Author:
#  Used module:    None
#  Input Param:
#       GPR5:Pointer to memory locations where writing shall be made
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro storeRegister
         dcbf 0, 5
         sync
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      cmppattern
#  Description: 
#       It checks that the patterns have been saved and loaded correctly.
#  Author:
#  Used module:    None
#  Input Param:
#       R2-R31:Registers including the patterns where the test shall be executed
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro cmppattern
         cmplw 30, 10
         bnel FailLsuTestConnectionData
         cmplw 31, 11
         bnel FailLsuTestConnectionData
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:     LSUDataTest
#  Description:   
#       It carries out the data connection test for the Load-Store Unit 
#  Author:
#  Used module:    None
#  Input Param:
#       GPR3:Test outcome vector
#		GPR4:Mask(OR) to be applied to check vector in case of exit without errors
#		GPR5:Address of memory area where the patterns shall be applied
#		GPR0:Address of calling function
#		GPR1:Stack address
#		GPR30-31:Patterns written/read
#		GPR10-11:Patterns written
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl LSUDataTest
LSUDataTest:

     PrologStackOp
     StackCheckBegin

     PushParam(3)
     PushParam(4)
     PushParam(5)

     StackCheckEnd
     StoreGPRs2Stack
     StoreCtrXerCr2Stack
     stw 31, OFFSET_PUNT_STACK_CORR(1)

     TestCPUStart

     mr 31, 1
     addi 31, 31, OFFSET_STACK_PARAMETERS 
     lwz 4, 4(31)
     lwz 5, 8(31)

#     mr 3, 5
#     bl CheckAddress
#     cmpwi 3, 1
#     bnel FailLsuTestConnectionData

     mr 31, 1
     addi 31, 31, OFFSET_STACK_PARAMETERS
     lwz 5, 8(31)

     lis 30, 0xAAAA
     ori 30, 30, 0xAAAA
     not 31, 30
     stmw 30, 0(5)
     storeRegister
     cppattern

     lmw 30, 0(5)
     cmppattern

     lis 30, 0xCCCC
     ori 30, 30, 0xCCCC
     not 31, 30
     stmw 30, 0(5)
     storeRegister
     cppattern
     lmw 30, 0(5)
     cmppattern

     lis 30, 0xF0F0
     ori 30, 30, 0xF0F0
     not 31, 30
     stmw 30, 0(5)
     storeRegister
     cppattern
     lmw 30, 0(5)
     cmppattern

     lis 30, 0xFF00
     ori 30, 30, 0xFF00
     not 31, 30
     stmw 30, 0(5)
     storeRegister
     cppattern
     lmw 30, 0(5)
     cmppattern

     lis 30, 0xFFFF
     not 31, 30
     stmw 30, 0(5)
     storeRegister
     cppattern
     lmw 30, 0(5)
     cmppattern

     TestConclusion
     
     b EndLusTestConnectionData

FailLsuTestConnectionData:
     ErrorHandling
EndLusTestConnectionData:
     StackEpilogBegin
     RestoreCtrXerCr
     StackEpilogEnd
     blr
 .end


