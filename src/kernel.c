
#include <terminal.h>

kmain()
{
    /* Initialize terminal interface */
    terminal_initialize();

    /* Since there is no support for newlines in terminal_putchar
     * yet, '\n' will produce some VGA specific character instead.
     * This is normal.
     */
    terminal_writestring("Hello, kernel World!\n");
}
