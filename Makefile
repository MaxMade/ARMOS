.PHONY: clean all debug qemu qemu-gdb tags user-doc kernel-doc doc app_objects_prefixed style-*

#####################
# List of Variables #
#####################

VERBOSE      = @
KERNEL       = boot/system
SYM_MAP      = boot/symbol_map
IMAGE        = boot/kernel8.img
LINKER       = boot/sections.ld
DTB          = boot/rpi3.dtb
APP          = busy_loop
LOGS         = serial0.log serial1.log

#########################
# List of Prerequisites #
#########################

CC_SOURCES = $(shell find . -name "*.cc" -not -path "./apps/*")
S_SOURCES = $(shell find . -name "*.S" -not -path "./apps/*")
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

OPTFLAGS = -mcpu=cortex-a53 -march=armv8-a -std=c++17

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
QEMUFLAGS = -machine raspi3 -m 1G -smp 4 \
			-chardev vc,id=char0,logfile=serial0.log -chardev vc,id=char1,logfile=serial1.log \
			-serial chardev:char0 -serial chardev:char1 \
			-kernel $(IMAGE) -dtb $(DTB)

###########
# CPPLINT #
###########

CPPLINTFLAGS =  --linelength=120 \
				--recursive \
				--extensions=h,cc \
				--quiet \
				--filter=-whitespace/tab,-legal/copyright,-build/include_order,-build/header_guard,-build/namespaces,-runtime/int

#######
# GDB #
#######

GDB = aarch64-linux-gnu-gdb
GDBFLAGS = -ex "target remote :1234"

###########
# doxygen #
###########

DOXYGEN_CONFG_BASE = doc/Doxyfile

########
# Apps #
########

include apps/$(APP)/Makefile
APP_LIBS = $(shell find ./apps/lib -name "*.cc")
APP_OBJECTS = $(APP_LIBS:.cc=.o) $(addprefix apps/$(APP)/, $(OBJS))

#################
# Generic Rules #
#################

apps/%.o: apps/%.cc
	@echo "CC		$@"
	$(VERBOSE) $(CC) $(CCFLAGS) -I apps/lib/ -c $^ -o $@

%.o: %.cc
	@echo "CC		$@"
	$(VERBOSE) $(CC) $(CCFLAGS) -I inc/ -I ./ -c $^ -o $@

%.o: %.S
	@echo "ASM		$@"
	$(VERBOSE) $(ASM) $(ASMFLAGS) $^ -o $@

###########
# Targets #
###########

%-verbose:
	@$(MAKE) --no-print-directory $* VERBOSE=""

all: app_objects_prefixed $(IMAGE)

$(KERNEL): $(CC_OBJECTS) $(S_OBJECTS) $(APP_OBJECTS)
	@echo "LD		$(KERNEL)"
	$(VERBOSE) $(LD) $(LDFLAGS) -o $(KERNEL) $^ -lgcc

$(IMAGE): $(KERNEL) $(SYM_MAP)
	@echo "OBJCOPY		$(KERNEL)"
	$(VERBOSE)  aarch64-linux-gnu-objcopy --update-section map=$(SYM_MAP) $(KERNEL)
	$(VERBOSE)  aarch64-linux-gnu-objcopy $(KERNEL) -O binary $(IMAGE)

app_objects_prefixed: $(APP_OBJECTS)
	@echo "OBJCOPY"
	$(VERBOSE) for i in $^; do aarch64-linux-gnu-objcopy --prefix-sections=app $${i} $${i}; done

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
	$(VERBOSE) rm -rf $(shell find -name "*.o") $(KERNEL) $(IMAGE) $(SYM_MAP) $(LOGS)

tags:
	@echo "TAGS"
	$(VERBOSE) ctags -R .

$(SYM_MAP): $(KERNEL)
	@echo "SYMBOLS		$(SYM_MAP)"
	$(VERBOSE) ./scripts/symbol_map $(KERNEL) $(SYM_MAP)

user-doc:
	@echo "DOC		$(DOXYGEN_CONFG_BASE)_User"
	$(VERBOSE) doxygen $(DOXYGEN_CONFG_BASE)_User

kernel-doc:
	@echo "DOC		$(DOXYGEN_CONFG_BASE)_Kernel"
	$(VERBOSE) doxygen $(DOXYGEN_CONFG_BASE)_Kernel

doc: user-doc kernel-doc

style-%: %
	@echo "STYLE		$^"
	$(VERBOSE) cpplint $(CPPLINTFLAGS) $^
