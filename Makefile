rwildcard = $(foreach d, $(wildcard $1*), $(filter $(subst *, %, $2), $d) $(call rwildcard, $d/, $2))

CC := i686-elf-gcc
AS := i686-elf-as
QEMU := qemu-system-i386
BOCHS := bochs

warnings := -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable
CFLAGS = -std=c11 -O2 $(warnings) -ffreestanding -I .
LDFLAGS = -O2 -ffreestanding -nostdlib -lgcc

dir_object := obj
dir_test_assembly := test_assembly

sources := $(call rwildcard, */, *.c *.s)
objects := $(patsubst %.s, $(dir_object)/%.o, $(patsubst %.c, $(dir_object)/%.o, $(sources)))  # TODO: Find a way to make this shorter
test_assembly := $(patsubst %.c, $(dir_test_assembly)/%.s, $(call rwildcard, */, *.c))

.PHONY: all
all: kernel.img

.PHONY: clean
clean:
	@rm -rf $(dir_object) $(dir_test_assembly) kernel.img cdroot/boot/kernel.img wafflos.iso
	@echo "Clean!"

.PHONY: run
run: kernel.img
	@$(QEMU) -kernel $<

.PHONY: bochs
bochs: wafflos.iso
	@$(BOCHS) -f bochsrc

.PHONY: wno
wno: warnings :=
wno: all

.PHONY: assembly
assembly: CFLAGS := $(CFLAGS) -I kernel/include/ -I libc/free/
assembly: $(test_assembly)

wafflos.iso: kernel.img
	@cp kernel.img cdroot/boot/kernel.img
	@grub-mkrescue -o wafflos.iso cdroot

kernel.img: CFLAGS := $(CFLAGS) -I kernel/include/ -I libc/free/
kernel.img: $(filter-out $(dir_object)/kernel/* $(dir_object)/libc/free/*, $(objects))
	@echo "[$(CC)] Linking $@"
	@$(LINK.o) -T kernel/linker.ld $(OUTPUT_OPTION) $^

$(dir_object)/%.o: %.s
	@mkdir -p "$(@D)"
	@echo "[$(AS)] Compiling $<"
	@$(AS) $(OUTPUT_OPTION) $<

$(dir_object)/%.o: %.c
	@mkdir -p "$(@D)"
	@echo "[$(CC)] Compiling $<"
	@$(COMPILE.c) $(OUTPUT_OPTION) $<

$(dir_test_assembly)/%.s: %.c
	@mkdir -p "$(@D)"
	@echo "[$(CC)] Compiling $<"
	@$(COMPILE.c) $(OUTPUT_OPTION) -S -fverbose-asm $<
