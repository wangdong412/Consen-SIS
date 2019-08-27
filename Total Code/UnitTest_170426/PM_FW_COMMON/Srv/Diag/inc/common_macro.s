
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      PrologStackOp
#  Description:     
#      创建堆栈，更新堆栈指针以及LR寄存器值。
#      
#  Author:           
#  Used module:    None
#  Input Param:	   None
#      GPR1:Current stack pointer
#	   GPR31:Current stack pointer starting from the parameter location
#  Output Param:   None
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define PrologStackOp \
    stwu 1, -DEFAULT_STACK_SIZE(1) ;\
    stw  0, +OFFSET_STORE_TEMP_GPR0(1) ;\
    mflr 0 ;\
    stw  0, +OFFSET_SAVE_LR(1) ;\
    stw  31, +OFFSET_STORE_TEMP_GPR31(1) ;\
    mr   31, 1


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      StackCheckBegin
#  Description:    
#      It carries out starting operations to check the formal parameters saved in the stack.
#      
#  Author:           
#  Used module:    None
#  Input Param:
#      GPR1:Current stack pointer
#	   GPR31:Current stack pointer starting from the parameter location
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define StackCheckBegin \
    addi 31, 31, OFFSET_STACK_PARAMETERS ;\
    lis 0,0 ;\
    stw 0, OFFSET_STORE_PAR(1) ;\
    stw 30, +OFFSET_STORE_TEMP_GPR30(1)



