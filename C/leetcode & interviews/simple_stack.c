#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_SIZE 1000

typedef struct {
    int top;
    int items[MAX_SIZE];
    int maxItems[MAX_SIZE];
} Stack;

void initialize(Stack* s) {
    s->top = -1;
}

int isEmpty(Stack* s) {
    return s->top == -1;
}

int isFull(Stack* s) {
    return s->top == MAX_SIZE - 1;
}

void push(Stack* s, int value) {
    if (isFull(s)) {
        printf("Stack Overflow\n");
        return;
    }
    s->top++;
    s->items[s->top] = value;

    if (s->top == 0) {
        s->maxItems[s->top] = value;
    } else {
        s->maxItems[s->top] = (value > s->maxItems[s->top - 1]) ? value : s->maxItems[s->top - 1];
    }
}

int pop(Stack* s) {
    if (isEmpty(s)) {
        printf("Stack Underflow\n");
        return INT_MIN;
    }
    return s->items[s->top--];
}

int peek(Stack* s) {
    if (isEmpty(s)) {
        printf("Stack is empty\n");
        return INT_MIN;
    }
    return s->items[s->top];
}

int getMax(Stack* s) {
    if (isEmpty(s)) {
        printf("Stack is empty\n");
        return INT_MIN;
    }
    return s->maxItems[s->top];
}

int main() {
    Stack s;
    initialize(&s);

    push(&s, 3);
    push(&s, 4);
    push(&s, 2);
    push(&s, 5);
    push(&s, 1);

    printf("Current top: %d\n", peek(&s));
    printf("Current max: %d\n", getMax(&s));

    printf("Popped: %d\n", pop(&s));
    printf("New top: %d\n", peek(&s));
    printf("New max: %d\n", getMax(&s));

    printf("Popped: %d\n", pop(&s));
    printf("New top: %d\n", peek(&s));
    printf("New max: %d\n", getMax(&s));

    return 0;
}