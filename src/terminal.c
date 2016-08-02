
#include <vga.h>
#include <terminal.h>
#include <string.h>

static size_t	 terminal_row;
static size_t 	 terminal_column;
static uint8_t	 terminal_color;
static uint16_t* terminal_buffer;

void terminal_initialize (void)
{
	terminal_row    = 0;
	terminal_column = 0;
	terminal_color  = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) VGA_DISPLAY_MEM;

	size_t row    = 0;
	size_t column = 0;

	for (row = 0; row < VGA_HEIGHT; ++row)
	{
		for (column = 0; column < VGA_WIDTH; ++column)
		{
			const size_t index = row * VGA_WIDTH + column;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor (uint8_t color)
{
	terminal_color = color;
}
 
void terminal_putentryat (char c, uint8_t color, size_t row, size_t column)
{
	const size_t index = row * VGA_WIDTH + column;
	terminal_buffer[index] = make_vgaentry(c, color);
}
 
void terminal_putchar (char c)
{
	terminal_putentryat(c, terminal_color, terminal_row, terminal_column);

	if (++terminal_column == VGA_WIDTH)
	{
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
		{
			terminal_row = 0;
		}
	}
}
 
void terminal_writestring (const char* data)
{
	size_t datalen = strlen(data);
	for (size_t i = 0; i < datalen; i++)
	{
		terminal_putchar(data[i]);
	}
}
