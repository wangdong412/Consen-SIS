#include "../inc/common_macro.s"
#include "../inc/common_def.h"

 .if 0 
#测试算术指令操作数
  DBsZero01  .float   0.9
  DBsZero02  .float   1.8
  DBsZero03  .float   2.7
  DBsZero04  .float   3.6
  DBsZero05  .float   4.5
  DBsZero06  .float   5.4
  DBsZero07  .float   6.3
  DBsZero08  .float   7.2
  DBsZero09  .float   8.1
  DBsZero10  .float   9.0
  
  DBsUno01   .float   0.1
  DBsUno02   .float   1.2
  DBsUno03   .float   2.3
  DBsUno04   .float   3.4
  DBsUno05   .float   4.5
  DBsUno06   .float   5.6
  DBsUno07   .float   6.7
  DBsUno08   .float   7.8
  DBsUno09   .float   8.9
  DBsUno10   .float   9.0
#测试加法指令结果
  DBsAdd01   .float   1.0
  DBsAdd02   .float   3.0
  DBsAdd03   .float   5.0
  DBsAdd04   .float   7.0
  DBsAdd05   .float   9.0
  DBsAdd06   .float   11.0
  DBsAdd07   .float   13.0
  DBsAdd08   .float   15.0
  DBsAdd09   .float   17.0
  DBsAdd10   .float   18.0
#测试减法指令结果
  DBsSub01   .float   0.8
  DBsSub02   .float   0.6
  DBsSub03   .float   0.4
  DBsSub04   .float   0.2
  DBsSub05   .float   0.0
  DBsSub06   .float   -0.2
  DBsSub07   .float   -0.4
  DBsSub08   .float   -0.6
  DBsSub09   .float   -0.8
  DBsSub10   .float   -0.0
#测试乘法指令结果
  DBsMul01   .float   0.09
  DBsMul02   .float   2.16
  DBsMul03   .float   6.21
  DBsMul04   .float   12.24
  DBsMul05   .float   20.25
  DBsMul06   .float   30.24
  DBsMul07   .float   42.21
  DBsMul08   .float   56.16
  DBsMul09   .float   72.09
  DBsMul10   .float   81.00
#测试除法指令结果
  DBsDiv01   .float   0.45
  DBsDiv02   .float   0.90
  DBsDiv03   .float   1.35
  DBsDiv04   .float   1.80
  DBsDiv05   .float   2.25
  DBsDiv06   .float   2.7
  DBsDiv07   .float   3.15
  DBsDiv08   .float   3.6
  DBsDiv09   .float   4.05
  DBsDiv10   .float   4.5
#测试除法指令除数
  DBsDived   .float   0.5
#测试平方根指令操作数
  DBsSqrtB01   .float   0.09
  DBsSqrtB02   .float   0.81
  DBsSqrtB03   .float   0.25
#测试平方根指令结果   
  DBsSqrtR01   .float   0.3
  DBsSqrtR02   .float   0.9
  DBsSqrtR03   .float   0.5
#测试倒数指令操作数
  DBsResB01   .float   0.2
  DBsResB02   .float   0.5
  DBsResB03   .float   0.9
#测试倒数指令结果下限  
  DBsResL01   .float   4.999999999
  DBsResL02   .float   1.999999999
  DBsResL03   .float   1.111111111
#测试倒数指令结果上限
  DBsResH01   .float   5.000000001
  DBsResH02   .float   2.000000001
  DBsResH03   .float   1.111111112
#测试平方倒数指令结果上限，对应操作数为DBsSqrtB01-03
  DBsSrReL01   .float   3.333333333
  DBsSrReL02   .float   1.111111111
  DBsSrReL03   .float   1.999999999
#测试平方倒数指令结果下限
  DBsSrReH01   .float   5.000000001
  DBsSrReH02   .float   1.111111112
  DBsSrReH03   .float   2.000000001
#测试乘加指令结果，操作数依次为DBsZero01-03,DBsUno01-03,DBsUno01-03
  DBsMAddR01   .float   0.19
  DBsMAddR02   .float   3.36
  DBsMAddR03   .float   8.51
#测试乘减指令结果，操作数依次为DBsZero01-03,DBsUno01-03,DBsUno01-03
  DBsMSubR01   .float   -0.01
  DBsMSubR02   .float   1.96
  DBsMSubR03   .float   3.91
#测试乘加结果取反指令结果，操作数依次为DBsZero01-03,DBsUno01-03,DBsUno01-03
  DBsNMAddR01   .float   -0.19
  DBsNMAddR02   .float   -3.36
  DBsNMAddR03   .float   -8.51
#测试乘减结果取反指令结果，操作数依次为DBsZero01-03,DBsUno01-03,DBsUno01-03
  DBsNMSubR01   .float   0.01
  DBsNMSubR02   .float   -1.96
  DBsNMSubR03   .float   -3.91
 .endif

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:     PipelineFlush
#  Description: 
#		Introduction of a NOP operation to empty the FPU pipeline.
#  Author:
#  Used module:    None
#  Input Param:
#		num_istr:Number of instructions inserted in the pipeline
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define PipelineFlush(num_istr) \
    .if num_istr > 0 ;\
        nop ;\
        nop ;\
        nop ;\
        nop ;\
        nop ;\
    .endif

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:     FPU1Test
#  Description:   
#       It carries out the logic and arithmetic operation test 
#
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
 .globl FPU1Test

# .section init_section,"ax"

FPU1Test:
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
	
# load interconnect pattern into GPR

########################装载预设精度################     
     lis  r10,DBsAcrcy@h
     ori  r10, r10,DBsAcrcy@l
     lfd f1, 0(r10)
