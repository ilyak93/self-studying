// File: union_find.h
#ifndef UNION_FIND_H
#define UNION_FIND_H

typedef void* UFelem;

// Function pointer types for copying and freeing data
typedef UFelem (*copy_func)(const void* data);
typedef void (*free_func)(void* data);

typedef struct UnionFind* UnionFind;

// Status enumeration for error handling
typedef enum {
    UF_OK,
    UF_ERROR,
    UF_NO_MEMORY,
    UF_NOT_FOUND
} UFStatus;

// Function declarations
UnionFind uf_create(int count, copy_func cpy, free_func fr);
void uf_destroy(UnionFind uf);
UFStatus uf_make_set(UnionFind uf, int index, const void* data);
UFStatus uf_union(UnionFind uf, int x, int y);
UFStatus uf_find(const UnionFind uf, int x, int* root);
UFelem uf_get_data(const UnionFind uf, int index);
void uf_set_data(UnionFind uf, int index, const void* data);

#endif // UNION_FIND_H
