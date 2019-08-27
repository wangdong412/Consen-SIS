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

FS_ENABLE 				=y
FS_FLASH				=f#o->nor,s->spi,f->fpga,n->empty
NOR_FLASH_ENABLE		=y#nor flash driver
FPGA_SPI_FLASH_ENABLE	=y#fpga flash driver
DEMO_ENABLE				=y
FSL_DEV_BOARD			=n#n->cag board,y->fsl board

#checking configurations:

ifeq ($(FSL_DEV_BOARD) , y) 		#fsl board should config here too
IIC_ENABLE			= y
SPI_ENABLE			= y
SPI_FLASH_ENABLE	= y
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

else 								#cag board #default close I2C and spi
IIC_ENABLE			= n
SPI_ENABLE			= n
SPI_FLASH_ENABLE	= n
endif

#cfg flash 

ifeq ($(NOR_FLASH_ENABLE),y) 				#enable nor driver
	MODULE += -DCONFIG_NOR_FLASH
endif


ifeq ($(FPGA_SPI_FLASH_ENABLE),y) 			#enable fpga spi flash driver
	MODULE += -DCONFIG_FPGA_FLASH
endif	

ifeq ($(DEMO_ENABLE),y) 					#enable demo
	MODULE += -DCONFIG_DEMO
endif

#cfg filesystem 

ifeq ($(FS_ENABLE),y)						#enable filesystem
ifeq ($(FS_FLASH),n)
  $(error enable fs without flash)
endif

	MODULE += -DSFFS_MULTI_DEVICE

#select flash for filesystem
ifeq ($(FS_FLASH),o)						#select nor flash
ifeq ($(NOR_FLASH_ENABLE),n)
  $(error fs use nor flash but you did not enable nor flash)	
endif
	MODULE += -DSFFS_NOR
	MODULE += -DCONFIG_FS


else ifeq ($(FS_FLASH),s)					#select spi flash
ifeq ($(SPI_ENABLE),n)
  $(error fs use spi flash but you did not enable spi or you are using cag board)
endif
	MODULE += -DSFFS_SPI
	MODULE += -DCONFIG_FS

else ifeq ($(FS_FLASH),f)					#select fpga spi flash
ifeq ($(FPGA_SPI_FLASH_ENABLE),n)
  $(error fs use fpga spi flash but you did not enable fpga spi flash)
endif
	MODULE += -DSFFS_FPGA_SPI
	MODULE += -DCONFIG_FS

else ifeq ($(FS_FLASH),n)

endif

endif


CPU           :=  PPCE500V2
BSP           :=  fsl_p1p2
TOOL          :=  gnu
ARCH          :=  powerpc
FLOAT         :=  SPE

BUILDCORE ?= CORE0
BUILDCORE := $(subst \,/,$(BUILDCORE))

CAGHOME ?= $(PWD)
CAGHOME := $(subst \,/,$(CAGHOME))

TOOLCHAIN ?= powerpc-fsl-linux-gnuspe-
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

VB ?:= 0
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
#TEXT_BASE = 0x04eb0000
#LDSF = core0.lds
TEXT_BASE = 0x04fe0000
LDSF = coreos.lds
else
TEXT_BASE =0x00eb0000
#TEXT_BASE =0x00fe0000
LDSF = core1.lds
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
CFLAGS  += -mabi=spe -mno-altivec -mspe -mcpu=8548 -mfloat-gprs=double -mno-string -m32 -Wall 
ASFLAGS += -mabi=spe -mno-altivec -mspe -mcpu=8548 -mfloat-gprs=double -mno-string -m32 -Wall
else 
CFLAGS  += -mno-altivec -mspe=no -mfloat-gprs=double -mabi=no-spe -me500 -mno-spe
ASFLAGS += -mno-altivec -mspe=no -mfloat-gprs=double -mabi=no-spe -me500 -mno-spe
endif

# module macro define
CFLAGS += $(MODULE)


ARFLAGS=crv

ifeq (CORE0,$(BUILDCORE))
LDFLAGS= -Bstatic -EB -defsym _CAG_DATA_ALIGN=1 -T $(LDSF) --gc-section -Ttext=$(TEXT_BASE) -X -N 
else
LDFLAGS= -Bstatic -EB -defsym _CAG_DATA_ALIGN=1 -T $(LDSF) --gc-section -Ttext=$(TEXT_BASE) -X -N 
endif

%.s:    %.S 
	$(CC) $(ASFLAGS) -c -o $@ $<
%.o:	%.S
	$(CC) $(ASFLAGS) -c -o $@ $<
%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

