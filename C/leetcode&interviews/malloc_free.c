#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

/*
This code implements a custom memory allocator that manages a 1GB memory space.
It uses a strategy where the first half of the memory is used for allocation,
and the second half is used as a free list.
The allocator can handle 64-bit pointers but stores only the lower 32 bits to save space.
*/

// Define the size of memory in integers (1GB / 4 bytes per int)
#define MEMSIZE_INT 1073741824 / 4

// Declare a global array to represent our memory
int MEM[MEMSIZE_INT];

// Pointers to keep track of the start and end of our memory
int* start = MEM;
int* end = &MEM[MEMSIZE_INT];



// Reset the memory management pointers
static void ResetMem(){
    start = MEM;
    end = &MEM[MEMSIZE_INT];
}

// Getter functions for memory addresses
static void* GetMEMStartAdr(){ return MEM; }
static void* GetMEMEndAdr(){ return &MEM[MEMSIZE_INT]; }
static void* GetCurStartAdr(){ return (void*)start; }
static void* GetCurEndAdr(){ return (void*)end; }

// Function to get the upper 32 bits of the start pointer
// this 32 bits are permanent from the moment MEM is created,
// because we use only 1GB of memory, so the 32 most significant bits
// doesnt change.
static uint32_t getPrefix() {
    return (uint32_t)((uint64_t)start >> 32);
}

// Function to store only the lower 32 bits of a pointer in an int array.
static void storePointer(int* array, int index, void* ptr) {
    array[index] = (int)((uint64_t)ptr & 0xFFFFFFFF);
}

// Function to restore a full 64-bit pointer from a stored 32-bit value,
// concating the prefix of the most significant 32 bits using logical or.
static void* restorePointer(int* array, int index) {
    uint64_t prefix = (uint64_t)getPrefix() << 32;
    uint64_t suffix = (uint64_t)(uint32_t)array[index];
    return (void*)(prefix | suffix);
}

// Memory allocation function
void* MyMalloc1(){
    //if blocks were freed before
    if(end < &MEM[MEMSIZE_INT]) {
        // Allocate from the free list (second half)
        void* ptr = restorePointer(end, 0);
        end++;
        return ptr;
    }
    // else
    if(start < &MEM[MEMSIZE_INT/2]) {
        // Allocate from the first half
        int *tmp = start;
        start++;
        return (void*)tmp;
    }
    return NULL; // if no freed blocks and index is greater or even the half - Out of memory
}

// Memory deallocation function
void MyFree1(int* ptr){
    // Check if ptr is within bounds
    if(ptr >= &MEM[MEMSIZE_INT] || ptr < MEM) {
        fprintf(stderr, "Out of bounds of 1GB memory\n");
        return;
    }

    // Check if ptr is in the allocated region
    if(ptr >= start) {
        fprintf(stderr, "Trying to free not allocated memory\n");
        return;
    }

    // Add ptr to the free list by saving the bottom 32 bits
    storePointer(--end, 0, ptr);
}

// Test simple allocation and freeing
int testSimpleAllocFree()
{
    int* allocated = (int*) MyMalloc1();
    assert(allocated == GetMEMStartAdr());
    assert(allocated+1 == GetCurStartAdr());
    MyFree1(allocated);
    assert(end == (int*)GetMEMEndAdr() - 1);
    void* freedPtr = restorePointer(end, 0);
    assert(freedPtr == allocated);
    ResetMem();
    return 0;
}

// Test allocating, freeing, and reallocating
int testAllocFreeAllocFromFreed()
{
    int* allocated = (int *) MyMalloc1();
    assert(allocated != NULL);
    MyFree1(allocated);
    int* reallocated = (int *) MyMalloc1();
    assert(reallocated == allocated);
    MyFree1(reallocated);
    int* reallocated2 = (int *) MyMalloc1();
    assert(reallocated2 == reallocated);
    ResetMem();
    return 0;
}

// Test out of bounds allocation
int testOutOfBoundsAlloc() {
    int* startAdr = (int *) GetMEMStartAdr();
    MyFree1(--startAdr);
    printf("Line %d printed Out of bounds error as expected\n", __LINE__-1);
    int* endAdr = (int *) GetMEMEndAdr();
    MyFree1(endAdr);
    printf("Line %d printed Out of bounds error as expected\n", __LINE__-1);
    return 0;
}

// Test allocating all available memory
int testFullMEMAlloc() {
    int** allocated = malloc(sizeof(int*) * (MEMSIZE_INT / 2));
    int* start = (int*)GetMEMStartAdr();
    for(int i = 0; i < MEMSIZE_INT / 2; i++) {
        allocated[i] = (int *) MyMalloc1();
        assert(allocated[i] == start++);
    }
    int* nullAlloc = (int *) MyMalloc1();
    assert(nullAlloc == NULL);
    int* curAdr = (int*)GetCurStartAdr();
    int* HalfAdr = &(((int*)GetMEMStartAdr())[MEMSIZE_INT / 2]);
    assert(curAdr == HalfAdr);
    free(allocated);

    ResetMem();
    int* curAdrAfterReset = (int*)GetCurStartAdr();
    int* startAdr = (int*)GetMEMStartAdr();
    assert(curAdrAfterReset == startAdr);
    int* curEndAdrAfterReset = (int*)GetCurEndAdr();
    int* EndAdr = (int*)GetMEMEndAdr();
    assert(curEndAdrAfterReset == EndAdr);
    return 0;
}

