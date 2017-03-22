
#include<irq.h>
#include<terminal.h>
#include<boot.h>

/* This will keep track of how many ticks that
 * the system has been running for
 */
volatile uint32_t timer_ticks = 0;
volatile uint32_t timer_sec   = 0;

void timer_handler(struct regs *r)
{
    /* Increment our 'tick count' */
    timer_ticks++;

    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    if (timer_ticks % 18 == 0)
    {
		++timer_sec;
        printf("%d seconds has elapsed!>", timer_sec);
    }
}

/* This will continuously loop until the
 * given time has been reached
 */
void timer_wait(int32_t ticks)
{
    uint32_t eticks;

    eticks = timer_ticks + ticks;
    while (timer_ticks < eticks);
}

/* Sets up the system clock by installing the
 * timer handler into IRQ0 */
void timer_install()
{
    /* Installs 'timer_handler' to IRQ0 */
    irq_install_handler(0, timer_handler);
}
