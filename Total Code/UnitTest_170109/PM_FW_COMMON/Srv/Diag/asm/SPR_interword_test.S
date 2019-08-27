#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march0
#  Description:    
#       Set xer and ctr special value.
#  Author:
#  Used module:    None
#  Input Param:
#		GPR20:DB0
#		GPR30:DB0 masked
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march0
       mtspr XER, 30
	   mtspr CTR, 20
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march1
#  Description:    
#       Read ctr content out and compare the value  with preset one, then write a value            
#       into ctr and read out to compare with the preset one, at last write another            
#       value into ctr, and the same with xer.
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:GPR19
#		GPR20:DB0
#		GPR21:DB1
#		GPR30:DB0 masked
#		GPR31:DB1 masked
#		SPR 1:XER
#		SPR 9:Count Register (CTR) 
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march1
       mfspr 19, CTR
	   cmplw 19, 20
	   bnel FailSPRInterwordTest
	   mfspr 19, CTR
	   cmplw 19, 20
	   bnel FailSPRInterwordTest
	   mtspr CTR, 20
	   mfspr 19, CTR
	   cmplw 19, 20
	   bnel FailSPRInterwordTest
	   mtspr CTR, 21
	   mfspr 19, XER
	   cmplw 19, 30
	   bnel FailSPRInterwordTest
	   mfspr 19, XER
	   cmplw 19, 30
	   bnel FailSPRInterwordTest
	   mtspr XER, 30
	   mfspr 19, XER
	   cmplw 19, 30
	   bnel FailSPRInterwordTest
	   mtspr XER, 31
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march2
#  Description:    
#       The same with march1, only the operation sequence is opposite with march1.
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:GPR19
#		GPR20:DB0
#		GPR21:DB1
#		GPR30:DB0 masked
#		GPR31:DB1 masked
#		SPR 1:XER
#		SPR 9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march2
       mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRInterwordTest
	   mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRInterwordTest
	   mtspr XER, 31
	   mfspr 19, XER
	   cmplw 19, 31	   
	   bnel FailSPRInterwordTest
	   mtspr XER, 30
	   mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRInterwordTest
	   mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRInterwordTest
	   mtspr CTR, 21
	   mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRInterwordTest
	   mtspr CTR, 20
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march3
#  Description:    
#       The same with march2, only data background is different.
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:GPR19
#		GPR20:DB0
#		GPR21:DB1
#		GPR30:DB0 masked
#		GPR31:DB1 masked
#		SPR 1:XER
#		SPR 9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march3
       mfspr 19, XER
	   cmplw 19, 30
	   bnel FailSPRInterwordTest
	   mfspr 19, XER
	   cmplw 19, 30
	   bnel FailSPRInterwordTest
	   mtspr XER, 30
	   mfspr 19, XER
	   cmplw 19, 30
	   bnel FailSPRInterwordTest
	   mtspr XER, 31
	   mfspr 19, CTR
	   cmplw 19, 20
	   bnel FailSPRInterwordTest
	   mfspr 19, CTR
	   cmplw 19, 20
	   bnel FailSPRInterwordTest
	   mtspr CTR, 20
	   mfspr 19, CTR
	   cmplw 19, 20
	   bnel FailSPRInterwordTest
	   mtspr CTR, 21
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march4
#  Description:    
#       The same with march3, only data background is different.
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:GPR19
#		GPR20:DB0
#		GPR21:DB1
#		GPR30:DB0 masked
#		GPR31:DB1 masked
#		SPR 1:XER
#		SPR 9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march4
       mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRInterwordTest
	   mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRInterwordTest
	   mtspr XER, 31
	   mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRInterwordTest
	   mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRInterwordTest
	   mtspr XER, 30
	   mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRInterwordTest
	   mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRInterwordTest
	   mtspr CTR, 21
	   mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRInterwordTest
	   mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRInterwordTest
	   mtspr CTR, 20
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march5
#  Description:    
#       Read xer content out and compare with the preset value,the same with ctr.
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:GPR19
#		GPR20:DB0
#		GPR30:DB0 masked
#		SPR 1:XER
#		SPR 9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march5
       mfspr 19, XER
	   cmplw 19, 30
	   bnel FailSPRInterwordTest
	   mfspr 19, CTR
	   cmplw 19, 20
	   bnel FailSPRInterwordTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      SPRInterwordTest
#  Description:    
#       It carries out the test for special-purpose registers (interword) to test
#		SPR inter-word faults(faults types: AFs,TFs,SAFs,etc).
#  Author:
#  Used module:    None
#  Input Param:
#       GPR3:Test outcome vector
#		GPR4:Mask(OR) to be applied to check vector in case of exit without errors
#		GPR0:Address of calling function
#		GPR2:Address of error memory area
#		GPR19:Compare register
#		GPR20:First pattern (DB0)
#		GPR21:Second pattern (DB1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl SPRInterwordTest

SPRInterwordTest:
               
	   PrologStackOp
	   StackCheckBegin
       
	   PushParam(3)
	   PushParam(4)

	   StackCheckEnd
	   StoreGPRs2Stack
	   StoreCtrXerCr2Stack

	   stw 31, OFFSET_PUNT_STACK_CORR(1)
	   TestCPUStart

	   lis 2, 0xE000           #set mask
	   ori 2, 2, 0x007F
	   lis 20, DB0_HI
	   ori 20, 20, DB0_LO
	   not 21, 20              #r21 = 0xFFFFFFFF
	   and 30, 2, 20           #r30 = 0x00000000
	   and 31, 2, 21           #r31 = 0xE000007F

	   march0
	   march1
	   march2
	   march3
	   march4
	   march5
       
	   TestConclusion
	   b EndSPRInterwordTest

FailSPRInterwordTest:

       ErrorHandling

EndSPRInterwordTest:
       
	   StackEpilogBegin
	   RestoreCtrXerCr
	   StackEpilogEnd
	   blr
 .end
