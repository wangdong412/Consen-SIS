#include "../inc/common_macro.s"
#include "../inc/common_def.h"
 
#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:     mvpat
#  Description: 
#		It moves the patterns and positions then starting from R10 or R20
#  Author:
#  Used module:    None
#  Input Param:
#		position:1 moves registers R10-19 in R20-29  0 movers registers R20-29 in R10-19
#		GPR10-19:Input or output registers according to the position parameter value
#		GPR20-29:Input or output registers according to the position parameter value
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define mvpat(param) \
	.if param == 1 ;\
		mr 20, 10 ;\
		mr 21, 11 ;\
		mr 22, 12 ;\
		mr 23, 13 ;\
		mr 24, 14 ;\
		mr 25, 15 ;\
		mr 26, 16 ;\
		mr 27, 17 ;\
		mr 28, 18 ;\
		mr 29, 19 ;\
	.else  ;\
		mr 10, 20 ;\
		mr 11, 21 ;\
		mr 12, 22 ;\
		mr 13, 23 ;\
		mr 14, 24 ;\
		mr 15, 25 ;\
		mr 16, 26 ;\
		mr 17, 27 ;\
		mr 18, 28 ;\
		mr 19, 29 ;\
	.endif


#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:     PipelineFlush
#  Description: 
#		Introduction of a NOP number to empty the IU pipeline.
#  Author:
#  Used module:    None
#  Input Param:
#		num_istr:Number of instructions inserted in the pipeline
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
#define PipelineFlush(num_istr) \
 .if 0  ;\
	    mtctr 4  ;\
10:		nop ;\
		bdnz 10b ;\
  .endif	 ;\
 .if (num_istr > 0) ;\
        nop ;\
        nop ;\
        nop ;\
        nop ;\
        nop ;\
 .endif
  

#*************************************************************************************************
#                                      Module Name: self_test
#  Func Name:     IU1Test
#  Description:   
#       It carries out the logic and arithmetic operation test 
#		(multiplication and division excluded). 
#  Author:
#  Used module:    None
#  Input Param:
#		GPR3:Test outcome vector
#		GPR4:Mask(OR) to be applied to check vector in case of exit without errors
#		R0-R31:Registers where the test shall be made
#  Output Param:    
#
#  Version:        V1.0
#  Revision:
#  Remarks:        None
#---------------------------------------------------------*/
 .globl IU1Test

# .section init_section,"ax"

IU1Test:
	PrologStackOp
	StackCheckBegin

	PushParam(3)
	PushParam(4)

	StackCheckEnd
	StoreGPRs2Stack
	StoreCtrXerCr2Stack

	stw 31, OFFSET_PUNT_STACK_CORR(1)
	TestCPUStart
# load interconnect pattern into GPR
	lis 10, 0xAAAA
	ori 10, 10, 0xAAAA
	not 11, 10
	lis 12, 0xCCCC
	ori 12, 12, 0xCCCC
	not 13, 12
	lis 14, 0xF0F0
	ori 14, 14, 0xF0F0
	not 15, 14
	lis 16, 0xFF00
	ori 16, 16, 0xFF00
	not 17, 16
	lis 18, 0xFFFF
	not 19, 18
    
# .Section output_pattern_section, "ax"
	lis 8, 0xAAAA
	ori 8, 8, 0xAAA9
	mr 7, 8
	mr 6, 8

	PipelineFlush(4)
# test addi instruction
	addi 20, 6, 0x0001
	addi 21, 7, 0x0001
	addi 22, 8, 0x0001
	cmpw 20, 10
	bnel FailIU1Test
	cmpw 21, 10
	bnel FailIU1Test
	cmpw 22, 10
	bnel FailIU1Test

	lis 6, 0x5554
	ori 6, 6, 0x5555
	mr 7, 6
	mr 8, 7

	PipelineFlush(4)
# test addis instruction
	addis 20, 6, 0x0001
	addis 21, 7, 0x0001
	addis 22, 8, 0x0001
	cmpw 20, 11
	bnel FailIU1Test
	cmpw 21, 11
	bnel FailIU1Test
	cmpw 22, 11
	bnel FailIU1Test

	lis 7, 0xCCCC
	mr 6, 7
	mr 8, 6

	PipelineFlush(3)
# test ori instruction
	ori 20, 6, 0xCCCC
	ori 21, 7, 0xCCCC
	ori 22, 8, 0xCCCC
             
	cmpw 20, 12
	bnel FailIU1Test
	cmpw 21, 12
	bnel FailIU1Test
	cmpw 22, 12
	bnel FailIU1Test

	xor 8, 8, 8
	ori 8, 8, 0x3333
	mr 7, 8
	mr 6, 7

	PipelineFlush(4)
# test oris instruction
	oris 20, 6, 0x3333
	oris 21, 7, 0x3333
	oris 22, 8, 0x3333

	cmpw 20, 13
	bnel FailIU1Test
	cmpw 21, 13
	bnel FailIU1Test
	cmpw 22, 13
	bnel FailIU1Test

	lis 7, 0xF0F0
	ori 7, 7, 0xDDDD
	mr 8, 7
	mr 6, 8

	PipelineFlush(4)
# test xori instruction
	xori 20, 6, 0x2D2D
	xori 21, 7, 0x2D2D
	xori 22, 8, 0x2D2D

	cmpw 20, 14
	bnel FailIU1Test
	cmpw 21, 14
	bnel FailIU1Test
	cmpw 22, 14
	bnel FailIU1Test

	lis 6, 0xDDDD
	ori 6, 6, 0x0F0F
	mr 8, 6
	mr 7, 8

	PipelineFlush(4)
# test xoris instruction
	xoris 20, 6, 0xD2D2
	xoris 21, 7, 0xD2D2
	xoris 22, 8, 0xD2D2
	cmpw 20, 15
	bnel FailIU1Test
	cmpw 21, 15
	bnel FailIU1Test
	cmpw 22, 15
	bnel FailIU1Test

	lis 6, 0xFF00
	ori 6, 6, 0xFF10
	mr 7, 6
	mr 8, 7
	li 9, 0x0010

	PipelineFlush(5)
# test subf instruction
	subf 20, 9, 6
	subf 21, 9, 7
	subf 22, 9, 8
	cmpw 20, 16
	bnel FailIU1Test
	cmpw 21, 16
	bnel FailIU1Test
	cmpw 22, 16
	bnel FailIU1Test

	lis 8, 0x00FF
	ori 8, 8, 0x01FF
	mr 7, 8
	mr 6, 8
	li 9, 0x0100

	PipelineFlush(5)
#test subf. instruction
	subf. 20, 9, 6
	bngl FailIU1Test
	subf. 21, 9, 7
	bngl FailIU1Test
	subf. 22, 9, 8
	bngl FailIU1Test
	cmpw 20, 17
	bnel FailIU1Test
	cmpw 21, 17
	bnel FailIU1Test
	cmpw 22, 17
	bnel FailIU1Test

	lis 7, 0xFFFF
	ori 7, 7, 0xFFFF
	mr 6, 7
	mr 8, 7
