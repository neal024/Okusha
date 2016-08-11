/******************************************************************************
 *
 * OKUSHA:	Kernel Code
 *			Get your Hands Dirty!
 *
 ******************************************************************************/

#include <terminal.h>
#include <multiboot.h>

/*
 * src/bootstrap.s or src/bootstrap.asm
 * EAX Pushed First to Stack
 * EBX Pushed Second to Stack
 *
 * Order of kernel_main parameters are dependent on
 * How we have pushed parameters to Stack
 */
uint32_t
kernel_main(void *mb_info_addr, uint32_t mb_hdr_magic)
{
    /* Initialize terminal interface */
    terminal_init();

    puts("OKUSHA: Learning OS from scratch!\n");

	printf("Multiboot Loader Magic: 0x%x\n", mb_hdr_magic);
	printf("Multiboot Sector in RAM: (%p)\n", mb_info_addr);

	return (69);
}
