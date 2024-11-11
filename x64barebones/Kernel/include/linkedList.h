#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdint.h>

typedef struct LinkedListCDT* LinkedList;

typedef struct Node {
    void* data;
    struct Node *prev;
    struct Node *next;
} Node;

LinkedList createLinkedList();
void queue(LinkedList list, Node *node);
void push(LinkedList list, Node *node);
void remove(LinkedList list, Node *node);
void* dequeue(LinkedList list);
int isEmpty(LinkedList list);
void startIterator(LinkedList list);
int getLength(LinkedList list);
int listHasNext(LinkedList list);
void *listNext(LinkedList list);
void freeList(LinkedList list);
uint8_t listConcatenate(LinkedList from, LinkedList to);

#endif //LINKEDLIST_H