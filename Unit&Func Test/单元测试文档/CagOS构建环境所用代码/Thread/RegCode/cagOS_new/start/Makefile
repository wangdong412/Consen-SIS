# Makefile
# =========

#------------------------------------------------------------
# Project:	cagOS
# Goal:		makefile for the "boot" module.
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
# This file contains the makefile rules for building cagBL
#

include ../config.mk

EXTRA_INCLUDE += -I$(TOPDIR) -I$(INCLUDEDIR)

ifeq (gnu,$(TOOL))
CC_OPTIM += -std=c99 -fdata-sections -ffunction-sections
else
CC_OPTIM += -Xsection-split
endif

CFLAGS += $(EXTRA_INCLUDE) $(CC_OPTIM)
ASFLAGS += $(EXTRA_INCLUDE) $(CC_OPTIM)

LIB =  libstart.a
OBJS = sysLaw.o sysL2Cache.o sysBoard.o sysCpuLib.o sysBanner.o access.o sysGpio.o
SOBJS = sysAccess.o sysCpuALib.o

all:$(LIB)

$(LIB): $(OBJS) $(SOBJS)
	$(AR) crv $@ $^

.depend:Makefile $(SOBJS) $(OBJS) $@
	$(CC) -M $(ASFLAGS) $(SOBJS:%.o=%.S) $(CFLAGS) $(OBJS:%.o=%.c) > $@

sinclude .depend

#########################################################################

sinclude $(obj).depend

#########################################################################

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.bak .depend
