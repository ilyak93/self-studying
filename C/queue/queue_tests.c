#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

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

void test_createQueue() {
    Queue *q = createQueue(copyInt, freeInt);
    assert(q != NULL);
    assert(q->front == NULL);
    assert(q->rear == NULL);
    assert(q->copyFunc == copyInt);
    assert(q->freeFunc == freeInt);

    clearQueue(q);
    free(q);
    printf("createQueue() test passed.\n");
}

void test_enqueue() {
    Queue *q = createQueue(copyInt, freeInt);
    int a = 10, b = 20;

    // Testing successful enqueue
    assert(enqueue(q, &a) == QUEUE_OK);
    assert(q->front != NULL && q->rear != NULL && *(int *)q->front->data == 10);
    assert(q->front == q->rear); // Only one element should be present

    // Testing multiple enqueues
    assert(enqueue(q, &b) == QUEUE_OK);
    assert(q->rear != NULL && q->rear != q->front && *(int *)q->rear->data == 20);

    clearQueue(q);
    free(q);
    printf("enqueue() test passed.\n");
}

void test_dequeue() {
    Queue *q = createQueue(copyInt, freeInt);
    int a = 10, b = 20;
    int *out;

    enqueue(q, &a);
    enqueue(q, &b);

    // Dequeue first element
    assert(dequeue(q, (Element *)&out) == QUEUE_OK);
    assert(*out == 10);
    free(out);

    // Dequeue second element
    assert(dequeue(q, (Element *)&out) == QUEUE_OK);
    assert(*out == 20);
    free(out);

    // Attempt to dequeue from an empty queue
    assert(dequeue(q, (Element *)&out) == QUEUE_ERROR_EMPTY);

    clearQueue(q);
    free(q);
    printf("dequeue() test passed.\n");
}

void test_clearQueue() {
    Queue *q = createQueue(copyInt, freeInt);
    int a = 10, b = 20;

    enqueue(q, &a);
    enqueue(q, &b);

    clearQueue(q);
    assert(q->front == NULL && q->rear == NULL);

    free(q);
    printf("clearQueue() test passed.\n");
}



int main() {
    test_createQueue();
    test_enqueue();
    test_dequeue();
    test_clearQueue();

    printf("All tests passed.\n");
    return 0;
}