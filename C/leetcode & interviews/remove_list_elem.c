#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
    int e;
    struct node* next;
} Node;

Node* remove_item(Node* head, int val) {

    if(!head) return head;

    if(head->e == val){
        Node* next = head->next;
        head->next = NULL;
        return next;
    }

    for(Node* itr = head; itr != NULL; itr = itr->next) {
        if(itr->next->e == val) {
            itr->next = itr->next ? itr->next->next : NULL;
            break;
        }
    }
    return head;
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

    Node* rev_l = remove_item(head, 2);

    printf("end");
}