#ifndef LINKEDLIST_H
#define LINKEDLIST_H
typedef struct LinkedListCDT *LinkedList;

typedef struct Node {
    void* data;
    struct Node *prev;
    struct Node *next;
}Node;


#endif //LINKEDLIST_H