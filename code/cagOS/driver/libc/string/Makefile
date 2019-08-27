# Makefile - makefile for pkgs/os/lang-lib/libc/libc-kernel/src/libc/string
#
# Copyright (c) 2015 CAG Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 03jun15,Jeff created 
#
# DESCRIPTION
# This file contains the makefile rules for building VXBL
#
#

include ../../../config.mk

EXTRA_INCLUDE += -I$(TOPDIR) -I$(INCLUDEDIR) 
ifeq (gnu,$(TOOL))
CC_OPTIM += -std=c99 -fdata-sections -ffunction-sections
else
CC_OPTIM += -Xsection-split
endif

CFLAGS += $(EXTRA_INCLUDE) $(CC_OPTIM)
ASFLAGS += $(EXTRA_INCLUDE) $(CC_OPTIM)

LIB =  libstring.a

OBJS	= bLib.o memcmp.o memmove.o strcpy.o strncat.o strncpy.o \
         memchr.o memcpy.o  memset.o strcmp.o strlen.o 
all:$(LIB)

$(LIB): $(OBJS) 
	$(AR) crv $@ $^

.depend:Makefile $(OBJS) $@
	$(CC) -M $(CFLAGS) $(OBJS:%.o=%.c) > $@

clean:
	rm -f $(OBJS)
	rm -f $(LIB) 
	rm -f *.bak 
	rm -f .depend

#########################################################################

sinclude $(obj).depend

#########################################################################
