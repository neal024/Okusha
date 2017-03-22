
#ifndef __IRQ_H__
#define __IRQ_H__

#include <system.h>

void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_install();

#endif /* __IRQ_H__ */
