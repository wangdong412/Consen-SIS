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

include config.mk

LIBS = start/libstart.a
LIBS += arch/libarch.a
LIBS += kernel/ports/libkernelport.a
LIBS += kernel/thread/libkernelthread.a
LIBS += kernel/timers/libkerneltimer.a
LIBS += kernel/epic/libkernelepic.a
LIBS += utility/libutility.a
LIBS += driver/libc/stdio/libstdio.a
LIBS += driver/libc/string/libstring.a
LIBS += driver/libc/stdlib/libstdlib.a
LIBS += driver/device/dfs/libdfs.a
LIBS += driver/device/pcie/libpcie.a
LIBS +=	driver/libhwinit.a

ifeq ($(SPI_ENABLE),y)
	LIBS += driver/device/spi/libspi.a
endif

ifeq ($(IIC_ENABLE),y)
	LIBS += driver/device/iic/libiic.a
endif

ifeq ($(NOR_FLASH_ENABLE),y)
	LIBS += driver/device/norflash/libnor.a
endif

ifeq ($(FS_ENABLE),y)
	LIBS += driver/fs/sffs/libsffs.a
	LIBS += driver/fs/libfs.a
endif

ifeq ($(DEMO_ENABLE),y)
	LIBS += demo/libdemo.a
endif

LIBS += start/libc.a
LIBS += start/libm.a
LIBS += start/libgcc.a
LIBS += start/libgcc_eh.a

ALL :
	$(MAKE) info
	$(MAKE) distclean
	#$(MAKE) cleanlib>/dev/null
	@/bin/cp -f  /home/fsl-sdk/v1.7/QorIQ-SDK-V1.7-20141218-yocto/poky/build_p1020rdb_release/tmp/work/ppce500v2-fsl-linux-gnuspe/eglibc/2.15-r3/packages-split/eglibc-staticdev/usr/lib/libm.a ./start/ 
	@/bin/cp -f /home/fsl-sdk/v1.7/QorIQ-SDK-V1.7-20141218-yocto/poky/build_p1020rdb_release/tmp/work/ppce500v2-fsl-linux-gnuspe/eglibc/2.15-r3/packages-split/eglibc-staticdev/usr/lib/libc.a ./start/
	#$(MAKE) SLIBS>/dev/null 
	@/bin/cp -f /home/fsl-sdk/v1.7/QorIQ-SDK-V1.7-20141218-yocto/poky/build_p1020rdb_release/tmp/work/ppce500v2-fsl-linux-gnuspe/libgcc/4.8.1+fsl-r0/packages-split/libgcc-dev/usr/lib/powerpc-fsl-linux-gnuspe/4.8.1/libgcc.a ./start/
	@/bin/cp -f /home/fsl-sdk/v1.7/QorIQ-SDK-V1.7-20141218-yocto/poky/build_p1020rdb_release/tmp/work/ppce500v2-fsl-linux-gnuspe/libgcc/4.8.1+fsl-r0/packages-split/libgcc-dev/usr/lib/powerpc-fsl-linux-gnuspe/4.8.1/libgcc_eh.a ./start/
	@$(MAKE) SLIBS $(MAKEFLAG) 
	@$(MAKE) cagOs $(MAKEFLAG)
rall:
#	$(MAKE) clean
	$(MAKE) ALL 

cagOs:$(LIBS) 
ifeq (gnu,$(TOOL))
	$(LD)  $(PLATFORM_LIBS) -o $(BINARYDIR)cagOs$(BUILDCORE)  $(LDFLAGS) --start-group $(LIBS) $(LIB_DIR) --end-group -Map $(BINARYDIR)cagOs$(BUILDCORE).map 
	$(OBJCOPY) --gap-fill=0xff -O binary $(BINARYDIR)cagOs$(BUILDCORE) $(BINARYDIR)cagOs$(BUILDCORE).bin 
#cp cagOs.bin u-boot.bin
	#/bin/date "+%Y/%m/%d %H:%M:%S" >> cagOs.bin
	$(OBJDUMP) -D $(BINARYDIR)cagOs$(BUILDCORE) > $(BINARYDIR)cagOs$(BUILDCORE)dump
else
        $(error unsupported diab TOOL now)
endif
	@echo @@@@Building done!

