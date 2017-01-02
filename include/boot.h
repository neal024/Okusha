/*
 * Taken form: linux/arch/x86/boot/boot.h
 * http://lxr.free-electrons.com/source/arch/x86/boot/boot.h
 */

#ifndef __BOOT_H__
#define __BOOT_H__

#include <types.h>

/* Basic port I/O */

static inline void outb(uint8_t value, uint16_t port)
{
	asm volatile ("outb %0,%1" : : "a" (value), "dN" (port));
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t value;
	asm volatile ("inb %1,%0" : "=a" (value) : "dN" (port));
	return value;
}

static inline void outw(uint16_t value, uint16_t port)
{
	asm volatile ("outw %0,%1" : : "a" (value), "dN" (port));
}

static inline uint16_t inw(uint16_t port)
{
	uint16_t value;
    asm volatile ("inw %1,%0" : "=a" (value) : "dN" (port));
    return value;
}

static inline void outl(uint32_t value, uint16_t port)
{
    asm volatile("outl %0,%1" : : "a" (value), "dN" (port));
}

static inline uint32_t inl(uint16_t port)
{
    uint32_t value;
    asm volatile("inl %1,%0" : "=a" (value) : "dN" (port));
    return value;
}
#endif /* __BOOT_H__ */
