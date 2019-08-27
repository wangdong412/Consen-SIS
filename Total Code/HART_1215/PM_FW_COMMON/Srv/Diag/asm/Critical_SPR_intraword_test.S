#include "../inc/common_macro.s"
#include "../inc/common_def.h"
 
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march5
#  Description:    
#       test spr25-27 with given write-read-compare operation
#  Author:
#  Used module:    None
#  Input Param:
#		GPR19:Read value
#		GPR20-31:DB0-DB11
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march5
        mtspr 25, 22
        mtspr 25, 23
        mfspr 19, 25
        and 19, 19, 18
        and 17, 23, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra

        mtspr 26, 22
        mtspr 26, 23
        mfspr 19, 26
        cmplw 19, 23
        bnel FailCritiCalSprTestIntra
        
        mtspr 27, 22
        mtspr 27, 23
        mfspr 19, 27
        cmplw 19, 23
        bnel FailCritiCalSprTestIntra
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march6
#  Description:    
#       test spr25-27 with given write-read-compare operation
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20-31:DB0-DB11
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march6
        mfspr 19, 27
        cmplw 19, 23
        bnel FailCritiCalSprTestIntra
        mtspr 27, 22
        mfspr 19, 27
        cmplw 19, 22
        bnel FailCritiCalSprTestIntra
        
        mfspr 19, 26
        cmplw 19, 23
        bnel FailCritiCalSprTestIntra
        mtspr 26, 22
        mfspr 19, 26
        cmplw 19, 22
        bnel FailCritiCalSprTestIntra    
        
        mfspr 19, 25
        and 19, 19, 18
        and 17, 23, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra
        mtspr 25, 22
        mfspr 19, 25
        and 19, 19, 18
        and 17, 22, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra  
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march7
#  Description:    
#       test spr25-27 with given write-read-compare operation
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20-31:DB0-DB11
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march7
        mfspr 19, 25
        and 19, 19, 18
        and 17, 22, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra 
        mtspr 25, 24
        mtspr 25, 25
        mfspr 19, 25
        and 19, 19, 18
        and 17, 25, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra
        
        mfspr 19, 26
        cmplw 19, 22
        bnel FailCritiCalSprTestIntra 
        mtspr 26, 24
        mtspr 26, 25
        mfspr 19, 26
        cmplw 19, 25
        bnel FailCritiCalSprTestIntra 
        
        mfspr 19, 27
        cmplw 19, 22
        bnel FailCritiCalSprTestIntra 
        mtspr 27, 24
        mtspr 27, 25
        mfspr 19, 27
        cmplw 19, 25
        bnel FailCritiCalSprTestIntra  
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march8
#  Description:    
#       test spr25-27 with given write-read-compare operation
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20-31:DB0-DB11
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march8
        mfspr 19, 27
        cmplw 19, 25
        bnel FailCritiCalSprTestIntra
        mtspr 27, 24
        mfspr 19, 27
        cmplw 19, 24
        bnel FailCritiCalSprTestIntra
        
        mfspr 19, 26
        cmplw 19, 25
        bnel FailCritiCalSprTestIntra
        mtspr 26, 24
        mfspr 19, 26
        cmplw 19, 24
        bnel FailCritiCalSprTestIntra
        
        mfspr 19, 25
        and 19, 19, 18
        and 17, 25, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra
        mtspr 25, 24
        mfspr 19, 25
        and 19, 19, 18
        and 17, 24, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra
        
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march9
#  Description:    
#       test spr25-27 with given write-read-compare operation
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20-31:DB0-DB11
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march9
        mfspr 19, 25
        and 19, 19, 18
        and 17, 24, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra
        mtspr 25, 26
        mtspr 25, 27
        mfspr 19, 25
        and 19, 19, 18
        and 17, 27, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra
        
        mfspr 19, 26
        cmplw 19, 24
        bnel FailCritiCalSprTestIntra
        mtspr 26, 26
        mtspr 26, 27
        mfspr 19, 26
        cmplw 19, 27
        bnel FailCritiCalSprTestIntra    
        
        mfspr 19, 27
        cmplw 19, 24
        bnel FailCritiCalSprTestIntra
        mtspr 27, 26
        mtspr 27, 27
        mfspr 19, 27
        cmplw 19, 27
        bnel FailCritiCalSprTestIntra  
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march10
#  Description:    
#       test spr25-27 with given write-read-compare operation
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20-31:DB0-DB11
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/  
 .macro march10
        mfspr 19, 27
        cmplw 19, 27
        bnel FailCritiCalSprTestIntra 
        mtspr 27, 26
        mfspr 19, 27
        cmplw 19, 26
        bnel FailCritiCalSprTestIntra
        
        mfspr 19, 26
        cmplw 19, 27
        bnel FailCritiCalSprTestIntra 
        mtspr 26, 26
        mfspr 19, 26
        cmplw 19, 26
        bnel FailCritiCalSprTestIntra
                
        mfspr 19, 25
        and 19, 19, 18
        and 17, 27, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra 
        mtspr 25, 26
        mfspr 19, 25
        and 19, 19, 18
        and 17, 26, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march11