#######################测试加法运算#####################	
#预设运算前数据和结果数据
#装载加法运算结果
    lis  r10,DBsAdd01@h
    ori  r10, r10,DBsAdd01@l
	lfd f10, 0(r10)
	lis  r10,DBsAdd02@h
    ori  r10, r10,DBsAdd02@l
	lfd f11, 0(r10)
    lis  r10,DBsAdd03@h
    ori  r10, r10,DBsAdd03@l
    lfd f12, 0(r10)
    lis  r10,DBsAdd04@h
    ori  r10, r10,DBsAdd04@l
    lfd f13, 0(r10)
    lis  r10,DBsAdd05@h
    ori  r10, r10,DBsAdd05@l
    lfd f14, 0(r10)
    lis  r10,DBsAdd06@h
    ori  r10, r10,DBsAdd06@l
    lfd f15, 0(r10)
    lis  r10,DBsAdd07@h
    ori  r10, r10,DBsAdd07@l
    lfd f16, 0(r10)
    lis  r10,DBsAdd08@h
    ori  r10, r10,DBsAdd08@l
    lfd f17, 0(r10)
    lis  r10,DBsAdd09@h
    ori  r10, r10,DBsAdd09@l
    lfd f18, 0(r10)
    lis  r10,DBsAdd10@h
    ori  r10, r10,DBsAdd10@l
    lfd f19, 0(r10)
#######################测试fadd指令######################	
#装载预设值
	lis  r10,DBsZero01@h
	ori r10, r10, DBsZero01@l
	lfs f20, 0(r10)
	lis  r10,DBsZero02@h
	ori r10, r10, DBsZero02@l
	lfs f21, 0(r10)
	lis  r10,DBsZero03@h
	ori r10, r10, DBsZero03@l
	lfs f22, 0(r10)
	
	lis  r10,DBsUno01@h
	ori r10, r10, DBsUno01@l
	lfs f6, 0(r10)
	lis  r10,DBsUno02@h
	ori r10, r10, DBsUno02@l
	lfs f7, 0(r10)
	lis  r10,DBsUno03@h
	ori r10, r10, DBsUno03@l
	lfs f8, 0(r10)

	PipelineFlush(10)
#测试指令
	fadd f20, f20,f6
	fadd f21, f21,f7
	fadd f22, f22,f8
	
    fsub f20, f20,f10
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test

    fsub f21, f21,f11
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f12
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#######################测试fadd.指令######################
#装载预设值
	lis  r10,DBsZero04@h
	ori r10, r10, DBsZero04@l
	lfs f20, 0(r10)
	lis  r10,DBsZero05@h
	ori r10, r10, DBsZero05@l
	lfs f21, 0(r10)
	lis  r10,DBsZero06@h
	ori r10, r10, DBsZero06@l
	lfs f22, 0(r10)
	
	lis  r10,DBsUno04@h
	ori r10, r10, DBsUno04@l
	lfs f6, 0(r10)
	lis  r10,DBsUno05@h
	ori r10, r10, DBsUno05@l
	lfs f7, 0(r10)
	lis  r10,DBsUno06@h
	ori r10, r10, DBsUno06@l
	lfs f8, 0(r10)
                         
	PipelineFlush (10)
#测试指令
    crxor cr1,cr1,cr1       #清除条件位
	fadd. f20, f20,f6
	bsol cr1, FailFPU1Test
	crxor cr1,cr1,cr1       #清除条件位
	fadd. f21, f21,f7
	bsol cr1, FailFPU1Test
	crxor cr1,cr1,cr1       #清除条件位
	fadd. f22, f22,f8
	bsol cr1,FailFPU1Test
	
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test

    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

	
#######################测试fadds指令######################	
#装载预设值
    lis  r10,DBsZero01@h
    ori r10,r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10,r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10,r10, DBsZero03@l
    lfs f22, 0(r10)
    
    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)
             
    PipelineFlush (10)
#测试指令
    fadds f20, f20,f6
    fadds f21, f21,f7
    fadds f22, f22,f8
    
    fsub f20, f20,f10
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test

    fsub f21, f21,f11
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f12
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test


#######################测试fadds.指令######################	
#装载预设值
    lis  r10,DBsZero04@h
    ori r10, r10, DBsZero04@l
    lfs f20, 0(r10)
    lis  r10,DBsZero05@h
    ori r10, r10, DBsZero05@l
    lfs f21, 0(r10)
    lis  r10,DBsZero06@h
    ori r10, r10, DBsZero06@l
    lfs f22, 0(r10)
    
    lis  r10,DBsUno04@h
    ori r10, r10, DBsUno04@l
    lfs f6, 0(r10)
    lis  r10,DBsUno05@h
    ori r10, r10, DBsUno05@l
    lfs f7, 0(r10)
    lis  r10,DBsUno06@h
    ori r10, r10, DBsUno06@l
    lfs f8, 0(r10)
                         
    PipelineFlush (10)
#测试指令
    crxor cr1,cr1,cr1       #清除条件位
    fadds. f20, f20,f6
    bsol cr1, FailFPU1Test
    crxor cr1,cr1,cr1       #清除条件位
    fadds. f21, f21,f7
    bsol cr1, FailFPU1Test
    crxor cr1,cr1,cr1       #清除条件位
    fadds. f22, f22,f8
    bsol cr1, FailFPU1Test

    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test

    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test


#######################减法指令测试######################
#装载减法运算结果
    lis  r10,DBsSub01@h
    ori r10, r10, DBsSub01@l
	lfd f10, 0(r10)
	lis  r10,DBsSub02@h
    ori r10, r10, DBsSub02@l
	lfd f11, 0(r10)
    lis  r10,DBsSub03@h
    ori r10, r10, DBsSub03@l
    lfd f12, 0(r10)
    lis  r10,DBsSub04@h
    ori r10, r10, DBsSub04@l
    lfd f13, 0(r10)
    lis  r10,DBsSub05@h
    ori r10, r10, DBsSub05@l
    lfd f14, 0(r10)
    lis  r10,DBsSub06@h
    ori r10, r10, DBsSub06@l
    lfd f15, 0(r10)
    lis  r10,DBsSub07@h
    ori r10, r10, DBsSub07@l
    lfd f16, 0(r10)
    lis  r10,DBsSub08@h
    ori r10, r10, DBsSub08@l
    lfd f17, 0(r10)
    lis  r10,DBsSub09@h
    ori r10, r10, DBsSub09@l
    lfd f18, 0(r10)
    lis  r10,DBsSub10@h
    ori r10, r10, DBsSub10@l
    lfd f19, 0(r10)

#######################测试fsub指令######################	
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)
    
    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)
             
    PipelineFlush (10)
