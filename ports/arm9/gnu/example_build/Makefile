# build threadx sample

PREFIX?=arm-none-eabi-
CC:=$(PREFIX)gcc
LD:=$(PREFIX)ld
AR:=$(PREFIX)ar
SIZE:=$(PREFIX)size
STRIP:=$(PREFIX)strip
OBJCOPY:=$(PREFIX)objcopy
OBJDUMP:=$(PREFIX)objdump

LOAD_ADDR:=0x00000000
LINK_ADDR:=0x80000000

LINK_SCRIPT:=sample_threadx.ld

TARGET:=sample_threadx

# sample_objs:=crt0.o reset.o sample_threadx.o
sample_objs:=crt0.o reset.o main.o

sample_objs_in_build = $(addprefix build/, $(sample_objs))

# Threadx core
tx_dirs = ../src ../../../../common/src/
tx_c_srcs = $(foreach dir, $(tx_dirs), $(wildcard $(dir)/*.c))

tx_asm_srcs = $(foreach dir, $(tx_dirs), $(wildcard $(dir)/*.S))
tx_asm_srcs += ./tx_initialize_low_level.S

tx_objs += $(foreach src, $(tx_c_srcs), $(patsubst %.c,%.o,$(src)))
tx_objs += $(foreach src, $(tx_asm_srcs), $(patsubst %.S,%.o,$(src)))

tx_objs_base = $(foreach obj, $(tx_objs), $(notdir $(obj)))
tx_objs_in_build = $(addprefix build/, $(tx_objs_base))

default:$(TARGET).img

$(TARGET).bin: tx
	$(LD) -A arm9 -T $(LINK_SCRIPT) -g $(sample_objs_in_build) tx.a ./libc.a libgcc.a -o $(TARGET).elf -M > $(TARGET).map

	$(OBJCOPY) -O binary -S $(TARGET).elf $(TARGET).bin
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis
	$(SIZE) $(TARGET).elf

# You need u-boot-tools to do this, install it on ubuntu just type like this:
# sudo apt install u-boot-tools -y
$(TARGET).img: $(TARGET).bin
	mkimage -C none -A arm -T sunxi_egon -e $(LINK_ADDR) -d $(TARGET).bin $@

setup:
	mkdir -p build/

# build tx core first
tx: setup $(tx_objs) $(sample_objs)
	rm -rf tx.a

	$(AR) -r tx.a $(tx_objs_in_build)

	@echo "#### build tx.a okay"

%.o:%.S
	$(CC) -c -mcpu=arm9 -I../../../../common/inc -I../inc -o build/$(notdir $@) $<
%.o:%.c
	$(CC) -c -mcpu=arm9 -I../../../../common/inc -I../inc -o build/$(notdir $@) $<

.PHONY:clean
clean:
	cd build && \
	rm -rf *.img *.map *.dis *.elf *.o *.bin *.o
