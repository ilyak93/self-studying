//
// Created by YOSSI on 4/30/2024.
//

#ifndef CPPPRACTICE_BTREENODE_HPP
#define CPPPRACTICE_BTREENODE_HPP

#include "BiNode.hpp"
#include <iostream>

template<typename T, int B, typename Comparator = std::less<T>>
class BTreeNode {
private:
    static const int minKeys = (B + 1) / 2 - 1;  // ceil(B/2) - 1
    static const int maxKeys = B - 1;  // B - 1
    Node<T>* keysHead;
    Node<BTreeNode*>* childrenHead;
    bool leaf;

public:
    BTreeNode(bool _leaf) : leaf(_leaf), keysHead(nullptr), childrenHead(nullptr) {}

    // Methods to manage keys
    void insertKey(T k, Comparator comp = Comparator());
    bool isFull() const { return countKeys() == maxKeys; }
    Node<T>* getKeysHead() const { return keysHead; }

    // Methods to manage children
    void splitChild(Node<BTreeNode*>* childNode, Comparator comp = Comparator());
    Node<BTreeNode*>* getChildrenHead() const { return childrenHead; }
    void insertChild(BTreeNode* child);

    // Utilities
    void traverse();
    BTreeNode* search(T k, Comparator comp = Comparator());
    int countKeys() const;
    Node<BTreeNode*>* findChildLink(BTreeNode* child);

    // Additional public interface methods as needed...
};

template<typename T, int B, typename Comparator>
void BTreeNode<T, B, Comparator>::insertKey(T k, Comparator comp) {
    Node<T>* current = keysHead;
    Node<T>* previous = nullptr;

    while (current && comp(current->data, k)) {
        previous = current;
        current = current->next;
    }

    Node<T>* newNode = new Node<T>(k);
    newNode->next = current;
    if (current) {
        newNode->prev = current->prev;
        current->prev = newNode;
    }
    if (previous) {
        previous->next = newNode;
    } else {
        keysHead = newNode;
    }

    if (!leaf) {
        Node<BTreeNode*>* childNode = childrenHead;
        while (childNode && childNode->next && childNode->next->data != current) {
            childNode = childNode->next;
        }
        if (childNode->data->isFull()) {
            splitChild(childNode);
            if (comp(childNode->next->data->getKeysHead()->data, k)) {
                childNode = childNode->next;
            }
        }
        childNode->data->insertKey(k);
    }
}

template<typename T, int B, typename Comparator>
void BTreeNode<T, B, Comparator>::splitChild(Node<BTreeNode*>* childNode, Comparator comp) {
    BTreeNode* y = childNode->data;
    BTreeNode* z = new BTreeNode(y->leaf);
    Node<T>* mid = y->keysHead;

    // Find the middle key to move up
    for (int i = 0; i < B / 2 - 1; ++i) {
        mid = mid->next;
    }

    // Create new node for keys greater than the middle key
    z->keysHead = mid->next;
    mid->next->prev = nullptr;
    mid->next = nullptr;

    // Transfer children to new node
    if (!y->leaf) {
        z->childrenHead = mid->next->childrenHead;
        mid->next->childrenHead->prev = nullptr;
        mid->next->childrenHead = nullptr;
    }

    // Insert new key and child into parent
    Node<T>* newKeyNode = new Node<T>(mid->data);
    newKeyNode->next = childNode->next;
    newKeyNode->prev = childNode;
    if (childNode->next) {
        childNode->next->prev = newKeyNode;
    }
    childNode->next = newKeyNode;

    Node<BTreeNode*>* newChildNode = new Node<BTreeNode*>(z);
    newChildNode->next = childNode->next->childrenHead;
    newChildNode->prev = childNode;
    if (childNode->next->childrenHead) {
        childNode->next->childrenHead->prev = newChildNode;
    }
    childNode->next->childrenHead = newChildNode;
}

template<typename T, int B, typename Comparator>
int BTreeNode<T, B, Comparator>::countKeys() const {
    int count = 0;
    Node<T>* current = keysHead;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

template<typename T, int B, typename Comparator>
BTreeNode<T, B, Comparator>* BTreeNode<T, B, Comparator>::search(T k, Comparator comp) {
    BTreeNode<T, B, Comparator>* current = this;

    while (current != nullptr) {
        Node<T>* key = current->keysHead;

        // Move to the first key greater than or equal to k
        while (key != nullptr && comp(key->data, k)) {
            key = key->next;
        }

        // If the found key is equal to k, return this node
        if (key != nullptr && key->data == k) {
            return current;
        }

        // If the key is not found here and this is a leaf node
        if (current->leaf) {
            return nullptr;
        }

        // Otherwise, move to the appropriate child
        Node<BTreeNode*>* child = current->childrenHead;
        while (child->next != nullptr && child->next->data->keysHead->data <= k) {
            child = child->next;
        }
        current = child->data;
    }

    return nullptr;
}

template<typename T, int B, typename Comparator>
Node<BTreeNode<T, B, Comparator>*>* BTreeNode<T, B, Comparator>::findChildLink(BTreeNode* child) {
    Node<BTreeNode*>* link = childrenHead;
    while (link != nullptr && link->data != child) {
        link = link->next;
    }
    return link;
}




#endif //CPPPRACTICE_BTREENODE_HPP