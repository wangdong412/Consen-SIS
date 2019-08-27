#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march5
#  Description:    
#       test XER and CTR by write special value,then read out ,            
#       compare if the value is the last write one,the data background is(0xA000002A,0x40000055)
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:Read value
#		GPR20:DB2
#		GPR21:DB3
#		GPR30:DB2 Mask
#		GPR31:DB3 Mask
#		SPR 1:XER
#		SPR 9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march5
       mtspr XER, 30            #
	   mtspr XER, 31
	   mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRIntrawordTest
       mtspr CTR, 22
	   mtspr CTR, 23
	   mfspr 19, CTR
	   cmplw 19, 23
	   bnel FailSPRIntrawordTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march6
#  Description:    
#       Test XER and CTR by write special value,then read out ,            
#       compare if the value is the last write one.
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:Read value
#		GPR20:DB2
#		GPR21:DB3
#		GPR30:DB2 Mask
#		GPR31:DB3 Mask
#		SPR 1:XER
#		SPR 9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march6
       mfspr 19, CTR
	   cmplw 19, 23
	   bnel FailSPRIntrawordTest
	   mtspr CTR, 20
	   mfspr 19, CTR
	   cmplw 19, 20
	   bnel FailSPRIntrawordTest
	   mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRIntrawordTest
	   mtspr XER, 30
	   mfspr 19, XER
	   cmplw 19, 30
	   bnel FailSPRIntrawordTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march7
#  Description:    
#       Test xer and ctr ,the step is:read xer out, then compare with a preset             
#       value to check if is equal, then write a new value and read out to check if           
#       there is no bits errors, the same is ctr data background is(0xC000004C,0x20000033).
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:Read value
#		GPR20:DB4
#		GPR21:DB5
#		GPR22:DB2
#		GPR29:DB2 Mask
#		GPR30:DB4 Mask
#		GPR31:DB5 Mask
#		SPR1:XER
#		SPR9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march7
       mfspr 19, XER
	   cmplw 19, 29
	   bnel FailSPRIntrawordTest
	   mtspr XER, 30
	   mtspr XER, 31
	   mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRIntrawordTest
	   mfspr 19, CTR
	   cmplw 19, 22
	   bnel FailSPRIntrawordTest
	   mtspr CTR, 20
	   mtspr CTR, 21
	   mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRIntrawordTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march8
#  Description:    
#       Test xer and ctr ,the step is:read xer out, then compare with a preset            
#       value to check if is equal, then write a new value and read out to check if            
#       there is no bits errors, the same is ctr, the diagnosis sequence of xer and ctr            
#       is diposite with march7.
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:Read value
#		GPR20:DB4
#		GPR21:DB5
#		GPR30:DB4 Mask
#		GPR31:DB5 Mask
#		SPR1:XER
#		SPR9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march8
       mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRIntrawordTest
	   mtspr CTR, 20
	   mfspr 19, CTR
	   cmplw 19, 20
	   bnel FailSPRIntrawordTest
	   mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRIntrawordTest
	   mtspr XER, 30
	   mfspr 19, XER
	   cmplw 19, 30
	   bnel FailSPRIntrawordTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march9
#  Description:    
#       Test xer and ctr ,the step is:read xer out, then compare with a preset            
#       value to check if is equal, then write a new value and read out to check if            
#       there is no bits errors, the same is ctr data background is(0xC000004C,0x20000033).
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:Read value
#		GPR20:DB6
#		GPR21:DB7
#		GPR22:DB4
#		GPR29:DB4 Mask
#		GPR30:DB6 Mask
#		GPR31:DB7 Mask
#		SPR1:XER
#		SPR9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march9
       mfspr 19, XER
	   cmplw 19, 29
	   bnel FailSPRIntrawordTest
	   mtspr XER, 30
	   mtspr XER, 31
	   mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRIntrawordTest
	   mfspr 19, CTR
	   cmplw 19, 22
	   bnel FailSPRIntrawordTest
	   mtspr CTR, 20
	   mtspr CTR, 21
	   mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRIntrawordTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march10
#  Description:    
#       Test xer and ctr ,the step is:read xer out, then compare with a preset            
#       value to check if is equal, then write a new value and read out to check if            
#       there is no bits errors, the same is ctr, the diagnosis sequence of xer and ctr            
#       is diposite with march9.
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:Read value
#		GPR20:DB6
#		GPR21:DB7
#		GPR30:DB6 Mask
#		GPR31:DB7 Mask
#		SPR1:XER
#		SPR9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march10
       mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRIntrawordTest
       mtspr CTR, 20
	   mfspr 19, CTR
	   cmplw 19, 20
	   bnel FailSPRIntrawordTest
	   mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRIntrawordTest
	   mtspr CTR, 20
	   mfspr 19, CTR
	   cmplw 19, 20
	   bnel FailSPRIntrawordTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march11
#  Description:    
#       Test xer and ctr ,the step is:read xer out, then compare with a preset            
#       value to check if is equal, then write a new value and read out to check if            
#       there is no bits errors, the same is ctr data background is(0xC000004C,0x20000033).
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:Read value
#		GPR20:DB8
#		GPR21:DB9
#		GPR22:DB6
#		GPR29:DB6 Mask
#		GPR30:DB8 Mask
#		GPR31:DB9 Mask
#		SPR1:XER
#		SPR9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march11
       mfspr 19, XER
	   cmplw 19, 29
	   bnel FailSPRIntrawordTest
	   mtspr XER, 30
	   mtspr XER, 31
	   mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRIntrawordTest
	   mfspr 19, CTR
	   cmplw 19, 22
	   bnel FailSPRIntrawordTest
	   mtspr CTR, 20
	   mtspr CTR, 21
	   mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRIntrawordTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march12
