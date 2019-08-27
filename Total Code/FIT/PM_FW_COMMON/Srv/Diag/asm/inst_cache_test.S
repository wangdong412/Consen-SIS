#include "../inc/common_macro.s"
#include "../inc/common_def.h"
 
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      instrstr_1
#  Description:    
#		40-byte instruction block
#  Author:
#  Used module:    None
#  Input Param:
#		GPR5:Address of instruction to be executed
#		GPR6:Repetition counter
#		GPR9:Operating code of instruction to be replaced
#		GPR10:Check the instruction cache starting from the indicated value
#		GPR11:Value added to and subtracted 
#		GPR15:Offset for correct repositioning at the subsequent code change
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro instrstr_1
	    add 10, 10, 11
	    cmpwi 6, 1            #r6 == ctr
	    bnel $+16
	    lis 9, 0x7D4A
	    ori 9, 9, 0x5A14
	    b $+12
	    lis 9, 0x7D4B
	    ori 9, 9, 0x5050
	    stwx 9, 5, 15
	    dcbst 5, 15
	    sync
	    icbi 5, 15
	    sync
	    isync
	    addi 15, 15, 60
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      instrstr_10
#  Description:    
#		execute instrstr_1 instruction 10 times
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
 .macro instrstr_10
        instrstr_1
        instrstr_1
        instrstr_1
        instrstr_1
        instrstr_1
        instrstr_1
        instrstr_1
        instrstr_1
        instrstr_1
        instrstr_1
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      instrstr_50
#  Description:    
#       execute instrstr_1 instruction 50 times
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
 .macro instrstr_50
        instrstr_10
        instrstr_10
        instrstr_10
        instrstr_10
        instrstr_10
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      instrstr_70
#  Description:    
#       execute instrstr_1 instruction 70 times
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
 .macro instrstr_70
        instrstr_50
        instrstr_10
        instrstr_10
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      instrstr_200
#  Description:    
#       execute instrstr_1 instruction 200 times
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
 .macro instrstr_200
        instrstr_50
        instrstr_50
        instrstr_50
        instrstr_50
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      InstrCacheTest
#  Description:    
#       It carries out the instruction cache test.
#  Author:
#  Used module:    None
#  Input Param:
#		GPR3:Test outcome vector
#		GPR4:Mask(OR) to be applied to check vector in case of exit without errors
#		GPR0:Address of calling function
#		GPR2:Address of error memory area
#		GPR5:Instruction address
#		GPR6:Number of cycles to be executed
#		GPR9:It will include the operating code of instruction subf 10, 11, 10
#		GPR10:Check the instruction cache starting from the indicated value
#		GPR11:Value added to and subtracted from R10
#		GPR15:Offset for correct repositioning at the subsequent code change
#		GPR30:Mask for setting to 1 bit 20 of HID0
#		GPR31:Copy of HID0 changed to invalidate the instruction cache
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl InstrCacheTest
InstrCacheTest:
              
      PrologStackOp
      StackCheckBegin

      PushParam(3)     #keep input parameter in stack
      PushParam(4)
      
      StackCheckEnd
      StoreGPRs2Stack
      StoreCtrXerCr2Stack

      mfspr 0, HID0
      PushReg(0)
      stw 31, OFFSET_PUNT_STACK_CORR(1)
      TestCPUStart
# invalidate instruction cache
      lis 30, MASK_HID0_ICFI_BITS_HI
      ori 30, 30, MASK_HID0_ICFI_BITS_LO
      mfspr 31, HID0
      or 30,31,30
      mtspr HID0, 30
      mtspr HID0, 31
      isync

      li 10, 0
      li 11, 1
      li 6, 2 
      mtctr 6
      bl $+4  
      mflr 5

      addi 5, 5, 16
loop_test:
     mfctr 6
     li 15, 0
     instrstr_200
     instrstr_70
     instrstr_1
     instrstr_1
     instrstr_1

     bdnz loop_test      #decement,ctr not zero and cr0 not zero
     cmpwi 10, 0
     bnel FailInstrCacheTest

     TestConclusion
     b EndInstrCacheTest

FailInstrCacheTest:

     ErrorHandling

EndInstrCacheTest:
     StackEpilogBegin

     PopReg (0)
     sync
     mtspr HID0, 0
     sync
     isync

     RestoreCtrXerCr
     StackEpilogEnd

     blr
