ODIR = ../build
SDIR = ./

export _OBJS_ASM = boot.o interrupts_list.o
export _OBJS_C = multiboot.o memory.o base.o interrupts.o keyboard.o processes.o command.o gui.o term.o kernel.o vga.o timer.o
export _OBJS = $(_OBJS_ASM) $(_OBJS_C)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

export ENABLE_GRAPHICS = 1

all:
	make -C asm
	make -C c
	i686-elf-gcc -T linker.ld -o ../build/xos.bin -ffreestanding -O2 -nostdlib $(OBJS)

clean:
	make -C asm clean
	make -C c clean