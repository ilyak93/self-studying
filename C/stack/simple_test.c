#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"

// Function to copy an integer
void* copyInt(const void* src) {
    int* newInt = malloc(sizeof(int));
    if (newInt) {
        *newInt = *(int*)src;
    }
    return newInt;
}

// Function to free an integer
void freeInt(void* data) {
    free(data);
}

// Main function to demonstrate stack operations
int main() {
    Stack myStack = createStack(copyInt, freeInt);

    int a = 10;
    int b = 20;
    int c = 30;
    void* poppedData;

    push(myStack, &a);
    push(myStack, &b);
    push(myStack, &c);

    if (peek(myStack, &poppedData) == STACK_OK) {
        printf("Top element is %d\n", *(int*)poppedData);
    }

    if (pop(myStack, &poppedData) == STACK_OK) {
        printf("Popped element is %d\n", *(int*)poppedData);
        freeInt(poppedData);
    }

    if (peek(myStack, &poppedData) == STACK_OK) {
        printf("Top element is now %d\n", *(int*)poppedData);
    }

    // Free the remaining elements in the stack
    freeStack(myStack);

    return 0;
}