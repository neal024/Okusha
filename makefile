
#*******************************************************************************
#
# OKUSHA makefile
#
#*******************************************************************************

##
## ASSEBLER for bootstrap loader
##
#ASSEMBLER = as
#ASFLAGS = --32
#BOOTSTARP_SRC = src/bootstrap.s

ASSEMBLER = nasm
ASFLAGS = -f elf32
BOOTSTARP_SRC = src/bootstrap.asm

##
## Linker for linking bootstarp and kernel binary together
##
LINKER = ld
LDFLAGS = -m elf_i386 -T
#LDSRC = src/linker_gas.ld
LDSRC = src/linker_nasm.ld


##
## COMPILER for kernel binary
##
COMPILER = gcc
INC = -I include/
CFLAGS  = -m32 -c
#CFLAGS += -Wall -O
#CFLAGS += -fstrength-reduce
#CFLAGS += -fomit-frame-pointer
#CFLAGS += -finline-functions
CFLAGS += -ffreestanding
CFLAGS += -fno-builtin
#CFLAGS += -fno-exceptions
#CFLAGS += -fno-hosted
#CFLAGS += -fno-leading-underscore
#CFLAGS += -fno-rtti
CFLAGS += -fno-stack-protector
#CFLAGS += -fno-use-cxa-atexit  #for C++
CFLAGS += -nodefaultlibs
CFLAGS += -nostartfiles
CFLAGS += -nostdinc
CFLAGS += -nostdlib


OBJS  = obj/bootstrap.o
OBJS += obj/string.o
OBJS += obj/terminal.o
OBJS += obj/gdt.o
OBJS += obj/idt.o
OBJS += obj/irq.o
OBJS += obj/isrs.o
OBJS += obj/kb.o
OBJS += obj/timer.o
OBJS += obj/kernel.o

OUTPUT  = okusha/boot/kernel.bin
GRUBCFG = okusha/boot/grub/grub.cfg

all:$(OBJS)
	mkdir okusha/ -p
	mkdir okusha/boot/ -p
	mkdir okusha/boot/grub/ -p
	$(LINKER) $(LDFLAGS) $(LDSRC) -o $(OUTPUT) $(OBJS)
	rm -rf obj/*

obj/bootstrap.o:$(BOOTSTARP_SRC)
	$(ASSEMBLER) $(ASFLAGS) -o obj/bootstrap.o $(BOOTSTARP_SRC)

obj/string.o:src/string.c
	$(COMPILER) $(CFLAGS) $(INC) src/string.c -o obj/string.o

obj/terminal.o:src/terminal.c
	$(COMPILER) $(CFLAGS) $(INC) src/terminal.c -o obj/terminal.o

obj/gdt.o:src/gdt.c
	$(COMPILER) $(CFLAGS) $(INC) src/gdt.c -o obj/gdt.o

obj/idt.o:src/idt.c
	$(COMPILER) $(CFLAGS) $(INC) src/idt.c -o obj/idt.o

obj/irq.o:src/irq.c
	$(COMPILER) $(CFLAGS) $(INC) src/irq.c -o obj/irq.o

obj/isrs.o:src/isrs.c
	$(COMPILER) $(CFLAGS) $(INC) src/isrs.c -o obj/isrs.o

obj/kb.o:src/kb.c
	$(COMPILER) $(CFLAGS) $(INC) src/kb.c -o obj/kb.o

obj/timer.o:src/timer.c
	$(COMPILER) $(CFLAGS) $(INC) src/timer.c -o obj/timer.o

obj/kernel.o:src/kernel.c
	$(COMPILER) $(CFLAGS) $(INC) src/kernel.c -o obj/kernel.o

build:all
	#Activate the install xorr if you do not have it already installed
	rm okush/boot/grub/ -r -f
	echo "set timeout=0"					>  $(GRUBCFG)
	echo "set default=0"					>> $(GRUBCFG)
	echo "menuentry "Okusha" {"				>> $(GRUBCFG)
	echo "	set root='(hd96)'"				>> $(GRUBCFG)
	echo "	multiboot /boot/kernel.bin"		>> $(GRUBCFG)
	echo "	boot"							>> $(GRUBCFG)
	echo "}"								>> $(GRUBCFG)

	grub-mkrescue -o okusha.iso okusha/


run:build
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm 'Okusha' ./okusha.iso


clean:
	if [ -e obj ]; then rm -f obj/*.o; else mkdir obj; fi;
	if [ -e okusha ]; then rm -r -f okusha/* ; else mkdir okusha; fi;
	if [ -e okusha.iso ]; then rm okusha.iso; fi;

install:
	sudo cp okusha/boot/kernel.bin /boot/okusha_kernel.bin
