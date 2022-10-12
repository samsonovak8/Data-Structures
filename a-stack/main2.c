#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct stack
{
    char *data;
    size_t elem_size;
    size_t capacity;
    size_t size;
};

int const MIN_CAP = 1;

struct stack *stack_new(size_t elem_size)
{
    struct stack *st = (struct stack*) malloc(sizeof(struct stack));
    st->elem_size = elem_size;
    st->capacity = MIN_CAP;
    st->size = 0;
    st->data = (char *) malloc(elem_size * st->capacity);
    return st;
}

int stack_empty(struct stack const *st)
{
    if (st == NULL)
        return 0;
    if (st->size == 0)
        return 1;
    else 
        return 0;
}

int stack_push(struct stack *st, void const *elem)
{
    if (st->size == st->capacity)
    {
        st->capacity <<= 1;
        char *check_pointer;
        check_pointer = realloc(st->data, st->elem_size * st->capacity);
        if (check_pointer != NULL)
            st->data = check_pointer;
        else 
            return 1;
    }
    memcpy(st->data + (st->size) * st->elem_size, elem, st->elem_size);
    st->size++;
    return 0;
}

int stack_pop(struct stack *st, void *elem)
{
    /*if(check(st->size - 1, st->capacity))
    {
        st->capacity >>= 1;
        void *check_pointer;
        check_pointer = realloc(st->data, st->elem_size * st->capacity);
        if (check_pointer != NULL)
            st->data = check_pointer;
        else 
            return 0;
    }*/
    if (stack_empty(st) || elem == NULL)
        return 1;
    memcpy(elem, st->data + (st->size - 1) * st->elem_size, st->elem_size);
    st->size--;
    return 0;
}
int stack_top(struct stack *st, void *elem)
{
    if(stack_empty(st))
        return 1;
    memcpy(elem, st->data + (st->size - 1) * st->elem_size , st->elem_size);
    return 0;
}
void stack_print(struct stack const *st, void (*pf)(void const *st))
{
   //assert(st);
    printf("[");
    if (stack_empty(st)) { printf("]\n"); return; }

    pf(st->data);

    for(int i = 1; i < st->size; i++)
    {   
        printf(", ");
        pf(st->data + st->elem_size * i);
    }
    printf("]\n");
    return;
}
struct stack *stack_delete(struct stack *st)
{
    free(st->data);
    free(st);

    return NULL;
}
