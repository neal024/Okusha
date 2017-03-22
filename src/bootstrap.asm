;******************************************************************************;
; Taken from OSDev: http://wiki.osdev.org/Bare_Bones_with_NASM
;******************************************************************************;

[BITS 32]
ALIGN 4

;******************************************************************************;
; Declare constants for the multiboot header.
;******************************************************************************;
__mboot:
	MULTIBOOT_PAGE_ALIGN	equ	1<<0	; align loaded modules on page boundaries
	MULTIBOOT_MEMORY_INFO	equ	1<<1	; provide memory map
	MULTIBOOT_AOUT_KLUDGE	equ	1<<16	; Set if Executable Format is Other than ELF
	MULTIBOOT_HEADER_MAGIC	equ	0x1BADB002
	MULTIBOOT_HEADER_FLAGS	equ (MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE)
	MULTIBOOT_CHECKSUM      equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

	EXTERN code
	EXTERN bss
	EXTERN end

	;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
	; This is the GRUB Multiboot header. A boot signature
	; Declare a multiboot header that marks the program as a kernel.
	; These are magic values that are documented in the multiboot standard.
	; The bootloader will search for this signature in the first
	; 8 KiB (8192bytes) of the kernel file, aligned at a 32-bit boundary.
	; The signature is in its own section so the header can be forced to be
	; within the first 8 KiB of the kernel file.
	;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_CHECKSUM

	; AOUT kludge - must be physical addresses. Make a note of these:
	; The linker script fills in the data for these ones!
	dd __mboot
	dd code
	dd bss
	dd end
	dd __start


extern kernel_main
extern printf
extern puts
global __start

__start:

	mov esp, __sys_stack	; This points the stack to our new stack area

	push eax
	push ebx

    call kernel_main

	push eax
	push $__exit_msg
	call printf

	jmp  __stop

__stop:
	push $__halt_msg	; Push Halt Message into stack
	call puts			; Call print function
	cli
	hlt					; System Will Be Halted Here
	jmp $				; $ is current instruction. Loop Over Current Instruction.

__exit_msg db "ERROR: Kernel Returned %u. Bye!", 0x0A, 0	; Message - Line-Break - NULL
__halt_msg db 0x0A, "System Halted...", 0x0A, 0

; This will set up our new segment registers. We need to do
; something special in order to set CS. We do what is called a
; far jump. A jump that includes a segment as well as an offset.
; This is declared in C as 'extern void gdt_flush();'
global gdt_flush
extern gdt_p
gdt_flush:
    lgdt [gdt_p]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:__flush2
__flush2:
    ret

; Loads the IDT defined in 'idt_p' into the processor.
; This is declared in C as 'extern void idt_load();'
global idt_load
extern idt_p
idt_load:
    lidt [idt_p]
    ret

;
; INCLUDE Interrupt Service Request Stubs
; INCLUDE Interrupt Request Stubs
;
%include "src/isrs.asm"
%include "src/irqs.asm"


; Here is the definition of our BSS section. Right now, we'll use
; it just to store the stack. Remember that a stack actually grows
; downwards, so we declare the size of the data before declaring
; the identifier '__sys_stack'
SECTION .bss
    resb 8192               ; This reserves 8KBytes of memory here
__sys_stack:

