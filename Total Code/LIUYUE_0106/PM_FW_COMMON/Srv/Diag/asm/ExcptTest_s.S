#define  DDR_MASK                                  (0x00080000)
#define  DDR_ECC_DATA_INJECT_HI            (0x20002e00)
#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      ExcptTestPrtctEnv
#  Description:    
#      save current registers value.
#  Author:
#  Used module:    None
#  Input Param:
#      void
#  Output Param:
#      void
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/    
    .globl       ExcptTestPrtctEnv
ExcptTestPrtctEnv:
    stwu    1,-32(1)                #create stack
    stw     5, 28(1)                #save r5
    stw     4, 24(1)                #save r4
    stw     3, 20(1)                #save r3
    mflr    4
    stw     4, 36(1)                #save lr in stack
   
    bl      ExcptTestGetCtrlInfo
    mr      5,3
    addi    5,5,4                 #加上幻数偏移
    lwz     4,36(1)
    stw     4,0(5)                #save lr,pointer to next instruction of lr to skip iec call function
    mfxer   4
    stw     4,4(5)                #save xer
    mr      4,1
    addi    4,4,32
    stw     4,8(5)                #save stack position
    mfcr    4
    stw     4,12(5)                #save cr
    mfctr   4
    stw     4,16(5)                #save ctr
    
    mflr    4

    stw     4,20(5)                #save pc
    mfmsr   4
    stw     4,24(5)                #save msr
    lwz     4,20(1)
    stmw    0,28(5)                #save r0-r31
    mr      4,1
    addi    4,4,32
    stw     4,32(5)                #update save r1
    lwz     4,28(1)
    stw     4,48(5)                #update save r5
    lwz     4,24(1)
    stw     4,44(5)                #update save r4
    lwz     4,20(1)
    stw     4,40(5)                #update save r3
    sync

    lwz     4, 24(1)              #recover r4,get function address
    lwz     3, 20(1)              #recover r3,get parameter
    mtctr   3
    mr      3,4
    bctrl

##################################################
    lwz     5, 28(1)                #recover r5
    lwz     4, 24(1)                #recover r4
    lwz     3, 20(1)                #recover r3
    lwz     4, 36(1)                #recover lr
    mtlr    4

    addi    1,1,32                #recover stack
    blr

#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      ExcptTestRcvrEnv
#  Description:    
#      recover current registers value.
#  Author:
#  Used module:    None
#  Input Param:
#      void
#  Output Param:
#      void
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/    
    .globl       ExcptTestRcvrEnv
ExcptTestRcvrEnv:
    stwu    1,-32(1)                #create stack
    stw     5, 24(1)                #save r5
    stw     4, 20(1)                #save r4
################recover gpr################

    bl      ExcptTestGetCtrlInfo
    mr      5,3
    addi    5,5,4                 #加上幻数偏移
    lwz     4,28(5)
    mr      0,4                    #recover r0
    lwz     4,36(5)
    mr      2,4                    #recover r2

    lmw     6,52(5)
################################################# 
    lwz     4,4(5)
    mtxer   4                        #recover xer
    lwz     4,12(5)
    mtcr    4                        #recover cr
    lwz     4,16(5)
    mtctr   4                        #recover ctr
    lwz     4,24(5)

    mtmsr   4                        #recover msr
    sync
    
    isync
    lwz     4,8(5)
    mr      1,4                    #recover stack pointer
##################print stackpos lr##############   
    lwz     4,0(5)
    mtlr    4                        #recover lr register
####################return############
    lwz     3,40(5)
    lwz     4,44(5)
    lwz     5,48(5)                #recover r5
#########
    blr                                #return

#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      ExcptTestEnMsrEEBit
#  Description:    
#      
#  Author:
#  Used module:    None
#  Input Param:
#      void
#  Output Param:
#      void
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/    
    .globl       ExcptTestEnMsrEEBit
ExcptTestEnMsrEEBit:
    xor 4,4,4                    #clear r4
    li 4,0x0001
    rlwnm    4,4,3,12,31            #transfer bit id to uint
    mfmsr 3
    or    3,3,4
    mtmsr    3
    xor 3,3,3
    isync
    blr

#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      ExcptTestClrMsrEEBit
#  Description:    
#      
#  Author:
#  Used module:    None
#  Input Param:
#      void
#  Output Param:
#      void
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/    
    .globl       ExcptTestClrMsrEEBit
