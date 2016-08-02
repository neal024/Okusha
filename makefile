
ASSEMBLER = nasm
ASFLAGS = -f elf32

COMPILER = gcc
INC = -I include/
CFLAGS = -m32 -c -ffreestanding

LINKER = ld
LDFLAGS = -m elf_i386 -T src/link.ld


OBJS = obj/kasm.o
OBJS += obj/kc.o

OUTPUT = okusha/boot/kernel.bin

run:all
    $(EMULATOR) $(EMULATOR_FLAGS) $(OUTPUT)

all:$(OBJS)
	mkdir okusha/ -p
	mkdir okusha/boot/ -p
	$(LINKER) $(LDFLAGS) -o $(OUTPUT) $(OBJS)

obj/kasm.o:src/kernel.asm
	$(ASSEMBLER) $(ASFLAGS) -o obj/kasm.o src/kernel.asm
	
obj/kc.o:src/kernel.c
	$(COMPILER) $(CFLAGS) $(INC) src/kernel.c -o obj/kc.o 
	
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
	rm -f obj/*.o
	rm -r -f okusha/
