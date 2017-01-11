#include <gdt.h>

struct desc_struct gdt[SEG_MAX];	/* null, unused, code, data */

#ifndef __LINUX_GDT__
void gdt_entry_init(struct desc_struct entry, uint32_t base, uint32_t limit, uint8_t flags)
{
	uint8_t *dst = (uint8_t *)&entry;

	/*
	entry.limit_low			= ;
	entry.base_low			= ;
	entry.base_middle		= ;
	entry.type				= ;
	entry.flags_limit_hi	= ;
	entry.base_hi			= ;
	 */

	/* Encode Limit */
	if (limit <= 65536) {
		dst[6] = 0x40;	/* 16bit Entry */
	}
	else {
		if ((limit & 0xFFF) != 0xFFF) {
			limit = (limit >> 12) - 1;
		}
		else {
			limit = (limit >> 12);
		}
		dst[6] = 0xC0; 	/* Not in 16bit Mode */
	}

	dst[0]  =  (limit & 0x000000FF);
	dst[1]  = ((limit & 0x0000FF00) >> 8);
	dst[6] |= ((limit & 0x000F0000) >> 16);

	/* Encode Base */
	dst[2] =  (base & 0x000000FF);
	dst[3] = ((base & 0x0000FF00) >> 8);
	dst[4] = ((base & 0x00FF0000) >> 16);
	dst[7] = ((base & 0xFF000000) >> 24);

	/* Access Right */
	dst[5] = flags;
}

uint32_t gdt_entry_get_base(struct desc_struct entry)
{
	uint8_t *dst = (uint8_t *)&entry;
	uint32_t ret = 0;

	ret = dst[7];
	ret = (ret << 8) + dst[4];
	ret = (ret << 8) + dst[3];
	ret = (ret << 8) + dst[2];

	return ret;
}

uint32_t gdt_entry_get_limit(struct desc_struct entry)
{
	uint8_t *dst = (uint8_t *)&entry;
	uint32_t ret = 0;

	ret = dst[6] & 0x0F;
	ret = (ret << 8) + dst[1];
	ret = (ret << 8) + dst[0];

	if ((dst[6] & 0xC0) == 0xC0) {
		ret = (ret << 12) | 0xFFF;
	}

	return ret;
}

uint16_t offset_of_segment(enum segment_type seg)
{
	return (uint16_t)((uint32_t)&gdt[seg] - (uint32_t)&gdt[0]);
}

void gdt_init(void)
{
	gdt_entry_init(gdt[SEG_NULL], 0, 0, 0);
	gdt_entry_init(gdt[SEG_UNSD], 0, 0, 0);
	gdt_entry_init(gdt[SEG_CODE], 0, 64*1024*1024, 0x9A);	/* Start at 0 (Zero) of Size 64MB, Flags 0x9a */
	gdt_entry_init(gdt[SEG_DATA], 0, 64*1024*1024, 0x92);	/* Start at 0 (Zero) of Size 64MB, Flags 0x92 */

	uint32_t i[2];
	i[1] = (uint32_t) &gdt;
	i[0] = sizeof(gdt) << 16;

	asm volatile ("lgdt (%0)": :"p" (((uint8_t *) i)+2));
}

void gdt_remove(void)
{
}

#else /* __LINUX_GDT__ */
#endif /* __LINUX_GDT__ */
