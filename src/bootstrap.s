
/* Multiboot Segment */
.set MAGIC, 0x1BADB002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
	.long MAGIC
	.long FLAGS
	.long CHECKSUM


/* Code Segment */
.section .text
.extern kernel_main
.extern puts
.extern printf
.global _start_bootstrap_loader

_start_bootstrap_loader:
	mov $kernel_stack, %esp /* set kernel stack to stack pointer register */
	/*
	 * here you need to call the initializer function
	 * which will initialize the constructor section defined here
	 */
	push %eax			/* get multiboot structure location in RAM from AX register
						 * and push to kernel stack */
	push %ebx			/* get multiboot MAGIC number from BX reister
						 * and push to kernel stack */
	call kernel_main	/* Actual kernel call() */

	/*
     * We are out of kernel. Give some message.
     */
    push %eax			/* Return from kernel stored in AX register
						 * Push it to stack */
	push $exit_msg		/* Push Exit Message into stack */
	call printf			/* Call print function */
	push $halt_msg		/* Push Halt Message into stack */
	call puts			/* Call print function */

_stop:
	cli
	hlt
	jmp _stop

exit_msg:
	.asciz	"\nKernel Returned %u. Bye!\n"
halt_msg:
	.asciz  "System Halted..."

/* Stack Segment */
.section .bss
.space 2*1024*1024	/* 2Mb */
kernel_stack:
