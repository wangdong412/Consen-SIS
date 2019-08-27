#include "../inc/common_macro.s"
#include "../inc/common_def.h"

 .if 0
##############################
 DBsS01:
     .float     0.1
 DBsS02:       
     .float     -0.2
 DBsS03:      
     .float     0.3
##############################
  DBsD01:
     .double    0.1
  DBsD02:   
     .double    -0.2
  DBsD03:   
     .double    0.3
##############################
  DBsD2B01: 
     .double    10.6
  DBsD2B02:
     .double    -20.2
  DBsD2B03:
     .double    30.4
##############################
  DBsLN01: 
     .long      11
  DBsLN02:
     .long      -20
  DBsLN03:
     .long      30
##############################
  DBsLZ01:
     .long      10
  DBsLZ02:
     .long      -20
  DBsLZ03:
     .long      30
##############################
  DBsLS01:
     .long      10
  DBsLS02:
     .long      -21
  DBsLS03:
     .long      30
##############################
  DBsLL01:
     .long      11
  DBsLL02:
     .long      -20
  DBsLL03:
     .long      31
##############################
  DBsD2BI01:
     .double    -10.6
  DBsD2BI02:
     .double    +20.2
  DBsD2BI03:
     .double    -30.4
##############################
  DBsD2BS01:
     .double    -10.6
  DBsD2BS02:
     .double    -20.2
  DBsD2BS03:
     .double    -30.4
##############################
  DBsD2BC01:
     .double    +10.6
  DBsD2BC02:
     .double    +20.2
  DBsD2BC03:
     .double    +30.4
 .endif
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:     FPU2Test
#  Description: 
#       It carries out the multiplication and division test.
#  Author:
#  Used module:    None
#  Input Param:
#		GPR3:Test outcome vector
#		GPR4:Mask(OR) to be applied to check vector in case of exit without errors
#		FPR0-FPR31:Registers where the test shall be made
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl FPU2Test 

# .section enter_section, "ax"

FPU2Test:
    
     PrologStackOp
     StackCheckBegin

     PushParam (3)
     PushParam (4)

     StackCheckEnd
     StoreGPRs2Stack
     StoreFPRs2Stack
     StoreCtrXerCr2Stack
     StoreFpscr2Stack           #保存FPSCR寄存器

     stw 31, OFFSET_PUNT_STACK_CORR(1)
     TestCPUStart
     
########################装载预设精度################     
     lis  r10,DBsAcrcy@h
     ori  r10, r10,DBsAcrcy@l
     lfd f1, 0(r10)
#########################测试frsp指令###############
#装载单精度转换指令操作结果
     lis  r10,DBsS01@h
     ori  r10, r10,DBsS01@l
     lfs f10, 0(r10)
     lis  r10,DBsS02@h
     ori  r10, r10,DBsS02@l
     lfs f11, 0(r10)
     lis  r10,DBsS03@h
     ori  r10, r10,DBsS03@l
     lfs f12, 0(r10)
#装载操作数值
     lis  r10,DBsD01@h
     ori  r10, r10,DBsD01@l
     lfd f20, 0(r10)
     lis  r10,DBsD02@h
     ori  r10, r10,DBsD02@l
     lfd f21, 0(r10)
     lis  r10,DBsD03@h
     ori  r10, r10,DBsD03@l
     lfd f22, 0(r10)

#测试指令
     frsp f20,f20
     frsp f21,f21
     frsp f22,f22
     
     fsub f20, f20,f10
     fabs f20, f20
     fcmpo cr0, f20, f1
     bgel FailFPU2Test
     
     
     fsub f21, f21,f11
     fabs f21, f21
     fcmpo cr0, f21, f1
     bgel FailFPU2Test
     
     fsub f22, f22,f12
     fabs f22, f22
     fcmpo cr0, f22, f1
     bgel FailFPU2Test

#########################测试frsp.指令###############
#装载单精度转换指令操作结果
     lis  r10,DBsS01@h
     ori  r10, r10,DBsS01@l
     lfs f10, 0(r10)
     lis  r10,DBsS02@h
     ori  r10, r10,DBsS02@l
     lfs f11, 0(r10)
     lis  r10,DBsS03@h
     ori  r10, r10,DBsS03@l
     lfs f12, 0(r10)
#装载操作数值
     lis  r10,DBsD01@h
     ori  r10, r10,DBsD01@l
     lfd f20, 0(r10)
     lis  r10,DBsD02@h
     ori  r10, r10,DBsD02@l
     lfd f21, 0(r10)
     lis  r10,DBsD03@h
     ori  r10, r10,DBsD03@l
     lfd f22, 0(r10)

#测试指令

     frsp. f20,f20
     bsol cr1, FailFPU2Test

     frsp. f21,f21
     bsol cr1, FailFPU2Test

     frsp. f22,f22
     bsol cr1, FailFPU2Test

     fsub f20, f20,f10
     fabs f20, f20
     fcmpo cr0, f20, f1
     bgel FailFPU2Test
     
     
     fsub f21, f21,f11
     fabs f21, f21
     fcmpo cr0, f21, f1
     bgel FailFPU2Test
     
     fsub f22, f22,f12
     fabs f22, f22
     fcmpo cr0, f22, f1
     bgel FailFPU2Test

#########################测试fctiw指令###############
     mtfsb0  30            #00设置为最近
     mtfsb0  31
#装载浮点型至有符号整型指令操作结果
     lis  r10,DBsLN01I@h
     ori  r10, r10,DBsLN01I@l
     lwz  r23, 0(r10)
     lis  r10,DBsLN02I@h
     ori  r10, r10,DBsLN02I@l
     lwz  r24, 0(r10)
     lis  r10,DBsLN03I@h
     ori  r10, r10,DBsLN03I@l
     lwz  r25, 0(r10)

#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs  f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs  f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs  f22, 0(r10)
#测试指令
     fctiw f20, f20
     fctiw f21, f21
     fctiw f22, f22

     addi r31,r1,OFFSET_TEST_VAR
     stfd  f20,0(r31)
     stfd  f21,8(r31)
     stfd  f22,16(r31)

     lwz   r20,4(r31)
     lwz   r21,12(r31)
     lwz   r22,20(r31)
     
     cmp cr0, r20, r23
     bnel FailFPU2Test
     cmp cr0, r21, r24
     bnel FailFPU2Test
     cmp cr0, r22, r25
     bnel FailFPU2Test
   
############
     mtfsb0  30            #01设置为偏向0
     mtfsb1  31
#装载浮点型至有符号整型指令操作结果
     lis  r10,DBsLZ01I@h
     ori  r10, r10,DBsLZ01I@l
     lwz  r23, 0(r10)
     lis  r10,DBsLZ02I@h
     ori  r10, r10,DBsLZ02I@l
     lwz  r24, 0(r10)
     lis  r10,DBsLZ03I@h
     ori  r10, r10,DBsLZ03I@l
     lwz  r25, 0(r10)

#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs f22, 0(r10)
#测试指令
     fctiw f20, f20
     fctiw f21, f21
     fctiw f22, f22
     
     addi r31,r1,OFFSET_TEST_VAR
     stfd  f20,0(r31)
     stfd  f21,8(r31)
     stfd  f22,16(r31)
    
     lwz   r20,4(r31)
     lwz   r21,12(r31)
     lwz   r22,20(r31)
    
     cmp cr0, r20, r23
     bnel FailFPU2Test
     cmp cr0, r21, r24
     bnel FailFPU2Test
     cmp cr0, r22, r25
     bnel FailFPU2Test

################
     mtfsb1  30            #10设置为偏向正无穷
     mtfsb0  31
#装载浮点型至有符号整型指令操作结果
     lis  r10,DBsLL01I@h
     ori  r10, r10,DBsLL01I@l
     lwz  r23, 0(r10)
     lis  r10,DBsLL02I@h
     ori  r10, r10,DBsLL02I@l
     lwz  r24, 0(r10)
     lis  r10,DBsLL03I@h
     ori  r10, r10,DBsLL03I@l
     lwz  r25, 0(r10)

#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs f22, 0(r10)
#测试指令
     fctiw f20, f20
     fctiw f21, f21
     fctiw f22, f22
     
     addi  r31,r1,OFFSET_TEST_VAR
     stfd  f20,0(r31)
     stfd  f21,8(r31)
     stfd  f22,16(r31)
    
     lwz   r20,4(r31)
     lwz   r21,12(r31)
     lwz   r22,20(r31)
    
     cmp cr0, r20, r23
     bnel FailFPU2Test
     cmp cr0, r21, r24
     bnel FailFPU2Test
     cmp cr0, r22, r25
     bnel FailFPU2Test

###############
     mtfsb1  30            #11设置为偏向负无穷
     mtfsb1  31
