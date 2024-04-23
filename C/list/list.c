// list.c
#include "list.h"

// Implementation of createNode
Node* createNode(Element data, CopyFunction copy) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return NULL;  // Memory allocation failed
    }
    newNode->data = copy(data);  // Use copy function to store the data
    if (!newNode->data) {
        free(newNode);
        return NULL;
    }
    newNode->next = NULL;
    return newNode;
}

// Implementation of insertAtBeginning
ListError insertAtBeginning(List* list, Element data) {
    Node* newNode = createNode(data, list->copy);
    if (newNode == NULL) {
        return LIST_ERR_ALLOC;  // Return allocation error
    }
    newNode->next = list->head;
    list->head = newNode;
    return LIST_OK;
}

// Implementation of deleteNode
ListError deleteNode(List* list, Element data) {
    Node* temp = list->head, *prev = NULL;

    while (temp != NULL && list->compare(temp->data, data) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        return LIST_ERR_NOT_FOUND;  // Data not found
    }

    if (prev == NULL) {
        list->head = temp->next;
    } else {
        prev->next = temp->next;
    }

    list->freeElem(temp->data);
    free(temp);
    return LIST_OK;
}

// Implementation of printList
void printList(List* list) {
    FOREACH(list, var) {
        list->print(var->data);
        printf(" -> ");
    }
    printf("NULL\n");
}