#include "common.h"

void outb(u16int port, u8int value ) {
    asm volatile ("outb %1, %0" :: "dN" (port), "a" (value));
}

u8int inb(u16int port) {
    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

u16int inw(u16int port) {
    u16int ret;
    asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void panic(const char * err, int line, const char * file) {
    monitor_write("problem in line ");
    monitor_write_dec(line);
    monitor_write(" of file " );
    monitor_write(file);
    monitor_put('.');
    monitor_put('\n');
    monitor_write(err);
    while(1) ; // never return!
}

