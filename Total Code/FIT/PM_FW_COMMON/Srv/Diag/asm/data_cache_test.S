#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      load_pattern
#  Description:    
#       It loads the pattern inside registers GPR24-31,accord to index,set r24-31
#       to 0 or 0xFFFFFFFF
#  Author:
#  Used module:    None
#  Input Param:    
#       index:pattern parameter,0    =>    0x00000000, 1    =>    0xFFFFFFFF
#       GPR24-31:Patterns
#  Output Param:   
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define load_pattern(index) \
        .if index == 0 ;\
            xor 24, 24, 24 ;\
            xor 25, 25, 25 ;\
            xor 26, 26, 26 ;\
            xor 27, 27, 27 ;\
            xor 28, 28, 28 ;\
            xor 29, 29, 29 ;\
            xor 30, 30, 30 ;\
            xor 31, 31, 31 ;\
        .endif ;\
        .if index == 1 ;\
            lis 24, 0xFFFF ;\
            ori 24, 24, 0xFFFF ;\
            lis 25, 0xFFFF ;\
            ori 25, 25, 0xFFFF ;\
            lis 26, 0xFFFF ;\
            ori 26, 26, 0xFFFF ;\
            lis 27, 0xFFFF ;\
            ori 27, 27, 0xFFFF ;\
            lis 28, 0xFFFF ;\
            ori 28, 28, 0xFFFF ;\
            lis 29, 0xFFFF ;\
            ori 29, 29, 0xFFFF ;\
            lis 30, 0xFFFF ;\
            ori 30, 30, 0xFFFF ;\
            lis 31, 0xFFFF ;\
            ori 31, 31, 0xFFFF ;\
        .endif


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      check_pattern
#  Description:    
#       It checks the content of registers GPR24-31 if equal to the given value
#  Author:
#  Used module:    None
#  Input Param:    
#       index:pattern parameter,0    =>    0x00000000, 1    =>    0xFFFFFFFF
#       GPR24-31:Patterns
#  Output Param:   
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define check_pattern(index) \
        .if index == 0 ;\
            xor 23, 23, 23 ;\
            cmplw 24, 23 ;\
            bnel FailDataCacheTest ;\
            cmplw 25, 23 ;\
            bnel FailDataCacheTest ;\
            cmplw 26, 23 ;\
            bnel FailDataCacheTest ;\
            cmplw 27, 23 ;\
            bnel FailDataCacheTest ;\
            cmplw 28, 23 ;\
            bnel FailDataCacheTest ;\
            cmplw 29, 23 ;\
            bnel FailDataCacheTest ;\
            cmplw 30, 23 ;\
            bnel FailDataCacheTest ;\
            cmplw 31, 23 ;\
            bnel FailDataCacheTest ;\
        .endif ;\
        .if index == 1 ;\
            lis 23, 0xFFFF ;\
            ori 23, 23, 0xFFFF ;\
            cmplw 24, 23 ;\
            bnel FailDataCacheTest ;\
            cmplw 25, 23 ;\
            bnel FailDataCacheTest ;\
            cmplw 26, 23 ;\
            bnel FailDataCacheTest ;\
            cmplw 27, 23 ;\
            bnel FailDataCacheTest ;\
            cmplw 28, 23 ;\
            bnel FailDataCacheTest ;\
            cmplw 29, 23 ;\
            bnel FailDataCacheTest ;\
            cmplw 30, 23 ;\
            bnel FailDataCacheTest ;\
            cmplw 31, 23 ;\
            bnel FailDataCacheTest ;\
        .endif


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      unlock_way
#  Description:    
#       It locks all cache ways (from 0 to 3) excluded that indicated
#       by the parameter 'num'.Register LDSTCR is used for this purpose.
#  Author:
#  Used module:    None
#  Input Param:    
#       num:Number of cache way to unlock
#       GPR9:Swap register for modification of HID2, HID0
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define unlock_way(num) \
        xor 9, 9, 9 ;\
        .if num == 0 ;\
            ori 9, 9, 0x0000 ;\
        .elseif num == 1 ;\
            ori 9, 9, 0x0020 ;\
        .elseif num == 2 ;\
            ori 9, 9, 0x0040 ;\
        .elseif num == 3 ;\
            ori 9, 9, 0x0060 ;\
        .endif ;\
        mfspr 18, HID2 ;\
        lis 14, MASK_HID2_DWLCK_BITS_HI ;\
        ori 14, 14, MASK_HID2_DWLCK_BITS_LO ;\
        and 18, 18, 14 ;\
        or 18, 18, 9 ;\
        sync ;\
        mtspr HID2, 18 ;\
        sync ;\
        isync


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march0_single_set
#  Description:    
#       It executes March test number 0 on a single set 
#  Author:
#  Used module:    None
#  Input Param:    
#       addr:Register including the base address of the block to be used
#       GPR11:Block address
#       GPR24-31:Patterns
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define march0_single_set(addr) \
        mr 11, addr ;\
        unlock_way(3) ;\
        stmw 24, 0(11) ;\
        addi 11, 11, 0x1000 ;\
        unlock_way(2) ;\
        stmw 24, 0(11) ;\
        addi 11, 11, 0x1000 ;\
        unlock_way(1) ;\
        stmw 24, 0(11) ;\
        addi 11, 11, 0x1000 ;\
        unlock_way(0) ;\
        stmw 24, 0(11)


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march1234_single_set
#  Description:    
#       It executes March test number 1,2,3,4 on a single set
#  Author:
#  Used module:    None
#  Input Param:    
#       addr:Register including the base address of the block to be used       
#       not_addr: Register including the base address of the block (negated) to be used    
#       direction:March direction: March 0 down-to-up, 1 up-to-down 0 
#       r_pattern:Pattern index expected for reading           
#       w_pattern:Pattern index for writing
#       GPR11:Block address
#       GPR15:Block negated address
#       GPR23:Pattern compare value
#       GPR24-31:Patterns
#  Output Param:   
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define march1234_single_set(addr, not_addr, r_pattern, w_pattern) \
        mr 11, addr ;\
        mr 15, not_addr ;\
        addi 11, 11, 0x3000 ;\
        lmw 24, 0(11) ;\
        check_pattern(r_pattern) ;\
        load_pattern(w_pattern) ;\
        stmw 24, 0(15) ;\
        lmw 24, 0(15) ;\
        check_pattern(w_pattern) ;\
        subi 11, 11, 0x1000 ;\
        addi 15, 15, 0x1000 ;\
        lmw 24, 0(11) ;\
        check_pattern(r_pattern) ;\
        load_pattern(w_pattern) ;\
        stmw 24, 0(15) ;\
        lmw 24, 0(15) ;\
        check_pattern(w_pattern) ;\
        subi 11, 11, 0x1000 ;\
        addi 15, 15, 0x1000 ;\
        lmw 24, 0(11) ;\
        check_pattern(r_pattern) ;\
        load_pattern(w_pattern) ;\
        stmw 24, 0(15) ;\
        lmw 24, 0(15) ;\
        check_pattern(w_pattern) ;\
        subi 11, 11, 0x1000 ;\
        addi 15, 15, 0x1000 ;\
        lmw 24, 0(11) ;\
        check_pattern(r_pattern) ;\
        load_pattern(w_pattern) ;\
        stmw 24, 0(15) ;\
        lmw 24, 0(15) ;\
        check_pattern(w_pattern) ;\
        subi 11, 11, 0x1000 ;\
        addi 15, 15, 0x1000

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march5_single_set
#  Description:    
#       It executes March test number 5 on a single set.
#  Author:
#  Used module:    None
#  Input Param:
#       addr:Register including the base address of the block to be used 
#       GPR11:Block address
#       GPR24-31:Patterns
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define march5_single_set(addr) \
        mr 11, addr ;\
        lmw 24, 0(11) ;\
        check_pattern(0) ;\
        addi 11, 11, 0x1000 ;\
        lmw 24, 0(11) ;\
        check_pattern(0) ;\
        addi 11, 11, 0x1000 ;\
        lmw 24, 0(11) ;\
        check_pattern(0) ;\
        addi 11, 11, 0x1000 ;\
        lmw 24, 0(11) ;\
        check_pattern(0) ;\
        addi 11, 11, 0x1000

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      DataCacheTest
#  Description:    
#       It executes March test C- extended on the data cache 
#       (covering SAF, SOF, AF, TF, CFin, CFid, CFst, i.e. all faults)
#  Author:
#  Used module:    None
#  Input Param:
#       GPR3:Test outcome vector
#       GPR4:Mask (OR) to be applied to the check vector in case of exit without errors
#       GPR5:Address of area where the test shall be executed
#       GPR6:Address (negated) of a further area where the test shall be executed
#       GPR7:set where the test shall be executed    
#       GPR0:Address of calling function
#       GPR2:Address of calling function
#       GPR9:HID2, HID0 mask
#       GPR10:Swap of GPR5 content for movement among sets
#       GPR11:GPR10 swap for movement inside the set
#       GPR12:GPR6 content swap for movement among sets
#       GPR13:GPR12 swap for movement inside the set
#       GPR23:Value read by the cache
#       GPR24-30:Pattern
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl DataCacheTest
DataCacheTest:
      PrologStackOp
      StackCheckBegin
      PushParam(3)
      PushParam(4)
      PushParam(5)
      PushParam(6)
      PushParam(7)
      StackCheckEnd
      StoreGPRs2Stack
      StoreCtrXerCr2Stack

      mfspr 0, HID2
      PushReg(0)
      stw 31, OFFSET_PUNT_STACK_CORR(1)
      TestCPUStart

      mr 31, 1
      addi 31, 31, OFFSET_STACK_PARAMETERS
      lwz 7, 16(31)
      lwz 6, 12(31)
      lwz 5, 8(31)
      lwz 4, 4(31)

      xor 20, 20, 20   #r20 = 0
      li 20, 0x0020
      mullw 21, 7, 20

      add 10, 5, 21
      add 12, 6, 21
#store addres
      mtspr 274,10
      mtspr 275,12
      
      mfspr 3, 274                    #check if the parameter pass in is correct
      li 4, 1                    #set flag
      bl CheckAddressDataCache
      cmpwi 3, 1
      bnel FailDataCacheTest        #if address check failed ,jump to fault handle function
      
      mfspr 3, 275
      li 4, 2                    #set flag
      bl CheckAddressDataCache
      cmpwi 3, 1
      bnel FailDataCacheTest        #if address check failed ,jump to fault handle function
#restore address      
      mfspr 10, 274
      mfspr 12, 275
      
      load_pattern(0)
      
      march0_single_set(10)

      march1234_single_set(10, 12, 0, 1)

      march1234_single_set(12, 10, 1, 1)

      march1234_single_set(10, 12, 1, 0)

      march1234_single_set(12, 10, 0, 0)

      march5_single_set(10)

      TestConclusion
      b EndDataCacheTest

FailDataCacheTest:
      ErrorHandling
EndDataCacheTest:
      StackEpilogBegin
      PopReg(0)
      sync
      mtspr HID2, 0
      sync
      isync

      RestoreCtrXerCr
      StackEpilogEnd
      blr