# test andis. instruction             
	PipelineFlush(4)
	andis. 20, 6, 0xFFFF
	bnll FailIU1Test
	andis. 21, 7, 0xFFFF
	bnll FailIU1Test
	andis. 22, 8, 0xFFFF
	bnll FailIU1Test

	cmpw 20, 18
	bnel FailIU1Test
	cmpw 21, 18
	bnel FailIU1Test
	cmpw 22, 18
	bnel FailIU1Test

	lis 6, 0xFFFF
	ori 6, 6, 0xFFFF
	mr 7, 6
	mr 8, 6
	PipelineFlush(4)
	andi. 20, 6, 0xFFFF
	bngl FailIU1Test
	andi. 21, 6, 0xFFFF
	bngl FailIU1Test
	andi. 22, 6, 0xFFFF
	bngl FailIU1Test

	cmpw 20, 19
	bnel FailIU1Test
	cmpw 21, 19
	bnel FailIU1Test
	cmpw 22, 19
	bnel FailIU1Test

# .section input_pattern_section, "ax"
	mvpat(1)

	lis 6, 0x5555
	ori 6, 6, 0x5556

	PipelineFlush(2)

	neg 10, 20
	neg 11, 20
	neg 12, 20
	cmpw 6, 10
	bnel FailIU1Test
	cmpw 6, 11
	bnel FailIU1Test
	cmpw 6, 12
	bnel FailIU1Test

	lis 7, 0x2222
	ori 7, 7, 0x2221
 
	PipelineFlush(2)

	add 10, 21, 22
	add 11, 21, 22
	add 12, 21, 22
    cmpw 10, 7
    bnel FailIU1Test
    cmpw 11, 7
    bnel FailIU1Test
    cmpw 12, 7
    bnel FailIU1Test

    lis 9, 0xCCCC
    ori 9, 9, 0xCCCD

    PipelineFlush(2)

    neg. 10, 23
    bnll FailIU1Test
    neg. 11, 23
    bnll FailIU1Test
    neg. 12, 23
    bnll FailIU1Test
    cmpw 10, 9
    bnel FailIU1Test
    cmpw 11, 9
    bnel FailIU1Test
    cmpw 12, 9
    bnel FailIU1Test

    xor 8, 8, 8
    PipelineFlush(1)
    and 10, 24, 25
    and 11, 24, 25
    and 12, 24, 25
    cmpw 10, 8
    bnel FailIU1Test
    cmpw 11, 8
    bnel FailIU1Test
    cmpw 12, 8
    bnel FailIU1Test

    lis 7, 0x00FF
    ori 7, 7, 0x01FF

    PipelineFlush(2)

    subfic 10, 26, 0x00FF
    mcrxr 1
    beql 1, FailIU1Test
    subfic 11, 26, 0x00FF
    mcrxr 2
    beql 2, FailIU1Test
    subfic 12, 26, 0x00FF
    mcrxr 3
    beql 3, FailIU1Test
    cmpw 10, 7
    bnel FailIU1Test
    cmpw 11, 7
    bnel FailIU1Test
    cmpw 12, 7
    bnel FailIU1Test

    lis 9, 0x00FF
    ori 9, 9, 0xFF00

    PipelineFlush(2)
# test eqv instruction
    eqv 10, 27, 28
    eqv 11, 27, 28
    eqv 12, 27, 28
    cmpw 10, 9
    bnel FailIU1Test
    cmpw 11, 9
    bnel FailIU1Test
    cmpw 12, 9
    bnel FailIU1Test

    lis 8, 0xFFFF
    ori 8, 8, 0x0001

    PipelineFlush(2)

    nego. 10, 29
    bnll FailIU1Test
    mcrxr 1
    bsol 1, FailIU1Test
    nego. 11, 29
    bnll FailIU1Test
    mcrxr 2
    bsol 2, FailIU1Test
    nego. 12, 29
    bnll FailIU1Test
    mcrxr 3
    bsol 3, FailIU1Test
    cmpw 10, 8
    bnel FailIU1Test
    cmpw 11, 8
    bnel FailIU1Test
    cmpw 12, 8
    bnel FailIU1Test

# .section extra_instruction_section, "ax"
    xor 5, 5, 5
    li 8, 0x0020

    PipelineFlush(2)

    cntlzw 10, 5
    cntlzw 11, 5
    cntlzw 12, 5
    cmpw 10, 8
    bnel FailIU1Test
    cmpw 11, 8
    bnel FailIU1Test
    cmpw 12, 8
    bnel FailIU1Test

    li 5, 0x0079
    lis 6, 0x0000
    ori 6, 6, 0x0079

    PipelineFlush(3)

    extsb 10, 5
    extsb 11, 5
    extsb 12, 5
    cmpw 10, 6
    bnel FailIU1Test
    cmpw 11, 6
    bnel FailIU1Test
    cmpw 12, 6
    bnel FailIU1Test

    li 5, 0x7999
    lis 6, 0x0000
    ori 6, 6, 0x7999

    PipelineFlush(3)

    extsh 10, 5
    extsh 11, 5
    extsh 12, 5
    cmpw 10, 6
    bnel FailIU1Test
    cmpw 11, 6
    bnel FailIU1Test
    cmpw 12, 6
    bnel FailIU1Test

    xor 5, 5, 5
    lis 7, 0xFFFF
    ori 7, 7, 0xFFFF

    PipelineFlush(3)

    nand 10, 5, 5
    nand 11, 5, 5
    nand 12, 5, 5
    cmpw 10, 7
    bnel FailIU1Test
    cmpw 11, 7
    bnel FailIU1Test
    cmpw 12, 7
    bnel FailIU1Test


    lis 8, 0xFFFF
    ori 8, 8, 0xFFFF
    xor 6, 6, 6

    PipelineFlush(3)

    nor 10, 8, 8
    nor 11, 8, 8
    nor 12, 8, 8
    cmpw 10, 6
    bnel FailIU1Test
    cmpw 11, 6
    bnel FailIU1Test
    cmpw 12, 6
    bnel FailIU1Test

    lis 9, 0x6666
    ori 9, 9, 0x9999
    lis 6, 0xFFFF
    ori 6, 6, 0xFFFF

    PipelineFlush(4)

    orc 10, 9, 9
    orc 11, 9, 9
    orc 12, 9, 9
    cmpw 10, 6
    bnel FailIU1Test
    cmpw 11, 6
    bnel FailIU1Test
    cmpw 12, 6
    bnel FailIU1Test

    lis 5, 0xAAAA
    ori 5, 5, 0xAAAA
    mr 10, 5
    mr 11, 5
    mr 12, 5
    lis 6, 0xD555
    ori 6, 6, 0x5555
    
    PipelineFlush(7)
    rlwimi 10, 5, 1, 1, 31
    rlwimi 11, 5, 1, 1, 31
    rlwimi 12, 5, 1, 1, 31
    cmpw 10, 6
    bnel FailIU1Test
    cmpw 11, 6
    bnel FailIU1Test
    cmpw 12, 6
    bnel FailIU1Test

    lis 5, 0x8000
    li 8, 0x0001
    
    PipelineFlush(2)
    rlwinm 10, 5, 1, 0, 31
    rlwinm 11, 5, 1, 0, 31
    rlwinm 12, 5, 1, 0, 31
    cmpw 10, 8
    bnel FailIU1Test
    cmpw 11, 8
    bnel FailIU1Test
    cmpw 12, 8
    bnel FailIU1Test

    li 6, 0x0001
    lis 9, 0x8000
    li 5, 0x001F
    PipelineFlush(3)
    rlwnm 10, 6, 5, 0, 31
    rlwnm 11, 6, 5, 0, 31
    rlwnm 12, 6, 5, 0, 31
    cmpw 10, 9
    bnel FailIU1Test
    cmpw 11, 9
    bnel FailIU1Test
    cmpw 12, 9
    bnel FailIU1Test

    lis 9, 0xFFFF
    ori 9, 9, 0xFFFF
    li 5, 0x0020
    xor 8, 8, 8
    PipelineFlush(4)
    slw 10, 9, 5
    slw 11, 9, 5
    slw 12, 9, 5
    cmpw 10, 8
    bnel FailIU1Test
    cmpw 11, 8
    bnel FailIU1Test
    cmpw 12, 8
    bnel FailIU1Test

    lis 7, 0xFFFF
    ori 7, 7, 0xFFFF
    li 9, 0x0020
    xor 6, 6, 6
    PipelineFlush(4)
    srw 10, 7, 9
    srw 11, 7, 9
    srw 12, 7, 9
    cmpw 10, 6
    bnel FailIU1Test
    cmpw 11, 6
    bnel FailIU1Test
    cmpw 12, 6
    bnel FailIU1Test

