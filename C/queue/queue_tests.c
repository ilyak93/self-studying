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
    Queue q = createQueue(copyInt, freeInt);
    assert(q != NULL); // Check queue creation

    // Here, internal structure is not visible, but we can assert no error in creation
    freeQueue(q);
    printf("createQueue() test passed.\n");
}

void test_enqueue_dequeue() {
    Queue q = createQueue(copyInt, freeInt);
    assert(q != NULL);

    int a = 10, b = 20;
    assert(enqueue(q, &a) == QUEUE_OK); // Test successful enqueue
    assert(enqueue(q, &b) == QUEUE_OK); // Enqueue another item

    int *out;
    assert(dequeue(q, (Element *)&out) == QUEUE_OK); // Dequeue first item
    assert(*out == 10);
    free(out); // Free the dequeued integer

    assert(dequeue(q, (Element *)&out) == QUEUE_OK); // Dequeue second item
    assert(*out == 20);
    free(out);

    assert(dequeue(q, (Element *)&out) == QUEUE_ERROR_EMPTY); // Queue should be empty now

    freeQueue(q);
    printf("enqueue() and dequeue() tests passed.\n");
}

void test_clearQueue_freeQueue() {
    Queue q = createQueue(copyInt, freeInt);
    assert(q != NULL);

    int items[] = {5, 10, 15};
    for (int i = 0; i < 3; i++) {
        assert(enqueue(q, &items[i]) == QUEUE_OK);
    }

    clearQueue(q); // Clear the queue; assume it works correctly without visible side effects

    // After clearing, test that no elements can be dequeued
    int *out;
    assert(dequeue(q, (Element *)&out) == QUEUE_ERROR_EMPTY);

    freeQueue(q); // Safely free the queue
    printf("clearQueue() and freeQueue() tests passed.\n");
}




int main() {
    test_createQueue();
    test_enqueue_dequeue();
    test_clearQueue_freeQueue();

    printf("All tests passed.\n");
    return 0;
}