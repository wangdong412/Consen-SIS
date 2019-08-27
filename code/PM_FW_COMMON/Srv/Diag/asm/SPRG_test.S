#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march0
#  Description:    
#       set spr with 0x0000000
#  Author:
#  Used module:    None
#  Input Param:
#       GPR20:DB0
#		SPRGR0-7:General SPR (0-7)
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march0
       mtspr SPRG0, 20
	   mtspr SPRG1, 20
       mtspr SPRG2, 20
	   mtspr SPRG3, 20
	   mtspr SPRG4, 20
	   mtspr SPRG5, 20
	   mtspr SPRG6, 20
	   mtspr SPRG7, 20
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march1
#  Description:	    
#       Read value from spr7-0 and compare with 0x0000000,then write 0xFFFFFFFF into spr7-0
#  Author:
#  Used module:    None
#  Input Param:    No
#       GPR19:Read value
#		GPR20:DB0
#		GPR21:DB1
#		SPRG0-7:SPRG0-7
#  Output Param:   No
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march1
       mfspr 19, SPRG7
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG7
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG7, 20
	   mfspr 19, SPRG7
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG7, 21

	   mfspr 19, SPRG6
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG6
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG6, 20
	   mfspr 19, SPRG6
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG6, 21
	   
	   mfspr 19, SPRG5
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG5
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG5, 20
	   mfspr 19, SPRG5
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG5, 21
	   
	   mfspr 19, SPRG4
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG4
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG4, 20
	   mfspr 19, SPRG4
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG4, 21
	   
	   mfspr 19, SPRG3
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG3
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG3, 20
	   mfspr 19, SPRG3
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG3, 21
	   
	   mfspr 19, SPRG2
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG2
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG2, 20
	   mfspr 19, SPRG2
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG2, 21

	   mfspr 19, SPRG1
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG1
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG1, 20
	   mfspr 19, SPRG1
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG1, 21
	   
	   mfspr 19, SPRG0
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG0
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG0, 20
	   mfspr 19, SPRG0
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG0, 21
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march2
#  Description:    
#       Read out spr0-7 value and compare with 0xFFFFFFFF, then write 0x0000000 into spr0-7
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20:DB0
#		GPR21:DB1
#		SPRG0-7:SPRG0-7
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march2
       mfspr 19, SPRG0
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mfspr 19, SPRG0
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG0, 21
	   mfspr 19, SPRG0
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG0, 20

	   mfspr 19, SPRG1
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mfspr 19, SPRG1
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG1, 21
	   mfspr 19, SPRG1
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG1, 20
	   
	   mfspr 19, SPRG2
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mfspr 19, SPRG2
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG2, 21
	   mfspr 19, SPRG2
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG2, 20
	   
	   mfspr 19, SPRG3
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mfspr 19, SPRG3
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG3, 21
	   mfspr 19, SPRG3
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG3, 20
	   
	   mfspr 19, SPRG4
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mfspr 19, SPRG4
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG4, 21
	   mfspr 19, SPRG4
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG4, 20
	   
	   mfspr 19, SPRG5
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mfspr 19, SPRG5
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG5, 21
	   mfspr 19, SPRG5
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG5, 20
	   	   
	   mfspr 19, SPRG6
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mfspr 19, SPRG6
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG6, 21
	   mfspr 19, SPRG6
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG6, 20
	   	   
	   mfspr 19, SPRG7
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mfspr 19, SPRG7
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG7, 21
	   mfspr 19, SPRG7
	   cmplw 19, 21
	   bnel FailSPRGTest
	   mtspr SPRG7, 20
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march3
#  Description:    
#       Read out spr0-7 value and compare with 0x00000000, then write 0xFFFFFFFF into spr0-7
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20:DB0
#		GPR21:DB1
#		SPRG0-7:SPRG0-7
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march3
       mfspr 19, SPRG0
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG0
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG0, 20
	   mfspr 19, SPRG0
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG0, 21

       mfspr 19, SPRG1
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG1
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG1, 20
	   mfspr 19, SPRG1
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG1, 21

       mfspr 19, SPRG2
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG2
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG2, 20
	   mfspr 19, SPRG2
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG2, 21

       mfspr 19, SPRG3
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG3
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG3, 20
	   mfspr 19, SPRG3
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG3, 21

       mfspr 19, SPRG4
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG4
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG4, 20
	   mfspr 19, SPRG4
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG4, 21

       mfspr 19, SPRG5
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG5
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG5, 20
	   mfspr 19, SPRG5
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG5, 21

       mfspr 19, SPRG6
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG6
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG6, 20
	   mfspr 19, SPRG6
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG6, 21

       mfspr 19, SPRG7
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mfspr 19, SPRG7
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG7, 20
	   mfspr 19, SPRG7
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG7, 21
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march4
#  Description:    
#       Read out spr0-7 value and compare with 0xFFFFFFFF, then write 0x0000000 into spr0-7
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20:DB0
#		GPR21:DB1
#		SPRG0-7:SPRG0-7
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march4
      mfspr 19, SPRG0
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG0
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG0, 21
	  mfspr 19, SPRG0
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG0
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG0, 20

      mfspr 19, SPRG1
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG1
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG1, 21
	  mfspr 19, SPRG1
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG1
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG1, 20
	  
      mfspr 19, SPRG2
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG2
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG2, 21
	  mfspr 19, SPRG2
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG2
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG2, 20
	  
      mfspr 19, SPRG3
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG3
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG3, 21
	  mfspr 19, SPRG3
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG3
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG3, 20
	  
      mfspr 19, SPRG4
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG4
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG4, 21
	  mfspr 19, SPRG4
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG4
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG4, 20
	  
      mfspr 19, SPRG5
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG5
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG5, 21
	  mfspr 19, SPRG5
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG5
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG5, 20
	  
      mfspr 19, SPRG6
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG6
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG6, 21
	  mfspr 19, SPRG6
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG6
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG6, 20
	  
      mfspr 19, SPRG7
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG7
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG7, 21
	  mfspr 19, SPRG7
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mfspr 19, SPRG7
	  cmplw 19, 21
	  bnel FailSPRGTest
	  mtspr SPRG7, 20
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march5
#  Description:    
#       Operation sequence to spr0-7 is [r(0x00000000),w(0xAAAAAAAA),w(0x55555555),r(0x55555555)]
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20:DB0
#		GPR22:DB2
#		GPR23:DB3
#		SPRG0-7:SPRG0-7
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march5
       mfspr 19, SPRG0
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG0, 22
	   mtspr SPRG0, 23
	   mfspr 19, SPRG0
	   cmplw 19, 23
	   bnel FailSPRGTest

       mfspr 19, SPRG1
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG1, 22
	   mtspr SPRG1, 23
	   mfspr 19, SPRG1
	   cmplw 19, 23
	   bnel FailSPRGTest

       mfspr 19, SPRG2
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG2, 22
	   mtspr SPRG2, 23
	   mfspr 19, SPRG2
	   cmplw 19, 23
	   bnel FailSPRGTest

       mfspr 19, SPRG3
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG3, 22
	   mtspr SPRG3, 23
	   mfspr 19, SPRG3
	   cmplw 19, 23
	   bnel FailSPRGTest

       mfspr 19, SPRG4
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG4, 22
	   mtspr SPRG4, 23
	   mfspr 19, SPRG4
	   cmplw 19, 23
	   bnel FailSPRGTest

       mfspr 19, SPRG5
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG5, 22
	   mtspr SPRG5, 23
	   mfspr 19, SPRG5
	   cmplw 19, 23
	   bnel FailSPRGTest

       mfspr 19, SPRG6
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG6, 22
	   mtspr SPRG6, 23
	   mfspr 19, SPRG6
	   cmplw 19, 23
	   bnel FailSPRGTest

       mfspr 19, SPRG7
	   cmplw 19, 20
	   bnel FailSPRGTest
	   mtspr SPRG7, 22
	   mtspr SPRG7, 23
	   mfspr 19, SPRG7
	   cmplw 19, 23
	   bnel FailSPRGTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march6
