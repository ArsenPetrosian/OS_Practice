#include <stdlib.h>
#include <stdio.h>

#include "queue.h"

node_t* node_create(int value, node_t* next)
{
    node_t* node = (node_t*)malloc(sizeof(node_t));
    if (node == NULL)
        return node;
    
    node->_value = value;
    node->_next  = next;

    return node;
}

queue_t* queue_create()
{
    queue_t* inst = (queue_t*)malloc(sizeof(queue_t));
    if (inst == NULL)
        return NULL;
    
    inst->_size = 0;
    inst->_head = NULL;
    inst->_tail = NULL;

    return inst;
}

bool queue_empty(const queue_t* queue)
{
    return queue->_size == 0;
}

size_t queue_size(const queue_t* queue)
{
    return queue->_size;
}

int queue_front(const queue_t* queue)
{
    return queue->_head->_value;
}

int queue_back(const queue_t* queue)
{
    return queue->_tail->_value;
}

void queue_push(queue_t* queue, const int item)
{
    queue->_size++;
    if (queue->_size == 1) {
        queue->_head = node_create(item, NULL);
        queue->_tail = queue->_head;
        return;
    }

    queue->_tail->_next = node_create(item, NULL);
    queue->_tail        = queue->_tail->_next; 
}

void queue_pop(queue_t* queue)
{
    node_t* head = queue->_head;
    queue->_head = queue->_head->_next;
    free(head);
    if (--queue->_size == 0)
        queue->_tail = NULL;
}

void queue_dump(queue_t* queue)
{
    if (queue->_size == 0) {
        printf("Queue is empty!\n");
        return;
    }
    node_t* it = queue->_head;
    while (it != NULL) {
        printf("%d ", it->_value);
        it = it->_next;       
    }
    printf("\n");
}

void queue_destroy(queue_t* queue)
{
    node_t* it = queue->_head;
    while (it != NULL) {
        node_t* delete_node = it;
        it = it->_next;
        free(delete_node);
    }
    free(queue);
    queue = NULL;
}