ExcptTestClrMsrEEBit:
    xor 4,4,4                    #clear r4
    li 4,0x0001
    rlwnm    4,4,3,12,31            #transfer bit id to uint
    lis    5,0xffff
    ori    5,5,0xffff
    xor 5,5,4
    mfmsr 3
    and    3,3,5
    mtmsr    3
    xor 3,3,3
    isync
    blr
    
#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      ExcptTestSync
#  Description:    
#      :同步数据以及指令操作
#  Author:
#  Used module:    None
#  Input Param:
#      void
#  Output Param:
#      void
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/    
    .globl       ExcptTestSync
ExcptTestSync:
    sync
    isync
    blr

#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      ExcptTestDelay
#  Description:    
#      
#  Author:
#  Used module:    None
#  Input Param:
#      r3:延时指令周期数
#  Output Param:
#      void
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/    
    .globl       ExcptTestDelay
ExcptTestDelay:
Loop:  
    subic.    3,3,1       #判断延时周期计数值是否达到设定值
    bge     Loop
    blr
#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      GetSrr1
#  Description:    
#      
#  Author:
#  Used module:    None
#  Input Param:
#      void
#  Output Param:
#      r3:返回SRR1值
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/    
    .globl       GetSrr1
GetSrr1:
    mfspr 3, 27
    blr
#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      GetSprg
#  Description:    
#      
#  Author:
#  Used module:    None
#  Input Param:
#      r3:特殊功能寄存器编号
#  Output Param:
#      r3:返回相应寄存器值，参数错误返回-1
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/    
    .globl       GetSprg
GetSprg:
    cmpli 3,0              #根据输入索引编号获取SPRG0-7值
    beq   0f                #SPRG0
    cmpli 3,1
    beq   1f                #SPRG1
    cmpli 3,2
    beq   2f                #SPRG2
    cmpli 3,3
    beq   3f                #SPRG3
    cmpli 3,4
    beq   4f                #SPRG4
    cmpli 3,5
    beq   5f                #SPRG5
    cmpli 3,6
    beq   6f                #SPRG6
    cmpli 3,7
    beq   7f                #SPRG7
    xor   3,3,3
    addi  3,3,-1          #如果索引值错误，r3 = -1
    b     GetSprg_End
0:
    mfspr 3, 272
    b     GetSprg_End
1:
    mfspr 3, 273
    b     GetSprg_End
2:
    mfspr 3, 274
    b     GetSprg_End
3:
    mfspr 3, 275
    b     GetSprg_End
4:
    mfspr 3, 276
    b     GetSprg_End
5:
    mfspr 3, 277
    b     GetSprg_End
6:
    mfspr 3, 278
    b     GetSprg_End
7:
    mfspr 3, 279
GetSprg_End:                #返回
    blr
#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      SetSprg
#  Description:    
#      
#  Author:
#  Used module:    None
#  Input Param:
#      r3:特殊功能寄存器编号0-7
#      r4:写入值
#  Output Param:
#      r3:正确返回编号，错误返回-1
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/    
    .globl       SetSprg
SetSprg:
    cmpli 3,0              #根据输入索引编号设置SPRG0-7
    beq   0f                #SPRG0
    cmpli 3,1
    beq   1f                #SPRG0
    cmpli 3,2
    beq   2f                #SPRG0
    cmpli 3,3
    beq   3f                #SPRG0
    cmpli 3,4
    beq   4f                #SPRG0
    cmpli 3,5
    beq   5f                #SPRG0
    cmpli 3,6
    beq   6f                #SPRG0
    cmpli 3,7
    beq   7f                #SPRG0
    xor   3,3,3          #如果索引值错误，r3 = -1
    addi  3,3,-1
    b     SetSprg_End
0:
    mtspr 272, 4
    b     SetSprg_End
1:
    mtspr 273, 4
    b     SetSprg_End
2:
    mtspr 274, 4
    b     SetSprg_End
3:
    mtspr 275, 4
    b     SetSprg_End
4:
    mtspr 276, 4
    b     SetSprg_End
5:
    mtspr 277, 4
    b     SetSprg_End
6:
    mtspr 278, 4
    b     SetSprg_End
7:
    mtspr 279, 4
SetSprg_End:                #返回
    blr
#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      GetFpscr
#  Description:    
#       
#  Author:
#  Used module:    None
#  Input Param:
#      void
#  Output Param:
#      r3:FPSCR寄存器值
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/    
    .globl       GetFpscr
