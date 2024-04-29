// stack.c
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

 struct stack_t {
    Node top;
    CopyFunction copyFunc;
    FreeFunction freeFunc;
} stack_t;

// Function to create a new stack
Stack createStack(CopyFunction copyFunc, FreeFunction freeFunc) {
    Stack s = (Stack) malloc(sizeof(*s));
    if (!s) {
        fprintf(stderr, "Failed to allocate memory for the stack\n");
        exit(EXIT_FAILURE);
    }
    s->top = NULL;
    s->copyFunc = copyFunc;
    s->freeFunc = freeFunc;
    return s;
}

// Function to create a new node
static Node createNode(Element data, CopyFunction copyFunc) {
    Node newNode = (Node) malloc(sizeof(*newNode));
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

// Check if the stack is empty
bool isEmpty(Stack s) {
    return s->top == NULL;
}

// Push an element to the stack
StackStatus push(Stack s, Element data) {
    Node newNode = createNode(data, s->copyFunc);
    if (newNode == NULL) {
        return STACK_OVERFLOW;
    }
    newNode->next = s->top;
    s->top = newNode;
    return STACK_OK;
}

// Pop an element from the stack
StackStatus pop(Stack s, Element* data) {
    if (isEmpty(s)) {
        return STACK_EMPTY;
    }
    Node temp = s->top;
    *data = temp->data;
    s->top = temp->next;
    free(temp);
    return STACK_OK;
}

// Peek the top element of the stack
StackStatus peek(Stack s, Element* data) {
    if (isEmpty(s)) {
        return STACK_EMPTY;
    }
    *data = s->top->data;
    return STACK_OK;
}

// Free the entire stack
void freeStack(Stack s) {
    Node current = s->top;
    Node temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        s->freeFunc(temp->data);
        free(temp);
    }
    s->top = NULL;
}