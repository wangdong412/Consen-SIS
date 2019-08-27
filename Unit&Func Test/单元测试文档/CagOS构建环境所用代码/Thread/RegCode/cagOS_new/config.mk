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

FS_ENABLE 			= n
FS_FLASH			= o# o->nor,s->spi,f->fpga,n->empty
NOR_FLASH_ENABLE		= y
DEMO_ENABLE			= n
FSL_DEV_BOARD			= n

#check:

ifeq ($(FSL_DEV_BOARD) , y)
IIC_ENABLE			= y
SPI_ENABLE			= y
SPI_FLASH_ENABLE		= n
	MODULE += -DFSL_BOARD
ifeq ($(IIC_ENABLE),y)
	MODULE += -DCONFIG_IIC
endif
ifeq ($(SPI_ENABLE),y)
	MODULE += -DCONFIG_SPI
endif
ifeq ($(SPI_FLASH_ENABLE),y)
	MODULE += -DCONFIG_SPI_FLASH
endif
else
IIC_ENABLE			= n
SPI_ENABLE			= n
SPI_FLASH_ENABLE	= n
endif


ifeq ($(FS_ENABLE) ,y)
	MODULE += -DCONFIG_FS
endif

ifeq ($(NOR_FLASH_ENABLE),y)
	MODULE += -DCONFIG_NOR_FLASH
endif

ifeq ($(DEMO_ENABLE),y)
	MODULE += -DCONFIG_DEMO
endif

ifeq ($(FS_FLASH),o)
	MODULE += -DSFFS_NOR
	MODULE += -DSFFS_MULTI_DEVICE
else ifeq ($(FS_FLASH),s)
	MODULE += -DSFFS_SPI
else ifeq ($(FS_FLASH),f)
	MODULE += -DSFFS_FPGA_SPI
endif

CPU           :=  PPCE500V2
BSP           :=  fsl_p1p2
TOOL          :=  gnu
ARCH          :=  powerpc
FLOAT         :=  SPE

BUILDCORE ?= COREX
BUILDCORE := $(subst \,/,$(BUILDCORE))

CAGHOME ?= /home/CAG_DEV_GIT/Release_DVD_ME/Release_DVD/Source/PM/cagOS
CAGHOME := $(subst \,/,$(CAGHOME))

TOOLCHAIN ?= /opt/fsl-networking/QorIQ-SDK-V1.7/sysroots/x86_64-fslsdk-linux/usr/bin/powerpc-fsl-linux-gnuspe/powerpc-fsl-linux-gnuspe-
TOOLCHAIN := $(subst \,/,$(TOOLCHAIN))

TOPDIR        :=  $(CAGHOME)/
DRIVERDIR     :=  $(CAGHOME)/driver/
BOOTDIR       :=  $(CAGHOME)/boot/
KERNELDIR     :=  $(CAGHOME)/kernel/
EXTKERNELDIR  :=  $(CAGHOME)/extkernel/
INCLUDEDIR    :=  $(CAGHOME)/include/
BINARYDIR     :=  $(CAGHOME)/binary/$(BUILDCORE)/
DOCDIR        :=  $(CAGHOME)/doc/

# checkdir
$(shell if [ ! -d $(CAGHOME)/binary ]; then mkdir $(CAGHOME)/binary ;fi)

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
  CPLUS	  = $(CROSS_COMPILE)g++ --sysroot=/opt/fsl-networking/QorIQ-SDK-V1.7/sysroots/ppce500v2-fsl-linux-gnuspe
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
TEXT_BASE =0x1E00000 
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
          -mhard-float -mstrict-align -fno-zero-initialized-in-bss -fstrength-reduce -D_CAG_START_TEXT=$(TEXT_BASE) -DSFFS_CONFIG

ifeq (SPE,$(FLOAT))
CFLAGS  += -mabi=spe -mno-altivec -mspe -mcpu=8548 -mfloat-gprs=double -mno-string  -mspe=yes -Wall 
ASFLAGS += -mabi=spe -mno-altivec -mspe -mcpu=8548 -mfloat-gprs=double -mno-string  -mspe=yes -Wall
else 
CFLAGS  += -mno-altivec -mspe=no -mfloat-gprs=double -mabi=no-spe -me500 -mno-spe
ASFLAGS += -mno-altivec -mspe=no -mfloat-gprs=double -mabi=no-spe -me500 -mno-spe
endif

# module macro define
CFLAGS += $(MODULE)


ARFLAGS=crv

ifeq (CORE0,$(BUILDCORE))
LDFLAGS= -Bstatic -EB -defsym _CAG_DATA_ALIGN=1 -T cagOSCORE0.lds --gc-section -Ttext=$(TEXT_BASE) -X -N 
else
LDFLAGS= -Bstatic -EB -defsym _CAG_DATA_ALIGN=1 -T cagOSCORE1.lds --gc-section -Ttext=$(TEXT_BASE) -X -N 
endif

%.s:    %.S 
	$(CC) $(ASFLAGS) -c -o $@ $<
%.o:	%.S
	$(CC) $(ASFLAGS) -c -o $@ $<
%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