# .section xer_verification_section, "ax"
    xor 5, 5, 5
    mtcr 5
    lis 5, 0x8000
    lis 6, 0x2C2C
    ori 6, 6, 0x2C00
    PipelineFlush(4)
    nego 10, 5
    mcrxr 1
    nego 11, 5
    mcrxr 3
    nego 12, 5
    mcrxr 5
    cmpw 10, 5
    cmpw 2, 11, 5
    cmpw 4, 12, 5
    mfcr 0
    cmpw 0, 6
    bnel FailIU1Test

    xor 5, 5, 5
    mtcr 5
    lis 7, 0x7FFF
    ori 7, 7, 0xFFFF
    li 6, 0x0002
    lis 8, 0x8000
    ori 8, 8, 0x0001
    PipelineFlush(6)
    addo 10, 6, 7
    mcrxr 1
    addo 11, 6, 7
    mcrxr 3
    addo 12, 6, 7
    mcrxr 5
    cmpw 10, 8
    cmpw 2, 11, 8
    cmpw 4, 12, 8
    lis 20, 0x2C2C
    ori 20, 20, 0x2C00
    mfcr 5
    cmpw 5, 20
    bnel FailIU1Test

    xor 9, 9, 9
    mtcr 9
    lis 8, 0x7FFF
    ori 8, 8, 0xFFFF
    lis 6, 0x8000
    PipelineFlush(4)
    addic 10, 8, 0x0001
    mcrxr 1
    addic 11, 8, 0x0001
    mcrxr 3
    addic 12, 8, 0x0001
    mcrxr 5
    cmpw 10, 6
    cmpw 2, 11, 6
    cmpw 4, 12, 6
    lis 7, 0x2020
    ori 7, 7, 0x2000
    mfcr 9
    cmpw 9, 7
    bnel FailIU1Test

    xor 29, 29, 29
    mtcr 29
    lis 28, 0x7FFF
    ori 28, 28, 0xFFFF
    lis 26, 0x8000
    li 25, 0x0001
    PipelineFlush(5)
    addc 20, 28, 25
    mcrxr 1
    addc 21, 28, 25
    mcrxr 3
    addc 22, 28, 25
    mcrxr 5
    cmpw 20, 26
    cmpw 2, 21, 26
    cmpw 4, 22, 26
    lis 27, 0x2020
    ori 27, 27, 0x2000
    mfcr 29
    cmpw 29, 27
    bnel FailIU1Test

    lis 6, 0xFFFF
    ori 6, 6, 0xFFFF
    lis 8, 0x8000
    lis 9, 0x7FFF
    ori 9, 9, 0xFFFF
    PipelineFlush(5)
    andc 10, 6, 8
    andc 11, 6, 8
    andc 12, 6, 8
    cmpw 10, 9
    bnel FailIU1Test
    cmpw 11, 9
    bnel FailIU1Test
    cmpw 12, 9
    bnel FailIU1Test

    xor 29, 29, 29
    mtcr 29
    lis 28, 0x7FFF
    ori 28, 28, 0xFFFF
    lis 26, 0x8000
    li 25, 0x0001
    PipelineFlush(5)
    addco 20, 28, 25
    mcrxr 1
    addco 21, 28, 25
    mcrxr 3
    addco 22, 28, 25
    mcrxr 5
    cmpw 20, 26
    cmpw 2, 21, 26
    cmpw 4, 22, 26
    lis 27, 0x2C2C
    ori 27, 27, 0x2C00
    mfcr 29
    cmpw 29, 27
    bnel FailIU1Test

    xor 28, 28, 28
    lis 29, 0x2000
    mtspr XER, 29
    li 25, 0x0001
    PipelineFlush(3)
    adde 20, 28, 28
    mtspr XER, 29
    adde 21, 28, 28
    mtspr XER, 29
    adde 22, 28, 28
    cmpw 20, 25
    bnel FailIU1Test
    cmpw 21, 25
    bnel FailIU1Test
    cmpw 22, 25
    bnel FailIU1Test

    xor 30, 30, 30
    mtcr 30
    lis 25, 0x2000
    mtspr XER, 25
    PipelineFlush(2)
    lis 29, 0x7FFF
    ori 29, 29, 0xFFFF
    lis 27, 0xFFFF
    ori 27, 27, 0xFFFF
    addeo 20, 29, 29
    mcrxr 1
    mtspr XER, 25
    addeo 21, 29, 29
    mcrxr 3
    mtspr XER, 25
    addeo 22, 29, 29
    mcrxr 5
    cmpw 20, 27
    cmpw 2, 21, 27
    cmpw 4, 22, 27
    lis 25, 0x2C2C
    ori 25, 25, 0x2C00
    mfcr 30
    cmpw 30, 25
    bnel FailIU1Test

    xor 28, 28, 28
    mtspr XER, 28
    lis 24, 0xFFFF
    ori 24, 24, 0xFFFF
    PipelineFlush(3)
    addme 20, 28
    mtspr XER, 28
    addme 21, 28
    mtspr XER, 28
    addme 22, 28
    cmpw 20, 24
    bnel FailIU1Test
    cmpw 21, 24
    bnel FailIU1Test
    cmpw 22, 24
    bnel FailIU1Test

    lis 26, 0x2000
    mtspr XER, 26
    xor 23, 23, 23
    xor 24, 24, 24
    PipelineFlush(3)
    addmeo 20, 23
    mcrxr 1
    bnel 1, FailIU1Test
    mtspr XER, 26
    addmeo 21, 23
    mcrxr 3
    bnel 3, FailIU1Test
    mtspr XER, 26
    addmeo 22, 23
    mcrxr 5
    bnel 5, FailIU1Test
    cmpw 20, 24
    bnel FailIU1Test
    cmpw 21, 24
    bnel FailIU1Test
    cmpw 21, 24
    bnel FailIU1Test

    xor 25, 25, 25
    lis 24, 0x2000
    mtspr XER, 24
    PipelineFlush(2)
    addze 20, 25
    mtspr XER, 24
    addze 21, 25
    mtspr XER, 24
    addze 22, 25
    cmpwi 20, 0x0001
    bnel FailIU1Test
    cmpwi 21, 0x0001
    bnel FailIU1Test
    cmpwi 22, 0x0001
    bnel FailIU1Test

    lis 25, 0x7FFF
    ori 25, 25, 0xFFFF
    lis 24, 0x2000
    PipelineFlush(3)
    mtspr XER, 24
    lis 27, 0x8000
    addzeo 20, 25
    mcrxr 1
    mtspr XER, 24
    addzeo 21, 25
    mcrxr 3
    mtspr XER, 24
    addzeo 22, 25
    mcrxr 5
    cmpw 20, 27
    cmpw 2, 21, 27
    cmpw 4, 22, 27
    lis 29, 0x2C2C
    ori 29, 29, 0x2C00
    mfcr 27
    cmpw 29, 27
    bnel FailIU1Test

    li 24, 0x0001
    PipelineFlush(1)
    subfc 20, 24, 24
    mcrxr 1
    bnel 1, FailIU1Test
    subfc 21, 24, 24
    mcrxr 3
    bnel 3, FailIU1Test
    subfc 22, 24, 24
    mcrxr 5
    bnel 5, FailIU1Test
    cmpwi 20, 0x0000
    bnel FailIU1Test
    cmpwi 21, 0x0000
    bnel FailIU1Test
    cmpwi 22, 0x0000
    bnel FailIU1Test

    lis 24, 0x8000
    lis 25, 0x7FFF
    ori 25, 25, 0xFFFF
    xor 28, 28, 28
    PipelineFlush(4)
    mtcr 28
    subfco 20, 25, 24
    mcrxr 1
    subfco 21, 25, 24
    mcrxr 3
    subfco 22, 25, 24
    mcrxr 5
    cmpwi 20, 0x0001
    cmpwi 2, 21, 0x0001
    cmpwi 4, 22, 0x0001
    lis 26, 0x2E2E
    ori 26, 26, 0x2E00
    mfcr 28
    cmpw 28, 26
    bnel FailIU1Test
     
    lis 25, 0x2000
    mtspr XER, 25
    li 24, 0x0001
    PipelineFlush(2)
    subfe 20, 24, 24
    mcrxr 1
    bnel 1, FailIU1Test
    mtspr XER, 25
    subfe 21, 24, 24
    mcrxr 3
    bnel 3, FailIU1Test
    mtspr XER, 25
    subfe 22, 24, 24
    mcrxr 5
    bnel 5, FailIU1Test
    cmpwi 20, 0x0000
    bnel FailIU1Test
    cmpwi 21, 0x0000
    bnel FailIU1Test
    cmpwi 22, 0x0000
    bnel FailIU1Test
    
    lis 24, 0x8000
    lis 25, 0x7FFF
    ori 25, 25, 0xFFFF
    xor 28, 28, 28
    mtcr 28
    lis 26, 0x2000
    PipelineFlush(5)
    mtspr XER, 26
    subfeo 20, 25, 24
    mcrxr 1
    mtspr XER, 26
    subfeo 21, 25, 24
    mcrxr 3
    mtspr XER, 26
    subfeo 22, 25, 24
    mcrxr 5
    cmpwi 20, 0x0001
    cmpwi 2, 21, 0x0001
    cmpwi 4, 22, 0x0001
    lis 26, 0x2E2E
    ori 26, 26, 0x2E00
    mfcr 28
    cmpw 28, 26
    bnel FailIU1Test
     
    xor 24, 24, 24
    mtcr 24 
    lis 27, 0xFFFF
    ori 27, 27, 0xFFFF
    lis 28, 0x2000
    PipelineFlush(4)
    mtspr XER, 28
    subfme 20, 24
    mcrxr 1
    mtspr XER, 28
    subfme 21, 24
    mcrxr 3
    mtspr XER, 28
    subfme 22, 24
    mcrxr 5
    cmpw 20, 27
    cmpw 2, 21, 27
    cmpw 4, 22, 27
    lis 27, 0x2222
    ori 27, 27, 0x2200
    mfcr 28
    cmpw 28, 27
    bnel FailIU1Test
     
    xor 24, 24, 24
    mtcr 24
    lis 27, 0x7FFF
    ori 27, 27, 0xFFFE
    lis 25, 0x8000
    xor 28, 28, 28
    PipelineFlush(5)
    mtspr XER, 28
    subfmeo 20, 25
    mcrxr 1
    mtspr XER, 28
    subfmeo 21, 25
    mcrxr 3
    mtspr XER, 28
    subfmeo 22, 25
    mcrxr 5
    cmpw 20, 27
    cmpw 2, 21, 27
    cmpw 4, 22, 27
    lis 27, 0x2222
    ori 27, 27, 0x2200
    mfcr 28
    cmpw 28, 27
    bnel FailIU1Test
     
    lis 26, 0x7FFF
    ori 26, 26, 0xFFFF
    li 24, 0x0001
    lis 25, 0x8000
    ori 25, 25, 0x0002
    lis 28, 0x2020
    ori 28, 28, 0x2000
    xor 27, 27, 27
    PipelineFlush(8)
    mtcr 27
    subfo 20, 26, 24
    mcrxr 1
    subfo 21, 26, 24
    mcrxr 3
    subfo 22, 26, 24
    mcrxr 5
    cmpw 20, 25
    cmpw 2, 21, 25
    cmpw 4, 22, 25
    mfcr 25
    cmpw 25, 28
    bnel FailIU1Test
     
    lis 24, 0x7FFF
    ori 24, 24, 0xFFFF
    xor 28, 28, 28
    mtcr 28
    lis 28, 0x8000
    ori 28, 28, 0x0001
    lis 25, 0x2000
    lis 27, 0x2020
    ori 27, 27, 0x2000
    PipelineFlush(8)
    mtspr XER, 25
    subfze 20, 24
    mcrxr 1
    mtspr XER, 25
    subfze 21, 24
    mcrxr 3
    mtspr XER, 25
    subfze 22, 24
    mcrxr 5
    mtspr XER, 25
    cmpw 20, 28
    cmpw 2, 21, 28
    cmpw 4, 22, 28
    mfcr 28
    cmpw 28, 27
    bnel FailIU1Test
     
    lis 24, 0x7FFF
    ori 24, 24, 0xFFFF
    xor 28, 28, 28
    mtcr 28
    lis 28, 0x8000
    ori 28, 28, 0x0001
    lis 25, 0x2000
    lis 27, 0x2020
    ori 27, 27, 0x2000
    PipelineFlush(8)
    mtspr XER, 25
    subfzeo 20, 24
    mcrxr 1
    mtspr XER, 25
    subfzeo 21, 24
    mcrxr 3
    mtspr XER, 25
    subfzeo 22, 24
    mcrxr 5
    cmpw 20, 28
    cmpw 2, 21, 28
    cmpw 4, 22, 28
    mfcr 28
    cmpw 28, 27
    bnel FailIU1Test
     
    lis 26, 0x8000
    xor 29, 29, 29
    lis 24, 0x2000
    PipelineFlush(3)
    mtspr XER, 24
    sraw 20, 26, 29
    mcrxr 1
    beql 1, FailIU1Test
    mtspr XER, 24
    sraw 21, 26, 29
    mcrxr 3
    beql 3, FailIU1Test
    mtspr XER, 24
    sraw 22, 26, 29
    mcrxr 5
    beql 5, FailIU1Test
    cmpw 20, 26
    bnel FailIU1Test
    cmpw 21, 26
    bnel FailIU1Test
    cmpw 22, 26
    bnel FailIU1Test
     
    lis 26, 0x8000
    lis 24, 0x2000
    PipelineFlush(2)
    mtspr XER, 24
    srawi 20, 26, 0
    mcrxr 1
    beql 1, FailIU1Test
    mtspr XER, 24
    srawi 21, 26, 0
    mcrxr 3
    beql 3, FailIU1Test
    mtspr XER, 24
    srawi 22, 26, 0
    mcrxr 5
    beql 5, FailIU1Test
    cmpw 20, 26
    bnel FailIU1Test
    cmpw 21, 26
    bnel FailIU1Test
    cmpw 22, 26
    bnel FailIU1Test
     
