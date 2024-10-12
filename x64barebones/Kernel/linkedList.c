#include "include/linkedList.h"
#include "include/memoryManager.h"
#include "include/processes.h"

typedef struct LinkedListCDT {
    int len;
    Node *current;
    Node *first;
    Node *last;
} LinkedListCDT;

LinkedList createLinkedList() {
    LinkedList list = my_malloc(sizeof(LinkedListCDT));
    list->len = 0;
    list->current = NULL;
    list->first = NULL;
    list->last = NULL;
    return list;
}

void queue(LinkedList list, Node *node) {
    if (list->len == 0) {
        list->first = node;
        list->last = node;
        node->prev = NULL;
        node->next = NULL;
    } else {
        node->prev = list->last;
        node->next = NULL;
        list->last->next = node;
        list->last = node;
    }
    list->len++;
}

void push(LinkedList list, Node *node) {
    if (list->len == 0) {
        list->first = node;
        list->last = node;
        node->prev = NULL;
        node->next = NULL;
    } else {
        node->next = list->first;
        node->prev = NULL;
        list->first->prev = node;
        list->first = node;
    }
    list->len++;
}

void remove(LinkedList list, Node *node) {
    if (node == NULL || list->len == 0) return;

    if (node == list->first) {
        list->first = node->next;
        if (list->first != NULL)
            list->first->prev = NULL;
    } else if (node == list->last) {
        list->last = node->prev;
        if (list->last != NULL)
            list->last->next = NULL;
    } else {
        if (node->prev != NULL)
            node->prev->next = node->next;
        if (node->next != NULL)
            node->next->prev = node->prev;
    }

    my_free(node);
    list->len--;
}

int isEmpty(LinkedList list) {
    if(list == NULL) return 0;

    return list->len == 0;
}

int getLength(LinkedList list){
    if(list == NULL) return 0;

    return list->len;
}

void startIterator(LinkedList list) {
    if(list == NULL) return;

    list->current = list->first;
}

int listHasNext(LinkedList list) {
    if(list == NULL) return 0;

    return list->current != NULL;
}

void *listNext(LinkedList list){
    if (list != NULL && listHasNext(list)){
        Node * aux = list->current;
        list->current = list->current->next;
        return aux;
    }
    return NULL;
}


void freeList(LinkedList list) {
    if(list == NULL) return;

    while(listHasNext(list)){
        Node * current = listNext(list);
        free_process_memory(current->data);
    }
    
    my_free(list);
}


