//
// Created by YOSSI on 4/22/2024.
//

#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    QUEUE_OK,
    QUEUE_ERROR_ALLOC,
    QUEUE_ERROR_EMPTY
} QueueResult;

typedef void* Element;

typedef void* (*CopyFunction)(const void*);
typedef void (*FreeFunction)(void*);

typedef struct node {
    Element data;
    struct node *next;
} Node;

typedef struct {
    Node *front;
    Node *rear;
    CopyFunction copyFunc;
    FreeFunction freeFunc;
} Queue;

void* createQueue(CopyFunction copyFunc, FreeFunction freeFunc);
QueueResult enqueue(Queue*, Element);
QueueResult dequeue(Queue*, Element*);
void clearQueue(Queue* );


#endif //QUEUE_H