#  Description:    
#       Operation sequence to spr0-7 is [r(0x55555555),w(0xAAAAAAAA)]
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR20:DB0
#		GPR22:DB2
#		GPR23:DB3
#		SPRG0-7:SPRG0-7
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march6
       mfspr 19, SPRG7
	   cmplw 19, 23
	   bnel FailSPRGTest
	   mtspr SPRG7, 22
	   mfspr 19, SPRG7
	   cmplw 19, 22
	   bnel FailSPRGTest

       mfspr 19, SPRG6
	   cmplw 19, 23
	   bnel FailSPRGTest
	   mtspr SPRG6, 22
	   mfspr 19, SPRG6
	   cmplw 19, 22
	   bnel FailSPRGTest

       mfspr 19, SPRG5
	   cmplw 19, 23
	   bnel FailSPRGTest
	   mtspr SPRG5, 22
	   mfspr 19, SPRG5
	   cmplw 19, 22
	   bnel FailSPRGTest

       mfspr 19, SPRG4
	   cmplw 19, 23
	   bnel FailSPRGTest
	   mtspr SPRG4, 22
	   mfspr 19, SPRG4
	   cmplw 19, 22
	   bnel FailSPRGTest

       mfspr 19, SPRG3
	   cmplw 19, 23
	   bnel FailSPRGTest
	   mtspr SPRG3, 22
	   mfspr 19, SPRG3
	   cmplw 19, 22
	   bnel FailSPRGTest

       mfspr 19, SPRG2
	   cmplw 19, 23
	   bnel FailSPRGTest
	   mtspr SPRG2, 22
	   mfspr 19, SPRG2
	   cmplw 19, 22
	   bnel FailSPRGTest

       mfspr 19, SPRG1
	   cmplw 19, 23
	   bnel FailSPRGTest
	   mtspr SPRG1, 22
	   mfspr 19, SPRG1
	   cmplw 19, 22
	   bnel FailSPRGTest

       mfspr 19, SPRG0
	   cmplw 19, 23
	   bnel FailSPRGTest
	   mtspr SPRG0, 22
	   mfspr 19, SPRG0
	   cmplw 19, 22
	   bnel FailSPRGTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march7