#装载浮点型至有符号整型指令操作结果
     lis  r10,DBsLS01I@h
     ori  r10, r10,DBsLS01I@l
     lwz  r23, 0(r10)
     lis  r10,DBsLS02I@h
     ori  r10, r10,DBsLS02I@l
     lwz  r24, 0(r10)
     lis  r10,DBsLS03I@h
     ori  r10, r10,DBsLS03I@l
     lwz  r25, 0(r10)

#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs f22, 0(r10)
#测试指令
     fctiw f20, f20
     fctiw f21, f21
     fctiw f22, f22
     
     addi  r31,r1,OFFSET_TEST_VAR
     stfd  f20,0(r31)
     stfd  f21,8(r31)
     stfd  f22,16(r31)
    
     lwz   r20,4(r31)
     lwz   r21,12(r31)
     lwz   r22,20(r31)
    
     cmp cr0, r20, r23
     bnel FailFPU2Test
     cmp cr0, r21, r24
     bnel FailFPU2Test
     cmp cr0, r22, r25
     bnel FailFPU2Test

     lis r3,0x8000
#########################测试fctiw.指令###############
     mtfsb0  30            #00设置为最近
     mtfsb0  31
#装载浮点型至有符号整型转换指令操作结果
     lis  r10,DBsLN01I@h
     ori  r10, r10,DBsLN01I@l
     lwz  r23, 0(r10)
     lis  r10,DBsLN02I@h
     ori  r10, r10,DBsLN02I@l
     lwz  r24, 0(r10)
     lis  r10,DBsLN03I@h
     ori  r10, r10,DBsLN03I@l
     lwz  r25, 0(r10)

#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs  f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs  f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs  f22, 0(r10)
#测试指令

     fctiw. f20, f20
     bsol cr1, FailFPU2Test

     fctiw. f21, f21
     bsol cr1, FailFPU2Test

     fctiw. f22, f22
     bsol cr1, FailFPU2Test
     
     addi  r31,r1,OFFSET_TEST_VAR
     stfd  f20,0(r31)
     stfd  f21,8(r31)
     stfd  f22,16(r31)
    
     lwz   r20,4(r31)
     lwz   r21,12(r31)
     lwz   r22,20(r31)

     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r20, r23
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r21, r24
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r22, r25
     bnel FailFPU2Test

############
     mtfsb0  30            #01设置为偏向0
     mtfsb1  31
#装载浮点型至有符号整型转换指令操作结果
     lis  r10,DBsLZ01I@h
     ori  r10, r10,DBsLZ01I@l
     lwz  r23, 0(r10)
     lis  r10,DBsLZ02I@h
     ori  r10, r10,DBsLZ02I@l
     lwz  r24, 0(r10)
     lis  r10,DBsLZ03I@h
     ori  r10, r10,DBsLZ03I@l
     lwz  r25, 0(r10)

#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs f22, 0(r10)
#测试指令
     
     fctiw. f20, f20
     bsol cr1, FailFPU2Test
     
     fctiw. f21, f21
     bsol cr1, FailFPU2Test

     fctiw. f22, f22
     bsol cr1, FailFPU2Test
     
     addi  r31,r1,OFFSET_TEST_VAR
     stfd  f20,0(r31)
     stfd  f21,8(r31)
     stfd  f22,16(r31)
    
     lwz   r20,4(r31)
     lwz   r21,12(r31)
     lwz   r22,20(r31)

     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r20, r23
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r21, r24
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r22, r25
     bnel FailFPU2Test

################
     mtfsb1  30            #10设置为偏向正无穷
     mtfsb0  31
#装载浮点型至有符号整型转换指令操作结果
     lis  r10,DBsLL01I@h
     ori  r10, r10,DBsLL01I@l
     lwz  r23, 0(r10)
     lis  r10,DBsLL02I@h
     ori  r10, r10,DBsLL02I@l
     lwz  r24, 0(r10)
     lis  r10,DBsLL03I@h
     ori  r10, r10,DBsLL03I@l
     lwz  r25, 0(r10)
#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs f22, 0(r10)
#测试指令

     fctiw. f20, f20
     bsol cr1, FailFPU2Test

     fctiw. f21, f21
     bsol cr1, FailFPU2Test

     fctiw. f22, f22
     bsol cr1, FailFPU2Test
     
     addi  r31,r1,OFFSET_TEST_VAR
     stfd  f20,0(r31)
     stfd  f21,8(r31)
     stfd  f22,16(r31)
    
     lwz   r20,4(r31)
     lwz   r21,12(r31)
     lwz   r22,20(r31)

     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r20, r23
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r21, r24
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r22, r25
     bnel FailFPU2Test


###############
     mtfsb1  30            #11设置为偏向负无穷
     mtfsb1  31
