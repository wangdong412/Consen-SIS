#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march0
#  Description:    
#       move 0x00000000 to FPSCR
#  Author:
#  Used module:    None
#  Input Param:
#       FPR20:FPSCR_DBs00
#		FPSCR:浮点型状态控制寄存器
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march0
    mtfsf 0xff, f20
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march1
#  Description:    
#       move 0xfffff7ff to FPSCR and read out to check if the value is correct
#  Author:
#  Used module:    None
#  Input Param:
#       FPR18:Read value
#       FPR19:Read value
#       FPR20:FPSCR_DBs00
#       FPR21:FPSCR_DBs01
#		FPSCR:浮点型状态控制寄存器
#  Output Param:
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march1
    mtfsf 0xff, f21
    lfd f18, 0(r20)
    lfd f19, 0(r20)
    mffs f19
    mffs f18

    addi r31,r1,OFFSET_TEST_VAR
    stfd f18,0(r31)
    stfd f19,8(r31)
    lwz  r10,4(r31)
    lwz  r11,12(r31)
    lwz  r15,4(r21)
    

    cmp r11,r15
    bnel fail_FPSCR_test

    cmp r10,r15
    bnel fail_FPSCR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march2
#  Description:    
#       move 0x00000000 to FPSCR and read out to check if the value is correct
#  Author:
#  Used module:    None
#  Input Param:
#       FPR18:Read value
#       FPR19:Read value
#       FPR20:FPSCR_DBs00
#		FPSCR:浮点型状态控制寄存器
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march2
    mtfsf 0xff, f20
    lfd f18, 0(r20)
    lfd f19, 0(r20)
    mffs f19
    mffs f18

    addi r31,r1,OFFSET_TEST_VAR
    stfd f18,0(r31)
    stfd f19,8(r31)
    lwz  r10,4(r31)
    lwz  r11,12(r31)
    lwz  r15,4(r20)
    
    cmp  r11, r15
    bnel fail_FPSCR_test
    cmp  r10, r15
    bnel fail_FPSCR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march3
#  Description:    
#       move 0xAAAAA2AA to FPSCR and then move 0x55555555 to FPSCR,            
#       read value fron FPSCR and check if the value is 0x55555555 
#  Author:
#  Used module:    None
#  Input Param:
#       FPR18:Read value
#       FPR19:Read value
#       FPR22:FPSCR_DBs10
#       FPR23:FPSCR_DBs11
#		FPSCR:浮点型状态控制寄存器
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march3
    mtfsf 0xff, f22
    mtfsf 0xff, f23
    lfd f18, 0(r20)
    lfd f19, 0(r20)
    mffs f19
    mffs f18
1:
    addi r31,r1,OFFSET_TEST_VAR
    stfd f18,0(r31)
    stfd f19,8(r31)
    lwz  r10,4(r31)
    lwz  r11,12(r31)
#    lwz  r15,4(r23)
    lis r15,0xd555
    ori r15,r15,0x5555

    cmp  r11, r15
    b 1b
#    bnel fail_FPSCR_test
    cmp  r10, r15
    b 1b