// Test allocating all memory and then freeing it
int testFullMEMAllocFree() {
    int** allocated = malloc(sizeof(int*) * (MEMSIZE_INT / 2));
    for(int i = 0; i < MEMSIZE_INT / 2; i++) {
        allocated[i] = (int *) MyMalloc1();
        assert(allocated[i] != NULL);
    }
    int* nullAlloc = (int *) MyMalloc1();
    assert(nullAlloc == NULL);
    int* start = (int*)GetMEMStartAdr();
    for(int i = 0; i < MEMSIZE_INT / 2; i++) {
        MyFree1(start++);
    }
    int* start2 = (int*)GetCurStartAdr() - 1;
    int* end2 = (int*)GetCurEndAdr();
    for(int i = 0; i < MEMSIZE_INT / 2; i++) {
        assert(start2-- == restorePointer(end2++, 0));
    }
    int* curAdr = (int*)GetCurStartAdr();
    int* HalfAdr = &(((int*)GetMEMStartAdr())[MEMSIZE_INT / 2]);
    assert(curAdr == HalfAdr);
    int* curEndAdrAfterReset = (int*)GetCurEndAdr();
    int* EndAdr = HalfAdr;
    assert(curEndAdrAfterReset == EndAdr);
    MyFree1(start);
    printf("Line %d printed freeing not allocated memory error as expected\n", __LINE__-1);
    free(allocated);
    ResetMem();
    return 0;
}

// Test allocating all memory and freeing it in a different pattern
int testFullMEMAllocFree2() {
    int** allocated = malloc(sizeof(int*) * (MEMSIZE_INT / 2));
    for(int i = 0; i < MEMSIZE_INT / 2; i++) {
        allocated[i] = (int *) MyMalloc1();
        assert(allocated[i] != NULL);
    }
    int* start = (int*)GetMEMStartAdr();
    for(int i = 0; i < MEMSIZE_INT / 4; i++) {
        MyFree1(start);
        start += 2;
    }
    start = (int*)GetMEMStartAdr() + 1;
    for(int i = 0; i < MEMSIZE_INT / 4; i++) {
        MyFree1(start);
        start += 2;
    }
    int* curAdr = (int*)GetCurStartAdr();
    int* HalfAdr = &(((int*)GetMEMStartAdr())[MEMSIZE_INT / 2]);
    assert(curAdr == HalfAdr);
    int* curEndAdrAfterReset = (int*)GetCurEndAdr();
    int* EndAdr = HalfAdr;
    assert(curEndAdrAfterReset == EndAdr);
    int* start2 = start - 2;
    int* end2 = (int*)GetCurEndAdr();
    for(int i = 0; i < MEMSIZE_INT / 4; i++) {
        assert(start2 == restorePointer(end2++, 0));
        start2 -= 2;
    }
    int* start3 = start - 1 - 2;
    for(int i = 0; i < MEMSIZE_INT / 4; i++) {
        assert(start3 == restorePointer(end2++, 0));
        start3 -= 2;
    }
    free(allocated);
    ResetMem();
    return 0;
}

// Test freeing not allocated memory
int testNotAllocatedFree() {
    int* startAdr = (int *) GetMEMStartAdr();
    MyFree1(startAdr);
    printf("Line %d printed freeing not allocated memory error as expected\n", __LINE__-1);
    return 0;
}

int main() {
    //checks : that allocated ptr is the first MEM ptr
    //       : that freed ptr is allocated ptr
    //       : pointers of start and end of MEM correctly changed.
    assert(testSimpleAllocFree() == 0);
    //checks : that not allocated ptr (which is not out of bounds)
    //         cannot be freed (printing error into stderr).
    //         and doesn't change the state of the memory.
    assert(testNotAllocatedFree() == 0);
    //checks : that reallocated ptr (allocated, freed and allocated again)
    //         is the same ptr as firstly allocated
    //         check repeated for another free and reallocation
    assert(testAllocFreeAllocFromFreed() == 0);
    //checks : that out of bounds ptr (out of bound error precede
    //         cannot be freed (printing error into stderr).
    //         and doesn't change the state of the memory.
    assert(testOutOfBoundsAlloc() == 0);
    //checks : that allocated first half of the 1GB memory is a sequence of
    //         pointers started at MEM ptr.
    //         that additional allocation above that returns with NULL
    //         that pointer at the end is at the half of the array.
    //         that ResetMem does what it should (resets pointers).
    assert(testFullMEMAlloc() == 0);
    //checks : that the freed half is a sequence such that
    // the last freed is pointed by the end ptr and will be the first
    // allocated (like a stack), i.e the freed is the exact sequence
    // right to left like the allocated left to right.
    assert(testFullMEMAllocFree() == 0);
    //checks : like the previous check, but with different pattern,
    //         firstly we allocate the the first half as a sequence, next
    //         free odd addresses are in it, then even,
    //         and check that they are in reverse order starting at the half
    //         and continuing to the right.
    assert(testFullMEMAllocFree2() == 0);
    printf("All tests passed successfully!\n");
    return 0;
}