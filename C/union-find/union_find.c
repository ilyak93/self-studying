// File: union_find.c
#include "union_find.h"
#include <stdlib.h>

struct UnionFind {
    int* parent;
    int* size;
    UFelem* data;
    int count;
    copy_func copy;
    free_func free;
};

UnionFind uf_create(int count, copy_func cpy, free_func fr) {
    UnionFind uf = malloc(sizeof(*uf));
    if (!uf) return NULL;

    uf->count = count;
    uf->parent = malloc(sizeof(int) * count);
    uf->size = malloc(sizeof(int) * count);
    uf->data = malloc(sizeof(*uf->data) * count);
    uf->copy = cpy;
    uf->free = fr;

    if (!uf->parent || !uf->size || !uf->data) {
        uf_destroy(uf); // Clean up partially allocated memory
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        uf_make_set(uf, i, NULL);
    }
    return uf;
}

void uf_destroy(UnionFind uf) {
    if (uf) {
        for (int i = 0; i < uf->count; i++) {
            if (uf->data[i]) {
                uf->free(uf->data[i]);
            }
        }
        free(uf->parent);
        free(uf->size);
        free(uf->data);
        free(uf);
    }
}

UFStatus uf_make_set(UnionFind uf, int index, const void* data) {
    if (index < 0 || index >= uf->count) return UF_ERROR;
    uf->parent[index] = index;
    uf->size[index] = 1;
    uf->data[index] = data ? uf->copy(data) : NULL;
    return UF_OK;
}

UFStatus uf_union(UnionFind uf, int x, int y) {
    int rootX, rootY;
    if (uf_find(uf, x, &rootX) != UF_OK || uf_find(uf, y, &rootY) != UF_OK)
        return UF_ERROR;

    if (rootX != rootY) {
        if (uf->size[rootX] < uf->size[rootY]) {
            uf->parent[rootX] = rootY;
            uf->size[rootY] += uf->size[rootX];
        } else {
            uf->parent[rootY] = rootX;
            uf->size[rootX] += uf->size[rootY];
        }
    }
    return UF_OK;
}

UFStatus uf_find(const UnionFind uf, int x, int* root) {
    if (x < 0 || x >= uf->count) return UF_ERROR;
    if (uf->parent[x] != x) {
        int status = uf_find(uf, uf->parent[x], root);
        if (status != UF_OK) return status;
        uf->parent[x] = *root; // Mutates uf, need to consider implications
    } else {
        *root = x;
    }
    return UF_OK;
}

UFelem uf_get_data(const UnionFind uf, int index) {
    int root = uf_find(uf, index, &root); // Needs to handle return status
    return uf->data[root];
}

void uf_set_data(UnionFind uf, int index, const void* data) {
    if (uf->data[index]) {
        uf->free(uf->data[index]);
    }
    uf->data[index] = uf->copy(data);
}