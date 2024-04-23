// stack.c
#include "Stack.h"
#include <stdio.h>
#include <stdlib.h>

// Function to create a new node

static Node* createNode(Element data, CopyFunction copyFunc) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    if (!newNode) {
        return NULL;
    }
    newNode->data = copyFunc(data);
    if (!newNode->data) {
        free(newNode);
        return NULL;
    }
    newNode->next = NULL;
    return newNode;
}

// Initialize the stack
void initStack(Stack* s, CopyFunction copyFunc, FreeFunction freeFunc) {
    s->top = NULL;
    s->copyFunc = copyFunc;
    s->freeFunc = freeFunc;
}

// Check if the stack is empty
bool isEmpty(Stack* s) {
    return s->top == NULL;
}

// Push an element to the stack
StackStatus push(Stack* s, Element data) {
    Node* newNode = createNode(data, s->copyFunc);
    if (newNode == NULL) {
        return STACK_OVERFLOW;
    }
    newNode->next = s->top;
    s->top = newNode;
    return STACK_OK;
}

// Pop an element from the stack
StackStatus pop(Stack* s, Element* data) {
    if (isEmpty(s)) {
        return STACK_EMPTY;
    }
    Node* temp = s->top;
    *data = temp->data;
    s->top = temp->next;
    free(temp);
    return STACK_OK;
}

// Peek the top element of the stack
StackStatus peek(Stack* s, Element* data) {
    if (isEmpty(s)) {
        return STACK_EMPTY;
    }
    *data = s->top->data;
    return STACK_OK;
}

// Free the entire stack
void freeStack(Stack* s) {
    Node* current = s->top;
    Node* temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        s->freeFunc(temp->data);
        free(temp);
    }
    s->top = NULL;
}