#装载浮点型至有符号整型转换指令操作结果
     lis  r10,DBsLS01I@h
     ori  r10, r10,DBsLS01I@l
     lwz  r23, 0(r10)
     lis  r10,DBsLS02I@h
     ori  r10, r10,DBsLS02I@l
     lwz  r24, 0(r10)
     lis  r10,DBsLS03I@h
     ori  r10, r10,DBsLS03I@l
     lwz  r25, 0(r10)
#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs f22, 0(r10)
#测试指令

     fctiw. f20, f20
     bsol cr1, FailFPU2Test

     fctiw. f21, f21
     bsol cr1, FailFPU2Test

     fctiw. f22, f22
     bsol cr1, FailFPU2Test
     
     addi  r31,r1,OFFSET_TEST_VAR
     stfd  f20,0(r31)
     stfd  f21,8(r31)
     stfd  f22,16(r31)
    
     lwz   r20,4(r31)
     lwz   r21,12(r31)
     lwz   r22,20(r31)

     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r20, r23
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r21, r24
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r22, r25
     bnel FailFPU2Test


     lis r3,0x0001

#########################测试fctiwz指令###############
#装载浮点型至有符号整型转换指令操作结果
     lis  r10,DBsLZ01I@h
     ori  r10, r10,DBsLZ01I@l
     lwz  r23, 0(r10)
     lis  r10,DBsLZ02I@h
     ori  r10, r10,DBsLZ02I@l
     lwz  r24, 0(r10)
     lis  r10,DBsLZ03I@h
     ori  r10, r10,DBsLZ03I@l
     lwz  r25, 0(r10)
#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs f22, 0(r10)
#测试指令
     fctiwz f20, f20
     fctiwz f21, f21
     fctiwz f22, f22
     
     addi  r31,r1,OFFSET_TEST_VAR
     stfd  f20,0(r31)
     stfd  f21,8(r31)
     stfd  f22,16(r31)
    
     lwz   r20,4(r31)
     lwz   r21,12(r31)
     lwz   r22,20(r31)
    
     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r20, r23
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r21, r24
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r22, r25
     bnel FailFPU2Test


    lis r3,0x0002
#########################测试fctiwz.指令###############
#装载浮点型至有符号整型转换指令操作结果
     lis  r10,DBsLZ01I@h
     ori  r10, r10,DBsLZ01I@l
     lwz  r23, 0(r10)
     lis  r10,DBsLZ02I@h
     ori  r10, r10,DBsLZ02I@l
     lwz  r24, 0(r10)
     lis  r10,DBsLZ03I@h
     ori  r10, r10,DBsLZ03I@l
     lwz  r25, 0(r10)
#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs f22, 0(r10)
#测试指令

     fctiwz. f20, f20
     bsol cr1, FailFPU2Test

     fctiwz. f21, f21
     bsol cr1, FailFPU2Test

     fctiwz. f22, f22
     bsol cr1, FailFPU2Test
     
     addi  r31,r1,OFFSET_TEST_VAR
     stfd  f20,0(r31)
     stfd  f21,8(r31)
     stfd  f22,16(r31)
    
     lwz   r20,4(r31)
     lwz   r21,12(r31)
     lwz   r22,20(r31)
    
     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r20, r23
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r21, r24
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     cmp cr0, r22, r25
     bnel FailFPU2Test


    lis r3,0x0004
#########################测试fneg指令###############
#装载取反指令操作结果
     lis  r10,DBsD2BI01@h
     ori  r10, r10,DBsD2BI01@l
     lfs f10, 0(r10)
     lis  r10,DBsD2BI02@h
     ori  r10, r10,DBsD2BI02@l
     lfs f11, 0(r10)
     lis  r10,DBsD2BI03@h
     ori  r10, r10,DBsD2BI03@l
     lfs f12, 0(r10)
#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs  f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs  f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs  f22, 0(r10)
#测试指令
     fneg f20, f20
     fneg f21, f21
     fneg f22, f22
     
     fcmpu f20, f10
     bnel FailFPU2Test
     fcmpu f21, f11
     bnel FailFPU2Test
     fcmpu f22, f12
     bnel FailFPU2Test

    lis r3,0x0008
#########################测试fneg.指令###############
#装载取反指令操作结果
     lis  r10,DBsD2BI01@h
     ori  r10, r10,DBsD2BI01@l
     lfs f10, 0(r10)
     lis  r10,DBsD2BI02@h
     ori  r10, r10,DBsD2BI02@l
     lfs f11, 0(r10)
     lis  r10,DBsD2BI03@h
     ori  r10, r10,DBsD2BI03@l
     lfs f12, 0(r10)