# .section cr_verification_section, "ax"
    lis 7, 0x7FFF
    ori 7, 7, 0xFFFF
    li 6, 0x0002
    lis 8, 0x8000
    ori 8, 8, 0x0001
    PipelineFlush(5)
    add. 10, 6, 7
    bsol FailIU1Test
    add. 11, 6, 7
    bsol FailIU1Test
    add. 12, 6, 7
    bsol FailIU1Test
    cmpw 10, 8
    bnel FailIU1Test
    cmpw 11, 8
    bnel FailIU1Test
    cmpw 12, 8
    bnel FailIU1Test
     
    lis 5, 0xFFFF
    ori 5, 5, 0xFFFF
    lis 8, 0x7FFF
    ori 8, 8, 0xFFFF
    PipelineFlush(4)
    and. 10, 8, 5
    bltl FailIU1Test
    and. 11, 8, 5
    bltl FailIU1Test
    and. 12, 8, 5
    bltl FailIU1Test
    cmpw 10, 8
    bnel FailIU1Test
    cmpw 11, 8
    bnel FailIU1Test
    cmpw 12, 8
    bnel FailIU1Test
     
    lis 6, 0xFFFF
    ori 6, 6, 0xFFFF
    lis 8, 0x8000
    lis 9, 0x7FFF
    ori 9, 9, 0xFFFF
    PipelineFlush(5)
    andc. 10, 6, 8
    bngl FailIU1Test
    andc. 11, 6, 8   
    bngl FailIU1Test
    andc. 12, 6, 8
    bngl FailIU1Test
    cmpw 10, 9
    bnel FailIU1Test
    cmpw 11, 9
    bnel FailIU1Test
    cmpw 12, 9
    bnel FailIU1Test
     
    lis 5, 0xFFFF
    ori 5, 5, 0xFFFF
    xor 8, 8, 8
    PipelineFlush(3)
    cntlzw. 10, 5
    bltl FailIU1Test
    cntlzw. 11, 5
    bltl FailIU1Test
    cntlzw. 12, 5
    bltl FailIU1Test
    cmpw 10, 8
    bnel FailIU1Test
    cmpw 11, 8
    bnel FailIU1Test
    cmpw 12, 8
    bnel FailIU1Test
     
    li 5, 0x0010
    li 6, 0x0002
    lis 9, 0xFFFF
    ori 9, 9, 0xFFED
    PipelineFlush(4)
    eqv. 10, 5, 6
    bnll FailIU1Test
    eqv. 11, 5, 6
    bnll FailIU1Test
    eqv. 12, 5, 6
    bnll FailIU1Test
    cmpw 10, 9
    bnel FailIU1Test
    cmpw 11, 9
    bnel FailIU1Test
    cmpw 12, 9
    bnel FailIU1Test
     
    li 5, 0x00AA
    lis 6, 0xFFFF
    ori 6, 6, 0xFFAA
    PipelineFlush(3)
    extsb. 10, 5
    bnll FailIU1Test
    extsb. 11, 5
    bnll FailIU1Test
    extsb. 12, 5
    bnll FailIU1Test
    cmpw 10, 6
    bnel FailIU1Test
    cmpw 11, 6
    bnel FailIU1Test
    cmpw 12, 6
    bnel FailIU1Test
     
    xor 5, 5, 5
    ori 5, 5, 0xAAAA
    lis 6, 0xFFFF
    ori 6, 6, 0xAAAA
    PipelineFlush(4)
    extsh. 10, 5
    bnll FailIU1Test
    extsh. 11, 5
    bnll FailIU1Test
    extsh. 12, 5
    bnll FailIU1Test
    cmpw 10, 6
    bnel FailIU1Test
    cmpw 11, 6
    bnel FailIU1Test
    cmpw 12, 6
    bnel FailIU1Test
     
    xor 7, 7, 7
    lis 5, 0xFFFF
    ori 5, 5, 0xFFFF
    PipelineFlush(3)
    nand. 10, 7, 7
    beql FailIU1Test
    nand. 11, 7, 7
    beql FailIU1Test
    nand. 12, 7, 7
    beql FailIU1Test
    cmpw 10, 5
    bnel FailIU1Test
    cmpw 11, 5
    bnel FailIU1Test
    cmpw 12, 5
    bnel FailIU1Test
     
    lis 8, 0xFFFF
    ori 8, 8, 0xFFFF
    xor 6, 6, 6
    PipelineFlush(3)
    nor. 10, 6, 6
    bnll FailIU1Test
    nor. 11, 6, 6
    bnll FailIU1Test
    nor. 12, 6, 6
    bnll FailIU1Test
    cmpw 10, 8
    bnel FailIU1Test
    cmpw 11, 8
    bnel FailIU1Test
    cmpw 12, 8
    bnel FailIU1Test
     
    xor 7, 7, 7
    lis 9, 0x8000
    PipelineFlush(2)
    or. 10, 7, 9
    bgtl FailIU1Test
    or. 11, 7, 9
    bgtl FailIU1Test
    or. 12, 7, 9
    bgtl FailIU1Test
    cmpw 10, 9
    bnel FailIU1Test
    cmpw 11, 9
    bnel FailIU1Test
    cmpw 12, 9
    bnel FailIU1Test
     
    lis 9, 0x6666
    ori 9, 9, 0x9999
    lis 6, 0xFFFF
    ori 6, 6, 0xFFFF
    PipelineFlush(4)
    orc. 10, 9, 9
    bnll FailIU1Test
    orc. 11, 9, 9
    bnll FailIU1Test
    orc. 12, 9, 9
    bnll FailIU1Test
    cmpw 10, 6
    bnel FailIU1Test
    cmpw 11, 6
    bnel FailIU1Test
    cmpw 12, 6
    bnel FailIU1Test
     
    lis 5, 0xAAAA
    ori 5, 5, 0xAAAA
    mr 10, 5
    mr 11, 5
    mr 12, 5
    lis 6, 0xD555
    ori 6, 6, 0x5555
    PipelineFlush(7)
    rlwimi. 10, 5, 1, 1, 31
    bnll FailIU1Test
    rlwimi. 11, 5, 1, 1, 31
    bnll FailIU1Test
    rlwimi. 12, 5, 1, 1, 31
    bnll FailIU1Test
    cmpw 10, 6
    bnel FailIU1Test
    cmpw 11, 6 
    bnel FailIU1Test
    cmpw 12, 6
    bnel FailIU1Test
     
    lis 5, 0x8000
    li 8, 0x0001
    PipelineFlush(2)
    rlwinm. 10, 8, 31, 0, 31
    bgtl FailIU1Test
    rlwinm. 11, 8, 31, 0, 31
    bgtl FailIU1Test
    rlwinm. 12, 8, 31, 0, 31
    bgtl FailIU1Test
    cmpw 10, 5
    bnel FailIU1Test
    cmpw 11, 5
    bnel FailIU1Test
    cmpw 12, 5
    bnel FailIU1Test
     
    li 6, 0x0001
    lis 9, 0x8000
    li 5, 0x0001
    PipelineFlush(3)
    rlwnm. 10, 9, 6, 0, 31
    bngl FailIU1Test
    rlwnm. 11, 9, 6, 0, 31
    bngl FailIU1Test
    rlwnm. 12, 9, 6, 0, 31
    bngl FailIU1Test
    cmpw 10, 5
    bnel FailIU1Test
    cmpw 11, 5
    bnel FailIU1Test
    cmpw 12, 5
    bnel FailIU1Test
     
    lis 9, 0xFFFF
    ori 9, 9, 0xFFFF
    li 5, 0x0020
    xor 8, 8, 8
    PipelineFlush(4)
    slw. 10, 9, 5
    bnel FailIU1Test
    slw. 11, 9, 5
    bnel FailIU1Test
    slw. 12, 9, 5
    bnel FailIU1Test
    cmpw 10, 8
    bnel FailIU1Test
    cmpw 11, 8
    bnel FailIU1Test
    cmpw 12, 8
    bnel FailIU1Test
     
    lis 7, 0xFFFF
    ori 7, 7, 0xFFFF
    li 9, 0x0020
    xor 6, 6, 6
    PipelineFlush(4)
    srw. 10, 7, 9
    bnel FailIU1Test
    srw. 11, 7, 9
    bnel FailIU1Test
    srw. 12, 7, 9
    bnel FailIU1Test
    cmpw 10, 6
    bnel FailIU1Test
    cmpw 11, 6
    bnel FailIU1Test
    cmpw 12, 6
    bnel FailIU1Test
     
    lis 9, 0x7777
    ori 9, 9, 0x7777
    lis 5, 0xFFFF
    ori 5, 5, 0xFFFF
    lis 8, 0x8888
    ori 8, 8, 0x8888
    PipelineFlush(6)
    xor. 10, 9, 5
    bnll FailIU1Test
    xor. 11, 9, 5
    bnll FailIU1Test
    xor. 12, 9, 5
    bnll FailIU1Test
    cmpw 10, 8
    bnel FailIU1Test
    cmpw 11, 8
    bnel FailIU1Test
    cmpw 12, 8
    bnel FailIU1Test

