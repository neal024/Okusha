
#include<string.h>
#include<idt.h>

/* This exists in 'bootstrap.{s|asm}', and is used to load our IDT */
extern void idt_load();

/* Declare an IDT of IDT_TABLE_SIZE entries. */
struct idt_entry idt[IDT_TABLE_SIZE];
struct idt_ptr   idt_p;

/* Use this function to set an entry in the IDT. */
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    /* The interrupt routine's base address */
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    /* The segment or 'selector' that this IDT entry will use
    *  is set here, along with any access flags */
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

/* Installs the IDT */
void idt_install()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idt_p.limit = (sizeof (struct idt_entry) * IDT_TABLE_SIZE) - 1;
    idt_p.base  = (uint32_t)&idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, sizeof(struct idt_entry) * IDT_TABLE_SIZE);

    /* Add any new ISRs to the IDT here using idt_set_gate */


    /* Points the processor's internal register to the new IDT */
    idt_load();
}
