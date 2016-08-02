
##
## Bootstrap and their Corresponding linker definitions were used from various sources like
## IKnow YouTube channel (Very basic)
## OSDev: http://wiki.osdev.org (Very Organized)
##
## So this DEFLINK is used as command line input
## make DEFLINK={OSDEV|IKNOW}
##
DEFLINK = IKNOW

ASSEMBLER = nasm
ASFLAGS = -f elf32

COMPILER = gcc
INC = -I include/
CFLAGS = -m32 -c -ffreestanding
ifeq ($(DEFLINK), OSDEV)
    LINKER_CFLAGS = -D__OS_DEV_LINKER__
else
    LINKER_CFLAGS = -D__IKNOW_LINKER__
endif

LINKER = ld
LDFLAGS = -m elf_i386 -T
ifeq ($(DEFLINK), OSDEV)
    LDSRC = src/linker.ld
else
    LDSRC = src/link.ld
endif


ifeq ($(DEFLINK), OSDEV)
    OBJS = obj/boots.o
else
    OBJS = obj/kasm.o
endif
OBJS += obj/kc.o
OBJS += obj/terminal.o
OBJS += obj/vga.o
OBJS += obj/string.o

OUTPUT = okusha/boot/kernel.bin

run:all


all:$(OBJS)
	mkdir okusha/ -p
	mkdir okusha/boot/ -p
	$(LINKER) $(LDFLAGS) $(LDSRC) -o $(OUTPUT) $(OBJS)

obj/kasm.o:src/kernel.asm
	$(ASSEMBLER) $(ASFLAGS) -o obj/kasm.o src/kernel.asm
	
obj/boots.o:src/bootstrap.asm
	$(ASSEMBLER) $(ASFLAGS) -o obj/boots.o src/bootstrap.asm
	
obj/kc.o:src/kernel.c
	$(COMPILER) $(CFLAGS) $(LINKER_CFLAGS) $(INC) src/kernel.c -o obj/kc.o 
	
obj/terminal.o:src/terminal.c
	$(COMPILER) $(CFLAGS) $(INC) src/terminal.c -o obj/terminal.o 
	
obj/vga.o:src/vga.c
	$(COMPILER) $(CFLAGS) $(INC) src/vga.c -o obj/vga.o 
	
obj/string.o:src/string.c
	$(COMPILER) $(CFLAGS) $(INC) src/string.c -o obj/string.o 
	
build:all
	#Activate the install xorr if you do not have it already installed
	rm okush/boot/grub/ -r -f
	mkdir okusha/boot/grub/ -p
	echo set default=0 >> okusha/boot/grub/grub.cfg
	echo set timeout=0 >> okusha/boot/grub/grub.cfg
	echo menuentry "Okusha" { >> okusha/boot/grub/grub.cfg
	echo         set root='(hd96)' >> okusha/boot/grub/grub.cfg
	echo         multiboot /boot/kernel.bin >> okusha/boot/grub/grub.cfg
	echo } >> okusha/boot/grub/grub.cfg

	grub-mkrescue -o okusha.iso okusha/
	
clean:
	if [ -e obj ]; then rm -f obj/*.o; fi;
	if [ -e okusha ]; then rm -r -f okusha/ ; fi;
	if [ -e okusha.iso ]; then rm okusha.iso; fi;
