
#include<terminal.h>
#include<multiboot.h>
#include<gdt.h>
#include<idt.h>
#include<isrs.h>
#include<irq.h>
#include<timer.h>
#include<kb.h>

/*
 * src/bootstrap.s or src/bootstrap.asm
 * EAX Pushed First to Stack
 * EBX Pushed Second to Stack
 *
 * Order of kernel_main parameters are dependent on
 * How we have pushed parameters into Stack
 */
uint32_t
kernel_main(void *mb_info_addr, uint32_t mb_hdr_magic)
{
    /*
	 * Initialize terminal interface
	 */
    terminal_init();

	/*
	 * Welcome Message
	 */
    puts("OKUSHA: Learning OS from scratch!\n");
	printf("Multiboot Loader Magic  : 0x%x\n", mb_hdr_magic);
	printf("Multiboot Sector in RAM : %p\n", mb_info_addr);

	/*
	 * Manage Descriptor Tables
	 */
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();

	/*
	 * Configure Interrupts Handlers
	 */
    timer_install();
    keyboard_install();

	/*
	 * Start Interrupt
	 */
    __asm__ __volatile__ ("sti");


	/* Division-By-Zero *///int O = 10/0;

	/*
	 * Kernel-Loop
	 */
	while(1);

	return (69);
}
