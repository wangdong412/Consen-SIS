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

include config.mk

#OBJS := $(addprefix $(obj),$(OBJS))
ifeq (CORE0,$(BUILDCORE))
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Scheduler/lib/CORE0/cagOsCORE0-0.a
else
ifeq (BIN0, $(BUILDBIN))
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Scheduler/lib/CORE1/cagOsCORE1-0.a
endif
ifeq (BIN1, $(BUILDBIN))
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Scheduler/lib/CORE1/cagOsCORE1-1.a
endif
ifeq (BIN2, $(BUILDBIN))
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Scheduler/lib/CORE1/cagOsCORE1-2.a
endif
endif

LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Scheduler/lib/libm.a
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Scheduler/lib/libgcc.a
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Scheduler/lib/libgcc_eh.a
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Scheduler/lib/libc.a

LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/SharedMem/src/libSharedMem.a
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Algrithm/src/libAlgrithm.a
#LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Debug/src/libDebug.a
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Diag/asm/libAsmDiag.a
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Diag/src/libCDiag.a
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Log/src/libLog.a
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/PMBUS/src/libPMBUS.a
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/SafetyProtocol/src/libSafetyProtocol.a
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Sys/src/libSysCommon.a
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/IPBUS/src/libIPBUS.a
LIBS += $(INC_PREFIX)/PM_FW_COMMON/Srv/Ver/src/libVer.a


RTS_LIBS = App/Main/src/libMain.a

RTS_LIBS += App/Comm/src/libComm.a
RTS_LIBS += App/Diag/src/libDiag.a
RTS_LIBS += App/FileManager/src/libFileManager.a
RTS_LIBS += App/LEDManager/src/libLEDManager.a
RTS_LIBS += App/Manager/src/libManager.a
RTS_LIBS += App/Monitor/src/libMonitor.a
RTS_LIBS += App/ProjectConfig/src/libProjectConfig.a
RTS_LIBS += App/Sync/src/libSync.a
RTS_LIBS += App/SysStateManager/src/libSysStateManager.a
#RTS_LIBS += App/Updater/src/libUpdater.a

RTS_LIBS += Srv/CommProtocol/src/libCMBusProtocol.a
RTS_LIBS += Srv/SysResource/src/libSys.a


all:
	$(MAKE) info
	$(MAKE) cleanlib>/dev/null
	@$(MAKE) PM_FW_COMMON #$(MAKEFLAG)
	@$(MAKE) SAMPLE_SLIBS $(MAKEFLAG)
	@$(MAKE) RTS $(MAKEFLAG)
	

rall:
#	$(MAKE) clean
	$(MAKE) all 

RTS:$(LIBS) $(RTS_LIBS) 
ifeq (gnu,$(TOOL))

ifeq (BIN0, $(BUILDBIN))
	$(LD) $(PLATFORM_LIBS) -o $(BINARYDIR)RTS$(BUILDCORE) $(LDFLAGS) --start-group $(LIBS) $(RTS_LIBS) $(LIB_DIR) --end-group -Map $(BINARYDIR)RTS$(BUILDCORE).map0
	$(OBJCOPY) --gap-fill=0xff -O binary $(BINARYDIR)RTS$(BUILDCORE) $(BINARYDIR)RTS$(BUILDCORE).bin 
	./binary/CORE1/crc0.sh  ./binary/CORE1/RTSCORE1.bin
	$(OBJDUMP) -D $(BINARYDIR)RTS$(BUILDCORE) > $(BINARYDIR)RTS$(BUILDCORE)dump0
endif
ifeq (BIN1, $(BUILDBIN))
	$(LD) $(PLATFORM_LIBS) -o $(BINARYDIR)RTS$(BUILDCORE) $(LDFLAGS) --start-group $(LIBS) $(RTS_LIBS) $(LIB_DIR) --end-group -Map $(BINARYDIR)RTS$(BUILDCORE).map1
	$(OBJCOPY) --gap-fill=0xff -O binary $(BINARYDIR)RTS$(BUILDCORE) $(BINARYDIR)RTS$(BUILDCORE).bin 
	./binary/CORE1/crc1.sh  ./binary/CORE1/RTSCORE1.bin
	$(OBJDUMP) -D $(BINARYDIR)RTS$(BUILDCORE) > $(BINARYDIR)RTS$(BUILDCORE)dump1
endif
ifeq (BIN2, $(BUILDBIN))
	$(LD) $(PLATFORM_LIBS) -o $(BINARYDIR)RTS$(BUILDCORE) $(LDFLAGS) --start-group $(LIBS) $(RTS_LIBS) $(LIB_DIR) --end-group -Map $(BINARYDIR)RTS$(BUILDCORE).map2
	$(OBJCOPY) --gap-fill=0xff -O binary $(BINARYDIR)RTS$(BUILDCORE) $(BINARYDIR)RTS$(BUILDCORE).bin 
	./binary/CORE1/crc2.sh  ./binary/CORE1/RTSCORE1.bin
	$(OBJDUMP) -D $(BINARYDIR)RTS$(BUILDCORE) > $(BINARYDIR)RTS$(BUILDCORE)dump2
endif
	rm -f $(BINARYDIR)RTS$(BUILDCORE).map 
	rm -f $(BINARYDIR)RTS$(BUILDCORE)dump
else
        $(error unsupported diab TOOL now)
endif
	@echo @@@@Building done!

SAMPLE_SLIBS:
	for dir in $(RTS_LIBS) ; do $(MAKE) -C `dirname $$dir`; done
	