# .section cr_xer_verification_section, "ax"
     
    xor 5, 5, 5
    mtcr 5
    lis 7, 0x7FFF
    ori 7, 7, 0xFFFF
    li 6, 0x0002
    lis 8, 0x8000
    ori 8, 8, 0x0001
    PipelineFlush(6)
    addo. 10, 6, 7
    bnll FailIU1Test
    mcrxr 1
    addo. 11, 6, 7
    bnll FailIU1Test
    mcrxr 3
    addo. 12, 6, 7
    bnll FailIU1Test
    mcrxr 5
    cmpw 10, 8
    cmpw 2, 11, 8
    cmpw 4, 12, 8
    lis 20, 0x2C2C
    ori 20, 20, 0x2C00
    mfcr 5
    cmpw 5, 20
    bnel FailIU1Test

    xor 9, 9, 9
    mtcr 9
    lis 8, 0x7FFF
    ori 8, 8, 0xFFFF
    lis 6, 0x8000
    PipelineFlush(4)
    addic. 10, 8, 0x0001
    mcrxr 1
    addic. 11, 8, 0x0001
    mcrxr 3
    addic. 12, 8, 0x0001
    mcrxr 5
    cmpw 10, 6
    cmpw 2, 11, 6
    cmpw 4, 12, 6
    lis 7, 0x2020
    ori 7, 7, 0x2000
    mfcr 9
    cmpw 9, 7
    bnel FailIU1Test

    xor 9, 9, 9
    mtcr 9
    lis 8, 0x7FFF
    ori 8, 8, 0xFFFF
    lis 6, 0x8000
    li 5, 0x0001
    PipelineFlush(5)
    addc. 10, 8, 5
    bnll FailIU1Test
    mcrxr 1
    addc. 11, 8, 5
    bnll FailIU1Test
    mcrxr 3
    addc. 12, 8, 5
    bnll FailIU1Test
    mcrxr 5
    cmpw 10, 6
    cmpw 2, 11, 6
    cmpw 4, 12, 6
    lis 7, 0x2020
    ori 7, 7, 0x2000
    mfcr 9
    cmpw 9, 7
    bnel FailIU1Test

    xor 9, 9, 9
    mtcr 9
    lis 8, 0x7FFF
    ori 8, 8, 0xFFFF
    lis 6, 0x8000
    li 5, 0x0001
    PipelineFlush(5)
    addco. 10, 8, 5
    bnll FailIU1Test
    mcrxr 1
    addco. 11, 8, 5
    bnll FailIU1Test
    mcrxr 3
    addco. 12, 8, 5
    bnll FailIU1Test
    mcrxr 5
    cmpw 10, 6
    cmpw 2, 11, 6
    cmpw 4, 12, 6
    lis 7, 0x2C2C
    ori 7, 7, 0x2C00
    mfcr 9
    cmpw 9, 7
    bnel FailIU1Test

    xor 8, 8, 8
    lis 9, 0x2000
    mtspr XER, 9
    li 5, 0x0001 
    PipelineFlush(3)
    adde. 10, 8, 8
    bngl FailIU1Test
    mtspr XER, 9
    adde. 11, 8, 8
    bngl FailIU1Test
    mtspr XER, 9
    adde. 12, 8, 8
    bngl FailIU1Test
    cmpw 10, 5
    bnel FailIU1Test
    cmpw 11, 5
    bnel FailIU1Test
    cmpw 12, 5
    bnel FailIU1Test

    xor 30, 30, 30
    mtcr 30
    lis 5, 0x2000
    mtspr XER, 5
    lis 9, 0x7FFF
    ori 9, 9, 0xFFFF
    lis 7, 0xFFFF
    ori 7, 7, 0xFFFF
    PipelineFlush(6)
    addeo. 10, 9, 9
    bgtl FailIU1Test
    mcrxr 1
    mtspr XER, 5
    addeo. 11, 9, 9
    bgtl FailIU1Test
    mcrxr 3
    mtspr XER, 5
    addeo. 12, 9, 9
    bgtl FailIU1Test
    mcrxr 5
    cmpw 10, 7
    cmpw 2, 11, 7
    cmpw 4, 12, 7
    lis 5, 0x2C2C
    ori 5, 5, 0x2C00
    mfcr 30 
    cmpw 30, 5
    bnel FailIU1Test

    xor 28, 28, 28
    mtspr XER, 28
    lis 24, 0xFFFF
    ori 24, 24, 0xFFFF
    PipelineFlush(3)
    addme. 20, 28
    bnll FailIU1Test
    mtspr XER, 28
    addme. 21, 28
    bnll FailIU1Test
    mtspr XER, 28
    addme. 22, 28
    bnll FailIU1Test
    cmpw 20, 24
    bnel FailIU1Test
    cmpw 21, 24
    bnel FailIU1Test
    cmpw 22, 24
    bnel FailIU1Test

    lis 26, 0x2000
    mtspr XER, 26
    xor 23, 23, 23
    xor 24, 24, 24
    PipelineFlush(3)
    addmeo. 20, 23
    bnel FailIU1Test
    mcrxr 1
    bnel 1, FailIU1Test
    mtspr XER, 26
    addmeo. 21, 23
    bnel FailIU1Test
    mcrxr 3
    bnel 3, FailIU1Test
    mtspr XER, 26
    addmeo. 22, 23 
    bnel FailIU1Test
    mcrxr 5
    bnel 5, FailIU1Test
    cmpw 20, 24
    bnel FailIU1Test
    cmpw 21, 24
    bnel FailIU1Test
    cmpw 22, 24 
    bnel FailIU1Test

    xor 25, 25, 25
    lis 24, 0x2000
    PipelineFlush(2)
    mtspr XER, 24
    addze. 20, 25
    bngl FailIU1Test
    mtspr XER, 24
    addze. 21, 25
    bngl FailIU1Test
    mtspr XER, 24
    addze. 22, 25
    bngl FailIU1Test
    cmpwi 20, 0x0001
    bnel FailIU1Test
    cmpwi 21, 0x0001
    bnel FailIU1Test
    cmpwi 22, 0x0001
    bnel FailIU1Test

    lis 25, 0x7FFF
    ori 25, 25, 0xFFFF
    lis 24, 0x2000
    mtspr XER, 24
    lis 27, 0x8000
    PipelineFlush(4)
    addzeo. 20, 25
    bnll FailIU1Test
    mcrxr 1
    mtspr XER, 24
    addzeo. 21, 25
    bnll FailIU1Test
    mcrxr 3
    mtspr XER, 24
    addzeo. 22, 25
    bnll FailIU1Test
    mcrxr 5
    cmpw 20, 27
    cmpw 2, 21, 27
    cmpw 4, 22, 27
    lis 29, 0x2C2C
    ori 29, 29, 0x2C00
    mfcr 27
    cmpw 29, 27
    bnel FailIU1Test

    li 24, 0x0001
    PipelineFlush(1) 
    subfc. 20, 24, 24
    bnel FailIU1Test
    mcrxr 1
    bnel 1, FailIU1Test
    subfc. 21, 24, 24
    bnel FailIU1Test
    mcrxr 3
    bnel 3, FailIU1Test
    subfc. 22, 24, 24
    bnel FailIU1Test
    mcrxr 5
    bnel 5, FailIU1Test
    cmpwi 20, 0x0000
    bnel FailIU1Test
    cmpwi 21, 0x0000
    bnel FailIU1Test
    cmpwi 22, 0x0000
    bnel FailIU1Test

    lis 24, 0x8000
    lis 25, 0x7FFF
    ori 25, 25, 0xFFFF
    xor 28, 28, 28
    mtcr 28
    PipelineFlush(4)
    subfco. 20, 25, 24
    bngl FailIU1Test
    mcrxr 1
    subfco. 21, 25, 24
    bngl FailIU1Test
    mcrxr 3
    subfco. 22, 25, 24
    bngl FailIU1Test
    mcrxr 5
    cmpwi 20, 0x0001
    cmpwi 2, 21, 0x0001
    cmpwi 4, 22, 0x0001
    lis 26, 0x2E2E
    ori 26, 26, 0x2E00
    mfcr 28
    cmpw 28, 26
    bnel FailIU1Test

    lis 25, 0x2000
    mtspr XER, 25
    li 24, 0x0001
    PipelineFlush(2)
    subfe. 20, 24, 24
    bnel FailIU1Test
    mcrxr 1
    bnel 1, FailIU1Test
    mtspr XER, 25
    subfe. 21, 24, 24
    bnel FailIU1Test
    mcrxr 3
    bnel 3, FailIU1Test
    mtspr XER, 25
    subfe. 22, 24, 24
    bnel FailIU1Test
    mcrxr 5
    bnel 5, FailIU1Test
    cmpwi 20, 0x0000
    bnel FailIU1Test
    cmpwi 21, 0x0000
    bnel FailIU1Test
    cmpwi 22, 0x0000
    bnel FailIU1Test

    lis 24, 0x8000
    lis 25, 0x7FFF
    ori 25, 25, 0xFFFF
    xor 28, 28, 28
    mtcr 28
    lis 26, 0x2000
    mtspr XER, 26
    PipelineFlush(5)
    subfeo. 20, 25, 24
    bngl FailIU1Test
    mcrxr 1
    mtspr XER, 26
    subfeo. 21, 25, 24
    bngl FailIU1Test
    mcrxr 3
    mtspr XER, 26
    subfeo. 22, 25, 24
    bngl FailIU1Test
    mcrxr 5
    cmpwi 20, 0x0001
    cmpwi 2, 21, 0x0001
    cmpwi 4, 22, 0x0001
    lis 26, 0x2E2E
    ori 26, 26, 0x2E00
    mfcr 28
    cmpw 28, 26
    bnel FailIU1Test

    xor 24, 24, 24
    mtcr 24
    lis 27, 0xFFFF
    ori 27, 27, 0xFFFF
    lis 28, 0x2000
    PipelineFlush(4)
    mtspr XER, 28
    subfme. 20, 24
    bnll FailIU1Test
    mcrxr 1
    mtspr XER, 28
    subfme. 21, 24
    bnll FailIU1Test
    mcrxr 3
    mtspr XER, 28
    subfme. 22, 24
    bnll FailIU1Test
    mcrxr 5
    cmpw 20, 27
    cmpw 2, 21, 27
    cmpw 4, 22, 27
    lis 27, 0x2222
    ori 27, 27, 0x2200
    mfcr 28
    cmpw 28, 27
    bnel FailIU1Test

    xor 24, 24, 24
    mtcr 24
    lis 27, 0x7FFF
    ori 27, 27, 0xFFFE
    lis 25, 0x8000
    xor 28, 28, 28
    PipelineFlush(5)
    mtspr XER, 28
    subfmeo. 20, 25
    bltl FailIU1Test
    mcrxr 1
    mtspr XER, 28
    subfmeo. 21, 25
    bltl FailIU1Test
    mcrxr 3
    mtspr XER, 28
    subfmeo. 22, 25
    bltl FailIU1Test
    mcrxr 5
    cmpw 20, 27
    cmpw 2, 21, 27
    cmpw 4, 22, 27
    lis 27, 0x2222
    ori 27, 27, 0x2200
    mfcr 28
    cmpw 28, 27
    bnel FailIU1Test

    lis 26, 0x7FFF
    ori 26, 26, 0xFFFF
    li 24, 0x0001
    lis 25, 0x8000
    ori 25, 25, 0x0002
    lis 28, 0x2020
    ori 28, 28, 0x2000
    xor 27, 27, 27
    mtcr 27
    PipelineFlush(8)
    subfo. 20, 26, 24
    bnll FailIU1Test
    mcrxr 1
    subfo. 21, 26, 24
    bnll FailIU1Test
    mcrxr 3
    subfo. 22, 26, 24
    bnll FailIU1Test
    mcrxr 5
    cmpw 20, 25
    cmpw 2, 21, 25
    cmpw 4, 22, 25
    mfcr 25
    cmpw 25, 28
    bnel FailIU1Test

    lis 24, 0x7FFF
    ori 24, 24, 0xFFFF
    xor 28, 28, 28
    mtcr 28
    lis 25, 0x2000
    lis 27, 0x2020
    ori 27, 27, 0x2000
    mtspr XER, 25
    lis 28, 0x8000
    ori 28, 28, 0x0001
    PipelineFlush(8)
    subfze. 20, 24
    bnll FailIU1Test
    mcrxr 1
    mtspr XER, 25
    subfze. 21, 24
    bnll FailIU1Test
    mcrxr 3
    mtspr XER, 25
    subfze. 22, 24
    bnll FailIU1Test
    mcrxr 5
    cmpw 20, 28
    cmpw 2, 21, 28
    cmpw 4, 22, 28
    mfcr 28
    cmpw 28, 27
    bnel FailIU1Test

    lis 24, 0x7FFF
    ori 24, 24, 0xFFFF
    xor 28, 28, 28
    mtcr 28
    lis 25, 0x2000
    lis 27, 0x2020
    ori 27, 27, 0x2000
    mtspr XER, 25
    lis 28, 0x8000
    ori 28, 28, 0x0001
    PipelineFlush(8)
    subfzeo. 20, 24
    bnll FailIU1Test
    mcrxr 1
    mtspr XER, 25
    subfzeo. 21, 24
    bnll FailIU1Test
    mcrxr 3
    mtspr XER, 25
    subfzeo. 22, 24
    bnll FailIU1Test
    mcrxr 5
    cmpw 20, 28
    cmpw 2, 21, 28
    cmpw 4, 22, 28
    mfcr 28
    cmpw 28, 27
    bnel FailIU1Test

    lis 25, 0xEFFF
    ori 25, 25, 0xFFFF
    lis 27, 0xF7FF
    ori 27, 27, 0xFFFF
    li 29, 0x0001
    xor 26, 26, 26
    PipelineFlush(6)
    mtspr XER, 26
    sraw. 20, 25, 29
    bgtl FailIU1Test
    mcrxr 1
    bnel 1, FailIU1Test
    mtspr XER, 26
    sraw. 21, 25, 29
    bgtl FailIU1Test
    mcrxr 3
    bnel 3, FailIU1Test
    mtspr XER, 26
    sraw. 22, 25, 29
    bgtl FailIU1Test
    mcrxr 5
    bnel 5, FailIU1Test
    cmpw 20, 27
    bnel FailIU1Test
    cmpw 21, 27
    bnel FailIU1Test
    cmpw 22, 27
    bnel FailIU1Test

    lis 25, 0xEFFF
    ori 25, 25, 0xFFFF
    lis 27, 0xF7FF
    ori 27, 27, 0xFFFF
    xor 26, 26, 26
    PipelineFlush(5)
    mtspr XER, 26
    srawi. 20, 25, 1
    bgtl FailIU1Test
    mcrxr 1
    bnel 1, FailIU1Test
    mtspr XER, 26
    srawi. 21, 25, 1
    bgtl FailIU1Test
    mcrxr 3
    bnel 3, FailIU1Test
    mtspr XER, 26
    srawi. 22, 25, 1
    bgtl FailIU1Test
    mcrxr 5
    bnel 5, FailIU1Test
    cmpw 20, 27
    bnel FailIU1Test
    cmpw 21, 27
    bnel FailIU1Test
    cmpw 22, 27
    bnel FailIU1Test

