#include "paging.h"
#include "common.h"

// A bitset of frames - used or free.
u32int *frames;
u32int nframes;

// Defined in kheap.c
extern u32int placement_address;


// The kernel's page directory
page_directory_t *kernel_directory=0;

// The current page directory;
page_directory_t *current_directory=0;

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

// Static function to set a bit in the frames bitset
static void set_frame(u32int frame_addr) {
   u32int frame = frame_addr/0x1000;
   u32int idx = INDEX_FROM_BIT(frame);
   u32int off = OFFSET_FROM_BIT(frame);
   frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(u32int frame_addr) {
   u32int frame = frame_addr/0x1000;
   u32int idx = INDEX_FROM_BIT(frame);
   u32int off = OFFSET_FROM_BIT(frame);
   frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
static u32int test_frame(u32int frame_addr) {
   u32int frame = frame_addr/0x1000;
   u32int idx = INDEX_FROM_BIT(frame);
   u32int off = OFFSET_FROM_BIT(frame);
   return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
static u32int first_frame() {
   u32int i, j;
   for (i = 0; i < INDEX_FROM_BIT(nframes); i++) {
       if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
       {
           // at least one bit is free here.
           for (j = 0; j < 32; j++) {
               u32int toTest = 0x1 << j;
               if ( !(frames[i]&toTest) ) {
                   return i*4*8+j;
               }
           }
       }
   }
}

void alloc_frame(page_t* page, int is_kernel, int is_writeable) {
    if (page->frame != 0) {
        return; // already allocated - we're good
    }

    u32int idx = first_frame();
    if (idx == (u32int) -1) {
        PANIC("No free frames!");
    }

    set_frame(idx * 0x1000);
    page->present = 1;
    page->rw = is_writeable ? 1 : 0;
    page->user = is_kernel;
    page->frame = idx;
}

void free_frame(page_t * page) {
    u32int frame = page->frame;
    if (! frame) {
        return;
    }

    clear_frame(frame);
    page->frame = 0;
}

void initialise_paging() {
    u32int mem_end_page = 0x1000000; //TODO: assumes 16MB
    nframes = mem_end_page / 0x1000;
    frames = (u32int*)kmalloc(INDEX_FROM_BIT(nframes));
    bzero(frames, INDEX_FROM_BIT(nframes));

    kernel_directory = (page_directory_t*) kmalloc_a(sizeof(page_directory_t));
    current_directory = kernel_directory;

    int i = 0;
    while(i < placement_address) {
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }

    register_interrupt_handler(14, page_fault);
    switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t* dir) {
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r" (&dir->tablesPhysical));
    u32int cr0;
    asm volatile("mov %%cr0, %0": "=r" (cr0));
    cr0 |= 0x80000000; // enable paging
    asm volatile("mov %0, %%cr0":: "r" (cr0));
}

page_t* get_page(u32int address, int make, page_directory_t* dir) {
    address /= 0x1000; // address to index
    u32int table_idx = address / 1024; // find table for this index;
    if (dir->tables[table_idx]) {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else if (make) {
        u32int tmp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT | RW | USER
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else {
        return 0;
    }
}

void page_fault(registers_t regs) {
   // A page fault has occurred.
   // The faulting address is stored in the CR2 register.
   u32int faulting_address;
   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   // The error code gives us details of what happened.
   int present   = !(regs.err_code & 0x1); // Page not present
   int rw = regs.err_code & 0x2;           // Write operation?
   int us = regs.err_code & 0x4;           // Processor was in user-mode?
   int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
   int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

   // Output an error message.
   monitor_write("Page fault! ( ");
   if (present) {monitor_write("not present ");}
   if (rw) {monitor_write("read-only ");}
   if (us) {monitor_write("user-mode ");}
   if (reserved) {monitor_write("reserved ");}
   monitor_write(") at 0x");
   monitor_write_hex(faulting_address);
   monitor_write("\n");
   PANIC("Page fault");
}
