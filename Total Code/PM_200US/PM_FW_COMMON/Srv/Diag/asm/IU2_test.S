
#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:     IU2Test
#  Description: 
#       It carries out the multiplication and division test.
#  Author:
#  Used module:    None
#  Input Param:
#		GPR3:Test outcome vector
#		GPR4:Mask(OR) to be applied to check vector in case of exit without errors
#		R0-R31:Registers where the test shall be made
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl IU2Test 

# .section enter_section, "ax"

IU2Test:
    
         PrologStackOp
         StackCheckBegin

         PushParam(3)
         PushParam(4)

         StackCheckEnd
         StoreGPRs2Stack
         StoreCtrXerCr2Stack

         stw 31, OFFSET_PUNT_STACK_CORR(1)
         TestCPUStart

         lis 10, 0xAAAA
         ori 10, 10, 0xAAAA
         not 11, 10
         lis 12, 0xCCCC
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

# .section output_pattern_test, "ax"
       
         li 0, 0x0002
         mullw 2, 11, 0
         cmpw 2, 10
         bnel FailIU2Test

         lis 2, 0x1111
         ori 2, 2, 0x1111
         li 5, 0x0005
         mullw. 6, 5, 2
         bngl FailIU2Test
         cmpw 6, 11
         bnel FailIU2Test

         lis 7, 0x1111
         ori 7, 7, 0x1111
         lis 8, 0x0000
         mtspr XER, 8
         mtcr 8
         li 5, 0x000C
         mullwo 6, 5, 7
         mcrxr 1
         cmpw 6, 12
         bnel FailIU2Test
         lis 31, 0x2C00
         mfcr 30
         cmpw 30, 31
         bnel FailIU2Test

         lis 6, 0x1111
         ori 6, 6, 0x1111
         mulli 8, 6, 0x0003
         cmpw 8, 13
         bnel FailIU2Test

         li 26, 0x0001
         divw. 27, 14, 26
         bnll FailIU2Test
         cmpw 27, 14
         bnel FailIU2Test

         li 27, 0x0001
         divwo 28, 15, 27
         mcrxr 1
         cmpw 28, 15
         lis 2, 0x2000
         mfcr 5
         cmplw 2, 5
         bnel FailIU2Test
         
         li 28, 0x0001
         divwo. 29, 16, 28
         bnl FailIU2Test
         mcrxr 1
         cmpw 29, 16
         mfcr 5
         cmplw 2, 5
         bnel FailIU2Test

         lis 29, 0x04FB
         ori 29, 29, 0x04FB
         li 30, 0x0005
         divwu 31, 29, 30
         cmpw 31, 17
         bnel FailIU2Test

         li 31, 0x0001
         divwu. 30, 18, 31
         bnll FailIU2Test
         cmpw 30, 18
         bnel FailIU2Test

         lis 31, 0x00AA
         ori 31, 31, 0xFF55
         li 30, 0x00AB
         divwuo 12, 31, 30
         mcrxr 1
         cmpw 12, 19
         lis 2, 0x2000
         mfcr 5
         cmplw 2, 5 
         bnel FailIU2Test

# .section input_pattern_test, "ax"
         lis 9, 0xE38E
         ori 9, 9, 0x38E3
         mulhw 20, 10, 11
         cmpw 20, 9
         bnel FailIU2Test

         lis 20, 0x0000
         ori 20, 20, 0x3332
         mulhw. 21, 12, 13
         bngl FailIU2Test
         cmpw 21, 20
         bnel FailIU2Test
         
         lis 22, 0x0E2C
         ori 22, 22, 0x4A68
         mulhwu 23, 14, 15
         cmpw 23, 22
         bnel FailIU2Test

         lis 24, 0x00FE
         ori 24, 24, 0x02FC
         mulhwu. 25, 16, 17
         bngl FailIU2Test
         cmpw 25, 24
         bnel FailIU2Test

         lis 25, 0xFFFF
         ori 25, 25, 0xFFFF
         divw 26, 18, 19
         cmpw 26, 25
         bnel FailIU2Test

# .section overflow_test, "ax"
         
         lis 6, 0x8000
         xor 0, 0, 0 
         mtcr 0
         lis 7, 0xFFFF
         ori 7, 7, 0xFFFF
         lis 28, 0x8000
         divwuo. 29, 6, 7
         mcrxr 0
         bgtl FailIU2Test

         xor 0, 0, 0
         mtcr 0
         lis 7, 0x0000
         ori 7, 7, 0x0002
         mullwo. 5, 6, 7
         mcrxr 1
         bnel FailIU2Test
         lis 2, 0x3C00
         mfcr 0
         cmplw 0, 2
         bnel FailIU2Test
         cmplwi 5, 0x0000
         bnel FailIU2Test

# .section return_section, "ax"
         TestConclusion
         b EndIU2Test

FailIU2Test:
         ErrorHandling

EndIU2Test:
         StackEpilogBegin
         RestoreCtrXerCr
         StackEpilogEnd

         blr

 .end