#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      StackCheckEnd
#  Description:    
#      It carries out final operations to check the formal parameters saved in the stack.
#      
#  Author:           
#  Used module:    None
#  Input Param:
#      GPR1:Current stack pointer
#	   GPR31:Current stack pointer starting from the parameter location
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define StackCheckEnd \
    lwz 30, OFFSET_STORE_TEMP_GPR30(1)


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      StoreCtrXerCr2Stack
#  Description:    
#      It carries out the operations to save registers CTR, XER, CR.
#      
#  Author:           
#  Used module:    None
#  Input Param:
#      CTR:register CTR
#	   XER:register XER
#	   CR :register CR
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define StoreCtrXerCr2Stack \
    mfctr 0 ;\
    PushReg(0) ;\
    mfspr 0, XER ;\
    PushReg(0) ;\
    mfcr 0 ;\
    PushReg(0) 

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      StoreFpscr2Stack
#  Description:    
#      保存FPSCR寄存器值
#      
#  Author:         Chen Ke
#  Used module:    None
#  Input Param:
#      FPSCR
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define StoreFpscr2Stack \
    mffs f0 ;\
    addi r31, r1, OFFSET_STORE_FPSCR ;\
    stfd f0, 0(r31) 


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      StoreGPRs2Stack
#  Description:    
#      It carries out the operations to save registers GPR0 - GPR31 in the stack.
#      
#  Author:           
#  Used module:    None
#  Input Param:
#      GPR1:Current stack pointer
#	   GPR0-31:Registers to be saved in the stack
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define StoreGPRs2Stack \
    addi 31, 1, OFFSET_STORE_REG ;\
    lwz 0, OFFSET_STORE_TEMP_GPR0(1) ;\
    stmw 0, 0(31) ;\
    addi 31, 31, DIM_GPR_MENO_4 ;\
    stw   30, +OFFSET_STORE_TEMP_GPR30(1) ;\
    mr 30, 31 ;\
    lwz 31, OFFSET_STORE_TEMP_GPR31(1) ;\
    stw 31, 0(30) ;\
    addi 30, 30, 4 ;\
    mr 31, 30 ;\
    lwz 30, OFFSET_STORE_TEMP_GPR30(1) 


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      StoreFPRs2Stack
#  Description:    
#      保存浮点型寄存器至堆栈中。
#      
#  Author:         Chen Ke
#  Used module:    None
#  Input Param:
#      GPR1:栈顶指针
#      GPR31:当前栈指针
#	   FPR0-31:浮点型寄存器
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define StoreFPRs2Stack \
    addi 31, 1, OFFSET_STORE_FPR ;\
    stfd f0, 0(31) ;\
    stfd f1, 8(31) ;\
    stfd f2, 16(31) ;\
    stfd f3, 24(31) ;\
    stfd f4, 32(31) ;\
    stfd f5, 40(31) ;\
    stfd f6, 48(31) ;\
    stfd f7, 56(31) ;\
    stfd f8, 64(31) ;\
    stfd f9, 72(31) ;\
    stfd f10, 80(31) ;\
    stfd f11, 88(31) ;\
    stfd f12, 96(31)  ;\
    stfd f13, 104(31) ;\
    stfd f14, 112(31) ;\
    stfd f15, 120(31) ;\
    stfd f16, 128(31) ;\
    stfd f17, 136(31) ;\
    stfd f18, 144(31) ;\
    stfd f19, 152(31) ;\
    stfd f20, 160(31) ;\
    stfd f21, 168(31) ;\
    stfd f22, 176(31) ;\
    stfd f23, 184(31) ;\
    stfd f24, 192(31) ;\
    stfd f25, 200(31) ;\
    stfd f26, 208(31) ;\
    stfd f27, 216(31) ;\
    stfd f28, 224(31) ;\
    stfd f29, 232(31) ;\
    stfd f30, 240(31) ;\
    stfd f31, 248(31) ;\
    addi 31, 31, DIM_FPR


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      RestoreFPRs
#  Description:    
#       
#      
#  Author:         Chen Ke
#  Used module:    None
#  Input Param:
#      GPR1:栈顶指针
#      GPR31:当前栈指针
#	   FPR0-31:浮点型寄存器
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define RestoreFPRs \
    addi 31, 1, OFFSET_STORE_FPR;\
    lfd f0, 0(31);\
    lfd f1, 8(31);\
    lfd f2, 16(31);\
    lfd f3, 24(31);\
    lfd f4, 32(31);\
    lfd f5, 40(31);\
    lfd f6, 48(31);\
    lfd f7, 56(31);\
    lfd f8, 64(31);\
    lfd f9, 72(31);\
    lfd f10, 80(31);\
    lfd f11, 88(31);\
    lfd f12, 96(31);\
    lfd f13, 104(31);\
    lfd f14, 112(31);\
    lfd f15, 120(31);\
    lfd f16, 128(31);\
    lfd f17, 136(31);\
    lfd f18, 144(31);\
    lfd f19, 152(31);\
    lfd f20, 160(31);\
    lfd f21, 168(31);\
    lfd f22, 176(31);\
    lfd f23, 184(31);\
    lfd f24, 192(31);\
    lfd f25, 200(31);\
    lfd f26, 208(31);\
    lfd f27, 216(31);\
    lfd f28, 224(31);\
    lfd f29, 232(31);\
    lfd f30, 240(31);\
    lfd f31, 248(31)


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      TestCPUStart
#  Description:    
#      It carries out the starting operations of the routine assembler.
#      
#  Author:           
#  Used module:    None
#  Input Param:
#      GPR1:Current stack pointer
#	   GPR31:Current stack pointer starting from the parameter location
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define TestCPUStart \
    mr 31, 1 ;\
    addi 31, 31, OFFSET_STACK_PARAMETERS ;\
    lwz 3, OFFSET_PUNT_STACK_CORR(1)               ;\
    mr 4, 1                                        ;\
    lwz 5, +4(31)                                  ;\
    lwz 6, +OFFSET_STORE_PAR(1)                    ;\
    mr 31, 1                                        ;\
    addi 31, 31, OFFSET_STACK_PARAMETERS            ;\
    lwz 3, +4(31) ;\
    mr 31, 1 ;\
    addi 31, 31, OFFSET_STACK_PARAMETERS ;\
    lwz 3, +0(31) ;\
    lwz 4, +4(31) ;\
    bl ResetTestBit


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      TestConclusion
#  Description:    
#      It carries out the operations requested in case of check without errors.
#      
#  Author:           
#  Used module:    None
#  Input Param:
#      GPR3:Check vector
#	   GPR4:Mask (OR) for check vector
#	   GPR31:Current pointer to stack where the register shall be read
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define TestConclusion \
    mr 31, 1 ;\
    addi 31, 31, OFFSET_STACK_PARAMETERS ;\
    lwz 3, +0(31) ;\
    lwz 4, +4(31) ;\
    lwz 30, 0(3) ;\
    or 30, 30, 4 ;\
    stw 30, 0(3) 


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      StackEpilogBegin
#  Description:    
#      It carries out the starting operations for the stack epilog
#      
#  Author:           
#  Used module:    None
#  Input Param:
#      GPR1:current stack pointer
#	   LR:link register
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define StackEpilogBegin \
    lwz 0, +OFFSET_SAVE_LR(1) ;\
    mtlr 0 ;\
    lwz 31, OFFSET_PUNT_STACK_CORR(1) 


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      RestoreCtrXerCr
#  Description:    
#      recover cr, xer and ctr
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
#s---------------------------------------------------------*/

#define RestoreCtrXerCr \
    PopReg(0) ;\
    mtcr 0 ;\
    PopReg(0) ;\
    mtspr XER, 0 ;\
    PopReg(0) ;\
    mtctr 0


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      RestoreFpscr
#  Description:    
#      recover fpscr
#      
#  Author:         chen ke
#  Used module:    None
#  Input Param:
#      
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#s---------------------------------------------------------*/

