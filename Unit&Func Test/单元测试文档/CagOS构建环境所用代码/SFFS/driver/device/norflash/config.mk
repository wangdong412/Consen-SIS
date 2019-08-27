# ------------------------------------------------------------
# Project:cagOS
# Goal:
# Copyright (c) 2013, 2014 CAG Systems, Inc.
# ------------------------------------------------------------
# 
#     @@@@     @@@      @@@@@       ####      ######
#   @@       @@  @@    @@         ##    ##   ##
#  @@       @@    @@  @@   @@@@  ##      ## ######
#  @@      @@@@@@@@@@ @@    @@    ##    ##      ##
#   @@@@@ @@@      @@@  @@@@@@      ####  #######
# 
#  The right to copy, distribute, modify or otherwise make use
#  of this software may be licensed only pursuant to the terms
#  of an applicable CAG license agreement.
# 
#  1 tab == 4 spaces!
# ------------------------------------------------------------
#  modification history
# --------------------
#  03jun15,Jeff created
# 
#  DESCRIPTION
#  This file contains
#

CPU           :=  PPCE500V2
BSP           :=  fsl_p1p2
TOOL          :=  gnu
ARCH          :=  powerpc
FLOAT         :=  SPE

TOOLCHAIN := powerpc-unknown-linux-gnuspe-

CROSS_COMPILE = $(TOOLCHAIN)
AS      = $(CROSS_COMPILE)as
LD	  = $(CROSS_COMPILE)ld
CC	  = $(CROSS_COMPILE)gcc
CPP	  = $(CROSS_COMPILE)cpp
AR	  = $(CROSS_COMPILE)ar
NM	  = $(CROSS_COMPILE)nm
STRIP	  = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB  = $(CROSS_COMPILE)RANLIB

%.s:    %.S 
	$(CC) $(ASFLAGS) -c -o $@ $<
%.o:	%.S
	$(CC) $(ASFLAGS) -c -o $@ $<
%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

