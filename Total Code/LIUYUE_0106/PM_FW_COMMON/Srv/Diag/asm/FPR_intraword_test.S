#include "../inc/common_macro.s"
#include "../inc/common_def.h"

 .if 0
 DBs00   .equ   0x55555555
 DBs01   .equ   0xAAAAAAAA
 DBs10   .equ   0x33333333
 DBs11   .equ   0xCCCCCCCC
 DBs20   .equ   0xF0F0F0F0
 DBs21   .equ   0x0F0F0F0F
 DBs30   .equ   0xFF00FF00
 DBs31   .equ   0x00FF00FF
 DBs40   .equ   0xFFFF0000
 DBs41   .equ   0x0000FFFF
 .endif
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march5
#  Description:    
#       set gpr4 to 0x5555,set other gprs to 0xaaaa            
#       ,and after performed not operation, gprs except gpr4 are compared with gpr4 to check inter-word faults           
#       ,through sequence March5:↑[w(0xAAAAAAAA),w(0x55555555),r(0x55555555)]
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
 .macro march5
 
      lfd f4,0(r3)
      
      lfd f0,0(r4)                      #比较FPR0 与FPR4
      lfd f0,0(r3)
      fcmpo f0, f4
      bnel fail_fpr_test_intraword
      
      lfd f1,0(r4)                      #比较FPR1 与FPR4
      lfd f1,0(r3)
      fcmpo f1, f4
      bnel fail_fpr_test_intraword
      
      lfd f2,0(r4)                      #比较FPR2 与FPR4
      lfd f2,0(r3)
      fcmpo f2, f4
      bnel fail_fpr_test_intraword
      
      lfd f3,0(r4)                      #比较FPR3 与FPR4
      lfd f3,0(r3)
      fcmpo f3, f4
      bnel fail_fpr_test_intraword
      
      lfd f5,0(r4)                      #比较FPR5 与FPR4
      lfd f5,0(r3)
      fcmpo f5, f4
      bnel fail_fpr_test_intraword
      
      lfd f6,0(r4)                      #比较FPR6 与FPR4
      lfd f6,0(r3)
      fcmpo f6, f4
      bnel fail_fpr_test_intraword
      
      lfd f7,0(r4)                      #比较FPR7 与FPR4
      lfd f7,0(r3)
      fcmpo f7, f4
      bnel fail_fpr_test_intraword
      
      lfd f8,0(r4)                      #比较FPR8 与FPR4
      lfd f8,0(r3)
      fcmpo f8, f4
      bnel fail_fpr_test_intraword
      
      lfd f9,0(r4)                      #比较FPR9 与FPR4
      lfd f9,0(r3)
      fcmpo f9, f4
      bnel fail_fpr_test_intraword
      
      lfd f10,0(r4)                     #比较FPR10 与FPR4
      lfd f10,0(r3)
      fcmpo f10, f4
      bnel fail_fpr_test_intraword
      
      lfd f11,0(r4)                     #比较FPR11 与FPR4
      lfd f11,0(r3)
      fcmpo f11, f4
      bnel fail_fpr_test_intraword
      
      lfd f12,0(r4)                     #比较FPR12 与FPR4
      lfd f12,0(r3)
      fcmpo f12, f4
      bnel fail_fpr_test_intraword
      
      lfd f13,0(r4)                     #比较FPR13 与FPR4
      lfd f13,0(r3)
      fcmpo f13, f4
      bnel fail_fpr_test_intraword
      
      lfd f14,0(r4)                     #比较FPR14 与FPR4
      lfd f14,0(r3)
      fcmpo f14, f4
      bnel fail_fpr_test_intraword
      
      lfd f15,0(r4)                     #比较FPR15 与FPR4
      lfd f15,0(r3)
      fcmpo f15, f4
      bnel fail_fpr_test_intraword
      
      lfd f16,0(r4)                     #比较FPR16 与FPR4
      lfd f16,0(r3)
      fcmpo f16, f4
      bnel fail_fpr_test_intraword
      
      lfd f17,0(r4)                     #比较FPR17 与FPR4
      lfd f17,0(r3)
      fcmpo f17, f4
      bnel fail_fpr_test_intraword
      
      lfd f18,0(r4)                     #比较FPR18 与FPR4
      lfd f18,0(r3)
      fcmpo f18, f4
      bnel fail_fpr_test_intraword
      
      lfd f19,0(r4)                     #比较FPR19 与FPR4
      lfd f19,0(r3)
      fcmpo f19, f4
      bnel fail_fpr_test_intraword
      
      lfd f20,0(r4)                     #比较FPR20 与FPR4
      lfd f20,0(r3)
      fcmpo f20, f4
      bnel fail_fpr_test_intraword
      
      lfd f21,0(r4)                     #比较FPR21 与FPR4
      lfd f21,0(r3)
      fcmpo f21, f4
      bnel fail_fpr_test_intraword
      
      lfd f22,0(r4)                     #比较FPR22 与FPR4
      lfd f22,0(r3)
      fcmpo f22, f4
      bnel fail_fpr_test_intraword
      
      lfd f23,0(r4)                     #比较FPR23 与FPR4
      lfd f23,0(r3)
      fcmpo f23, f4
      bnel fail_fpr_test_intraword
      
      lfd f24,0(r4)                     #比较FPR24 与FPR4
      lfd f24,0(r3)
      fcmpo f24, f4
      bnel fail_fpr_test_intraword
      
      lfd f25,0(r4)                     #比较FPR25 与FPR4
      lfd f25,0(r3)
      fcmpo f25, f4
      bnel fail_fpr_test_intraword
      
      lfd f26,0(r4)                     #比较FPR26 与FPR4
      lfd f26,0(r3)
      fcmpo f26, f4
      bnel fail_fpr_test_intraword
      
      lfd f27,0(r4)                     #比较FPR27 与FPR4
      lfd f27,0(r3)
      fcmpo f27, f4
      bnel fail_fpr_test_intraword
      
      lfd f28,0(r4)                     #比较FPR28 与FPR4
      lfd f28,0(r3)
      fcmpo f28, f4
      bnel fail_fpr_test_intraword
      
      lfd f29,0(r4)                     #比较FPR29 与FPR4
      lfd f29,0(r3)
      fcmpo f29, f4
      bnel fail_fpr_test_intraword
      
      lfd f30,0(r4)                     #比较FPR30 与FPR4
      lfd f30,0(r3)
      fcmpo f30, f4
      bnel fail_fpr_test_intraword
      
      lfd f31,0(r4)                     #比较FPR31 与FPR4
      lfd f31,0(r3)
      fcmpo f31, f4
      bnel fail_fpr_test_intraword
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march6
#  Description:    
#       check gprs through sequence March6:↓[r0x55555555,w0xAAAAAAAA,r0xAAAAAAAA]
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
 .macro march6
 
      fcmpo f31, f5
      bnel fail_fpr_test_intraword              #比较FPR31与FPRG5
      lfd f31,0(r4)
      lfd f5,0(r4)
      fcmpo f31, f5
      bnel fail_fpr_test_intraword              
      lfd f5,0(r3)
      
      fcmpo f30, f5
      bnel fail_fpr_test_intraword              #比较FPR30与FPRG5
      lfd f30,0(r4)
      lfd f5,0(r4)
      fcmpo f30, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f29, f5
      bnel fail_fpr_test_intraword              #比较FPR29与FPRG5
      lfd f29,0(r4)
      lfd f5,0(r4)
      fcmpo f29, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f28, f5
      bnel fail_fpr_test_intraword              #比较FPR28与FPRG5
      lfd f28,0(r4)
      lfd f5,0(r4)
      fcmpo f28, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f27, f5
      bnel fail_fpr_test_intraword              #比较FPR27与FPRG5
      lfd f27,0(r4)
      lfd f5,0(r4)
      fcmpo f27, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f26, f5
      bnel fail_fpr_test_intraword              #比较FPR26与FPRG5
      lfd f26,0(r4)
      lfd f5,0(r4)
      fcmpo f26, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f25, f5
      bnel fail_fpr_test_intraword              #比较FPR25与FPRG5
      lfd f25,0(r4)
      lfd f5,0(r4)
      fcmpo f25, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f24, f5
      bnel fail_fpr_test_intraword              #比较FPR24与FPRG5
      lfd f24,0(r4)
      lfd f5,0(r4)
      fcmpo f24, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f23, f5
      bnel fail_fpr_test_intraword              #比较FPR23与FPRG5
      lfd f23,0(r4)
      lfd f5,0(r4)
      fcmpo f23, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f22, f5
      bnel fail_fpr_test_intraword              #比较FPR22与FPRG5
      lfd f22,0(r4)
      lfd f5,0(r4)
      fcmpo f22, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f21, f5
      bnel fail_fpr_test_intraword              #比较FPR21与FPRG5
      lfd f21,0(r4)
      lfd f5,0(r4)
      fcmpo f21, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f20, f5
      bnel fail_fpr_test_intraword              #比较FPR20与FPRG5
      lfd f20,0(r4)
      lfd f5,0(r4)
      fcmpo f20, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f19, f5
      bnel fail_fpr_test_intraword              #比较FPR19与FPRG5
      lfd f19,0(r4)
      lfd f5,0(r4)
      fcmpo f19, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f18, f5
      bnel fail_fpr_test_intraword              #比较FPR18与FPRG5
      lfd f18,0(r4)
      lfd f5,0(r4)
      fcmpo f18, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f17, f5
      bnel fail_fpr_test_intraword              #比较FPR17与FPRG5
      lfd f17,0(r4)
      lfd f5,0(r4)
      fcmpo f17, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f16, f5
      bnel fail_fpr_test_intraword              #比较FPR16与FPRG5
      lfd f16,0(r4)
      lfd f5,0(r4)
      fcmpo f16, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f15, f5
      bnel fail_fpr_test_intraword              #比较FPR15与FPRG5
      lfd f15,0(r4)
      lfd f5,0(r4)
      fcmpo f15, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f14, f5
      bnel fail_fpr_test_intraword              #比较FPR14与FPRG5
      lfd f14,0(r4)
      lfd f5,0(r4)
      fcmpo f14, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f13, f5
      bnel fail_fpr_test_intraword              #比较FPR13与FPRG5
      lfd f13,0(r4)
      lfd f5,0(r4)
      fcmpo f13, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f12, f5
      bnel fail_fpr_test_intraword              #比较FPR12与FPRG5
      lfd f12,0(r4)
      lfd f5,0(r4)
      fcmpo f12, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f11, f5
      bnel fail_fpr_test_intraword              #比较FPR11与FPRG5
      lfd f11,0(r4)
      lfd f5,0(r4)
      fcmpo f11, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f10, f5
      bnel fail_fpr_test_intraword              #比较FPR10与FPRG5
      lfd f10,0(r4)
      lfd f5,0(r4)
      fcmpo f10, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f9, f5
      bnel fail_fpr_test_intraword              #比较FPR9与FPRG5
      lfd f9,0(r4)
      lfd f5,0(r4)
      fcmpo f9, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f8, f5
      bnel fail_fpr_test_intraword              #比较FPR8与FPRG5
      lfd f8,0(r4)
      lfd f5,0(r4)
      fcmpo f8, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f7, f5
      bnel fail_fpr_test_intraword              #比较FPR7与FPRG5
      lfd f7,0(r4)
      lfd f5,0(r4)
      fcmpo f7, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f6, f5
      bnel fail_fpr_test_intraword              #比较FPR6与FPRG5
      lfd f6,0(r4)
      lfd f5,0(r4)
      fcmpo f6, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f4, f5
      bnel fail_fpr_test_intraword              #比较FPR4与FPRG5
      lfd f4,0(r4)
      lfd f5,0(r4)
      fcmpo f4, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f3, f5
      bnel fail_fpr_test_intraword              #比较FPR3与FPRG5
      lfd f3,0(r4)
      lfd f5,0(r4)
      fcmpo f3, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f2, f5
      bnel fail_fpr_test_intraword              #比较FPR2与FPRG5
      lfd f2,0(r4)
      lfd f5,0(r4)
      fcmpo f2, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f1, f5
      bnel fail_fpr_test_intraword              #比较FPR1与FPRG5
      lfd f1,0(r4)
      lfd f5,0(r4)
      fcmpo f1, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)
      
      fcmpo f0, f5
      bnel fail_fpr_test_intraword              #比较FPR0与FPRG5
      lfd f0,0(r4)
      lfd f5,0(r4)
      fcmpo f0, f5
      bnel fail_fpr_test_intraword
      lfd f5,0(r3)

      lfd f5,0(r4)
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march7
#  Description:    
#       check intra-word faults through sequence            
#       March7:↑[r(0xAAAAAAAA), w(0xCCCCCCCC), w(0x33333333),r(0x33333333)]
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
 .macro march7
 
     lfd f6,0(r4)
     fcmpo f0, f6
     bnel fail_fpr_test_intraword              #比较FPR0与FPRG6
     lfd f0,0(r6)
     lfd f0,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f0, f6
     bnel fail_fpr_test_intraword    
     
     lfd f6,0(r4)
     fcmpo f1, f6
     bnel fail_fpr_test_intraword              #比较FPR1与FPRG6
     lfd f1,0(r6)
     lfd f1,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f1, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f2, f6
     bnel fail_fpr_test_intraword              #比较FPR2与FPRG6
     lfd f2,0(r6)
     lfd f2,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f2, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f3, f6
     bnel fail_fpr_test_intraword              #比较FPR3与FPRG6
     lfd f3,0(r6)
     lfd f3,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f3, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f4, f6
     bnel fail_fpr_test_intraword              #比较FPR4与FPRG6
     lfd f4,0(r6)
     lfd f4,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f4, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f5, f6
     bnel fail_fpr_test_intraword              #比较FPR5与FPRG6
     lfd f5,0(r6)
     lfd f5,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f5, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f7, f6
     bnel fail_fpr_test_intraword              #比较FPR7与FPRG6
     lfd f7,0(r6)
     lfd f7,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f7, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f8, f6
     bnel fail_fpr_test_intraword              #比较FPR8与FPRG6
     lfd f8,0(r6)
     lfd f8,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f8, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f9, f6
     bnel fail_fpr_test_intraword              #比较FPR9与FPRG6
     lfd f9,0(r6)
     lfd f9,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f9, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f10, f6
     bnel fail_fpr_test_intraword              #比较FPR10与FPRG6
     lfd f10,0(r6)
     lfd f10,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f10, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f11, f6
     bnel fail_fpr_test_intraword              #比较FPR11与FPRG6
     lfd f11,0(r6)
     lfd f11,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f11, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f12, f6
     bnel fail_fpr_test_intraword              #比较FPR12与FPRG6
     lfd f12,0(r6)
     lfd f12,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f12, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f13, f6
     bnel fail_fpr_test_intraword              #比较FPR13与FPRG6
     lfd f13,0(r6)
     lfd f13,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f13, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f14, f6
     bnel fail_fpr_test_intraword              #比较FPR14与FPRG6
     lfd f14,0(r6)
     lfd f14,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f14, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f15, f6
     bnel fail_fpr_test_intraword              #比较FPR15与FPRG6
     lfd f15,0(r6)
     lfd f15,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f15, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f16, f6
     bnel fail_fpr_test_intraword              #比较FPR16与FPRG6
     lfd f16,0(r6)
     lfd f16,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f16, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f17, f6
     bnel fail_fpr_test_intraword              #比较FPR17与FPRG6
     lfd f17,0(r6)
     lfd f17,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f17, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f18, f6
     bnel fail_fpr_test_intraword              #比较FPR18与FPRG6
     lfd f18,0(r6)
     lfd f18,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f18, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f19, f6
     bnel fail_fpr_test_intraword              #比较FPR19与FPRG6
     lfd f19,0(r6)
     lfd f19,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f19, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f20, f6
     bnel fail_fpr_test_intraword              #比较FPR20与FPRG6
     lfd f20,0(r6)
     lfd f20,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f20, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f21, f6
     bnel fail_fpr_test_intraword              #比较FPR21与FPRG6
     lfd f21,0(r6)
     lfd f21,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f21, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f22, f6
     bnel fail_fpr_test_intraword              #比较FPR22与FPRG6
     lfd f22,0(r6)
     lfd f22,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f22, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f23, f6
     bnel fail_fpr_test_intraword              #比较FPR23与FPRG6
     lfd f23,0(r6)
     lfd f23,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f23, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f24, f6
     bnel fail_fpr_test_intraword              #比较FPR24与FPRG6
     lfd f24,0(r6)
     lfd f24,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f24, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f25, f6
     bnel fail_fpr_test_intraword              #比较FPR25与FPRG6
     lfd f25,0(r6)
     lfd f25,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f25, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f26, f6
     bnel fail_fpr_test_intraword              #比较FPR26与FPRG6
     lfd f26,0(r6)
     lfd f26,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f26, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f27, f6
     bnel fail_fpr_test_intraword              #比较FPR27与FPRG6
     lfd f27,0(r6)
     lfd f27,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f27, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f28, f6
     bnel fail_fpr_test_intraword              #比较FPR28与FPRG6
     lfd f28,0(r6)
     lfd f28,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f28, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f29, f6
     bnel fail_fpr_test_intraword              #比较FPR29与FPRG6
     lfd f29,0(r6)
     lfd f29,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f29, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f30, f6
     bnel fail_fpr_test_intraword              #比较FPR30与FPRG6
     lfd f30,0(r6)
     lfd f30,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f30, f6
     bnel fail_fpr_test_intraword  
     
     lfd f6,0(r4)
     fcmpo f31, f6
     bnel fail_fpr_test_intraword              #比较FPR31与FPRG6
     lfd f31,0(r6)
     lfd f31,0(r5)
     lfd f6,0(r6)
     lfd f6,0(r5)
     fcmpo f31, f6
     bnel fail_fpr_test_intraword  
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march8
#  Description:    
#       check intra-word faults through sequence March8:↓[r(0x33333333), w(0xCCCCCCCC),r(0xCCCCCCCC)]
#  Author:         wang fan 
#  Used module:    None
#  Input Param:
#		GPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/  
 .macro march8
 
     fcmpo f31, f7
     bnel fail_fpr_test_intraword              #比较FPR31与FPRG7
     lfd f31,0(r6)
     lfd f7,0(r6)
     fcmpo f31, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f30, f7
     bnel fail_fpr_test_intraword              #比较FPR30与FPRG7
     lfd f30,0(r6)
     lfd f7,0(r6)
     fcmpo f30, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f29, f7
     bnel fail_fpr_test_intraword              #比较FPR29与FPRG7
     lfd f29,0(r6)
     lfd f7,0(r6)
     fcmpo f29, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f28, f7
     bnel fail_fpr_test_intraword              #比较FPR28与FPRG7
     lfd f28,0(r6)
     lfd f7,0(r6)
     fcmpo f28, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f27, f7
     bnel fail_fpr_test_intraword              #比较FPR27与FPRG7
     lfd f27,0(r6)
     lfd f7,0(r6)
     fcmpo f27, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f26, f7
     bnel fail_fpr_test_intraword              #比较FPR26与FPRG7
     lfd f26,0(r6)
     lfd f7,0(r6)
     fcmpo f26, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f25, f7
     bnel fail_fpr_test_intraword              #比较FPR25与FPRG7
     lfd f25,0(r6)
     lfd f7,0(r6)
     fcmpo f25, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f24, f7
     bnel fail_fpr_test_intraword              #比较FPR24与FPRG7
     lfd f24,0(r6)
     lfd f7,0(r6)
     fcmpo f24, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f23, f7
     bnel fail_fpr_test_intraword              #比较FPR23与FPRG7
     lfd f23,0(r6)
     lfd f7,0(r6)
     fcmpo f23, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f22, f7
     bnel fail_fpr_test_intraword              #比较FPR22与FPRG7
     lfd f22,0(r6)
     lfd f7,0(r6)
     fcmpo f22, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f21, f7
     bnel fail_fpr_test_intraword              #比较FPR21与FPRG7
     lfd f21,0(r6)
     lfd f7,0(r6)
     fcmpo f21, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f20, f7
     bnel fail_fpr_test_intraword              #比较FPR20与FPRG7
     lfd f20,0(r6)
     lfd f7,0(r6)
     fcmpo f20, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f19, f7
     bnel fail_fpr_test_intraword              #比较FPR19与FPRG7
     lfd f19,0(r6)
     lfd f7,0(r6)
     fcmpo f19, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f18, f7
     bnel fail_fpr_test_intraword              #比较FPR18与FPRG7
     lfd f18,0(r6)
     lfd f7,0(r6)
     fcmpo f18, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f17, f7
     bnel fail_fpr_test_intraword              #比较FPR17与FPRG7
     lfd f17,0(r6)
     lfd f7,0(r6)
     fcmpo f17, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f16, f7
     bnel fail_fpr_test_intraword              #比较FPR16与FPRG7
     lfd f16,0(r6)
     lfd f7,0(r6)
     fcmpo f16, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f15, f7
     bnel fail_fpr_test_intraword              #比较FPR15与FPRG7
     lfd f15,0(r6)
     lfd f7,0(r6)
     fcmpo f15, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f14, f7
     bnel fail_fpr_test_intraword              #比较FPR14与FPRG7
     lfd f14,0(r6)
     lfd f7,0(r6)
     fcmpo f14, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f13, f7
     bnel fail_fpr_test_intraword              #比较FPR13与FPRG7
     lfd f13,0(r6)
     lfd f7,0(r6)
     fcmpo f13, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f12, f7
     bnel fail_fpr_test_intraword              #比较FPR12与FPRG7
     lfd f12,0(r6)
     lfd f7,0(r6)
     fcmpo f12, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f11, f7
     bnel fail_fpr_test_intraword              #比较FPR11与FPRG7
     lfd f11,0(r6)
     lfd f7,0(r6)
     fcmpo f11, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f10, f7
     bnel fail_fpr_test_intraword              #比较FPR10与FPRG7
     lfd f10,0(r6)
     lfd f7,0(r6)
     fcmpo f10, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f9, f7
     bnel fail_fpr_test_intraword              #比较FPR9与FPRG7
     lfd f9,0(r6)
     lfd f7,0(r6)
     fcmpo f9, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f8, f7
     bnel fail_fpr_test_intraword              #比较FPR8与FPRG7
     lfd f8,0(r6)
     lfd f7,0(r6)
     fcmpo f8, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f6, f7
     bnel fail_fpr_test_intraword              #比较FPR6与FPRG7
     lfd f6,0(r6)
     lfd f7,0(r6)
     fcmpo f6, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f5, f7
     bnel fail_fpr_test_intraword              #比较FPR5与FPRG7
     lfd f5,0(r6)
     lfd f7,0(r6)
     fcmpo f5, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f4, f7
     bnel fail_fpr_test_intraword              #比较FPR4与FPRG7
     lfd f4,0(r6)
     lfd f7,0(r6)
     fcmpo f4, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f3, f7
     bnel fail_fpr_test_intraword              #比较FPR3与FPRG7
     lfd f3,0(r6)
     lfd f7,0(r6)
     fcmpo f3, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f2, f7
     bnel fail_fpr_test_intraword              #比较FPR2与FPRG7
     lfd f2,0(r6)
     lfd f7,0(r6)
     fcmpo f2, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f1, f7
     bnel fail_fpr_test_intraword              #比较FPR1与FPRG7
     lfd f1,0(r6)
     lfd f7,0(r6)
     fcmpo f1, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)
     
     fcmpo f0, f7
     bnel fail_fpr_test_intraword              #比较FPR0与FPRG7
     lfd f0,0(r6)
     lfd f7,0(r6)
     fcmpo f0, f7
     bnel fail_fpr_test_intraword  
     lfd f7,0(r5)

     lfd f7,0(r6)
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march9
#  Description:    
#       check intra-word faults through sequence            
#       March9:↑[r(0xCCCCCCCC), w(0xF0F0F0F0), w(0x0F0F0F0F),r(0x0F0F0F0F)]
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
 .macro march9

     lfd f8,0(r6)
     fcmpo f0, f8
     bnel fail_fpr_test_intraword              #比较FPR0与FPRG8
     lfd f0,0(r7)
     lfd f0,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f0, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f1, f8
     bnel fail_fpr_test_intraword              #比较FPR1与FPRG8
     lfd f1,0(r7)
     lfd f1,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f1, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f2, f8
     bnel fail_fpr_test_intraword              #比较FPR2与FPRG8
     lfd f2,0(r7)
     lfd f2,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f2, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f3, f8
     bnel fail_fpr_test_intraword              #比较FPR3与FPRG8
     lfd f3,0(r7)
     lfd f3,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f3, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f4, f8
     bnel fail_fpr_test_intraword              #比较FPR4与FPRG8
     lfd f4,0(r7)
     lfd f4,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f4, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f5, f8
     bnel fail_fpr_test_intraword              #比较FPR5与FPRG8
     lfd f5,0(r7)
     lfd f5,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f5, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f6, f8
     bnel fail_fpr_test_intraword              #比较FPR6与FPRG8
     lfd f6,0(r7)
     lfd f6,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f6, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f7, f8
     bnel fail_fpr_test_intraword              #比较FPR7与FPRG8
     lfd f7,0(r7)
     lfd f7,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f7, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f9, f8
     bnel fail_fpr_test_intraword              #比较FPR9与FPRG8
     lfd f9,0(r7)
     lfd f9,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f9, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f10, f8
     bnel fail_fpr_test_intraword              #比较FPR10与FPRG8
     lfd f10,0(r7)
     lfd f10,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f10, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f11, f8
     bnel fail_fpr_test_intraword              #比较FPR11与FPRG8
     lfd f11,0(r7)
     lfd f11,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f11, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f12, f8
     bnel fail_fpr_test_intraword              #比较FPR12与FPRG8
     lfd f12,0(r7)
     lfd f12,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f12, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f13, f8
     bnel fail_fpr_test_intraword              #比较FPR13与FPRG8
     lfd f13,0(r7)
     lfd f13,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f13, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f14, f8
     bnel fail_fpr_test_intraword              #比较FPR14与FPRG8
     lfd f14,0(r7)
     lfd f14,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f14, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f15, f8
     bnel fail_fpr_test_intraword              #比较FPR15与FPRG8
     lfd f15,0(r7)
     lfd f15,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f15, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f16, f8
     bnel fail_fpr_test_intraword              #比较FPR16与FPRG8
     lfd f16,0(r7)
     lfd f16,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f16, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f17, f8
     bnel fail_fpr_test_intraword              #比较FPR17与FPRG8
     lfd f17,0(r7)
     lfd f17,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f17, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f18, f8
     bnel fail_fpr_test_intraword              #比较FPR18与FPRG8
     lfd f18,0(r7)
     lfd f18,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f18, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f19, f8
     bnel fail_fpr_test_intraword              #比较FPR19与FPRG8
     lfd f19,0(r7)
     lfd f19,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f19, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f20, f8
     bnel fail_fpr_test_intraword              #比较FPR20与FPRG8
     lfd f20,0(r7)
     lfd f20,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f20, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f21, f8
     bnel fail_fpr_test_intraword              #比较FPR21与FPRG8
     lfd f21,0(r7)
     lfd f21,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f21, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f22, f8
     bnel fail_fpr_test_intraword              #比较FPR22与FPRG8
     lfd f22,0(r7)
     lfd f22,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f22, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f23, f8
     bnel fail_fpr_test_intraword              #比较FPR23与FPRG8
     lfd f23,0(r7)
     lfd f23,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f23, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f24, f8
     bnel fail_fpr_test_intraword              #比较FPR24与FPRG8
     lfd f24,0(r7)
     lfd f24,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f24, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f25, f8
     bnel fail_fpr_test_intraword              #比较FPR25与FPRG8
     lfd f25,0(r7)
     lfd f25,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f25, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f26, f8
     bnel fail_fpr_test_intraword              #比较FPR26与FPRG8
     lfd f26,0(r7)
     lfd f26,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f26, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f27, f8
     bnel fail_fpr_test_intraword              #比较FPR27与FPRG8
     lfd f27,0(r7)
     lfd f27,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f27, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f28, f8
     bnel fail_fpr_test_intraword              #比较FPR28与FPRG8
     lfd f28,0(r7)
     lfd f28,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f28, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f29, f8
     bnel fail_fpr_test_intraword              #比较FPR29与FPRG8
     lfd f29,0(r7)
     lfd f29,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f29, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f30, f8
     bnel fail_fpr_test_intraword              #比较FPR30与FPRG8
     lfd f30,0(r7)
     lfd f30,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f30, f8
     bnel fail_fpr_test_intraword
     
     lfd f8,0(r6)
     fcmpo f31, f8
     bnel fail_fpr_test_intraword              #比较FPR31与FPRG8
     lfd f31,0(r7)
     lfd f31,0(r8)
     lfd f8,0(r7)
     lfd f8,0(r8)
     fcmpo f31, f8
     bnel fail_fpr_test_intraword
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march10
#  Description:    
#       check intra-word faults through sequence            
#       March10:↓[r(0x0F0F0F0F), w(0xF0F0F0F0),r(0xF0F0F0F0)]
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
 .macro march10

     fcmpo f31, f9
     bnel fail_fpr_test_intraword              #比较FPR31与FPRG9
     lfd f31,0(r7)
     lfd f9,0(r7)
     fcmpo f31, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f30, f9
     bnel fail_fpr_test_intraword              #比较FPR30与FPRG9
     lfd f30,0(r7)
     lfd f9,0(r7)
     fcmpo f30, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f29, f9
     bnel fail_fpr_test_intraword              #比较FPR29与FPRG9
     lfd f29,0(r7)
     lfd f9,0(r7)
     fcmpo f29, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f28, f9
     bnel fail_fpr_test_intraword              #比较FPR28与FPRG9
     lfd f28,0(r7)
     lfd f9,0(r7)
     fcmpo f28, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f27, f9
     bnel fail_fpr_test_intraword              #比较FPR27与FPRG9
     lfd f27,0(r7)
     lfd f9,0(r7)
     fcmpo f27, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f26, f9
     bnel fail_fpr_test_intraword              #比较FPR26与FPRG9
     lfd f26,0(r7)
     lfd f9,0(r7)
     fcmpo f26, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f25, f9
     bnel fail_fpr_test_intraword              #比较FPR25与FPRG9
     lfd f25,0(r7)
     lfd f9,0(r7)
     fcmpo f25, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f24, f9
     bnel fail_fpr_test_intraword              #比较FPR24与FPRG9
     lfd f24,0(r7)
     lfd f9,0(r7)
     fcmpo f24, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f23, f9
     bnel fail_fpr_test_intraword              #比较FPR23与FPRG9
     lfd f23,0(r7)
     lfd f9,0(r7)
     fcmpo f23, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f22, f9
     bnel fail_fpr_test_intraword              #比较FPR22与FPRG9
     lfd f22,0(r7)
     lfd f9,0(r7)
     fcmpo f22, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f21, f9
     bnel fail_fpr_test_intraword              #比较FPR21与FPRG9
     lfd f21,0(r7)
     lfd f9,0(r7)
     fcmpo f21, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f20, f9
     bnel fail_fpr_test_intraword              #比较FPR20与FPRG9
     lfd f20,0(r7)
     lfd f9,0(r7)
     fcmpo f20, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f19, f9
     bnel fail_fpr_test_intraword              #比较FPR19与FPRG9
     lfd f19,0(r7)
     lfd f9,0(r7)
     fcmpo f19, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f18, f9
     bnel fail_fpr_test_intraword              #比较FPR18与FPRG9
     lfd f18,0(r7)
     lfd f9,0(r7)
     fcmpo f18, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f17, f9
     bnel fail_fpr_test_intraword              #比较FPR17与FPRG9
     lfd f17,0(r7)
     lfd f9,0(r7)
     fcmpo f17, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f16, f9
     bnel fail_fpr_test_intraword              #比较FPR16与FPRG9
     lfd f16,0(r7)
     lfd f9,0(r7)
     fcmpo f16, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f15, f9
     bnel fail_fpr_test_intraword              #比较FPR15与FPRG9
     lfd f15,0(r7)
     lfd f9,0(r7)
     fcmpo f15, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f14, f9
     bnel fail_fpr_test_intraword              #比较FPR14与FPRG9
     lfd f14,0(r7)
     lfd f9,0(r7)
     fcmpo f14, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f13, f9
     bnel fail_fpr_test_intraword              #比较FPR13与FPRG9
     lfd f13,0(r7)
     lfd f9,0(r7)
     fcmpo f13, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f12, f9
     bnel fail_fpr_test_intraword              #比较FPR12与FPRG9
     lfd f12,0(r7)
     lfd f9,0(r7)
     fcmpo f12, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f11, f9
     bnel fail_fpr_test_intraword              #比较FPR11与FPRG9
     lfd f11,0(r7)
     lfd f9,0(r7)
     fcmpo f11, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f10, f9
     bnel fail_fpr_test_intraword              #比较FPR10与FPRG9
     lfd f10,0(r7)
     lfd f9,0(r7)
     fcmpo f10, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f8, f9
     bnel fail_fpr_test_intraword              #比较FPR8与FPRG9
     lfd f8,0(r7)
     lfd f9,0(r7)
     fcmpo f8, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f7, f9
     bnel fail_fpr_test_intraword              #比较FPR7与FPRG9
     lfd f7,0(r7)
     lfd f9,0(r7)
     fcmpo f7, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f6, f9
     bnel fail_fpr_test_intraword              #比较FPR6与FPRG9
     lfd f6,0(r7)
     lfd f9,0(r7)
     fcmpo f6, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f5, f9
     bnel fail_fpr_test_intraword              #比较FPR5与FPRG9
     lfd f5,0(r7)
     lfd f9,0(r7)
     fcmpo f5, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f4, f9
     bnel fail_fpr_test_intraword              #比较FPR4与FPRG9
     lfd f4,0(r7)
     lfd f9,0(r7)
     fcmpo f4, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f3, f9
     bnel fail_fpr_test_intraword              #比较FPR3与FPRG9
     lfd f3,0(r7)
     lfd f9,0(r7)
     fcmpo f3, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f2, f9
     bnel fail_fpr_test_intraword              #比较FPR2与FPRG9
     lfd f2,0(r7)
     lfd f9,0(r7)
     fcmpo f2, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f1, f9
     bnel fail_fpr_test_intraword              #比较FPR1与FPRG9
     lfd f1,0(r7)
     lfd f9,0(r7)
     fcmpo f1, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
     
     fcmpo f0, f9
     bnel fail_fpr_test_intraword              #比较FPR0与FPRG9
     lfd f0,0(r7)
     lfd f9,0(r7)
     fcmpo f0, f9
     bnel fail_fpr_test_intraword
     lfd f9,0(r8)
                             
     lfd f9,0(r7)
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march11
#  Description:    
#       check intra-word faults through sequence            
#       March11:↑[r(0xF0F0F0F0), w(0xFF00FF00), w(0x00FF00FF),r(0x00FF00FF)]
#  Author:         wang fan 
#  Used module:    None
#  Input Param:
#		GPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march11
    fcmpo f0, f10
    bnel fail_fpr_test_intraword              #比较FPR0与FPRG10
    lfd f0,0(r9)
    lfd f0,0(r10)
    lfd f10,0(r10)
    fcmpo f0, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)
   
    fcmpo f1, f10
    bnel fail_fpr_test_intraword              #比较FPR1与FPRG10
    lfd f1,0(r9)
    lfd f1,0(r10)
    lfd f10,0(r10)
    fcmpo f1, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f2, f10
    bnel fail_fpr_test_intraword              #比较FPR2与FPRG10
    lfd f2,0(r9)
    lfd f2,0(r10)
    lfd f10,0(r10)
    fcmpo f2, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f3, f10
    bnel fail_fpr_test_intraword              #比较FPR3与FPRG10
    lfd f3,0(r9)
    lfd f3,0(r10)
    lfd f10,0(r10)
    fcmpo f3, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f4, f10
    bnel fail_fpr_test_intraword              #比较FPR4与FPRG10
    lfd f4,0(r9)
    lfd f4,0(r10)
    lfd f10,0(r10)
    fcmpo f4, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f5, f10
    bnel fail_fpr_test_intraword              #比较FPR5与FPRG10
    lfd f5,0(r9)
    lfd f5,0(r10)
    lfd f10,0(r10)
    fcmpo f5, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f6, f10
    bnel fail_fpr_test_intraword              #比较FPR6与FPRG10
    lfd f6,0(r9)
    lfd f6,0(r10)
    lfd f10,0(r10)
    fcmpo f6, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f7, f10
    bnel fail_fpr_test_intraword              #比较FPR7与FPRG10
    lfd f7,0(r9)
    lfd f7,0(r10)
    lfd f10,0(r10)
    fcmpo f7, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f8, f10
    bnel fail_fpr_test_intraword              #比较FPR8与FPRG10
    lfd f8,0(r9)
    lfd f8,0(r10)
    lfd f10,0(r10)
    fcmpo f8, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f9, f10
    bnel fail_fpr_test_intraword              #比较FPR9与FPRG10
    lfd f9,0(r9)
    lfd f9,0(r10)
    lfd f10,0(r10)
    fcmpo f9, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f11, f10
    bnel fail_fpr_test_intraword              #比较FPR11与FPRG10
    lfd f11,0(r9)
    lfd f11,0(r10)
    lfd f10,0(r10)
    fcmpo f11, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f12, f10
    bnel fail_fpr_test_intraword              #比较FPR12与FPRG10
    lfd f12,0(r9)
    lfd f12,0(r10)
    lfd f10,0(r10)
    fcmpo f12, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f13, f10
    bnel fail_fpr_test_intraword              #比较FPR13与FPRG10
    lfd f13,0(r9)
    lfd f13,0(r10)
    lfd f10,0(r10)
    fcmpo f13, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f14, f10
    bnel fail_fpr_test_intraword              #比较FPR14与FPRG10
    lfd f14,0(r9)
    lfd f14,0(r10)
    lfd f10,0(r10)
    fcmpo f14, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f15, f10
    bnel fail_fpr_test_intraword              #比较FPR15与FPRG10
    lfd f15,0(r9)
    lfd f15,0(r10)
    lfd f10,0(r10)
    fcmpo f15, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f16, f10
    bnel fail_fpr_test_intraword              #比较FPR16与FPRG10
    lfd f16,0(r9)
    lfd f16,0(r10)
    lfd f10,0(r10)
    fcmpo f16, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f17, f10
    bnel fail_fpr_test_intraword              #比较FPR17与FPRG10
    lfd f17,0(r9)
    lfd f17,0(r10)
    lfd f10,0(r10)
    fcmpo f17, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f18, f10
    bnel fail_fpr_test_intraword              #比较FPR18与FPRG10
    lfd f18,0(r9)
    lfd f18,0(r10)
    lfd f10,0(r10)
    fcmpo f18, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f19, f10
    bnel fail_fpr_test_intraword              #比较FPR19与FPRG10
    lfd f19,0(r9)
    lfd f19,0(r10)
    lfd f10,0(r10)
    fcmpo f19, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f20, f10
    bnel fail_fpr_test_intraword              #比较FPR20与FPRG10
    lfd f20,0(r9)
    lfd f20,0(r10)
    lfd f10,0(r10)
    fcmpo f20, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f21, f10
    bnel fail_fpr_test_intraword              #比较FPR21与FPRG10
    lfd f21,0(r9)
    lfd f21,0(r10)
    lfd f10,0(r10)
    fcmpo f21, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f22, f10
    bnel fail_fpr_test_intraword              #比较FPR22与FPRG10
    lfd f22,0(r9)
    lfd f22,0(r10)
    lfd f10,0(r10)
    fcmpo f22, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f23, f10
    bnel fail_fpr_test_intraword              #比较FPR23与FPRG10
    lfd f23,0(r9)
    lfd f23,0(r10)
    lfd f10,0(r10)
    fcmpo f23, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f24, f10
    bnel fail_fpr_test_intraword              #比较FPR24与FPRG10
    lfd f24,0(r9)
    lfd f24,0(r10)
    lfd f10,0(r10)
    fcmpo f24, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f25, f10
    bnel fail_fpr_test_intraword              #比较FPR25与FPRG10
    lfd f25,0(r9)
    lfd f25,0(r10)
    lfd f10,0(r10)
    fcmpo f25, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f26, f10
    bnel fail_fpr_test_intraword              #比较FPR26与FPRG10
    lfd f26,0(r9)
    lfd f26,0(r10)
    lfd f10,0(r10)
    fcmpo f26, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f27, f10
    bnel fail_fpr_test_intraword              #比较FPR27与FPRG10
    lfd f27,0(r9)
    lfd f27,0(r10)
    lfd f10,0(r10)
    fcmpo f27, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f28, f10
    bnel fail_fpr_test_intraword              #比较FPR28与FPRG10
    lfd f28,0(r9)
    lfd f28,0(r10)
    lfd f10,0(r10)
    fcmpo f28, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f29, f10
    bnel fail_fpr_test_intraword              #比较FPR29与FPRG10
    lfd f29,0(r9)
    lfd f29,0(r10)
    lfd f10,0(r10)
    fcmpo f29, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f30, f10
    bnel fail_fpr_test_intraword              #比较FPR30与FPRG10
    lfd f30,0(r9)
    lfd f30,0(r10)
    lfd f10,0(r10)
    fcmpo f30, f10
    bnel fail_fpr_test_intraword
    lfd f10,0(r7)

    fcmpo f31, f10
    bnel fail_fpr_test_intraword              #比较FPR31与FPRG10
    lfd f31,0(r9)
    lfd f31,0(r10)
    lfd f10,0(r10)
    fcmpo f31, f10
    bnel fail_fpr_test_intraword

