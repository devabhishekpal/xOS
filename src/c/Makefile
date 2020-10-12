CC = i686-elf-gcc
BUILD_TIMESTAMP := $(shell TZ='Asia/Kolkata' date +"%D %r")
BUILD_NUMBER := $(shell ../build_num.sh)
FLAGS = -std=gnu99 -ffreestanding -Wall -Wextra -Wno-int-to-pointer-cast -DBUILD_TIMESTAMP='$(BUILD_TIMESTAMP)' -DBUILD_NUMBER='$(BUILD_NUMBER)' -isystem../system-include -I../include -c
ODIR = ../../build


_OBJS = $(_OBJS_C)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))
SRC = $(_OBJS:%.o=%.c)

FLAGS := $(FLAGS) -DD_ENABLE_GRAPHICS=$(ENABLE_GRAPHICS)

all: $(OBJS)


../../build/%.o: %.c
	$(CC) -o $(ODIR)/$(subst .c,.o,$^) $^ $(FLAGS)

clean:
	$(shell ../build_num_subtract.sh)