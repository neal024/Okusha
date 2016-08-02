/*
 * OSHUKA: This header includes the necessary bare minimum funtions
 *         to prepare entries (values) which can be put into VGA dispaly table.
 */
#ifndef __VGA_H__
#define __VGA_H__

#include <types.h>

#define VGA_DISPLAY_MEM (0xB8000)

#define VGA_WIDTH   (80)
#define VGA_HEIGHT  (25)
#define VGA_DEPTH   (2)

typedef uint16_t vga_entry_t;

/* Hardware text mode color constants. */
typedef enum vga_color {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
} vga_color_t;

#define VGA_BG_MASK     (4)
#define VGA_COLOR_MASK  (8)

uint8_t make_color (vga_color_t fg, vga_color_t bg);
uint16_t make_vgaentry (char c, uint8_t color);

#endif /* __VGA_H__ */