#  Description:    
#       Operation sequence to spr0-7 is [r(0xAAAAAAAA),w(0xCCCCCCCC),w(0x33333333),r(0x33333333)]
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR22:DB2
#		GPR24:DB4
#		GPR25:DB5
#		SPRG0-7:SPRG0-7
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march7
       mfspr 19, SPRG0
	   cmplw 19, 22
	   bnel FailSPRGTest
	   mtspr SPRG0, 24
	   mtspr SPRG0, 25
	   mfspr 19, SPRG0
	   cmplw 19, 25
	   bnel FailSPRGTest

       mfspr 19, SPRG1
	   cmplw 19, 22
	   bnel FailSPRGTest
	   mtspr SPRG1, 24
	   mtspr SPRG1, 25
	   mfspr 19, SPRG1
	   cmplw 19, 25
	   bnel FailSPRGTest

       mfspr 19, SPRG2
	   cmplw 19, 22
	   bnel FailSPRGTest
	   mtspr SPRG2, 24
	   mtspr SPRG2, 25
	   mfspr 19, SPRG2
	   cmplw 19, 25
	   bnel FailSPRGTest

	   mfspr 19, SPRG3
	   cmplw 19, 22
	   bnel FailSPRGTest
	   mtspr SPRG3, 24
	   mtspr SPRG3, 25
	   mfspr 19, SPRG3
	   cmplw 19, 25
	   bnel FailSPRGTest

       mfspr 19, SPRG4
	   cmplw 19, 22
	   bnel FailSPRGTest
	   mtspr SPRG4, 24
	   mtspr SPRG4, 25
	   mfspr 19, SPRG4
	   cmplw 19, 25
	   bnel FailSPRGTest

       mfspr 19, SPRG5
	   cmplw 19, 22
	   bnel FailSPRGTest
	   mtspr SPRG5, 24
	   mtspr SPRG5, 25
	   mfspr 19, SPRG5
	   cmplw 19, 25
	   bnel FailSPRGTest

       mfspr 19, SPRG6
	   cmplw 19, 22
	   bnel FailSPRGTest
	   mtspr SPRG6, 24
	   mtspr SPRG6, 25
	   mfspr 19, SPRG6
	   cmplw 19, 25
	   bnel FailSPRGTest

       mfspr 19, SPRG7
	   cmplw 19, 22
	   bnel FailSPRGTest
	   mtspr SPRG7, 24
	   mtspr SPRG7, 25
	   mfspr 19, SPRG7
	   cmplw 19, 25
	   bnel FailSPRGTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march8