GetFpscr:
    stwu    1,-24(1)
    mflr    3
    stw     3,28(1)
    stfd    f0,8(1)
    mffs    f0             #拷贝FPSCR至f0
    stfd    f0,16(1)
    lfd     f0,8(1)
    lwz     3,20(1)      #取FPSCR至r3
    addi    1,1,24
    lwz     4,4(1)
    mtlr    4
    blr
#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      ECCGen
#  Description:    
#      
#  Author:
#  Used module:    None
#  Input Param:
#      void
#  Output Param:
#      void
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
    .globl  ECCGen
ECCGen:
CleanSBEG:
    xor 5,5,5
    lis 4,0x2000
    ori 4,4,0x2e58
    lis 3,0x2000
    ori 3,3,0x2e08         #load inject ADDRESS
    lwz 6,0(3)             #load inject value

    ori 6,6,0x0110
    stw 6,0(3)             #set inject
    sync 
    isync
    
    lis 3 , 0x00ff
    ori 3, 3, 0xc000
    xor 5,5,5
    lis 6,0x0
    ori 6,6,0x0100
WLOOPG:
    stwx 3,3,5
    addi 5,5,4
    cmp 6,5
    bgt WLOOPG
    xor 5,5,5
LOOPG: 
    lwzx 4,5,3
    addi 5,5,4
    cmp 6,5
    bgt LOOPG
    
    xor 5,5,5
    lis 3,0x2000
    ori 3,3,0x2e08         #load inject ADDRESS
    stw 5,0(3)             #clear inject value
    sync 
    isync

    xor 7,7,7
    lis 3 , 0x00ff
    ori 3, 3, 0xc000
WLOOPGR:
    stwx 7,3,5
    addi 5,5,4
    cmp 6,5
    bgt WLOOPGR
    blr

#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      EccSingleErrGen
#  Description:    
#      
#  Author:
#  Used module:    None
#  Input Param:
#      r3   #测试地址
#      r4   ECC单位错误产生bit位置范围(0-31)
#  Output Param:
#      void
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
    .globl  EccSingleErrGen
EccSingleErrGen:
    cmpi 4,31              #判断输入参数
    bng Enter
    li   4,31
Enter:
    lis 5,0x2000           #r5指向DATA_INJECT_HI
    ori 5,5,0x2e00
#    lis r6,0x2000           #r6指向DATA_INJECT_LO
#    ori r6,r6,0x2e04
    
    lis 7,0x2000           #r7指向ERR_SBE
    ori 7,7,0x2e58
    
    li 8,1
    rlwnm  8,8,4,0,31    #设置ECC错误bit错误掩码
    stw 8,4(5)            #写入DATA_INJECT_LO
    sync
    lwz 8,0(7)            #读取 err_sbe寄存器值
    rlwinm 8,8,0,0,23
    stw r8,0(7)            #清除 err_sbe寄存器值
    sync
    lwz r8,8(5)
    ori r8,r8,0x0100        #使能错误注入 
    stw r8,8(5)
    sync

    lwz 8,0(7)            #读取 err_sbe寄存器值
    rlwinm 8,8,0,0,23
    mulli 8,8,2            #设置循环次数
     
    xor 4,4,4           #r4作为计数值变量
1:
    stwx 3,3,4
    addi 4,4,4
    cmp 8,4
    bgt 1b
    xor 4,4,4
2: 
    lwzx 6,4,3
    addi 4,4,4
    cmp 8,4
    bgt 2b
    xor 4,4,4
    
    xor 6,6,6            #清除错误注入掩码
    stw 6,4(5)
    lwz 6,8(5)
    xori 6,6,0x0100        #清除错误注入
    stw 6,8(5)
    sync 
    
    xor 6,6,6
3:
    stwx 6,3,4
    addi 4,4,4
    cmp 8,4
    bgt 3b
    blr
#*************************************************************************************************
#                                      模块名称: ExcptTest_s
#函数名:      ForTESTECCFunc
#函数描述:
#测试ECC纠错功能
#作者:
#调用模块    None
#输入参数:   R3 结果记录变量地址
#结果记录变量
#输出参数:
#无
#版本信息:        V1.0
#修订信息:
#标注:        None
#---------------------------------------------------------*/ 
    .globl  ForTESTECCFunc
