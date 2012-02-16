#include "common.h"
#include "monitor.h"
#include "isr.h"

isr_t interrupt_handlers[256];

void register_interrupt_handler(u8int n, isr_t handler) {
    interrupt_handlers[n] = handler;
} 

void isr_handler(registers_t regs) {
    if (interrupt_handlers[regs.int_no]) {
        interrupt_handlers[regs.int_no](regs);
    }
    else {
        monitor_write("unhandled interrupt: ");
        monitor_write_dec(regs.int_no);
        monitor_put('\n');
    }
}

void irq_handler(registers_t regs) {
    if (regs.int_no >= 40)
        outb(0xa0, 0x20); //reset signal to slave PIC
    outb(0x20, 0x20); //reset signal to master PIC

//    if (regs.int_no != 0x20) {
//        monitor_write("IRQ handler 0x");
//        monitor_write_hex(regs.int_no);
//        monitor_put('\n');
//    }

    if (interrupt_handlers[regs.int_no]) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}
