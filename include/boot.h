/*
 * Taken form: linux/arch/x86/boot/boot.h
 * http://lxr.free-electrons.com/source/arch/x86/boot/boot.h
 */

#ifndef __BOOT_H__
#define __BOOT_H__

#include <types.h>

/* Basic port I/O */

static inline void outb(uint8_t __value, uint16_t __port)
{
	__asm__ __volatile__ ("outb %0,%1" : : "a" (__value), "dN" (__port));
}

static inline uint8_t inb(uint16_t __port)
{
	uint8_t __value;
	__asm__ __volatile__ ("inb %1,%0" : "=a" (__value) : "dN" (__port));
	return __value;
}

static inline void outw(uint16_t __value, int16_t __port)
{
	__asm__ __volatile__ ("outw %0,%1" : : "a" (__value), "dN" (__port));
}

static inline uint16_t inw(uint16_t __port)
{
	uint16_t __value;
    __asm__ __volatile__ ("inw %1,%0" : "=a" (__value) : "dN" (__port));
    return __value;
}
#endif /* __BOOT_H__ */