ForTESTECCFunc:
#R3传入结果记录变量地址

    li r8,0
    stw r8,0(r3)             #清除标志位

    lis r7,0x00ff            #装载测试地址
    ori r7,r7,0xc200 

    lis r8,0x2000
    ori r8,r8,0x2E00         #装载DATA注入掩码地址
    xor r5,r5,r5             
    ori r5,r5,0x0001         
    stw r5,0(r8)             #设置DATA注入掩码

    xor r5,r5,r5
    lis r8,0x2000
    ori r8,r8,0x2e08         #装载DATA注入使能地址
    lwz r6,0(r8)             
    ori r6,r6,0x0100
    stw r6,0(r8)             #使能注入
    sync
    isync

    lis r4,0xaaff
    ori r4,r4,0xffaa
    stw r4,0(r7)             #对测试地址写入0xaaffffaa
    sync 
    isync

    xor r5,r5,r5
    lis r8,0x2000
    ori r8,r8,0x2e08         #装载DATA注入使能地址
    stw r5,0(r8)             #禁止注入
    sync 
    isync

    lis r8,0x2000
    ori r8,r8,0x2E00         #装载DATA注入掩码地址
    xor r6,r6,r6             
    stw r6,0(r8)             #清除DATA注入掩码
    sync

    lwz r8,0(r7)             #从测试地址读取值与预设值比较
    stw r8,0(r7)             #自修复

    lis r4,0xaaff
    ori r4,r4,0xffaa
    cmp 0,r8,r4              #对比数据检查单位错修复
    bne ECCFunc_L
    lis r8,0x0010            #设置掩码MASK_ECC
    ori r8,r8,0
    stw r8,0(r3)             #写入标志位
ECCFunc_L:
    li  r4,0
    stw r4,0(r7)             #清除测试地址内数据，刷新ECC纠错码
    sync
    blr
#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      ForTESTECCECC_D
#  Description:    
#      双位ECC错误自检
#  Author:
#  Used module:    None
#  Input Param:
#      r3:测试地址
#  Output Param:
#      none
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
    .globl  ForTESTECCECC_D
ForTESTECCECC_D: 

    lis r5,0x2000           #r5指向DATA_INJECT_HI
    ori r5,r5,0x2e00
   
    lis r7,0x2000           #r7指向ERR_SBE
    ori r7,r7,0x2e58
    
    lis r8,0
    ori r8,r8,0x0003
    stw r8,4(r5)            #设置错误注入寄存器
    sync
 .if 0
    lis r8,0
    ori r8,r8,0x0003
    stw r8,0(r5)            #设置错误注入寄存器
    sync
 .endif   
    lwz r8,8(r5)
    ori r8,r8,0x0100        #使能错误注入 
    stw r8,8(r5)
    sync
#######for test############
    xor   r4,r4,r4
    lis   r4,0x0102
    ori   r4,r4,0x0304
    mtspr 274, r4
###########################
    stw r8,0(r3)
    isync 

#######for test############
    xor   r4,r4,r4
    lis   r4,0x0203
    ori   r4,r4,0x0405
    mtspr 274, r4
###########################
    lwz r6,8(r5)
    xori r6,r6,0x0100        #清除错误注入 
    stw r6,8(r5)              
    isync 
 
    xor r6,r6,r6            #清除错误注入掩码
    stw r6,4(r5)
    isync
#######for test############
    xor   r4,r4,r4
    lis   r4,0x0304
    ori   r4,r4,0x0506
    mtspr 274, r4
###########################    
    stw r8,0(r3)
    sync 
    
    xor r6,r6,r6
    blr

#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      MachineExcptGen
#  Description:    
#      
#  Author:
#  Used module:    None
#  Input Param:
#      #
#  Output Param:
#      void
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
    .globl  MachineExcptGen
MachineExcptGen: 
    lis  r4,0xE600      #通过对实际不存在地址0xE6000F00写入值产生machine错误
    addi r4,r4,0x0F00
    lis  r5,0x013F
    addi r5,r5,0xf000
    stw  r5,4(r4)
    blr

#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      AlignExcptGen
#  Description:    
#      #对齐异常产生函数，lmw指令必须四字节对齐
#  Author:
#  Used module:    None
#  Input Param:
#      r3       #测试地址
#  Output Param:
#      #无
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
    .globl  AlignExcptGen
