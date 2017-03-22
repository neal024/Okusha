/*
 * OSHUKA: This header file contains all string related functions.
 */
#ifndef __STRING_H__
#define __STRING_H__

#include <types.h>

void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, uint8_t val, size_t count);
uint16_t *memsetw(uint16_t *dest, uint16_t val, size_t count);
size_t strlen(const char* str);
void itoa(char *buf, int base, int d);

#endif /* __STRING_H__ */