#  Description:     
#       Test xer and ctr ,the step is:read xer out, then compare with a preset          
#       value to check if is equal, then write a new value and read out to check if            
#       there is no bits errors, the same is ctr, the diagnosis sequence of xer and ctr            
#       is diposite with march11.
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:Read value
#		GPR20:DB8
#		GPR21:DB9
#		GPR30:DB8 Mask
#		GPR31:DB9 Mask
#		SPR1:XER
#		SPR9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march12
       mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRIntrawordTest
	   mtspr CTR, 20
	   mfspr 19, CTR
	   cmplw 19, 20
	   bnel FailSPRIntrawordTest
	   mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRIntrawordTest
	   mtspr XER, 30
	   mfspr 19, XER
	   cmplw 19, 30
	   bnel FailSPRIntrawordTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march13
#  Description:    
#       Test xer and ctr ,the step is:read xer out, then compare with a preset            
#       value to check if is equal, then write a new value and read out to check if            
#       there is no bits errors, the same is ctr data background is(0xC000004C,0x20000033).
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:Read value
#		GPR20:DB10
#		GPR21:DB11
#		GPR22:DB8
#		GPR29:DB8 Mask
#		GPR30:DB10 Mask
#		GPR31:DB11 Mask
#		SPR1:XER
#		SPR9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march13
       mfspr 19, XER
	   cmplw 19, 29
	   bnel FailSPRIntrawordTest
	   mtspr XER, 30
	   mtspr XER, 31
	   mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRIntrawordTest
	   mfspr 19, CTR
	   cmplw 19, 22
	   bnel FailSPRIntrawordTest
	   mtspr CTR, 20
	   mtspr CTR, 21
	   mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRIntrawordTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march14
#  Description:     
#       Test xer and ctr ,the step is:read xer out, then compare with a preset           
#       value to check if is equal, then write a new value and read out to check if            
#       there is no bits errors, the same is ctr, the diagnosis sequence of xer and ctr            
#       is diposite with march13.
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:Read value
#		GPR20:DB10
#		GPR21:DB11
#		GPR30:DB10 Mask
#		GPR31:DB11 Mask
#		SPR1:XER
#		SPR9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march14
       mfspr 19, CTR
	   cmplw 19, 21
	   bnel FailSPRIntrawordTest
	   mtspr CTR, 20
	   mfspr 19, CTR
	   cmplw 19, 20
	   bnel FailSPRIntrawordTest
	   mfspr 19, XER
	   cmplw 19, 31
	   bnel FailSPRIntrawordTest
	   mtspr XER, 30
	   mfspr 19, XER
	   cmplw 19, 30
	   bnel FailSPRIntrawordTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march15
#  Description:    
#       Read xer and ctr and compare with the preset value to check if is equal.
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:Read value
#		GPR20:DB10
#		GPR30:DB10 Mask
#		SPR1:XER
#		SPR9:Count Register (CTR)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march15
       mfspr 19, XER
	   cmplw 19, 30
	   bnel FailSPRIntrawordTest
	   mfspr 19, CTR
	   cmplw 19, 20
	   bnel FailSPRIntrawordTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      SPRIntrawordTest
#  Description:    
#       It carries out the test for special-purpose registers (intraword) to test 
#		SPR intra-word faults(couple faults)
#  Author:
#  Used module:    None
#  Input Param:
#       GPR3:Test outcome vector
#		GPR4:Mask(OR) to be applied to check vector in case of exit without errors
#		GPR0:Address of calling function
#		GPR2:Address of error memory area
#		GPR19:Compare register
#		GPR20:pattern (DB2-4-6-8-10)
#		GPR21:pattern (DB3-5-7-9-11)
#		GPR22:pattern (DB2-4-6-8)
#		GPR29:pattern (DB2-4-6-8) masked
#		GPR30:pattern (DB2-4-6-8-10) masked
#		GPR31:pattern (DB3-5-7-9-11) masked
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl SPRIntrawordTest

SPRIntrawordTest:
               
	    PrologStackOp
		StackCheckBegin

		PushParam(3)
		PushParam(4)

		StackCheckEnd
	    StoreGPRs2Stack
	    StoreCtrXerCr2Stack

		stw 31, OFFSET_PUNT_STACK_CORR(1)
		TestCPUStart

		lis 2, 0xE000
		ori 2, 2, 0x007F
	    lis 20, DB2_HI
		ori 20, 20, DB2_LO 
		not 21, 20
		and 30, 2, 20
		and 31, 2, 21
		march5
		march6

		mr 22, 20
		mr 29, 30
		lis 20, DB4_HI
		ori 20, 20, DB4_LO
		not 21, 20
		and 30, 2, 20
		and 31, 2, 21
		march7
		march8

		mr 22, 20
		mr 29, 30
		lis 20, DB6_HI
		ori 20, 20, DB6_LO
		not 21, 20
		and 30, 2, 20
		and 31, 2, 21
		march9 
		march10

		mr 22, 20
		li 29, 0x000F
		lis 20, DB8_HI
		ori 20, 20, DB8_LO
		not 21, 20
		and 30, 2, 20
		and 31, 2, 21
		march11 
		march12

		mr 22, 20
		mr 29, 30
		lis 20, DB10_HI
		ori 20, 20, DB10_LO
		not 21, 20
		and 30, 2, 20
		and 31, 2, 21
		march13
		march14
		march15

	    TestConclusion
		b EndSPRIntrawordTest

FailSPRIntrawordTest:

	    ErrorHandling

EndSPRIntrawordTest:
	    
		StackEpilogBegin
		RestoreCtrXerCr
		StackEpilogEnd
		blr
 .end
