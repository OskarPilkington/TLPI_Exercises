#include "tlpi_hdr.h"
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

#define INC_SBRK 0x20000 // 2**17 bytes
#define MALLOC_INC 0x10
#define MALLOC_INC_MASK 0xf
#define LENGTH_BYTES 4 // 32 bits

#define FREE_OFFSET_NEXT 0
#define FREE_OFFSET_PREV 8

/* Variables for Malloc */
bool malloc_first = true;
void* malloc_prbrk;                 // maintain program break without calling each call of my_malloc
extern char end;                    // this is a variable at the end of the uninitialized data segment, i.e. just before the beginning of the heap
void* allocated_end = (void*)&end;  // define a variable to maintain the end of the allocated space.

/* Variables for Free */
bool free_first = true;
void* free_list_start = NULL;

void* my_malloc(size_t size){
    // Initialize malloc on first call
    if (malloc_first){
        allocated_end = (void*)((uintptr_t)allocated_end + MALLOC_INC + LENGTH_BYTES); // Ensure theres enough space to ensure we aren't corrupting any unforseen memory
        allocated_end = (void*)((uintptr_t)allocated_end & ~MALLOC_INC_MASK); // Make it alligned with a 16 byte increment
        malloc_first = false;
        malloc_prbrk = sbrk(0);     // obtain the program break
    }
    
    

    size_t size_rounded = ((size + LENGTH_BYTES) & ~MALLOC_INC_MASK) + MALLOC_INC; // Add 4 to account for the 32-bit length field, then round to nearest 16 above
    if (size_rounded < 32){// this just ensures there's enough space for the length field plus two 64 bit pointers.
        size_rounded = 32;
    }
    
    // TODO: First search if within the free list there is a sufficiently large block
/*
    void* current_ptr = free_list_start;
    bool found_memory = false;
    uint32_t current_size;

    while(current_ptr != NULL & !found_memory){
        current_size = *(uint32_t*)(current_ptr - LENGTH_BYTES);

    }
*/
    // Check if block being allocated will fit within the current heap
    if ((char*)allocated_end + size_rounded > (char*)malloc_prbrk){
        // TODO: use sbrk to increase size of heap
        printf("ERROR DIDN'T EXPECT THAT YET...\n");
    }

    // Write length of block
    printf("Debug - size: %ld\n", size_rounded);
    *(uint32_t*)allocated_end = (uint32_t)size_rounded;
    char* returned_pointer = (char*)((uintptr_t)allocated_end + LENGTH_BYTES);
    allocated_end = (char*)allocated_end + size_rounded;

    return  (void*)returned_pointer;

}

void my_free(void *ptr){
    if (free_first){ // Initialization function for free
        free_first = false;
    }
    
    bool empty_list = free_list_start == NULL; // Check if the free list is empty.

    if (empty_list){
        free_list_start = ptr;
        *(void**)((char*)free_list_start + FREE_OFFSET_NEXT) = NULL;
    }
    else {
        void* current_ptr = free_list_start;
        
        // Work our way to the end of the double linked list
        while( *(void**)((char*)current_ptr + FREE_OFFSET_NEXT) != NULL ){
            current_ptr = *(void**)((char*)current_ptr + FREE_OFFSET_NEXT);
        }

        // Set the next pointer on the last item to the new one and the prev pointer on the new one to the last one
        *(void**)((char*)current_ptr + FREE_OFFSET_NEXT) = ptr;
        *(void**)((char*)ptr + FREE_OFFSET_PREV) = current_ptr;
        *(void**)((char*)ptr + FREE_OFFSET_NEXT) = NULL;
    }

}

int main(){
    
    char *one = my_malloc(1);
    char *two = my_malloc(35);
    char *three = my_malloc(3);
    
    char *moo_0 = my_malloc(8);
    char *moo_1 = my_malloc(8);
    char *moo_2 = my_malloc(8);
    char *moo_3 = my_malloc(8);
    char *moo_4 = my_malloc(8);
    char *moo_5 = my_malloc(8);
    char *moo_6 = my_malloc(8);
    
    *(uint64_t*)moo_0 = 0x0deadbeef; 
    *(uint64_t*)moo_1 = 0x1deadbeef;
    *(uint64_t*)moo_2 = 0x2deadbeef;
    *(uint64_t*)moo_3 = 0x3deadbeef;
    *(uint64_t*)moo_4 = 0x4deadbeef;
    *(uint64_t*)moo_5 = 0x5deadbeef;
    *(uint64_t*)moo_6 = 0x6deadbeef;

    *one = 0xA5;
    *(uint32_t*)two = 0xdeadbeef; 
    *(uint16_t*)three = 0xbeef;
    
    my_free(moo_2);
    my_free(moo_3);
    my_free(moo_4);

    int fd = open("hexdump", O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
    write(fd, (void*)&end, 512); 

    printf("Address for one: %p\n", (char*)one);
    printf("Address for two: %p, diff: %ld\n", (char*)two, two-one);
    printf("Address for three: %p, diff: %ld\n", (char*)three, three-two);
    
    printf("Length param for one: 0x%lX\n", (uint64_t) *(one-LENGTH_BYTES));
    printf("Length param for two: 0x%lX\n", (uint64_t) *(two-LENGTH_BYTES));
    printf("Length param for three: 0x%lX\n", (uint64_t) *(three-LENGTH_BYTES));


    return 0;
}
