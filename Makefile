# Flick 0.0x Makefile

# name of Flick kernel
KERNEL_NAME = flick.img	

# C compiler to use
CC = gcc	# changing this may cause problems
CFLAGS = -c -Wall -I./include/	# arguments given to CC

# assembler to use
NASM = nasm	# assembler has to support Intel syntax
NASMFLAGS = -f elf 	# compile to ELF

# linker to use
LD = ld	# we're using the GNU linker
LDFLAGS = -T linker.ld -o $(KERNEL_NAME)  

OBJS = boot/start.o kernel/console.o kernel/main.o kernel/proc.o drivers/keyboard.o drivers/clock.o mm/malloc.o kernel/exception.o mm/i386/misc.o mm/paging.o mm/alloc.o mm/vmm.o mm/bitmap.o boot/setup.o boot/handler.o kernel/mutex.o kernel/kernel.o lib/stdio.o lib/string.o lib/vsprintf.o kernel/misc.o 

all: $(KERNEL_NAME)

install: 
	# this will install the kernel on a floppy
	# you may change this easily
	mount /floppy/
	cp $(KERNEL_NAME) /floppy/
	umount /floppy/

clean:
	rm $(OBJS)

# compile all assembler files
%.o : %.asm
	$(NASM) $(NASMFLAGS) -o $@ $<

# compile all C files
%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

# and link them all together
$(KERNEL_NAME): $(OBJS)	
	$(LD) $(LDFLAGS) $(OBJS)
