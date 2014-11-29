CC = i686-elf-gcc
AS = i686-elf-as
QEMU = qemu-system-i386

CFLAGS = -std=c11 -O2 -Wall -Wextra -Werror -ffreestanding 
LDFLAGS = -O2 -ffreestanding -nostdlib -lgcc

dir_object = ./obj

sources = $(wildcard **/*.c **/*.s)
objects = $(patsubst %.s, $(dir_object)/%.o, $(patsubst %.c, $(dir_object)/%.o, $(sources)))  # TODO: Find a way to make this shorter

.PHONY: all
all: kernel.img

.PHONY: clean
clean:
	@rm -rf $(dir_object) kernel.img
	@echo "Clean!"

.PHONY: run
run: kernel.img
	@$(QEMU) -kernel $<

kernel.img: $(filter-out $(dir_object)/kernel/*, $(objects))
	@echo "[$(CC)] Linking $@"
	@$(LINK.o) -T kernel/linker.ld $(OUTPUT_OPTION) $^

$(dir_object)/%.o: %.s
	@mkdir -p $(shell dirname $@)
	@echo "[$(AS)] Compiling $<"
	@$(AS) $(OUTPUT_OPTION) $<
$(dir_object)/%.o: %.c
	@mkdir -p $(shell dirname $@)
	@echo "[$(CC)] Compiling $<"
	@$(COMPILE.c) $(OUTPUT_OPTION) $<