#测试指令
    fsub f20, f20,f6
    fsub f21, f21,f7
    fsub f22, f22,f8
    
    fsub f20, f20,f10
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test

    fsub f21, f21,f11
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f12
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#######################测试fsub.指令######################	
#装载预设值
    lis  r10,DBsZero04@h
    ori r10, r10, DBsZero04@l
    lfs f20, 0(r10)
    lis  r10,DBsZero05@h
    ori r10, r10, DBsZero05@l
    lfs f21, 0(r10)
    lis  r10,DBsZero06@h
    ori r10, r10, DBsZero06@l
    lfs f22, 0(r10)
    
    lis  r10,DBsUno04@h
    ori r10, r10, DBsUno04@l
    lfs f6, 0(r10)
    lis  r10,DBsUno05@h
    ori r10, r10, DBsUno05@l
    lfs f7, 0(r10)
    lis  r10,DBsUno06@h
    ori r10, r10, DBsUno06@l
    lfs f8, 0(r10)
                         
    PipelineFlush (10)
#测试指令
    crxor cr1,cr1,cr1       #清除条件位
    fsub. f20, f20, f6
    bsol cr1, FailFPU1Test
    crxor cr1,cr1,cr1       #清除条件位
    fsub. f21, f21, f7
    bsol cr1, FailFPU1Test
    crxor cr1,cr1,cr1       #清除条件位
    fsub. f22, f22, f8
    bsol cr1, FailFPU1Test

    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test


    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#######################测试fsubs指令######################	
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)
    
    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)
             
    PipelineFlush (10)
#测试指令
    fsubs f20, f20,f6
    fsubs f21, f21,f7
    fsubs f22, f22,f8
    
    fsub f20, f20,f10
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test

    fsub f21, f21,f11
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f12
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#######################测试fsubs.指令######################	
#装载预设值
    lis  r10,DBsZero04@h
    ori r10, r10, DBsZero04@l
    lfs f20, 0(r10)
    lis  r10,DBsZero05@h
    ori r10, r10, DBsZero05@l
    lfs f21, 0(r10)
    lis  r10,DBsZero06@h
    ori r10, r10, DBsZero06@l
    lfs f22, 0(r10)
    
    lis  r10,DBsUno04@h
    ori r10, r10, DBsUno04@l
    lfs f6, 0(r10)
    lis  r10,DBsUno05@h
    ori r10, r10, DBsUno05@l
    lfs f7, 0(r10)
    lis  r10,DBsUno06@h
    ori r10, r10, DBsUno06@l
    lfs f8, 0(r10)
                         
    PipelineFlush (10)
#测试指令
    crxor cr1,cr1,cr1       #清除条件位
    fsubs. f20, f20, f6
    bsol cr1, FailFPU1Test
    crxor cr1,cr1,cr1       #清除条件位
    fsubs. f21, f21, f7
    bsol cr1, FailFPU1Test
    crxor cr1,cr1,cr1       #清除条件位
    fsubs. f22, f22, f8
    bsol cr1, FailFPU1Test
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test

    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

###################################乘法指令测试##############################
#装载乘法运算结果
    lis  r10,DBsMul01@h
    ori r10, r10, DBsMul01@l
	lfd f10, 0(r10)
	lis  r10,DBsMul02@h
    ori r10, r10, DBsMul02@l
	lfd f11, 0(r10)
    lis  r10,DBsMul03@h
    ori r10, r10, DBsMul03@l
    lfd f12, 0(r10)
    lis  r10,DBsMul04@h
    ori r10, r10, DBsMul04@l
    lfd f13, 0(r10)
    lis  r10,DBsMul05@h
    ori r10, r10, DBsMul05@l
    lfd f14, 0(r10)
    lis  r10,DBsMul06@h
    ori r10, r10, DBsMul06@l
    lfd f15, 0(r10)
    lis  r10,DBsMul07@h
    ori r10, r10, DBsMul07@l
    lfd f16, 0(r10)
    lis  r10,DBsMul08@h
    ori r10, r10, DBsMul08@l
    lfd f17, 0(r10)
    lis  r10,DBsMul09@h
    ori r10, r10, DBsMul09@l
    lfd f18, 0(r10)
    lis  r10,DBsMul10@h
    ori r10, r10, DBsMul10@l
    lfd f19, 0(r10)

#######################测试fmul指令######################	
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)
    
    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)
             
    PipelineFlush (10)
#测试指令
    fmul f20, f20,f6
    fmul f21, f21,f7
    fmul f22, f22,f8
    
    fsub f20, f20,f10
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test

    fsub f21, f21,f11
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f12
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#######################测试fmul.指令######################	
#装载预设值
    lis  r10,DBsZero04@h
    ori r10, r10, DBsZero04@l
    lfs f20, 0(r10)
    lis  r10,DBsZero05@h
    ori r10, r10, DBsZero05@l
    lfs f21, 0(r10)
    lis  r10,DBsZero06@h
    ori r10, r10, DBsZero06@l
    lfs f22, 0(r10)
    
    lis  r10,DBsUno04@h
    ori r10, r10, DBsUno04@l
    lfs f6, 0(r10)
    lis  r10,DBsUno05@h
    ori r10, r10, DBsUno05@l
    lfs f7, 0(r10)
    lis  r10,DBsUno06@h
    ori r10, r10, DBsUno06@l
    lfs f8, 0(r10)
                         
    PipelineFlush (10)
#测试指令
    crxor cr1,cr1,cr1       #清除条件位
    fmul. f20, f20,f6
    bsol cr1, FailFPU1Test
    crxor cr1,cr1,cr1       #清除条件位
    fmul. f21, f21,f7
    bsol cr1, FailFPU1Test
    crxor cr1,cr1,cr1       #清除条件位
    fmul. f22, f22,f8
    bsol cr1, FailFPU1Test
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test

    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#######################测试fmuls指令######################	
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    PipelineFlush (10)
#测试指令
    fmuls f20, f20,f6
    fmuls f21, f21,f7
    fmuls f22, f22,f8
    
    fsub f20, f20,f10
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test

    fsub f21, f21,f11
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f12
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test
    
#######################测试fmuls.指令######################	

