/*
 * OSHUKA: This header includes the necessary bare minimum funtions
 *         to be able to print ASCII characters in your dispaly screens
 */
#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include <vga.h>

void terminal_init(void);

void putch (char c);
void puts (const char* data);
void printf (const char *format, ...);
#endif /* __TERMINAL_H__ */
