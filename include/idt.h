
#ifndef __IDT_H__
#define __IDT_H__

#include <types.h>

#define IDT_TABLE_SIZE	(256)

/* Defines an IDT entry */
struct idt_entry {
	uint16_t	base_lo;
    uint16_t	sel;
    uint8_t		always0;
    uint8_t		flags;
    uint16_t	base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t	limit;
    uint32_t	base;
} __attribute__((packed));

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_install();

#endif /* __IDT_H__ */