########ADD BY CK 2012.2.23

########ADD END
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march12
#  Description:    
#       check intra-word faults through sequence           
#       March12:↓[r(0x00FF00FF), w(0xFF00FF00),r(0xFF00FF00)]
#  Author:         wang fan 
#  Used module:    None
#  Input Param:
#		GPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march12
 ########ADD BY CK 2012.2.23
    fcmpo f31, f11
    bnel fail_fpr_test_intraword              #比较FPR31与FPRG11
    lfd f31,0(r9)
    lfd f11,0(r9)
    fcmpo f31, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)
   
    fcmpo f30, f11
    bnel fail_fpr_test_intraword              #比较FPR30与FPRG11
    lfd f30,0(r9)
    lfd f11,0(r9)
    fcmpo f30, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f29, f11
    bnel fail_fpr_test_intraword              #比较FPR29与FPRG11
    lfd f29,0(r9)
    lfd f11,0(r9)
    fcmpo f29, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f28, f11
    bnel fail_fpr_test_intraword              #比较FPR28与FPRG11
    lfd f28,0(r9)
    lfd f11,0(r9)
    fcmpo f28, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f27, f11
    bnel fail_fpr_test_intraword              #比较FPR27与FPRG11
    lfd f27,0(r9)
    lfd f11,0(r9)
    fcmpo f27, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f26, f11
    bnel fail_fpr_test_intraword              #比较FPR26与FPRG11
    lfd f26,0(r9)
    lfd f11,0(r9)
    fcmpo f26, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f25, f11
    bnel fail_fpr_test_intraword              #比较FPR25与FPRG11
    lfd f25,0(r9)
    lfd f11,0(r9)
    fcmpo f25, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f24, f11
    bnel fail_fpr_test_intraword              #比较FPR24与FPRG11
    lfd f24,0(r9)
    lfd f11,0(r9)
    fcmpo f24, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f23, f11
    bnel fail_fpr_test_intraword              #比较FPR23与FPRG11
    lfd f23,0(r9)
    lfd f11,0(r9)
    fcmpo f23, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f22, f11
    bnel fail_fpr_test_intraword              #比较FPR22与FPRG11
    lfd f22,0(r9)
    lfd f11,0(r9)
    fcmpo f22, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f21, f11
    bnel fail_fpr_test_intraword              #比较FPR21与FPRG11
    lfd f21,0(r9)
    lfd f11,0(r9)
    fcmpo f21, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f20, f11
    bnel fail_fpr_test_intraword              #比较FPR20与FPRG11
    lfd f20,0(r9)
    lfd f11,0(r9)
    fcmpo f20, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f19, f11
    bnel fail_fpr_test_intraword              #比较FPR19与FPRG11
    lfd f19,0(r9)
    lfd f11,0(r9)
    fcmpo f19, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f18, f11
    bnel fail_fpr_test_intraword              #比较FPR18与FPRG11
    lfd f18,0(r9)
    lfd f11,0(r9)
    fcmpo f18, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f17, f11
    bnel fail_fpr_test_intraword              #比较FPR17与FPRG11
    lfd f17,0(r9)
    lfd f11,0(r9)
    fcmpo f17, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f16, f11
    bnel fail_fpr_test_intraword              #比较FPR16与FPRG11
    lfd f16,0(r9)
    lfd f11,0(r9)
    fcmpo f16, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f15, f11
    bnel fail_fpr_test_intraword              #比较FPR15与FPRG11
    lfd f15,0(r9)
    lfd f11,0(r9)
    fcmpo f15, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f14, f11
    bnel fail_fpr_test_intraword              #比较FPR14与FPRG11
    lfd f14,0(r9)
    lfd f11,0(r9)
    fcmpo f14, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f13, f11
    bnel fail_fpr_test_intraword              #比较FPR13与FPRG11
    lfd f13,0(r9)
    lfd f11,0(r9)
    fcmpo f13, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f12, f11
    bnel fail_fpr_test_intraword              #比较FPR12与FPRG11
    lfd f12,0(r9)
    lfd f11,0(r9)
    fcmpo f12, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f10, f11
    bnel fail_fpr_test_intraword              #比较FPR10与FPRG11
    lfd f10,0(r9)
    lfd f11,0(r9)
    fcmpo f10, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f9, f11
    bnel fail_fpr_test_intraword              #比较FPR9与FPRG11
    lfd f9,0(r9)
    lfd f11,0(r9)
    fcmpo f9, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f8, f11
    bnel fail_fpr_test_intraword              #比较FPR8与FPRG11
    lfd f8,0(r9)
    lfd f11,0(r9)
    fcmpo f8, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f7, f11
    bnel fail_fpr_test_intraword              #比较FPR7与FPRG11
    lfd f7,0(r9)
    lfd f11,0(r9)
    fcmpo f7, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f6, f11
    bnel fail_fpr_test_intraword              #比较FPR6与FPRG11
    lfd f6,0(r9)
    lfd f11,0(r9)
    fcmpo f6, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f5, f11
    bnel fail_fpr_test_intraword              #比较FPR5与FPRG11
    lfd f5,0(r9)
    lfd f11,0(r9)
    fcmpo f5, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f4, f11
    bnel fail_fpr_test_intraword              #比较FPR4与FPRG11
    lfd f4,0(r9)
    lfd f11,0(r9)
    fcmpo f4, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f3, f11
    bnel fail_fpr_test_intraword              #比较FPR3与FPRG11
    lfd f3,0(r9)
    lfd f11,0(r9)
    fcmpo f3, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f2, f11
    bnel fail_fpr_test_intraword              #比较FPR2与FPRG11
    lfd f2,0(r9)
    lfd f11,0(r9)
    fcmpo f2, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f1, f11
    bnel fail_fpr_test_intraword              #比较FPR1与FPRG11
    lfd f1,0(r9)
    lfd f11,0(r9)
    fcmpo f1, f11
    bnel fail_fpr_test_intraword
    lfd f11,0(r10)

    fcmpo f0, f11
    bnel fail_fpr_test_intraword              #比较FPR0与FPRG11
    lfd f0,0(r9)
    lfd f11,0(r9)
    fcmpo f0, f11
    bnel fail_fpr_test_intraword


 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march13
