// UnionFind.hpp
#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

template<typename T>
class UnionFind {
private:
    int* parent;   // Array to store the parent of each element
    int* size;     // Array to store the size of each set
    T* data;       // Array to store data associated with each element
    int n;         // Number of elements in Union-Find

public:
    UnionFind(int n);
    ~UnionFind();
    void makeSet(int i, const T& value);
    int find(int u);
    void unionSets(int u, int v);
    T& getData(int i);
    void setData(int i, const T& value);
};

#include "union_find.cpp"

#endif // UNIONFIND_HPP