#    bnel fail_FPSCR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march4
#  Description:    
#       move 0x55555555 to FPSCR and then move 0xAAAAA2AA to FPSCR,            
#       read value fron FPSCR and check if the value is 0xAAAAA2AA 
#  Author:
#  Used module:    None
#  Input Param:
#       FPR18:Read value
#       FPR19:Read value
#       FPR22:FPSCR_DBs10
#       FPR23:FPSCR_DBs11
#		FPSCR:浮点型状态控制寄存器
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march4
    mtfsf 0xff, f22
    lfd f18, 0(r20)
    lfd f19, 0(r20)
    mffs f19
    mffs f18
    
    addi r31,r1,OFFSET_TEST_VAR
    stfd f18,0(r31)
    stfd f19,8(r31)
    lwz  r10,4(r31)
    lwz  r11,12(r31)
    lwz  r15,4(r22)
    
    cmp  r11, r15
    bnel fail_FPSCR_test
    cmp  r10, r15
    bnel fail_FPSCR_test
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
#		FPSCR:浮点型状态控制寄存器
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march5
    mtfsf 0xff, f24
    mtfsf 0xff, f25
    lfd f18, 0(r20)
    lfd f19, 0(r20)
    mffs f19
    mffs f18

    addi r31,r1,OFFSET_TEST_VAR
    stfd f18,0(r31)
    stfd f19,8(r31)
    lwz  r10,4(r31)
    lwz  r11,12(r31)
    lwz  r15,4(r25)
    
    cmp  r11, r15
    bnel fail_FPSCR_test
    cmp  r10, r15
    bnel fail_FPSCR_test
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
#		FPSCR:浮点型状态控制寄存器
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march6
    mtfsf 0xff,f24
    lfd f18, 0(r20)
    lfd f19, 0(r20)
    mffs f19
    mffs f18
    
    addi r31,r1,OFFSET_TEST_VAR
    stfd f18,0(r31)
    stfd f19,8(r31)
    lwz  r10,4(r31)
    lwz  r11,12(r31)
    lwz  r15,4(r24)
    
    cmp  r11, r15
    bnel fail_FPSCR_test
    cmp  r10, r15
    bnel fail_FPSCR_test
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
#		FPSCR:浮点型状态控制寄存器
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 

 .macro march7
    mtfsf 0xff, f26
    mtfsf 0xff, f27
    lfd f18, 0(r20)
    lfd f19, 0(r20)
    mffs f19
    mffs f18

    addi r31,r1,OFFSET_TEST_VAR
    stfd f18,0(r31)
    stfd f19,8(r31)
    lwz  r10,4(r31)
    lwz  r11,12(r31)
    lwz  r15,4(r27)
    
    cmp  r11, r15
    bnel fail_FPSCR_test
    cmp  r10, r15
    bnel fail_FPSCR_test
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
#		FPSCR:浮点型状态控制寄存器
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march8
    mtfsf 0xff, f26
    lfd f18, 0(r20)
    lfd f19, 0(r20)
    mffs f19
    mffs f18

    addi r31,r1,OFFSET_TEST_VAR
    stfd f18,0(r31)
    stfd f19,8(r31)
    lwz  r10,4(r31)
    lwz  r11,12(r31)
    lwz  r15,4(r26)
    
    cmp  r11, r15
    bnel fail_FPSCR_test
    cmp  r10, r15
    bnel fail_FPSCR_test
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
#		FPSCR:浮点型状态控制寄存器
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march9
    mtfsf 0xff, f28
    mtfsf 0xff, f29
    lfd f18, 0(r20)
    lfd f19, 0(r20)
    mffs f19
    mffs f18

    addi r31,r1,OFFSET_TEST_VAR
    stfd f18,0(r31)
    stfd f19,8(r31)
    lwz  r10,4(r31)
    lwz  r11,12(r31)
    lwz  r15,4(r29)
    
    cmp  r11, r15
    bnel fail_FPSCR_test
    cmp  r10, r15
    bnel fail_FPSCR_test
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
#		FPSCR:浮点型状态控制寄存器
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march10
    mtfsf 0xff, f28
    lfd f18, 0(r20)
    lfd f19, 0(r20)
    mffs f19
    mffs f18

    addi r31,r1,OFFSET_TEST_VAR
    stfd f18,0(r31)
    stfd f19,8(r31)
    lwz  r10,4(r31)
    lwz  r11,12(r31)
    lwz  r15,4(r28)
    
    cmp  r11, r15
    bnel fail_FPSCR_test
    cmp  r10, r15
    bnel fail_FPSCR_test
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
#		FPSCR:浮点型状态控制寄存器
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march11
    mtfsf 0xff, f30
    mtfsf 0xff, f31
    lfd f18, 0(r20)
    lfd f19, 0(r20)
    mffs f19
    mffs f18

    addi r31,r1,OFFSET_TEST_VAR
    stfd f18,0(r31)
    stfd f19,8(r31)
    lwz  r10,4(r31)
    lwz  r11,12(r31)
    lwz  r15,4(r19)
    
    cmp  r11, r15
    bnel fail_FPSCR_test
    cmp  r10, r15
    bnel fail_FPSCR_test
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
#		FPSCR:浮点型状态控制寄存器
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march12
    mtfsf 0xff, f30
    lfd f18, 0(r20)
    lfd f19, 0(r20)
    mffs f19
    mffs f18

    addi r31,r1,OFFSET_TEST_VAR
    stfd f18,0(r31)
    stfd f19,8(r31)
    lwz  r10,4(r31)
    lwz  r11,12(r31)
    lwz  r15,4(r30)
    
    cmp  r11, r15
    bnel fail_FPSCR_test
    cmp  r10, r15
    bnel fail_FPSCR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      FPSCR_test