#  Description:    
#       Operation sequence to spr0-7 is [r(0x33333333),w(0x33333333),r(0xCCCCCCCC)]
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR22:DB2
#		GPR24:DB4
#		GPR25:DB5
#		SPRG0-7:SPRG0-7
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march8
       mfspr 19, SPRG7
	   cmplw 19, 25
	   bnel FailSPRGTest
	   mtspr SPRG7, 24
	   mfspr 19, SPRG7
	   cmplw 19, 24
	   bnel FailSPRGTest

       mfspr 19, SPRG6
	   cmplw 19, 25
	   bnel FailSPRGTest
	   mtspr SPRG6, 24
	   mfspr 19, SPRG6
	   cmplw 19, 24
	   bnel FailSPRGTest

       mfspr 19, SPRG5
	   cmplw 19, 25
	   bnel FailSPRGTest
	   mtspr SPRG5, 24
	   mfspr 19, SPRG5
	   cmplw 19, 24
	   bnel FailSPRGTest

       mfspr 19, SPRG4
	   cmplw 19, 25
	   bnel FailSPRGTest
	   mtspr SPRG4, 24
	   mfspr 19, SPRG4
	   cmplw 19, 24
	   bnel FailSPRGTest

       mfspr 19, SPRG3
	   cmplw 19, 25
	   bnel FailSPRGTest
	   mtspr SPRG3, 24
	   mfspr 19, SPRG3
	   cmplw 19, 24
	   bnel FailSPRGTest

       mfspr 19, SPRG2
	   cmplw 19, 25
	   bnel FailSPRGTest
	   mtspr SPRG2, 24
	   mfspr 19, SPRG2
	   cmplw 19, 24
	   bnel FailSPRGTest

       mfspr 19, SPRG1
	   cmplw 19, 25
	   bnel FailSPRGTest
	   mtspr SPRG1, 24
	   mfspr 19, SPRG1
	   cmplw 19, 24
	   bnel FailSPRGTest

       mfspr 19, SPRG0
	   cmplw 19, 25
	   bnel FailSPRGTest
	   mtspr SPRG0, 24
	   mfspr 19, SPRG0
	   cmplw 19, 24
	   bnel FailSPRGTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march9
