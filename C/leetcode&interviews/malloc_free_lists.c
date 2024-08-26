#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#define MEMORY_SIZE (1ULL * 1024 * 1024 * 1024) // 1GB
#define BLOCK_SIZE 4 // 4 bytes per block
#define NUM_BLOCKS (MEMORY_SIZE / BLOCK_SIZE)

// Global memory pool
static uint8_t memory_pool[MEMORY_SIZE];

// Pointer to the start of the memory pool
static void* next_available_block = memory_pool;

static const void* start = memory_pool;
static const void* end = memory_pool + NUM_BLOCKS * sizeof(int);


// Pointer to the head of the free list
static void* free_list_head = NULL;

// Check if the system is 64-bit
#if UINTPTR_MAX == 0xffffffffffffffff
#define IS_64_BIT 1
#else
#define IS_64_BIT 0
#endif

// Function to get the upper 32 bits of the start pointer
static unsigned int getPrefix() {
#if IS_64_BIT
    return (unsigned int)((uintptr_t)memory_pool >> 32);
#else
    return 0; // Not used in 32-bit systems
#endif
}

// Function to store only the lower 32 bits of a pointer in an int array.
static void storePointer(int* array, void* ptr) {
#if IS_64_BIT
    *array = (int)((uintptr_t)ptr & 0xFFFFFFFF);
#else
    *array = (int)ptr;
#endif
}

// Function to restore a full pointer from a stored 32-bit value
static void* restorePointer(int* array) {
#if IS_64_BIT
    uint64_t prefix = (uint64_t)getPrefix() << 32;
    uint64_t suffix = (uint64_t)(unsigned int)(*array);
    return (void*)(prefix | suffix);
#else
    return (void*)(*array);
#endif
}

// Allocate a 4-byte block
void* custom_alloc() {
    if (free_list_head != NULL) {
        void* allocated = free_list_head;
        free_list_head = restorePointer((int*)free_list_head);
        return allocated;
    } else if (next_available_block < end) {
        void* new_block = next_available_block;
        next_available_block+=sizeof(int);
        return new_block;
    }
    return NULL; // Out of memory
}

// Free a 4-byte block
void custom_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }
    storePointer((int*)ptr, free_list_head);
    free_list_head = ptr;
}

// Example usage
int main() {
    printf("Running on %d-bit system\n", IS_64_BIT ? 64 : 32);

    int* a = (int*)custom_alloc();
    int* b = (int*)custom_alloc();
    int* c = (int*)custom_alloc();

    if (a && b && c) {
        *a = 1;
        *b = 2;
        *c = 3;

        printf("Allocated values: %d, %d, %d\n", *a, *b, *c);

        custom_free(b);
        custom_free(c);
        custom_free(a);

        // Allocate again to demonstrate reuse
        int* d = (int*)custom_alloc();
        int* e = (int*)custom_alloc();
        if (d && e) {
            printf("Reallocated addresses: %p, %p\n", (void*)d, (void*)e);
        }
        // the free list after the free looks like a->c->b->NULL
        // thus d will get a addr ; c->b->NULL ; e will get c addr; b->NULL;
        assert(d == a && e == c);
    } else {
        printf("Allocation failed\n");
    }

    return 0;
}