#Clear the build environment first
make clean
#Do make
make
#Run with Qemu
qemu-system-i386 -kernel okusha/boot/kernel.bin
