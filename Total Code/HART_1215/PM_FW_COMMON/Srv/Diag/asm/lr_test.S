#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march0
#  Description: 
#		It executes March test number 0.
#  Author:          
#  Used module:    None
#  Input Param:
#		GPR20:DB0
#		LR:Link Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march0
    mtlr 20
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march1
#  Description: 
#       It executes March test number 1.
#  Author:         
#  Used module:    None
#  Input Param:
#       GPR19:GPR19
#		GPR20:DB0
#		GPR21:DB1
#		LR:Link Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march1
    mtlr 21
	mflr 19
	cmplw 19, 21
	bne fail_LR_test
	mflr 19
	cmplw 19, 21
	bne fail_LR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march2
#  Description: 
#       It executes March test number 2.
#  Author:          
#  Used module:    None
#  Input Param:
#       GPR19:GPR19
#		GPR20:DB0
#		LR:Link Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march2
    mtlr 20
	mflr 19
	cmplw 19, 20
	bne fail_LR_test
	mflr 19
	cmplw 19, 20
	bne fail_LR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march3
#  Description: 
#       It executes March test number 3.
#  Author:          
#  Used module:    None
#  Input Param:
#       GPR19:GPR19
#		GPR22:DB2
#		GPR23:DB3
#		LR:Link Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march3
    mtlr 22
	mtlr 23
	mflr 19
	cmplw 19, 23
	bne fail_LR_test
	mflr 19
	cmplw 19, 23
	bne fail_LR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march4
#  Description: 
#       It executes March test number 4.
#  Author:          
#  Used module:    None
#  Input Param:
#       GPR19:GPR19
#		GPR22:DB2
#		GPR23:DB3
#		LR:Link Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march4
     mtlr 22
	 mflr 19
	 cmplw 19, 22
	 bne fail_LR_test
	 mflr 19
	 cmplw 19, 22
	 bne fail_LR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march5
#  Description: 
#       It executes March test number 5.
#  Author:          
#  Used module:    None
#  Input Param:    
#       GPR19:GPR19
#		GPR24:DB4
#		GPR25:DB5
#		LR:Link Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march5
     mtlr 24
	 mtlr 25
	 mflr 19
	 cmplw 19, 25
	 bne fail_LR_test
	 mflr 19
	 cmplw 19, 25
	 bne fail_LR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march6
#  Description: 
#       It executes March test number 6.
#  Author:          
#  Used module:    None
#  Input Param:    
#       GPR19:GPR19
#		GPR24:DB4
#		GPR25:DB5
#		LR:Link Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march6
     mtlr 24
	 mflr 19
	 cmplw 19, 24
	 bne fail_LR_test
	 mflr 19
	 cmplw 19, 24
	 bne fail_LR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march7
#  Description: 
#       It executes March test number 7.
#  Author:          
#  Used module:    None
#  Input Param:    
#       GPR19:GPR19
#		GPR26:DB6
#		GPR27:DB7
#		LR:Link Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 

 .macro march7
     mtlr 26
	 mtlr 27
	 mflr 19
	 cmplw 19, 27
	 bne fail_LR_test
	 mflr 19
	 cmplw 19, 27
	 bne fail_LR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march8
#  Description: 
#       It executes March test number 8.
#  Author:          
#  Used module:    None
#  Input Param:    
#       GPR19:GPR19
#		GPR26:DB6
#		GPR27:DB7
#		LR:Link Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march8
     mtlr 26
	 mflr 19
	 cmplw 19, 26
	 bne fail_LR_test
	 mflr 19
	 cmplw 19, 26
	 bne fail_LR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march9
#  Description: 
#       It executes March test number 9.
#  Author:          
#  Used module:    None
#  Input Param:    
#       GPR19:GPR19
#		GPR28:DB8
#		GPR29:DB9
#		LR:Link Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march9
     mtlr 28
	 mtlr 29
	 mflr 19
	 cmplw 19, 29
	 bne fail_LR_test
	 mflr 19
	 cmplw 19, 29
	 bne fail_LR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march10
#  Description: 
#       It executes March test number 10.
#  Author:          
#  Used module:    None
#  Input Param:    
#       GPR19:GPR19
#		GPR28:DB8
#		GPR29:DB9
#		LR:Link Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march10
     mtlr 28
	 mflr 19
	 cmplw 19, 28
	 bne fail_LR_test
	 mflr 19
	 cmplw 19, 28
	 bne fail_LR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march11
#  Description: 
#       It executes March test number 11.
#  Author:          
#  Used module:    None
#  Input Param:    
#       GPR19:GPR19
#		GPR30:DB10
#		GPR31:DB11
#		LR:Link Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march11
     mtlr 30
	 mtlr 31
	 mflr 19
	 cmplw 19, 31
	 bne fail_LR_test
	 
	 mflr 19
	 cmplw 19, 31
	 bne fail_LR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march12
#  Description: 
#       It executes March test number 12.
#  Author:         
#  Used module:    None
#  Input Param:    
#       GPR19:GPR19
#		GPR30:DB10
#		GPR31:DB11
#		LR:Link Register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

 .macro march12
     mtlr 30
	 mflr 19
	 cmplw 19, 30
	 bne fail_LR_test
	 mflr 19
	 cmplw 19, 30
	 bne fail_LR_test
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      LR_test
#  Description: 
#       Testing of LR register.
#  Author:           
#  Used module:    None
#  Input Param:
#       GPR3:Test outcome vector
#		GPR4:Mask(OR) to be applied to check vector in case of exit without errors
#		GPR0:Address of calling function
#		GPR2:Address of error memory area
#		GPR19:Compare values
#		GPR20:Pattern (DB0)
#		GPR21:Pattern (DB1)
#		GPR22:Pattern (DB2)
#		GPR23:Pattern (DB3)
#		GPR24:Pattern (DB4)
#		GPR25:Pattern (DB5)
#		GPR26:Pattern (DB6)
#		GPR27:Pattern (DB7)
#		GPR28:Pattern (DB8)
#		GPR29:Pattern (DB9)
#		GPR30:Pattern (DB10)
#		GPR31:Pattern (DB11)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

    .globl TestLR
TestLR:
    PrologStackOp
    StackCheckBegin
    PushParam(3)
    PushParam(4)
    StackCheckEnd
    StoreGPRs2Stack
    StoreCtrXerCr2Stack
	stw 31, OFFSET_PUNT_STACK_CORR(1)
	TestCPUStart

	lis 20, DB0_HI
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
	b end_LR_test

fail_LR_test:
	ErrorHandling

end_LR_test:
   
    StackEpilogBegin
	RestoreCtrXerCr
    StackEpilogEnd
	blr
