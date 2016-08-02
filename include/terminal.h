/*
 * OSHUKA: This header includes the necessary bare minimum funtions
 *         to be able to print ASCII characters in your dispaly screens
 */
#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include <vga.h>

void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_writestring(const char* data);

#endif /* __TERMINAL_H__ */
