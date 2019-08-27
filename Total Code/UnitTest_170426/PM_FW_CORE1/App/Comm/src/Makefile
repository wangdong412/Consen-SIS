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

include ../../../../PM_FW_CORE1/config.mk

#EXTRA_INCLUDE += -I$(INCLUDEDIR)  -I$(TOPDIR)
EXTRA_INCLUDE += -I$(COMMON_INCLUDEDIR)  -I$(TOPDIR)

ifeq (gnu,$(TOOL))
CC_OPTIM += -std=c99 -fdata-sections -ffunction-sections
else
CC_OPTIM += -Xsection-split
endif

CFLAGS += $(EXTRA_INCLUDE) $(CC_OPTIM)
ASFLAGS += $(EXTRA_INCLUDE) $(CC_OPTIM)

LIB   = libComm.a
OBJS  = Comm.o 
SOBJS = 

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
