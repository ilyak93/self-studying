#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

Element copyInt(const void* elem) {
    int *newInt = malloc(sizeof(int));
    *newInt = *(int *)elem;
    return newInt;
}

void freeInt(void* elem) {
    free(elem);
}


// Assume copyInt and freeInt are implemented elsewhere

int main() {
    Queue q = createQueue(copyInt, freeInt);
    if (!q) {
        fprintf(stderr, "Failed to create queue\n");
        return 1;
    }

    int a = 10, b = 20;
    if (enqueue(q, &a) != QUEUE_OK || enqueue(q, &b) != QUEUE_OK) {
        fprintf(stderr, "Failed to enqueue elements\n");
    }

    int *deq;
    if (dequeue(q, (Element *)&deq) == QUEUE_OK) {
        printf("Dequeued: %d\n", *deq);
    }

    freeQueue(q);
    return 0;
}