#  Description:    
#       Operation sequence to spr0-7 is [r(0xCCCCCCCC),w(0xF0F0F0F0),w(0x0F0F0F0F),r(0x0F0F0F0F)]
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR24:DB4
#		GPR26:DB6
#		GPR27:DB7
#		SPRG0-7:SPRG0-7
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march9
       mfspr 19, SPRG0
	   cmplw 19, 24
	   bnel FailSPRGTest
	   mtspr SPRG0, 26
	   mtspr SPRG0, 27
	   mfspr 19, SPRG0
	   cmplw 19, 27
	   bnel FailSPRGTest

       mfspr 19, SPRG1
	   cmplw 19, 24
	   bnel FailSPRGTest
	   mtspr SPRG1, 26
	   mtspr SPRG1, 27
	   mfspr 19, SPRG1
	   cmplw 19, 27
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG2
	   cmplw 19, 24
	   bnel FailSPRGTest
	   mtspr SPRG2, 26
	   mtspr SPRG2, 27
	   mfspr 19, SPRG2
	   cmplw 19, 27
	   bnel FailSPRGTest

       mfspr 19, SPRG3
	   cmplw 19, 24
	   bnel FailSPRGTest
	   mtspr SPRG3, 26
	   mtspr SPRG3, 27
	   mfspr 19, SPRG3
	   cmplw 19, 27
	   bnel FailSPRGTest

       mfspr 19, SPRG4
	   cmplw 19, 24
	   bnel FailSPRGTest
	   mtspr SPRG4, 26
	   mtspr SPRG4, 27
	   mfspr 19, SPRG4
	   cmplw 19, 27
	   bnel FailSPRGTest

       mfspr 19, SPRG5
	   cmplw 19, 24
	   bnel FailSPRGTest
	   mtspr SPRG5, 26
	   mtspr SPRG5, 27
	   mfspr 19, SPRG5
	   cmplw 19, 27
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG6
	   cmplw 19, 24
	   bnel FailSPRGTest
	   mtspr SPRG6, 26
	   mtspr SPRG6, 27
	   mfspr 19, SPRG6
	   cmplw 19, 27
	   bnel FailSPRGTest

       mfspr 19, SPRG7
	   cmplw 19, 24
	   bnel FailSPRGTest
	   mtspr SPRG7, 26
	   mtspr SPRG7, 27
	   mfspr 19, SPRG7
	   cmplw 19, 27
	   bnel FailSPRGTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march10
#  Description:    
#       Operation sequence to spr0-7 is [r(0x0F0F0F0F),w(0xF0F0F0F0),r(0xF0F0F0F0)]
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR26:DB6
#		GPR27:DB7
#		SPRG0-7:SPRG0-7
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march10
       mfspr 19, SPRG7
	   cmplw 19, 27
	   bnel FailSPRGTest
	   mtspr SPRG7, 26
	   mfspr 19, SPRG7
	   cmplw 19, 26
	   bnel FailSPRGTest

	   mfspr 19, SPRG6
	   cmplw 19, 27
	   bnel FailSPRGTest
	   mtspr SPRG6, 26
	   mfspr 19, SPRG6
	   cmplw 19, 26
	   bnel FailSPRGTest
	   
	   mfspr 19, SPRG5
	   cmplw 19, 27
	   bnel FailSPRGTest
	   mtspr SPRG5, 26
	   mfspr 19, SPRG5
	   cmplw 19, 26
	   bnel FailSPRGTest
	   
	   mfspr 19, SPRG4
	   cmplw 19, 27
	   bnel FailSPRGTest
	   mtspr SPRG4, 26
	   mfspr 19, SPRG4
	   cmplw 19, 26
	   bnel FailSPRGTest
	   
	   mfspr 19, SPRG3
	   cmplw 19, 27
	   bnel FailSPRGTest
	   mtspr SPRG3, 26
	   mfspr 19, SPRG3
	   cmplw 19, 26
	   bnel FailSPRGTest
	   
	   mfspr 19, SPRG2
	   cmplw 19, 27
	   bnel FailSPRGTest
	   mtspr SPRG2, 26
	   mfspr 19, SPRG2
	   cmplw 19, 26
	   bnel FailSPRGTest
	   
	   mfspr 19, SPRG1
	   cmplw 19, 27
	   bnel FailSPRGTest
	   mtspr SPRG1, 26
	   mfspr 19, SPRG1
	   cmplw 19, 26
	   bnel FailSPRGTest
	   
	   mfspr 19, SPRG0
	   cmplw 19, 27
	   bnel FailSPRGTest
	   mtspr SPRG0, 26
	   mfspr 19, SPRG0
	   cmplw 19, 26
	   bnel FailSPRGTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march11
