#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#define MEMLENGTH 4096 // Define the total size of memory to be managed.

static double memory[MEMLENGTH]; // Static array to simulate memory allocation.

// Moves to the next memory header based on the current header's size.
header* nextHeader(header* current) {
    return (header*)((char*)current + current->size + sizeof(header));
}

// Custom malloc function with error reporting.
void *mymalloc(size_t size, char *file, int line) {
    if (size == 0) { // Check for zero size allocation request.
        printf("ERROR [%s:%d]: Cannot allocate 0 bytes.\n", file, line);
        return NULL;
    }

    // Adjust size to be a multiple of 8 for alignment purposes.
    size = (size + 7) & ~7;
    size_t totalSize = size + sizeof(header);

    if (totalSize > MEMLENGTH) { // Check for memory overflow.
        printf("ERROR [%s:%d]: Not enough memory.\n", file, line);
        return NULL;
    }

    header* current = (header*)memory;
    if (current->alloc == 0 && current->size == 0) { // Initialize if memory is unused.
        current->size = MEMLENGTH - sizeof(header);
    }

    // Iterate through memory to find a suitable block.
    while ((char*)current < (char*)&memory[MEMLENGTH]) {
        if (current->alloc == 0 && current->size >= size) {
            // Allocate memory block.
            current->alloc = 1;
            size_t remainingSpace = current->size - size;
            current->size = size;
            if (remainingSpace > sizeof(header)) {
                // Create new header for remaining space.
                header* newHeader = nextHeader(current);
                newHeader->size = remainingSpace - sizeof(header);
                newHeader->alloc = 0;
            }
            return (void*)((char*)current + sizeof(header));
        }
        current = nextHeader(current); // Move to next header.
    }
    return NULL; // Return NULL if no suitable block was found.
}

// Custom free function to deallocate memory, with enhanced error checking and reporting.
void myfree(void *ptr, char *file, int line) {
    // Check for NULL pointer to prevent undefined behavior.
    if (!ptr) {
        printf("ERROR [%s:%d]: Free called on NULL pointer.\n", file, line);
        return;
    }

    // Calculate the actual start address of the memory block by subtracting the size of the header.
    header* current = (header*)((char*)ptr - sizeof(header));

    // Validate pointer bounds to ensure it's within the managed memory range.
    if ((char*)current < (char*)memory || (char*)current >= (char*)memory + MEMLENGTH) {
        printf("ERROR [%s:%d]: Pointer outside managed memory bounds.\n", file, line);
        return;
    }

    // Check if the block was already freed or not allocated through mymalloc.
    if (current->alloc == 0) {
        printf("ERROR [%s:%d]: Double free or block not allocated through mymalloc.\n", file, line);
        return;
    }

    // Mark the block as free.
    current->alloc = 0;

    // Attempt to coalesce with adjacent free blocks to mitigate fragmentation.
    header* next = nextHeader(current); // Calculate the next block header.
    if ((char*)next < (char*)memory + MEMLENGTH && next->alloc == 0) {
        // If the next block is free, merge it with the current block.
        current->size += next->size + sizeof(header);
    }

    // Check for a preceding free block to merge with, only if not the first block.
    if (current != (header*)memory) {
        header* last = lastHeader(current); // Find the previous block header.
        if (last->alloc == 0) {
            // If the last block is free, merge the current block into it.
            last->size += current->size + sizeof(header);
        }
    }
}


// Checks if the custom memory is clear.
int memClear() {
    header* start = (header*)memory;
    if ((start->size == 0 || start->size == MEMLENGTH - sizeof(header)) && start->alloc == 0) {
        return 1; // Memory is clear.
    }
    return 0; // Memory is not clear.
}

// Finds the previous header in the memory.
header* lastHeader(header* current) {
    header* last = (header*)memory;
    while (nextHeader(last) < current) {
        last = nextHeader(last);
    }
    return last;
}
