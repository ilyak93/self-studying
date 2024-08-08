#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
    int e;
    struct node* next;
} Node;

Node* reverse_list(Node* head) {
    if (!head || !head->next) return head;
    if (!head->next->next) {
        head->next->next = head;
        Node* new_head = head->next;
        head->next = NULL;
        return new_head;
    }

    Node* prev = NULL;
    Node* current = head;
    Node* next = NULL;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    return prev;
}

int main() {
    // Create nodes
    Node *head = (Node *) malloc(sizeof(Node));
    Node *second = (Node *) malloc(sizeof(Node));
    Node *third = (Node *) malloc(sizeof(Node));
    Node *fourth = (Node *) malloc(sizeof(Node));

    // Assign values and link nodes
    head->e = 1;
    head->next = second;

    second->e = 2;
    second->next = third;

    third->e = 3;
    third->next = fourth;

    fourth->e = 4;
    fourth->next = NULL;

    Node* rev_l = reverse_list(head);

    printf("end");
}