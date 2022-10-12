#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_int(void const *data) {
   printf("%d", *(int *)data);
}

struct vector
{
    char *data;
    size_t elem_size;
    size_t capacity;
    size_t size;
};

struct vector *vector_new(size_t elems, size_t elem_size)
{
    struct vector* v = (struct vector*) calloc(1, sizeof(struct vector));

    if (v == NULL)
        return NULL;

    v->elem_size = elem_size;
    v->capacity = elems;
    v->data = (char*) malloc(elem_size * elems);
    v->size = elems;

    if(v->data == NULL)
    {
        free(v);
        return NULL;
    }

    return v;
}
/*Создать новый вектор, который будет содержать elems элементов размера elem_size.
 Элементы не инициализированы.*/
int vector_resize(struct vector *v, size_t new_size)
{
    if (v == NULL || v->data == NULL)
        return 1;
    if (v->size >= new_size)
    {
        v->size = new_size;
        return 0;
    }
    char* check_pointer = (char*) realloc(v->data, (new_size + 1) * v->elem_size);

    if(check_pointer == NULL)
        return 1;

    v->data = check_pointer;
    v->size = new_size;
    v->capacity = new_size + 1;

    return 0;
}
/*Изменить размер вектора v на new_size. 
Значение старых элементов вектора не должны измениться.
 Остальные элементы остаются неинициализированными.
  Вернуть 0, если операция успешна или 1 в противном случае.*/
void vector_newcapacity(struct vector *v, size_t new_capacity)
{
    char* check_pointer = (char*) realloc(v->data, new_capacity * v->elem_size);

    if (check_pointer == NULL)
        return;

    v->data = check_pointer;
    v->capacity = new_capacity;

    return;
}
int vector_push(struct vector *v, void const *elem)
{
    if (v == NULL || elem == NULL || v->data == NULL)
        return 1;

    if(v->capacity == v->size)
    {
       if (v->capacity == 0)
            v->capacity = 1; 
       vector_newcapacity(v, v->capacity << 1);
    }

    memcpy(v->data + v->size * v->elem_size, elem, v->elem_size);
    v->size++;

    return 0;
}
/*Добавить ещё один элемент elem к вектору v.
 Функция должна вернуть 0 при успехе и 1 при неуспехе любого рода.
 Амортизированная сложность этой операции должна быть O(1).
 После успешного исполнения данной операции размер вектора должен увеличиться на единицу.*/
int vector_pop (struct vector *v, void *elem)
{
    if (v == NULL || v->size == 0 || elem == 0)
        return 1;

    v->size--;
    memcpy(elem, v->data + v->elem_size * v->size, v->elem_size);

    if ((v->size << 2) <= v->capacity && v->capacity > 1)
        vector_newcapacity(v, v->capacity >> 1);

    return 0;
}
/*Убрать из вектора v последний элемент, если он существует, и положить его значение по адресу elem.
 Функция должна вернуть 0 при успехе и 1 при неуспехе любого рода.
  Амортизированная сложность этой операции должна быть O(1).*/
int vector_empty(struct vector const *st)
{
    if (st == NULL)
        return 0;

    return (st->size == 0 ? 1:0);
}
/*Функция-предикат. Она должна вернуть логическую истину (1), если вектор пуст и логический ноль (0)
 в противном случае.*/
struct vector *vector_delete(struct vector *v)
{
    if (v != NULL)
    {
        if (v->data != NULL)
            free(v->data);
        free(v);
    }
    return NULL;
}
/*Удалить объект st и вернуть NULL.*/
void vector_print(struct vector const *v, void (*pf)(void const *data))
{
    printf("[");

    if (v != NULL && v->size > 0)
    {
        pf(v->data);
        for(int i = 1; i < v->size; ++i)
        {
            printf(", ");
            pf(v->data + v->elem_size * i);
        }
    }

    printf("]\n");
    return;
}
/*Вывести на стандартный вывод содержимое вектора v.
 Второй аргумент — указатель на функцию, которая должна принимать указатель на одиночный
  элемент данных, хранимый в векторе и вывести его в соответствующем виде на стандартный вывод.*/
int vector_set(struct vector *v, size_t index, void const *elem)
{
    if (v == NULL || index >= v->size || elem == NULL)
        return 1;

    memcpy(v->data + v->elem_size * index, elem, v->elem_size);
    return 0;
}
/*Присвоить элементу index вектора v значение elem.
 Вернуть 0, если операция успешна или 1 в противном случае.*/
int vector_get(struct vector const *v, size_t index, void *elem)
{
    if(v == NULL || index >= v->size || elem == NULL)
        return 1;

    memcpy(elem, v->data + v->elem_size * index, v->elem_size);
    return 0;
}
/*Присвоить elem значение элемента index вектора v.
 Вернуть 0, если операция успешна или 1 в противном случае.*/
size_t vector_size(struct vector const *v)
{
    if(v == NULL)
        return 0;

    return v->size;
}
/*Возвратить количество элементов, которым владеет вектор v.*/


int main() {
    struct vector *v = vector_new(10, sizeof (int));
    for (int i = 0; i < 10; i++) {
        vector_set(v, i, &i);
    }
    vector_print(v, print_int);
    v = vector_delete(v);
}

// Должно быть выведено [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]\n