#  Description:    
#       check intra-word faults through sequence           
#       March13:↑[r(0xFF00FF00), w(0xFFFF0000), w(0x0000FFFF),r(0x0000FFFF)]
#  Author:         wang fan 
#  Used module:    None
#  Input Param:
#		GPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march13
 
########ADD BY CK 2012.2.23
     fcmpo f0, f12
     bnel fail_fpr_test_intraword              #比较FPR0与FPRG12
     lfd f0,0(r11)
     lfd f0,0(r12)
     lfd f12,0(r12)
     fcmpo f0, f12
     bnel fail_fpr_test_intraword
     lfd f12,0(r9)
    
     fcmpo f1, f12
     bnel fail_fpr_test_intraword              #比较FPR1与FPRG12
     lfd f1,0(r11)
     lfd f1,0(r12)
     lfd f12,0(r12)
     fcmpo f1, f12
     bnel fail_fpr_test_intraword
     lfd f12,0(r9)

     fcmpo f2, f12
      bnel fail_fpr_test_intraword              #比较FPR2与FPRG12
      lfd f2,0(r11)
      lfd f2,0(r12)
      lfd f12,0(r12)
      fcmpo f2, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f3, f12
      bnel fail_fpr_test_intraword              #比较FPR3与FPRG12
      lfd f3,0(r11)
      lfd f3,0(r12)
      lfd f12,0(r12)
      fcmpo f3, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

    fcmpo f4, f12
      bnel fail_fpr_test_intraword              #比较FPR4与FPRG12
      lfd f4,0(r11)
      lfd f4,0(r12)
      lfd f12,0(r12)
      fcmpo f4, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f5, f12
      bnel fail_fpr_test_intraword              #比较FPR5与FPRG12
      lfd f5,0(r11)
      lfd f5,0(r12)
      lfd f12,0(r12)
      fcmpo f5, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f6, f12
      bnel fail_fpr_test_intraword              #比较FPR6与FPRG12
      lfd f6,0(r11)
      lfd f6,0(r12)
      lfd f12,0(r12)
      fcmpo f6, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f7, f12
      bnel fail_fpr_test_intraword              #比较FPR7与FPRG12
      lfd f7,0(r11)
      lfd f7,0(r12)
      lfd f12,0(r12)
      fcmpo f7, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f8, f12
      bnel fail_fpr_test_intraword              #比较FPR8与FPRG12
      lfd f8,0(r11)
      lfd f8,0(r12)
      lfd f12,0(r12)
      fcmpo f8, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f9, f12
      bnel fail_fpr_test_intraword              #比较FPR9与FPRG12
      lfd f9,0(r11)
      lfd f9,0(r12)
      lfd f12,0(r12)
      fcmpo f9, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f10, f12
      bnel fail_fpr_test_intraword              #比较FPR10与FPRG12
      lfd f10,0(r11)
      lfd f10,0(r12)
      lfd f12,0(r12)
      fcmpo f10, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f11, f12
      bnel fail_fpr_test_intraword              #比较FPR11与FPRG12
      lfd f11,0(r11)
      lfd f11,0(r12)
      lfd f12,0(r12)
      fcmpo f11, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f13, f12
      bnel fail_fpr_test_intraword              #比较FPR13与FPRG12
      lfd f13,0(r11)
      lfd f13,0(r12)
      lfd f12,0(r12)
      fcmpo f13, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f14, f12
      bnel fail_fpr_test_intraword              #比较FPR14与FPRG12
      lfd f14,0(r11)
      lfd f14,0(r12)
      lfd f12,0(r12)
      fcmpo f14, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f15, f12
      bnel fail_fpr_test_intraword              #比较FPR15与FPRG12
      lfd f15,0(r11)
      lfd f15,0(r12)
      lfd f12,0(r12)
      fcmpo f15, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f16, f12
      bnel fail_fpr_test_intraword              #比较FPR16与FPRG12
      lfd f16,0(r11)
      lfd f16,0(r12)
      lfd f12,0(r12)
      fcmpo f16, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f17, f12
      bnel fail_fpr_test_intraword              #比较FPR17与FPRG12
      lfd f17,0(r11)
      lfd f17,0(r12)
      lfd f12,0(r12)
      fcmpo f17, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f18, f12
      bnel fail_fpr_test_intraword              #比较FPR18与FPRG12
      lfd f18,0(r11)
      lfd f18,0(r12)
      lfd f12,0(r12)
      fcmpo f18, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f19, f12
      bnel fail_fpr_test_intraword              #比较FPR19与FPRG12
      lfd f19,0(r11)
      lfd f19,0(r12)
      lfd f12,0(r12)
      fcmpo f19, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f20, f12
      bnel fail_fpr_test_intraword              #比较FPR20与FPRG12
      lfd f20,0(r11)
      lfd f20,0(r12)
      lfd f12,0(r12)
      fcmpo f20, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f21, f12
      bnel fail_fpr_test_intraword              #比较FPR21与FPRG12
      lfd f21,0(r11)
      lfd f21,0(r12)
      lfd f12,0(r12)
      fcmpo f21, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f22, f12
      bnel fail_fpr_test_intraword              #比较FPR22与FPRG12
      lfd f22,0(r11)
      lfd f22,0(r12)
      lfd f12,0(r12)
      fcmpo f22, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f23, f12
      bnel fail_fpr_test_intraword              #比较FPR23与FPRG12
      lfd f23,0(r11)
      lfd f23,0(r12)
      lfd f12,0(r12)
      fcmpo f23, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f24, f12
      bnel fail_fpr_test_intraword              #比较FPR24与FPRG12
      lfd f24,0(r11)
      lfd f24,0(r12)
      lfd f12,0(r12)
      fcmpo f24, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f25, f12
      bnel fail_fpr_test_intraword              #比较FPR25与FPRG12
      lfd f25,0(r11)
      lfd f25,0(r12)
      lfd f12,0(r12)
      fcmpo f25, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f26, f12
      bnel fail_fpr_test_intraword              #比较FPR26与FPRG12
      lfd f26,0(r11)
      lfd f26,0(r12)
      lfd f12,0(r12)
      fcmpo f26, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f27, f12
      bnel fail_fpr_test_intraword              #比较FPR27与FPRG12
      lfd f27,0(r11)
      lfd f27,0(r12)
      lfd f12,0(r12)
      fcmpo f27, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f28, f12
      bnel fail_fpr_test_intraword              #比较FPR28与FPRG12
      lfd f28,0(r11)
      lfd f28,0(r12)
      lfd f12,0(r12)
      fcmpo f28, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f29, f12
      bnel fail_fpr_test_intraword              #比较FPR29与FPRG12
      lfd f29,0(r11)
      lfd f29,0(r12)
      lfd f12,0(r12)
      fcmpo f29, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f30, f12
      bnel fail_fpr_test_intraword              #比较FPR30与FPRG12
      lfd f30,0(r11)
      lfd f30,0(r12)
      lfd f12,0(r12)
      fcmpo f30, f12
      bnel fail_fpr_test_intraword
      lfd f12,0(r9)

     fcmpo f31, f12
      bnel fail_fpr_test_intraword              #比较FPR31与FPRG12
      lfd f31,0(r11)
      lfd f31,0(r12)
      lfd f12,0(r12)
      fcmpo f31, f12
      bnel fail_fpr_test_intraword
      
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march14
#  Description:    
#       check intra-word faults through sequence           
#       March14:↓[r(0x0000FFFF), w(0xFFFF0000),r(0xFFFF0000)]
#  Author:         wang fan 
#  Used module:    None
#  Input Param:
#		GPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march14
 ########ADD BY CK 2012.2.23
     fcmpo f31, f13
     bnel fail_fpr_test_intraword              #比较FPR31与FPRG13
     lfd f31,0(r11)
     lfd f13,0(r11)
     fcmpo f31, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f30, f13
     bnel fail_fpr_test_intraword              #比较FPR30与FPRG13
     lfd f30,0(r11)
     lfd f13,0(r11)
     fcmpo f30, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f29, f13
     bnel fail_fpr_test_intraword              #比较FPR29与FPRG13
     lfd f29,0(r11)
     lfd f13,0(r11)
     fcmpo f29, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f28, f13
     bnel fail_fpr_test_intraword              #比较FPR28与FPRG13
     lfd f28,0(r11)
     lfd f13,0(r11)
     fcmpo f28, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f27, f13
     bnel fail_fpr_test_intraword              #比较FPR27与FPRG13
     lfd f27,0(r11)
     lfd f13,0(r11)
     fcmpo f27, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f26, f13
     bnel fail_fpr_test_intraword              #比较FPR26与FPRG13
     lfd f26,0(r11)
     lfd f13,0(r11)
     fcmpo f26, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f25, f13
     bnel fail_fpr_test_intraword              #比较FPR25与FPRG13
     lfd f25,0(r11)
     lfd f13,0(r11)
     fcmpo f25, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f24, f13
     bnel fail_fpr_test_intraword              #比较FPR24与FPRG13
     lfd f24,0(r11)
     lfd f13,0(r11)
     fcmpo f24, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f23, f13
     bnel fail_fpr_test_intraword              #比较FPR23与FPRG13
     lfd f23,0(r11)
     lfd f13,0(r11)
     fcmpo f23, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f22, f13
     bnel fail_fpr_test_intraword              #比较FPR22与FPRG13
     lfd f22,0(r11)
     lfd f13,0(r11)
     fcmpo f22, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f21, f13
     bnel fail_fpr_test_intraword              #比较FPR21与FPRG13
     lfd f21,0(r11)
     lfd f13,0(r11)
     fcmpo f21, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f20, f13
     bnel fail_fpr_test_intraword              #比较FPR20与FPRG13
     lfd f20,0(r11)
     lfd f13,0(r11)
     fcmpo f20, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f19, f13
     bnel fail_fpr_test_intraword              #比较FPR19与FPRG13
     lfd f19,0(r11)
     lfd f13,0(r11)
     fcmpo f19, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f18, f13
     bnel fail_fpr_test_intraword              #比较FPR18与FPRG13
     lfd f18,0(r11)
     lfd f13,0(r11)
     fcmpo f18, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f17, f13
     bnel fail_fpr_test_intraword              #比较FPR17与FPRG13
     lfd f17,0(r11)
     lfd f13,0(r11)
     fcmpo f17, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f16, f13
     bnel fail_fpr_test_intraword              #比较FPR16与FPRG13
     lfd f16,0(r11)
     lfd f13,0(r11)
     fcmpo f16, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f15, f13
     bnel fail_fpr_test_intraword              #比较FPR15与FPRG13
     lfd f15,0(r11)
     lfd f13,0(r11)
     fcmpo f15, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f14, f13
     bnel fail_fpr_test_intraword              #比较FPR14与FPRG13
     lfd f14,0(r11)
     lfd f13,0(r11)
     fcmpo f14, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f12, f13
     bnel fail_fpr_test_intraword              #比较FPR12与FPRG13
     lfd f12,0(r11)
     lfd f13,0(r11)
     fcmpo f12, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f11, f13
     bnel fail_fpr_test_intraword              #比较FPR11与FPRG13
     lfd f11,0(r11)
     lfd f13,0(r11)
     fcmpo f11, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f10, f13
     bnel fail_fpr_test_intraword              #比较FPR10与FPRG13
     lfd f10,0(r11)
     lfd f13,0(r11)
     fcmpo f10, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f9, f13
     bnel fail_fpr_test_intraword              #比较FPR9与FPRG13
     lfd f9,0(r11)
     lfd f13,0(r11)
     fcmpo f9, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f8, f13
     bnel fail_fpr_test_intraword              #比较FPR8与FPRG13
     lfd f8,0(r11)
     lfd f13,0(r11)
     fcmpo f8, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f7, f13
     bnel fail_fpr_test_intraword              #比较FPR7与FPRG13
     lfd f7,0(r11)
     lfd f13,0(r11)
     fcmpo f7, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f6, f13
     bnel fail_fpr_test_intraword              #比较FPR6与FPRG13
     lfd f6,0(r11)
     lfd f13,0(r11)
     fcmpo f6, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f5, f13
     bnel fail_fpr_test_intraword              #比较FPR5与FPRG13
     lfd f5,0(r11)
     lfd f13,0(r11)
     fcmpo f5, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f4, f13
     bnel fail_fpr_test_intraword              #比较FPR4与FPRG13
     lfd f4,0(r11)
     lfd f13,0(r11)
     fcmpo f4, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f3, f13
     bnel fail_fpr_test_intraword              #比较FPR3与FPRG13
     lfd f3,0(r11)
     lfd f13,0(r11)
     fcmpo f3, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f2, f13
     bnel fail_fpr_test_intraword              #比较FPR2与FPRG13
     lfd f2,0(r11)
     lfd f13,0(r11)
     fcmpo f2, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f1, f13
     bnel fail_fpr_test_intraword              #比较FPR1与FPRG13
     lfd f1,0(r11)
     lfd f13,0(r11)
     fcmpo f1, f13
     bnel fail_fpr_test_intraword
     lfd f13,0(r12)

      fcmpo f0, f13
     bnel fail_fpr_test_intraword              #比较FPR0与FPRG13
     lfd f0,0(r11)
     lfd f13,0(r11)
     fcmpo f0, f13
     bnel fail_fpr_test_intraword

 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march15
