#include "../inc/common_macro.s"
#include "../inc/common_def.h"

 .if 0
  MarchDBs0: 
            .double   4.503599627370480 
  MarchDBs1: 
            .double   -1.5E+17
 .endif
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march0
#  Description:    
#       write 0x0000 to gpr0-31
#  Author:
#  Used module:    None
#  Input Param:
#		FPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march0
     lfd f0, 0(r3)
     lfd f1, 0(r3)
     lfd f2, 0(r3)
     lfd f3, 0(r3)
     lfd f4, 0(r3)
     lfd f5, 0(r3)
     lfd f6, 0(r3)
     lfd f7, 0(r3)
     lfd f8, 0(r3)
     lfd f9, 0(r3)
     lfd f10, 0(r3)
     lfd f11, 0(r3)
     lfd f12, 0(r3)
     lfd f13, 0(r3)
     lfd f14, 0(r3)
     lfd f15, 0(r3)
     lfd f16, 0(r3)
     lfd f17, 0(r3)
     lfd f18, 0(r3)
     lfd f19, 0(r3)
     lfd f20, 0(r3)
     lfd f21, 0(r3)
     lfd f22, 0(r3)
     lfd f23, 0(r3)
     lfd f24, 0(r3)
     lfd f25, 0(r3)
     lfd f26, 0(r3)
     lfd f27, 0(r3)
     lfd f28, 0(r3)
     lfd f29, 0(r3)
     lfd f30, 0(r3)
     lfd f31, 0(r3) 
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march1
#  Description:    
#       check fpr through sequence of March1:↓(r0,r0,w0,r0,w1),  compare with gpr0
#  Author:
#  Used module:    None
#  Input Param:
#		FPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march1
     fcmpo cr0,f31, f0                  #比较FPRG31与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f31, f0
     bnel fail_fpr_test_interword
     lfd f31, 0(r3)
     fcmpo f31, f0 
     bnel fail_fpr_test_interword
     lfd f31, 0(r4)

     fcmpo cr0,f30, f0                  #比较FPRG30与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f30, f0
     bnel fail_fpr_test_interword
     lfd f30, 0(r3)
     fcmpo f30, f0 
     bnel fail_fpr_test_interword
     lfd f30, 0(r4)
     
     fcmpo cr0,f29, f0                  #比较FPRG29与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f29, f0
     bnel fail_fpr_test_interword
     lfd f29, 0(r3)
     fcmpo f29, f0 
     bnel fail_fpr_test_interword
     lfd f29, 0(r4)
     
     fcmpo cr0,f28, f0                  #比较FPRG28与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f28, f0
     bnel fail_fpr_test_interword
     lfd f28, 0(r3)
     fcmpo f28, f0 
     bnel fail_fpr_test_interword
     lfd f28, 0(r4)
     
     
     fcmpo cr0,f27, f0                  #比较FPRG27与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f27, f0
     bnel fail_fpr_test_interword
     lfd f27, 0(r3)
     fcmpo f27, f0 
     bnel fail_fpr_test_interword
     lfd f27, 0(r4)

     
     fcmpo cr0,f26, f0                  #比较FPRG26与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f26, f0
     bnel fail_fpr_test_interword
     lfd f26, 0(r3)
     fcmpo f26, f0 
     bnel fail_fpr_test_interword
     lfd f26, 0(r4)

     
     fcmpo cr0,f25, f0                  #比较FPRG25与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f25, f0
     bnel fail_fpr_test_interword
     lfd f25, 0(r3)
     fcmpo f25, f0 
     bnel fail_fpr_test_interword
     lfd f25, 0(r4)

     
     fcmpo cr0,f24, f0                  #比较FPRG24与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f24, f0
     bnel fail_fpr_test_interword
     lfd f24, 0(r3)
     fcmpo f24, f0 
     bnel fail_fpr_test_interword
     lfd f24, 0(r4)

     
     fcmpo cr0,f23, f0                  #比较FPRG23与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f23, f0
     bnel fail_fpr_test_interword
     lfd f23, 0(r3)
     fcmpo f23, f0 
     bnel fail_fpr_test_interword
     lfd f23, 0(r4)

     
     fcmpo cr0,f22, f0                  #比较FPRG22与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f22, f0
     bnel fail_fpr_test_interword
     lfd f22, 0(r3)
     fcmpo f22, f0 
     bnel fail_fpr_test_interword
     lfd f22, 0(r4)

     
     fcmpo cr0,f21, f0                  #比较FPRG21与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f21, f0
     bnel fail_fpr_test_interword
     lfd f21, 0(r3)
     fcmpo f21, f0 
     bnel fail_fpr_test_interword
     lfd f21, 0(r4)

     
     fcmpo cr0,f20, f0                  #比较FPRG20与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f20, f0
     bnel fail_fpr_test_interword
     lfd f20, 0(r3)
     fcmpo f20, f0 
     bnel fail_fpr_test_interword
     lfd f20, 0(r4)

     
     fcmpo cr0,f19, f0                  #比较FPRG19与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f19, f0
     bnel fail_fpr_test_interword
     lfd f19, 0(r3)
     fcmpo f19, f0 
     bnel fail_fpr_test_interword
     lfd f19, 0(r4)

     
     fcmpo cr0,f18, f0                  #比较FPRG18与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f18, f0
     bnel fail_fpr_test_interword
     lfd f18, 0(r3)
     fcmpo f18, f0 
     bnel fail_fpr_test_interword
     lfd f18, 0(r4)

     
     fcmpo cr0,f17, f0                  #比较FPRG17与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f17, f0
     bnel fail_fpr_test_interword
     lfd f17, 0(r3)
     fcmpo f17, f0 
     bnel fail_fpr_test_interword
     lfd f17, 0(r4)

     
     fcmpo cr0,f16, f0                  #比较FPRG16与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f16, f0
     bnel fail_fpr_test_interword
     lfd f16, 0(r3)
     fcmpo f16, f0 
     bnel fail_fpr_test_interword
     lfd f16, 0(r4)

     
     fcmpo cr0,f15, f0                  #比较FPRG15与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f15, f0
     bnel fail_fpr_test_interword
     lfd f15, 0(r3)
     fcmpo f15, f0 
     bnel fail_fpr_test_interword
     lfd f15, 0(r4)
     
     fcmpo cr0,f14, f0                  #比较FPRG14与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f14, f0
     bnel fail_fpr_test_interword
     lfd f14, 0(r3)
     fcmpo f14, f0 
     bnel fail_fpr_test_interword
     lfd f14, 0(r4)
     
     fcmpo cr0,f13, f0                  #比较FPRG13与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f13, f0
     bnel fail_fpr_test_interword
     lfd f13, 0(r3)
     fcmpo f13, f0 
     bnel fail_fpr_test_interword
     lfd f13, 0(r4)
     
     fcmpo cr0,f12, f0                  #比较FPRG12与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f12, f0
     bnel fail_fpr_test_interword
     lfd f12, 0(r3)
     fcmpo f12, f0 
     bnel fail_fpr_test_interword
     lfd f12, 0(r4)
     
     fcmpo cr0,f11, f0                  #比较FPRG11与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f11, f0
     bnel fail_fpr_test_interword
     lfd f11, 0(r3)
     fcmpo f11, f0 
     bnel fail_fpr_test_interword
     lfd f11, 0(r4)
     
     fcmpo cr0,f10, f0                  #比较FPRG10与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f10, f0
     bnel fail_fpr_test_interword
     lfd f10, 0(r3)
     fcmpo f10, f0 
     bnel fail_fpr_test_interword
     lfd f10, 0(r4)
     
     fcmpo cr0,f9, f0                  #比较FPRG9与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f9, f0
     bnel fail_fpr_test_interword
     lfd f9, 0(r3)
     fcmpo f9, f0 
     bnel fail_fpr_test_interword
     lfd f9, 0(r4)
     
     fcmpo cr0,f8, f0                  #比较FPRG8与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f8, f0
     bnel fail_fpr_test_interword
     lfd f8, 0(r3)
     fcmpo f8, f0 
     bnel fail_fpr_test_interword
     lfd f8, 0(r4)
     
     fcmpo cr0,f7, f0                  #比较FPRG7与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f7, f0
     bnel fail_fpr_test_interword
     lfd f7, 0(r3)
     fcmpo f7, f0 
     bnel fail_fpr_test_interword
     lfd f7, 0(r4)
     
     fcmpo cr0,f6, f0                  #比较FPRG6与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f6, f0
     bnel fail_fpr_test_interword
     lfd f6, 0(r3)
     fcmpo f6, f0 
     bnel fail_fpr_test_interword
     lfd f6, 0(r4)
     
     fcmpo cr0,f5, f0                  #比较FPRG5与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f5, f0
     bnel fail_fpr_test_interword
     lfd f5, 0(r3)
     fcmpo f5, f0 
     bnel fail_fpr_test_interword
     lfd f5, 0(r4)
     
     fcmpo cr0,f4, f0                  #比较FPRG4与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f4, f0
     bnel fail_fpr_test_interword
     lfd f4, 0(r3)
     fcmpo f4, f0 
     bnel fail_fpr_test_interword
     lfd f4, 0(r4)
     
     fcmpo cr0,f3, f0                  #比较FPRG3与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f3, f0
     bnel fail_fpr_test_interword
     lfd f3, 0(r3)
     fcmpo f3, f0 
     bnel fail_fpr_test_interword
     lfd f3, 0(r4)
     
     fcmpo cr0,f2, f0                  #比较FPRG2与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f2, f0
     bnel fail_fpr_test_interword
     lfd f2, 0(r3)
     fcmpo f2, f0 
     bnel fail_fpr_test_interword
     lfd f2, 0(r4)
     
     fcmpo cr0,f1, f0                  #比较FPRG1与FPRG0值
     bnel fail_fpr_test_interword
     fcmpo f1, f0
     bnel fail_fpr_test_interword
     lfd f1, 0(r3)
     fcmpo f1, f0 
     bnel fail_fpr_test_interword
     lfd f1, 0(r4)

     lfd f0, 0(r4)
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march2
#  Description:    
#       check fpr through sequence of March2:↑(r1,r1,w1,r1,w0),  compare with gpr1
#  Author:
#  Used module:    None
#  Input Param:
#		FPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march2
     fcmpo cr0,f0, f1                  #比较FPRG0与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f0, f1
     bnel fail_fpr_test_interword
     lfd f0, 0(r4)
     fcmpo f0, f1 
     bnel fail_fpr_test_interword
     lfd f0, 0(r3)

     fcmpo cr0,f2, f1                  #比较FPRG2与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f2, f1
     bnel fail_fpr_test_interword
     lfd f2, 0(r4)
     fcmpo f2, f1 
     bnel fail_fpr_test_interword
     lfd f2, 0(r3)
     
     fcmpo cr0,f3, f1                  #比较FPRG3与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f3, f1
     bnel fail_fpr_test_interword
     lfd f3, 0(r4)
     fcmpo f3, f1 
     bnel fail_fpr_test_interword
     lfd f3, 0(r3)
     
     fcmpo cr0,f4, f1                  #比较FPRG4与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f4, f1
     bnel fail_fpr_test_interword
     lfd f4, 0(r4)
     fcmpo f4, f1 
     bnel fail_fpr_test_interword
     lfd f4, 0(r3)
     
     fcmpo cr0,f5, f1                  #比较FPRG5与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f5, f1
     bnel fail_fpr_test_interword
     lfd f5, 0(r4)
     fcmpo f5, f1 
     bnel fail_fpr_test_interword
     lfd f5, 0(r3)
     
     fcmpo cr0,f6, f1                  #比较FPRG6与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f6, f1
     bnel fail_fpr_test_interword
     lfd f6, 0(r4)
     fcmpo f6, f1 
     bnel fail_fpr_test_interword
     lfd f6, 0(r3)
     
     fcmpo cr0,f7, f1                  #比较FPRG7与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f7, f1
     bnel fail_fpr_test_interword
     lfd f7, 0(r4)
     fcmpo f7, f1 
     bnel fail_fpr_test_interword
     lfd f7, 0(r3)
     
     fcmpo cr0,f8, f1                  #比较FPRG8与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f8, f1
     bnel fail_fpr_test_interword
     lfd f8, 0(r4)
     fcmpo f8, f1 
     bnel fail_fpr_test_interword
     lfd f8, 0(r3)
     
     fcmpo cr0,f9, f1                  #比较FPRG9与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f9, f1
     bnel fail_fpr_test_interword
     lfd f9, 0(r4)
     fcmpo f9, f1 
     bnel fail_fpr_test_interword
     lfd f9, 0(r3)
     
     fcmpo cr0,f10, f1                  #比较FPRG10与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f10, f1
     bnel fail_fpr_test_interword
     lfd f10, 0(r4)
     fcmpo f10, f1 
     bnel fail_fpr_test_interword
     lfd f10, 0(r3)
     
     fcmpo cr0,f11, f1                  #比较FPRG11与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f11, f1
     bnel fail_fpr_test_interword
     lfd f11, 0(r4)
     fcmpo f11, f1 
     bnel fail_fpr_test_interword
     lfd f11, 0(r3)
     
     fcmpo cr0,f12, f1                  #比较FPRG12与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f12, f1
     bnel fail_fpr_test_interword
     lfd f12, 0(r4)
     fcmpo f12, f1 
     bnel fail_fpr_test_interword
     lfd f12, 0(r3)
     
     fcmpo cr0,f13, f1                  #比较FPRG13与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f13, f1
     bnel fail_fpr_test_interword
     lfd f13, 0(r4)
     fcmpo f13, f1 
     bnel fail_fpr_test_interword
     lfd f13, 0(r3)
     
     fcmpo cr0,f14, f1                  #比较FPRG14与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f14, f1
     bnel fail_fpr_test_interword
     lfd f14, 0(r4)
     fcmpo f14, f1 
     bnel fail_fpr_test_interword
     lfd f14, 0(r3)
     
     fcmpo cr0,f15, f1                  #比较FPRG15与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f15, f1
     bnel fail_fpr_test_interword
     lfd f15, 0(r4)
     fcmpo f15, f1 
     bnel fail_fpr_test_interword
     lfd f15, 0(r3)
     
     fcmpo cr0,f16, f1                  #比较FPRG16与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f16, f1
     bnel fail_fpr_test_interword
     lfd f16, 0(r4)
     fcmpo f16, f1 
     bnel fail_fpr_test_interword
     lfd f16, 0(r3)
     
     fcmpo cr0,f17, f1                  #比较FPRG17与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f17, f1
     bnel fail_fpr_test_interword
     lfd f17, 0(r4)
     fcmpo f17, f1 
     bnel fail_fpr_test_interword
     lfd f17, 0(r3)
     
     fcmpo cr0,f18, f1                  #比较FPRG18与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f18, f1
     bnel fail_fpr_test_interword
     lfd f18, 0(r4)
     fcmpo f18, f1 
     bnel fail_fpr_test_interword
     lfd f18, 0(r3)
     
     fcmpo cr0,f19, f1                  #比较FPRG19与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f19, f1
     bnel fail_fpr_test_interword
     lfd f19, 0(r4)
     fcmpo f19, f1 
     bnel fail_fpr_test_interword
     lfd f19, 0(r3)
     
     fcmpo cr0,f20, f1                  #比较FPRG20与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f20, f1
     bnel fail_fpr_test_interword
     lfd f20, 0(r4)
     fcmpo f20, f1 
     bnel fail_fpr_test_interword
     lfd f20, 0(r3)
     
     fcmpo cr0,f21, f1                  #比较FPRG21与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f21, f1
     bnel fail_fpr_test_interword
     lfd f21, 0(r4)
     fcmpo f21, f1 
     bnel fail_fpr_test_interword
     lfd f21, 0(r3)
     
     fcmpo cr0,f22, f1                  #比较FPRG22与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f22, f1
     bnel fail_fpr_test_interword
     lfd f22, 0(r4)
     fcmpo f22, f1 
     bnel fail_fpr_test_interword
     lfd f22, 0(r3)
     
     fcmpo cr0,f23, f1                  #比较FPRG23与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f23, f1
     bnel fail_fpr_test_interword
     lfd f23, 0(r4)
     fcmpo f23, f1 
     bnel fail_fpr_test_interword
     lfd f23, 0(r3)
     
     fcmpo cr0,f24, f1                  #比较FPRG24与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f24, f1
     bnel fail_fpr_test_interword
     lfd f24, 0(r4)
     fcmpo f24, f1 
     bnel fail_fpr_test_interword
     lfd f24, 0(r3)
     
     fcmpo cr0,f25, f1                  #比较FPRG25与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f25, f1
     bnel fail_fpr_test_interword
     lfd f25, 0(r4)
     fcmpo f25, f1 
     bnel fail_fpr_test_interword
     lfd f25, 0(r3)
     
     fcmpo cr0,f26, f1                  #比较FPRG26与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f26, f1
     bnel fail_fpr_test_interword
     lfd f26, 0(r4)
     fcmpo f26, f1 
     bnel fail_fpr_test_interword
     lfd f26, 0(r3)
     
     fcmpo cr0,f27, f1                  #比较FPRG27与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f27, f1
     bnel fail_fpr_test_interword
     lfd f27, 0(r4)
     fcmpo f27, f1 
     bnel fail_fpr_test_interword
     lfd f27, 0(r3)
     
     fcmpo cr0,f28, f1                  #比较FPRG28与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f28, f1
     bnel fail_fpr_test_interword
     lfd f28, 0(r4)
     fcmpo f28, f1 
     bnel fail_fpr_test_interword
     lfd f28, 0(r3)
     
     fcmpo cr0,f29, f1                  #比较FPRG29与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f29, f1
     bnel fail_fpr_test_interword
     lfd f29, 0(r4)
     fcmpo f29, f1 
     bnel fail_fpr_test_interword
     lfd f29, 0(r3)
     
     fcmpo cr0,f30, f1                  #比较FPRG30与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f30, f1
     bnel fail_fpr_test_interword
     lfd f30, 0(r4)
     fcmpo f30, f1 
     bnel fail_fpr_test_interword
     lfd f30, 0(r3)
     
     fcmpo cr0,f31, f1                  #比较FPRG31与FPRG1值
     bnel fail_fpr_test_interword
     fcmpo f31, f1
     bnel fail_fpr_test_interword
     lfd f31, 0(r4)
     fcmpo f31, f1 
     bnel fail_fpr_test_interword
     lfd f31, 0(r3)

     lfd f1, 0(r3)
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march3
#  Description:    
#       check fpr through sequence of March3:↑(r0,r0,w0,r0,w1),  compare with gpr2
#  Author:
#  Used module:    None
#  Input Param:
#		FPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march3
     fcmpo cr0,f0, f2                  #比较FPRG0与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f0, f2
     bnel fail_fpr_test_interword
     lfd f0, 0(r3)
     fcmpo f0, f2
     bnel fail_fpr_test_interword
     lfd f0, 0(r4)

     fcmpo cr0,f1, f2                  #比较FPRG1与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f1, f2
     bnel fail_fpr_test_interword
     lfd f1, 0(r3)
     fcmpo f1, f2
     bnel fail_fpr_test_interword
     lfd f1, 0(r4)
     
     fcmpo cr0,f3, f2                  #比较FPRG3与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f3, f2
     bnel fail_fpr_test_interword
     lfd f3, 0(r3)
     fcmpo f3, f2
     bnel fail_fpr_test_interword
     lfd f3, 0(r4)
     
     fcmpo cr0,f4, f2                  #比较FPRG4与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f4, f2
     bnel fail_fpr_test_interword
     lfd f4, 0(r3)
     fcmpo f4, f2
     bnel fail_fpr_test_interword
     lfd f4, 0(r4)

     fcmpo cr0,f5, f2                  #比较FPRG5与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f5, f2
     bnel fail_fpr_test_interword
     lfd f5, 0(r3)
     fcmpo f5, f2
     bnel fail_fpr_test_interword
     lfd f5, 0(r4)

     fcmpo cr0,f6, f2                  #比较FPRG6与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f6, f2
     bnel fail_fpr_test_interword
     lfd f6, 0(r3)
     fcmpo f6, f2
     bnel fail_fpr_test_interword
     lfd f6, 0(r4)
     
     fcmpo cr0,f7, f2                  #比较FPRG7与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f7, f2
     bnel fail_fpr_test_interword
     lfd f7, 0(r3)
     fcmpo f7, f2
     bnel fail_fpr_test_interword
     lfd f7, 0(r4)
     
     fcmpo cr0,f8, f2                  #比较FPRG8与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f8, f2
     bnel fail_fpr_test_interword
     lfd f8, 0(r3)
     fcmpo f8, f2
     bnel fail_fpr_test_interword
     lfd f8, 0(r4)
     
     fcmpo cr0,f9, f2                  #比较FPRG9与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f9, f2
     bnel fail_fpr_test_interword
     lfd f9, 0(r3)
     fcmpo f9, f2
     bnel fail_fpr_test_interword
     lfd f9, 0(r4)
     
     fcmpo cr0,f10, f2                  #比较FPRG10与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f10, f2
     bnel fail_fpr_test_interword
     lfd f10, 0(r3)
     fcmpo f10, f2
     bnel fail_fpr_test_interword
     lfd f10, 0(r4)
     
     fcmpo cr0,f11, f2                  #比较FPRG11与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f11, f2
     bnel fail_fpr_test_interword
     lfd f11, 0(r3)
     fcmpo f11, f2
     bnel fail_fpr_test_interword
     lfd f11, 0(r4)
     
     fcmpo cr0,f12, f2                  #比较FPRG12与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f12, f2
     bnel fail_fpr_test_interword
     lfd f12, 0(r3)
     fcmpo f12, f2
     bnel fail_fpr_test_interword
     lfd f12, 0(r4)
     
     fcmpo cr0,f13, f2                  #比较FPRG13与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f13, f2
     bnel fail_fpr_test_interword
     lfd f13, 0(r3)
     fcmpo f13, f2
     bnel fail_fpr_test_interword
     lfd f13, 0(r4)
     
     fcmpo cr0,f14, f2                  #比较FPRG14与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f14, f2
     bnel fail_fpr_test_interword
     lfd f14, 0(r3)
     fcmpo f14, f2
     bnel fail_fpr_test_interword
     lfd f14, 0(r4)
     
     fcmpo cr0,f15, f2                  #比较FPRG15与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f15, f2
     bnel fail_fpr_test_interword
     lfd f15, 0(r3)
     fcmpo f15, f2
     bnel fail_fpr_test_interword
     lfd f15, 0(r4)
     
     fcmpo cr0,f16, f2                  #比较FPRG16与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f16, f2
     bnel fail_fpr_test_interword
     lfd f16, 0(r3)
     fcmpo f16, f2
     bnel fail_fpr_test_interword
     lfd f16, 0(r4)
     
     fcmpo cr0,f17, f2                  #比较FPRG17与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f17, f2
     bnel fail_fpr_test_interword
     lfd f17, 0(r3)
     fcmpo f17, f2
     bnel fail_fpr_test_interword
     lfd f17, 0(r4)
     
     fcmpo cr0,f18, f2                  #比较FPRG18与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f18, f2
     bnel fail_fpr_test_interword
     lfd f18, 0(r3)
     fcmpo f18, f2
     bnel fail_fpr_test_interword
     lfd f18, 0(r4)
     
     fcmpo cr0,f19, f2                  #比较FPRG19与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f19, f2
     bnel fail_fpr_test_interword
     lfd f19, 0(r3)
     fcmpo f19, f2
     bnel fail_fpr_test_interword
     lfd f19, 0(r4)
     
     fcmpo cr0,f20, f2                  #比较FPRG20与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f20, f2
     bnel fail_fpr_test_interword
     lfd f20, 0(r3)
     fcmpo f20, f2
     bnel fail_fpr_test_interword
     lfd f20, 0(r4)
     
     fcmpo cr0,f21, f2                  #比较FPRG21与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f21, f2
     bnel fail_fpr_test_interword
     lfd f21, 0(r3)
     fcmpo f21, f2
     bnel fail_fpr_test_interword
     lfd f21, 0(r4)
     
     fcmpo cr0,f22, f2                  #比较FPRG22与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f22, f2
     bnel fail_fpr_test_interword
     lfd f22, 0(r3)
     fcmpo f22, f2
     bnel fail_fpr_test_interword
     lfd f22, 0(r4)
     
     fcmpo cr0,f23, f2                  #比较FPRG23与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f23, f2
     bnel fail_fpr_test_interword
     lfd f23, 0(r3)
     fcmpo f23, f2
     bnel fail_fpr_test_interword
     lfd f23, 0(r4)
     
     fcmpo cr0,f24, f2                  #比较FPRG24与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f24, f2
     bnel fail_fpr_test_interword
     lfd f24, 0(r3)
     fcmpo f24, f2
     bnel fail_fpr_test_interword
     lfd f24, 0(r4)
     
     fcmpo cr0,f25, f2                  #比较FPRG25与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f25, f2
     bnel fail_fpr_test_interword
     lfd f25, 0(r3)
     fcmpo f25, f2
     bnel fail_fpr_test_interword
     lfd f25, 0(r4)
     
     fcmpo cr0,f26, f2                  #比较FPRG26与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f26, f2
     bnel fail_fpr_test_interword
     lfd f26, 0(r3)
     fcmpo f26, f2
     bnel fail_fpr_test_interword
     lfd f26, 0(r4)
     
     fcmpo cr0,f27, f2                  #比较FPRG27与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f27, f2
     bnel fail_fpr_test_interword
     lfd f27, 0(r3)
     fcmpo f27, f2
     bnel fail_fpr_test_interword
     lfd f27, 0(r4)
     
     fcmpo cr0,f28, f2                  #比较FPRG28与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f28, f2
     bnel fail_fpr_test_interword
     lfd f28, 0(r3)
     fcmpo f28, f2
     bnel fail_fpr_test_interword
     lfd f28, 0(r4)
     
     fcmpo cr0,f29, f2                  #比较FPRG29与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f29, f2
     bnel fail_fpr_test_interword
     lfd f29, 0(r3)
     fcmpo f29, f2
     bnel fail_fpr_test_interword
     lfd f29, 0(r4)
     
     fcmpo cr0,f30, f2                  #比较FPRG30与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f30, f2
     bnel fail_fpr_test_interword
     lfd f30, 0(r3)
     fcmpo f30, f2
     bnel fail_fpr_test_interword
     lfd f30, 0(r4)
     
     fcmpo cr0,f31, f2                  #比较FPRG31与FPRG2值
     bnel fail_fpr_test_interword
     fcmpo f31, f2
     bnel fail_fpr_test_interword
     lfd f31, 0(r3)
     fcmpo f31, f2
     bnel fail_fpr_test_interword
     lfd f31, 0(r4)
    
     lfd f2, 0(r4)
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march4
#  Description:    
#       check fpr through sequence of March4:↑(r1,r1,w1,r1,r1,w0),  compare with gpr3
#  Author:
#  Used module:    None
#  Input Param:
#		FPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march4
 
     fcmpo cr0,f0, f3                  #比较FPRG0与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f0, f3
     bnel fail_fpr_test_interword
     lfd f0, 0(r4)
     fcmpo f0, f3
     bnel fail_fpr_test_interword
     lfd f0, 0(r3)   
     fcmpo cr0,f1, f3                  #比较FPRG1与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f1, f3
     bnel fail_fpr_test_interword
     lfd f1, 0(r4)
     fcmpo f1, f3
     bnel fail_fpr_test_interword
     lfd f1, 0(r3)
     
     fcmpo cr0,f2, f3                  #比较FPRG2与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f2, f3
     bnel fail_fpr_test_interword
     lfd f2, 0(r4)
     fcmpo f2, f3
     bnel fail_fpr_test_interword
     lfd f2, 0(r3)
     
     fcmpo cr0,f4, f3                  #比较FPRG4与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f4, f3
     bnel fail_fpr_test_interword
     lfd f4, 0(r4)
     fcmpo f4, f3
     bnel fail_fpr_test_interword
     lfd f4, 0(r3)
     
     fcmpo cr0,f5, f3                  #比较FPRG5与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f5, f3
     bnel fail_fpr_test_interword
     lfd f5, 0(r4)
     fcmpo f5, f3
     bnel fail_fpr_test_interword
     lfd f5, 0(r3)
     
     fcmpo cr0,f6, f3                  #比较FPRG6与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f6, f3
     bnel fail_fpr_test_interword
     lfd f6, 0(r4)
     fcmpo f6, f3
     bnel fail_fpr_test_interword
     lfd f6, 0(r3)
     
     fcmpo cr0,f7, f3                  #比较FPRG7与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f7, f3
     bnel fail_fpr_test_interword
     lfd f7, 0(r4)
     fcmpo f7, f3
     bnel fail_fpr_test_interword
     lfd f7, 0(r3)
     
     fcmpo cr0,f8, f3                  #比较FPRG8与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f8, f3
     bnel fail_fpr_test_interword
     lfd f8, 0(r4)
     fcmpo f8, f3
     bnel fail_fpr_test_interword
     lfd f8, 0(r3)
     
     fcmpo cr0,f9, f3                  #比较FPRG9与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f9, f3
     bnel fail_fpr_test_interword
     lfd f9, 0(r4)
     fcmpo f9, f3
     bnel fail_fpr_test_interword
     lfd f9, 0(r3)
     
     fcmpo cr0,f10, f3                  #比较FPRG10与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f10, f3
     bnel fail_fpr_test_interword
     lfd f10, 0(r4)
     fcmpo f10, f3
     bnel fail_fpr_test_interword
     lfd f10, 0(r3)
     
     fcmpo cr0,f11, f3                  #比较FPRG11与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f11, f3
     bnel fail_fpr_test_interword
     lfd f11, 0(r4)
     fcmpo f11, f3
     bnel fail_fpr_test_interword
     lfd f11, 0(r3)
     
     fcmpo cr0,f12, f3                  #比较FPRG12与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f12, f3
     bnel fail_fpr_test_interword
     lfd f12, 0(r4)
     fcmpo f12, f3
     bnel fail_fpr_test_interword
     lfd f12, 0(r3)
     
     fcmpo cr0,f13, f3                  #比较FPRG13与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f13, f3
     bnel fail_fpr_test_interword
     lfd f13, 0(r4)
     fcmpo f13, f3
     bnel fail_fpr_test_interword
     lfd f13, 0(r3)
     
     fcmpo cr0,f14, f3                  #比较FPRG14与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f14, f3
     bnel fail_fpr_test_interword
     lfd f14, 0(r4)
     fcmpo f14, f3
     bnel fail_fpr_test_interword
     lfd f14, 0(r3)
     
     fcmpo cr0,f15, f3                  #比较FPRG15与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f15, f3
     bnel fail_fpr_test_interword
     lfd f15, 0(r4)
     fcmpo f15, f3
     bnel fail_fpr_test_interword
     lfd f15, 0(r3)
     
     fcmpo cr0,f16, f3                  #比较FPRG16与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f16, f3
     bnel fail_fpr_test_interword
     lfd f16, 0(r4)
     fcmpo f16, f3
     bnel fail_fpr_test_interword
     lfd f16, 0(r3)
     
     fcmpo cr0,f17, f3                  #比较FPRG17与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f17, f3
     bnel fail_fpr_test_interword
     lfd f17, 0(r4)
     fcmpo f17, f3
     bnel fail_fpr_test_interword
     lfd f17, 0(r3)
     
     fcmpo cr0,f18, f3                  #比较FPRG18与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f18, f3
     bnel fail_fpr_test_interword
     lfd f18, 0(r4)
     fcmpo f18, f3
     bnel fail_fpr_test_interword
     lfd f18, 0(r3)
     
     fcmpo cr0,f19, f3                  #比较FPRG19与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f19, f3
     bnel fail_fpr_test_interword
     lfd f19, 0(r4)
     fcmpo f19, f3
     bnel fail_fpr_test_interword
     lfd f19, 0(r3)
     
     fcmpo cr0,f20, f3                  #比较FPRG20与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f20, f3
     bnel fail_fpr_test_interword
     lfd f20, 0(r4)
     fcmpo f20, f3
     bnel fail_fpr_test_interword
     lfd f20, 0(r3)
     
     fcmpo cr0,f21, f3                  #比较FPRG21与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f21, f3
     bnel fail_fpr_test_interword
     lfd f21, 0(r4)
     fcmpo f21, f3
     bnel fail_fpr_test_interword
     lfd f21, 0(r3)
     
     fcmpo cr0,f22, f3                  #比较FPRG22与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f22, f3
     bnel fail_fpr_test_interword
     lfd f22, 0(r4)
     fcmpo f22, f3
     bnel fail_fpr_test_interword
     lfd f22, 0(r3)
     
     fcmpo cr0,f23, f3                  #比较FPRG23与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f23, f3
     bnel fail_fpr_test_interword
     lfd f23, 0(r4)
     fcmpo f23, f3
     bnel fail_fpr_test_interword
     lfd f23, 0(r3)
     
     fcmpo cr0,f24, f3                  #比较FPRG24与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f24, f3
     bnel fail_fpr_test_interword
     lfd f24, 0(r4)
     fcmpo f24, f3
     bnel fail_fpr_test_interword
     lfd f24, 0(r3)
     
     fcmpo cr0,f25, f3                  #比较FPRG25与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f25, f3
     bnel fail_fpr_test_interword
     lfd f25, 0(r4)
     fcmpo f25, f3
     bnel fail_fpr_test_interword
     lfd f25, 0(r3)
     
     fcmpo cr0,f26, f3                  #比较FPRG26与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f26, f3
     bnel fail_fpr_test_interword
     lfd f26, 0(r4)
     fcmpo f26, f3
     bnel fail_fpr_test_interword
     lfd f26, 0(r3)
     
     fcmpo cr0,f27, f3                  #比较FPRG27与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f27, f3
     bnel fail_fpr_test_interword
     lfd f27, 0(r4)
     fcmpo f27, f3
     bnel fail_fpr_test_interword
     lfd f27, 0(r3)
     
     fcmpo cr0,f28, f3                  #比较FPRG28与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f28, f3
     bnel fail_fpr_test_interword
     lfd f28, 0(r4)
     fcmpo f28, f3
     bnel fail_fpr_test_interword
     lfd f28, 0(r3)
     
     fcmpo cr0,f29, f3                  #比较FPRG29与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f29, f3
     bnel fail_fpr_test_interword
     lfd f29, 0(r4)
     fcmpo f29, f3
     bnel fail_fpr_test_interword
     lfd f29, 0(r3)
     
     fcmpo cr0,f30, f3                  #比较FPRG30与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f30, f3
     bnel fail_fpr_test_interword
     lfd f30, 0(r4)
     fcmpo f30, f3
     bnel fail_fpr_test_interword
     lfd f30, 0(r3)
     
     fcmpo cr0,f31, f3                  #比较FPRG31与FPRG3值
     bnel fail_fpr_test_interword
     fcmpo f31, f3
     bnel fail_fpr_test_interword
     lfd f31, 0(r4)
     fcmpo f31, f3
     bnel fail_fpr_test_interword
     lfd f31, 0(r3)
   
     lfd f3, 0(r3)
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march5
#  Description:    
#       check fpr through sequence of March5:↑(r0),  compare with gpr4
#  Author:
#  Used module:    None
#  Input Param:
#		GPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march5
 
     fcmpo cr0,f0, f4                 #比较FPRG0与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f1, f4                 #比较FPRG1与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f2, f4                 #比较FPRG2与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f3, f4                 #比较FPRG3与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f5, f4                 #比较FPRG5与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f6, f4                 #比较FPRG6与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f7, f4                 #比较FPRG7与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f8, f4                 #比较FPRG8与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f9, f4                 #比较FPRG9与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f10, f4                 #比较FPRG10与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f11, f4                 #比较FPRG11与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f12, f4                 #比较FPRG12与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f13, f4                 #比较FPRG13与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f14, f4                 #比较FPRG14与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f15, f4                 #比较FPRG15与FPRG4值
     bnel fail_fpr_test_interword 
     
     fcmpo cr0,f16, f4                 #比较FPRG16与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f17, f4                 #比较FPRG17与FPRG4值
     bnel fail_fpr_test_interword 
     
     fcmpo cr0,f18, f4                 #比较FPRG18与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f19, f4                 #比较FPRG19与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f20, f4                 #比较FPRG20与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f21, f4                 #比较FPRG21与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f22, f4                 #比较FPRG22与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f23, f4                 #比较FPRG23与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f24, f4                 #比较FPRG24与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f25, f4                 #比较FPRG25与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f26, f4                 #比较FPRG26与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f27, f4                 #比较FPRG27与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f28, f4                 #比较FPRG28与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f29, f4                #比较FPRG29与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f30, f4                 #比较FPRG30与FPRG4值
     bnel fail_fpr_test_interword
      
     fcmpo cr0,f31, f4                 #比较FPRG31与FPRG4值
     bnel fail_fpr_test_interword
 .endm
     
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:     FPR_interword_test
#  Description:   
#       执行浮点型寄存器测试，采用March测试方法
#  Author:
#  Used module:    None
#  Input Param:    
#		GPR3:错误记录变量
#		GPR4:错误掩码
#		FPR0-FPR31:浮点型寄存器
#		SPRG4:GPR1 value
#		SPRG5:SWAP register for GPR1
#  Output Param:   
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl FPRInterWordTest
 
