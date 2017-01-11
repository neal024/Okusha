/*
 * OSHUKA: This header includes the necessary bare minimum funtions
 *         to be able to print ASCII characters in your dispaly screens
 */
#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include <vga.h>

void terminal_init(void);
void terminal_set_color(uint8_t color);
void terminal_put_entryat(char c, uint8_t color, size_t x, size_t y);
void terminal_put_char(char c);
void terminal_write_string(const char* data);
void terminal_update_cursor(void);
void terminal_move_cursor(void);
void terminal_format_print(const char *format, ...);

void puts (const char* data);
void printf(const char *format, ...);
#endif /* __TERMINAL_H__ */
