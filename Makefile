rwildcard = $(foreach d, $(wildcard $1*), $(filter $(subst *, %, $2), $d) $(call rwildcard, $d/, $2))

# Programs
CC := i686-elf-gcc
AS := i686-elf-as
GRUB_MKRESCUE := grub-mkrescue
QEMU := qemu-system-i386
BOCHS := bochs
GDB := gdb

# Flags
CFLAGS := -g -std=c11 -O2 -Wall -Wextra -I . -I libc/free/ -I libc/hosted/
LDFLAGS := -O2

# Output files
kernel := kernel.img
iso := wafflos.iso

# Directories
dir_object := obj
dir_cd := .cdroot

# Files
sources := $(call rwildcard, */, *.c *.s)
objects := $(patsubst %.s, $(dir_object)/%.o, $(patsubst %.c, $(dir_object)/%.o, $(sources)))

$(dir_object)/%.o: %.s
	@mkdir -p "$(@D)"
	@echo "[$(AS)] Assembling $<"
	@$(COMPILE.s) $(OUTPUT_OPTION) $<

$(dir_object)/%.o: %.c
	@mkdir -p "$(@D)"
	@echo "[$(CC)] Compiling $<"
	@$(COMPILE.c) $(OUTPUT_OPTION) $<

$(kernel): CFLAGS += -ffreestanding
$(kernel): $(filter-out $(dir_object)/kernel/* $(dir_object)/libc/free/*, $(objects))
	@echo "[$(CC)] Linking $@"
	@$(LINK.o) -T kernel/linker.ld -ffreestanding -nostdlib -lgcc $(OUTPUT_OPTION) $^

$(iso): $(kernel)
	@cp $(kernel) $(dir_cd)/boot/$(kernel)
	@$(GRUB_MKRESCUE) -o $@ $(dir_cd)

.PHONY: all
all: $(kernel)

.PHONY: clean
clean:
	@echo "Clean!"
	@rm -rf $(dir_object) $(kernel) $(iso) $(dir_cd)/boot/$(kernel)

# Emulators, debuggers, other external programs
.PHONY: qemu
qemu: $(kernel)
	@$(QEMU) -kernel $(kernel)

.PHONY: gdb
gdb: $(kernel)
	@$(QEMU) -S -s -kernel $(kernel) &
	@$(GDB)

.PHONY: bochs
bochs: $(iso)
	@$(BOCHS) -f .bochsrc -q
