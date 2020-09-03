ODIR = ../../build

ASM = nasm
FLAGS = -felf32

_OBJS = $(_OBJS_ASM)
_OBJS = $(patstubst %, $(ODIR)/%, $(_OBJS))
SRC = $(_OBJS:%.o = %.asm)

all: $(OBJS)

../../build/%.o: %.asm
	$(ASM) -o $(ODIR)/$(subst .asm, .o, $^) $^ $(FLAGS)

clean: