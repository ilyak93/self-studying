// main.cpp
#include "union_find.hpp"
#include <iostream>

int main() {
    UnionFind<std::string> uf(10);

    // Example operations with associated data
    for (int i = 0; i < 10; ++i) {
        uf.makeSet(i, "initial" + std::to_string(i)); // Set initial data
    }

    uf.unionSets(1, 2);
    uf.unionSets(2, 3);
    uf.unionSets(4, 5);
    uf.unionSets(5, 6);
    uf.unionSets(6, 7);

    std::cout << "Data of element 1: " << uf.getData(1) << std::endl;
    std::cout << "Data of element 2: " << uf.getData(2) << std::endl;
    std::cout << "Data of element 3: " << uf.getData(3) << std::endl;
    std::cout << "Data of element 4: " << uf.getData(4) << std::endl;
    std::cout << "Data of element 5: " << uf.getData(5) << std::endl;
    std::cout << "Data of element 6: " << uf.getData(6) << std::endl;

    // Changing data of a set
    uf.setData(4, "NewData");
    std::cout << "Updated Data of element 5 group repr: " << uf.getData(4) << std::endl;

    // Changing data of a set
    uf.setData(5, "NewData");
    std::cout << "Updated Data of element 6 group repr: " << uf.getData(5) << std::endl;

    // Changing data of a set
    uf.setData(6, "NewData");
    std::cout << "Updated Data of element 7 group repr: " << uf.getData(6) << std::endl;


    return 0;
}