FPRInterWordTest:
    PrologStackOp
    StackCheckBegin
    
    PushParam 3                     #压参数入栈
    PushParam 4
    
    StackCheckEnd
    StoreGPRs2Stack
    StoreFPRs2Stack
    StoreCtrXerCr2Stack
    StoreFpscr2Stack
    mfspr 0, SPRG4
    PushReg 0
    mfspr 0, SPRG5
    PushReg 0
    
    stw 31, OFFSET_PUNT_STACK_CORR(1)
    
    TestCPUStart
    
    mtspr SPRG4, 1                  #SPRG4保存栈顶指针

    lis r3, MarchDBs0@h
    ori r3, r3, MarchDBs0@l
    lis r4, MarchDBs1@h
    ori r4, r4, MarchDBs1@l
    
    march0
    march1
    march2
    march3
    march4
    march5
    
    mfspr 1, SPRG4
    
    TestConclusion
    b end_FPRInterWordTest
    
fail_fpr_test_interword:
    mtspr SPRG5, 1
    mfspr 1, SPRG4
    
    ErrorHandling
    
    mtspr SPRG4, 1
    mfspr 1,SPRG5
    
    mfspr 2, SPRG7
    subi 2, 2, 256
    stw 1, 132(2)
    
    mfspr 1, SPRG4
 
end_FPRInterWordTest:
    StackEpilogBegin
    
    PopReg 0
    mtspr SPRG5, 0
    PopReg 0
    mtspr SPRG4, 0
    RestoreFpscr
    RestoreCtrXerCr
    RestoreFPRs
    StackEpilogEnd
    blr