PM_FW_COMMON:
	$(MAKE) -C $(INC_PREFIX)/PM_FW_COMMON/Srv/Algrithm/src/ 
#	$(MAKE) -C $(INC_PREFIX)/PM_FW_COMMON/Srv/Debug/src/	
	$(MAKE) -C $(INC_PREFIX)/PM_FW_COMMON/Srv/Diag/src/		
	$(MAKE) -C $(INC_PREFIX)/PM_FW_COMMON/Srv/Log/src/	
	$(MAKE) -C $(INC_PREFIX)/PM_FW_COMMON/Srv/PMBUS/src/	
	$(MAKE) -C $(INC_PREFIX)/PM_FW_COMMON/Srv/SafetyProtocol/src/	
	$(MAKE) -C $(INC_PREFIX)/PM_FW_COMMON/Srv/SharedMem/src/	
	$(MAKE) -C $(INC_PREFIX)/PM_FW_COMMON/Srv/Sys/src/	
	$(MAKE) -C $(INC_PREFIX)/PM_FW_COMMON/Srv/IPBUS/src/	
	$(MAKE) -C $(INC_PREFIX)/PM_FW_COMMON/Srv/Ver/src/
	
cleanlib:
	@find ./App/Main/src -name "*.a" -exec rm -f {} \;
	@rm -rf $(BINARYDIR)/$(BUILDCORE)/* 
	

.PHONY: clean	
clean:
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/Algrithm/src -name "*.o" -print | xargs  rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/Algrithm/src -name "*.a" -print | xargs rm -f
#	find $(INC_PREFIX)/PM_FW_COMMON/Srv/Debug/src -name "*.o" -print | xargs  rm -f
#	find $(INC_PREFIX)/PM_FW_COMMON/Srv/Debug/src -name "*.a" -print | xargs rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/Diag/src -name "*.o" -print | xargs  rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/Diag/src -name "*.a" -print | xargs rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/Log/src -name "*.o" -print | xargs  rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/Log/src -name "*.a" -print | xargs rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/PMBUS/src -name "*.o" -print | xargs  rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/PMBUS/src -name "*.a" -print | xargs rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/SafetyProtocol/src -name "*.o" -print | xargs  rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/SafetyProtocol/src -name "*.a" -print | xargs rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/SharedMem/src -name "*.o" -print | xargs  rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/SharedMem/src -name "*.a" -print | xargs rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/Sys/src -name "*.o" -print | xargs  rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/Sys/src -name "*.a" -print | xargs rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/IPBUS/src -name "*.o" -print | xargs  rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/IPBUS/src -name "*.a" -print | xargs rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/Ver/src -name "*.o" -print | xargs  rm -f
	find $(INC_PREFIX)/PM_FW_COMMON/Srv/Ver/src -name "*.a" -print | xargs rm -f

	find ./App/Comm/src -name "*.o" -print | xargs  rm -f
	find ./App/Comm/src -name "*.a" -print | xargs rm -f	
	find ./App/Diag/src -name "*.o" -print | xargs  rm -f
	find ./App/Diag/src -name "*.a" -print | xargs rm -f	
	find ./App/FileManager/src -name "*.o" -print | xargs  rm -f
	find ./App/FileManager/src -name "*.a" -print | xargs rm -f	
	find ./App/LEDManager/src -name "*.o" -print | xargs  rm -f
	find ./App/LEDManager/src -name "*.a" -print | xargs rm -f	
	find ./App/Main/src -name "*.o" -print | xargs  rm -f
	find ./App/Main/src -name "*.a" -print | xargs rm -f
	find ./App/Manager/src -name "*.o" -print | xargs  rm -f
	find ./App/Manager/src -name "*.a" -print | xargs rm -f	
	find ./App/Monitor/src -name "*.o" -print | xargs  rm -f
	find ./App/Monitor/src -name "*.a" -print | xargs rm -f
	find ./App/ProjectConfig/src -name "*.o" -print | xargs  rm -f
	find ./App/ProjectConfig/src -name "*.a" -print | xargs rm -f	
	find ./App/Sync/src -name "*.o" -print | xargs  rm -f
	find ./App/Sync/src -name "*.a" -print | xargs rm -f
	find ./App/SysStateManager/src -name "*.o" -print | xargs  rm -f
	find ./App/SysStateManager/src -name "*.a" -print | xargs rm -f
#	find ./App/Updater/src -name "*.o" -print | xargs  rm -f
#	find ./App/Updater/src -name "*.a" -print | xargs rm -f

	find ./Srv/CommProtocol/src -name "*.o" -print | xargs  rm -f
	find ./Srv/CommProtocol/src -name "*.a" -print | xargs rm -f
	find ./Srv/SysResource/src -name "*.o" -print | xargs  rm -f
	find ./Srv/SysResource/src -name "*.a" -print | xargs rm -f

	
	find . -name "*.bak" -print | xargs rm -f
	find . -name ".*.*.swp" -print | xargs rm -f
	find . -name "*~" -print | xargs rm -f
	find . -name "*.depend" -print | xargs  rm -f
	rm -f mkerror
	rm -f $(BINARYDIR)RTS$(BUILDCORE).bin 
	rm -f $(BINARYDIR)RTS$(BUILDCORE) 
	rm -f $(BINARYDIR)RTS$(BUILDCORE).map 
	rm -f $(BINARYDIR)RTS$(BUILDCORE)dump
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
info :
	@echo +++++++++++++++++++
	@echo + powered by Jeff +
	@echo +++++++++++++++++++
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
	@echo COMMON_INCLUDEDIR.............:$(COMMON_INCLUDEDIR)