#  Description:    
#       Operation sequence to spr0-7 is [r(0xF0F0F0F0),w(0xFF00FF00),w(0x00FF00FF),r(0x00FF00FF)]
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR26:DB6
#		GPR28:DB8
#		GPR29:DB9
#		SPRG0-7:SPRG0-7
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march11
       mfspr 19, SPRG0
	   cmplw 19, 26
	   bnel FailSPRGTest
	   mtspr SPRG0, 28
	   mtspr SPRG0, 29
	   mfspr 19, SPRG0
	   cmplw 19, 29
	   bnel FailSPRGTest

       mfspr 19, SPRG1
	   cmplw 19, 26
	   bnel FailSPRGTest
	   mtspr SPRG1, 28
	   mtspr SPRG1, 29
	   mfspr 19, SPRG1
	   cmplw 19, 29
	   bnel FailSPRGTest

	   mfspr 19, SPRG2
	   cmplw 19, 26
	   bnel FailSPRGTest
	   mtspr SPRG2, 28
	   mtspr SPRG2, 29
	   mfspr 19, SPRG2
	   cmplw 19, 29
	   bnel FailSPRGTest

	   mfspr 19, SPRG3
	   cmplw 19, 26
	   bnel FailSPRGTest
	   mtspr SPRG3, 28
	   mtspr SPRG3, 29
	   mfspr 19, SPRG3
	   cmplw 19, 29
	   bnel FailSPRGTest

	   mfspr 19, SPRG4
	   cmplw 19, 26
	   bnel FailSPRGTest
	   mtspr SPRG4, 28
	   mtspr SPRG4, 29
	   mfspr 19, SPRG4
	   cmplw 19, 29
	   bnel FailSPRGTest	   

	   mfspr 19, SPRG5
	   cmplw 19, 26
	   bnel FailSPRGTest
	   mtspr SPRG5, 28
	   mtspr SPRG5, 29
	   mfspr 19, SPRG5
	   cmplw 19, 29
	   bnel FailSPRGTest

	   mfspr 19, SPRG6
	   cmplw 19, 26
	   bnel FailSPRGTest
	   mtspr SPRG6, 28
	   mtspr SPRG6, 29
	   mfspr 19, SPRG6
	   cmplw 19, 29
	   bnel FailSPRGTest

	   mfspr 19, SPRG7
	   cmplw 19, 26
	   bnel FailSPRGTest
	   mtspr SPRG7, 28
	   mtspr SPRG7, 29
	   mfspr 19, SPRG7
	   cmplw 19, 29
	   bnel FailSPRGTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march12
#  Description:    
#       Operation sequence to spr0-7 is [r(0x00FF00FF),w(0xFF00FF00),r(0xF0F0F0F0)]
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR28:DB8
#		GPR29:DB9
#		SPRG0-7:SPRG0-7
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march12
       mfspr 19, SPRG7
	   cmplw 19, 29
	   bnel FailSPRGTest
	   mtspr SPRG7, 28
	   mfspr 19, SPRG7
	   cmplw 19, 28
	   bnel FailSPRGTest

       mfspr 19, SPRG6
	   cmplw 19, 29
	   bnel FailSPRGTest
	   mtspr SPRG6, 28
	   mfspr 19, SPRG6
	   cmplw 19, 28
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG5
	   cmplw 19, 29
	   bnel FailSPRGTest
	   mtspr SPRG5, 28
	   mfspr 19, SPRG5
	   cmplw 19, 28
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG4
	   cmplw 19, 29
	   bnel FailSPRGTest
	   mtspr SPRG4, 28
	   mfspr 19, SPRG4
	   cmplw 19, 28
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG3
	   cmplw 19, 29
	   bnel FailSPRGTest
	   mtspr SPRG3, 28
	   mfspr 19, SPRG3
	   cmplw 19, 28
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG2
	   cmplw 19, 29
	   bnel FailSPRGTest
	   mtspr SPRG2, 28
	   mfspr 19, SPRG2
	   cmplw 19, 28
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG1
	   cmplw 19, 29
	   bnel FailSPRGTest
	   mtspr SPRG1, 28
	   mfspr 19, SPRG1
	   cmplw 19, 28
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG0
	   cmplw 19, 29
	   bnel FailSPRGTest
	   mtspr SPRG0, 28
	   mfspr 19, SPRG0
	   cmplw 19, 28
	   bnel FailSPRGTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march13
