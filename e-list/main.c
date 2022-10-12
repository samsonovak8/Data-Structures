#include <stdio.h>
#include <stdlib.h>


struct list
{
    int data;
    struct list *next;
    struct list *previous;
};

struct list *list_new(int elem)
{
    struct list *list;
    list = (struct list*) malloc(sizeof(struct list));
    list->data = elem;
    list->next = NULL;
    list->previous = NULL;

    return list;
}

struct list *list_insert(struct list *head, int elem)
{
    struct list* node = list_new(elem);
    if (head == NULL)
    {
        head = node;
        tail = node;
    }
    else 
    {
        tail->next = node;
        node->previous = tail;

        tail = node;
    }
    return head;
}
struct list *list_find(struct list *head, int elem)
{
    struct list *current = head;
    while(current != NULL)
    {
        if (current->data == elem)
            return current;
        current = current->next;
    }
    return NULL;
}

struct list *list_erase(struct list *head, int elem)
{
    if (head == NULL)
        return NULL;

    if (head->data == elem)
    {
        if (head->next == NULL)
            tail = NULL;
        head = head->next;
        return head;
    }

    struct list *current = head;
    while(current != NULL)
    {
        if(current->data == elem)
        {
            if (current->next == NULL)
                tail = tail->previous;
            else 
                current->previous = current->next;
                
            free(current);
            break;
        }
        current = current->next;
    }
    return head;
}

struct list *list_insert_after(struct list *head, struct list *where, struct list *what)
{
    if(where == NULL || what == NULL)
        return head;
    if (head == NULL)
        return head = list_new(what->data);
    what->next = where->next;
    what->previous = where;
    if (where->next != NULL)
        where->next->previous = what;
    else 
        head->end = what;
    where->next = what;
    return head;
}

struct list *list_insert_before(struct list *head, struct list *where, struct list *what)
{
    if(where == NULL || what == NULL)
        return head;
    if (head == NULL)
        return head = list_new(what->data);
    what->next = where;
    what->previous = where->previous;
    if (where->previous != NULL)
        where->previous->next = what;
    where->previous = what;
    return head;
}

struct list *list_delete(struct list *head)
{
    struct list *current;
    while(head != NULL)
    {
        current = head->next;
        free(head);
        head = current;
    }
    return NULL;
}

struct list *list_next(struct list *curr)
{
    if(curr == NULL)
        return curr;
    else 
        return curr->next;
}
void list_print(struct list const *head)
{
    if(head == NULL)
    {
        printf("[]\n");
        return;
    }
    printf("[%d", head->data);
    struct list *current = head->next;
    while(current != NULL)
    {
        printf(", %d", current->data);
        current = current->next;
    }
    printf("]\n");
    return;
}


int main()
{
    return 0;
}