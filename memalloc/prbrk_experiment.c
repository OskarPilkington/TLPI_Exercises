#include "tlpi_hdr.h"
#include <stdlib.h>
#include <stdint.h>

#define ALLOC_CNT   4096
#define ALLOC_SIZE  1024

// This program allocates buffers of 1024 bytes using malloc and print the times at which the program break variable changes
int main()
{
    volatile char *ptrs[ALLOC_CNT];
    uint64_t *prgBrk, *prev_prgBrk;
    
    ptrs[0] = malloc(ALLOC_SIZE-8); // I subtract the 8 to account for the Length field before the memory block so we know we are actually allocating 1024 bytes
    *ptrs[0] += 1; // See if it just gets optimized out by the compiler
    printf("Check that length field is of expected size: %ld\n", *(ptrs[0]-8));
    prgBrk = sbrk(0); // Assign program break

    for (int i=1; i<ALLOC_CNT; i++)
    {
        ptrs[i] = malloc(ALLOC_SIZE-8);
        prev_prgBrk = prgBrk;
        prgBrk = sbrk(0);

        if (prgBrk != prev_prgBrk)
            printf("Increased after %d Allocs!!!\n", i);
    }
    printf("Check that length field is of expected size: %d\n", *(ptrs[20]-8));
    return 0;
}
