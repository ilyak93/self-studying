#include "Queue.h"
#include <stdlib.h>

typedef struct node {
    Element data;
    struct node *next;
} Node;

struct queue_t {
    Node *front;
    Node *rear;
    CopyFunction copy;
    FreeFunction free;
} queue_t;

Queue createQueue(CopyFunction copyFunc, FreeFunction freeFunc) {
    Queue queue = (Queue)malloc(sizeof(*queue));
    if (!queue) {
        return NULL;
    }
    queue->front = queue->rear = NULL;
    queue->copy = copyFunc;
    queue->free = freeFunc;
    return queue;
}

QueueResult enqueue(Queue queue, Element element) {
    if (!queue) return QUEUE_ERROR_ALLOC;
    Node *newNode = malloc(sizeof(Node));
    if (!newNode) return QUEUE_ERROR_ALLOC;

    newNode->data = queue->copy(element);
    newNode->next = NULL;
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    return QUEUE_OK;
}

QueueResult dequeue(Queue queue, Element *element) {
    if (!queue || queue->front == NULL) return QUEUE_ERROR_EMPTY;

    Node *temp = queue->front;
    *element = temp->data;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return QUEUE_OK;
}

void clearQueue(Queue queue) {
    Node *current = queue->front;
    while (current) {
        Node *next = current->next;
        queue->free(current->data);
        free(current);
        current = next;
    }
    queue->front = queue->rear = NULL;
}

void freeQueue(Queue queue) {
    if (queue) {
        clearQueue(queue);
        free(queue);
    }
}