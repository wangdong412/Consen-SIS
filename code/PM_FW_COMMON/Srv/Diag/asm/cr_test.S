
#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march0
#  Description:    
#       move 0x00000000 to cr
#  Author:           
#  Used module:    None
#  Input Param:
#       GPR20:DB0
#		CR:Control Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march0
    mtcr 20
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march1
#  Description:    
#       move 0xffffffff to cr and read out to check if the value is correct
#  Author:           
#  Used module:    None
#  Input Param:
#       GPR18:Read value
#       GPR19:Read value
#       GPR20:DB0
#       GPR21:DB1
#		CR:Control Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march1
    mtcr 21
    mfcr 19
    mfcr 18
    cmplw 19, 21
    bnel fail_CR_test
    cmplw 18, 21
    bnel fail_CR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march2
#  Description:    
#       move 0x00000000 to cr and read out to check if the value is correct
#  Author:           
#  Used module:    None
#  Input Param:
#       GPR18:Read value
#       GPR19:Read value
#       GPR20:DB0
#		CR:Control Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march2
    mtcr 20
    mfcr 19
    mfcr 18
    cmplw 19, 20
    bnel fail_CR_test
    cmplw 18, 20
    bnel fail_CR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march3
#  Description:    
#       move 0xAAAAAAAA cr and then move ~0xAAAAAAAA to cr,            
#       read value fron cr and check if the value is ~0xAAAAAAAA 
#  Author:           
#  Used module:    None
#  Input Param:
#       GPR18:Read value
#       GPR19:Read value
#       GPR22:DB2
#       GPR23:DB3
#		CR:Control Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march3
    mtcr 22
    mtcr 23
    mfcr 19
    mfcr 18
    cmplw 19, 23
    bnel fail_CR_test
    cmplw 18, 23
    bnel fail_CR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march4
#  Description:    
#       move ~0xAAAAAAAA cr and then move 0xAAAAAAAA to cr,            
#       read value fron cr and check if the value is 0xAAAAAAAA 
#  Author:           
#  Used module:    None
#  Input Param:
#       GPR18:Read value
#       GPR19:Read value
#       GPR22:DB2
#       GPR23:DB3
#		CR:Control Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march4
    mtcr 22
    mfcr 19
    mfcr 18
    cmplw 19, 22
    bnel fail_CR_test
    cmplw 18, 22
    bnel fail_CR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march5
#  Description:    
#       move 0xCCCCCCCC cr and then move ~0xCCCCCCCC to cr,            
#       read value fron cr and check if the value is ~0xCCCCCCCC 
#  Author:           
#  Used module:    None
#  Input Param:
#       GPR18:Read value
#       GPR19:Read value
#       GPR24:DB4
#       GPR25:DB5
#		CR:Control Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march5
    mtcr 24
    mtcr 25
    mfcr 19
    mfcr 18
    cmplw 19, 25
    bnel fail_CR_test
    cmplw 18, 25
    bnel fail_CR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march6
#  Description:    
#       move ~0xCCCCCCCC cr and then move 0xCCCCCCCC to cr,            
#       read value fron cr and check if the value is 0xCCCCCCCC 
#  Author:           
#  Used module:    None
#  Input Param:
#       GPR18:Read value
#       GPR19:Read value
#       GPR24:DB4
#       GPR25:DB5
#		CR:Control Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march6
    mtcr 24
    mfcr 19
    mfcr 18
    cmplw 19, 24
    bnel fail_CR_test
    cmplw 18, 24
    bnel fail_CR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march7
#  Description:    
#       move 0xF0F0F0F0 cr and then move ~0xF0F0F0F0 to cr,            
#       read value fron cr and check if the value is ~0xF0F0F0F0
#  Author:           
#  Used module:    None
#  Input Param:
#       GPR18:Read value
#       GPR19:Read value
#       GPR26:DB6
#       GPR27:DB7
#		CR:Control Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 

 .macro march7
    mtcr 26
    mtcr 27
    mfcr 19
    mfcr 18
    cmplw 19, 27
    bnel fail_CR_test
    cmplw 18, 27
    bnel fail_CR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march8