#  Description:    
#       Operation sequence to spr0-7 is [r(0xF0F0F0F0),w(0xFFFF0000),w(0x0000FFFF),r(0x0000FFFF)]
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR28:DB8
#		GPR30:DB10
#		GPR31:DB11
#		SPRG0-7:SPRG0-7
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march13
       mfspr 19, SPRG0
	   cmplw 19, 28
	   bnel FailSPRGTest
	   mtspr SPRG0, 30
	   mtspr SPRG0, 31
	   mfspr 19, SPRG0
	   cmplw 19, 31
	   bnel FailSPRGTest

       mfspr 19, SPRG1
	   cmplw 19, 28
	   bnel FailSPRGTest
	   mtspr SPRG1, 30
	   mtspr SPRG1, 31
	   mfspr 19, SPRG1
	   cmplw 19, 31
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG2
	   cmplw 19, 28
	   bnel FailSPRGTest
	   mtspr SPRG2, 30
	   mtspr SPRG2, 31
	   mfspr 19, SPRG2
	   cmplw 19, 31
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG3
	   cmplw 19, 28
	   bnel FailSPRGTest
	   mtspr SPRG3, 30
	   mtspr SPRG3, 31
	   mfspr 19, SPRG3
	   cmplw 19, 31
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG4
	   cmplw 19, 28
	   bnel FailSPRGTest
	   mtspr SPRG4, 30
	   mtspr SPRG4, 31
	   mfspr 19, SPRG4
	   cmplw 19, 31
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG5
	   cmplw 19, 28
	   bnel FailSPRGTest
	   mtspr SPRG5, 30
	   mtspr SPRG5, 31
	   mfspr 19, SPRG5
	   cmplw 19, 31
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG6
	   cmplw 19, 28
	   bnel FailSPRGTest
	   mtspr SPRG6, 30
	   mtspr SPRG6, 31
	   mfspr 19, SPRG6
	   cmplw 19, 31
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG7
	   cmplw 19, 28
	   bnel FailSPRGTest
	   mtspr SPRG7, 30
	   mtspr SPRG7, 31
	   mfspr 19, SPRG7
	   cmplw 19, 31
	   bnel FailSPRGTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march14
