.PHONY: clean all debug qemu qemu-gdb tags

#####################
# List of Variables #
#####################

VERBOSE  = @
KERNEL   = boot/system
LINKER   = boot/sections.ld
DTB      = boot/rpi3.dtb

#########################
# List of Prerequisites #
#########################

CC_SOURCES = $(shell find . -name "*.cc")
S_SOURCES = $(shell find . -name "*.S")
CC_OBJECTS = $(CC_SOURCES:.cc=.o)
S_OBJECTS = $(S_SOURCES:.S=.o)
O_OBJECTS = $(shell find . -name "*.o")

############
# Compiler #
############

CC = aarch64-linux-gnu-g++

WARNFLAGS = -Wall -Wextra -Werror

STANDALONEFLAGS = -ffreestanding -fno-builtin -nostartfiles -nodefaultlibs \
				  -nostdlib -nostdinc -fno-pie -no-pie -fno-omit-frame-pointer \
				  -mgeneral-regs-only -fno-exceptions -fno-rtti \
				  -fno-stack-protector -mno-omit-leaf-frame-pointer

OPTFLAGS = -mcpu=cortex-a53 -march=armv8-a -I inc/ -std=c++17

CCFLAGS = $(WARNFLAGS) $(OPTFLAGS) $(STANDALONEFLAGS) $(FLOATINGPOINT) $(DEFINEFLAGS)

ASM = aarch64-linux-gnu-g++
ASMFLAGS = -c

##########
# Linker #
##########

LD = aarch64-linux-gnu-ld
LDFLAGS = -nostartfiles -T $(LINKER)

########
# QEMU #
########

QEMU = qemu-system-aarch64
QEMUFLAGS = -machine raspi3 -m 1G -smp 4 -serial vc -kernel $(KERNEL) -dtb $(DTB)

#######
# GDB #
#######
GDB = aarch64-linux-gnu-gdb
GDBFLAGS = -ex "target remote :1234"

#################
# Generic Rules #
#################

%.o: %.cc
	@echo "CC		$@"
	$(VERBOSE) $(CC) $(CCFLAGS) -c $^ -o $@

%.o: %.S
	@echo "ASM		$@"
	$(VERBOSE) $(ASM) $(ASMFLAGS) $^ -o $@

###########
# Targets #
###########

%-verbose:
	@make --no-print-directory $* VERBOSE=""

$(KERNEL): $(CC_OBJECTS) $(S_OBJECTS)
	@echo "LD		$(KERNEL)"
	$(VERBOSE) $(LD) $(LDFLAGS) -o $(KERNEL) $^

all: $(KERNEL)

qemu: debug
	@echo "QEMU"
	$(VERBOSE) $(QEMU) $(QEMUFLAGS)

qemu-gdb: debug
	@echo "GDB"
	$(VERBOSE) $(QEMU) $(QEMUFLAGS) -s -S &
	@sleep 1
	$(VERBOSE) $(GDB) $(KERNEL) $(GDBFLAGS)

debug:
	@$(MAKE) --no-print-directory all CCFLAGS="$(CCFLAGS) -O0 -g"

clean:
	@echo "RM"
	$(VERBOSE) rm -rf $(O_OBJECTS) $(KERNEL)

tags:
	@echo "TAGS"
	$(VERBOSE) ctags -R .
