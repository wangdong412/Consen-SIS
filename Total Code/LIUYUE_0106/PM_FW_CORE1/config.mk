# ------------------------------------------------------------
# Project:RTS
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
BUILDBIN  := BIN0

CPU           :=  PPCE500V2
BSP           :=  fsl_p1p2
TOOL          :=  gnu
ARCH          :=  powerpc
FLOAT         :=  SPE

BUILDCORE ?= COREX
BUILDCORE := $(subst \,/,$(BUILDCORE))

CAGHOME ?= $(PWD)
CAGHOME := $(subst \,/,$(CAGHOME))

TOOLCHAIN ?= /opt/fsl-networking/QorIQ-SDK-V1.7/sysroots/i686-fslsdk-linux/usr/bin/powerpc-fsl-linux-gnuspe/powerpc-fsl-linux-gnuspe-
TOOLCHAIN := $(subst \,/,$(TOOLCHAIN))

TOPDIR        :=  $(CAGHOME)/
INC_PREFIX := $(shell dirname $(CAGHOME))
#INCLUDEDIR    :=  $(INC_PREFIX)/PM_FW_COMMON/Srv/Scheduler/include/
COMMON_INCLUDEDIR    :=  $(INC_PREFIX)/PM_FW_COMMON/
BINARYDIR     :=  $(CAGHOME)/binary/$(BUILDCORE)/
DOCDIR        :=  $(CAGHOME)/doc/

# Check if verbosity is ON for build process

VB ?:= 1
VB := $(subst \,/,$(VB))

ifeq ($(VB), 0)
        MAKEFLAG := -s --no-print-directory
else
        MAKEFLAG := -w
endif

ifneq (,$(findstring PPC,$(CPU)))
ARCH := ppc
else
  ifneq (,$(findstring ARM,$(CPU)))
    ARCH := arm
  else
    $(error unsupported CPU)	
  endif
endif

ifeq (gnu,$(TOOL))
  CROSS_COMPILE = $(TOOLCHAIN)
  AS      = $(CROSS_COMPILE)as
  LD	  = $(CROSS_COMPILE)ld
  CC	  = $(CROSS_COMPILE)gcc --sysroot=/opt/fsl-networking/QorIQ-SDK-V1.7/sysroots/ppce500v2-fsl-linux-gnuspe
  CPP	  = $(CROSS_COMPILE)cpp
  AR	  = $(CROSS_COMPILE)ar
  NM	  = $(CROSS_COMPILE)nm
  STRIP	  = $(CROSS_COMPILE)strip
  OBJCOPY = $(CROSS_COMPILE)objcopy
  OBJDUMP = $(CROSS_COMPILE)objdump
  RANLIB  = $(CROSS_COMPILE)RANLIB
else
 $(error unsupported diab TOOL now)
endif

ifeq (gnu,$(TOOL))
CC_OPTIM += -std=c99
else
CC_OPTIM += -Xdialect-c99
endif

#config the choosed CORE

ifeq (CORE0,$(BUILDCORE))
TEXT_BASE =0x100000 
else

ifeq (BIN0, $(BUILDBIN))
TEXT_BASE =0x1E00000
endif
ifeq (BIN1, $(BUILDBIN))
TEXT_BASE =0x6E00000 
endif
ifeq (BIN2, $(BUILDBIN))
TEXT_BASE =0xBE00000 
endif

endif

ifeq (COREX,$(BUILDCORE))
 $(error You should do "make BUILDCORE=CORE0 VB=0/1 or BUILDCORE=CORE1 VB=0/1 ***")
endif

gccincdir := $(shell $(CC) -print-file-name=include)
PLATFORM_LIBS += -L $(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`) -lgcc

CFLAGS = -g -O0 -ffunction-sections -fdata-sections -meabi -D__KERNEL__  -fno-builtin -ffreestanding  -nostdinc \
         -isystem $(gccincdir) -pipe -DCONFIG_PPC -D__powerpc__ -ffixed-r2 -Wa,-me500 \
         -Wstrict-prototypes -fno-stack-protector  -Wno-format-nonliteral  -Wno-format-security \
         -mhard-float -mstrict-align -fno-zero-initialized-in-bss -fstrength-reduce -D_CAG_START_TEXT=$(TEXT_BASE)

ASFLAGS = -g -O0 -D__ASSEMBLY__ -ffunction-sections -fdata-sections -meabi -D__KERNEL__ -fno-builtin -ffreestanding  -nostdinc \
          -isystem $(gccincdir) -pipe -DCONFIG_PPC -D__powerpc__ -ffixed-r2 -Wa,-me500 \
          -Wstrict-prototypes -fno-stack-protector  -Wno-format-nonliteral  -Wno-format-security \
          -mhard-float -mstrict-align -fno-zero-initialized-in-bss -fstrength-reduce -D_CAG_START_TEXT=$(TEXT_BASE)

ifeq (SPE,$(FLOAT))
CFLAGS  += -mabi=spe -mno-altivec -mspe -mcpu=8548 -mfloat-gprs=double -mno-string  -mspe=yes -Wall 
ASFLAGS += -mabi=spe -mno-altivec -mspe -mcpu=8548 -mfloat-gprs=double -mno-string  -mspe=yes -Wall
else 
CFLAGS  += -mno-altivec -mspe=no -mfloat-gprs=double -mabi=no-spe -me500 -mno-spe
ASFLAGS += -mno-altivec -mspe=no -mfloat-gprs=double -mabi=no-spe -me500 -mno-spe
endif

ARFLAGS=crv

ifeq (BIN0, $(BUILDBIN))
LDFLAGS= -Bstatic -EB -defsym _CAG_DATA_ALIGN=1 -T RTS0.lds --gc-section -Ttext=$(TEXT_BASE) -X -N 
endif
ifeq (BIN1, $(BUILDBIN))
LDFLAGS= -Bstatic -EB -defsym _CAG_DATA_ALIGN=1 -T RTS1.lds --gc-section -Ttext=$(TEXT_BASE) -X -N 
endif
ifeq (BIN2, $(BUILDBIN))
LDFLAGS= -Bstatic -EB -defsym _CAG_DATA_ALIGN=1 -T RTS2.lds --gc-section -Ttext=$(TEXT_BASE) -X -N 
endif

%.s:    %.S 
	$(CC) $(ASFLAGS) -c -o $@ $<
%.o:	%.S
	$(CC) $(ASFLAGS) -c -o $@ $<
%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

