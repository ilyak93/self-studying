#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define MAX_NODES 2000 // Maximum number of nodes in the tree

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** levelOrder(struct TreeNode* root, int* returnSize, int** returnColumnSizes) {
    int max_height = MAX_NODES;
    int** res = (int**) malloc(sizeof(int*)*max_height);
    (*returnColumnSizes) = (int*) malloc (sizeof(int) * max_height);
    TreeNode** children;
    TreeNode* curChildren[1];
    curChildren[0] = root;
    children = curChildren;
    int childNextLevel = 0;
    if(root) childNextLevel = 1;
    *returnSize = 0;

    while(childNextLevel){
        int curChildNum = childNextLevel;
        childNextLevel = 0;
        int* curArray = (int*) malloc (sizeof(int) * curChildNum);
        int maxNext = 2*curChildNum;
        TreeNode** next = (TreeNode**) malloc(sizeof(TreeNode*) * maxNext);
        int nextItr = 0;
        for(int i = 0; i < curChildNum; i++){
            curArray[i] = children[i]->val;
            if(children[i]->left) {
                next[nextItr++] = children[i]->left;
                childNextLevel++;
            }
            if(children[i]->right) {
                next[nextItr++] = children[i]->right;
                childNextLevel++;
            }
        }
        (*returnColumnSizes)[*returnSize] = curChildNum;
        res[(*returnSize)++] = curArray;
        children = next;
    }
    if((*returnSize) == 0) {
        (*returnColumnSizes)[0] = 0;
    }
    return res;


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
    //TreeNode* root = newNode(3);
    //root->left = newNode(9);
    //root->right = newNode(20);
    //root->right->left = newNode(15);
    //root->right->right = newNode(7);
    int* returnColumnSizes;
    int retSize = -1;
    TreeNode* root = NULL;
    int **res = levelOrder(root, &retSize, &returnColumnSizes);

    for(int i = 0; i < retSize; i++){
        for(int j = 0; j < returnColumnSizes[i]; j++){
            printf("%d ", res[i][j]);
        }
        printf("\n");
    }

    return 0;
}