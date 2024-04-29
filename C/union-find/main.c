// File: main.c
#include "union_find.h"
#include <stdio.h>
#include <stdlib.h>

void* copy_int(const void* data) {
    int* new_int = malloc(sizeof(int));
    *new_int = *(int*)data;
    return new_int;
}

void free_int(void* data) {
    free(data);
}

int main() {
    int n = 5;
    int data[5] = {10, 20, 30, 40, 50};

    UnionFind uf = uf_create(n, copy_int, free_int);
    if (!uf) {
        printf("Failed to create UnionFind.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (uf_make_set(uf, i, &data[i]) != UF_OK) {
            printf("Failed to make set for element %d\n", i);
        }
    }

    if (uf_union(uf, 0, 1) != UF_OK) {
        printf("Failed to union sets 0 and 1\n");
    }

    if (uf_union(uf, 2, 3) != UF_OK) {
        printf("Failed to union sets 2 and 3\n");
    }

    int leader;
    if (uf_find(uf, 1, &leader) == UF_OK) {
        printf("Leader of element 1 has data: %d\n", *(int*)uf_get_data(uf, leader));
    } else {
        printf("Failed to find leader for element 1\n");
    }

    uf_destroy(uf);
    return 0;
}