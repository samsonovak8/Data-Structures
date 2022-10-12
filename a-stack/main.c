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
    if(st != NULL)
    {
        st->elem_size = elem_size;
        st->capacity = MIN_CAP;
        st->size = 0;
        st->data = (char *) malloc(elem_size * st->capacity);
        if (st->data == NULL)
            return NULL;
    }
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

int stack_push(struct stack *st, const void *elem)
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
static void pf(void const *st)
{
    printf("%d", *(int *)st);
    return;
}
void stack_print(struct stack const *st, void (*pf)(void const *st))
{
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
    if (st != NULL)
    {
        if (st->data != NULL)
            free(st->data);
        free(st);
    }

    return NULL;
}

int main()
{
    struct stack *st = stack_new(sizeof(int));
    for(int i = 0; i < 10; i++)
    {
        int tmp = i;
        stack_push(st, &tmp);
    }
    printf("stack_push: ");
    stack_print(st, pf);

    int tmp;
    stack_top(st, &tmp);
    printf("\n stack_top: ");
    stack_print(st, pf);

    for(int i = 0; i < 10; i++)
    {
        if(stack_empty(st))
            printf("EMPTY\n");
        else 
            printf("NOT EMPTY\n");

        stack_pop(st, &tmp);
        stack_print(st, pf);
    }

    st = stack_delete(st);
    stack_print(st, pf);

    return 0;
}