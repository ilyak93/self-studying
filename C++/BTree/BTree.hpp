//
// Created by YOSSI on 4/30/2024.
//

#ifndef CPPPRACTICE_BTREE_HPP
#define CPPPRACTICE_BTREE_HPP

#include "BTreeNode.hpp"

template<typename T, int B, typename Comparator = std::less<T>>
class BTree {
private:
    BTreeNode<T, B, Comparator>* root;

public:
    BTree() : root(nullptr) {}

    void traverse() {
        if (root) root->traverse();
    }

    BTreeNode<T, B, Comparator>* search(T k, Comparator comp = Comparator()) {
        return root ? root->search(k, comp) : nullptr;
    }

    void insert(T k);
};

template<typename T, int B, typename Comparator>
void BTree<T, B, Comparator>::insert(T k) {
    if (root == nullptr) {
        root = new BTreeNode<T, B, Comparator>(true);
        root->insertKey(k);
        return;
    }

    // Start from the root and go down to the leaf
    BTreeNode<T, B, Comparator>* current = root;
    Node<BTreeNode<T, B, Comparator>*>* parentLink = nullptr; // To hold the parent's child link to current node

    // Find the correct node for insertion
    while (!current->leaf) {
        Node<T>* key = current->keysHead;
        Node<BTreeNode<T, B, Comparator>*>* child = current->childrenHead;

        // Find the child node to go to
        while (key != nullptr && comp(key->data, k)) {
            key = key->next;
            child = child->next;
        }

        parentLink = child;
        current = child->data;
    }

    // Now current is the leaf node where the key should be inserted
    current->insertKey(k);

    // Check if the node is full and needs splitting
    while (current->countKeys() > BTreeNode<T, B, Comparator>::maxKeys) {
        if (parentLink == nullptr) {
            // Split the root
            BTreeNode<T, B, Comparator>* newRoot = new BTreeNode<T, B, Comparator>(false);
            newRoot->childrenHead = new Node<BTreeNode<T, B, Comparator>*>(root); // Old root becomes the left child
            newRoot->splitChild(newRoot->childrenHead);
            root = newRoot;
            break;
        } else {
            // Split non-root node
            BTreeNode<T, B, Comparator>* parentNode = parentLink->prev->data; // Parent node
            parentNode->splitChild(parentLink);

            // Move up to the parent node
            current = parentNode;
            parentLink = parentNode->findChildLink(current);
        }
    }
}

#endif //CPPPRACTICE_BTREE_HPP
