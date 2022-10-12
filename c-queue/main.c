#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*static void print_int(void const *q) {
   printf("%d", *(int *)q);
}
static void print_double(void const *q) {
   printf("%lf", *(double *)q);
}*/
 static void print_double(void const *q) {
   printf("%lf", *(double *)q);
}


struct stack
{
    char *data;
    size_t elem_size;
    size_t capacity;
    size_t size;
};

int const MIN_CAP = 1000000;

struct stack *stack_new(size_t elem_size)
{
    struct stack *st = (struct stack*) calloc(1, sizeof(struct stack));
    st->elem_size = elem_size;
    st->capacity = MIN_CAP;
    st->size = 0;
    st->data = (char *) malloc(elem_size * st->capacity);
    return st;
}
void stack_print(struct stack const *st, void (*pf)(void const *st))
{
    if (st == NULL) {printf("[]\n"); return;}

    printf("[");
    if (st->size > 0)
        pf(st->data + st->elem_size * (st->size - 1));
    for(int i = st->size - 2; i >= 0; i--)
    {   
        printf(", ");
        pf(st->data + st->elem_size * i);
    }
    printf("]\n");

    return;
}

int stack_empty(struct stack const *st)
{
    if (st == NULL)
        return 1;
    return (st->size == 0);
}
int stack_push(struct stack *st, const void *elem)
{
    if (st == NULL || elem == NULL  || st->data == NULL)
        return 1;
    if (st->size < st->capacity)
    {
        memcpy(st->data + (st->size) * st->elem_size, elem, st->elem_size);
        st->size++;
        return 0;
    }

    char* check_pointer = (char*) realloc(st->data, st->capacity * 2 * st->elem_size);
    if (check_pointer == NULL)
        return 1;
    st->data = check_pointer;
    st->capacity *= 2;
    memcpy(st->data + (st->size) * st->elem_size, elem, st->elem_size);
    st->size++;
    return 0;
}
int stack_pop(struct stack *st, void *elem)
{
    if (stack_empty(st) || elem == NULL)
        return 1;
    st->size--;
    memcpy(elem, st->data + st->size * st->elem_size, st->elem_size);
    return 0;
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


struct queue
{
    struct stack *s1; //head stack
    struct stack *s2; //tail stack
};

struct queue *queue_new(size_t elem_size)
{
    struct queue* q = (struct queue*) calloc(1, sizeof(struct queue));
    q->s1 = stack_new(elem_size);
    q->s2 = stack_new(elem_size);
    return q;
}
/*Создать новую очередь, которая способна хранить элементы размера elem_size, вернуть указатель на неё.*/
int queue_push(struct queue *q, const void *elem)
{
    if (q == NULL)
        return 1;
    return stack_push(q->s1, elem);
}
/*Добавить в голову очереди q элемент elem.
Обратите внимание, что из elem должно быть извлечено для хранения ровно то количество байт,
которой было специфицировано при создании данного стека. Функция должна вернуть 0 при успехе и 
1 при неуспехе любого рода. Амортизированная сложность этой операции должна быть O(1). 
После успешного исполнения данной операции в очереди должно оказаться на один элемент больше.*/
int queue_pop (struct queue *q, void *elem)
{
     if (q == NULL)
        return 1;
    
    char* moved_elem = (char*) malloc(q->s1->elem_size);
    while(!stack_empty(q->s1))
    {
        if (stack_pop(q->s1, moved_elem))
            return 1;
        if (stack_push(q->s2, moved_elem))
            return 1;
    }
    free(moved_elem);
    return stack_pop(q->s2, elem);
}
/*Извлечь из хвоста очереди q элемент elem.
 Обратите внимание, что в elem должно быть положено ровно то количество байт, 
 которой было специфицировано при создании данного стека. 
 Функция должна вернуть 0 при успехе и 1 при неуспехе любого рода. 
 Амортизированная сложность этой операции должна быть O(1).*/
int queue_empty(struct queue const *q)
{
    if (q == NULL)
        return 1;
    return stack_empty(q->s1) && stack_empty(q->s2);
}
/*Функция-предикат. Она должна вернуть логическую истину (1), если очередь пуста и логический ноль (0)
 в противном случае.*/
struct queue *queue_delete(struct queue *q)
{
    if(q != NULL)
    {
        stack_delete(q->s1);
        stack_delete(q->s2);
        free(q);
    }
    return NULL;
}
/*Удалить объект q и вернуть NULL.*/
void queue_print(struct queue const *q, void (*pf)(void const *data))
{
    if (q == NULL) {printf("[]\n"); return;}

    char* moved_elem = (char*) malloc(q->s1->elem_size);
    while(!stack_empty(q->s1))
    {
        stack_pop(q->s1, moved_elem);
        stack_push(q->s2, moved_elem);
    }
    stack_print(q->s2, pf);
    free(moved_elem);
    return;
}
/*Так как очередь может содержать произвольные данные,
 но выводить информацию из неё необходимо, вторым аргументом функции является указатель на функцию, 
 которая должна принимать указатель на данные, располагаемые в очереди и выводить их в соответствующем 
 виде на стандартный вывод.*/

int main() {
    struct queue *q = queue_new(sizeof (double));
    for (int i = 0; i < 10; i++) {
        double tmp = i * i;
        queue_push(q, &tmp);
    }
    queue_print(q, print_double);
    while (!queue_empty(q)) {
        double tmp;
        queue_pop(q, &tmp);
        printf("%lf\n", tmp);
    }                                                                                                                                                                                        
 
    q = queue_delete(q);
}
// Будут выведены все числа от 0 до 9 включительно.