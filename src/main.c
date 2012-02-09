// Made for JamesM's tutorials 

#include "monitor.h"
#include "descriptor_tables.h"

int main(struct multiboot *mboot_ptr)
{
  init_descriptor_tables();
  monitor_clear();
  monitor_write("Hello, world!\n");
  monitor_write_hex(0xdeadbeef);
  monitor_put('\n');
  monitor_write_dec(1234567);
  

  asm volatile("int $3");
  asm volatile("int $5");
    
  return 0xDEADBABA;
}
