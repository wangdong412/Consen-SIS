#define    FloatExcptMode0       0xfffff6ff
#define    FloatExcptMode1       0x00000100
#define    FloatExcptMode2       0x00000800
#define    FloatExcptMode3       0x00000900
#*************************************************************************************************
#                                      Module Name: Boarddiag_s
#  Func Name:      SetFloatExcptMode
#  Description: 
#       set float-point exception mode,
#       ------------------------------
#      |Mode|    function             |
#       ------------------------------
#      |  0 |exception disabled       |
#       ------------------------------
#      |  1 |imprecise nonrecoverable |
#       ------------------------------
#      |  2 |imprecise recoverable    |
#       ------------------------------
#      |  3 |precise mode             |
#       ------------------------------
#  Author:         
#  Used module:    None
#  Input Param:    
#       Mode:
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define SetFloatExcptMode(Mode) \
    mfmsr 3 ;\
    lis FloatExcptMode0@h ;\
    ori 4, 4, FloatExcptMode0@l ;\
    and 3, 3, 4 ;\
    .if Mode == 1 ;\
    ori 3, 3, FloatExcptMode1 ;\
    .elseif Mode == 2 ;\
    ori 3, 3, FloatExcptMode2 ;\
    .elseif Mode == 3 ;\
    ori 3, 3, FloatExcptMode3 ;\
    .endif ;\
    mtmsr 3 ;


#*************************************************************************************************
#                                      Module Name: Boarddiag_s
#  Func Name:      EnableFloatException
#  Description: 
#       
#  Author:         
#  Used module:    None
#  Input Param:    
#       
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl EnableFloatException
EnableFloatException:
    mtfsb1 24           #使能浮点型指令无效异常
    mtfsb1 25           #使能浮点型操作结果上溢异常
    mtfsb1 26           #使能浮点型操作结果下溢异常
    mtfsb1 27           #使能除零异常

#    SetFloatExcptMode(1)
    blr
#*************************************************************************************************
#                                      Module Name: Boarddiag_s
#  Func Name:      DisableFloatException
#  Description: 
#       
#  Author:         
#  Used module:    None
#  Input Param:    
#       
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl DisableFloatException
DisableFloatException:
    mtfsb0 24           #使能浮点型指令无效异常
    mtfsb0 25           #使能浮点型操作结果上溢异常
    mtfsb0 26           #使能浮点型操作结果下溢异常
    mtfsb0 27           #使能除零异常
    mtfsb0 1            #使能全局异常
#    SetFloatExcptMode(0)
    blr
#*************************************************************************************************
#                                      Module Name: Boarddiag_s
#  Func Name:      ResetFloatExcpt
#  Description:清除Fpscr异常状态
#       
#  Author:         
#  Used module:    None
#  Input Param:    
#       
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl ResetFloatExcpt
ResetFloatExcpt:
#    mtfsfi 0,0x0            #清除各异常位
    mtfsb0 3
    mtfsb0 4
    mtfsb0 5
    mtfsb0 6
    mtfsb0 7
    mtfsb0 8
    mtfsb0 9
    mtfsb0 10
    mtfsb0 11
    mtfsb0 12
    mtfsb0 13
    mtfsb0 14
    mtfsb0 21
    mtfsb0 22
    mtfsb0 23
    mtfsb0 0
    blr
#*************************************************************************************************
#                                      Module Name: Boarddiag_s
#  Func Name:      RelocateStackPointer
#  Description: 
#       relocate stack bottom position
#  Author:         
#  Used module:    None
#  Input Param:    
#       r3:offset from current stack
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl RelocateStackPointer
RelocateStackPointer:
    mr 1,3                      #add offset to stack bottom
    blr

#*************************************************************************************************
#                                      Module Name: Boarddiag_s
#  Func Name:      BoardDiagJumpEntry
#  Description: 
#       create a stack to store the used registers and recover when TestAbsoluteJumpAsm call is finished
#  Author:         
#  Used module:    None
#  Input Param:    
#       r24-r31:used register
#        
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl BoardDiagJumpEntry                #global function
BoardDiagJumpEntry:
#push parameter
    stwu    1,-92(1)    #create stack
    mflr    0
    stmw    13,16(1)    #store nonvolatile register
    stw     0,96(1)
####set stack check environment 
    lis     12,0xaa55
    ori     12,12,0xaa55
    stw     12,8(1)
####end
#############################################
    bl TestAbsoluteJumpAsm            #call jump function
#############################################
#pop parameter
    xor     26,26,26                #set r26 = 0
    bl        SysGetCurExeIndex        #get current execute index    
    cmpwi    0,3,2                    #when jump from index2 to index 0 ,it needs be special handled
    bne        M1
    subi    3,3,2
    subis    26,26,0x0080            #set index
    b        M2
M1:    addis    26,26,0x0040
    addi    3,3,1                    #set index
M2:    
    bl        SysSetCurExeIndex        #update execute index
#####################update stack pointer##########################
    lwz        12,0(1)   
    add        12,12,26             #update
    stw        12,0(1)

    lwz     12,0(1)  
    lwz     13,0(12)              #get stack pointer of boarddiagcyclicity
    add        13,13,26             #update
    stw        13,0(12)

    lwz     14,0(12)
    lwz     13,0(14)
    add        13,13,26
    stw        13,0(14)
######################update lr register##########################

############get main function lr position################
    lwz     15,4(14)              #get main function lr value
    add        15,15,26                #update lr
    stw        15,4(14)                #write back
    lwz     15,4(12)                #recover lr(boarddiagcyclicity when stack size is 32)
    add        15,15,26                #update lr
    stw        15,4(12)                #write back
    mtlr    15
    
####check stack environment
    xor     3, 3, 3              #set init return value
    
    lis     4, 0xaa55
    ori     4, 4,0xaa55
    lwz     5, 8(1)               #get check code
    cmpd     4, 5 #rzxlszy
    beq     Check_OK
    lis     3, 0x00aa
    ori     3, 3, 0xaaaa  
Check_OK:   
####

####################end############################
    lmw     13,16(1)                #recover r13-r31
    lwz     1,0(1)
    blr
#*************************************************************************************************
#                                      Module Name: Boarddiag_s
#  Func Name:      TestAbsoluteJumpAsm
#  Description: 
#       get current lr and stack pointer and call BoardDiagRelocateStart function
#  Author:         
#  Used module:    None
#  Input Param:    
#       r3:address of s_tDiagCtrlInfo
#        r4:lr value
#        r5:stack bottom address
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl TestAbsoluteJumpAsm
TestAbsoluteJumpAsm:
    mr 5,1                #get last stack top pointer
    mflr 4                #get last lr value

    b  BoardDiagRelocateStart
#*************************************************************************************************
#                                      Module Name: Boarddiag_s
#  Func Name:      GetCallFunction
#  Description: 
#       get current lr
#  Author:         
#  Used module:    None
#  Input Param:    
#        
#  Output Param:    
#       r3:lr value
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

    .globl  GetCallFunction
GetCallFunction:
    lwz 3,0(1)
    lwz 3,4(3)
    blr 