#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs  f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs  f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs  f22, 0(r10)
#测试指令

     fneg. f20, f20
     bsol cr1, FailFPU2Test

     fneg. f21, f21
     bsol cr1, FailFPU2Test

     fneg. f22, f22
     bsol cr1, FailFPU2Test
     
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f20, f10
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f21, f11
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f22, f12
     bnel FailFPU2Test

    lis r3,0x0010
#########################测试fabs指令###############
#装载绝对值指令操作结果
     lis  r10,DBsD2BC01@h
     ori  r10, r10,DBsD2BC01@l
     lfs f10, 0(r10)
     lis  r10,DBsD2BC02@h
     ori  r10, r10,DBsD2BC02@l
     lfs f11, 0(r10)
     lis  r10,DBsD2BC03@h
     ori  r10, r10,DBsD2BC03@l
     lfs f12, 0(r10)
#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs  f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs  f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs  f22, 0(r10)
#测试指令
     fabs f20, f20
     fabs f21, f21
     fabs f22, f22
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f20, f10
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f21, f11
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f22, f12
     bnel FailFPU2Test

    lis r3,0x0020
#########################测试fabs.指令###############
#装载绝对值指令操作结果
     lis  r10,DBsD2BC01@h
     ori  r10, r10,DBsD2BC01@l
     lfs f10, 0(r10)
     lis  r10,DBsD2BC02@h
     ori  r10, r10,DBsD2BC02@l
     lfs f11, 0(r10)
     lis  r10,DBsD2BC03@h
     ori  r10, r10,DBsD2BC03@l
     lfs f12, 0(r10)
#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs  f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs  f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs  f22, 0(r10)
#测试指令

     fabs. f20, f20
     bsol cr1, FailFPU2Test

     fabs. f21, f21
     bsol cr1, FailFPU2Test

     fabs. f22, f22
     bsol cr1, FailFPU2Test
     
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f20, f10
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f21, f11
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f22, f12
     bnel FailFPU2Test

    lis r3,0x0040
#########################测试fnabs指令###############
#装载负绝对值指令操作结果
     lis  r10,DBsD2BS01@h
     ori  r10, r10,DBsD2BS01@l
     lfs f10, 0(r10)
     lis  r10,DBsD2BS02@h
     ori  r10, r10,DBsD2BS02@l
     lfs f11, 0(r10)
     lis  r10,DBsD2BS03@h
     ori  r10, r10,DBsD2BS03@l
     lfs f12, 0(r10)
#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs  f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs  f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs  f22, 0(r10)
#测试指令
     fnabs f20, f20
     fnabs f21, f21
     fnabs f22, f22
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f20, f10
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f21, f11
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f22, f12
     bnel FailFPU2Test

    lis r3,0x0080
#########################测试fnabs.指令###############
#装载负绝对值指令操作结果
     lis  r10,DBsD2BS01@h
     ori  r10, r10,DBsD2BS01@l
     lfs f10, 0(r10)
     lis  r10,DBsD2BS02@h
     ori  r10, r10,DBsD2BS02@l
     lfs f11, 0(r10)
     lis  r10,DBsD2BS03@h
     ori  r10, r10,DBsD2BS03@l
     lfs f12, 0(r10)
#装载操作数值
     lis  r10,DBsD2B01@h
     ori  r10, r10,DBsD2B01@l
     lfs  f20, 0(r10)
     lis  r10,DBsD2B02@h
     ori  r10, r10,DBsD2B02@l
     lfs  f21, 0(r10)
     lis  r10,DBsD2B03@h
     ori  r10, r10,DBsD2B03@l
     lfs  f22, 0(r10)
#测试指令

     fnabs. f20, f20
     bsol cr1, FailFPU2Test

     fnabs. f21, f21
     bsol cr1, FailFPU2Test

     fnabs. f22, f22
     bsol cr1, FailFPU2Test
     
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f20, f10
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f21, f11
     bnel FailFPU2Test
     crxor cr0,cr0,cr0           #清除条件位
     fcmpu f22, f12
     bnel FailFPU2Test



     TestConclusion
     b EndFPU2Test

FailFPU2Test:
     ErrorHandling
        
EndFPU2Test:
#     mtfsf  0xff, f31              
     StackEpilogBegin

     RestoreFpscr           #恢复FPSCR寄存器
     RestoreCtrXerCr
     RestoreFPRs

     StackEpilogEnd

     blr

 .end
