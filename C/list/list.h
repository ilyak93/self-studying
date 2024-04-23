// list.h
#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

// Define Element as a void pointer to allow storing any data type
typedef void* Element;

// Typedef for function pointers
typedef void* (*CopyFunction)(const void*);
typedef void (*FreeFunction)(void*);
typedef int (*CompareFunction)(const void*, const void*);
typedef void (*PrintFunction)(const void*);

// Node structure
typedef struct Node {
    Element data;
    struct Node* next;
} Node;

// List structure, including function pointers for element-specific operations
typedef struct List {
    Node* head;
    CopyFunction copy;
    FreeFunction freeElem;
    CompareFunction compare;
    PrintFunction print;
} List;

// Error codes enumeration
typedef enum {
    LIST_OK,
    LIST_ERR_ALLOC,
    LIST_ERR_NOT_FOUND
} ListError;

// Function declarations
Node* createNode(Element data, CopyFunction copy);
ListError insertAtBeginning(List* list, Element data);
ListError deleteNode(List* list, Element data);
void printList(List* list);

// Macro for iterating over each element in the list
#define FOREACH(list, var) for (Node* var = (list)->head; var != NULL; var = var->next)

#endif // LIST_H
