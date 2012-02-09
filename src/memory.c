#include "common.h"

void bzero(u8int* dest, u32int len) {
    for(; len != 0; len--) {
        *dest++ = 0;
    }
}
