#ifndef lcthw_Stack_h
#define lcthw_Stack_h

#include <stdlib.h>

#define STACK_FOREACH(S, C)                                                                                            \
    Stack *C = NULL;                                                                                                   \
    for (C = S; C != NULL && C->value != NULL; C = C->next)

typedef struct Stack
{
    void *value;
    struct Stack *next;
} Stack;

Stack *Stack_create()
{
    return (Stack *)calloc(1, sizeof(Stack));
}

void Stack_destroy(Stack *s)
{
    STACK_FOREACH(s, cur)
    {
        free(s);
        s = cur;
    }
    free(s);
}

void Stack_push(Stack *s, void *value)
{
    if (s->value == NULL)
    {
        s->value = value;
        return;
    }
    Stack *temp = (Stack *)calloc(1, sizeof(Stack));
    temp->value = s->value;
    temp->next = s->next;
    s->value = value;
    s->next = temp;
}

int Stack_count(Stack *s)
{
    int count = 0;
    STACK_FOREACH(s, cur)
    {
        count++;
    }
    return count;
}

void *Stack_peek(Stack *s)
{
    if (s == NULL)
        return NULL;
    else
    {
        return s->value;
    }
}

void *Stack_pop(Stack *s)
{
    if (s == NULL || s->value == NULL)
        return NULL;
    Stack *temp = s->next;
    void *value = s->value;
    if (temp != NULL)
    {
        s->next = temp->next;
        s->value = temp->value;
    }
    else
    {
        s->value = NULL;
        s->next = NULL;
    }
    free(temp);
    return value;
}

#endif
