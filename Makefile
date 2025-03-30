.PHONY: clean all debug qemu qemu-gdb tags doc

#####################
# List of Variables #
#####################

VERBOSE      = @
KERNEL       = boot/system
SYM_MAP      = boot/symbol_map
IMAGE        = boot/kernel8.img
LINKER       = boot/sections.ld
DTB          = boot/rpi3.dtb

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

DEFINEFLAGS = $(shell ./scripts/config)

STANDALONEFLAGS = -ffreestanding -fno-builtin -nostartfiles -nodefaultlibs \
				  -nostdlib -nostdinc -fno-pie -no-pie -fno-omit-frame-pointer \
				  -mgeneral-regs-only -fno-exceptions -fno-rtti \
				  -fno-stack-protector -mno-omit-leaf-frame-pointer \
				  -mno-outline-atomics -fno-use-cxa-atexit

OPTFLAGS = -mcpu=cortex-a53 -march=armv8-a -I inc/ -std=c++17

CCFLAGS = $(WARNFLAGS) $(OPTFLAGS) $(STANDALONEFLAGS) $(FLOATINGPOINT) $(DEFINEFLAGS)

ASM = aarch64-linux-gnu-g++
ASMFLAGS = -c

##########
# Linker #
##########

LD = aarch64-linux-gnu-g++
LDFLAGS = -nostartfiles -T $(LINKER)

########
# QEMU #
########

QEMU = qemu-system-aarch64
QEMUFLAGS = -machine raspi3 -m 1G -smp 4 -serial vc -serial vc -kernel $(IMAGE) -dtb $(DTB)

#######
# GDB #
#######
GDB = aarch64-linux-gnu-gdb
GDBFLAGS = -ex "target remote :1234"

###########
# doxygen #
###########

DOXYGENCONFG = doc/Doxyfile
DOXYGENBUILD = doc/build
DOXYGENTARGET = $(DOXYGENBUILD)/index.html


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
	$(VERBOSE) $(LD) $(LDFLAGS) -o $(KERNEL) $^ -lgcc

$(IMAGE): $(KERNEL) $(SYM_MAP)
	@echo "OBJCOPY		$(KERNEL)"
	$(VERBOSE)  aarch64-linux-gnu-objcopy --update-section map=$(SYM_MAP) $(KERNEL)
	$(VERBOSE)  aarch64-linux-gnu-objcopy $(KERNEL) -O binary $(IMAGE)

all: $(IMAGE)

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

$(SYM_MAP): $(KERNEL)
	@echo "SYMBOLS		$(SYM_MAP)"
	$(VERBOSE) ./scripts/symbol_map $(KERNEL) $(SYM_MAP)

doc:
	@echo "DOC		$(DOXYGENTARGET)"
	$(VERBOSE) doxygen $(DOXYGENCONFG)
