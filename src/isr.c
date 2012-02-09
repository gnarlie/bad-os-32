#include "common.h"
#include "monitor.h"

typedef struct registers {
    u32int ds; 
    u32int edi, esi, ebp, esp, ebx, edx, ecx, eax; //pushed by pusha
    u32int int_no, err_code;
    u32int eip, cs, eflags, useresp, ss; // pushed by proc for irupt
} registers_t;

void isr_handler(registers_t regs) {
    monitor_write("recieved interrupt: ");
    monitor_write_dec(regs.int_no);
    monitor_put('\n');
}
