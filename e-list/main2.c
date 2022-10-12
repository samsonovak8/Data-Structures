#include <stdio.h>
#include <stdlib.h>

struct list
{
    int data;
    struct list *next;
};

struct list *list_new(int elem)
{
    struct list *list;
    list = (struct list*) calloc(1, sizeof(struct list));
    list->data = elem;
    list->next = NULL;
    return list;
}

struct list *list_insert(struct list *head, int elem)
{
    if (head == NULL)
        return list_new(elem);
    struct list *old_head = head;
    while(head->next != NULL)
    {
        head = head->next;
    }
    head->next = list_new(elem);
    return old_head;
}

struct list *list_find(struct list *head, int elem)
{
    while(head != NULL)
    {
        if (head->data == elem)
            return head;
        head = head->next;
    }
    return NULL;
}

struct list *list_erase(struct list *head, int elem)
{
    if (head == NULL)
        return NULL;
    struct list *current = head;
    if (head->data == elem)
    {
        current = head->next;
        free(head);
        return current;
    }
    struct list *prev = head;
    while(current != NULL)
    {
        if(current->data == elem)
        {
            prev->next = current->next;
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }
    return head;
}

struct list *list_insert_after(struct list *head, struct list *where, struct list *what)
{
    if(where == NULL || what == NULL)
        return head;
    if (head == NULL)
    {
        head = list_new(what->data);
        return head;
    }
    what->next = where->next;
    where->next = what;
    return head;
}

struct list *list_insert_before(struct list *head, struct list *where, struct list *what)
{
    if(where == NULL || what == NULL)
        return head;
    if (head == NULL)
    {
        head = list_new(what->data);
        return head;
    }
    struct list *current = head;
    if (current == where)
    {
        what->next = head;
        return what;
    }
    while(current != NULL)
    {
        if (current->next == where)
        {
            what->next = where;
            current->next = what;
            return head;
        }
        current = current->next;
    }
    return head;
}

struct list *list_delete(struct list *head)
{
    struct list *current;
    while(head != NULL)
    {
        current = head;
        head = head->next;
        free(current);
    }
    return NULL;
}

struct list *list_next(struct list *curr)
{
    if(curr == NULL)
        return NULL;
    else 
        return curr->next;
}

void list_print(struct list const *head)
{
    printf("[");
    if(head != NULL)
    {
        printf("%d", head->data);
        head = head->next;
        while(head != NULL)
        {
            printf(", %d", head->data);
            head = head->next;
        }
    }
    printf("]\n");
    return;
}

int main()
{
    return 0;
}