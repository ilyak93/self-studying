#ifndef QUEUE_H
#define QUEUE_H

typedef void* Element;
typedef Element (*CopyFunction)(const void*);
typedef void (*FreeFunction)(void*);

typedef struct queue_t* Queue;  // Opaque structure declaration

// Result type enumeration
typedef enum {
    QUEUE_OK,
    QUEUE_ERROR_ALLOC,
    QUEUE_ERROR_EMPTY
} QueueResult;

// Function declarations
Queue createQueue(CopyFunction copy, FreeFunction free);
QueueResult enqueue(Queue queue, Element element);
QueueResult dequeue(Queue queue, Element *element);
void clearQueue(Queue queue);
void freeQueue(Queue queue);

#endif
