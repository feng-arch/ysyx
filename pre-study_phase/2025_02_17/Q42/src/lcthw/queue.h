#ifndef lcthw_Queue_h
#define lcthw_Queue_h

#include <stdlib.h>

typedef struct QueueNode
{
    void *value;
    struct QueueNode *next;
    struct QueueNode *prev;
} QueueNode;


typedef struct Queue
{
    int count;
    QueueNode *first;
    QueueNode *last;
} Queue;


#define QUEUE_FOREACH(Q, C)                                                                                            \
    QueueNode *C = NULL;                                                                                               \
    for (C = Q->first; C != NULL; C = C->next)


Queue *Queue_create()
{
    return calloc(1, sizeof(Queue));
}
void Queue_destroy(Queue *q)
{
    for (int i = 0; i < q->count;i++){
        QueueNode *temp = q->first;
        q->first = temp->next;
        free(temp);
    }
    free(q);
}
void Queue_send(Queue *q, void *value)
{
    q->count++;
    QueueNode *temp = calloc(1, sizeof(QueueNode));
    temp->value = value;
    if (q->first == NULL)
    {
        temp->prev = NULL;
        temp->next = NULL;
        q->first = temp;
        q->last = temp;
        return;
    }
    temp->next = q->first;
    q->first->prev = temp;
    temp->prev = NULL;
    q->first = temp;
}
void *Queue_peek(Queue *q)
{
    return q->last->value;
}
int Queue_count(Queue *q)
{
    if (q == NULL)
        return 0;
    return q->count;
}

void *Queue_recv(Queue *q)
{
    void *result = NULL;
    if (q->count == 0)
        return NULL;
    q->count--;
    result = q->last->value;
    QueueNode *temp = q->last;
    if (q->count == 0)
    {
        free(temp);
        return result;
    }
    q->last = q->last->prev;
    q->last->next = NULL;
    free(temp);
    return result;
}


#endif