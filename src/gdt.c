#include <gdt.h>

struct gdt_entry gdt[SEG_MAX];	/* null, unused, code, data */
struct gdt_ptr gdt_p;

#ifndef __LINUX_GDT__
/*
 * Setup a Descriptor in Global Descriptor Table
 */
void gdt_set_entry(uint8_t seg_num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{

	/* Setup the descriptor base access */
	gdt[seg_num].base_low = (base & 0xFFFF);
	gdt[seg_num].base_middle = (base >> 16) & 0xFF;
	gdt[seg_num].base_high = (base >> 24) & 0xFF;

	/* Setup the descriptor limits */
	gdt[seg_num].limit_low = (limit & 0xFFFF);
	gdt[seg_num].granularity = ((limit >> 16) & 0x0F);

	/* Finally, set up the granularity and access flags */
	gdt[seg_num].granularity |= (granularity & 0xF0);
	gdt[seg_num].access = access;
}

/*
 * Initialize The Global Descriptor Table
 */
void gdt_init(void)
{
	
	gdt_set_entry(SEG_NULL, 0, 0, 0, 0);					/* NULL descriptor */
	gdt_set_entry(SEG_CODE, 0, 64*1024*1024, 0x9A, 0xCF);	/* Kernel Code Segment */
	gdt_set_entry(SEG_DATA, 0, 64*1024*1024, 0x92, 0xCF);	/* Kernel Data Segment */
	gdt_set_entry(SEG_USR1, 0, 64*1024*1024, 0xFA, 0xCF);	/* User-Mode Segment 1 */
	gdt_set_entry(SEG_USR2, 0, 64*1024*1024, 0xF2, 0xCF);	/* User-Mode Segment 2 */
	//gdt_set_entry(SEG_TSS, 0, 0, 0, 0);

	/* Setup the GDT pointer and limit */
	gdt_p.limit = (sizeof(struct gdt_entry) * SEG_MAX) - 1;
	gdt_p.base = (uint32_t)&gdt;

	asm volatile ("lgdt (%0)": :"m" (gdt_p));
}
#else /* __LINUX_GDT__ */
#endif /* __LINUX_GDT__ */
