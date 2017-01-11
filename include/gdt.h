
/*
 * REF: //arch/x86/include/asm/desc_defs.h
 */

#ifndef __GDT_H__
#define __GDT_H__

#include <types.h>

/* 8-Byte Segment Descriptor */
#ifndef __LINUX_GDT__

/*
 * What "Limit 0:15" means is that -
 *		the field contains bits 0-15 of the limit value.
 *		The base is a 32 bit value containing the linear address where
 *		the segment begins. The limit, a 20 bit value, tells the maximum
 *		addressable unit (either in 1 byte units, or in pages).
 *		Hence, if you choose page granularity (4 KiB) and set the limit
 *		value to 0xFFFFF the segment will span the full 4 GiB address space.
 */

struct gdt_entry {
	uint16_t	limit_low;		/* Size of the Segment */
	uint16_t	base_low;		/* Starting Address of the Segment */
	uint8_t		base_middle;	/* Starting Address of the Segment */
	uint8_t		access;			/* Access Type */
	uint8_t		granularity;	/* Flags and Limit High Bits */
	uint8_t		base_high;		/* Starting Address of the Segment */
} __attribute__((packed));

struct gdt_ptr {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed));

enum segment_type {
	SEG_NULL = 0,
	SEG_CODE,
	SEG_DATA,
	SEG_USR1,
	SEG_USR2,
	//SEG_TSS,
	SEG_MAX,
};

void gdt_set_entry(uint8_t seg_num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
void gdt_init(void);

#else /* __LINUX_GDT__ */
struct gdt_entry {
	uint32_t	a;
	uint32_t	b;
}

#define MASK_LIMIT_LOW	(0x0FFFF)
#define MASK_LIMIT_HIGH	(0xF0000)

#define MASK_BASE_LOW		(0x0000FFFF)
#define MASK_BASE_MIDDLE	(0x00FF0000)
#define MASK_BASE_HIGH		(0xFF000000)
#define BASE_LOW_SHIFT		(16)
#define BASE_MIDDLE_SHIFT	(16)

#define MASK_FLAGS			(0xF0FF)
#define FLAGS_SHIFT			(8)

#define GDT_ENTRY_INIT(flags, base, limit)	{ \
		.a = ((limit) & MASK_LIMIT_LOW) | \
			 (((base) & MASK_BASE_LOW) << BASE_LOW_SHIFT), \
		.b = (((base) & MASK_BASE_MIDDLE) >> BASE_MIDDLE_SHIFT) | \
			 (((flags) & MASK_FLAGS) << FLAGS_SHIFT) | \
			 ((limit) & MASK_LIMIT_HIGH) | \
			 ((base) & MASK_BASE_HIGH), \
	}
#endif /* __LINUX_GDT__ */

#endif /* __GDT_H__ */
