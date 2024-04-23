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

// Function to report test results
void reportTest(const char* testName, int passed) {
    if (passed) {
        printf("%s: PASS\n", testName);
    } else {
        printf("%s: FAIL\n", testName);
    }
}

// Define a helper function for comparing integers
int assertIntEqual(int expected, int actual) {
    return expected == actual;
}

void test_initStack() {
    Stack s;
    initStack(&s, copyInt, freeInt);
    reportTest("test_initStack", s.top == NULL && s.copyFunc != NULL && s.freeFunc != NULL);
}

void test_push() {
    Stack s;
    initStack(&s, copyInt, freeInt);
    int a = 10;
    push(&s, &a);
    int result = assertIntEqual(10, *(int*)s.top->data);
    freeStack(&s);
    reportTest("test_push", result);
}

void test_pop() {
    Stack s;
    initStack(&s, copyInt, freeInt);
    int a = 10, b = 20;
    void* data;

    push(&s, &a);
    push(&s, &b);

    pop(&s, &data);
    int result = assertIntEqual(20, *(int*)data) && (s.top != NULL && *(int*)s.top->data == 10);
    freeInt(data);
    freeStack(&s);
    reportTest("test_pop", result);
}

void test_peek() {
    Stack s;
    initStack(&s, copyInt, freeInt);
    int a = 10;
    void* data;

    push(&s, &a);
    peek(&s, &data);
    int result = assertIntEqual(10, *(int*)data);
    freeStack(&s);
    reportTest("test_peek", result);
}

void test_isEmpty() {
    Stack s;
    initStack(&s, copyInt, freeInt);
    int result = isEmpty(&s); // Should be true

    int a = 10;
    push(&s, &a);
    result &= !isEmpty(&s); // Should be false now

    freeStack(&s);
    reportTest("test_isEmpty", result);
}

void test_freeStack() {
    Stack s;
    initStack(&s, copyInt, freeInt);
    int a = 10, b = 20;

    push(&s, &a);
    push(&s, &b);
    freeStack(&s);

    // The real test would require memory checking tools, but for simplicity:
    reportTest("test_freeStack", s.top == NULL);
}

int main() {
    test_initStack();
    test_push();
    test_pop();
    test_peek();
    test_isEmpty();
    test_freeStack();
    return 0;
}