#  Description:    
#       Operation sequence to spr0-7 is [r(0x0000FFFF),w(0xFFFF0000),r(0xFFFF0000)]
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR30:DB10
#		GPR31:DB11
#		SPRG0-7:SPRG0-7
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march14
       mfspr 19, SPRG7
	   cmplw 19, 31
	   bnel FailSPRGTest
	   mtspr SPRG7, 30
	   mfspr 19, SPRG7
	   cmplw 19, 30
	   bnel FailSPRGTest

       mfspr 19, SPRG6
	   cmplw 19, 31
	   bnel FailSPRGTest
	   mtspr SPRG6, 30
	   mfspr 19, SPRG6
	   cmplw 19, 30
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG5
	   cmplw 19, 31
	   bnel FailSPRGTest
	   mtspr SPRG5, 30
	   mfspr 19, SPRG5
	   cmplw 19, 30
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG4
	   cmplw 19, 31
	   bnel FailSPRGTest
	   mtspr SPRG4, 30
	   mfspr 19, SPRG4
	   cmplw 19, 30
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG3
	   cmplw 19, 31
	   bnel FailSPRGTest
	   mtspr SPRG3, 30
	   mfspr 19, SPRG3
	   cmplw 19, 30
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG2
	   cmplw 19, 31
	   bnel FailSPRGTest
	   mtspr SPRG2, 30
	   mfspr 19, SPRG2
	   cmplw 19, 30
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG1
	   cmplw 19, 31
	   bnel FailSPRGTest
	   mtspr SPRG1, 30
	   mfspr 19, SPRG1
	   cmplw 19, 30
	   bnel FailSPRGTest
	   
       mfspr 19, SPRG0
	   cmplw 19, 31
	   bnel FailSPRGTest
	   mtspr SPRG0, 30
	   mfspr 19, SPRG0
	   cmplw 19, 30
	   bnel FailSPRGTest
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march15
#  Description:    
#       Operation sequence to spr0-7 is [r(0xFFFF0000)]
#  Author:
#  Used module:    None
#  Input Param:
#       GPR19:Read value
#		GPR30:DB10
#		SPRG0-7:SPRG0-7
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march15
       mfspr 19, SPRG0
	   cmplw 19, 30
	   bnel FailSPRGTest

	   mfspr 19, SPRG1
	   cmplw 19, 30
	   bnel FailSPRGTest
	   
	   mfspr 19, SPRG2
	   cmplw 19, 30
	   bnel FailSPRGTest
	   
	   mfspr 19, SPRG3
	   cmplw 19, 30
	   bnel FailSPRGTest
	   
	   mfspr 19, SPRG4
	   cmplw 19, 30
	   bnel FailSPRGTest
	   
	   mfspr 19, SPRG5
	   cmplw 19, 30
	   bnel FailSPRGTest
	   
	   mfspr 19, SPRG6
	   cmplw 19, 30
	   bnel FailSPRGTest
	   
	   mfspr 19, SPRG7
	   cmplw 19, 30
	   bnel FailSPRGTest
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      SPRGTest
#  Description:    
#       Routine for the implementation of SPRG register testing.
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
 .globl  SPRGTest

SPRGTest:
             
    PrologStackOp
    StackCheckBegin

    PushParam(3)
    PushParam(4)

    StackCheckEnd
    StoreGPRs2Stack
    StoreCtrXerCr2Stack

	mfspr 0, SPRG0
	PushReg(0)	
	mfspr 0, SPRG1
	PushReg(0)
	mfspr 0, SPRG2
	PushReg(0)
	mfspr 0, SPRG3
	PushReg(0)
	mfspr 0, SPRG4
	PushReg(0)
	mfspr 0, SPRG5
	PushReg(0)
	mfspr 0, SPRG6
	PushReg(0)
	mfspr 0, SPRG7
	PushReg(0)

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
	march13
	march14
	march15

	TestConclusion
	b EndSPRGTest
FailSPRGTest:
    mtspr SPRG4, 31
	lwz 31, OFFSET_PUNT_STACK_CORR(1)
	mtspr SPRG5, 0
	PopReg(0)
	mtspr SPRG7, 0
	stw 31, OFFSET_PUNT_STACK_CORR(1)
	mfspr 0, SPRG5
	mfspr 31, SPRG4
	ErrorHandling
    b EndSPRGTestNoSPRG7
EndSPRGTest:
    lwz 31, OFFSET_PUNT_STACK_CORR(1)
	PopReg(0)
	mtspr SPRG7, 0
	stw 31, OFFSET_PUNT_STACK_CORR(1)
EndSPRGTestNoSPRG7:
    StackEpilogBegin
	PopReg(0)
	mtspr SPRG6, 0
	PopReg(0)
	mtspr SPRG5, 0
       PopReg(0)
	mtspr SPRG4, 0
	PopReg(0)
	mtspr SPRG3, 0
	PopReg(0)
	mtspr SPRG2, 0
       PopReg(0)
	mtspr SPRG1, 0
       PopReg(0)
	mtspr SPRG0, 0

    RestoreCtrXerCr
    StackEpilogEnd

	blr
 .end

            
