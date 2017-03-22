
#ifndef __GDT_H__
#define __GDT_H__

#include <types.h>

/* Defines a GDT entry */
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
	//SEG_USR1,
	//SEG_USR2,
	//SEG_TSS,
	SEG_MAX,
};

void gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
void gdt_install();

#endif /* __GDT_H__ */
