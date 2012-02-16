// Made for JamesM's tutorials 

#include "monitor.h"
#include "descriptor_tables.h"

int main(struct multiboot *mboot_ptr)
{
  init_descriptor_tables();
  asm volatile("sti");

  init_keyboard();

  monitor_clear();
  monitor_write("Hello, world!\n");
  monitor_write_hex(0xdeadbeef);
  monitor_put('\n');
  monitor_write_dec(1234567);
  monitor_put('\n');

  initialise_paging();

//  asm volatile("int $3");

  monitor_write("reading valid memory: ");
  u32int* ptrOkay = (u32int*)0x00001000;
  u32int dataOkay = *ptrOkay;
  monitor_write_hex(dataOkay);
  monitor_put('\n');

//  monitor_write("reading memory not around: ");
//  u32int* ptr = (u32int*)0xa0000000;
//  u32int data = *ptr;
//  monitor_write_hex(data);
//  monitor_put('\n');

  monitor_write("starting timer\n");
//  init_timer(50);
    
  return 0xDEADBABA;
}
