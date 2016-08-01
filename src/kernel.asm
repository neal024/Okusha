bits    32
section         .text               ; Text Segment Starts
        align   4                   ; 32bit OS aligned to 4bytes
        dd      0x1BADB002          ; Magic number to tell that it's a multiboot OS
        dd      0x00
        dd      - (0x1BADB002+0x00)
        
global start            ; Prototype start function as global so that linker can access it.
extern kmain            ; this function is gonna be located in our c code(kernel.c)
start:
        cli             ;clears the interrupts 
        call kmain      ;send processor to continue execution from the kamin funtion in c code
        hlt             ;halt the cpu(pause it from executing from this address