#  Description:    
#       move ~0xF0F0F0F0 cr and then move 0xF0F0F0F0 to cr,            
#       read value fron cr and check if the value is 0xF0F0F0F0
#  Author:           
#  Used module:    None
#  Input Param:
#       GPR18:Read value
#       GPR19:Read value
#       GPR26:DB6
#       GPR27:DB7
#		CR:Control Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march8
    mtcr 26
    mfcr 19
    mfcr 18
    cmplw 19, 26
    bnel fail_CR_test
    cmplw 18, 26
    bnel fail_CR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march9
#  Description:    
#       move 0xFF00FF00 cr and then move ~0xFF00FF00 to cr,            
#       read value fron cr and check if the value is ~0xFF00FF00 
#  Author:           
#  Used module:    None
#  Input Param:
#       GPR18:Read value
#       GPR19:Read value
#       GPR28:DB8
#       GPR29:DB9
#		CR:Control Register 
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march9
    mtcr 28
    mtcr 29
    mfcr 19
    mfcr 18
    cmplw 19, 29
    bnel fail_CR_test
    cmplw 18, 29
    bnel fail_CR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march10
#  Description:    
#       move ~0xFF00FF00 cr and then move 0xFF00FF00 to cr,            
#       read value fron cr and check if the value is 0xFF00FF00 
#  Author:           
#  Used module:    None
#  Input Param:
#       GPR18:Read value
#       GPR19:Read value
#       GPR28:DB8
#       GPR29:DB9
#		CR:Control Register 
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march10
    mtcr 28
    mfcr 19
    mfcr 18
    cmplw 19, 28
    bnel fail_CR_test
    cmplw 18, 28
    bnel fail_CR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march11
#  Description:    
#       move 0xFFFF0000 cr and then move ~0xFFFF0000 to cr,            
#       read value fron cr and check if the value is ~0xFFFF0000
#  Author:           
#  Used module:    None
#  Input Param:
#       GPR18:Read value
#       GPR19:Read value
#       GPR30:DB10
#       GPR31:DB11
#		CR:Control Register   
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march11
    mtcr 30
    mtcr 31
    mfcr 19
    mfcr 18
    cmplw 19, 31
    bnel fail_CR_test
    cmplw 18, 31
    bnel fail_CR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march12
#  Description:    
#       move ~0xFFFF0000 cr and then move 0xFFFF0000 to cr,            
#       read value fron cr and check if the value is 0xFFFF0000 
#  Author:           
#  Used module:    None
#  Input Param:
#       GPR18:Read value
#       GPR19:Read value
#       GPR30:DB10
#       GPR31:DB11
#		CR:Control Register    
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march12
    mtcr 30
    mfcr 19
    mfcr 18
    cmplw 19, 30
    bnel fail_CR_test
    cmplw 18, 30
    bnel fail_CR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      CR_test
#  Description:    
#       test cr with march method
#  Author:           
#  Used module:    None
#  Input Param:
#		GPR3:Test outcome vector
#		GPR4:Mask (OR) to be applied to the check vector in case of exit without errors
#		GPR0:Address of calling function
#		GPR2:Address of error memory area
#       GPR18:Read value
#       GPR19:Read value
#       GPR20-31:DB0-DB11
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#print_message_header  .ascii "print register\r\n"
   .globl TestCR
TestCR:
    PrologStackOp
    StackCheckBegin
    PushParam(3)
    PushParam(4)
    StackCheckEnd
    StoreGPRs2Stack
    StoreCtrXerCr2Stack
    stw 31, OFFSET_PUNT_STACK_CORR(1)
    TestCPUStart

    lis 20, DB0_HI         #set r20-31 with spectional data background sequence
    ori 20, 20, DB0_LO
    not 21, 20    

    lis 22, DB2_HI
    ori 22, 22, DB2_LO
    not 23, 22    

    lis 24, DB4_HI
    ori 24, 24, DB4_LO
    not 25, 24        

    lis 26, DB6_HI
    ori 26, 26, DB6_LO
    not 27, 26    

    lis 28, DB8_HI
    ori 28, 28, DB8_LO
    not 29, 28    

    lis 30, DB10_HI
    ori 30, 30, DB10_LO
    not 31, 30    

    march0
    march1
    march2
    march3
    march4
    march5
    march6
    march7
    march8
    march9
    march10
    march11
    march12
    TestConclusion
    b end_CR_test

fail_CR_test:
    ErrorHandling

end_CR_test:
   
    StackEpilogBegin
    RestoreCtrXerCr
    StackEpilogEnd
    blr
