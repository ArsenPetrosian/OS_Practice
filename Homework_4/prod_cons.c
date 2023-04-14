#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "queue.h"

typedef struct sync_context {
    pthread_cond_t   _full;
    pthread_mutex_t  _full_lock;
    pthread_cond_t   _empty;
    pthread_mutex_t  _empty_lock;
    size_t           _count;
    queue_t* _queue;   
} sync_context_t;

void* execute_producer(void* input)
{
    sync_context_t* context = input;

    pthread_cond_t  full  = context->_full;
    pthread_mutex_t lock  = context->_full_lock;

    size_t* count = &context->_count;

    for (size_t i = 0; i < 25UL; ++i) {
        sleep(1);

        const int new_item = rand() % 999;
        
        pthread_mutex_lock(&lock);

        if(*count >= context->_queue->_size)
            pthread_cond_wait(&full, &lock);

        printf("New item %3d is produced and inserted into the storage: ", new_item);

        queue_push(context->_queue, new_item);
        queue_dump(context->_queue);
        ++*count;

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

void* execute_consumer(void* input)
{
    sync_context_t* context = input;

    pthread_cond_t  empty  = context->_empty;
    pthread_mutex_t lock  = context->_empty_lock;

    size_t* count = &context->_count;

    for (size_t i = 0; i < 25UL; ++i) {
        sleep(1);

        pthread_mutex_lock(&lock);

        if(count <= 0)
            pthread_cond_wait(&empty, &lock);

        const int item = queue_front(context->_queue);
        printf("New item %3d is consumed and removed from the storage:  ", item);

        queue_pop(context->_queue);
        queue_dump(context->_queue);
        --*count;

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main(int argc, char** argv)
{
    if (argc != 4) {
        printf("%s", "Invalid number of arguments.\n");
        return 1;
    }

    const size_t queue_size = atoi(argv[1]);
    const size_t producers_count = atoi(argv[2]);
    const size_t consumers_count = atoi(argv[3]);

    sync_context_t sync_context;
    sync_context._queue = queue_create();
    sync_context._count = 0;

    pthread_cond_init(&sync_context._full, NULL);
    pthread_mutex_init(&sync_context._full_lock, NULL);
    pthread_cond_init(&sync_context._empty, NULL);
    pthread_mutex_init(&sync_context._empty_lock, NULL);

    srand(time(NULL));

    pthread_t consumers[consumers_count];
    for (size_t i = 0; i < consumers_count; i++)
        if(pthread_create(&consumers[i], NULL, execute_consumer, &sync_context) != 0)
            perror("Unable to create a consumer\n");

    pthread_t producers[producers_count];
    for (size_t i = 0; i < producers_count; i++)
        if(pthread_create(&producers[i], NULL, execute_producer, &sync_context) != 0)
            perror("Unable to create a producer\n");

    for (size_t i = 0; i < producers_count; i++)
        if(pthread_join(producers[i], NULL) != 0)
            perror("Unable to join a producer\n");

    for (size_t i = 0; i < consumers_count; i++)
        if(pthread_join(consumers[i], NULL) != 0)
            perror("Unable to join a consumer\n");

    pthread_cond_destroy(&sync_context._full);
    pthread_mutex_destroy(&sync_context._full_lock);
    pthread_cond_destroy(&sync_context._empty);
    pthread_mutex_destroy(&sync_context._empty_lock);

    return 0;
}