#define RestoreFpscr             \
    addi r31, r1, OFFSET_STORE_FPSCR ;\
    lfd   f0, 0(r31) ;\
    mtfsf 0xff, f0  

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      StackEpilogEnd
#  Description:    
#      Epilog linked with the stack management,recover r0, r2 and delete stack.
#      
#  Author:           
#  Used module:    None
#  Input Param:
#      GPR1:current stack pointer
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define StackEpilogEnd \
    lwz 0, +OFFSET_STORE_REG_GPR0(1) ;\
    lmw 2, +OFFSET_STORE_REG_GPR2_31(1) ;\
    addi 1, 1, DEFAULT_STACK_SIZE



#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      StackEpilogEndOfFpr
#  Description:    
#      Epilog linked with the stack management,recover r0, r2 and delete stack.
#      
#  Author:           
#  Used module:    None
#  Input Param:
#      GPR1:current stack pointer
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define StackEpilogEndOfFpr \
    lwz 0, +OFFSET_STORE_REG_GPR0(1) \
    lmw 2, +OFFSET_STORE_REG_GPR2_31(1) \
    addi 1, 1, DEFAULT_STACK_SIZE


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      ErrorHandling
#  Description:    
#      It manages the special context saving in case of detected error.
#      
#  Author:           
#  Used module:    None
#  Input Param:
#      GPR3:Address of the memory area for the context saving
#	   GPR16-31:Registers for saving SPR registers
#  Output Param:    
#      None
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define ErrorHandling \
    stmw 16, ADDRESS_SAVE_ERROR_GPR16_GPR31(1)  ;\
    stw 3, ADDRESS_SAVE_ERROR_GPR3(1)                  ;\
    lis 3,CPU_ERR_REGS_STORE_ADDR ;\
    stmw 0, 128(3) ;\
	mfsr 16, 0 ;\
	mfsr 17, 1 ;\
    mfsr 18, 1 ;\
    mfsr 18, 2 ;\
    mfsr 19, 3 ;\
    mfsr 20, 4 ;\
    mfsr 21, 5 ;\
    mfsr 22, 6 ;\
    mfsr 23, 7 ;\
    mfsr 24, 8 ;\
    mfsr 25, 9 ;\
    mfsr 26, 10 ;\
    mfsr 27, 11 ;\
    mfsr 28, 12 ;\
    mfsr 29, 13 ;\
    mfsr 30, 14 ;\
    mfsr 31, 15 ;\
    stmw 16, 64(3) ;\
    mflr 16 ;\
    subi 16, 16, 8 ;\
    mfcr 17 ;\
    mfxer 18 ;\
    mfctr 19 ;\
    mfmsr 20 ;\
    mfspr 21, HID1 ;\
    mfspr 22, HID0 ;\
    mfspr 23, PTEHI ;\
    mfspr 24, PTELO ;\
    mfspr 25, SDR1 ;\
    lis 26, 0x0000 ;\
    lis 27, 0x0000 ;\
    lis 28, 0x0000 ;\
    lis 29, 0x0000 ;\
    lis 30, 0x0000 ;\
    lis 31, 0x0000 ;\
    stmw 16, 0(3) ;\
    lwz 31, ADDRESS_SAVE_ERROR_GPR3(1) ;\
    stw 31, 140(3)                  ;\
    mr 31, 3 ;\
    addi 31, 31, 256 ;\
    mtspr SPRG7, 31 ;\
    lmw 16, ADDRESS_SAVE_ERROR_GPR16_GPR31(1) ;\
    lwz 3, ADDRESS_SAVE_ERROR_GPR3(1)


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      push_param
#  Description: 
#      It saves in the stack the formal parameters of the assembler function implementing the single test
#      
#  Author:           
#  Used module:    None
#  Input Param:
#      reg:General purpose register
#  Output Param:    
#      None
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define PushParam(reg) \
    stw reg, 0(31) ;\
    addi 31, 31, 4 ;\
    lwz 30, OFFSET_STORE_PAR(1) ;\
    addi 30, 30, 1 ;\
    stw 30, OFFSET_STORE_PAR(1)


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      PushReg
#  Description: 
#      It saves in the stack the CPU critical registers and modified by the test
#      
#  Author:           
#  Used module:    None
#  Input Param:
#      reg:General purpose register
#  Output Param:    
#      None
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define PushReg(reg) \
    stw reg, 0(31) ;   \
     addi 31, 31, 4 


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:      PopReg
#  Description: 
#      It manages the special context saving in case of detected error.
#      
#  Author:           
#  Used module:    None
#  Input Param:
#      reg:Register taken from the stack
#	   GPR31:Current pointer to the stack where the formal parameters shall be saved.
#  Output Param:    
#      None
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/

#define PopReg(reg) \
    subi 31, 31, 4 ;\
    lwz  reg, 0(31)

