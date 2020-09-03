CC = i686-elf-gcc
BUILD_TIMESTAMP := $(shell TZ='Asia/Kolkata' date +"%D %r")
BUILD_NUMBER := $(shell ../build_num.sh)

FLAGS = -std=gnu99 -ffreestanding -Wall -Wextra -DBUILD_TIMESTAMP='$(BUILD_TIMESTAMP)' -DBUILD_NUMBER='$(BUILD_NUMBER)' -I../include -c

ODIR = ../../build
_OBJS = $(_OBJS_C)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))
SRC = $(_OBJS:%.o=%.c)

all: $(OBJS)
../../build/%.o: %.c
	$(CC) -o $(ODIR)/$(subst .c,.o,$^) $^ $(FLAGS)

clean:
	$(shell ./build_num_reduce.sh)