#装载预设值
    lis  r10,DBsZero04@h
    ori r10, r10, DBsZero04@l
    lfs f20, 0(r10)
    lis  r10,DBsZero05@h
    ori r10, r10, DBsZero05@l
    lfs f21, 0(r10)
    lis  r10,DBsZero06@h
    ori r10, r10, DBsZero06@l
    lfs f22, 0(r10)
    
    lis  r10,DBsUno04@h
    ori r10, r10, DBsUno04@l
    lfs f6, 0(r10)
    lis  r10,DBsUno05@h
    ori r10, r10, DBsUno05@l
    lfs f7, 0(r10)
    lis  r10,DBsUno06@h
    ori r10, r10, DBsUno06@l
    lfs f8, 0(r10)
                         
    PipelineFlush (10)
#测试指令
    crxor cr1,cr1,cr1       #清除条件位
    fmuls. f20, f20,f6
    bsol cr1, FailFPU1Test
    crxor cr1,cr1,cr1       #清除条件位
    fmuls. f21, f21,f7
    bsol cr1, FailFPU1Test
    crxor cr1,cr1,cr1       #清除条件位
    fmuls. f22, f22,f8
    bsol cr1, FailFPU1Test
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test

    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#########################除法指令测试###############################
#装载除法运算结果
    lis  r10,DBsDiv01@h
    ori r10, r10, DBsDiv01@l
	lfd f10, 0(r10)
	lis  r10,DBsDiv02@h
    ori r10, r10, DBsDiv02@l
	lfd f11, 0(r10)
    lis  r10,DBsDiv03@h
    ori r10, r10, DBsDiv03@l
    lfd f12, 0(r10)
    lis  r10,DBsDiv04@h
    ori r10, r10, DBsDiv04@l
    lfd f13, 0(r10)
    lis  r10,DBsDiv05@h
    ori r10, r10, DBsDiv05@l
    lfd f14, 0(r10)
    lis  r10,DBsDiv06@h
    ori r10, r10, DBsDiv06@l
    lfd f15, 0(r10)
    lis  r10,DBsDiv07@h
    ori r10, r10, DBsDiv07@l
    lfd f16, 0(r10)
    lis  r10,DBsDiv08@h
    ori r10, r10, DBsDiv08@l
    lfd f17, 0(r10)
    lis  r10,DBsDiv09@h
    ori r10, r10, DBsDiv09@l
    lfd f18, 0(r10)
    lis  r10,DBsDiv10@h
    ori r10, r10, DBsDiv10@l
    lfd f19, 0(r10)
    
#######################测试fdiv指令######################
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)
    
    lis  r10,DBsDived@h
    ori r10, r10, DBsDived@l
    lfs f6, 0(r10)
    fmr f7, f6
    fmr f8, f6
             
    PipelineFlush (10)
#测试指令
    fdiv f20, f20,f6
    fdiv f21, f21,f7
    fdiv f22, f22,f8

    fsub f20, f20,f10
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test

    fsub f21, f21,f11
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f12
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test
    
#######################测试fdiv.指令######################
#装载预设值
 .if 0    
    mffs  f23
    mtfsfi 0,0x0
    mffs  f24
    mtfsfi 1,0x0
    mffs  f25
    mtfsfi 0,0xf
    mffs f26
    mtfsfi 1,0xf
    mffs f27
    mtfsfi 2,0xf
    mffs f28
    mtfsfi 3,0xf
    mffs f29
    mtfsfi 4,0xf
    mffs f30
    mtfsb0 16
    mffs f31
    mtfsf 0xff, f23
    mtfsb0 16
    mtfsb0 6
    mtfsb0 0
  .endif 
  
    lis  r10,DBsZero04@h
    ori  r10, r10, DBsZero04@l
    lfs  f20, 0(r10)
    
    lis  r10,DBsZero05@h
    ori r10, r10, DBsZero05@l
    lfs f21, 0(r10)
    
    lis  r10,DBsZero06@h
    ori r10, r10, DBsZero06@l
    lfs f22, 0(r10)
    
    lis  r10,DBsDived@h
    ori r10, r10, DBsDived@l
    lfs f6, 0(r10)
    fmr f7, f6
    fmr f8, f6
    
    PipelineFlush (10)
#测试指令

#设置溢出标志
#    xor r10, r10, r10
#    lis r10,0x0100
#    mfcr r11
#    or  r11,r11,r10
#    mtcr r11

    fdiv. f20, f20,f6
    bsol cr1, FailFPU1Test

    fdiv. f21, f21,f7
    bsol cr1, FailFPU1Test

    fdiv. f22, f22,f8
    bsol cr1, FailFPU1Test
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test
    
#######################测试fdivs指令######################
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)
    
    lis  r10,DBsDived@h
    ori r10, r10, DBsDived@l
    lfs f6, 0(r10)
    fmr f7, f6
    fmr f8, f6
             
    PipelineFlush (10)
#测试指令
    fdivs f20, f20,f6
    fdivs f21, f21,f7
    fdivs f22, f22,f8
    
    fsub f20, f20,f10
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test

    fsub f21, f21,f11
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f12
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#######################测试fdivs.指令######################	

#装载预设值
    lis  r10,DBsZero04@h
    ori r10, r10, DBsZero04@l
    lfs f20, 0(r10)
    lis  r10,DBsZero05@h
    ori r10, r10, DBsZero05@l
    lfs f21, 0(r10)
    lis  r10,DBsZero06@h
    ori r10, r10, DBsZero06@l
    lfs f22, 0(r10)
    
    lis  r10,DBsDived@h
    ori r10, r10, DBsDived@l
    lfs f6, 0(r10)
    fmr f7, f6
    fmr f8, f6
                         
    PipelineFlush (10)
#测试指令
    crxor cr1,cr1,cr1       #清除条件位
    fdivs. f20, f20,f6
    bsol cr1, FailFPU1Test
    crxor cr1,cr1,cr1       #清除条件位
    fdivs. f21, f21,f7
    bsol cr1, FailFPU1Test
    crxor cr1,cr1,cr1       #清除条件位
    fdivs. f22, f22,f8
    bsol cr1, FailFPU1Test
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test

    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

 .if 0     #编译器不支持下列指令    