#  Description:    
#       test spr25-27 with given write-read-compare operation
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20-31:DB0-DB11
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march11
        mfspr 19, 25
        and 19, 19, 18
        and 17, 26, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra
        mtspr 25, 28
        mtspr 25, 29
        mfspr 19, 25
        and 19, 19, 18
        and 17, 29, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra
        
        mfspr 19, 26
        cmplw 19, 26
        bnel FailCritiCalSprTestIntra
        mtspr 26, 28
        mtspr 26, 29
        mfspr 19, 26
        cmplw 19, 29
        bnel FailCritiCalSprTestIntra 
                
        mfspr 19, 27
        cmplw 19, 26
        bnel FailCritiCalSprTestIntra
        mtspr 27, 28
        mtspr 27, 29
        mfspr 19, 27
        cmplw 19, 29
        bnel FailCritiCalSprTestIntra  
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march12
#  Description:    
#       test spr25-27 with given write-read-compare operation
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20-31:DB0-DB11
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march12
        mfspr 19, 27
        cmplw 19, 29
        bnel FailCritiCalSprTestIntra
        mtspr 27, 28
        mfspr 19, 27
        cmplw 19, 28
        bnel FailCritiCalSprTestIntra
        
        mfspr 19, 26
        cmplw 19, 29
        bnel FailCritiCalSprTestIntra
        mtspr 26, 28
        mfspr 19, 26
        cmplw 19, 28
        bnel FailCritiCalSprTestIntra
         
        mfspr 19, 25
        and 19, 19, 18
        and 17, 29, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra
        mtspr 25, 28
        mfspr 19, 25
        and 19, 19, 18
        and 17, 28, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra 
 .endm 
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march13
#  Description:    
#       test spr25-27 with given write-read-compare operation
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20-31:DB0-DB11
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march13
        mfspr 19, 25
        and 19, 19, 18
        and 17, 28, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra
        mtspr 25, 30
        mtspr 25, 31
        mfspr 19, 25
        and 19, 19, 18
        and 17, 31, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra  
        
        mfspr 19, 26
        cmplw 19, 28
        bnel FailCritiCalSprTestIntra
        mtspr 26, 30
        mtspr 26, 31
        mfspr 19, 26
        cmplw 19, 31
        bnel FailCritiCalSprTestIntra  
        
        mfspr 19, 27
        cmplw 19, 28
        bnel FailCritiCalSprTestIntra
        mtspr 27, 30
        mtspr 27, 31
        mfspr 19, 27
        cmplw 19, 31
        bnel FailCritiCalSprTestIntra 
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march14
#  Description:    
#       test spr25-27 with given write-read-compare operation
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20-31:DB0-DB11
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march14
        mfspr 19, 27
        cmplw 19, 31
        bnel FailCritiCalSprTestIntra
        mtspr 27, 30
        mfspr 19, 27
        cmplw 19, 30
        bnel FailCritiCalSprTestIntra
        
        mfspr 19, 26
        cmplw 19, 31
        bnel FailCritiCalSprTestIntra
        mtspr 26, 30
        mfspr 19, 26
        cmplw 19, 30
        bnel FailCritiCalSprTestIntra
        
        mfspr 19, 25
        and 19, 19, 18
        and 17, 31, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra
        mtspr 25, 30
        mfspr 19, 25
        and 19, 19, 18
        and 17, 30, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march15
#  Description:    
#       read value from spr25-27 and compare with preset value to check if is equal 
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20-31:DB0-DB11
#		SPR25:SDR1
#		SPR26-27:Save and Restore Registers (SRR0-1)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .macro march15
        mfspr 19, 25
        and 19, 19, 18
        and 17, 30, 18
        cmplw 19, 17
        bnel FailCritiCalSprTestIntra
        
        mfspr 19, 26
        cmplw 19, 30
        bnel FailCritiCalSprTestIntra
        
        mfspr 19, 27
        cmplw 19, 30
        bnel FailCritiCalSprTestIntra
 .endm
 .text
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      CriticalSprTestIntra
#  Description:    
#       It carries out the test for special-purpose registers (intraword).
#  Author:
#  Used module:    None
#  Input Param:
#		GPR3:Test outcome vector
#		GPR4:Mask (OR) to be applied to the check vector in case of exit without errors
#		GPR0:Address of calling function
#		GPR2:Address of error memory area
#       GPR19:Compare register
#		GPR20-31:DB0-DB11
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl CriticalSprTestIntra
CriticalSprTestIntra:
         
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
          TestCPUStart                       #clear testbit
          
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
          
          lis 18, MASK_SDR1_RES_BITS_HI
          ori 18, 18, MASK_SDR1_RES_BITS_LO
          
          march5
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
          
          TestConclusion                    #record testbit value
          b EndCritiCalSprTestIntra
FailCritiCalSprTestIntra:

          ErrorHandling
EndCritiCalSprTestIntra:
          
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
          blr                           #return