#  Description:    
#		check intra-word faults through sequence 
#       March15:↑[r(0xF0FF0000FFFF0000)],[w(0xF0FFFFFF00000000)],[w(0x0F000000FFFFFFFF)],[r(0x0F000000FFFFFFFF)]
#  Author:         wang fan 
#  Used module:    None
#  Input Param:
#		GPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march15
########2012.2.23
     fcmpo f0, f14
     bnel fail_fpr_test_intraword              #比较FPR0与FPRG14
     lfd f0,0(r13)
     lfd f0,0(r14)
     lfd f14,0(r14)
     fcmpo f0, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

    fcmpo f1, f14
    bnel fail_fpr_test_intraword              #比较FPR1与FPRG14
    lfd f1,0(r13)
    lfd f1,0(r14)
    lfd f14,0(r14)
    fcmpo f1, f14
    bnel fail_fpr_test_intraword
    lfd f14,0(r11)

    fcmpo f2, f14
    bnel fail_fpr_test_intraword              #比较FPR2与FPRG14
    lfd f2,0(r13)
    lfd f2,0(r14)
    lfd f14,0(r14)
    fcmpo f2, f14
    bnel fail_fpr_test_intraword
    lfd f14,0(r11)

     fcmpo f3, f14
     bnel fail_fpr_test_intraword              #比较FPR3与FPRG14
     lfd f3,0(r13)
     lfd f3,0(r14)
     lfd f14,0(r14)
     fcmpo f3, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f4, f14
    bnel fail_fpr_test_intraword              #比较FPR4与FPRG14
    lfd f4,0(r13)
    lfd f4,0(r14)
    lfd f14,0(r14)
    fcmpo f4, f14
    bnel fail_fpr_test_intraword
    lfd f14,0(r11)

     fcmpo f5, f14
     bnel fail_fpr_test_intraword              #比较FPR5与FPRG14
     lfd f5,0(r13)
     lfd f5,0(r14)
     lfd f14,0(r14)
     fcmpo f5, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f6, f14
     bnel fail_fpr_test_intraword              #比较FPR6与FPRG14
     lfd f6,0(r13)
     lfd f6,0(r14)
     lfd f14,0(r14)
     fcmpo f6, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f7, f14
     bnel fail_fpr_test_intraword              #比较FPR7与FPRG14
     lfd f7,0(r13)
     lfd f7,0(r14)
     lfd f14,0(r14)
     fcmpo f7, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f8, f14
     bnel fail_fpr_test_intraword              #比较FPR8与FPRG14
     lfd f8,0(r13)
     lfd f8,0(r14)
     lfd f14,0(r14)
     fcmpo f8, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f9, f14
     bnel fail_fpr_test_intraword              #比较FPR9与FPRG14
     lfd f9,0(r13)
     lfd f9,0(r14)
     lfd f14,0(r14)
     fcmpo f9, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f10, f14
     bnel fail_fpr_test_intraword              #比较FPR10与FPRG14
     lfd f10,0(r13)
     lfd f10,0(r14)
     lfd f14,0(r14)
     fcmpo f10, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f11, f14
     bnel fail_fpr_test_intraword              #比较FPR11与FPRG14
     lfd f11,0(r13)
     lfd f11,0(r14)
     lfd f14,0(r14)
     fcmpo f11, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f12, f14
     bnel fail_fpr_test_intraword              #比较FPR12与FPRG14
     lfd f12,0(r13)
     lfd f12,0(r14)
     lfd f14,0(r14)
     fcmpo f12, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f13, f14
     bnel fail_fpr_test_intraword              #比较FPR13与FPRG14
     lfd f13,0(r13)
     lfd f13,0(r14)
     lfd f14,0(r14)
     fcmpo f13, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f15, f14
     bnel fail_fpr_test_intraword              #比较FPR15与FPRG14
     lfd f15,0(r13)
     lfd f15,0(r14)
     lfd f14,0(r14)
     fcmpo f15, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f16, f14
     bnel fail_fpr_test_intraword              #比较FPR16与FPRG14
     lfd f16,0(r13)
     lfd f16,0(r14)
     lfd f14,0(r14)
     fcmpo f16, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f17, f14
     bnel fail_fpr_test_intraword              #比较FPR17与FPRG14
     lfd f17,0(r13)
     lfd f17,0(r14)
     lfd f14,0(r14)
     fcmpo f17, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f18, f14
     bnel fail_fpr_test_intraword              #比较FPR18与FPRG14
     lfd f18,0(r13)
     lfd f18,0(r14)
     lfd f14,0(r14)
     fcmpo f18, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f19, f14
     bnel fail_fpr_test_intraword              #比较FPR19与FPRG14
     lfd f19,0(r13)
     lfd f19,0(r14)
     lfd f14,0(r14)
     fcmpo f19, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f20, f14
     bnel fail_fpr_test_intraword              #比较FPR20与FPRG14
     lfd f20,0(r13)
     lfd f20,0(r14)
     lfd f14,0(r14)
     fcmpo f20, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f21, f14
     bnel fail_fpr_test_intraword              #比较FPR21与FPRG14
     lfd f21,0(r13)
     lfd f21,0(r14)
     lfd f14,0(r14)
     fcmpo f21, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f22, f14
     bnel fail_fpr_test_intraword              #比较FPR22与FPRG14
     lfd f22,0(r13)
     lfd f22,0(r14)
     lfd f14,0(r14)
     fcmpo f22, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f23, f14
     bnel fail_fpr_test_intraword              #比较FPR23与FPRG14
     lfd f23,0(r13)
     lfd f23,0(r14)
     lfd f14,0(r14)
     fcmpo f23, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f24, f14
     bnel fail_fpr_test_intraword              #比较FPR24与FPRG14
     lfd f24,0(r13)
     lfd f24,0(r14)
     lfd f14,0(r14)
     fcmpo f24, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f25, f14
     bnel fail_fpr_test_intraword              #比较FPR25与FPRG14
     lfd f25,0(r13)
     lfd f25,0(r14)
     lfd f14,0(r14)
     fcmpo f25, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f26, f14
     bnel fail_fpr_test_intraword              #比较FPR26与FPRG14
     lfd f26,0(r13)
     lfd f26,0(r14)
     lfd f14,0(r14)
     fcmpo f26, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f27, f14
     bnel fail_fpr_test_intraword              #比较FPR27与FPRG14
     lfd f27,0(r13)
     lfd f27,0(r14)
     lfd f14,0(r14)
     fcmpo f27, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f28, f14
     bnel fail_fpr_test_intraword              #比较FPR28与FPRG14
     lfd f28,0(r13)
     lfd f28,0(r14)
     lfd f14,0(r14)
     fcmpo f28, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f29, f14
     bnel fail_fpr_test_intraword              #比较FPR29与FPRG14
     lfd f29,0(r13)
     lfd f29,0(r14)
     lfd f14,0(r14)
     fcmpo f29, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f30, f14
     bnel fail_fpr_test_intraword              #比较FPR30与FPRG14
     lfd f30,0(r13)
     lfd f30,0(r14)
     lfd f14,0(r14)
     fcmpo f30, f14
     bnel fail_fpr_test_intraword
     lfd f14,0(r11)

     fcmpo f31, f14
     bnel fail_fpr_test_intraword              #比较FPR31与FPRG14
     lfd f31,0(r13)
     lfd f31,0(r14)
     lfd f14,0(r14)
     fcmpo f31, f14
     bnel fail_fpr_test_intraword

 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march16
