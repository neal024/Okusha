
#include <terminal.h>

#ifdef __OS_DEV_LINKER__
kernel_main()
#else /* __IKNOW_LINER__ */
kmain()
#endif /* __OS_DEV_LINKER__ */
{
    /* Initialize terminal interface */
    terminal_initialize();

    /* Since there is no support for newlines in terminal_putchar
     * yet, '\n' will produce some VGA specific character instead.
     * This is normal.
     */
    terminal_writestring("Hello, kernel World!\n");
}
