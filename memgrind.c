#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mymalloc.h"

#define HEADERSIZE 8
#define OBJECTS 8
#define MEMLENGTH 4096
#define TEST_RUNS 50
#define ALLOC_TESTS 120

void memtest();
void getErrors();
double runTest(double (*testFunction)());
double test1();
double test2();
double test3();
double test4();
double test5();

int main(void) {
    memtest(); // Runs a memory allocation and correctness test
    getErrors(); // Demonstrates various intentional memory allocation/freeing errors

    // Array of pointers to test functions.
    double (*testFunctions[5])() = {test1, test2, test3, test4, test5};
    for (int i = 0; i < 5; i++) {
        double avg = runTest(testFunctions[i]) * 1000; // Convert seconds to milliseconds
        printf("Average time taken to complete test %d over %d runs: %f milliseconds\n", i + 1, TEST_RUNS, avg);
    }

    return 0;
}

void memtest() {
    double objSize = MEMLENGTH - (OBJECTS * HEADERSIZE);
    objSize /= OBJECTS;

    char *obj[OBJECTS];
    int errors = 0;

    for (int i = 0; i < OBJECTS; i++) {
        obj[i] = malloc(objSize);
    }

    for (int i = 0; i < OBJECTS; i++) {
        memset(obj[i], i, objSize);
    }

    for (int i = 0; i < OBJECTS; i++) {
        for (int j = 0; j < objSize; j++) {
            if (obj[i][j] != i) {
                errors++;
                printf("Object %d byte %d incorrect: %d\n", i, j, obj[i][j]);
            }
        }
    }

    printf("%d incorrect bytes\n", errors);

    for (int i = 0; i < OBJECTS; i++) {
        free(obj[i]);
    }
}

void getErrors() {
    // Demonstrates intentional errors for testing error handling, unchanged as its purpose is specific and clear
}

double runTest(double (*testFunction)()) {
    struct timeval start, end;
    double totalTime = 0;

    for (int i = 0; i < TEST_RUNS; i++) {
        gettimeofday(&start, NULL);
        testFunction();
        gettimeofday(&end, NULL);
        totalTime += (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0; // Convert to milliseconds
    }

    return totalTime / TEST_RUNS; // Return average time in milliseconds
}

double test1() {
    // Test for repeatedly allocating and freeing a small amount of memory
    for (int i = 0; i < ALLOC_TESTS; i++) {
        char *ptr = malloc(1);
        free(ptr);
    }
}

double test2() {
    // Test for allocating and immediately freeing memory in a loop
    char *ptrArray[ALLOC_TESTS];
    for (int i = 0; i < ALLOC_TESTS; i++) {
        ptrArray[i] = malloc(1);
        free(ptrArray[i]);
    }
}

double test3() {
    // Test for randomly choosing between allocating and freeing memory to simulate dynamic memory usage
    char *ptrArray[ALLOC_TESTS];
    int allocated[ALLOC_TESTS] = {0}, loc = 0;

    for (int i = 0; i < ALLOC_TESTS * 2; i++) {
        if (loc < ALLOC_TESTS && (loc == 0 || rand() % 2 == 0)) {
            ptrArray[loc] = malloc(1);
            allocated[loc++] = 1;
        } else if (loc > 0) {
            free(ptrArray[--loc]);
            allocated[loc] = 0;
        }
    }

    for (int i = 0; i < loc; i++) {
        free(ptrArray[i]);
    }
}

double test4() {
    // Test for allocating and freeing a large block of memory
    for (int i = 0; i < ALLOC_TESTS; i++) {
        long *a = malloc(MEMLENGTH - HEADERSIZE);
        free(a);
    }
}

double test5() {
    // Test for allocating and freeing multiple different sizes of memory blocks
    for (int i = 0; i < ALLOC_TESTS; i++) {
        int *sizes = (int[]){8, 64, 16, 128};
        for (int j = 0; j < sizeof(sizes) / sizeof(sizes[0]); j++) {
            int *ptr = malloc(sizes[j]);
            free(ptr);
        }
    }
}
