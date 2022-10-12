
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct set
{
    size_t *data;
    size_t capacity;
    int size;
};

struct set *set_new(size_t capacity)
{
    struct set *s;
    s = (struct set*) malloc(sizeof(struct set));

    if (s == NULL)
        return NULL;

    s->capacity = capacity;
    s->data = (size_t*) calloc(capacity, sizeof(size_t));

    if (s->data == NULL)
        return NULL;

    s->size = 0;
    return s;
}
/*Создать новое пустое множество, значения элементов которого могут лежать в границах от 0 
до capacity-1 включительно. Вернуть указатель на него.*/

int set_insert(struct set *s, size_t elem)
{
    if (s == NULL || s->data == NULL)
        return 1;
    if (elem >= s->capacity)
        return 1;
    size_t index = elem / sizeof(size_t) / 8;
    size_t position = elem % (sizeof(size_t) * 8);
    if (!(s->data[index]&position))
    {
        s->data[index] |= position;
        s->size++;
    }
    return 0;
}
/*Добавить во множество s элемент elem. Если элемент существовал, множество не изменится. 
Вернуть 0, если операция корректна, т.е. elem < capacity, иначе вернуть 1.*/

int set_erase(struct set *s, size_t elem)
{
    if (s == NULL || s->data == NULL)
        return 1;
    if (elem >= s->capacity)
        return 1;
    size_t index = elem / sizeof(size_t) / 8;
    size_t position = elem % (sizeof(size_t) * 8);
    if (s->data[index]&position)
    {
        s->data[index] ^= position;
        s->size--;
    }
    return 0;
}
/*Удалить элемент из множества. Если элемент не существовал, множество не изменится. 
Вернуть 0, если операция корректна, т.е. elem < capacity, иначе вернуть 1.*/

int set_find(struct set const *s, size_t elem)
{
    if (s == NULL || elem >= s->capacity)
        return 0;
    size_t index = elem / sizeof(size_t) / 8;
    size_t position = elem % (sizeof(size_t) * 8);
    return s->data[index]&position;
}
/*Вернуть 1, если элемент присутствует в множестве и 0 в противном случае.*/

struct set *set_delete(struct set *s)
{
    if (s != NULL)
    {
        if (s->data != NULL)
            free(s->data);
        free(s);
    }
    return NULL;
}
/*Удалить объект множество и вернуть NULL*/

int set_empty(struct set const *s)
{
    if (s == NULL)
        return 0;
    return s->size != 0;
}
/*Предикат: вернуть единицу, если в множестве есть хотя бы один элемент и ноль в противном 
случае.*/

ssize_t set_findfirst(struct set const *s, size_t start)
{
    if (s == NULL || start >= s->capacity || s->data == NULL)
        return -1;
    size_t index = elem / sizeof(size_t) / 8;
    for (int i = start; i < s->capacity; i++)
    {
        if (s->data[i])
            return i;
    }
    return -1;
}
/*Вернуть наименьший из элементов множества, не меньших start. Если таких элементов нет
 (например, множество пустое), то вернуть -1.*/

size_t set_size(struct set const *s)
{
    if (s == NULL)
        return 0;
    return s->size;
}
/*Вернуть количество элементов множества.*/

void set_print(struct set const *s)
{
    printf("[");
    if (s != NULL && s->data != NULL)
    {
        int f = 0;
        for (int i = 0; i < s->capacity; i++)
        {
            if (s->data[i])
            {
                if (f)
                    printf(", ");
                printf("%d", i);
                f = 1;
            }
        }
    }
    printf("]\n");
    return;
}
/*Вывести на стандартный вывод содержимое множества в формате по образцу: []\n или [1, 2, 3]\n.*/

int main()
{
    //set_print(NULL);
    return 0;
}