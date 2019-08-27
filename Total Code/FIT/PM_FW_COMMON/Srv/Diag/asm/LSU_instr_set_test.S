#include "../inc/common_macro.s"
#include "../inc/common_def.h"


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:     LSUInstructionTest
#  Description:   
#       It carries out the test on the Load-Store Unit instructions not tested yet.
#  Author:
#  Used module:    None
#  Input Param:
#       GPR3:Test outcome vector
#		GPR4:Mask(OR) to be applied to check vector in case of exit without errors
#		GPR5:Address of memory area where the operation shall be executed
#		GPR0:Address of calling function
#		GPR6:Swap register for GPR5
#		GPR7:Offset needed for load/store with indirect addressing and index
#		GPR10-19:Patterns stored in GPR
#		GPR20-23:Read patterns to be compared
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl LSUInstructionTest
LSUInstructionTest:
    
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

     mr 3, 5
#         bl CheckAddress
#         cmpwi 3, 1
#         bnel FailLSUInstructionTest

     mr 31, 1
     addi 31, 31, OFFSET_STACK_PARAMETERS
     lwz 5, 8(31)

     lis 10, 0x0000
     ori 10, 10, 0x8111
     lis 11, 0xFFFF
     ori 11, 11, 0x8111
     lis 12, 0xCCCC
     ori 12, 12, 0xCCCC
     not 13, 12
     lis 14, 0xF0F0
     ori 14, 14, 0xF0F0
     not 15, 14
     lis 16, 0xFF00
     ori 16, 16, 0xFF00
     not 17, 16

     mr 6, 5
     sthbrx 16, 0, 6
     dcbf 0, 6
     sync

     xor 20, 20, 20
     lhbrx 20, 0, 6
     cmplwi 20, 0xFF00
     bnel FailLSUInstructionTest

     stwbrx 16, 0, 6
     dcbf 0, 6
     sync

     lwbrx 20, 0, 6
     cmplw 20, 16
     bnel FailLSUInstructionTest
     
     //stswi etc. is not supported in PPC E500 core
     

     mr 5, 6
     sth 10, 0(5)

     li 7, 2
     sthx 10, 5, 7

     sthu 10, 4(5)
     sthux 10, 5, 7
     dcbf 0, 6
     sync

     mr 5, 6
     lha 20, 0(5)
     cmplw 20, 11
     bnel FailLSUInstructionTest

     dcbf 0, 6
     sync

     li 7, 2
     lhax 21, 5, 7
     cmplw 21, 11
     bnel FailLSUInstructionTest
     dcbf 0, 6
     sync

     lhau 22, 4(5)
     cmplw 22, 11
     bnel FailLSUInstructionTest
     dcbf 0, 6
     sync

     lhaux 23, 5, 7
     cmplw 23, 11
     bnel FailLSUInstructionTest

     mr 5, 6
     li 7, 0
     stwx 16, 5, 7
     lwarx 20, 5, 7
     cmplw 20, 16
     bnel FailLSUInstructionTest
     stwcx. 17, 5, 7
     bnel FailLSUInstructionTest
     lwzx 20, 5, 7
     cmplw 20, 17
     bnel FailLSUInstructionTest

     TestConclusion
     b EndLSUInstructionTest

FailLSUInstructionTest:
             
     ErrorHandling

EndLSUInstructionTest:
     StackEpilogBegin
     RestoreCtrXerCr
     StackEpilogEnd

     blr
 .end
