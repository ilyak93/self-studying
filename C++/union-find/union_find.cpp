#include "union_find.hpp"

// UnionFind.cpp
template<typename T>
UnionFind<T>::UnionFind(int n) : n(n) {
    parent = new int[n];
    size = new int[n];
    data = new T[n];
    for (int i = 0; i < n; ++i) {
        makeSet(i, T{}); // Initialize with default-constructed T
    }
}

template<typename T>
UnionFind<T>::~UnionFind() {
    delete[] parent;
    delete[] size;
    delete[] data;
}

template<typename T>
void UnionFind<T>::makeSet(int i, const T& value) {
    parent[i] = i;
    size[i] = 1;
    data[i] = value;
}

template<typename T>
int UnionFind<T>::find(int u) {
    if (parent[u] != u) {
        parent[u] = find(parent[u]); // Path compression
    }
    return parent[u];
}

template<typename T>
void UnionFind<T>::unionSets(int u, int v) {
    int rootU = find(u);
    int rootV = find(v);
    if (rootU != rootV) {
        if (size[rootU] < size[rootV]) {
            parent[rootU] = rootV;
            size[rootV] += size[rootU];
        } else {
            parent[rootV] = rootU;
            size[rootU] += size[rootV];
        }
    }
}

template<typename T>
T& UnionFind<T>::getData(int i) {
    return data[find(i)];  // Return data of the set representative
}

template<typename T>
void UnionFind<T>::setData(int i, const T& value) {
    data[i] = value;
}