AlignExcptGen:
    lmw r5,0(r3)
    blr

#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      ProgramExcptGen
#  Description:    
#           #Program异常产生函数
#  Author:
#  Used module:    None
#  Input Param:
#      r3       #自检指令保存地址
#  Output Param:
#      void
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
    .globl  ProgramExcptGen
ProgramExcptGen:
    mr  r4,r3
    lis r3, 0x3863       #/* 指令 addi 3, 3, 0x0001 */
    ori r3, r3, 0x0001
    stw r3, 0(r4)
    lis r3, 0x2063       #/* 指令 subfic 3, 3, 0x0001 */
    ori r3, r3, 0x0001
    stw r3, 4(r4)
    bl $+4
    mflr r3
    addi r3,r3, 20
    mtspr 279, r3
    mtlr r4
    blrl

#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      IsiExcptGen
#  Description:    
#           #Isi异常产生函数
#  Author:
#  Used module:    None
#  Input Param:
#      r3       #自检指令保存地址
#  Output Param:
#      void
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
    .globl  IsiExcptGen
IsiExcptGen:
    lis    r3,0xec00
    ori    r3,r3,0x0010
    lwz r4,0(r3)
    blr

#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      FloatXXExcptGen
#  Description:    
#           #浮点型Xx异常产生函数，通过设置异常标志位产生
#  Author:
#  Used module:    None
#  Input Param:
#      #无
#  Output Param:
#      #无
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
    .globl  FloatXxExcptGen
FloatXxExcptGen:
    mtfsb1 6
    blr
        
#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      FloatVxsnanExcptGen
#  Description:    
#           #浮点型Vxsnan异常产生函数，通过设置异常标志位产生
#  Author:
#  Used module:    None
#  Input Param:
#      #无
#  Output Param:
#      #无
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
    .globl  FloatVxsnanExcptGen
FloatVxsnanExcptGen:
    mtfsb1 7
    blr

#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      FloatVxisiExcptGen
#  Description:    
#           #浮点型Vxisi异常产生函数，通过设置异常标志位产生
#  Author:
#  Used module:    None
#  Input Param:
#      #无
#  Output Param:
#      #无
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
    .globl  FloatVxisiExcptGen
FloatVxisiExcptGen:
    mtfsb1 8
    blr    

#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      FloatVxidiExcptGen
#  Description:    
#           #浮点型Vxidi异常产生函数，通过设置异常标志位产生
#  Author:
#  Used module:    None
#  Input Param:
#      #无
#  Output Param:
#      #无
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
    .globl  FloatVxidiExcptGen
FloatVxidiExcptGen:
    mtfsb1 9
    blr   

#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      FloatVximzExcptGen
#  Description:    
#           #浮点型Vximz异常产生函数，通过设置异常标志位产生
#  Author:
#  Used module:    None
#  Input Param:
#      #无
#  Output Param:
#      #无
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
    .globl  FloatVximzExcptGen
FloatVximzExcptGen:
    mtfsb1 11
    blr  

#*************************************************************************************************
#                                      Module Name: ExcptTest_s
#  Func Name:      FloatVxvcExcptGen
#  Description:    
#           #浮点型Vxvc异常产生函数，通过设置异常标志位产生
#  Author:
#  Used module:    None
#  Input Param:
#      #无
#  Output Param:
#      #无
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/ 
    .globl  FloatVxvcExcptGen
FloatVxvcExcptGen:
    mtfsb1 12
    blr 

#*************************************************************************************************
#                                      模块名称: ExcptTest_s
#函数名:      CheckEccInject
#函数描述:
#
#作者:
#调用模块    None
#输入参数:   R3
#结果记录变量
#输出参数:
#无
#版本信息:        V1.0
#修订信息:
#标注:        None
#---------------------------------------------------------*/ 
    .globl  CheckEccInject
CheckEccInject:
#####设置MSR_ME位
    li r3,1
    rlwinm r3,r3,12,19,19
    mfmsr r4
    or  r4,r4,r3
    mtmsr r4
#####检查注入寄存器
    xor r3,r3,r3
    lis r4,0x2000
    ori r4,r4,0x2e00
    lwz r5,8(r4)
    cmp r5,r3
    beq End_Check
    mr  r6,r5
    stw r3,8(r4)
    sync 
    isync
    stw r3,0(r4)
    stw r3,4(r4)
    mr  r3,r6
End_Check:  
    blr 
