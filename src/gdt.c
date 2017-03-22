#include <gdt.h>

/* This is in start.asm. We use this to properly reload
 * the new segment registers
 */
extern void gdt_flush();

/* Our GDT, with 3 entries, and finally our special GDT pointer */
struct gdt_entry gdt[SEG_MAX];
struct gdt_ptr   gdt_p;

/* Setup a Descriptor in The Global Descriptor Table */
void gdt_set_entry(uint8_t seg_num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{

	/* Setup the descriptor base access */
	gdt[seg_num].base_low    = (base & 0xFFFF);
	gdt[seg_num].base_middle = (base >> 16) & 0xFF;
	gdt[seg_num].base_high   = (base >> 24) & 0xFF;

	/* Setup the descriptor limits */
	gdt[seg_num].limit_low   = (limit & 0xFFFF);
	gdt[seg_num].granularity = ((limit >> 16) & 0x0F);

	/* Finally, set up the granularity and access flags */
	gdt[seg_num].granularity |= (granularity & 0xF0);
	gdt[seg_num].access       = access;
}

/*
 * Initialize The Global Descriptor Table
 * Should be called by kernel_main. This will setup the special GDT
 * pointer, set up the first 3 entries in our GDT, and then
 * finally call gdt_flush() in our assembler file in order
 * to tell the processor where the new GDT is and update the
 * new segment registers
 */
void gdt_install(void)
{
    /* Setup the GDT pointer and limit */
    gdt_p.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gdt_p.base  = (uint32_t)&gdt;

    /* Our NULL descriptor */
	gdt_set_entry(SEG_NULL, 0, 0, 0, 0);

    /* The second entry is our Code Segment. The base address
    *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
    *  uses 32-bit opcodes, and is a Code Segment descriptor.
    *  Please check the table above in the tutorial in order
    *  to see exactly what each value means */
	//gdt_set_entry(SEG_CODE, 0, 64*1024*1024, 0x9A, 0xCF);	/* Kernel Code Segment */
	gdt_set_entry(SEG_CODE, 0, 0xFFFFFFFF, 0x9A, 0xCF);	/* Kernel Code Segment */

    /* The third entry is our Data Segment. It's EXACTLY the
    *  same as our code segment, but the descriptor type in
    *  this entry's access byte says it's a Data Segment */
	//gdt_set_entry(SEG_DATA, 0, 64*1024*1024, 0x92, 0xCF);	/* Kernel Data Segment */
	gdt_set_entry(SEG_DATA, 0, 0xFFFFFFFF, 0x92, 0xCF);	/* Kernel Data Segment */

	//gdt_set_entry(SEG_USR1, 0, 64*1024*1024, 0xFA, 0xCF);	/* User-Mode Segment 1 */
	//gdt_set_entry(SEG_USR2, 0, 64*1024*1024, 0xF2, 0xCF);	/* User-Mode Segment 2 */
	//gdt_set_entry(SEG_TSS, 0, 0, 0, 0);

    /* Flush out the old GDT and install the new changes! */
    gdt_flush();
}
