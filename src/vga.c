
#include <vga.h>

uint8_t make_color (vga_color_t fg, vga_color_t bg)
{
    return (fg | (bg << VGA_BG_MASK));
}

uint16_t make_vgaentry (char c, uint8_t color)
{
    uint16_t c16     = c;
    uint16_t color16 = color;
    return (c16 | (color16 << VGA_COLOR_MASK));
}
