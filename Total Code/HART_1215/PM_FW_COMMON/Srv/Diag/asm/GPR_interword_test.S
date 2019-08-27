#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march0
#  Description:    
#       write 0x0000 to gpr0-31
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
 .macro march0
     lis 0, 0x0000
     lis 1, 0x0000
     lis 2, 0x0000
     lis 3, 0x0000
     lis 4, 0x0000
     lis 5, 0x0000
     lis 6, 0x0000
     lis 7, 0x0000
     lis 8, 0x0000
     lis 9, 0x0000
     lis 10, 0x0000
     lis 11, 0x0000
     lis 12, 0x0000
     lis 13, 0x0000
     lis 14, 0x0000
     lis 15, 0x0000
     lis 16, 0x0000
     lis 17, 0x0000
     lis 18, 0x0000
     lis 19, 0x0000
     lis 20, 0x0000
     lis 21, 0x0000
     lis 22, 0x0000
     lis 23, 0x0000
     lis 24, 0x0000
     lis 25, 0x0000
     lis 26, 0x0000
     lis 27, 0x0000
     lis 28, 0x0000
     lis 29, 0x0000
     lis 30, 0x0000
     lis 31, 0x0000
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march1
#  Description:    
#       check gpr through sequence of March1:↓(r0,r0,w0,r0,w1),  compare with gpr0
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
 .macro march1
     cmplw 31, 0                  #compare r31 with r0
     bnel fail_gpr_test_interword
     cmplw 31, 0
     bnel fail_gpr_test_interword
     mr 31, 0
     cmplw 31, 0 
     bnel fail_gpr_test_interword
     not 31, 31
     
     cmplw 30, 0
     bnel fail_gpr_test_interword
     cmplw 30, 0
     bnel fail_gpr_test_interword
     mr 30, 0
     cmplw 30, 0 
     bnel fail_gpr_test_interword
     not 30, 30

     cmplw 29, 0
     bnel fail_gpr_test_interword
     cmplw 29, 0
     bnel fail_gpr_test_interword
     mr 29, 0
     cmplw 29, 0 
     bnel fail_gpr_test_interword
     not 29, 29

     cmplw 28, 0
     bnel fail_gpr_test_interword
     cmplw 28, 0
     bnel fail_gpr_test_interword
     mr 28, 0
     cmplw 28, 0 
     bnel fail_gpr_test_interword
     not 28, 28

     cmplw 27, 0
     bnel fail_gpr_test_interword
     cmplw 27, 0
     bnel fail_gpr_test_interword
     mr 27, 0
     cmplw 27, 0 
     bnel fail_gpr_test_interword
     not 27, 27

     cmplw 26, 0
     bnel fail_gpr_test_interword
     cmplw 26, 0
     bnel fail_gpr_test_interword
     mr 26, 0
     cmplw 26, 0 
     bnel fail_gpr_test_interword
     not 26, 26

     cmplw 25, 0
     bnel fail_gpr_test_interword
     cmplw 25, 0
     bnel fail_gpr_test_interword
     mr 25, 0
     cmplw 25, 0 
     bnel fail_gpr_test_interword
     not 25, 25

     cmplw 24, 0
     bnel fail_gpr_test_interword
     cmplw 24, 0
     bnel fail_gpr_test_interword
     mr 24, 0
     cmplw 24, 0 
     bnel fail_gpr_test_interword
     not 24, 24

     cmplw 23, 0
     bnel fail_gpr_test_interword
     cmplw 23, 0
     bnel fail_gpr_test_interword
     mr 23, 0
     cmplw 23, 0 
     bnel fail_gpr_test_interword
     not 23, 23

     cmplw 22, 0
     bnel fail_gpr_test_interword
     cmplw 22, 0
     bnel fail_gpr_test_interword
     mr 22, 0
     cmplw 22, 0 
     bnel fail_gpr_test_interword
     not 22, 22

     cmplw 21, 0
     bnel fail_gpr_test_interword
     cmplw 21, 0
     bnel fail_gpr_test_interword
     mr 21, 0
     cmplw 21, 0 
     bnel fail_gpr_test_interword
     not 21, 21

     cmplw 20, 0
     bnel fail_gpr_test_interword
     cmplw 20, 0
     bnel fail_gpr_test_interword
     mr 20, 0
     cmplw 20, 0 
     bnel fail_gpr_test_interword
     not 20, 20

     cmplw 19, 0
     bnel fail_gpr_test_interword
     cmplw 19, 0
     bnel fail_gpr_test_interword
     mr 19, 0
     cmplw 19, 0 
     bnel fail_gpr_test_interword
     not 19, 19

     cmplw 18, 0
     bnel fail_gpr_test_interword
     cmplw 18, 0
     bnel fail_gpr_test_interword
     mr 18, 0
     cmplw 18, 0 
     bnel fail_gpr_test_interword
     not 18, 18

     cmplw 17, 0
     bnel fail_gpr_test_interword
     cmplw 17, 0
     bnel fail_gpr_test_interword
     mr 17, 0
     cmplw 17, 0 
     bnel fail_gpr_test_interword
     not 17, 17
     
     cmplw 16, 0
     bnel fail_gpr_test_interword
     cmplw 16, 0
     bnel fail_gpr_test_interword
     mr 16, 0
     cmplw 16, 0 
     bnel fail_gpr_test_interword
     not 16, 16

     cmplw 15, 0
     bnel fail_gpr_test_interword
     cmplw 15, 0
     bnel fail_gpr_test_interword
     mr 15, 0
     cmplw 15, 0 
     bnel fail_gpr_test_interword
     not 15, 15

     cmplw 14, 0
     bnel fail_gpr_test_interword
     cmplw 14, 0
     bnel fail_gpr_test_interword
     mr 14, 0
     cmplw 14, 0 
     bnel fail_gpr_test_interword
     not 14, 14

     cmplw 13, 0
     bnel fail_gpr_test_interword
     cmplw 13, 0
     bnel fail_gpr_test_interword
     mr 13, 0
     cmplw 13, 0 
     bnel fail_gpr_test_interword
     not 13, 13

     cmplw 12, 0
     bnel fail_gpr_test_interword
     cmplw 12, 0
     bnel fail_gpr_test_interword
     mr 12, 0
     cmplw 12, 0 
     bnel fail_gpr_test_interword
     not 12, 12

     cmplw 11, 0
     bnel fail_gpr_test_interword
     cmplw 11, 0
     bnel fail_gpr_test_interword
     mr 11, 0
     cmplw 11, 0 
     bnel fail_gpr_test_interword
     not 11, 11

     cmplw 10, 0
     bnel fail_gpr_test_interword
     cmplw 10, 0
     bnel fail_gpr_test_interword
     mr 10, 0
     cmplw 10, 0 
     bnel fail_gpr_test_interword
     not 10, 10

     cmplw 9, 0
     bnel fail_gpr_test_interword
     cmplw 9, 0
     bnel fail_gpr_test_interword
     mr 9, 0
     cmplw 9, 0 
     bnel fail_gpr_test_interword
     not 9, 9

     cmplw 8, 0
     bnel fail_gpr_test_interword
     cmplw 8, 0
     bnel fail_gpr_test_interword
     mr 8, 0
     cmplw 8, 0 
     bnel fail_gpr_test_interword
     not 8, 8

     cmplw 7, 0
     bnel fail_gpr_test_interword
     cmplw 7, 0
     bnel fail_gpr_test_interword
     mr 7, 0
     cmplw 7, 0 
     bnel fail_gpr_test_interword
     not 7, 7

     cmplw 6, 0
     bnel fail_gpr_test_interword
     cmplw 6, 0
     bnel fail_gpr_test_interword
     mr 6, 0
     cmplw 6, 0 
     bnel fail_gpr_test_interword
     not 6, 6

     cmplw 5, 0
     bnel fail_gpr_test_interword
     cmplw 5, 0
     bnel fail_gpr_test_interword
     mr 5, 0
     cmplw 5, 0 
     bnel fail_gpr_test_interword
     not 5, 5
     
     cmplw 4, 0
     bnel fail_gpr_test_interword
     cmplw 4, 0
     bnel fail_gpr_test_interword
     mr 4, 0
     cmplw 4, 0 
     bnel fail_gpr_test_interword
     not 4, 4
     
     cmplw 3, 0
     bnel fail_gpr_test_interword
     cmplw 3, 0
     bnel fail_gpr_test_interword
     mr 3, 0
     cmplw 3, 0 
     bnel fail_gpr_test_interword
     not 3, 3
     
     cmplw 2, 0
     bnel fail_gpr_test_interword
     cmplw 2, 0
     bnel fail_gpr_test_interword
     mr 2, 0
     cmplw 2, 0 
     bnel fail_gpr_test_interword
     not 2, 2
     
     cmplw 1, 0
     bnel fail_gpr_test_interword
     cmplw 1, 0
     bnel fail_gpr_test_interword
     mr 1, 0
     cmplw 1, 0 
     bnel fail_gpr_test_interword
     not 1, 1

     not 0 , 0
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march2
#  Description:    
#       check gpr through sequence of March2:↑(r1,r1,w1,r1,w0),  compare with gpr1
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
 .macro march2
     cmplw 0, 1
     bnel fail_gpr_test_interword
     cmplw 0, 1
     bnel fail_gpr_test_interword
     mr 0, 1
     cmplw 0, 1
     bnel fail_gpr_test_interword
     not 0, 0

     cmplw 2, 1
     bnel fail_gpr_test_interword
     cmplw 2, 1
     bnel fail_gpr_test_interword
     mr 2, 1
     cmplw 2, 1
     bnel fail_gpr_test_interword
     not 2, 2

     cmplw 3, 1
     bnel fail_gpr_test_interword
     cmplw 3, 1
     bnel fail_gpr_test_interword
     mr 3, 1
     cmplw 3, 1
     bnel fail_gpr_test_interword
     not 3, 3

     cmplw 4, 1
     bnel fail_gpr_test_interword
     cmplw 4, 1
     bnel fail_gpr_test_interword
     mr 4, 1
     cmplw 4, 1
     bnel fail_gpr_test_interword
     not 4, 4
     
     cmplw 5, 1
     bnel fail_gpr_test_interword
     cmplw 5, 1
     bnel fail_gpr_test_interword
     mr 5, 1
     cmplw 5, 1
     bnel fail_gpr_test_interword
     not 5, 5
     
     cmplw 6, 1
     bnel fail_gpr_test_interword
     cmplw 6, 1
     bnel fail_gpr_test_interword
     mr 6, 1
     cmplw 6, 1
     bnel fail_gpr_test_interword
     not 6, 6
     
     cmplw 7, 1
     bnel fail_gpr_test_interword
     cmplw 7, 1
     bnel fail_gpr_test_interword
     mr 7, 1
     cmplw 7, 1
     bnel fail_gpr_test_interword
     not 7, 7
     
     cmplw 8, 1
     bnel fail_gpr_test_interword
     cmplw 8, 1
     bnel fail_gpr_test_interword
     mr 8, 1
     cmplw 8, 1
     bnel fail_gpr_test_interword
     not 8, 8
     
     cmplw 9, 1
     bnel fail_gpr_test_interword
     cmplw 9, 1
     bnel fail_gpr_test_interword
     mr 9, 1
     cmplw 9, 1
     bnel fail_gpr_test_interword
     not 9, 9
     
     cmplw 10, 1
     bnel fail_gpr_test_interword
     cmplw 10, 1
     bnel fail_gpr_test_interword
     mr 10, 1
     cmplw 10, 1
     bnel fail_gpr_test_interword
     not 10, 10
     
     cmplw 11, 1
     bnel fail_gpr_test_interword
     cmplw 11, 1
     bnel fail_gpr_test_interword
     mr 11, 1
     cmplw 11, 1
     bnel fail_gpr_test_interword
     not 11, 11
     
     cmplw 12, 1
     bnel fail_gpr_test_interword
     cmplw 12, 1
     bnel fail_gpr_test_interword
     mr 12, 1
     cmplw 12, 1
     bnel fail_gpr_test_interword
     not 12, 12
     
     cmplw 13, 1
     bnel fail_gpr_test_interword
     cmplw 13, 1
     bnel fail_gpr_test_interword
     mr 13, 1
     cmplw 13, 1
     bnel fail_gpr_test_interword
     not 13, 13

     cmplw 14, 1
     bnel fail_gpr_test_interword
     cmplw 14, 1
     bnel fail_gpr_test_interword
     mr 14, 1
     cmplw 14, 1
     bnel fail_gpr_test_interword
     not 14, 14

     cmplw 15, 1
     bnel fail_gpr_test_interword
     cmplw 15, 1
     bnel fail_gpr_test_interword
     mr 15, 1
     cmplw 15, 1
     bnel fail_gpr_test_interword
     not 15, 15
     
     cmplw 16, 1
     bnel fail_gpr_test_interword
     cmplw 16, 1
     bnel fail_gpr_test_interword
     mr 16, 1
     cmplw 16, 1
     bnel fail_gpr_test_interword
     not 16, 16
     
     cmplw 17, 1
     bnel fail_gpr_test_interword
     cmplw 17, 1
     bnel fail_gpr_test_interword
     mr 17, 1
     cmplw 17, 1
     bnel fail_gpr_test_interword
     not 17, 17
     
     cmplw 18, 1
     bnel fail_gpr_test_interword
     cmplw 18, 1
     bnel fail_gpr_test_interword
     mr 18, 1
     cmplw 18, 1
     bnel fail_gpr_test_interword
     not 18, 18
     
     cmplw 19, 1
     bnel fail_gpr_test_interword
     cmplw 19, 1
     bnel fail_gpr_test_interword
     mr 19, 1
     cmplw 19, 1
     bnel fail_gpr_test_interword
     not 19, 19
     
     cmplw 20, 1
     bnel fail_gpr_test_interword
     cmplw 20, 1
     bnel fail_gpr_test_interword
     mr 20, 1
     cmplw 20, 1
     bnel fail_gpr_test_interword
     not 20, 20
     
     cmplw 21, 1
     bnel fail_gpr_test_interword
     cmplw 21, 1
     bnel fail_gpr_test_interword
     mr 21, 1
     cmplw 21, 1
     bnel fail_gpr_test_interword
     not 21, 21
     
     cmplw 22, 1
     bnel fail_gpr_test_interword
     cmplw 22, 1
     bnel fail_gpr_test_interword
     mr 22, 1
     cmplw 22, 1
     bnel fail_gpr_test_interword
     not 22, 22
     
     cmplw 23, 1
     bnel fail_gpr_test_interword
     cmplw 23, 1
     bnel fail_gpr_test_interword
     mr 23, 1
     cmplw 23, 1
     bnel fail_gpr_test_interword
     not 23, 23
     
     cmplw 24, 1
     bnel fail_gpr_test_interword
     cmplw 24, 1
     bnel fail_gpr_test_interword
     mr 24, 1
     cmplw 24, 1
     bnel fail_gpr_test_interword
     not 24, 24
     
     cmplw 25, 1
     bnel fail_gpr_test_interword
     cmplw 25, 1
     bnel fail_gpr_test_interword
     mr 25, 1
     cmplw 25, 1
     bnel fail_gpr_test_interword
     not 25, 25
     
     cmplw 26, 1
     bnel fail_gpr_test_interword
     cmplw 26, 1
     bnel fail_gpr_test_interword
     mr 26, 1
     cmplw 26, 1
     bnel fail_gpr_test_interword
     not 26, 26
     
     cmplw 27, 1
     bnel fail_gpr_test_interword
     cmplw 27, 1
     bnel fail_gpr_test_interword
     mr 27, 1
     cmplw 27, 1
     bnel fail_gpr_test_interword
     not 27, 27
     
     cmplw 28, 1
     bnel fail_gpr_test_interword
     cmplw 28, 1
     bnel fail_gpr_test_interword
     mr 28, 1
     cmplw 28, 1
     bnel fail_gpr_test_interword
     not 28, 28
     
     cmplw 29, 1
     bnel fail_gpr_test_interword
     cmplw 29, 1
     bnel fail_gpr_test_interword
     mr 29, 1
     cmplw 29, 1
     bnel fail_gpr_test_interword
     not 29, 29
     
     cmplw 30, 1
     bnel fail_gpr_test_interword
     cmplw 30, 1
     bnel fail_gpr_test_interword
     mr 30, 1
     cmplw 30, 1
     bnel fail_gpr_test_interword
     not 30, 30
     
     cmplw 31, 1
     bnel fail_gpr_test_interword
     cmplw 31, 1
     bnel fail_gpr_test_interword
     mr 31, 1
     cmplw 31, 1
     bnel fail_gpr_test_interword
     not 31, 31

     not 1, 1
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march3
#  Description:    
#       check gpr through sequence of March3:↑(r0,r0,w0,r0,w1),  compare with gpr2
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
 .macro march3
     cmplw 0, 2
     bnel fail_gpr_test_interword
     cmplw 0, 2
     bnel fail_gpr_test_interword
     mr 0, 2
     cmplw 0, 2
     bnel fail_gpr_test_interword
     not 0, 0

     cmplw 1, 2
     bnel fail_gpr_test_interword
     cmplw 1, 2
     bnel fail_gpr_test_interword
     mr 1, 2
     cmplw 1, 2
     bnel fail_gpr_test_interword
     not 1, 1

     cmplw 3, 2
     bnel fail_gpr_test_interword
     cmplw 3, 2
     bnel fail_gpr_test_interword
     mr 3, 2
     cmplw 3, 2
     bnel fail_gpr_test_interword
     not 3, 3
     
     cmplw 4, 2
     bnel fail_gpr_test_interword
     cmplw 4, 2
     bnel fail_gpr_test_interword
     mr 4, 2
     cmplw 4, 2
     bnel fail_gpr_test_interword
     not 4, 4
     
     cmplw 5, 2
     bnel fail_gpr_test_interword
     cmplw 5, 2
     bnel fail_gpr_test_interword
     mr 5, 2
     cmplw 5, 2
     bnel fail_gpr_test_interword
     not 5, 5
     
     cmplw 6, 2
     bnel fail_gpr_test_interword
     cmplw 6, 2
     bnel fail_gpr_test_interword
     mr 6, 2
     cmplw 6, 2
     bnel fail_gpr_test_interword
     not 6, 6
     
     cmplw 7, 2
     bnel fail_gpr_test_interword
     cmplw 7, 2
     bnel fail_gpr_test_interword
     mr 7, 2
     cmplw 7, 2
     bnel fail_gpr_test_interword
     not 7, 7
     
     cmplw 8, 2
     bnel fail_gpr_test_interword
     cmplw 8, 2
     bnel fail_gpr_test_interword
     mr 8, 2
     cmplw 8, 2
     bnel fail_gpr_test_interword
     not 8, 8
     
     cmplw 9, 2
     bnel fail_gpr_test_interword
     cmplw 9, 2
     bnel fail_gpr_test_interword
     mr 9, 2
     cmplw 9, 2
     bnel fail_gpr_test_interword
     not 9, 9
     
     cmplw 10, 2
     bnel fail_gpr_test_interword
     cmplw 10, 2
     bnel fail_gpr_test_interword
     mr 10, 2
     cmplw 10, 2
     bnel fail_gpr_test_interword
     not 10, 10
     
     cmplw 11, 2
     bnel fail_gpr_test_interword
     cmplw 11, 2
     bnel fail_gpr_test_interword
     mr 11, 2
     cmplw 11, 2
     bnel fail_gpr_test_interword
     not 11, 11
     
     cmplw 12, 2
     bnel fail_gpr_test_interword
     cmplw 12, 2
     bnel fail_gpr_test_interword
     mr 12, 2
     cmplw 12, 2
     bnel fail_gpr_test_interword
     not 12, 12
     
     cmplw 13, 2
     bnel fail_gpr_test_interword
     cmplw 13, 2
     bnel fail_gpr_test_interword
     mr 13, 2
     cmplw 13, 2
     bnel fail_gpr_test_interword
     not 13, 13
     
     cmplw 14, 2
     bnel fail_gpr_test_interword
     cmplw 14, 2
     bnel fail_gpr_test_interword
     mr 14, 2
     cmplw 14, 2
     bnel fail_gpr_test_interword
     not 14, 14
     
     cmplw 15, 2
     bnel fail_gpr_test_interword
     cmplw 15, 2
     bnel fail_gpr_test_interword
     mr 15, 2
     cmplw 15, 2
     bnel fail_gpr_test_interword
     not 15, 15

     cmplw 16, 2
     bnel fail_gpr_test_interword
     cmplw 16, 2
     bnel fail_gpr_test_interword
     mr 16, 2
     cmplw 16, 2
     bnel fail_gpr_test_interword
     not 16, 16

     cmplw 17, 2
     bnel fail_gpr_test_interword
     cmplw 17, 2
     bnel fail_gpr_test_interword
     mr 17, 2
     cmplw 17, 2
     bnel fail_gpr_test_interword
     not 17, 17
     
     cmplw 18, 2
     bnel fail_gpr_test_interword
     cmplw 18, 2
     bnel fail_gpr_test_interword
     mr 18, 2
     cmplw 18, 2
     bnel fail_gpr_test_interword
     not 18, 18
     
     cmplw 19, 2
     bnel fail_gpr_test_interword
     cmplw 19, 2
     bnel fail_gpr_test_interword
     mr 19, 2
     cmplw 19, 2
     bnel fail_gpr_test_interword
     not 19, 19
     
     cmplw 20, 2
     bnel fail_gpr_test_interword
     cmplw 20, 2
     bnel fail_gpr_test_interword
     mr 20, 2
     cmplw 20, 2
     bnel fail_gpr_test_interword
     not 20, 20
     
     cmplw 21, 2
     bnel fail_gpr_test_interword
     cmplw 21, 2
     bnel fail_gpr_test_interword
     mr 21, 2
     cmplw 21, 2
     bnel fail_gpr_test_interword
     not 21, 21

     cmplw 22, 2
     bnel fail_gpr_test_interword
     cmplw 22, 2
     bnel fail_gpr_test_interword
     mr 22, 2
     cmplw 22, 2
     bnel fail_gpr_test_interword
     not 22, 22
     
     cmplw 23, 2
     bnel fail_gpr_test_interword
     cmplw 23, 2
     bnel fail_gpr_test_interword
     mr 23, 2
     cmplw 23, 2
     bnel fail_gpr_test_interword
     not 23, 23
     
     cmplw 24, 2
     bnel fail_gpr_test_interword
     cmplw 24, 2
     bnel fail_gpr_test_interword
     mr 24, 2
     cmplw 24, 2
     bnel fail_gpr_test_interword
     not 24, 24
     
     cmplw 25, 2
     bnel fail_gpr_test_interword
     cmplw 25, 2
     bnel fail_gpr_test_interword
     mr 25, 2
     cmplw 25, 2
     bnel fail_gpr_test_interword
     not 25, 25
     
     cmplw 26, 2
     bnel fail_gpr_test_interword
     cmplw 26, 2
     bnel fail_gpr_test_interword
     mr 26, 2
     cmplw 26, 2
     bnel fail_gpr_test_interword
     not 26, 26
     
     cmplw 27, 2
     bnel fail_gpr_test_interword
     cmplw 27, 2
     bnel fail_gpr_test_interword
     mr 27, 2
     cmplw 27, 2
     bnel fail_gpr_test_interword
     not 27, 27
     
     cmplw 28, 2
     bnel fail_gpr_test_interword
     cmplw 28, 2
     bnel fail_gpr_test_interword
     mr 28, 2
     cmplw 28, 2
     bnel fail_gpr_test_interword
     not 28, 28
     
     cmplw 29, 2
     bnel fail_gpr_test_interword
     cmplw 29, 2
     bnel fail_gpr_test_interword
     mr 29, 2
     cmplw 29, 2
     bnel fail_gpr_test_interword
     not 29, 29
     
     cmplw 30, 2
     bnel fail_gpr_test_interword
     cmplw 30, 2
     bnel fail_gpr_test_interword
     mr 30, 2
     cmplw 30, 2
     bnel fail_gpr_test_interword
     not 30, 30
     
     cmplw 31, 2
     bnel fail_gpr_test_interword
     cmplw 31, 2
     bnel fail_gpr_test_interword
     mr 31, 2
     cmplw 31, 2
     bnel fail_gpr_test_interword
     not 31, 31
    
     not 2, 2
 .endm

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march4
#  Description:    
#       check gpr through sequence of March4:↑(r1,r1,w1,r1,r1,w0),  compare with gpr3
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
 .macro march4
 
     cmplw 0, 3
     bnel fail_gpr_test_interword
     cmplw 0, 3
     bnel fail_gpr_test_interword
     mr 0, 3
     cmplw 0, 3
     bnel fail_gpr_test_interword
     cmplw 0, 3
     bnel fail_gpr_test_interword
     not 0, 0
 
     cmplw 1, 3
     bnel fail_gpr_test_interword
     cmplw 1, 3
     bnel fail_gpr_test_interword
     mr 1, 3
     cmplw 1, 3
     bnel fail_gpr_test_interword
     cmplw 1, 3
     bnel fail_gpr_test_interword
     not 1, 1
      
     cmplw 2, 3
     bnel fail_gpr_test_interword
     cmplw 2, 3
     bnel fail_gpr_test_interword
     mr 2, 3
     cmplw 2, 3
     bnel fail_gpr_test_interword
     cmplw 2, 3
     bnel fail_gpr_test_interword
     not 2, 2
      
     cmplw 4, 3
     bnel fail_gpr_test_interword
     cmplw 4, 3
     bnel fail_gpr_test_interword
     mr 4, 3
     cmplw 4, 3
     bnel fail_gpr_test_interword
     cmplw 4, 3
     bnel fail_gpr_test_interword
     not 4, 4
      
     cmplw 5, 3
     bnel fail_gpr_test_interword
     cmplw 5, 3
     bnel fail_gpr_test_interword
     mr 5, 3
     cmplw 5, 3
     bnel fail_gpr_test_interword
     cmplw 5, 3
     bnel fail_gpr_test_interword
     not 5, 5
      
     cmplw 6, 3
     bnel fail_gpr_test_interword
     cmplw 6, 3
     bnel fail_gpr_test_interword
     mr 6, 3
     cmplw 6, 3
     bnel fail_gpr_test_interword
     cmplw 6, 3
     bnel fail_gpr_test_interword
     not 6, 6
      
     cmplw 7, 3
     bnel fail_gpr_test_interword
     cmplw 7, 3
     bnel fail_gpr_test_interword
     mr 7, 3
     cmplw 7, 3
     bnel fail_gpr_test_interword
     cmplw 7, 3
     bnel fail_gpr_test_interword
     not 7, 7
      
     cmplw 8, 3
     bnel fail_gpr_test_interword
     cmplw 8, 3
     bnel fail_gpr_test_interword
     mr 8, 3
     cmplw 8, 3
     bnel fail_gpr_test_interword
     cmplw 8, 3
     bnel fail_gpr_test_interword
     not 8, 8
      
     cmplw 9, 3
     bnel fail_gpr_test_interword
     cmplw 9, 3
     bnel fail_gpr_test_interword
     mr 9, 3
     cmplw 9, 3
     bnel fail_gpr_test_interword
     cmplw 9, 3
     bnel fail_gpr_test_interword
     not 9, 9
      
     cmplw 10, 3
     bnel fail_gpr_test_interword
     cmplw 10, 3
     bnel fail_gpr_test_interword
     mr 10, 3
     cmplw 10, 3
     bnel fail_gpr_test_interword
     cmplw 10, 3
     bnel fail_gpr_test_interword
     not 10, 10
      
     cmplw 11, 3
     bnel fail_gpr_test_interword
     cmplw 11, 3
     bnel fail_gpr_test_interword
     mr 11, 3
     cmplw 11, 3
     bnel fail_gpr_test_interword
     cmplw 11, 3
     bnel fail_gpr_test_interword
     not 11, 11
      
     cmplw 12, 3
     bnel fail_gpr_test_interword
     cmplw 12, 3
     bnel fail_gpr_test_interword
     mr 12, 3
     cmplw 12, 3
     bnel fail_gpr_test_interword
     cmplw 12, 3
     bnel fail_gpr_test_interword
     not 12, 12
      
     cmplw 13, 3
     bnel fail_gpr_test_interword
     cmplw 13, 3
     bnel fail_gpr_test_interword
     mr 13, 3
     cmplw 13, 3
     bnel fail_gpr_test_interword
     cmplw 13, 3
     bnel fail_gpr_test_interword
     not 13, 13
      
     cmplw 14, 3
     bnel fail_gpr_test_interword
     cmplw 14, 3
     bnel fail_gpr_test_interword
     mr 14, 3
     cmplw 14, 3
     bnel fail_gpr_test_interword
     cmplw 14, 3
     bnel fail_gpr_test_interword
     not 14, 14
      
     cmplw 15, 3
     bnel fail_gpr_test_interword
     cmplw 15, 3
     bnel fail_gpr_test_interword
     mr 15, 3
     cmplw 15, 3
     bnel fail_gpr_test_interword
     cmplw 15, 3
     bnel fail_gpr_test_interword
     not 15, 15
      
     cmplw 16, 3
     bnel fail_gpr_test_interword
     cmplw 16, 3
     bnel fail_gpr_test_interword
     mr 16, 3
     cmplw 16, 3
     bnel fail_gpr_test_interword
     cmplw 16, 3
     bnel fail_gpr_test_interword
     not 16, 16
      
     cmplw 17, 3
     bnel fail_gpr_test_interword
     cmplw 17, 3
     bnel fail_gpr_test_interword
     mr 17, 3
     cmplw 17, 3
     bnel fail_gpr_test_interword
     cmplw 17, 3
     bnel fail_gpr_test_interword
     not 17, 17
      
     cmplw 18, 3
     bnel fail_gpr_test_interword
     cmplw 18, 3
     bnel fail_gpr_test_interword
     mr 18, 3
     cmplw 18, 3
     bnel fail_gpr_test_interword
     cmplw 18, 3
     bnel fail_gpr_test_interword
     not 18, 18
      
     cmplw 19, 3
     bnel fail_gpr_test_interword
     cmplw 19, 3
     bnel fail_gpr_test_interword
     mr 19, 3
     cmplw 19, 3
     bnel fail_gpr_test_interword
     cmplw 19, 3
     bnel fail_gpr_test_interword
     not 19, 19
      
     cmplw 20, 3
     bnel fail_gpr_test_interword
     cmplw 20, 3
     bnel fail_gpr_test_interword
     mr 20, 3
     cmplw 20, 3
     bnel fail_gpr_test_interword
     cmplw 20, 3
     bnel fail_gpr_test_interword
     not 20, 20
      
     cmplw 21, 3
     bnel fail_gpr_test_interword
     cmplw 21, 3
     bnel fail_gpr_test_interword
     mr 21, 3
     cmplw 21, 3
     bnel fail_gpr_test_interword
     cmplw 21, 3
     bnel fail_gpr_test_interword
     not 21, 21
      
     cmplw 22, 3
     bnel fail_gpr_test_interword
     cmplw 22, 3
     bnel fail_gpr_test_interword
     mr 22, 3
     cmplw 22, 3
     bnel fail_gpr_test_interword
     cmplw 22, 3
     bnel fail_gpr_test_interword
     not 22, 22
      
     cmplw 23, 3
     bnel fail_gpr_test_interword
     cmplw 23, 3
     bnel fail_gpr_test_interword
     mr 23, 3
     cmplw 23, 3
     bnel fail_gpr_test_interword
     cmplw 23, 3
     bnel fail_gpr_test_interword
     not 23, 23
      
     cmplw 24, 3
     bnel fail_gpr_test_interword
     cmplw 24, 3
     bnel fail_gpr_test_interword
     mr 24, 3
     cmplw 24, 3
     bnel fail_gpr_test_interword
     cmplw 24, 3
     bnel fail_gpr_test_interword
     not 24, 24
      
     cmplw 25, 3
     bnel fail_gpr_test_interword
     cmplw 25, 3
     bnel fail_gpr_test_interword
     mr 25, 3
     cmplw 25, 3
     bnel fail_gpr_test_interword
     cmplw 25, 3
     bnel fail_gpr_test_interword
     not 25, 25
      
     cmplw 26, 3
     bnel fail_gpr_test_interword
     cmplw 26, 3
     bnel fail_gpr_test_interword
     mr 26, 3
     cmplw 26, 3
     bnel fail_gpr_test_interword
     cmplw 26, 3
     bnel fail_gpr_test_interword
     not 26, 26
      
     cmplw 27, 3
     bnel fail_gpr_test_interword
     cmplw 27, 3
     bnel fail_gpr_test_interword
     mr 27, 3
     cmplw 27, 3
     bnel fail_gpr_test_interword
     cmplw 27, 3
     bnel fail_gpr_test_interword
     not 27, 27
      
     cmplw 28, 3
     bnel fail_gpr_test_interword
     cmplw 28, 3
     bnel fail_gpr_test_interword
     mr 28, 3
     cmplw 28, 3
     bnel fail_gpr_test_interword
     cmplw 28, 3
     bnel fail_gpr_test_interword
     not 28, 28
      
     cmplw 29, 3
     bnel fail_gpr_test_interword
     cmplw 29, 3
     bnel fail_gpr_test_interword
     mr 29, 3
     cmplw 29, 3
     bnel fail_gpr_test_interword
     cmplw 29, 3
     bnel fail_gpr_test_interword
     not 29, 29
      
     cmplw 30, 3
     bnel fail_gpr_test_interword
     cmplw 30, 3
     bnel fail_gpr_test_interword
     mr 30, 3
     cmplw 30, 3
     bnel fail_gpr_test_interword
     cmplw 30, 3
     bnel fail_gpr_test_interword
     not 30, 30
      
     cmplw 31, 3
     bnel fail_gpr_test_interword
     cmplw 31, 3
     bnel fail_gpr_test_interword
     mr 31, 3
     cmplw 31, 3
     bnel fail_gpr_test_interword
     cmplw 31, 3
     bnel fail_gpr_test_interword
     not 31, 31
      
     not 3, 3
 .endm
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      march5
#  Description:    
#       check gpr through sequence of March5:↑(r0),  compare with gpr4
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
     cmplw 0, 4
     bnel fail_gpr_test_interword
     
     cmplw 1, 4
     bnel fail_gpr_test_interword
     
     cmplw 2, 4
     bnel fail_gpr_test_interword
          
     cmplw 3, 4
     bnel fail_gpr_test_interword
          
     cmplw 5, 4
     bnel fail_gpr_test_interword
          
     cmplw 6, 4
     bnel fail_gpr_test_interword
          
     cmplw 7, 4
     bnel fail_gpr_test_interword
          
     cmplw 8, 4
     bnel fail_gpr_test_interword
          
     cmplw 9, 4
     bnel fail_gpr_test_interword
          
     cmplw 10, 4
     bnel fail_gpr_test_interword
          
     cmplw 11, 4
     bnel fail_gpr_test_interword
          
     cmplw 12, 4
     bnel fail_gpr_test_interword
          
     cmplw 13, 4
     bnel fail_gpr_test_interword
          
     cmplw 14, 4
     bnel fail_gpr_test_interword
          
     cmplw 15, 4
     bnel fail_gpr_test_interword
          
     cmplw 16, 4
     bnel fail_gpr_test_interword
          
     cmplw 17, 4
     bnel fail_gpr_test_interword
          
     cmplw 18, 4
     bnel fail_gpr_test_interword
          
     cmplw 19, 4
     bnel fail_gpr_test_interword
          
     cmplw 20, 4
     bnel fail_gpr_test_interword
          
     cmplw 21, 4
     bnel fail_gpr_test_interword
          
     cmplw 22, 4
     bnel fail_gpr_test_interword
          
     cmplw 23, 4
     bnel fail_gpr_test_interword
          
     cmplw 24, 4
     bnel fail_gpr_test_interword
          
     cmplw 25, 4
     bnel fail_gpr_test_interword
          
     cmplw 26, 4
     bnel fail_gpr_test_interword
          
     cmplw 27, 4
     bnel fail_gpr_test_interword
          
     cmplw 28, 4
     bnel fail_gpr_test_interword
          
     cmplw 29, 4
     bnel fail_gpr_test_interword
          
     cmplw 30, 4
     bnel fail_gpr_test_interword
          
     cmplw 31, 4
     bnel fail_gpr_test_interword
 .endm
     
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:     GPR_interword_test
#  Description:   
#       test General purpose register inter-word faults using March method
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
 .globl GPRInterWordTest
 
GPRInterWordTest:
    PrologStackOp
    StackCheckBegin
    
    PushParam (3)           #push parameter into stack
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
    
    march0
    march1
    march2
    march3
    march4
    march5
    
    mfspr 1, SPRG4
    
    TestConclusion
    b end_GPRInterWordTest
    
fail_gpr_test_interword:
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
