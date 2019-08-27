#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:     LSUEffectiveAddressTest
#  Description:   
#       It carries out the data and address connection test for the Load-Store Unit.
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
#		GPR24:Mask for setting to 0 bit 17 of HID0
#		GPR25:HID0 copy
#		GPR26:HID0 copy changed to disable data cache
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl LSUEffectiveAddressTest

LSUEffectiveAddressTest:

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
#         bnel FailLSUEffectiveAddressTest

     mr 31, 1
     addi 31, 31, OFFSET_STACK_PARAMETERS
     lwz 5, 8(31)

     lis 10, 0xAAAA
     ori 10, 10, 0xAAAA
     not 11, 10
     lis 12,0xCCCC
     ori 12, 12, 0xCCCC
     not 13, 12
     lis 14, 0xF0F0
     ori 14, 14, 0xF0F0
     not 15, 14
     lis 16, 0xFF00
     ori 16, 16, 0xFF00
     not 17, 16
     lis 18, 0xFFFF
     not 19, 18

     mr 6, 5
     stb 10, 0(5)

     li 7, 1
     stbx 11, 5, 7

     stbu 12, 2(5)

     stbux 13, 5, 7

     dcbf 0, 6
     sync

     mr 5, 6
     lbz 20, 0(5)
     cmplwi 20, 0x00AA
     bnel FailLSUEffectiveAddressTest

     dcbf 0, 6
     sync

     li 7, 1
     lbzx 21, 5, 7
     cmplwi 21, 0x0055
     bnel FailLSUEffectiveAddressTest

     dcbf 0, 6
     sync

     lbzu 22, 2(5)
     cmplwi 22, 0x00CC
     bnel FailLSUEffectiveAddressTest

     dcbf 0, 6
     sync

     lbzux 23, 5, 7
     cmplwi 23, 0x0033
     bnel FailLSUEffectiveAddressTest

     mr 5, 6
     sth 14, 0(5)

     li 7, 2
     sthx 15, 5, 7
     sthu 16, 4(5)

     sthux 17, 5, 7

     dcbf 0, 6
     sync

     mr 5, 6
     lhz 20, 0(5)
     cmplwi 20, 0xF0F0
     bnel FailLSUEffectiveAddressTest

     dcbf 0, 6
     sync

     li 7, 2
     lhzx 21, 5, 7
     cmplwi 21, 0x0F0F
     bnel FailLSUEffectiveAddressTest

     dcbf 0, 6
     sync

     lhzu 22, 4(5)
     cmplwi 22, 0xFF00
     bnel FailLSUEffectiveAddressTest
     dcbf 0, 6
     sync

     lhzux 23, 5, 7
     cmplwi 23, 0x00FF
     bnel FailLSUEffectiveAddressTest

     mr 5, 6
     stw 18, 0(5)

     li 7, 4
     stwx 19, 5, 7
     stwu 10, 8(5)
     stwux 11, 5, 7

     dcbf 0, 6
     sync

     mr 5, 6
     lwz 20, 0(5)
     cmplw 20 ,18
     bnel FailLSUEffectiveAddressTest

     dcbf 0, 6
     sync

     li 7, 4
     lwzx 21, 5, 7
     cmplw 21, 19
     bnel FailLSUEffectiveAddressTest
     dcbf 0, 6
     sync

     lwzu 22, 8(5)
     cmplw 22, 10
     bnel FailLSUEffectiveAddressTest
     dcbf 0, 6
     sync

     lwzux 23, 5, 7
     cmplw 23, 11
     bnel FailLSUEffectiveAddressTest
     
     TestConclusion
     b EndLSUEffectiveAddressTest

FailLSUEffectiveAddressTest:
     ErrorHandling

EndLSUEffectiveAddressTest:
     StackEpilogBegin
     RestoreCtrXerCr
     StackEpilogEnd

     blr
 .end
