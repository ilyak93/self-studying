#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

template<typename T>
class LinkedList {
public:
    class Iterator;

private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Node* head;

public:
    LinkedList() : head(nullptr) {}
    ~LinkedList();

    void insertAtBeginning(const T& value);
    void deleteValue(const T& value);
    void print() const;

    Iterator begin() {
        return Iterator(head, this);
    }

    Iterator end() {
        return Iterator(nullptr, this);
    }

    class Iterator {
        Node* current;
        LinkedList<T>* list;  // Pointer to the list being iterated over

    public:
        Iterator(Node* node, LinkedList<T>* lst) : current(node), list(lst) {}

        T& operator*() const {
            return current->data;
        }

        Iterator& operator++() {
            current = current->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };
};

template<typename T>
LinkedList<T>::~LinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

template<typename T>
void LinkedList<T>::insertAtBeginning(const T& value) {
    Node* newNode = new Node(value);
    newNode->next = head;
    head = newNode;
}

template<typename T>
void LinkedList<T>::deleteValue(const T& value) {
    Node **ptr = &head;
    while (*ptr && (*ptr)->data != value) {
        ptr = &((*ptr)->next);
    }
    if (*ptr) {
        Node* temp = *ptr;
        *ptr = (*ptr)->next;
        delete temp;
    }
}

template<typename T>
void LinkedList<T>::print() const {
    for (Node* node = head; node != nullptr; node = node->next) {
        std::cout << node->data << " -> ";
    }
    std::cout << "null" << std::endl;
}

#endif // LINKEDLIST_H


