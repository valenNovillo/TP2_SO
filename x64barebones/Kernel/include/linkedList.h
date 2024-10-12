#ifndef LINKEDLIST_H
#define LINKEDLIST_H

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
int isEmpty(LinkedList list);
void startIterator(LinkedList list);
int getLength(LinkedList list);
int listHasNext(LinkedList list);
void *listNext(LinkedList list);
void freeList(LinkedList list);

#endif //LINKEDLIST_H