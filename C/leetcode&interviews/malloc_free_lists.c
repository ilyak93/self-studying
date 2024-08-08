#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define MEMORY_SIZE (1ULL * 1024 * 1024 * 1024) // 1GB
#define BLOCK_SIZE 4 // 4 bytes per block
#define NUM_BLOCKS (MEMORY_SIZE / BLOCK_SIZE)

// Global memory pool
static uint8_t memory_pool[MEMORY_SIZE];

// Pointer to the start of the memory pool
static void* start = memory_pool;

// Next available block index
static size_t next_available_block = 0;

// Free block structure (used for both 32-bit and 64-bit systems)
typedef struct FreeBlock {
    struct FreeBlock* next;
} FreeBlock;

// Check if the system is 64-bit
#if UINTPTR_MAX == 0xffffffffffffffff
#define IS_64_BIT 1
#else
#define IS_64_BIT 0
#endif

#if IS_64_BIT
// 64-bit specific functions
static unsigned int getPrefix() {
    return (unsigned int)((uintptr_t)start >> 32);
}

static void storePointer(int* array, void* ptr) {
    *array = (int)((uintptr_t)ptr & 0xFFFFFFFF);
}

static void* restorePointer(int* array) {
    uint64_t prefix = (uint64_t)getPrefix() << 32;
    uint64_t suffix = (uint64_t)(unsigned int)(*array);
    return (void*)(prefix | suffix);
}

#define STORE_NEXT(block, next) storePointer((int*)block, next)
#define GET_NEXT(block) restorePointer((int*)block)

#else
// 32-bit specific macros
#define STORE_NEXT(block, next) (((FreeBlock*)block)->next = (FreeBlock*)next)
#define GET_NEXT(block) (((FreeBlock*)block)->next)

#endif

// Pointer to the head of the free list
static void* free_list_head = NULL;

// Allocate a 4-byte block
void* custom_alloc() {
    if (free_list_head != NULL) {
        void* allocated = free_list_head;
        free_list_head = GET_NEXT(free_list_head);
        return allocated;
    } else if (next_available_block < NUM_BLOCKS) {
        void* new_block = &memory_pool[next_available_block * BLOCK_SIZE];
        next_available_block++;
        return new_block;
    }
    return NULL; // Out of memory
}

// Free a 4-byte block
void custom_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }
    STORE_NEXT(ptr, free_list_head);
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
    } else {
        printf("Allocation failed\n");
    }

    return 0;
}