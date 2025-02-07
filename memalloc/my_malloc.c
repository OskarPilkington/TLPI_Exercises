#include "tlpi_hdr.h"
#include <stdlib.h>
#include <stdint.h>

#define INC_SBRK 0x20000 // 2**17 bytes
#define MALLOC_INC 0x10
#define MALLOC_INC_MASK 0xff
#define LENGTH_BYTES 4 // 32 bits

bool malloc_first = true;
void* malloc_prbrk;                 // maintain program break without calling each call of my_malloc

extern char end;                    // this is a variable at the end of the uninitialized data segment, i.e. just before the beginning of the heap
void* allocated_end = (void*)&end;  // define a variable to maintain the end of the allocated space.

void* my_malloc(size_t size){
    // Initialize malloc on first call
    if (malloc_first){
        allocated_end += MALLOC_INC + LENGTH_BYTES; // Ensure theres enough space to ensure we aren't corrupting any unforseen memory
        allocated_end = (void*)((uintptr_t)allocated_end & ~MALLOC_INC_MASK); // Make it alligned with a 16 byte increment
        malloc_first = false;
        malloc_prbrk = sbrk(0);     // obtain the program break
    }
    
    // TODO: First search if within the free list there is a sufficiently large block

    size_t size_rounded = ((size + LENGTH_BYTES) & ~MALLOC_INC_MASK) + MALLOC_INC; // Add 4 to account for the 32-bit length field, then round to nearest 16 above
    if (size_rounded < 32)// this just ensures there's enough space for the length field plus two 64 bit pointers.
        size_rounded = 32;

    // Check if block being allocated will fit within the current heap
    if ((char*)allocated_end + size_rounded > (char*)malloc_prbrk){
        // TODO: use sbrk to increase size of heap
        printf("ERROR DIDN'T EXPECT THAT YET...\n");
    }

    // Write length of block
    printf("Debug - size: %ld\n", size_rounded);
    *(uint32_t*)allocated_end = (uint32_t)size_rounded;
    char* returned_pointer = allocated_end + LENGTH_BYTES;
    allocated_end = (char*)allocated_end + size_rounded;

    return  (void*)returned_pointer;

}

int main(){
    
    char *one = my_malloc(1);
    char *two = my_malloc(30);
    char *three = my_malloc(3);

    printf("Address for one: %p\n", (char*)one);
    printf("Address for two: %p, diff: %ld\n", (char*)two, two-one);
    printf("Address for three: %p, diff: %ld\n", (char*)three, three-two);
    
    printf("Length param for one: 0x%lX\n", (uint64_t) *(one-LENGTH_BYTES));
    printf("Length param for two: 0x%lX\n", (uint64_t) *(two-LENGTH_BYTES));
    printf("Length param for three: 0x%lX\n", (uint64_t) *(three-LENGTH_BYTES));


    return 0;
}
