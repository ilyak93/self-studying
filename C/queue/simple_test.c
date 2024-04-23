#include "queue.h"

Element copyInt(const void* elem) {
    int *newInt = malloc(sizeof(int));
    *newInt = *(int *)elem;
    return newInt;
}

void freeInt(void* elem) {
    free(elem);
}

int main() {
    int a = 10, b = 20;
    Queue *q = createQueue(copyInt, freeInt);
    QueueResult result;

    // Enqueue elements
    result = enqueue(q, &a);
    if (result != QUEUE_OK) {
        printf("Failed to enqueue %d\n", a);
    }

    result = enqueue(q, &b);
    if (result != QUEUE_OK) {
        printf("Failed to enqueue %d\n", b);
    }

    int *deq;
    // Dequeue first element and check for errors
    result = dequeue(q, (Element *)&deq);
    if (result == QUEUE_OK) {
        printf("Dequeued: %d\n", *deq);
        free(deq); // Don't forget to free the memory if dequeue was successful
    } else {
        printf("Failed to dequeue. Queue may be empty or an error occurred.\n");
    }

    // Try to dequeue second element
    result = dequeue(q, (Element *)&deq);
    if (result == QUEUE_OK) {
        printf("Dequeued: %d\n", *deq);
        free(deq);
    } else {
        printf("Failed to dequeue. Queue may be empty or an error occurred.\n");
    }

    // Clear the remaining elements in the queue
    clearQueue(q);

    // Free the queue structure itself
    free(q);

    return 0;
}
