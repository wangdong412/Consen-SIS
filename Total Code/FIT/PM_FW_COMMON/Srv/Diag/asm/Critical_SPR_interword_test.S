#include "../inc/common_macro.s"
#include "../inc/common_def.h"
 
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march0
#  Description:    
#       It executes March test number 0.
#  Author:
#  Used module:    None
#  Input Param:
#		GPR20:DB0
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march0
        mtspr 25, 20            #spr25 is sdr1
        mtspr 26, 20            #spr26 is srr0
        mtspr 27, 20            #spr27 is srr1
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march1
#  Description:    
#       test spr25-27 using word-oriendrd memories march  algorithm,           
#       write specifical value and read out to compare
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:GPR19
#		GPR21:DB1
#		GPR20:DB0
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march1
        mfspr 19, 27
        cmplw 19, 20
        bnel FailCritiCalSprTestInter
        mfspr 19, 27
        cmplw 19, 20
        bnel FailCritiCalSprTestInter
        mtspr 27, 20
        mfspr 19, 27
        cmplw 19, 20
        bnel FailCritiCalSprTestInter
        mtspr 27, 21

        mfspr 19, 26
        cmplw 19, 20
        bnel FailCritiCalSprTestInter
        mfspr 19, 26
        cmplw 19, 20
        bnel FailCritiCalSprTestInter
        mtspr 26, 20
        mfspr 19, 26
        cmplw 19, 20
        bnel FailCritiCalSprTestInter
        mtspr 26, 21  
        
        mfspr 19, 25
        and 19, 19, 18
        and 17, 20, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestInter
        mfspr 19, 25
        and 19, 19, 18
        and 17, 20, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestInter
        mtspr 25, 20
        mfspr 19, 25
        and 19, 19, 18
        and 17, 20, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestInter
        mtspr 25, 21   
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march2
#  Description:    
#       test spr25-27 by write - read value of spr and compare 
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:GPR19
#		GPR21:DB1
#		GPR20:DB0
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/  
 .macro march2
        mfspr 19, 25
        and 19, 19, 18
        and 17, 21, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestInter
        mfspr 19, 25
        and 19, 19, 18
        and 17, 21, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestInter
        mtspr 25, 21
        mfspr 19, 25
        and 19, 19, 18
        and 17, 21, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestInter
        mtspr 25, 20
        
        mfspr 19, 26
        cmplw 19, 21
        bnel FailCritiCalSprTestInter
        mfspr 19, 26
        cmplw 19, 21
        bnel FailCritiCalSprTestInter
        mtspr 26, 21
        mfspr 19, 26
        cmplw 19, 21
        bnel FailCritiCalSprTestInter
        mtspr 26, 20
        
        mfspr 19, 27
        cmplw 19, 21
        bnel FailCritiCalSprTestInter
        mfspr 19, 27
        cmplw 19, 21
        bnel FailCritiCalSprTestInter
        mtspr 27, 21
        mfspr 19, 27
        cmplw 19, 21
        bnel FailCritiCalSprTestInter
        mtspr 27, 20
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march3
#  Description:    
#       test spr25-27 by write - read value of spr and compare 
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:GPR19
#		GPR21:DB1
#		GPR20:DB0
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march3
        mfspr 19, 25
        and 19, 19, 18
        and 17, 20, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestInter
        mfspr 19, 25
        and 19, 19, 18
        and 17, 20, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestInter
        mtspr 25, 20
        mfspr 19, 25
        and 19, 19, 18
        and 17, 20, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestInter
        mtspr 25, 21
        
        mfspr 19, 26
        cmplw 19, 20
        bnel FailCritiCalSprTestInter
        mfspr 19, 26
        cmplw 19, 20
        bnel FailCritiCalSprTestInter
        mtspr 26, 20
        mfspr 19, 26
        cmplw 19, 20
        bnel FailCritiCalSprTestInter
        mtspr 26, 21
 
        mfspr 19, 27
        cmplw 19, 20
        bnel FailCritiCalSprTestInter
        mfspr 19, 27
        cmplw 19, 20
        bnel FailCritiCalSprTestInter
        mtspr 27, 20
        mfspr 19, 27
        cmplw 19, 20
        bnel FailCritiCalSprTestInter
        mtspr 27, 21 
 .endm
     
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march4
#  Description:    
#       test spr25-27 by write - read value of spr and compare 
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:GPR19
#		GPR21:DB1
#		GPR20:DB0
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/   
 .macro march4
        mfspr 19, 25
        and 19, 19, 18
        and 17, 21, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestInter
        mfspr 19, 25
        and 19, 19, 18
        and 17, 21, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestInter
        mtspr 25, 21
        mfspr 19, 25
        and 19, 19, 18
        and 17, 21, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestInter
        mfspr 19, 25
        and 19, 19, 18
        and 17, 21, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestInter
        mtspr 25, 20
        
        mfspr 19, 26
        cmplw 19, 21
        bnel FailCritiCalSprTestInter
        mfspr 19, 26
        cmplw 19, 21
        bnel FailCritiCalSprTestInter
        mtspr 26, 21
        mfspr 19, 26
        cmplw 19, 21
        bnel FailCritiCalSprTestInter
        mfspr 19, 26
        cmplw 19, 21
        bnel FailCritiCalSprTestInter
        mtspr 26, 20
        
        mfspr 19, 27
        cmplw 19, 21
        bnel FailCritiCalSprTestInter
        mfspr 19, 27
        cmplw 19, 21
        bnel FailCritiCalSprTestInter
        mtspr 27, 21
        mfspr 19, 27
        cmplw 19, 21
        bnel FailCritiCalSprTestInter
        mfspr 19, 27
        cmplw 19, 21
        bnel FailCritiCalSprTestInter
        mtspr 27, 20 
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march5
#  Description:    
#       test spr25-27 by write - read value of spr and compare 
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:GPR19
#		GPR21:DB1
#		GPR20:DB0
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/  
 .macro march5
        mfspr 19, 25
        and 19, 19, 18
        and 17, 20, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestInter
        mfspr 19, 26
        cmplw 19, 20
        bnel FailCritiCalSprTestInter
        mfspr 19, 27
        cmplw 19, 20
        bnel FailCritiCalSprTestInter
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      CriticalSprTestInter
#  Description:    
#       It carries out the test on special-purpose registers (interword).
#  Author:
#  Used module:    None
#  Input Param:
#		GPR3:Test outcome vector
#		GPR4:Mask (OR) to be applied to the check vector in case of exit without errors
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
 .globl CriticalSprTestInter
  