# .section cr_op_verification_section, "ax"
     
    lis 25, 0xD685
    ori 25, 25, 0x5E99
    lis 22, 0xE94A
    ori 22, 22, 0xAD65
    mtcr 22
    crand 29, 30, 31
    crand 19, 20, 21
    crand 9, 10, 11

    crnand 28, 30, 31
    crnand 18, 20, 21
    crnand 8, 10, 11

    cror 27, 30, 31
    cror 17, 20, 21
    cror 7, 10, 11

    crnor 26, 30, 31
    crnor 16, 20, 21
    crnor 6, 10, 11

    crorc 25, 30, 31
    crorc 15, 20, 21
    crorc 5, 10, 11

    crandc 24, 31, 30
    crandc 14, 21, 20
    crandc 4, 11, 10

    creqv 23, 30, 31
    creqv 13, 20, 21
    creqv 3, 10, 11

    crxor 22, 30, 31
    crxor 12, 20, 21
    crxor 2, 10, 11

    mfcr 30
    cmpw 30, 25
    bnel FailIU1Test

# .section return_code, "ax"
     
    TestConclusion
    b EndIU1Test
FailIU1Test:
    ErrorHandling

EndIU1Test:
    StackEpilogBegin
    RestoreCtrXerCr
    StackEpilogEnd

 blr







             
             
         
             
             
             
             
             
             
             
             
             









             










 




             

                         
