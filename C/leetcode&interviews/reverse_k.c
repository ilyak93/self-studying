#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
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

Node* reverseK(Node* head, int k) {
    Node* newHead = NULL;
    bool first = true;
    Node* curHead = head;
    Node* end = NULL;
    while(curHead) {
        Node *nextHead = curHead;
        int c = 0;
        Node* curEnd = NULL;
        while(c < k){
            if(c == k - 1){
                curEnd = nextHead;
            }
            if(nextHead) {
                nextHead = nextHead->next;
                c++;
            } else {
                break;
            }
        }
        if(curEnd) {
            curEnd->next = NULL;
        }
        Node* prevEnd = NULL;
        if(end) prevEnd = end;
        end = curHead;
        if(c < k){
            prevEnd->next = curHead;
            return newHead;
        }
        Node* revKList = reverse_list(curHead);
        if(first) {
            newHead = revKList;
            first = false;
        }
        if(prevEnd) {
            prevEnd->next = revKList;
        }
        curHead = nextHead;
    }
    return newHead;

}


int main() {
    // Create nodes
    Node *head = (Node *) malloc(sizeof(Node));
    Node *second = (Node *) malloc(sizeof(Node));
    Node *third = (Node *) malloc(sizeof(Node));
    Node *fourth = (Node *) malloc(sizeof(Node));
    Node *fifth = (Node *) malloc(sizeof(Node));
    Node *sixt = (Node *) malloc(sizeof(Node));


    // Assign values and link nodes
    head->e = 1;
    head->next = second;

    second->e = 2;
    second->next = third;

    third->e = 3;
    third->next = fourth;

    fourth->e = 4;
    fourth->next = fifth;

    fifth->e = 5;
    fifth->next = NULL;

    //sixt->e = 6;
    //sixt->next = NULL;

    Node* rev_l = reverseK(head, 3);

    printf("end");
}