#######################测试fsqrt指令######################
#装载平方根操作结果
    lis  r10,DBsSqrtR01@h
    ori r10, r10, DBsSqrtR01@l
	lfd f10, 0(r10)
	lis  r10,DBsSqrtR02@h
    ori r10, r10, DBsSqrtR02@l
	lfd f11, 0(r10)
    lis  r10,DBsSqrtR03@h
    ori r10, r10, DBsSqrtR03@l
    lfd f12, 0(r10)

#装载预设值
    lis  r10,DBsSqrtB01@h
    ori r10, r10, DBsSqrtB01@l
    lfd f20, 0(r10)
    lis  r10,DBsSqrtB02@h
    ori r10, r10, DBsSqrtB02@l
    lfd f21, 0(r10)
    lis  r10,DBsSqrtB03@h
    ori r10, r10, DBsSqrtB03@l
    lfd f22, 0(r10)
        
    PipelineFlush (3)
#测试指令
    fsqrt f20, f20
    fsqrt f21, f21
    fsqrt f22, f22
    fcmpo cr0, f20, f10
    bnel FailFPU1Test
    fcmpo cr0, f21, f11
    bnel FailFPU1Test
    fcmpo cr0, f22, f12
    bnel FailFPU1Test
#######################测试fsqrt.指令######################

#装载预设值
    lis  r10,DBsSqrtB01@h
    ori r10, r10, DBsSqrtB01@l
    lfd f20, 0(r10)
    lis  r10,DBsSqrtB02@h
    ori r10, r10, DBsSqrtB02@l
    lfd f21, 0(r10)
    lis  r10,DBsSqrtB03@h
    ori r10, r10, DBsSqrtB03@l
    lfd f22, 0(r10)
        
    PipelineFlush (3)
#测试指令
    crxor cr0,cr0,cr0       #清除条件位
    fsqrt. f20, f20
    bngl FailFPU1Test
    crxor cr0,cr0,cr0       #清除条件位
    fsqrt. f21, f21
    bngl FailFPU1Test
    crxor cr0,cr0,cr0       #清除条件位
    fsqrt. f22, f22
    bngl FailFPU1Test
    fcmpo cr0, f20, f10
    bnel FailFPU1Test
    fcmpo cr0, f21, f11
    bnel FailFPU1Test
    fcmpo cr0, f22, f12
    bnel FailFPU1Test
    
#######################测试fsqrts指令######################
      
#装载预设值
    lis  r10,DBsSqrtB01@h
    ori r10, r10, DBsSqrtB01@l
    lfs f10, 0(r10)
    lis  r10,DBsSqrtB02@h
    ori r10, r10, DBsSqrtB02@l
    lfs f11, 0(r10)
    lis  r10,DBsSqrtB03@h
    ori r10, r10, DBsSqrtB03@l
    lfs f12, 0(r10)
        
    PipelineFlush (3)
#测试指令
    fsqrts f20, f20
    fsqrts f21, f21
    fsqrts f22, f22
    fcmpo cr0, f20, f10
    bnel FailFPU1Test
    fcmpo cr0, f21, f11
    bnel FailFPU1Test
    fcmpo cr0, f22, f12
    bnel FailFPU1Test

#######################测试fsqrts.指令######################	

#装载预设值
    lis  r10,DBsSqrtB01@h
    ori r10, r10, DBsSqrtB01@l
    lfs f10, 0(r10)
    lis  r10,DBsSqrtB02@h
    ori r10, r10, DBsSqrtB02@l
    lfs f11, 0(r10)
    lis  r10,DBsSqrtB03@h
    ori r10, r10, DBsSqrtB03@l
    lfs f12, 0(r10)
        
    PipelineFlush (3)
#测试指令
    crxor cr0,cr0,cr0       #清除条件位
    fsqrts. f20, f20
    bngl FailFPU1Test
    crxor cr0,cr0,cr0       #清除条件位
    fsqrts. f21, f21
    bngl FailFPU1Test
    crxor cr0,cr0,cr0       #清除条件位
    fsqrts. f22, f22
    bngl FailFPU1Test
    fcmpo cr0, f20, f10
    bnel FailFPU1Test
    fcmpo cr0, f21, f11
    bnel FailFPU1Test
    fcmpo cr0, f22, f12
    bnel FailFPU1Test
 .endif

#######################测试fres指令######################
#装载倒数指令操作结果
    lis  r10,DBsResL01@h
    ori r10, r10, DBsResL01@l
	lfd f10, 0(r10)
	lis  r10,DBsResL02@h
    ori r10, r10, DBsResL02@l
	lfd f11, 0(r10)
    lis  r10,DBsResL03@h
    ori r10, r10, DBsResL03@l
    lfd f12, 0(r10)
    
    lis  r10,DBsResH01@h
    ori r10, r10, DBsResH01@l
	lfd f13, 0(r10)
	lis  r10,DBsResH02@h
    ori r10, r10, DBsResH02@l
	lfd f14, 0(r10)
    lis  r10,DBsResH03@h
    ori r10, r10, DBsResH03@l
    lfd f15, 0(r10)
#装载预设值
    lis  r10,DBsResB01@h
    ori r10, r10, DBsResB01@l
    lfs f20, 0(r10)
    lis  r10,DBsResB02@h
    ori r10, r10, DBsResB02@l
    lfs f21, 0(r10)
    lis  r10,DBsResB03@h
    ori r10, r10, DBsResB03@l
    lfs f22, 0(r10)
    
    PipelineFlush (3)
#测试指令
    fres f20, f20
    fres f21, f21
    fres f22, f22
    fcmpo cr0, f20, f10
    bngl FailFPU1Test
    fcmpo cr0, f21, f11
    bngl FailFPU1Test
    fcmpo cr0, f22, f12
    bngl FailFPU1Test
    fcmpo cr0, f20, f13
    bnll FailFPU1Test
    fcmpo cr0, f21, f14
    bnll FailFPU1Test
    fcmpo cr0, f22, f15
    bnll FailFPU1Test
    
#######################测试fres.指令######################

#装载预设值
    lis  r10,DBsResB01@h
    ori r10, r10, DBsResB01@l
    lfs f20, 0(r10)
    lis  r10,DBsResB02@h
    ori r10, r10, DBsResB02@l
    lfs f21, 0(r10)
    lis  r10,DBsResB03@h
    ori r10, r10, DBsResB03@l
    lfs f22, 0(r10)
    
    PipelineFlush (3)
