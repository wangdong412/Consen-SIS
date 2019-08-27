include ../config.mk

LIB = libdemo.a

OBJS = 
ifeq ($(SPI_ENABLE),y)
OBJS += spi_demo.o
endif
ifeq ($(IIC_ENABLE),y)
OBJS += iic_demo.o
endif
ifeq ($(NOR_ENABLE),y)
OBJS += nor_demo.o
endif
ifeq ($(FS_ENABLE),y)
OBJS += sffs_demo.o
endif
OBJS += demo_cmd.o demo.o dfs_demo.o task_demo.o sem_demo.o task_sem_demo.o mt_demo.o pcie_demo.o pmbus_demo.o cmbus_demo.o dma_demo.o

EXTRA_INCLUDE += -I$(TOPDIR) -I$(INCLUDEDIR)

ifeq (gnu,$(TOOL))
CC_OPTIM += -std=c99 -fdata-sections -ffunction-sections
else
CC_OPTIM += -Xsection-split
endif

CFLAGS += $(EXTRA_INCLUDE) $(CC_OPTIM)
ASFLAGS += $(EXTRA_INCLUDE) $(CC_OPTIM)

all: $(LIB)

$(LIB) : $(OBJS)
	$(AR) crv $@ $^

.depend:Makefile $(SOBJS) $(OBJS) $@
	$(CC) -M $(ASFLAGS) $(SOBJS:%.o=%.S) $(CFLAGS) $(OBJS:%.o=%.c) > $@

clean:
	rm -f *.o 
	rm -f *.out
tag:
	ctags -R
