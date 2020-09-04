ODIR = ../build
SDIR = ./

_OBJS_ASM = boot.o interrupts_list.o
_OBJS_C = memory.o base.o interrupts.o keyboard.o processes.o command.o gui.o term.o kernel.o
_OBJS = $(_OBJS_ASM) $(_OBJS_C)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

export _OBJS_ASM
export _OBJS_C

all:
	make -C asm
	make -C c
	i686-elf-gcc -T linker.ld -o ../build/xos.bin -ffreestanding -O2 -nostdlib $(OBJS)

clean:
	make -C asm clean
	make -C c clean