#测试指令

    fres. f20, f20
    bsol cr1, FailFPU1Test

    fres. f21, f21
    bsol cr1, FailFPU1Test

    fres. f22, f22
    bsol cr1, FailFPU1Test
    
    fcmpo cr0, f20, f10
    bngl FailFPU1Test
    
    fcmpo cr0, f21, f11
    bngl FailFPU1Test
    
    fcmpo cr0, f22, f12
    bngl FailFPU1Test
    
    fcmpo cr0, f20, f13
    bnll FailFPU1Test
    
    fcmpo cr0, f21, f14
    bnll FailFPU1Test
    
    fcmpo cr0, f22, f15
    bnll FailFPU1Test
    
#######################测试frsqrte指令######################
#装载平方根倒数指令操作结果
    lis  r10,DBsSrReL01@h
    ori r10, r10, DBsSrReL01@l
    lfd f10, 0(r10)
    lis  r10,DBsSrReL02@h
    ori r10, r10, DBsSrReL02@l
    lfd f11, 0(r10)
    lis  r10,DBsSrReL03@h
    ori r10, r10, DBsSrReL03@l
    lfd f12, 0(r10)
    
    lis  r10,DBsSrReH01@h
    ori r10, r10, DBsSrReH01@l
    lfd f13, 0(r10)
    lis  r10,DBsSrReH02@h
    ori r10, r10, DBsSrReH02@l
    lfd f14, 0(r10)
    lis  r10,DBsSrReH03@h
    ori r10, r10, DBsSrReH03@l
    lfd f15, 0(r10)
       
#装载预设值
    lis  r10,DBsSqrtB01@h
    ori r10, r10, DBsSqrtB01@l
    lfd f20, 0(r10)
    lis  r10,DBsSqrtB02@h
    ori r10, r10, DBsSqrtB02@l
    lfd f21, 0(r10)
    lis  r10,DBsSqrtB03@h
    ori r10, r10, DBsSqrtB03@l
    lfd f22, 0(r10)
    
    PipelineFlush (3)
#测试指令
    frsqrte f20, f20
    frsqrte f21, f21
    frsqrte f22, f22
    
    fcmpo cr0, f20, f10
    bngl FailFPU1Test
    
    fcmpo cr0, f21, f11
    bngl FailFPU1Test
    
    fcmpo cr0, f22, f12
    bngl FailFPU1Test
    
    fcmpo cr0, f20, f13
    bnll FailFPU1Test
    
    fcmpo cr0, f21, f14
    bnll FailFPU1Test
    
    fcmpo cr0, f22, f15
    bnll FailFPU1Test
    
#######################测试frsqrte.指令######################	
       
#装载预设值
    lis  r10,DBsSqrtB01@h
    ori r10, r10, DBsSqrtB01@l
    lfd f20, 0(r10)
    lis  r10,DBsSqrtB02@h
    ori r10, r10, DBsSqrtB02@l
    lfd f21, 0(r10)
    lis  r10,DBsSqrtB03@h
    ori r10, r10, DBsSqrtB03@l
    lfd f22, 0(r10)
    
    PipelineFlush (3)
#测试指令

    frsqrte. f20, f20
    bsol cr1, FailFPU1Test

    frsqrte. f21, f21
    bsol cr1, FailFPU1Test

    frsqrte. f22, f22
    bsol cr1, FailFPU1Test
    
    fcmpo cr0, f20, f10
    bngl FailFPU1Test
    
    fcmpo cr0, f21, f11
    bngl FailFPU1Test
    
    fcmpo cr0, f22, f12
    bngl FailFPU1Test
    
    fcmpo cr0, f20, f13
    bnll FailFPU1Test
    
    fcmpo cr0, f21, f14
    bnll FailFPU1Test
    
    fcmpo cr0, f22, f15
    bnll FailFPU1Test
    
#####################测试fsel指令########################
#装载选择指令操作结果
    lis  r10,DBsSrReL01@h
    ori r10, r10, DBsSrReL01@l
    lfd f10, 0(r10)
    lis  r10,DBsSrReL02@h
    ori r10, r10, DBsSrReL02@l
    lfd f11, 0(r10)
    lis  r10,DBsSrReL03@h
    ori r10, r10, DBsSrReL03@l
    lfd f12, 0(r10)
#装载预设值
    lis  r10,DBsSub04@h
    ori r10, r10, DBsSub04@l
    lfs f20, 0(r10)
    lis  r10,DBsSub05@h
    ori r10, r10, DBsSub05@l
    lfs f21, 0(r10)
    lis  r10,DBsSub06@h
    ori r10, r10, DBsSub06@l
    lfs f22, 0(r10)
    
    lis  r10,DBsSrReL01@h
    ori r10, r10, DBsSrReL01@l
    lfs f6, 0(r10)
    lis  r10,DBsSrReL02@h
    ori r10, r10, DBsSrReL02@l
    lfs f7, 0(r10)
    lis  r10,DBsSrReL03@h
    ori r10, r10, DBsSrReL03@l
    lfs f8, 0(r10)
    
    lis  r10,DBsSrReH01@h
    ori r10, r10, DBsSrReH01@l
    lfs f9, 0(r10)
    lis  r10,DBsSrReH02@h
    ori r10, r10, DBsSrReH02@l
    lfs f10, 0(r10)
    lis  r10,DBsSrReH03@h
    ori r10, r10, DBsSrReH03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令
    fsel f20,f20,f6,f10
    fsel f21,f21,f7,f11
    fsel f22,f22,f8,f12
    
    fcmpo cr0, f20, f6
    bnel FailFPU1Test
    
    fcmpo cr0, f21, f7
    bnel FailFPU1Test
    
    fcmpo cr0, f22, f12
    bnel FailFPU1Test
    
#########################测试fmadd指令##################
#装载乘加指令操作结果
    lis  r10,DBsMAddR01@h
    ori r10, r10, DBsMAddR01@l
    lfd f13, 0(r10)
    lis  r10,DBsMAddR02@h
    ori r10, r10, DBsMAddR02@l
    lfd f14, 0(r10)
    lis  r10,DBsMAddR03@h
    ori r10, r10, DBsMAddR03@l
    lfd f15, 0(r10)
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f10, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f11, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f12, 0(r10)
    PipelineFlush (9)