#  Description:    
#		check intra-word faults through sequence 
#       March16:↓[r(0x0F000000FFFFFFFF)],[w(0xF0FFFFFF00000000)],[r(0xF0FFFFFF00000000)]
#  Author:         wang fan 
#  Used module:    None
#  Input Param:
#		GPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march16
    fcmpo f31, f15
    bnel fail_fpr_test_intraword              #比较FPR31与FPRG15
    lfd f31,0(r13)
    lfd f15,0(r13)
    fcmpo f31, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

    fcmpo f30, f15
    bnel fail_fpr_test_intraword              #比较FPR30与FPRG15
    lfd f30,0(r13)
    lfd f15,0(r13)
    fcmpo f30, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f29, f15
    bnel fail_fpr_test_intraword              #比较FPR29与FPRG15
    lfd f29,0(r13)
    lfd f15,0(r13)
    fcmpo f29, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f28, f15
    bnel fail_fpr_test_intraword              #比较FPR28与FPRG15
    lfd f28,0(r13)
    lfd f15,0(r13)
    fcmpo f28, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f27, f15
    bnel fail_fpr_test_intraword              #比较FPR27与FPRG15
    lfd f27,0(r13)
    lfd f15,0(r13)
    fcmpo f27, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f26, f15
    bnel fail_fpr_test_intraword              #比较FPR26与FPRG15
    lfd f26,0(r13)
    lfd f15,0(r13)
    fcmpo f26, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f25, f15
    bnel fail_fpr_test_intraword              #比较FPR25与FPRG15
    lfd f25,0(r13)
    lfd f15,0(r13)
    fcmpo f25, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f24, f15
    bnel fail_fpr_test_intraword              #比较FPR24与FPRG15
    lfd f24,0(r13)
    lfd f15,0(r13)
    fcmpo f24, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f23, f15
    bnel fail_fpr_test_intraword              #比较FPR23与FPRG15
    lfd f23,0(r13)
    lfd f15,0(r13)
    fcmpo f23, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f22, f15
    bnel fail_fpr_test_intraword              #比较FPR22与FPRG15
    lfd f22,0(r13)
    lfd f15,0(r13)
    fcmpo f22, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f21, f15
    bnel fail_fpr_test_intraword              #比较FPR21与FPRG15
    lfd f21,0(r13)
    lfd f15,0(r13)
    fcmpo f21, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f20, f15
    bnel fail_fpr_test_intraword              #比较FPR20与FPRG15
    lfd f20,0(r13)
    lfd f15,0(r13)
    fcmpo f20, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f19, f15
    bnel fail_fpr_test_intraword              #比较FPR19与FPRG15
    lfd f19,0(r13)
    lfd f15,0(r13)
    fcmpo f19, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f18, f15
    bnel fail_fpr_test_intraword              #比较FPR18与FPRG15
    lfd f18,0(r13)
    lfd f15,0(r13)
    fcmpo f18, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f17, f15
    bnel fail_fpr_test_intraword              #比较FPR17与FPRG15
    lfd f17,0(r13)
    lfd f15,0(r13)
    fcmpo f17, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f16, f15
    bnel fail_fpr_test_intraword              #比较FPR16与FPRG15
    lfd f16,0(r13)
    lfd f15,0(r13)
    fcmpo f16, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f14, f15
    bnel fail_fpr_test_intraword              #比较FPR14与FPRG15
    lfd f14,0(r13)
    lfd f15,0(r13)
    fcmpo f14, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f13, f15
    bnel fail_fpr_test_intraword              #比较FPR13与FPRG15
    lfd f13,0(r13)
    lfd f15,0(r13)
    fcmpo f13, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f12, f15
    bnel fail_fpr_test_intraword              #比较FPR12与FPRG15
    lfd f12,0(r13)
    lfd f15,0(r13)
    fcmpo f12, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f11, f15
    bnel fail_fpr_test_intraword              #比较FPR11与FPRG15
    lfd f11,0(r13)
    lfd f15,0(r13)
    fcmpo f11, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f10, f15
    bnel fail_fpr_test_intraword              #比较FPR10与FPRG15
    lfd f10,0(r13)
    lfd f15,0(r13)
    fcmpo f10, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f9, f15
    bnel fail_fpr_test_intraword              #比较FPR9与FPRG15
    lfd f9,0(r13)
    lfd f15,0(r13)
    fcmpo f9, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f8, f15
    bnel fail_fpr_test_intraword              #比较FPR8与FPRG15
    lfd f8,0(r13)
    lfd f15,0(r13)
    fcmpo f8, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f7, f15
    bnel fail_fpr_test_intraword              #比较FPR7与FPRG15
    lfd f7,0(r13)
    lfd f15,0(r13)
    fcmpo f7, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f6, f15
    bnel fail_fpr_test_intraword              #比较FPR6与FPRG15
    lfd f6,0(r13)
    lfd f15,0(r13)
    fcmpo f6, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f5, f15
    bnel fail_fpr_test_intraword              #比较FPR5与FPRG15
    lfd f5,0(r13)
    lfd f15,0(r13)
    fcmpo f5, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f4, f15
    bnel fail_fpr_test_intraword              #比较FPR4与FPRG15
    lfd f4,0(r13)
    lfd f15,0(r13)
    fcmpo f4, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f3, f15
    bnel fail_fpr_test_intraword              #比较FPR3与FPRG15
    lfd f3,0(r13)
    lfd f15,0(r13)
    fcmpo f3, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f2, f15
    bnel fail_fpr_test_intraword              #比较FPR2与FPRG15
    lfd f2,0(r13)
    lfd f15,0(r13)
    fcmpo f2, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)

     fcmpo f1, f15
    bnel fail_fpr_test_intraword              #比较FPR1与FPRG15
    lfd f1,0(r13)
    lfd f15,0(r13)
    fcmpo f1, f15
    bnel fail_fpr_test_intraword
    lfd f15,0(r14)
    
    fcmpo f0, f15
    bnel fail_fpr_test_intraword              #比较FPR0与FPRG15
    lfd f0,0(r13)
    lfd f15,0(r13)
    fcmpo f0, f15
    bnel fail_fpr_test_intraword

 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march17
