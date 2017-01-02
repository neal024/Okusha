
#*******************************************************************************
#
# OKUSHA makefile
#
#*******************************************************************************

##
## ASSEBLER for bootstrap loader
##
ASSEMBLER = as
ASFLAGS = --32
BOOTSTARP_SRC = src/bootstrap.s
#ASSEMBLER = nasm
#ASFLAGS = -f elf32
#BOOTSTARP_SRC = src/bootstrap.asm


##
## COMPILER for kernel binary
##
COMPILER = gcc
INC = -I include/
CFLAGS = -m32 -c -ffreestanding
#CFLAGS += -w	# Disable all warning
#CFLAGS += -nostartfiles
#CFLAGS += -nodefaultlibs
#CFLAGS += -nostdlib
#CFLAGS += -fno-use-cxa-atexit
#CFLAGS += -fno-builtin
#CFLAGS += -fno-rtti
#CFLAGS += -fno-exceptions
#CFLAGS += -fno-leading-underscore
#CFLAGS += -ffreestanding
#CFLAGS += -fno-hosted

##
## Linker for linking bootstarp and kernel binary together
##
LINKER = ld
LDFLAGS = -m elf_i386 -T
LDSRC = src/linker.ld


OBJS = obj/bootstrap.o
OBJS += obj/gdt.o
OBJS += obj/kernel.o
OBJS += obj/terminal.o
OBJS += obj/vga.o
OBJS += obj/string.o

OUTPUT = okusha/boot/kernel.bin
GRUBCFG = okusha/boot/grub/grub.cfg

run:all


all:$(OBJS)
	mkdir okusha/ -p
	mkdir okusha/boot/ -p
	mkdir okusha/boot/grub/ -p
	$(LINKER) $(LDFLAGS) $(LDSRC) -o $(OUTPUT) $(OBJS)

obj/bootstrap.o:$(BOOTSTARP_SRC)
	$(ASSEMBLER) $(ASFLAGS) -o obj/bootstrap.o $(BOOTSTARP_SRC)
	
obj/gdt.o:src/gdt.c
	$(COMPILER) $(CFLAGS) $(INC) src/gdt.c -o obj/gdt.o 
	
obj/kernel.o:src/kernel.c
	$(COMPILER) $(CFLAGS) $(INC) src/kernel.c -o obj/kernel.o 
	
obj/terminal.o:src/terminal.c
	$(COMPILER) $(CFLAGS) $(INC) src/terminal.c -o obj/terminal.o 
	
obj/vga.o:src/vga.c
	$(COMPILER) $(CFLAGS) $(INC) src/vga.c -o obj/vga.o 
	
obj/string.o:src/string.c
	$(COMPILER) $(CFLAGS) $(INC) src/string.c -o obj/string.o 
	
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

clean:
	if [ -e obj ]; then rm -f obj/*.o; else mkdir obj; fi;
	if [ -e okusha ]; then rm -r -f okusha/* ; else mkdir okusha; fi;
	if [ -e okusha.iso ]; then rm okusha.iso; fi;

install:
	sudo cp okusha/boot/kernel.bin /boot/okusha_kernel.bin
