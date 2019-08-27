#include "../inc/common_macro.s"
#include "../inc/common_def.h"

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      ImageSwitchRTS0ToRTS1
#  Description:    
#
#  Author:
#  Used module:    None
#  Input Param:
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl ImageSwitchRTS0ToRTS1
 
ImageSwitchRTS0ToRTS1:
        
//sp
    lis 1, RTS1_RTS_STACKI_BOTTOMx@h
    ori 1, 1, RTS1_RTS_STACKI_BOTTOMx@l

//pc
    lis 2, 0x50FF010@h
    ori 2, 2, 0x50FF010@l
    mtlr 2
    blr



 .globl ImageSwitchRTS1ToRTS0

ImageSwitchRTS1ToRTS0:

//sp
    lis 1, RTS0_RTS_STACKI_BOTTOMx@h
    ori 1, 1, RTS0_RTS_STACKI_BOTTOMx@l

//pc
    lis 2, 0x0FF010@h
    ori 2, 2, 0x0FF010@l
    mtlr 2
    blr


 .end


