rwildcard = $(foreach d, $(wildcard $1*), $(filter $(subst *, %, $2), $d) $(call rwildcard, $d/, $2))

CC := i686-elf-gcc
AS := i686-elf-as
QEMU := qemu-system-i386

warnings := -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter
CFLAGS = -std=c11 -O2 $(warnings) -ffreestanding
LDFLAGS = -O2 -ffreestanding -nostdlib -lgcc

dir_object := obj

sources := $(call rwildcard, */, *.c *.s)
objects := $(patsubst %.s, $(dir_object)/%.o, $(patsubst %.c, $(dir_object)/%.o, $(sources)))  # TODO: Find a way to make this shorter

.PHONY: all
all: kernel.img

.PHONY: clean
clean:
	@rm -rf $(dir_object) kernel.img
	@echo "Clean!"

.PHONY: run
run: kernel.img
	@$(QEMU) -kernel $<

.PHONY: wno
wno: warnings := 
wno: all

kernel.img: CFLAGS := $(CFLAGS) -I libc/free/
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
