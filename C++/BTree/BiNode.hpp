//
// Created by YOSSI on 4/30/2024.
//

#ifndef CPPPRACTICE_BINODE_HPP
#define CPPPRACTICE_BINODE_HPP


template<typename T>
struct Node {
    T data;
    Node<T>* prev;
    Node<T>* next;

    Node(T val) : data(val), prev(nullptr), next(nullptr) {}
};

#endif //CPPPRACTICE_BINODE_HPP