#  Description:    
#		check intra-word faults through sequence 
#       March17:↑[r(0xF0FFFFFF00000000)]
#  Author:         wang fan 
#  Used module:    None
#  Input Param:
#		GPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march17
    lfd f0,0(r13)
    fcmpo f0, f16
    bnel fail_fpr_test_intraword              #比较FPR0与FPRG16

    lfd f1,0(r13)
    fcmpo f1, f16
    bnel fail_fpr_test_intraword              #比较FPR1与FPRG16

    lfd f2,0(r13)
    fcmpo f2, f16
    bnel fail_fpr_test_intraword              #比较FPR2与FPRG16

    lfd f3,0(r13)
    fcmpo f3, f16
    bnel fail_fpr_test_intraword              #比较FPR3与FPRG16

    lfd f4,0(r13)
    fcmpo f4, f16
    bnel fail_fpr_test_intraword              #比较FPR4与FPRG16

    lfd f5,0(r13)
    fcmpo f5, f16
    bnel fail_fpr_test_intraword              #比较FPR5与FPRG16

    lfd f6,0(r13)
    fcmpo f6, f16
    bnel fail_fpr_test_intraword              #比较FPR6与FPRG16

    lfd f7,0(r13)
    fcmpo f7, f16
    bnel fail_fpr_test_intraword              #比较FPR7与FPRG16

    lfd f8,0(r13)
    fcmpo f8, f16
    bnel fail_fpr_test_intraword              #比较FPR8与FPRG16

    lfd f9,0(r13)
    fcmpo f9, f16
    bnel fail_fpr_test_intraword              #比较FPR9与FPRG16

    lfd f10,0(r13)
    fcmpo f10, f16
    bnel fail_fpr_test_intraword              #比较FPR10与FPRG16

    lfd f11,0(r13)
    fcmpo f11, f16
    bnel fail_fpr_test_intraword              #比较FPR11与FPRG16

    lfd f12,0(r13)
    fcmpo f12, f16
    bnel fail_fpr_test_intraword              #比较FPR12与FPRG16

    lfd f13,0(r13)
    fcmpo f13, f16
    bnel fail_fpr_test_intraword              #比较FPR13与FPRG16

    lfd f14,0(r13)
    fcmpo f14, f16
    bnel fail_fpr_test_intraword              #比较FPR14与FPRG16

    lfd f15,0(r13)
    fcmpo f15, f16
    bnel fail_fpr_test_intraword              #比较FPR15与FPRG16

    lfd f17,0(r13)
    fcmpo f17, f16
    bnel fail_fpr_test_intraword              #比较FPR17与FPRG16

    lfd f18,0(r13)
    fcmpo f18, f16
    bnel fail_fpr_test_intraword              #比较FPR18与FPRG16

    lfd f19,0(r13)
    fcmpo f19, f16
    bnel fail_fpr_test_intraword              #比较FPR19与FPRG16

    lfd f20,0(r13)
    fcmpo f20, f16
    bnel fail_fpr_test_intraword              #比较FPR20与FPRG16

    lfd f21,0(r13)
    fcmpo f21, f16
    bnel fail_fpr_test_intraword              #比较FPR21与FPRG16

    lfd f22,0(r13)
    fcmpo f22, f16
    bnel fail_fpr_test_intraword              #比较FPR22与FPRG16

    lfd f23,0(r13)
    fcmpo f23, f16
    bnel fail_fpr_test_intraword              #比较FPR23与FPRG16

    lfd f24,0(r13)
    fcmpo f24, f16
    bnel fail_fpr_test_intraword              #比较FPR24与FPRG16

    lfd f25,0(r13)
    fcmpo f25, f16
    bnel fail_fpr_test_intraword              #比较FPR25与FPRG16

    lfd f26,0(r13)
    fcmpo f26, f16
    bnel fail_fpr_test_intraword              #比较FPR26与FPRG16

    lfd f27,0(r13)
    fcmpo f27, f16
    bnel fail_fpr_test_intraword              #比较FPR27与FPRG16

    lfd f28,0(r13)
    fcmpo f28, f16
    bnel fail_fpr_test_intraword              #比较FPR28与FPRG16

    lfd f29,0(r13)
    fcmpo f29, f16
    bnel fail_fpr_test_intraword              #比较FPR29与FPRG16

    lfd f30,0(r13)
    fcmpo f30, f16
    bnel fail_fpr_test_intraword              #比较FPR30与FPRG16

    lfd f31,0(r13)
    fcmpo f31, f16
    bnel fail_fpr_test_intraword              #比较FPR31与FPRG16
   
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:     FPR_intraword_test
#  Description:   
#       It carries out the test for floating point registers to check FPR intra-word faults(couple faults)
#  Author:
#  Used module:    None
#  Input Param:
#		GPR3:Test outcome vector
#		GPR4:Mask(OR) to be applied to check vector in case of exit without errors
#		FPR0-FPR31:Registers where the test shall be carried out
#		SPRG4:GPR1 value
#		SPRG5:SWAP register for GPR1
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .globl FPRIntraWordTest
 
 FPRIntraWordTest:
     PrologStackOp
     StackCheckBegin
     
     PushParam 3      #keep imput parameter in stack
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
     
     mtspr SPRG4, 1

     lis r3,DBs00@h
     ori r3, r3, DBs00@l
     
     lis r4,DBs01@h
     ori r4, r4, DBs01@l

     lis r5,DBs10@h
     ori r5, r5, DBs10@l

     lis r6,DBs11@h
     ori r6, r6, DBs11@l

     lis r7,DBs20@h
     ori r7, r7, DBs20@l

     lis r8,DBs21@h
     ori r8, r8, DBs21@l

     lis r9,DBs30@h
     ori r9, r9, DBs30@l

     lis r10,DBs31@h
     ori r10, r10, DBs31@l

     lis r11,DBs40@h
     ori r11, r11, DBs40@l

     lis r12,DBs41@h
     ori r12, r12, DBs41@l

########ADD BY CK 2012.2.23
    lis r13,DBs50@h
    ori r13, r13, DBs50@l

    lis r14,DBs51@h
    ori r14, r14, DBs51@l

    lis r15,DBs60@h
    ori r15, r15, DBs60@l

    lis r16,DBs61@h
    ori r16, r16, DBs61@l
########ADD END
     march5        #call march algorithm 
     march6
     march7
     march8
     march9
     march10
     march11
     march12
     march13
     march14
     march15
     march16
     march17
     
     mfspr 1, SPRG4
     TestConclusion
     b end_FPRInterWordTest
     
fail_fpr_test_intraword:
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
 .end
