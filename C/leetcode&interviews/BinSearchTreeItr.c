#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define MAX_NODES 100000 // Maximum number of nodes in the tree

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct {
    TreeNode** stack;
    int top;
    int capacity;
} BSTIterator;

BSTIterator* bSTIteratorCreate(TreeNode* root) {
    BSTIterator* obj = (BSTIterator*)malloc(sizeof(BSTIterator));
    int max_height = (int)(log2(MAX_NODES) + 1);
    obj->stack = (TreeNode**)malloc(max_height * sizeof(TreeNode*));
    obj->capacity = max_height;
    obj->top = -1;

    while (root) {
        obj->stack[++obj->top] = root;
        root = root->left;
    }

    return obj;
}

int bSTIteratorNext(BSTIterator* obj) {
    TreeNode* node = obj->stack[obj->top--];
    int result = node->val;

    node = node->right;
    while (node) {
        obj->stack[++obj->top] = node;
        node = node->left;
    }

    return result;
}

bool bSTIteratorHasNext(BSTIterator* obj) {
    return obj->top >= 0;
}

void bSTIteratorFree(BSTIterator* obj) {
    free(obj->stack);
    free(obj);
}

// Helper function to create a new node (for testing purposes)
TreeNode* newNode(int val) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->val = val;
    node->left = node->right = NULL;
    return node;
}

int main() {
    // Create a sample BST
    TreeNode* root = newNode(7);
    root->left = newNode(3);
    root->right = newNode(15);
    root->right->left = newNode(9);
    root->right->right = newNode(20);

    BSTIterator* obj = bSTIteratorCreate(root);

    printf("In-order traversal: ");
    while (bSTIteratorHasNext(obj)) {
        printf("%d ", bSTIteratorNext(obj));
    }
    printf("\n");

    bSTIteratorFree(obj);
    return 0;
}