SLIBS:
	@for dir in $(LIBS) ; do $(MAKE) -C `dirname $$dir`; done

cleanlib:
	@find . -name "*.a" -exec rm -f {} \;
	@rm -f $(BINARYDIR)cagOs$(BUILDCORE).bin 
	@rm -f $(BINARYDIR)cagOs$(BUILDCORE)

.PHONY: clean	
clean:
	sed -i "/CAGHOME ?= /d" config.mk
	sed -i "/CAGHOME := / i\CAGHOME ?= $(PWD)" config.mk
	find . -name "*.depend" -print | xargs  rm -f
	find . -name "*.o" -print | xargs  rm -f
	find . -name "*.a" -print | xargs rm -f
	find . -name "*.bak" -print | xargs rm -f
	find . -name ".*.*.swp" -print | xargs rm -f
	find . -name "*~" -print | xargs rm -f
	find . -name "*.svg" -print | xargs rm -f
	rm -f *.bin
	rm -f mkerror
	rm -f $(BINARYDIR)cagOs$(BUILDCORE).bin 
	rm -f $(BINARYDIR)cagOs$(BUILDCORE) 
	rm -f $(BINARYDIR)cagOs$(BUILDCORE).map 
	rm -f $(BINARYDIR)cagOs$(BUILDCORE)dump
#	find ./ -type f \
		\( -name 'core' -o -name '*.bak' -o -name '*~' \
		-o -name '*.o'  -o -name '*.a' -o -name ".depend" \) -print \
		| xargs rm -f

# Docs
.PHONY: doc
doc:
	doxygen $(CAGHOME)/Doxyfile

# Clean up the documentation.
.PHONY: cleandoc
cleandoc:
	@echo "!rm the doc directory"
	rm -f doc/*

tag:
	find . -name "*.[chS]" -print | xargs  ctags -R
	find . -name "*.[chS]" -print | xargs  cscope -Rbkq

TAG_SUBDIRS := $(CAGHOME)

FIND := find
FINDFLAGS := -L

tags ctags:
		ctags -w -o ctags `$(FIND) $(FINDFLAGS) $(TAG_SUBDIRS) \
						-name '*.[chS]' -print`
etags:
		etags -a -o $(obj)etags `$(FIND) $(FINDFLAGS) $(TAG_SUBDIRS) \
						-name '*.[chS]' -print`
cscope:
		$(FIND) $(FINDFLAGS) $(TAG_SUBDIRS) -name '*.[chS]' -print > \
						cscope.files
		cscope -b -q -k


init:
	sed -i "/CAGHOME ?= /d" config.mk
	sed -i "/CAGHOME := / i\CAGHOME ?= $(PWD)" config.mk
	make clean

info :
	@echo +++++++++++++++++++
	@echo + powered by Jeff +
	@echo +++++++++++++++++++
	@echo "->building the CAGOS AMP project direcctory:"
	@echo "->building the CAGOS AMP project direcctory:" 
	@echo CAGHOME .............................: $(CAGHOME)
	@echo Selected Architecture................: $(ARCH)
	@echo Selected Cpu.........................: $(CPU)
	@echo Selected CORE........................: CORE$(BUILDCORE)
	@echo Selected Board.......................: $(BSP)
	@echo Selected TEXT BASE...................: $(TEXT_BASE)
	@echo Selected Compiler....................: $(CROSS_COMPILE)
	@echo Output directory for binary files....: $(BINARYDIR)
	@echo Output directory for library files...: $(LIBDIR)
	@echo Output directory for documentation...: $(DOCDIR)
	@echo the platform ........................: $(PLATFORM_LIBS)
	@echo The make args is ....................: $(MAKEFLAG)

distclean:
	@echo "Clean the project first"
	make BUILDCORE=CORE0 VB=0 clean
	make BUILDCORE=CORE1 VB=0 clean

help :
	@echo "CAGOS v0.1: makefile help"
	@echo "make 		: make OS images"
	@echo "make info 	: print compile environment variables, help to check errors"
	@echo "make tag		: create ctags and cscopes files"
	@echo "make clean	: clean project"
	@echo "make doc		: create help document"
	@echo "make help	: print help information"
	@echo "prepare		: 1. edit CAGHOME in config.mk; 2. edit TOOLCHAIN in config.mk or add ppc gcc into your PATH"
