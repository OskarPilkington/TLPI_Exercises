#include "tlpi_hdr.h"
#include <stdlib.h>
#include <stdint.h>

#define PTR_CNT 64

int main(){
    
    char *ptrs[PTR_CNT];

    for (int i=0; i<PTR_CNT; i++)
    {
        ptrs[i] = malloc(i); // Allocate an increasing number of bytes 
        if(i != 0)
            printf("cnt: %d, adr: %p, diff: %ld, Len: %ld, PrBrk: %p\n", i, (char*)ptrs[i], (ptrs[i]-ptrs[i-1]), (uint64_t) *(ptrs[i]-8), sbrk(0));
        else
            printf("cnt: %d, adr: %p, Len: %ld, PrBrk: %p\n", i, (char*)ptrs[i], (uint64_t) *(ptrs[i]-8), sbrk(0));
    }
/*
    printf("Address for one: %p\n", (char*)one);
    printf("Address for two: %p, diff: %ld\n", (char*)two, two-one);
    printf("Address for three: %p, diff: %ld\n", (char*)three, three-two);
    
    printf("Length param for one: 0x%lX\n", (uint64_t) *(one-8));
    printf("Length param for two: 0x%lX\n", (uint64_t) *(two-8));
    printf("Length param for three: 0x%lX\n", (uint64_t) *(three-8));
*/

    return 0;
}
