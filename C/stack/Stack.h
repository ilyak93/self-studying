// stack.h
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

typedef void* Element;

typedef void* (*CopyFunction)(const void*);
typedef void (*FreeFunction)(void*);

// Node structure for the stack
typedef struct Node {
    Element data;
    struct Node* next;
} Node;



// Stack structure
typedef struct Stack {
    Node* top;
    CopyFunction copyFunc;
    FreeFunction freeFunc;
} Stack;

// Error codes
typedef enum {
    STACK_OK,
    STACK_ERROR,
    STACK_EMPTY,
    STACK_OVERFLOW
} StackStatus;

// Function prototypes
void initStack(Stack* s, CopyFunction copyFunc, FreeFunction freeFunc);
bool isEmpty(Stack* s);
StackStatus push(Stack* s, Element data);
StackStatus pop(Stack* s, Element* data);
StackStatus peek(Stack* s, Element* data);
void freeStack(Stack* s);
void* copyInt(const void* src);
void freeInt(void* data);

#endif // STACK_