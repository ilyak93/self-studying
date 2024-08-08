#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>

// Define the structure for a tree node
typedef struct TreeNode {
    int value;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;


bool IsBinary(TreeNode* root, TreeNode* prev) {
    if(!root) return true;
    bool res1 = true;
    if(root->left) {
        if (root->left->value < root->value) res1 = IsBinary(root->left, prev);
        else return false;
    }
    if(!res1) return false;
    if(prev && root->value <= prev->value) return false;
    bool res2 = true;
    if(root->right) {
        if (root->right->value > root->value) res2 = IsBinary(root->right, root);
        else return false;
    }
    return res1 && res2;
}

// Function to create a new node
TreeNode* createNode(int value) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to print the tree (in-order traversal)
void printTree(TreeNode* root) {
    if (root != NULL) {
        printTree(root->left);
        printf("%d ", root->value);
        printTree(root->right);
    }
}

int main() {
    // Create the root node with value 2
    TreeNode* root = createNode(10);

    // Create left child with value 1
    root->left = createNode(5);

    // Create right child with value 3
    root->right = createNode(15);

    root->right->left = createNode(11);

    root->right->right = createNode(16);

    // Print the tree
    printf("The binary tree (in-order traversal): ");
    printTree(root);
    printf("\n");

    bool res = IsBinary(root, NULL);

    printf("The binary tree %d: ", res);

    // Free the allocated memory
    free(root->left);
    free(root->right);
    free(root);

    return 0;
}