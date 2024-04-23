// Main function to demonstrate list usage
#include "list.h"

// Example functions for handling integers
Element copyInt(const void* data) {
    int* newInt = malloc(sizeof(int));
    *newInt = *(const int*)data;
    return newInt;
}

void freeInt(void* data) {
    free(data);
}

int compareInt(const void* a, const void* b) {
    return *(const int*)a - *(const int*)b;
}

void printInt(const void* data) {
    printf("%d", *(const int*)data);
}

int main() {
    List myList = {
            .head = NULL,
            .copy = copyInt,
            .freeElem = freeInt,
            .compare = compareInt,
            .print = printInt
    };

    int data1 = 10, data2 = 20, data3 = 30;

    insertAtBeginning(&myList, &data1);
    insertAtBeginning(&myList, &data2);
    insertAtBeginning(&myList, &data3);

    printList(&myList);

    // Example of using FOREACH macro
    printf("Iterating with FOREACH macro:\n");
    FOREACH(&myList, node) {
        myList.print(node->data);
        printf(" -> ");
    }
    printf("NULL\n");

    deleteNode(&myList, &data2);

    printList(&myList);

    // Clean up the remaining list
    while (myList.head != NULL) {
        deleteNode(&myList, myList.head->data);
    }

    return 0;
}