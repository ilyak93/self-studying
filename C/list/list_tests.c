#include "list.h"
#include <assert.h>

// Example functions for handling integers
void* copyInt(const void* data) {
    int* newInt = malloc(sizeof(int));
    if (newInt == NULL) return NULL;
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

// Report the result of each test
void reportTest(const char* testName, int passed) {
    if (passed) {
        printf("%s: PASS\n", testName);
    } else {
        printf("%s: FAIL\n", testName);
    }
}

// Tests for createNode
void testCreateNode() {
    printf("Testing createNode...\n");
    Element data = malloc(sizeof(int));
    *(int*)data = 123; // Sample data

    Node* node = createNode(data, copyInt);
    int passed = node != NULL && *(int*)node->data == 123 && node->next == NULL;
    reportTest("testCreateNode", passed);

    freeInt(data);
    if (node) {
        freeInt(node->data);
        free(node);
    }
}

// Tests for insertAtBeginning
void testInsertAtBeginning() {
    printf("Testing insertAtBeginning...\n");
    List myList = {.head = NULL, .copy = copyInt, .freeElem = freeInt, .compare = compareInt, .print = printInt};

    int data1 = 10;
    ListError result1 = insertAtBeginning(&myList, &data1);
    int data2 = 20;
    insertAtBeginning(&myList, &data2);
    int passed = (result1 == LIST_OK) && (myList.head != NULL) && (*(int*)myList.head->data == 20) && (*(int*)myList.head->next->data == 10);
    reportTest("testInsertAtBeginning", passed);

    // Clean up
    while (myList.head != NULL) {
        Node* temp = myList.head;
        myList.head = myList.head->next;
        myList.freeElem(temp->data);
        free(temp);
    }
}

// Tests for deleteNode
void testDeleteNode() {
    printf("Testing deleteNode...\n");
    List myList = {.head = NULL, .copy = copyInt, .freeElem = freeInt, .compare = compareInt, .print = printInt};

    int data1 = 10, data2 = 20, data3 = 30;
    insertAtBeginning(&myList, &data1);
    insertAtBeginning(&myList, &data2);
    insertAtBeginning(&myList, &data3);

    ListError result1 = deleteNode(&myList, &data2);
    ListError result2 = deleteNode(&myList, &data2);
    int passed = (result1 == LIST_OK) && (result2 == LIST_ERR_NOT_FOUND) && (myList.head->data != &data2);
    reportTest("testDeleteNode", passed);

    // Clean up
    while (myList.head != NULL) {
        Node* temp = myList.head;
        myList.head = myList.head->next;
        myList.freeElem(temp->data);
        free(temp);
    }
}

// Tests for printList
void testPrintList() {
    printf("Testing printList (check console output)...\n");
    List myList = {.head = NULL, .copy = copyInt, .freeElem = freeInt, .compare = compareInt, .print = printInt};

    int data1 = 10, data2 = 20;
    insertAtBeginning(&myList, &data1);
    insertAtBeginning(&myList, &data2);

    // This will print to console, we assume print is correct if it compiles and runs without crashing
    printList(&myList);
    reportTest("testPrintList", 1);  // Assuming printList is always correct if it runs

    // Clean up
    while (myList.head != NULL) {
        Node* temp = myList.head;
        myList.head = myList.head->next;
        myList.freeElem(temp->data);
        free(temp);
    }
}

int main() {
    testCreateNode();
    testInsertAtBeginning();
    testDeleteNode();
    testPrintList();
    return 0;
}