#include "queue.h"

void* createQueue(CopyFunction copyFunc, FreeFunction freeFunc) {
    Queue *q = malloc(sizeof(Queue));
    if (q == NULL) {
        return NULL;
    }
    q->front = q->rear = NULL;
    q->copyFunc = copyFunc;
    q->freeFunc = freeFunc;
    return q;
}

QueueResult enqueue(Queue *q, Element value) {
    if (!q) return QUEUE_ERROR_ALLOC;

    Node *newNode = malloc(sizeof(Node));
    if (!newNode) {
        return QUEUE_ERROR_ALLOC;
    }

    newNode->data = q->copyFunc(value);
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    return QUEUE_OK;
}

QueueResult dequeue(Queue *q, Element *dataOut) {
    if (!q || q->front == NULL) {
        return QUEUE_ERROR_EMPTY;
    }

    Node *temp = q->front;
    *dataOut = temp->data; // Return data to caller
    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    return QUEUE_OK;
}

void clearQueue(Queue *q) {
    Node *current = q->front;
    Node *next;

    while (current != NULL) {
        next = current->next;
        q->freeFunc(current->data); // Free the element
        free(current);
        current = next;
    }

    q->front = q->rear = NULL;
}