CriticalSprTestInter:
         PrologStackOp
         StackCheckBegin
         PushParam (3)
         PushParam (4)
         StackCheckEnd
         StoreGPRs2Stack
         StoreCtrXerCr2Stack
         
         mfspr 0, 25
         PushReg (0)
         mfspr 0, 26
         PushReg (0)
         mfspr 0, 27
         PushReg (0)
         
         stw 31, OFFSET_PUNT_STACK_CORR(1)
         TestCPUStart
         
         lis 20, DB0_HI
         ori 20, 20, DB0_LO
         not 21, 20
         
         lis 18, MASK_SDR1_RES_BITS_HI
         ori 18, 18, MASK_SDR1_RES_BITS_LO   
#start test
         march0
         march1
         march2
         march3
         march4
         march5
         
         TestConclusion
         b EndCritiCalSprTestInter
         
FailCritiCalSprTestInter: 

         ErrorHandling
         
EndCritiCalSprTestInter:
         
         StackEpilogBegin
         
         PopReg (0)
         sync
         mtspr 27, 0
         sync
         isync
         
         PopReg (0)
         sync
         mtspr 26, 0
         sync
         isync   
                      
         PopReg (0)
         sync
         mtspr 25, 0
         sync
         isync 
         
         RestoreCtrXerCr
         StackEpilogEnd
         
         blr                
          
