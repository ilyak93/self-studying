#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
    int e;
    struct node* next;
} Node;

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode dummy;
    struct ListNode* current = &dummy;
    dummy.next = NULL;

    while (l1 != NULL && l2 != NULL) {
        if (l1->val < l2->val) {
            current->next = l1;
            l1 = l1->next;
        } else {
            current->next = l2;
            l2 = l2->next;
        }
        current = current->next;
    }

    if (l1 != NULL) {
        current->next = l1;
    } else {
        current->next = l2;
    }

    return dummy.next;
}

struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {
    if (listsSize == 0) return NULL;

    while (listsSize > 1) {
        int newSize = (listsSize + 1) / 2;
        for (int i = 0; i < listsSize / 2; i++) {
            lists[i] = mergeTwoLists(lists[i], lists[listsSize - 1 - i]);
        }
        listsSize = newSize;
    }

    return lists[0];
}

struct ListNode* createNode(int val) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

struct ListNode* createList(int start, int n) {
    struct ListNode dummy;
    struct ListNode* current = &dummy;
    for (int i = 0; i < n; i++) {
        current->next = createNode(start + i);
        current = current->next;
    }
    return dummy.next;
}

void printList(struct ListNode* head) {
    while (head) {
        printf("%d ", head->val);
        head = head->next;
    }
    printf("\n");
}

// Function declarations (assuming these are defined elsewhere)
struct ListNode* mergeKLists(struct ListNode** lists, int listsSize);

int main() {
    const int N = 4;
    struct ListNode* lists[N];

    // Create N lists, each with N elements
    for (int i = 0; i < N; i++) {
        lists[i] = createList(i * N + 1, N);
        printf("List %d: ", i + 1);
        printList(lists[i]);
    }

    // Merge the lists
    struct ListNode* result = mergeKLists(lists, N);

    // Print the result
    printf("Merged list: ");
    printList(result);

    // Free allocated memory (not implemented here for brevity)

    return 0;
}