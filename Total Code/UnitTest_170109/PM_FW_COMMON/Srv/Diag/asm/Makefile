# Makefile
# =========

#------------------------------------------------------------
# Project:	cagOS
# Goal:		makefile for the ext_kernel module.
# Copyright (c) 2013, 2014 CAG Systems, Inc.
#------------------------------------------------------------
#
#    @@@@     @@@      @@@@@       ####      ######
#  @@       @@  @@    @@         ##    ##   ##
# @@       @@    @@  @@   @@@@  ##      ## ######
# @@      @@@@@@@@@@ @@    @@    ##    ##      ##
#  @@@@@ @@@      @@@  @@@@@@      ####  #######
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable CAG license agreement.
#
# 1 tab == 4 spaces!
#------------------------------------------------------------
# modification history
# --------------------
# 03jun15,Jeff created
#
# DESCRIPTION
# This file contains the makefile rules
#

include ../../../../PM_FW_CORE0/config.mk

#EXTRA_INCLUDE += -I$(INCLUDEDIR)  -I$(TOPDIR)
EXTRA_INCLUDE += -I$(COMMON_INCLUDEDIR)  -I$(TOPDIR)

ifeq (gnu,$(TOOL))
CC_OPTIM += -std=c99 -fdata-sections -ffunction-sections
else
CC_OPTIM += -Xsection-split
endif

CFLAGS += $(EXTRA_INCLUDE) $(CC_OPTIM)
ASFLAGS += $(EXTRA_INCLUDE) $(CC_OPTIM)

LIB   = libAsmDiag.a
OBJS  = 
SOBJS = BPU_cond_branch_test_bca.o BPU_cond_branch_test_bcla.o BPU_cond_branch_test.o \
        BPU_uncond_branch_test_ba_bla.o BPU_uncond_branch_test.o cr_test.o Critical_SPR_interword_test.o \
        Critical_SPR_intraword_test.o data_cache_test.o GPR_interword_test.o GPR_intraword_test.o \
        inst_cache_test.o IU1_test.o IU2_test.o lr_test.o LSU_ea_calc_test.o LSU_instr_set_test.o \
        SPR_interword_test.o SPR_intraword_test.o SPRG_test.o ImageSwitch.o
#ExcptTest_s.o FPR_interword_test.o FPR_intraword_test.o LSU_dat_test.o  Pipeline_test.o
#FPSCR_test.o FPU1_test.o FPU2_test.o
all:$(LIB)

$(LIB): $(OBJS) $(SOBJS)
	$(AR) crv $@ $^

.depend:Makefile $(SOBJS) $(OBJS) $@
	$(CC) -M $(ASFLAGS) $(SOBJS:%.o=%.S) $(CFLAGS) $(OBJS:%.o=%.c) > $@

sinclude .depend

clean:
	rm -f $(OBJS) $(SOBJS)
	rm -f $(LIB) *.bak .depend

#########################################################################

sinclude $(obj).depend

#########################################################################