#测试指令
    fmadd f20,f20,f6,f10
    fmadd f21,f21,f7,f11
    fmadd f22,f22,f8,f12
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test
    
#########################测试fmadd.指令#################
#装载乘加指令操作结果
    lis  r10,DBsMAddR01@h
    ori r10, r10, DBsMAddR01@l
    lfd f13, 0(r10)
    lis  r10,DBsMAddR02@h
    ori r10, r10, DBsMAddR02@l
    lfd f14, 0(r10)
    lis  r10,DBsMAddR03@h
    ori r10, r10, DBsMAddR03@l
    lfd f15, 0(r10)
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令
    crxor cr1,cr1,cr1       #清除条件位
    fmadd. f20,f20,f6,f9
    bsol cr1, FailFPU1Test
    crxor cr1,cr1,cr1       #清除条件位
    fmadd. f21,f21,f7,f10
    bsol cr1, FailFPU1Test
    crxor cr1,cr1,cr1       #清除条件位
    fmadd. f22,f22,f8,f11
    bsol cr1, FailFPU1Test
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#########################测试fmadds指令#################
#装载乘加单精度指令操作结果
    lis  r10,DBsMAddR01@h
    ori r10, r10, DBsMAddR01@l
    lfd f13, 0(r10)
    lis  r10,DBsMAddR02@h
    ori r10, r10, DBsMAddR02@l
    lfd f14, 0(r10)
    lis  r10,DBsMAddR03@h
    ori r10, r10, DBsMAddR03@l
    lfd f15, 0(r10)
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令
    fmadds f20,f20,f6,f9
    fmadds f21,f21,f7,f10
    fmadds f22,f22,f8,f11
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#########################测试fmadds.指令################
#装载乘加单精度指令操作结果
    lis  r10,DBsMAddR01@h
    ori r10, r10, DBsMAddR01@l
    lfd f13, 0(r10)
    lis  r10,DBsMAddR02@h
    ori r10, r10, DBsMAddR02@l
    lfd f14, 0(r10)
    lis  r10,DBsMAddR03@h
    ori r10, r10, DBsMAddR03@l
    lfd f15, 0(r10)
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令

    fmadds. f20,f20,f6,f9
    bsol cr1, FailFPU1Test

    fmadds. f21,f21,f7,f10
    bsol cr1, FailFPU1Test

    fmadds. f22,f22,f8,f11
    bsol cr1, FailFPU1Test
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#########################测试fmsub指令##################
#装载乘减指令操作结果
    lis  r10,DBsMSubR01@h
    ori r10, r10, DBsMSubR01@l
    lfd f13, 0(r10)
    lis  r10,DBsMSubR02@h
    ori r10, r10, DBsMSubR02@l
    lfd f14, 0(r10)
    lis  r10,DBsMSubR03@h
    ori r10, r10, DBsMSubR03@l
    lfd f15, 0(r10)
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令
    fmsub f20,f20,f6,f9
    fmsub f21,f21,f7,f10
    fmsub f22,f22,f8,f11
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#########################测试fmsub.指令#################
#装载乘减指令操作结果
    lis  r10,DBsMSubR01@h
    ori r10, r10, DBsMSubR01@l
    lfd f13, 0(r10)
    lis  r10,DBsMSubR02@h
    ori r10, r10, DBsMSubR02@l
    lfd f14, 0(r10)
    lis  r10,DBsMSubR03@h
    ori r10, r10, DBsMSubR03@l
    lfd f15, 0(r10)
#装载操作数值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令

    fmsub. f20,f20,f6,f9
    bsol cr1, FailFPU1Test

    fmsub. f21,f21,f7,f10
    bsol cr1, FailFPU1Test

    fmsub. f22,f22,f8,f11
    bsol cr1, FailFPU1Test
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#########################测试fmsubs指令#################
#装载乘减指令操作结果
    lis  r10,DBsMSubR01@h
    ori r10, r10, DBsMSubR01@l
    lfd f13, 0(r10)
    lis  r10,DBsMSubR02@h
    ori r10, r10, DBsMSubR02@l
    lfd f14, 0(r10)
    lis  r10,DBsMSubR03@h
    ori r10, r10, DBsMSubR03@l
    lfd f15, 0(r10)
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令
    fmsubs f20,f20,f6,f9
    fmsubs f21,f21,f7,f10
    fmsubs f22,f22,f8,f11
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test
    
#########################测试fmsubs.指令################
#装载乘加指令操作结果
    lis  r10,DBsMSubR01@h
    ori r10, r10, DBsMSubR01@l
    lfd f13, 0(r10)
    lis  r10,DBsMSubR02@h
    ori r10, r10, DBsMSubR02@l
    lfd f14, 0(r10)
    lis  r10,DBsMSubR03@h
    ori r10, r10, DBsMSubR03@l
    lfd f15, 0(r10)
#装载操作数值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令

    fmsubs. f20,f20,f6,f9
    bsol cr1, FailFPU1Test

    fmsubs. f21,f21,f7,f10
    bsol cr1, FailFPU1Test

    fmsubs. f22,f22,f8,f11
    bsol cr1, FailFPU1Test
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#########################测试fnmadd指令#################
#装载乘加取反指令操作结果
    lis  r10,DBsNMAddR01@h
    ori r10, r10, DBsNMAddR01@l
    lfd f13, 0(r10)
    lis  r10,DBsNMAddR02@h
    ori r10, r10, DBsNMAddR02@l
    lfd f14, 0(r10)
    lis  r10,DBsNMAddR03@h
    ori r10, r10, DBsNMAddR03@l
    lfd f15, 0(r10)
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令
    fnmadd f20,f20,f6,f9
    fnmadd f21,f21,f7,f10
    fnmadd f22,f22,f8,f11
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test
    
