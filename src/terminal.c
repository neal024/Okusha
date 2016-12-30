
#include <vga.h>
#include <terminal.h>
#include <string.h>
#include <boot.h>
#include <ascii.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * VGA ENVIRONMENT VARIABLES
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static size_t	 term_cur_row;
static size_t 	 term_cur_column;
static uint8_t	 term_cur_color;
static uint8_t	 term_cur_tabspace;
static size_t    term_cur_buf_pos;
static uint16_t* term_buffer;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * STATIC INLINE DEFINITIONS
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static inline void
vga_scroll_up (uint8_t line);

static inline uint8_t
vga_get_color (vga_color_t fg, vga_color_t bg)
{
    return (fg | (bg << VGA_BG_MASK));
}

static inline uint16_t
vga_get_entry (char c, uint8_t color)
{
    uint16_t c16     = c;
    uint16_t color16 = color;
    return ((c16 & 0x00FF) | (color16 << VGA_COLOR_MASK));
}

static inline void
vga_overflow_check(void)
{
	if (term_cur_row >= VGA_HEIGHT)
	{
		vga_scroll_up(1);
	}
}

static inline void
vga_update_cursor (void)
{
	uint32_t pos;
	pos = (term_cur_row * VGA_WIDTH) + term_cur_column; /* Position: (Row * Width) + Column */

	outb(VGA_PORT_HIGH, VGA_REG_CTRL);		/* CRT Control Register: Select Cursor Location */
	outb((0x00FF & (pos >> 8)), VGA_REG_DATA);	/* Send the High Byte accross the BUS */
	outb(VGA_PORT_LOW, VGA_REG_CTRL);		/* CRT Control Register: Select Send Low Byte */
	outb((0x00FF & pos), VGA_REG_DATA);		/* Send the Low Byte of the Cursor Location */
}

static inline void
vga_clear_line(uint8_t from, uint8_t to)
{
	size_t start = (VGA_WIDTH * from);
	size_t stop  = (VGA_WIDTH * to);

	do
	{
		term_buffer[start] = vga_get_entry(VGA_CLEAR_CHAR,
										   term_cur_color);
		++(start);
	} while (start < stop);
	vga_update_cursor();
}

static inline void
vga_scroll_up (uint8_t line)
{
	/*TODO: Check for line !> VGA_HEIGHT */

	size_t index = 0;
	size_t copy_len = (VGA_WIDTH * (VGA_HEIGHT-line));
	size_t cleared  = (VGA_WIDTH * line);

	/* Clear Lines */
	vga_clear_line(0, (line - 1));

	/* Copy Buffer */
	for (index = 0; index < copy_len; ++index)
	{
		term_buffer[index] = term_buffer[index+cleared];
	}

	/* Clear Lines */
	vga_clear_line((VGA_HEIGHT - line -1), (VGA_HEIGHT - 1));

	term_cur_row -= line;
	vga_update_cursor();
}

#if 0
static inline void
vga_move_line_up(uint8_t)
{
}
#endif /* 0 */

static inline void
vga_put_char_at (char c, uint8_t color, size_t row, size_t column)
{
	const size_t index = (row * VGA_WIDTH) + column;
	term_buffer[index] = vga_get_entry(c, color);
}

static inline void
vga_put_char_colored()
{
}

static inline void
vga_put_char_colored_at()
{
}

static inline void
vga_put_char (char c)
{
	uint8_t scrolled = 0;

	term_buffer[term_cur_buf_pos] = vga_get_entry(c, term_cur_color);

	/* Update Column and Row */
	if (++(term_cur_column) == VGA_WIDTH)
	{
		term_cur_column = 0;
		if (++(term_cur_row) == VGA_HEIGHT)
		{
			vga_overflow_check();
			vga_scroll_up(1);
			scrolled += 1;
		}
	}

	/* Update current buffer position */
	if (++(term_cur_buf_pos) == VGA_ENTRY_COUNT)
	{
		if (scrolled)
		{
			term_cur_buf_pos -= scrolled * VGA_WIDTH;
		}
		else
		{
			/* Over Flow */
			term_cur_buf_pos = 0;
		}
	}
}
 
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * PUBLIC APIs
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void terminal_clear(void)
{
	vga_clear_line(0, (VGA_HEIGHT-1));
	term_cur_row    = 0;
	term_cur_column = 0;
	term_buffer     = (uint16_t*) VGA_DISPLAY_MEM;
}

void terminal_init (void)
{
	term_cur_color		= vga_get_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	term_cur_tabspace	= 8;
	term_cur_row		= 0;
	term_cur_column		= 0;
	term_buffer     	= (uint16_t*) VGA_DISPLAY_MEM;
	terminal_clear();
}

void terminal_set_color (uint8_t color)
{
	term_cur_color = color;
	/*TODO: Update All Colors */
}

void terminal_set_tabspace (uint8_t tabspace)
{
	term_cur_tabspace = tabspace;
}

void putch (char c)
{
	switch(c)
	{
		case BS:
			if (term_cur_column > 0) {
				--(term_cur_column);
				vga_put_char(NUL);
			}
			break;

		case TAB:
			term_cur_column = (term_cur_tabspace + term_cur_column) & \
							 ~(term_cur_tabspace - 1);
			break;

		case LF:
			term_cur_buf_pos += (VGA_WIDTH-term_cur_column);
			term_cur_column = 0;
			++(term_cur_row);
			/* TODO: Adjust if out of order*/
			break;

		case CR:
			term_cur_buf_pos -= term_cur_column;
			term_cur_column = 0;
			break;

		default:
			vga_put_char(c);
			break;
	}
	vga_update_cursor();
	vga_overflow_check();
}

void putch_at (char c, uint8_t row, uint8_t column)
{
	vga_put_char_at(c, term_cur_color, row, column);
}

void puts (const char* data)
{
	size_t datalen = strlen(data);
	for (size_t i = 0; i < datalen; i++)
	{
		putch(data[i]);
	}
}

void printf(const char *format, ...)
{
	char **arg = (char **) &format;
	int  c;
	char buf[20];

	++(arg);

	while ((c = *format++) != 0)
	{
		if (c != '%') {
			putch(c);
		}
		else {
			char *p;
			c = *format++;

			switch (c)
			{
				case 'd':
				case 'u':
				case 'x':
					itoa (buf, c, *((int *) arg++));
					p = buf;
					goto string;
					break;

				case 'p':
					itoa (buf, c, *((int *) arg++));
					p = buf;
					if (p) {
						putch('0');
						putch('x');
					}
					goto string;
					break;

				case 's':
					p = *arg++;
					if (!p) {
						p = "(null)";
					}

				string:
					while (*p) {
						putch(*p++);
					}
					break;

				default:
					putch(*((int *) arg++));
					break;
			}
		}
	}
}
