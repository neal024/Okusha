
#include <vga.h>
#include <terminal.h>
#include <string.h>
#include <boot.h>
#include <ascii.h>
#include <util.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * VGA ENVIRONMENT VARIABLES
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static uint16_t* term_buffer;       /* VGA Buffer Pointer */
/* Co-Ordinates */
static size_t	 term_cur_row;      /* Current Row */
static size_t 	 term_cur_column;   /* Current Column */
static size_t    term_cur_buf_pos;  /* Current Position in VGA Buffer */
/* Attributes */
static uint8_t	 term_cur_color;    /* Current Foreground & Background
                                     * Color Combination */
static uint8_t	 term_cur_htspace;  /* Horizontal TAB Space */
static uint8_t   term_cur_vtspace;  /* Vertical TAB Space */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * STATIC INLINE DEFINITIONS
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Return Combined Foreground & Background Color */
static inline uint8_t
vga_get_color (vga_color_t fg, vga_color_t bg)
{
    return (fg | (bg << VGA_BG_MASK));
}

/* Return Combined Color & Character */
static inline uint16_t
vga_get_entry (char c, uint8_t color)
{
    uint16_t c16     = c;
    uint16_t color16 = color;
    return ((c16 & 0x00FF) | (color16 << VGA_COLOR_MASK));
}

/* Set Cursor Position */
static inline void
vga_update_cursor (void)
{
	uint32_t pos = term_cur_buf_pos;
	outb(VGA_PORT_HIGH, VGA_REG_CTRL);		    /* CRT Control Register: Select Cursor Location */
	outb((0x00FF & (pos >> 8)), VGA_REG_DATA);	/* Send the High Byte accross the BUS */
	outb(VGA_PORT_LOW, VGA_REG_CTRL);		    /* CRT Control Register: Select Send Low Byte */
    outb((0x00FF & pos), VGA_REG_DATA);		    /* Send the Low Byte of the Cursor Location */
}

/* 0 (Zero) Based Line Index
 * Won't update cursor position
 */
static inline void
vga_clear_line(uint8_t from, uint8_t to)
{
    if (from > to) {
        swap(from, to);
    }
    if (to > VGA_HEIGHT) {
        to = VGA_HEIGHT;
    }
    if (from > VGA_HEIGHT) {
        from = 0;
    }

	size_t start = (VGA_WIDTH * from);
	size_t stop  = (VGA_WIDTH * to);

	do
	{
		term_buffer[start] = vga_get_entry(VGA_CLEAR_CHAR,
										   term_cur_color);
		++(start);
	} while (start < stop);
}

/* Clear The VGA Buffer */
static inline void
vga_clear_screen(void)
{
	vga_clear_line(0, VGA_HEIGHT);

	term_cur_row     = 0;
	term_cur_column  = 0;
    term_cur_buf_pos = 0;

    vga_update_cursor();
}

/* Scroll Up */
static inline void
vga_scroll_up (uint8_t line)
{
    if (line > VGA_HEIGHT)
    {
        line = VGA_HEIGHT;
    }

	size_t index = 0;
	size_t copy_len = (VGA_WIDTH * (VGA_HEIGHT-line));
	size_t cleared  = (VGA_WIDTH * line);

	/* OVERWRITE: Copy Buffer */
	for (index = 0; index < copy_len; ++index)
	{
		term_buffer[index] = term_buffer[index+cleared];
	}

	/* Clear Lines */
	vga_clear_line((VGA_HEIGHT - line), VGA_HEIGHT);

    /* Update Counters */
    term_cur_buf_pos -= cleared;

    /* Update Cursor */
	vga_update_cursor();
}

/* Check Buffer Overflow and Adjust */
static inline void
vga_overflow_check(void)
{
	if (term_cur_buf_pos >= VGA_ENTRY_COUNT)
	{
		vga_scroll_up(1);
	}
    else
    {
        /* Update Cursor */
        vga_update_cursor();
    }
}

/*
 * Write Character on Buffer
 * Adjust Screen
 *      Only Top-Down Adjustment
 *      No Left-Right Adjustment
 */
static inline void
vga_put_char (char c)
{
	uint8_t scrolled = 0;

    /* Write Character in Current Buffer Position */
	term_buffer[term_cur_buf_pos] = vga_get_entry(c, term_cur_color);

	/* Update Column, Row & Position Counter */
    ++(term_cur_buf_pos);
    vga_overflow_check();
    term_cur_row    = term_cur_buf_pos / VGA_WIDTH;
    term_cur_column = term_cur_buf_pos % VGA_WIDTH;
}
 
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * PUBLIC APIs
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void terminal_init (void)
{
	term_cur_color		= vga_get_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	term_cur_htspace	= 8;
	term_cur_vtspace	= 6;

	term_buffer     	= (uint16_t*) VGA_DISPLAY_MEM;

	vga_clear_screen();
}

void putch (char c)
{
	switch(c)
	{
		case BS: /* Back Space */
			if (term_cur_buf_pos > 0) {
				--(term_cur_buf_pos);
				vga_put_char(NUL);
			}
			break;

		case HT: /* Horizontal TAB */
            term_cur_buf_pos += term_cur_htspace;
	        vga_overflow_check();
            term_cur_row    = term_cur_buf_pos / VGA_WIDTH;
            term_cur_column = term_cur_buf_pos % VGA_WIDTH;
			break;

		case LF: /* New Line */
			term_cur_buf_pos += (VGA_WIDTH - term_cur_column);
	        vga_overflow_check();
            term_cur_row    = term_cur_buf_pos / VGA_WIDTH;
            term_cur_column = term_cur_buf_pos % VGA_WIDTH;
			break;

        case VT: /* Vertical TAB */
            /* NOT IMPLEMENTATED */
            break;

        case CR: /* Carriage Return: means to return to the beginning
                  * of the current line without advancing downward.
                  */
			term_cur_buf_pos -= term_cur_column;
	        vga_overflow_check();
            term_cur_row    = term_cur_buf_pos / VGA_WIDTH;
            term_cur_column = term_cur_buf_pos % VGA_WIDTH;
			break;

		default:
			vga_put_char(c);
			break;
	}
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
