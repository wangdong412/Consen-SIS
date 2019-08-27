#include "../inc/common_macro.s"
#include "../inc/common_def.h"

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
#		GPR0-31:Registers where the test shall be carried out
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .macro march5
      lis 4, 0x5555
      ori 4, 4, 0x5555
      
      lis 0, 0xAAAA
      ori 0, 0, 0xAAAA
      not 0, 0
      cmplw 0, 4
      bnel fail_gpr_test_intraword

      lis 1, 0xAAAA
      ori 1, 1, 0xAAAA
      not 1, 1
      cmplw 1, 4
      bnel fail_gpr_test_intraword
            
      lis 2, 0xAAAA
      ori 2, 2, 0xAAAA
      not 2, 2
      cmplw 2, 4
      bnel fail_gpr_test_intraword
            
      lis 3, 0xAAAA
      ori 3, 3, 0xAAAA
      not 3, 3
      cmplw 3, 4
      bnel fail_gpr_test_intraword
            
      lis 5, 0xAAAA
      ori 5, 5, 0xAAAA
      not 5, 5
      cmplw 5, 4
      bnel fail_gpr_test_intraword
            
      lis 6, 0xAAAA
      ori 6, 6, 0xAAAA
      not 6, 6
      cmplw 6, 4
      bnel fail_gpr_test_intraword
            
      lis 7, 0xAAAA
      ori 7, 7, 0xAAAA
      not 7, 7
      cmplw 7, 4
      bnel fail_gpr_test_intraword
            
      lis 8, 0xAAAA
      ori 8, 8, 0xAAAA
      not 8, 8
      cmplw 8, 4
      bnel fail_gpr_test_intraword
            
      lis 9, 0xAAAA
      ori 9, 9, 0xAAAA
      not 9, 9
      cmplw 9, 4
      bnel fail_gpr_test_intraword
            
      lis 10, 0xAAAA
      ori 10, 10, 0xAAAA
      not 10, 10
      cmplw 10, 4
      bnel fail_gpr_test_intraword
            
      lis 11, 0xAAAA
      ori 11, 11, 0xAAAA
      not 11, 11
      cmplw 11, 4
      bnel fail_gpr_test_intraword
            
      lis 12, 0xAAAA
      ori 12, 12, 0xAAAA
      not 12, 12
      cmplw 12, 4
      bnel fail_gpr_test_intraword
            
      lis 13, 0xAAAA
      ori 13, 13, 0xAAAA
      not 13, 13
      cmplw 13, 4
      bnel fail_gpr_test_intraword
            
      lis 14, 0xAAAA
      ori 14, 14, 0xAAAA
      not 14, 14
      cmplw 14, 4
      bnel fail_gpr_test_intraword
            
      lis 15, 0xAAAA
      ori 15, 15, 0xAAAA
      not 15, 15
      cmplw 15, 4
      bnel fail_gpr_test_intraword
            
      lis 16, 0xAAAA
      ori 16, 16, 0xAAAA
      not 16, 16
      cmplw 16, 4
      bnel fail_gpr_test_intraword
            
      lis 17, 0xAAAA
      ori 17, 17, 0xAAAA
      not 17, 17
      cmplw 17, 4
      bnel fail_gpr_test_intraword
            
      lis 18, 0xAAAA
      ori 18, 18, 0xAAAA
      not 18, 18
      cmplw 18, 4
      bnel fail_gpr_test_intraword
            
      lis 19, 0xAAAA
      ori 19, 19, 0xAAAA
      not 19, 19
      cmplw 19, 4
      bnel fail_gpr_test_intraword
            
      lis 20, 0xAAAA
      ori 20, 20, 0xAAAA
      not 20, 20
      cmplw 20, 4
      bnel fail_gpr_test_intraword
            
      lis 21, 0xAAAA
      ori 21, 21, 0xAAAA
      not 21, 21
      cmplw 21, 4
      bnel fail_gpr_test_intraword
            
      lis 22, 0xAAAA
      ori 22, 22, 0xAAAA
      not 22, 22
      cmplw 22, 4
      bnel fail_gpr_test_intraword
            
      lis 23, 0xAAAA
      ori 23, 23, 0xAAAA
      not 23, 23
      cmplw 23, 4
      bnel fail_gpr_test_intraword
            
      lis 24, 0xAAAA
      ori 24, 24, 0xAAAA
      not 24, 24
      cmplw 24, 4
      bnel fail_gpr_test_intraword
            
      lis 25, 0xAAAA
      ori 25, 25, 0xAAAA
      not 25, 25
      cmplw 25, 4
      bnel fail_gpr_test_intraword
            
      lis 26, 0xAAAA
      ori 26, 26, 0xAAAA
      not 26, 26
      cmplw 26, 4
      bnel fail_gpr_test_intraword
            
      lis 27, 0xAAAA
      ori 27, 27, 0xAAAA
      not 27, 27
      cmplw 27, 4
      bnel fail_gpr_test_intraword
            
      lis 28, 0xAAAA
      ori 28, 28, 0xAAAA
      not 28, 28
      cmplw 28, 4
      bnel fail_gpr_test_intraword
            
      lis 29, 0xAAAA
      ori 29, 29, 0xAAAA
      not 29, 29
      cmplw 29, 4
      bnel fail_gpr_test_intraword
            
      lis 30, 0xAAAA
      ori 30, 30, 0xAAAA
      not 30, 30
      cmplw 30, 4
      bnel fail_gpr_test_intraword
            
      lis 31, 0xAAAA
      ori 31, 31, 0xAAAA
      not 31, 31
      cmplw 31, 4
      bnel fail_gpr_test_intraword
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
      cmplw 31, 5
      bnel fail_gpr_test_intraword
      not 31, 31
      not 5, 5
      cmplw 31, 5
      bnel fail_gpr_test_intraword
      not 5, 5
      
      cmplw 30, 5
      bnel fail_gpr_test_intraword
      not 30, 30
      not 5, 5
      cmplw 30, 5
      bnel fail_gpr_test_intraword
      not 5, 5
            
      cmplw 29, 5
      bnel fail_gpr_test_intraword
      not 29, 29
      not 5, 5
      cmplw 29, 5
      bnel fail_gpr_test_intraword
      not 5, 5
            
      cmplw 28, 5
      bnel fail_gpr_test_intraword
      not 28, 28
      not 5, 5
      cmplw 28, 5
      bnel fail_gpr_test_intraword
      not 5, 5
            
      cmplw 27, 5
      bnel fail_gpr_test_intraword
      not 27, 27
      not 5, 5
      cmplw 27, 5
      bnel fail_gpr_test_intraword
      not 5, 5
            
      cmplw 26, 5
      bnel fail_gpr_test_intraword
      not 26, 26
      not 5, 5
      cmplw 26, 5
      bnel fail_gpr_test_intraword
      not 5, 5
            
      cmplw 25, 5
      bnel fail_gpr_test_intraword
      not 25, 25
      not 5, 5
      cmplw 25, 5
      bnel fail_gpr_test_intraword
      not 5, 5
            
      cmplw 24, 5
      bnel fail_gpr_test_intraword
      not 24, 24
      not 5, 5
      cmplw 24, 5
      bnel fail_gpr_test_intraword
      not 5, 5
            
      cmplw 23, 5
      bnel fail_gpr_test_intraword
      not 23, 23
      not 5, 5
      cmplw 23, 5
      bnel fail_gpr_test_intraword
      not 5, 5
            
      cmplw 22, 5
      bnel fail_gpr_test_intraword
      not 22, 22
      not 5, 5
      cmplw 22, 5
      bnel fail_gpr_test_intraword
      not 5, 5
            
      cmplw 21, 5
      bnel fail_gpr_test_intraword
      not 21, 21
      not 5, 5
      cmplw 21, 5
      bnel fail_gpr_test_intraword
      not 5, 5
            
      cmplw 20, 5
      bnel fail_gpr_test_intraword
      not 20, 20
      not 5, 5
      cmplw 20, 5
      bnel fail_gpr_test_intraword
      not 5, 5
            
      cmplw 19, 5
      bnel fail_gpr_test_intraword
      not 19, 19
      not 5, 5
      cmplw 19, 5
      bnel fail_gpr_test_intraword
      not 5, 5
            
      cmplw 18, 5
      bnel fail_gpr_test_intraword
      not 18, 18
      not 5, 5
      cmplw 18, 5
      bnel fail_gpr_test_intraword
      not 5, 5
            
      cmplw 17, 5
      bnel fail_gpr_test_intraword
      not 17, 17
      not 5, 5
      cmplw 17, 5
      bnel fail_gpr_test_intraword
      not 5, 5
      
      cmplw 16, 5
      bnel fail_gpr_test_intraword
      not 16, 16
      not 5, 5
      cmplw 16, 5
      bnel fail_gpr_test_intraword
      not 5, 5
                  
      cmplw 15, 5
      bnel fail_gpr_test_intraword
      not 15, 15
      not 5, 5
      cmplw 15, 5
      bnel fail_gpr_test_intraword
      not 5, 5
                  
      cmplw 14, 5
      bnel fail_gpr_test_intraword
      not 14, 14
      not 5, 5
      cmplw 14, 5
      bnel fail_gpr_test_intraword
      not 5, 5
                  
      cmplw 13, 5
      bnel fail_gpr_test_intraword
      not 13, 13
      not 5, 5
      cmplw 13, 5
      bnel fail_gpr_test_intraword
      not 5, 5
                  
      cmplw 12, 5
      bnel fail_gpr_test_intraword
      not 12, 12
      not 5, 5
      cmplw 12, 5
      bnel fail_gpr_test_intraword
      not 5, 5
                  
      cmplw 11, 5
      bnel fail_gpr_test_intraword
      not 11, 11
      not 5, 5
      cmplw 11, 5
      bnel fail_gpr_test_intraword
      not 5, 5
                  
      cmplw 10, 5
      bnel fail_gpr_test_intraword
      not 10, 10
      not 5, 5
      cmplw 10, 5
      bnel fail_gpr_test_intraword
      not 5, 5
                  
      cmplw 9, 5
      bnel fail_gpr_test_intraword
      not 9, 9
      not 5, 5
      cmplw 9, 5
      bnel fail_gpr_test_intraword
      not 5, 5
                  
      cmplw 8, 5
      bnel fail_gpr_test_intraword
      not 8, 8
      not 5, 5
      cmplw 8, 5
      bnel fail_gpr_test_intraword
      not 5, 5
                  
      cmplw 7, 5
      bnel fail_gpr_test_intraword
      not 7, 7
      not 5, 5
      cmplw 7, 5
      bnel fail_gpr_test_intraword
      not 5, 5
                  
      cmplw 6, 5
      bnel fail_gpr_test_intraword
      not 6, 6
      not 5, 5
      cmplw 6, 5
      bnel fail_gpr_test_intraword
      not 5, 5
                  
      cmplw 4, 5
      bnel fail_gpr_test_intraword
      not 4, 4
      not 5, 5
      cmplw 4, 5
      bnel fail_gpr_test_intraword
      not 5, 5
                  
      cmplw 3, 5
      bnel fail_gpr_test_intraword
      not 3, 3
      not 5, 5
      cmplw 3, 5
      bnel fail_gpr_test_intraword
      not 5, 5
                  
      cmplw 2, 5
      bnel fail_gpr_test_intraword
      not 2, 2
      not 5, 5
      cmplw 2, 5
      bnel fail_gpr_test_intraword
      not 5, 5
                      
      cmplw 1, 5
      bnel fail_gpr_test_intraword
      not 1, 1
      not 5, 5
      cmplw 1, 5
      bnel fail_gpr_test_intraword
      not 5, 5  
                        
      cmplw 0, 5
      bnel fail_gpr_test_intraword
      not 0, 0
      not 5, 5
      cmplw 0, 5
      bnel fail_gpr_test_intraword
      not 5, 5
      
      not 5, 5
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
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 0, 6
     bnel fail_gpr_test_intraword
     lis 0, 0xCCCC
     ori 0, 0, 0xCCCC
     not 0,0
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 0, 6
     bnel fail_gpr_test_intraword
     
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 1, 6
     bnel fail_gpr_test_intraword
     lis 1, 0xCCCC
     ori 1, 1, 0xCCCC
     not 1,1
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 1, 6
     bnel fail_gpr_test_intraword
         
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 2, 6
     bnel fail_gpr_test_intraword
     lis 2, 0xCCCC
     ori 2, 2, 0xCCCC
     not 2,2
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 2, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 3, 6
     bnel fail_gpr_test_intraword
     lis 3, 0xCCCC
     ori 3, 3, 0xCCCC
     not 3,3
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 3, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 4, 6
     bnel fail_gpr_test_intraword
     lis 4, 0xCCCC
     ori 4, 4, 0xCCCC
     not 4,4
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 4, 6
     bnel fail_gpr_test_intraword
     
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 5, 6
     bnel fail_gpr_test_intraword
     lis 5, 0xCCCC
     ori 5, 5, 0xCCCC
     not 5,5
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 5, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 7, 6
     bnel fail_gpr_test_intraword
     lis 7, 0xCCCC
     ori 7, 7, 0xCCCC
     not 7, 7
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 7, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 8, 6
     bnel fail_gpr_test_intraword
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     not 8, 8
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 8, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 9, 6
     bnel fail_gpr_test_intraword
     lis 9, 0xCCCC
     ori 9, 9, 0xCCCC
     not 9, 9
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 9, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 10, 6
     bnel fail_gpr_test_intraword
     lis 10, 0xCCCC
     ori 10, 10, 0xCCCC
     not 10,10
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 10, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 11, 6
     bnel fail_gpr_test_intraword
     lis 11, 0xCCCC
     ori 11, 11, 0xCCCC
     not 11, 11
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 11, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 12, 6
     bnel fail_gpr_test_intraword
     lis 12, 0xCCCC
     ori 12, 12, 0xCCCC
     not 12, 12
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 12, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 13, 6
     bnel fail_gpr_test_intraword
     lis 13, 0xCCCC
     ori 13, 13, 0xCCCC
     not 13, 13
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 13, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 14, 6
     bnel fail_gpr_test_intraword
     lis 14, 0xCCCC
     ori 14, 14, 0xCCCC
     not 14, 14
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 14, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 15, 6
     bnel fail_gpr_test_intraword
     lis 15, 0xCCCC
     ori 15, 15, 0xCCCC
     not 15, 15
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 15, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 16, 6
     bnel fail_gpr_test_intraword
     lis 16, 0xCCCC
     ori 16, 16, 0xCCCC
     not 16, 16
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 16, 6
     bnel fail_gpr_test_intraword
    
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 17, 6
     bnel fail_gpr_test_intraword
     lis 17, 0xCCCC
     ori 17, 17, 0xCCCC
     not 17, 17
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 17, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 18, 6
     bnel fail_gpr_test_intraword
     lis 18, 0xCCCC
     ori 18, 18, 0xCCCC
     not 18, 18
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 18, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 19, 6
     bnel fail_gpr_test_intraword
     lis 19, 0xCCCC
     ori 19, 19, 0xCCCC
     not 19, 19
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 19, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 20, 6
     bnel fail_gpr_test_intraword
     lis 20, 0xCCCC
     ori 20, 20, 0xCCCC
     not 20, 20
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 20, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 21, 6
     bnel fail_gpr_test_intraword
     lis 21, 0xCCCC
     ori 21, 21, 0xCCCC
     not 21, 21
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 21, 6
     bnel fail_gpr_test_intraword
          
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 22, 6
     bnel fail_gpr_test_intraword
     lis 22, 0xCCCC
     ori 22, 22, 0xCCCC
     not 22, 22
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 22, 6
     bnel fail_gpr_test_intraword 
       
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 23, 6
     bnel fail_gpr_test_intraword
     lis 23, 0xCCCC
     ori 23, 23, 0xCCCC
     not 23, 23
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 23, 6
     bnel fail_gpr_test_intraword
       
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 24, 6
     bnel fail_gpr_test_intraword
     lis 24, 0xCCCC
     ori 24, 24, 0xCCCC
     not 24, 24
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 24, 6
     bnel fail_gpr_test_intraword
            
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 25, 6
     bnel fail_gpr_test_intraword
     lis 25, 0xCCCC
     ori 25, 25, 0xCCCC
     not 25, 25
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 25, 6
     bnel fail_gpr_test_intraword
            
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 26, 6
     bnel fail_gpr_test_intraword
     lis 26, 0xCCCC
     ori 26, 26, 0xCCCC
     not 26, 26
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 26, 6
     bnel fail_gpr_test_intraword
            
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 27, 6
     bnel fail_gpr_test_intraword
     lis 27, 0xCCCC
     ori 27, 27, 0xCCCC
     not 27, 27
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 27, 6
     bnel fail_gpr_test_intraword
            
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 28, 6
     bnel fail_gpr_test_intraword
     lis 28, 0xCCCC
     ori 28, 28, 0xCCCC
     not 28, 28
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 28, 6
     bnel fail_gpr_test_intraword
            
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 29, 6
     bnel fail_gpr_test_intraword
     lis 29, 0xCCCC
     ori 29, 29, 0xCCCC
     not 29, 29
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 29, 6
     bnel fail_gpr_test_intraword
            
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 30, 6
     bnel fail_gpr_test_intraword
     lis 30, 0xCCCC
     ori 30, 30, 0xCCCC
     not 30, 30
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 30, 6
     bnel fail_gpr_test_intraword
            
     lis 6, 0xAAAA
     ori 6, 6, 0xAAAA
     cmplw 31, 6
     bnel fail_gpr_test_intraword
     lis 31, 0xCCCC
     ori 31, 31, 0xCCCC
     not 31, 31
     lis 6, 0x3333
     ori 6, 6, 0x3333
     cmplw 31, 6
     bnel fail_gpr_test_intraword
     
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march8
#  Description:    
#       check intra-word faults through sequence March8:↓[r(0x33333333), w(0xCCCCCCCC),r(0xCCCCCCCC)]
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
 .macro march8
     cmplw 31, 7
     bnel fail_gpr_test_intraword
     not 31, 31
     not 7, 7
     cmplw 31, 7
     bnel fail_gpr_test_intraword
     not 7, 7
     
     cmplw 30, 7
     bnel fail_gpr_test_intraword
     not 30, 30
     not 7, 7
     cmplw 30, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 29, 7
     bnel fail_gpr_test_intraword
     not 29, 29
     not 7, 7
     cmplw 29, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 28, 7
     bnel fail_gpr_test_intraword
     not 28, 28
     not 7, 7
     cmplw 28, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 27, 7
     bnel fail_gpr_test_intraword
     not 27, 27
     not 7, 7
     cmplw 27, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 26, 7
     bnel fail_gpr_test_intraword
     not 26, 26
     not 7, 7
     cmplw 26, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 25, 7
     bnel fail_gpr_test_intraword
     not 25, 25
     not 7, 7
     cmplw 25, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 24, 7
     bnel fail_gpr_test_intraword
     not 24, 24
     not 7, 7
     cmplw 24, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 23, 7
     bnel fail_gpr_test_intraword
     not 23, 23
     not 7, 7
     cmplw 23, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 22, 7
     bnel fail_gpr_test_intraword
     not 22, 22
     not 7, 7
     cmplw 22, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 21, 7
     bnel fail_gpr_test_intraword
     not 21, 21
     not 7, 7
     cmplw 21, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 20, 7
     bnel fail_gpr_test_intraword
     not 20, 20
     not 7, 7
     cmplw 20, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 19, 7
     bnel fail_gpr_test_intraword
     not 19, 19
     not 7, 7
     cmplw 19, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 18, 7
     bnel fail_gpr_test_intraword
     not 18, 18
     not 7, 7
     cmplw 18, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 17, 7
     bnel fail_gpr_test_intraword
     not 17, 17
     not 7, 7
     cmplw 17, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 16, 7
     bnel fail_gpr_test_intraword
     not 16, 16
     not 7, 7
     cmplw 16, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 15, 7
     bnel fail_gpr_test_intraword
     not 15, 15
     not 7, 7
     cmplw 15, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 14, 7
     bnel fail_gpr_test_intraword
     not 14, 14
     not 7, 7
     cmplw 14, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 13, 7
     bnel fail_gpr_test_intraword
     not 13, 13
     not 7, 7
     cmplw 13, 7
     bnel fail_gpr_test_intraword
     not 7, 7
     
     cmplw 12, 7
     bnel fail_gpr_test_intraword
     not 12, 12
     not 7, 7
     cmplw 12, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 11, 7
     bnel fail_gpr_test_intraword
     not 11, 11
     not 7, 7
     cmplw 11, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 10, 7
     bnel fail_gpr_test_intraword
     not 10, 10
     not 7, 7
     cmplw 10, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 9, 7
     bnel fail_gpr_test_intraword
     not 9, 9
     not 7, 7
     cmplw 9, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 8, 7
     bnel fail_gpr_test_intraword
     not 8, 8
     not 7, 7
     cmplw 8, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 6, 7
     bnel fail_gpr_test_intraword
     not 6, 6
     not 7, 7
     cmplw 6, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 5, 7
     bnel fail_gpr_test_intraword
     not 5, 5
     not 7, 7
     cmplw 5, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 4, 7
     bnel fail_gpr_test_intraword
     not 4, 4
     not 7, 7
     cmplw 4, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 3, 7
     bnel fail_gpr_test_intraword
     not 3, 3
     not 7, 7
     cmplw 3, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 2, 7
     bnel fail_gpr_test_intraword
     not 2, 2
     not 7, 7
     cmplw 2, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 1, 7
     bnel fail_gpr_test_intraword
     not 1, 1
     not 7, 7
     cmplw 1, 7
     bnel fail_gpr_test_intraword
     not 7, 7
          
     cmplw 0, 7
     bnel fail_gpr_test_intraword
     not 0, 0
     not 7, 7
     cmplw 0, 7
     bnel fail_gpr_test_intraword
     not 7, 7
     
     not 7, 7
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
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 0, 8
     bnel fail_gpr_test_intraword
     lis 0, 0xF0F0
     ori 0, 0, 0xF0F0
     not 0, 0
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 0, 8
     bnel fail_gpr_test_intraword
     
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 1, 8
     bnel fail_gpr_test_intraword
     lis 1, 0xF0F0
     ori 1, 1, 0xF0F0
     not 1, 1
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 1, 8
     bnel fail_gpr_test_intraword
          
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 2, 8
     bnel fail_gpr_test_intraword
     lis 2, 0xF0F0
     ori 2, 2, 0xF0F0
     not 2, 2
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 2, 8
     bnel fail_gpr_test_intraword
          
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 3, 8
     bnel fail_gpr_test_intraword
     lis 3, 0xF0F0
     ori 3, 3, 0xF0F0
     not 3, 3
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 3, 8
     bnel fail_gpr_test_intraword
          
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 4, 8
     bnel fail_gpr_test_intraword
     lis 4, 0xF0F0
     ori 4, 4, 0xF0F0
     not 4, 4
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 4, 8
     bnel fail_gpr_test_intraword
          
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 5, 8
     bnel fail_gpr_test_intraword
     lis 5, 0xF0F0
     ori 5, 5, 0xF0F0
     not 5, 5
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 5, 8
     bnel fail_gpr_test_intraword
          
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 6, 8
     bnel fail_gpr_test_intraword
     lis 6, 0xF0F0
     ori 6, 6, 0xF0F0
     not 6, 6
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 6, 8
     bnel fail_gpr_test_intraword
          
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 7, 8
     bnel fail_gpr_test_intraword
     lis 7, 0xF0F0
     ori 7, 7, 0xF0F0
     not 7, 7
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 7, 8
     bnel fail_gpr_test_intraword
          
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 9, 8
     bnel fail_gpr_test_intraword
     lis 9, 0xF0F0
     ori 9, 9, 0xF0F0
     not 9, 9
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 9, 8
     bnel fail_gpr_test_intraword
          
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 10, 8
     bnel fail_gpr_test_intraword
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     not 10, 10
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 10, 8
     bnel fail_gpr_test_intraword
               
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 11, 8
     bnel fail_gpr_test_intraword
     lis 11, 0xF0F0
     ori 11, 11, 0xF0F0
     not 11, 11
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 11, 8
     bnel fail_gpr_test_intraword
               
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 12, 8
     bnel fail_gpr_test_intraword
     lis 12, 0xF0F0
     ori 12, 12, 0xF0F0
     not 12, 12
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 12, 8
     bnel fail_gpr_test_intraword
               
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 13, 8
     bnel fail_gpr_test_intraword
     lis 13, 0xF0F0
     ori 13, 13, 0xF0F0
     not 13, 13
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 13, 8
     bnel fail_gpr_test_intraword
               
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 14, 8
     bnel fail_gpr_test_intraword
     lis 14, 0xF0F0
     ori 14, 14, 0xF0F0
     not 14, 14
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 14, 8
     bnel fail_gpr_test_intraword
               
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 15, 8
     bnel fail_gpr_test_intraword
     lis 15, 0xF0F0
     ori 15, 15, 0xF0F0
     not 15, 15
     lis 8, 0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 15, 8
     bnel fail_gpr_test_intraword
               
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 16, 8
     bnel fail_gpr_test_intraword
     lis 16, 0xF0F0
     ori 16, 16, 0xF0F0
     not 16, 16
     lis 8, 0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 16, 8
     bnel fail_gpr_test_intraword
               
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 17, 8
     bnel fail_gpr_test_intraword
     lis 17, 0xF0F0
     ori 17, 17, 0xF0F0
     not 17, 17
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 17, 8
     bnel fail_gpr_test_intraword
               
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 18, 8
     bnel fail_gpr_test_intraword
     lis 18, 0xF0F0
     ori 18, 18, 0xF0F0
     not 18, 18
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 18, 8
     bnel fail_gpr_test_intraword
               
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 19, 8
     bnel fail_gpr_test_intraword
     lis 19, 0xF0F0
     ori 19, 19, 0xF0F0
     not 19, 19
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 19, 8
     bnel fail_gpr_test_intraword
               
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 20, 8
     bnel fail_gpr_test_intraword
     lis 20, 0xF0F0
     ori 20, 20, 0xF0F0
     not 20, 20
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 20, 8
     bnel fail_gpr_test_intraword
                    
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 21, 8
     bnel fail_gpr_test_intraword
     lis 21, 0xF0F0
     ori 21, 21, 0xF0F0
     not 21, 21
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 21, 8
     bnel fail_gpr_test_intraword
                              
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 22, 8
     bnel fail_gpr_test_intraword
     lis 22, 0xF0F0
     ori 22, 22, 0xF0F0
     not 22, 22
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 22, 8
     bnel fail_gpr_test_intraword
                              
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 23, 8
     bnel fail_gpr_test_intraword
     lis 23, 0xF0F0
     ori 23, 23, 0xF0F0
     not 23, 23
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 23, 8
     bnel fail_gpr_test_intraword
                              
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 24, 8
     bnel fail_gpr_test_intraword
     lis 24, 0xF0F0
     ori 24, 24, 0xF0F0
     not 24, 24
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 24, 8
     bnel fail_gpr_test_intraword
                              
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 25, 8
     bnel fail_gpr_test_intraword
     lis 25, 0xF0F0
     ori 25, 25, 0xF0F0
     not 25, 25
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 25, 8
     bnel fail_gpr_test_intraword
                              
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 26, 8
     bnel fail_gpr_test_intraword
     lis 26, 0xF0F0
     ori 26, 26, 0xF0F0
     not 26, 26
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 26, 8
     bnel fail_gpr_test_intraword
                                   
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 27, 8
     bnel fail_gpr_test_intraword
     lis 27, 0xF0F0
     ori 27, 27, 0xF0F0
     not 27, 27
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 27, 8
     bnel fail_gpr_test_intraword
                                   
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 28, 8
     bnel fail_gpr_test_intraword
     lis 28, 0xF0F0
     ori 28, 28, 0xF0F0
     not 28, 28
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 28, 8
     bnel fail_gpr_test_intraword
                                   
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 29, 8
     bnel fail_gpr_test_intraword
     lis 29, 0xF0F0
     ori 29, 29, 0xF0F0
     not 29, 29
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 29, 8
     bnel fail_gpr_test_intraword
                                   
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 30, 8
     bnel fail_gpr_test_intraword
     lis 30, 0xF0F0
     ori 30, 30, 0xF0F0
     not 30, 30
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 30, 8
     bnel fail_gpr_test_intraword
                                   
     lis 8, 0xCCCC
     ori 8, 8, 0xCCCC
     cmplw 31, 8
     bnel fail_gpr_test_intraword
     lis 31, 0xF0F0
     ori 31, 31, 0xF0F0
     not 31, 31
     lis 8,0x0F0F
     ori 8, 8, 0x0F0F
     cmplw 31, 8
     bnel fail_gpr_test_intraword
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
     cmplw 31, 9
     bnel fail_gpr_test_intraword 
     not 31, 31
     not 9, 9
     cmplw 31, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                      
     cmplw 30, 9
     bnel fail_gpr_test_intraword 
     not 30, 30
     not 9, 9
     cmplw 30, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                           
     cmplw 29, 9
     bnel fail_gpr_test_intraword 
     not 29, 29
     not 9, 9
     cmplw 29, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                           
     cmplw 28, 9
     bnel fail_gpr_test_intraword 
     not 28, 28
     not 9, 9
     cmplw 28, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                           
     cmplw 27, 9
     bnel fail_gpr_test_intraword 
     not 27, 27
     not 9, 9
     cmplw 27, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                           
     cmplw 26, 9
     bnel fail_gpr_test_intraword 
     not 26, 26
     not 9, 9
     cmplw 26, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                           
     cmplw 25, 9
     bnel fail_gpr_test_intraword 
     not 25, 25
     not 9, 9
     cmplw 25, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                           
     cmplw 24, 9
     bnel fail_gpr_test_intraword 
     not 24, 24
     not 9, 9
     cmplw 24, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                           
     cmplw 23, 9
     bnel fail_gpr_test_intraword 
     not 23, 23
     not 9, 9
     cmplw 23, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                           
     cmplw 22, 9
     bnel fail_gpr_test_intraword 
     not 22, 22
     not 9, 9
     cmplw 22, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                           
     cmplw 21, 9
     bnel fail_gpr_test_intraword 
     not 21, 21
     not 9, 9
     cmplw 21, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                           
     cmplw 20, 9
     bnel fail_gpr_test_intraword 
     not 20, 20
     not 9, 9
     cmplw 20, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                           
     cmplw 19, 9
     bnel fail_gpr_test_intraword 
     not 19, 19
     not 9, 9
     cmplw 19, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 18, 9
     bnel fail_gpr_test_intraword 
     not 18, 18
     not 9, 9
     cmplw 18, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 17, 9
     bnel fail_gpr_test_intraword 
     not 17, 17
     not 9, 9
     cmplw 17, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 16, 9
     bnel fail_gpr_test_intraword 
     not 16, 16
     not 9, 9
     cmplw 16, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 15, 9
     bnel fail_gpr_test_intraword 
     not 15, 15
     not 9, 9
     cmplw 15, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 14, 9
     bnel fail_gpr_test_intraword 
     not 14, 14
     not 9, 9
     cmplw 14, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 13, 9
     bnel fail_gpr_test_intraword 
     not 13, 13
     not 9, 9
     cmplw 13, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 12, 9
     bnel fail_gpr_test_intraword 
     not 12, 12
     not 9, 9
     cmplw 12, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 11, 9
     bnel fail_gpr_test_intraword 
     not 11, 11
     not 9, 9
     cmplw 11, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 10, 9
     bnel fail_gpr_test_intraword 
     not 10, 10
     not 9, 9
     cmplw 10, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 8, 9
     bnel fail_gpr_test_intraword 
     not 8, 8
     not 9, 9
     cmplw 8, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 7, 9
     bnel fail_gpr_test_intraword 
     not 7, 7
     not 9, 9
     cmplw 7, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 6, 9
     bnel fail_gpr_test_intraword 
     not 6, 6
     not 9, 9
     cmplw 6, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 5, 9
     bnel fail_gpr_test_intraword 
     not 5, 5
     not 9, 9
     cmplw 5, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 4, 9
     bnel fail_gpr_test_intraword 
     not 4, 4
     not 9, 9
     cmplw 4, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 3, 9
     bnel fail_gpr_test_intraword 
     not 3, 3
     not 9, 9
     cmplw 3, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 2, 9
     bnel fail_gpr_test_intraword 
     not 2, 2
     not 9, 9
     cmplw 2, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 1, 9
     bnel fail_gpr_test_intraword 
     not 1, 1
     not 9, 9
     cmplw 1, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     cmplw 0, 9
     bnel fail_gpr_test_intraword 
     not 0, 0
     not 9, 9
     cmplw 0, 9
     bnel fail_gpr_test_intraword
     not 9, 9
                                
     not 9 ,9
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march11
#  Description:    
#       check intra-word faults through sequence            
#       March11:↑[r(0xF0F0F0F0), w(0xFF00FF00), w(0x00FF00FF),r(0x00FF00FF)]
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
 .macro march11
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 0, 10
     bnel fail_gpr_test_intraword
     lis 0, 0xFF00
     ori 0, 0, 0xFF00
     not 0, 0 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 0, 10
     bnel fail_gpr_test_intraword
     
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 1, 10
     bnel fail_gpr_test_intraword
     lis 1, 0xFF00
     ori 1, 1, 0xFF00
     not 1, 1 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 1, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 2, 10
     bnel fail_gpr_test_intraword
     lis 2, 0xFF00
     ori 2, 2, 0xFF00
     not 2, 2 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 2, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 3, 10
     bnel fail_gpr_test_intraword
     lis 3, 0xFF00
     ori 3, 3, 0xFF00
     not 3, 3 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 3, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 4, 10
     bnel fail_gpr_test_intraword
     lis 4, 0xFF00
     ori 4, 4, 0xFF00
     not 4, 4 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 4, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 5, 10
     bnel fail_gpr_test_intraword
     lis 5, 0xFF00
     ori 5, 5, 0xFF00
     not 5, 5 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 5, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 6, 10
     bnel fail_gpr_test_intraword
     lis 6, 0xFF00
     ori 6, 6, 0xFF00
     not 6, 6 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 6, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 7, 10
     bnel fail_gpr_test_intraword
     lis 7, 0xFF00
     ori 7, 7, 0xFF00
     not 7, 7 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 7, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 8, 10
     bnel fail_gpr_test_intraword
     lis 8, 0xFF00
     ori 8, 8, 0xFF00
     not 8, 8 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 8, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 9, 10
     bnel fail_gpr_test_intraword
     lis 9, 0xFF00
     ori 9, 9, 0xFF00
     not 9, 9 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 9, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 11, 10
     bnel fail_gpr_test_intraword
     lis 11, 0xFF00
     ori 11, 11, 0xFF00
     not 11, 11
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 11, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 12, 10
     bnel fail_gpr_test_intraword
     lis 12, 0xFF00
     ori 12, 12, 0xFF00
     not 12, 12 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 12, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 13, 10
     bnel fail_gpr_test_intraword
     lis 13, 0xFF00
     ori 13, 13, 0xFF00
     not 13, 13 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 13, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 14, 10
     bnel fail_gpr_test_intraword
     lis 14, 0xFF00
     ori 14, 14, 0xFF00
     not 14, 14 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 14, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 15, 10
     bnel fail_gpr_test_intraword
     lis 15, 0xFF00
     ori 15, 15, 0xFF00
     not 15, 15 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 15, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 16, 10
     bnel fail_gpr_test_intraword
     lis 16, 0xFF00
     ori 16, 16, 0xFF00
     not 16, 16 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 16, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 17, 10
     bnel fail_gpr_test_intraword
     lis 17, 0xFF00
     ori 17, 17, 0xFF00
     not 17, 17
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 17, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 18, 10
     bnel fail_gpr_test_intraword
     lis 18, 0xFF00
     ori 18, 18, 0xFF00
     not 18, 18 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 18, 10
     bnel fail_gpr_test_intraword
          
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 19, 10
     bnel fail_gpr_test_intraword
     lis 19, 0xFF00
     ori 19, 19, 0xFF00
     not 19, 19 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 19, 10
     bnel fail_gpr_test_intraword
               
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 20, 10
     bnel fail_gpr_test_intraword
     lis 20, 0xFF00
     ori 20, 20, 0xFF00
     not 20, 20
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 20, 10
     bnel fail_gpr_test_intraword
               
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 21, 10
     bnel fail_gpr_test_intraword
     lis 21, 0xFF00
     ori 21, 21, 0xFF00
     not 21, 21 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 21, 10
     bnel fail_gpr_test_intraword
               
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 22, 10
     bnel fail_gpr_test_intraword
     lis 22, 0xFF00
     ori 22, 22, 0xFF00
     not 22, 22 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 22, 10
     bnel fail_gpr_test_intraword
               
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 23, 10
     bnel fail_gpr_test_intraword
     lis 23, 0xFF00
     ori 23, 23, 0xFF00
     not 23, 23
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 23, 10
     bnel fail_gpr_test_intraword
               
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 24, 10
     bnel fail_gpr_test_intraword
     lis 24, 0xFF00
     ori 24, 24, 0xFF00
     not 24, 24 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 24, 10
     bnel fail_gpr_test_intraword
               
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 25, 10
     bnel fail_gpr_test_intraword
     lis 25, 0xFF00
     ori 25, 25, 0xFF00
     not 25, 25 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 25, 10
     bnel fail_gpr_test_intraword
               
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 26, 10
     bnel fail_gpr_test_intraword
     lis 26, 0xFF00
     ori 26, 26, 0xFF00
     not 26, 26 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 26, 10
     bnel fail_gpr_test_intraword
               
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 27, 10
     bnel fail_gpr_test_intraword
     lis 27, 0xFF00
     ori 27, 27, 0xFF00
     not 27, 27 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 27, 10
     bnel fail_gpr_test_intraword
               
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 28, 10
     bnel fail_gpr_test_intraword
     lis 28, 0xFF00
     ori 28, 28, 0xFF00
     not 28, 28 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 28, 10
     bnel fail_gpr_test_intraword
               
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 29, 10
     bnel fail_gpr_test_intraword
     lis 29, 0xFF00
     ori 29, 29, 0xFF00
     not 29, 29 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 29, 10
     bnel fail_gpr_test_intraword
               
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 30, 10
     bnel fail_gpr_test_intraword
     lis 30, 0xFF00
     ori 30, 30, 0xFF00
     not 30, 30 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 30, 10
     bnel fail_gpr_test_intraword
               
     lis 10, 0xF0F0
     ori 10, 10, 0xF0F0
     cmplw 31, 10
     bnel fail_gpr_test_intraword
     lis 31, 0xFF00
     ori 31, 31, 0xFF00
     not 31, 31 
     lis 10, 0x00FF
     ori 10, 10, 0x00FF
     cmplw 31, 10
     bnel fail_gpr_test_intraword
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march12
#  Description:    
#       check intra-word faults through sequence           
#       March12:↓[r(0x00FF00FF), w(0xFF00FF00),r(0xFF00FF00)]
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
 .macro march12
       cmplw 31, 11
       bnel fail_gpr_test_intraword
       not 31, 31
       not 11, 11
       cmplw 31, 11
       bnel fail_gpr_test_intraword
       not 11, 11

       cmplw 30, 11
       bnel fail_gpr_test_intraword
       not 30, 30
       not 11, 11
       cmplw 30, 11
       bnel fail_gpr_test_intraword
       not 11, 11

       cmplw 29, 11
       bnel fail_gpr_test_intraword
       not 29, 29
       not 11, 11
       cmplw 29, 11
       bnel fail_gpr_test_intraword
       not 11, 11

       cmplw 28, 11
       bnel fail_gpr_test_intraword
       not 28, 28
       not 11, 11
       cmplw 28, 11
       bnel fail_gpr_test_intraword
       not 11, 11
       
       cmplw 27, 11
       bnel fail_gpr_test_intraword
       not 27, 27
       not 11, 11
       cmplw 27, 11
       bnel fail_gpr_test_intraword
       not 11, 11
       
       cmplw 26, 11
       bnel fail_gpr_test_intraword
       not 26, 26
       not 11, 11
       cmplw 26, 11
       bnel fail_gpr_test_intraword
       not 11, 11
       
       cmplw 25, 11
       bnel fail_gpr_test_intraword
       not 25, 25
       not 11, 11
       cmplw 25, 11
       bnel fail_gpr_test_intraword
       not 11, 11
       
       cmplw 24, 11
       bnel fail_gpr_test_intraword
       not 24, 24
       not 11, 11
       cmplw 24, 11
       bnel fail_gpr_test_intraword
       not 11, 11
       
       cmplw 23, 11
       bnel fail_gpr_test_intraword
       not 23, 23
       not 11, 11
       cmplw 23, 11
       bnel fail_gpr_test_intraword
       not 11, 11
       
       cmplw 22, 11
       bnel fail_gpr_test_intraword
       not 22, 22
       not 11, 11
       cmplw 22, 11
       bnel fail_gpr_test_intraword
       not 11, 11
       
       cmplw 21, 11
       bnel fail_gpr_test_intraword
       not 21, 21
       not 11, 11
       cmplw 21, 11
       bnel fail_gpr_test_intraword
       not 11, 11
       
       cmplw 20, 11
       bnel fail_gpr_test_intraword
       not 20, 20
       not 11, 11
       cmplw 20, 11
       bnel fail_gpr_test_intraword
       not 11, 11
       
       cmplw 19, 11
       bnel fail_gpr_test_intraword
       not 19, 19
       not 11, 11
       cmplw 19, 11
       bnel fail_gpr_test_intraword
       not 11, 11
              
       cmplw 18, 11
       bnel fail_gpr_test_intraword
       not 18, 18
       not 11, 11
       cmplw 18, 11
       bnel fail_gpr_test_intraword
       not 11, 11
              
       cmplw 17, 11
       bnel fail_gpr_test_intraword
       not 17, 17
       not 11, 11
       cmplw 17, 11
       bnel fail_gpr_test_intraword
       not 11, 11
              
       cmplw 16, 11
       bnel fail_gpr_test_intraword
       not 16, 16
       not 11, 11
       cmplw 16, 11
       bnel fail_gpr_test_intraword
       not 11, 11
              
       cmplw 15, 11
       bnel fail_gpr_test_intraword
       not 15, 15
       not 11, 11
       cmplw 15, 11
       bnel fail_gpr_test_intraword
       not 11, 11
              
       cmplw 14, 11
       bnel fail_gpr_test_intraword
       not 14, 14
       not 11, 11
       cmplw 14, 11
       bnel fail_gpr_test_intraword
       not 11, 11
              
       cmplw 13, 11
       bnel fail_gpr_test_intraword
       not 13, 13
       not 11, 11
       cmplw 13, 11
       bnel fail_gpr_test_intraword
       not 11, 11
              
       cmplw 12, 11
       bnel fail_gpr_test_intraword
       not 12, 12
       not 11, 11
       cmplw 12, 11
       bnel fail_gpr_test_intraword
       not 11, 11
              
       cmplw 10, 11
       bnel fail_gpr_test_intraword
       not 10, 10
       not 11, 11
       cmplw 10, 11
       bnel fail_gpr_test_intraword
       not 11, 11
              
       cmplw 9, 11
       bnel fail_gpr_test_intraword
       not 9, 9
       not 11, 11
       cmplw 9, 11
       bnel fail_gpr_test_intraword
       not 11, 11
              
       cmplw 8, 11
       bnel fail_gpr_test_intraword
       not 8, 8
       not 11, 11
       cmplw 8, 11
       bnel fail_gpr_test_intraword
       not 11, 11
                     
       cmplw 7, 11
       bnel fail_gpr_test_intraword
       not 7, 7
       not 11, 11
       cmplw 7, 11
       bnel fail_gpr_test_intraword
       not 11, 11
                     
       cmplw 6, 11
       bnel fail_gpr_test_intraword
       not 6, 6
       not 11, 11
       cmplw 6, 11
       bnel fail_gpr_test_intraword
       not 11, 11
                     
       cmplw 5, 11
       bnel fail_gpr_test_intraword
       not 5, 5
       not 11, 11
       cmplw 5, 11
       bnel fail_gpr_test_intraword
       not 11, 11
                     
       cmplw 4, 11
       bnel fail_gpr_test_intraword
       not 4, 4
       not 11, 11
       cmplw 4, 11
       bnel fail_gpr_test_intraword
       not 11, 11
                     
       cmplw 3, 11
       bnel fail_gpr_test_intraword
       not 3, 3
       not 11, 11
       cmplw 3, 11
       bnel fail_gpr_test_intraword
       not 11, 11
                     
       cmplw 2, 11
       bnel fail_gpr_test_intraword
       not 2, 2
       not 11, 11
       cmplw 2, 11
       bnel fail_gpr_test_intraword
       not 11, 11
                     
       cmplw 1, 11
       bnel fail_gpr_test_intraword
       not 1, 1
       not 11, 11
       cmplw 1, 11
       bnel fail_gpr_test_intraword
       not 11, 11
                     
       cmplw 0, 11
       bnel fail_gpr_test_intraword
       not 0, 0
       not 11, 11
       cmplw 0, 11
       bnel fail_gpr_test_intraword
       not 11, 11
                     
       not 11, 11
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march13
#  Description:    
#       check intra-word faults through sequence           
#       March13:↑[r(0xFF00FF00), w(0xFFFF0000), w(0x0000FFFF),r(0x0000FFFF)]
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
 .macro march13
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 0, 12
      bnel fail_gpr_test_intraword
      lis 0, 0xFFFF
      not 0, 0
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 0, 12
      bnel fail_gpr_test_intraword

      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 1, 12
      bnel fail_gpr_test_intraword
      lis 1, 0xFFFF
      not 1, 1
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 1, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 2, 12
      bnel fail_gpr_test_intraword
      lis 2, 0xFFFF
      not 2, 2
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 2, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 3, 12
      bnel fail_gpr_test_intraword
      lis 3, 0xFFFF
      not 3, 3
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 3, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 4, 12
      bnel fail_gpr_test_intraword
      lis 4, 0xFFFF
      not 4, 4
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 4, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 5, 12
      bnel fail_gpr_test_intraword
      lis 5, 0xFFFF
      not 5, 5
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 5, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 6, 12
      bnel fail_gpr_test_intraword
      lis 6, 0xFFFF
      not 6, 6
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 6, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 7, 12
      bnel fail_gpr_test_intraword
      lis 7, 0xFFFF
      not 7, 7
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 7, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 8, 12
      bnel fail_gpr_test_intraword
      lis 8, 0xFFFF
      not 8, 8
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 8, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 9, 12
      bnel fail_gpr_test_intraword
      lis 9, 0xFFFF
      not 9, 9
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 9, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 10, 12
      bnel fail_gpr_test_intraword
      lis 10, 0xFFFF
      not 10, 10
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 10, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 11, 12
      bnel fail_gpr_test_intraword
      lis 11, 0xFFFF
      not 11, 11
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 11, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 13, 12
      bnel fail_gpr_test_intraword
      lis 13, 0xFFFF
      not 13, 13
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 13, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 14, 12
      bnel fail_gpr_test_intraword
      lis 14, 0xFFFF
      not 14, 14
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 14, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 15, 12
      bnel fail_gpr_test_intraword
      lis 15, 0xFFFF
      not 15, 15
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 15, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 16, 12
      bnel fail_gpr_test_intraword
      lis 16, 0xFFFF
      not 16, 16
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 16, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 17, 12
      bnel fail_gpr_test_intraword
      lis 17, 0xFFFF
      not 17, 17
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 17, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 18, 12
      bnel fail_gpr_test_intraword
      lis 18, 0xFFFF
      not 18, 18
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 18, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 19, 12
      bnel fail_gpr_test_intraword
      lis 19, 0xFFFF
      not 19, 19
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 19, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 20, 12
      bnel fail_gpr_test_intraword
      lis 20, 0xFFFF
      not 20, 20
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 20, 12
      bnel fail_gpr_test_intraword
      
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 21, 12
      bnel fail_gpr_test_intraword
      lis 21, 0xFFFF
      not 21, 21
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 21, 12
      bnel fail_gpr_test_intraword
            
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 22, 12
      bnel fail_gpr_test_intraword
      lis 22, 0xFFFF
      not 22, 22
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 22, 12
      bnel fail_gpr_test_intraword
            
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 23, 12
      bnel fail_gpr_test_intraword
      lis 23, 0xFFFF
      not 23, 23
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 23, 12
      bnel fail_gpr_test_intraword
            
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 24, 12
      bnel fail_gpr_test_intraword
      lis 24, 0xFFFF
      not 24, 24
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 24, 12
      bnel fail_gpr_test_intraword
            
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 25, 12
      bnel fail_gpr_test_intraword
      lis 25, 0xFFFF
      not 25, 25
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 25, 12
      bnel fail_gpr_test_intraword
            
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 26, 12
      bnel fail_gpr_test_intraword
      lis 26, 0xFFFF
      not 26, 26
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 26, 12
      bnel fail_gpr_test_intraword
            
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 27, 12
      bnel fail_gpr_test_intraword
      lis 27, 0xFFFF
      not 27, 27
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 27, 12
      bnel fail_gpr_test_intraword
            
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 28, 12
      bnel fail_gpr_test_intraword
      lis 28, 0xFFFF
      not 28, 28
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 28, 12
      bnel fail_gpr_test_intraword
            
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 29, 12
      bnel fail_gpr_test_intraword
      lis 29, 0xFFFF
      not 29, 29
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 29, 12
      bnel fail_gpr_test_intraword
            
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 30, 12
      bnel fail_gpr_test_intraword
      lis 30, 0xFFFF
      not 30, 30
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 30, 12
      bnel fail_gpr_test_intraword
                  
      lis 12, 0xFF00
      ori 12, 12, 0xFF00
      cmplw 31, 12
      bnel fail_gpr_test_intraword
      lis 31, 0xFFFF
      not 31, 31
      xor 12, 12, 12
      ori 12, 12, 0xFFFF
      cmplw 31, 12
      bnel fail_gpr_test_intraword
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march14
#  Description:    
#       check intra-word faults through sequence           
#       March14:↓[r(0x0000FFFF), w(0xFFFF0000),r(0xFFFF0000)]
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
 .macro march14
     cmplw 31, 13
     bnel fail_gpr_test_intraword
     not 31, 31
     not 13, 13
     cmplw 31, 13
     bnel fail_gpr_test_intraword
     not 13, 13
     
     cmplw 30, 13
     bnel fail_gpr_test_intraword
     not 30, 30
     not 13, 13
     cmplw 30, 13
     bnel fail_gpr_test_intraword
     not 13, 13
          
     cmplw 29, 13
     bnel fail_gpr_test_intraword
     not 29, 29
     not 13, 13
     cmplw 29, 13
     bnel fail_gpr_test_intraword
     not 13, 13
               
     cmplw 28, 13
     bnel fail_gpr_test_intraword
     not 28, 28
     not 13, 13
     cmplw 28, 13
     bnel fail_gpr_test_intraword
     not 13, 13
               
     cmplw 27, 13
     bnel fail_gpr_test_intraword
     not 27, 27
     not 13, 13
     cmplw 27, 13
     bnel fail_gpr_test_intraword
     not 13, 13
               
     cmplw 26, 13
     bnel fail_gpr_test_intraword
     not 26, 26
     not 13, 13
     cmplw 26, 13
     bnel fail_gpr_test_intraword
     not 13, 13
               
     cmplw 25, 13
     bnel fail_gpr_test_intraword
     not 25, 25
     not 13, 13
     cmplw 25, 13
     bnel fail_gpr_test_intraword
     not 13, 13
               
     cmplw 24, 13
     bnel fail_gpr_test_intraword
     not 24, 24
     not 13, 13
     cmplw 24, 13
     bnel fail_gpr_test_intraword
     not 13, 13
               
     cmplw 23, 13
     bnel fail_gpr_test_intraword
     not 23, 23
     not 13, 13
     cmplw 23, 13
     bnel fail_gpr_test_intraword
     not 13, 13
               
     cmplw 22, 13
     bnel fail_gpr_test_intraword
     not 22, 22
     not 13, 13
     cmplw 22, 13
     bnel fail_gpr_test_intraword
     not 13, 13
               
     cmplw 21, 13
     bnel fail_gpr_test_intraword
     not 21, 21
     not 13, 13
     cmplw 21, 13
     bnel fail_gpr_test_intraword
     not 13, 13
               
     cmplw 20, 13
     bnel fail_gpr_test_intraword
     not 20, 20
     not 13, 13
     cmplw 20, 13
     bnel fail_gpr_test_intraword
     not 13, 13
               
     cmplw 19, 13
     bnel fail_gpr_test_intraword
     not 19, 19
     not 13, 13
     cmplw 19, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                 
     cmplw 18, 13
     bnel fail_gpr_test_intraword
     not 18, 18
     not 13, 13
     cmplw 18, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                      
     cmplw 17, 13
     bnel fail_gpr_test_intraword
     not 17, 17
     not 13, 13
     cmplw 17, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                      
     cmplw 16, 13
     bnel fail_gpr_test_intraword
     not 16, 16
     not 13, 13
     cmplw 16, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                      
     cmplw 15, 13
     bnel fail_gpr_test_intraword
     not 15, 15
     not 13, 13
     cmplw 15, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                      
     cmplw 14, 13
     bnel fail_gpr_test_intraword
     not 14, 14
     not 13, 13
     cmplw 14, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                      
     cmplw 12, 13
     bnel fail_gpr_test_intraword
     not 12, 12
     not 13, 13
     cmplw 12, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                           
     cmplw 11, 13
     bnel fail_gpr_test_intraword
     not 11, 11
     not 13, 13
     cmplw 11, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                      
     cmplw 10, 13
     bnel fail_gpr_test_intraword
     not 10, 10
     not 13, 13
     cmplw 10, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                      
     cmplw 9, 13
     bnel fail_gpr_test_intraword
     not 9, 9
     not 13, 13
     cmplw 9, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                           
     cmplw 8, 13
     bnel fail_gpr_test_intraword
     not 8, 8
     not 13, 13
     cmplw 8, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                           
     cmplw 7, 13
     bnel fail_gpr_test_intraword
     not 7, 7
     not 13, 13
     cmplw 7, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                           
     cmplw 6, 13
     bnel fail_gpr_test_intraword
     not 6, 6
     not 13, 13
     cmplw 6, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                           
     cmplw 5, 13
     bnel fail_gpr_test_intraword
     not 5, 5
     not 13, 13
     cmplw 5, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                           
     cmplw 4, 13
     bnel fail_gpr_test_intraword
     not 4, 4
     not 13, 13
     cmplw 4, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                           
     cmplw 3, 13
     bnel fail_gpr_test_intraword
     not 3, 3
     not 13, 13
     cmplw 3, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                           
     cmplw 2, 13
     bnel fail_gpr_test_intraword
     not 2, 2
     not 13, 13
     cmplw 2, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                           
     cmplw 1, 13
     bnel fail_gpr_test_intraword
     not 1, 1
     not 13, 13
     cmplw 1, 13
     bnel fail_gpr_test_intraword
     not 13, 13
                           
     cmplw 0, 13
     bnel fail_gpr_test_intraword
     not 0, 0
     not 13, 13
     cmplw 0, 13
     bnel fail_gpr_test_intraword
     not 13, 13
     
     not 13, 13
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march15
#  Description:    
#		check intra-word faults through sequence March15:↑[r(0xFFFF0000)]
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
 .macro march15
     cmplw 0, 14
     bnel fail_gpr_test_intraword
     
     cmplw 1, 14
     bnel fail_gpr_test_intraword
          
     cmplw 2, 14
     bnel fail_gpr_test_intraword
          
     cmplw 3, 14
     bnel fail_gpr_test_intraword
          
     cmplw 4, 14
     bnel fail_gpr_test_intraword
          
     cmplw 5, 14
     bnel fail_gpr_test_intraword
          
     cmplw 6, 14
     bnel fail_gpr_test_intraword
          
     cmplw 7, 14
     bnel fail_gpr_test_intraword
          
     cmplw 8, 14
     bnel fail_gpr_test_intraword
          
     cmplw 9, 14
     bnel fail_gpr_test_intraword
          
     cmplw 10, 14
     bnel fail_gpr_test_intraword
          
     cmplw 11, 14
     bnel fail_gpr_test_intraword
          
     cmplw 12, 14
     bnel fail_gpr_test_intraword
          
     cmplw 13, 14
     bnel fail_gpr_test_intraword
          
     cmplw 15, 14
     bnel fail_gpr_test_intraword
          
     cmplw 16, 14
     bnel fail_gpr_test_intraword
          
     cmplw 17, 14
     bnel fail_gpr_test_intraword
          
     cmplw 18, 14
     bnel fail_gpr_test_intraword
          
     cmplw 19, 14
     bnel fail_gpr_test_intraword
          
     cmplw 20, 14
     bnel fail_gpr_test_intraword
     
     cmplw 21, 14
     bnel fail_gpr_test_intraword
          
     cmplw 21, 14
     bnel fail_gpr_test_intraword
          
     cmplw 22, 14
     bnel fail_gpr_test_intraword
          
     cmplw 23, 14
     bnel fail_gpr_test_intraword
          
     cmplw 24, 14
     bnel fail_gpr_test_intraword
          
     cmplw 25, 14
     bnel fail_gpr_test_intraword
          
     cmplw 26, 14
     bnel fail_gpr_test_intraword
          
     cmplw 27, 14
     bnel fail_gpr_test_intraword
          
     cmplw 28, 14
     bnel fail_gpr_test_intraword
          
     cmplw 29, 14
     bnel fail_gpr_test_intraword
          
     cmplw 30, 14
     bnel fail_gpr_test_intraword
          
     cmplw 31, 14
     bnel fail_gpr_test_intraword
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:     GPR_intraword_test
#  Description:   
#       It carries out the test for general-purpose registers to check GPR intra-word faults(couple faults)
#  Author:
#  Used module:    None
#  Input Param:
#		GPR3:Test outcome vector
#		GPR4:Mask(OR) to be applied to check vector in case of exit without errors
#		GPR0-GPR31:Registers where the test shall be carried out
#		SPRG4:GPR1 value
#		SPRG5:SWAP register for GPR1
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
 .globl GPRIntraWordTest
 
 GPRIntraWordTest:
     PrologStackOp
     StackCheckBegin
     
     PushParam (3)      #keep imput parameter in stack
     PushParam (4)
     
     StackCheckEnd
     StoreGPRs2Stack
     StoreCtrXerCr2Stack
     
     mfspr 0, SPRG4
     PushReg (0)
     mfspr 0, SPRG5
     PushReg (0)
     
     stw 31, OFFSET_PUNT_STACK_CORR(1)
     TestCPUStart
     
     mtspr SPRG4, 1
     
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
     
     mfspr 1, SPRG4
     TestConclusion
     b end_GPRInterWordTest
     
fail_gpr_test_intraword:
     mtspr SPRG5, 1
     mfspr 1, SPRG4
     
     ErrorHandling
     
     mtspr SPRG4, 1
     mfspr 1,SPRG5
     
     mfspr 2, SPRG7
     subi 2, 2, 256
     stw 1, 132(2)
     
     mfspr 1, SPRG4
     
end_GPRInterWordTest:
     StackEpilogBegin
     
     PopReg (0)
     mtspr SPRG5, 0
     PopReg (0)
     mtspr SPRG4, 0
     
     RestoreCtrXerCr
     StackEpilogEnd
     blr
 .end
