#ifndef _DOUBLY_LINKED_LIST_QUEUE
#define _DOUBLY_LINKED_LIST_QUEUE

#include <stddef.h>
#include <stdbool.h>

typedef struct node {
    int          _value;
    struct node* _next;
} node_t;

node_t* node_create(int value, node_t* next);

typedef struct queue {
    size_t  _size;
    node_t* _head;
    node_t* _tail; 
} queue_t;

queue_t* queue_create();

bool queue_empty(const queue_t* queue);

size_t queue_size(const queue_t* queue);

int queue_front(const queue_t* queue);

int queue_back(const queue_t* queue);

void queue_push(queue_t* queue, const int item);

void queue_pop(queue_t* queue);

void queue_dump(queue_t* queue);

void queue_destroy(queue_t* queue);

#endif