#  Description:    
#       test FPSCR with march method
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
   .globl FpscrTest
FpscrTest:
    PrologStackOp
    StackCheckBegin
    PushParam (3)
    PushParam (4)
    StackCheckEnd
    StoreGPRs2Stack
    StoreFPRs2Stack
    StoreCtrXerCr2Stack
    StoreFpscr2Stack
    stw 31, OFFSET_PUNT_STACK_CORR(1)
    TestCPUStart

    lis r20, FPSCR_DBs00@h
    ori r20, r20, FPSCR_DBs00@l
    lis r21, FPSCR_DBs01@h
    ori r21, r21, FPSCR_DBs01@l
    lis r22, FPSCR_DBs10@h
    ori r22, r22, FPSCR_DBs10@l
    lis r23, FPSCR_DBs11@h
    ori r23, r23, FPSCR_DBs11@l
    lis r24, FPSCR_DBs20@h
    ori r24, r24, FPSCR_DBs20@l
    lis r25, FPSCR_DBs21@h
    ori r25, r25, FPSCR_DBs21@l
    lis r26, FPSCR_DBs30@h
    ori r26, r26, FPSCR_DBs30@l
    lis r27, FPSCR_DBs31@h
    ori r27, r27, FPSCR_DBs31@l
    lis r28, FPSCR_DBs40@h
    ori r28, r28, FPSCR_DBs40@l
    lis r29, FPSCR_DBs41@h
    ori r29, r29, FPSCR_DBs41@l
    lis r30, FPSCR_DBs50@h
    ori r30, r30, FPSCR_DBs50@l
    lis r31, FPSCR_DBs51@h
    ori r31, r31, FPSCR_DBs51@l
    mr  r19, r31
#先设置f18,f19内容为0x0000000000000000
    lfd f18, 0(r20)
    lfd f19, 0(r20)
    lfd f20, 0(r20)
    lfd f21, 0(r21)
    lfd f22, 0(r22)
    lfd f23, 0(r23)
    lfd f24, 0(r24)
    lfd f25, 0(r25)
    lfd f26, 0(r26)
    lfd f27, 0(r27)
    lfd f28, 0(r28)
    lfd f29, 0(r29)
    lfd f30, 0(r30)
    lfd f31, 0(r31)
    
 .if 0
    li 5,0x00ff
    extsb 5,5
    xori 5,5,MSR_FP
    mfmsr 6
    and 5,5,6
    mtmsr 5
 
    mtfsf 0xff,f20
    mtfsb1 24
    mtfsb1 25
    mtfsb1 26
    mtfsb1 27
    mtfsb1 28
    mtfsb1 29
    mtfsb1 30
    mtfsb1 31
    mffs f18
    stfd f18,0(r20)
    lwz r10,4(r20)
    lwz r11,4(r21)
    cmp r10,r11
    bnel  fail_FPSCR_test
    
    mtfsb0 31
    mffs f18
    stfd f18,0(r20)
    lwz r10,4(r20)
    lis r11,0x0
    ori r11,r11,0x00fe
    cmp r10,r11
    bnel fail_FPSCR_test

    mtfsfi 7,0xa
    mffs f18
    stfd f18,0(r20)
    lwz r10,4(r20)
    lis r11,0x0
    ori r11,r11,0x00fa
    cmp r10,r11
    bnel fail_FPSCR_test

#    mtfsf 0xff,f26
    mtfsb1 0
    mtfsb1 1
    
    mtfsb1 25
    mtfsb1 3
    
    mffs f18
    stfd f18,0(r20)
    lwz r10,4(r20)
    lis r11,0xd000
    ori r11,r11,0x00fa
    cmp r10,r11
    bnel fail_FPSCR_test
 .endif
    march0
    march1
    march2
    march3
#    march4
#    march5
#    march6
#    march7
#    march8
#    march9
#    march10
#    march11
#    march12
  
    TestConclusion
    b end_FPSCR_test

fail_FPSCR_test:
    ErrorHandling
end_FPSCR_test:
   
    StackEpilogBegin
    RestoreFpscr
    RestoreCtrXerCr
    RestoreFPRs
    StackEpilogEnd
    blr
