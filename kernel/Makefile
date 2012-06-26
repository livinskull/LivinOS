# LivinOS Makefile

SOURCES = $(shell find -name '*.[cs]')
OBJS = $(addsuffix .o,$(basename $(SOURCES)))

CFLAGS=-m32 -c -ffreestanding -nostdinc -nostdlib -fno-builtin -fno-stack-protector -Wall -Wextra -Iinclude
LDFLAGS=-melf_i386 -Tlink.ld
ASFLAGS=-felf32 -w+orphan-labels


all: $(OBJS)
	ld $(LDFLAGS) -o kernel $^

%.o: %.c
	gcc $(CFLAGS) $^

%.o: %.s
	nasm $(ASFLAGS) $^ 


clean:
	rm $(OBJS)
	
.PHONY: all clean