#########################测试fnmadd.指令################
#装载乘加指令操作结果
    lis  r10,DBsNMAddR01@h
    ori r10, r10, DBsNMAddR01@l
    lfd f13, 0(r10)
    lis  r10,DBsNMAddR02@h
    ori r10, r10, DBsNMAddR02@l
    lfd f14, 0(r10)
    lis  r10,DBsNMAddR03@h
    ori r10, r10, DBsNMAddR03@l
    lfd f15, 0(r10)
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令

    fnmadd. f20,f20,f6,f9
    bsol cr1, FailFPU1Test

    fnmadd. f21,f21,f7,f10
    bsol cr1, FailFPU1Test

    fnmadd. f22,f22,f8,f11
    bsol cr1, FailFPU1Test
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#########################测试fnmadds指令################
#装载乘加单精度指令操作结果
    lis  r10,DBsNMAddR01@h
    ori r10, r10, DBsNMAddR01@l
    lfd f13, 0(r10)
    lis  r10,DBsNMAddR02@h
    ori r10, r10, DBsNMAddR02@l
    lfd f14, 0(r10)
    lis  r10,DBsNMAddR03@h
    ori r10, r10, DBsNMAddR03@l
    lfd f15, 0(r10)
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令
    fnmadds f20,f20,f6,f9
    fnmadds f21,f21,f7,f10
    fnmadds f22,f22,f8,f11
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#########################测试fnmadds.指令###############
#装载乘加取反单精度指令操作结果
    lis  r10,DBsNMAddR01@h
    ori r10, r10, DBsNMAddR01@l
    lfd f13, 0(r10)
    lis  r10,DBsNMAddR02@h
    ori r10, r10, DBsNMAddR02@l
    lfd f14, 0(r10)
    lis  r10,DBsNMAddR03@h
    ori r10, r10, DBsNMAddR03@l
    lfd f15, 0(r10)
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令

    fnmadds. f20,f20,f6,f9
    bsol cr1, FailFPU1Test

    fnmadds. f21,f21,f7,f10
    bsol cr1, FailFPU1Test

    fnmadds. f22,f22,f8,f11
    bsol cr1, FailFPU1Test
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#########################测试fnmsub指令#################
#装载乘减指令操作结果
    lis  r10,DBsNMSubR01@h
    ori r10, r10, DBsNMSubR01@l
    lfd f13, 0(r10)
    lis  r10,DBsNMSubR02@h
    ori r10, r10, DBsNMSubR02@l
    lfd f14, 0(r10)
    lis  r10,DBsNMSubR03@h
    ori r10, r10, DBsNMSubR03@l
    lfd f15, 0(r10)
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令
    fnmsub f20,f20,f6,f9
    fnmsub f21,f21,f7,f10
    fnmsub f22,f22,f8,f11
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#########################测试fnmsub.指令################
#装载乘减指令操作结果
    lis  r10,DBsNMSubR01@h
    ori r10, r10, DBsNMSubR01@l
    lfd f13, 0(r10)
    lis  r10,DBsNMSubR02@h
    ori r10, r10, DBsNMSubR02@l
    lfd f14, 0(r10)
    lis  r10,DBsNMSubR03@h
    ori r10, r10, DBsNMSubR03@l
    lfd f15, 0(r10)
#装载操作数值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令

    fnmsub. f20,f20,f6,f9
    bsol cr1, FailFPU1Test

    fnmsub. f21,f21,f7,f10
    bsol cr1, FailFPU1Test

    fnmsub. f22,f22,f8,f11
    bsol cr1, FailFPU1Test
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#########################测试fnmsubs指令################
#装载乘减指令操作结果
    lis  r10,DBsNMSubR01@h
    ori r10, r10, DBsNMSubR01@l
    lfd f13, 0(r10)
    lis  r10,DBsNMSubR02@h
    ori r10, r10, DBsNMSubR02@l
    lfd f14, 0(r10)
    lis  r10,DBsNMSubR03@h
    ori r10, r10, DBsNMSubR03@l
    lfd f15, 0(r10)
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令
    fnmsubs f20,f20,f6,f9
    fnmsubs f21,f21,f7,f10
    fnmsubs f22,f22,f8,f11
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test

#########################测试fnmsubs.指令###############
#装载乘加指令操作结果
    lis  r10,DBsNMSubR01@h
    ori r10, r10, DBsNMSubR01@l
    lfd f13, 0(r10)
    lis  r10,DBsNMSubR02@h
    ori r10, r10, DBsNMSubR02@l
    lfd f14, 0(r10)
    lis  r10,DBsNMSubR03@h
    ori r10, r10, DBsNMSubR03@l
    lfd f15, 0(r10)
#装载预设值
    lis  r10,DBsZero01@h
    ori r10, r10, DBsZero01@l
    lfs f20, 0(r10)
    lis  r10,DBsZero02@h
    ori r10, r10, DBsZero02@l
    lfs f21, 0(r10)
    lis  r10,DBsZero03@h
    ori r10, r10, DBsZero03@l
    lfs f22, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f6, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f7, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f8, 0(r10)

    lis  r10,DBsUno01@h
    ori r10, r10, DBsUno01@l
    lfs f9, 0(r10)
    lis  r10,DBsUno02@h
    ori r10, r10, DBsUno02@l
    lfs f10, 0(r10)
    lis  r10,DBsUno03@h
    ori r10, r10, DBsUno03@l
    lfs f11, 0(r10)
    PipelineFlush (9)
#测试指令

    fnmsubs. f20,f20,f6,f9
    bsol cr1, FailFPU1Test

    fnmsubs. f21,f21,f7,f10
    bsol cr1, FailFPU1Test

    fnmsubs. f22,f22,f8,f11
    bsol cr1, FailFPU1Test
    
    fsub f20, f20,f13
    fabs f20, f20
	fcmpo cr0, f20, f1
	bgel FailFPU1Test


    fsub f21, f21,f14
    fabs f21, f21
    fcmpo cr0, f21, f1
    bgel FailFPU1Test

    fsub f22, f22,f15
    fabs f22, f22
    fcmpo cr0, f22, f1
    bgel FailFPU1Test


# .section return_code, "ax"
     
    TestConclusion
    b EndFPU1Test
FailFPU1Test:
    ErrorHandling
        
EndFPU1Test:
    StackEpilogBegin
    RestoreFpscr
    RestoreCtrXerCr
    RestoreFPRs
    StackEpilogEnd

 blr                        
