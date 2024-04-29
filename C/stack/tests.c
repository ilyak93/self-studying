#include "Stack.h"
#include <stdio.h>
#include <stdlib.h>

// Function to copy an integer (for testing)
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

// Simple assertion function to report test results
void assertTest(const char* testName, int condition) {
    if (condition) {
        printf("%s: PASS\n", testName);
    } else {
        printf("%s: FAIL\n", testName);
    }
}

// Test pushing and popping items
void testPushPop() {
    Stack s = createStack(copyInt, freeInt);
    int a = 10;
    int b = 20;
    push(s, &a);
    push(s, &b);

    int* topValue;
    StackStatus status = pop(s, (Element*)&topValue);
    assertTest("testPushPop - Should pop the last pushed value (20)", status == STACK_OK && *topValue == 20);
    freeInt(topValue); // Remember to free after popping

    status = pop(s, (Element*)&topValue);
    assertTest("testPushPop - Should pop the next value (10)", status == STACK_OK && *topValue == 10);
    freeInt(topValue); // Remember to free after popping

    freeStack(s);
}

// Test isEmpty function
void testIsEmpty() {
    Stack s = createStack(copyInt, freeInt);
    assertTest("testIsEmpty - Stack should be empty initially", isEmpty(s));

    int a = 10;
    push(s, &a);
    assertTest("testIsEmpty - Stack should not be empty after push", !isEmpty(s));

    int* topValue;
    pop(s, (Element*)&topValue);
    assertTest("testIsEmpty - Stack should be empty after pop", isEmpty(s));
    freeInt(topValue);

    freeStack(s);
}

// Test peek function
void testPeek() {
    Stack s = createStack(copyInt, freeInt);
    int a = 30;
    push(s, &a);

    int* topValue;
    StackStatus status = peek(s, (Element*)&topValue);
    assertTest("testPeek - Top value should be peekable and correct", status == STACK_OK && *topValue == 30);

    freeStack(s);
}

// Main function to run all tests
int main() {
    testPushPop();
    testIsEmpty();
    testPeek();
    return 0;
}