// stack.h
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

typedef void* Element;

typedef void* (*CopyFunction)(const void*);
typedef void (*FreeFunction)(void*);

typedef struct node_t {
    Element data;
    struct node_t* next; // Use struct node_t* instead of Node to resolve forward declaration issues
} node_t;

typedef node_t* Node; // Define Node as a pointer to node_t

typedef struct stack_t* Stack; // Define Stack as a pointer to stack_t

// Error codes
typedef enum {
    STACK_OK,
    STACK_ERROR,
    STACK_EMPTY,
    STACK_OVERFLOW
} StackStatus;

// Function prototypes
Stack createStack(CopyFunction copyFunc, FreeFunction freeFunc);
void freeStack(Stack s);
bool isEmpty(Stack s);
StackStatus push(Stack s, Element data);
StackStatus pop(Stack s, Element* data);
StackStatus peek(Stack s, Element* data);

#endif // STACK_H
