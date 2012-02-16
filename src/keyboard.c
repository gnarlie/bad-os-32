#include "common.h"
#include "isr.h"

// from http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
static u8int unshifted[] = 
    "\000\0001234567890-=\b\tqwertyuiop[]\n\000asdfghjkl;'`\000\\zxcvbnm,./\000"
    "*\000 \0001234567890" //f-keys, actually
    "\000\000789-456+1230." //keypad
    ;
static u8int shifted[] = 
    "\000\0001234567890-=\b\tQWERTYUIOP[]\n\000ASDFGHJKL;\"~\000|ZXCVBNM<>?\000"
    "*\000 \0001234567890" //f-keys, actually
    "\000\000789-456+1230." //keypad
    ;

static u8int is_shifted = 0;

void keyboard_irq_handler(registers_t regs) {
    u8int scan = inb(0x60);

    if (scan == 0x2a) //lshift
        is_shifted = 1;
    else if (scan == (0xaa))
        is_shifted = 0;

    if (!(scan & 0x80) && scan < sizeof(unshifted)) {
        u8int * codes = (is_shifted ? shifted : unshifted);
        u8int xlate = codes[scan];
        if (xlate) {
            monitor_put(xlate);
        }
    }

    outb(0x20, 0x20);
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_irq_handler);
}

