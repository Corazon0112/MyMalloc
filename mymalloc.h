#ifndef _MYMALLOC_H
#define _MYMALLOC_H
#define MEMLENGTH 4096

#include <stdlib.h>
#include <stdio.h>

#define malloc(s)   mymalloc(s, __FILE__, __LINE__)
#define free(p)     myfree(p, __FILE__, __LINE__)

void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);

typedef struct Node {
    size_t size;
    struct Node* next;
} Node;

typedef struct{
    int alloc;
    int size; 
}header;

int memClear();

header* nextHeader();
header* lastHeader();

#endif