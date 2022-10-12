#include <stdio.h>
#include <stdlib.h>

enum tree_type{XOR, MAX};

int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

void build(int* tree, int* a, size_t v, size_t l, size_t r, int type)
{
    if (l == r)
    {
        tree[v] = a[l];
    }
    else 
    {
        size_t mid = (l + r) >> 1;
        build(tree, a, (v << 1) + 1, l, mid, type);
        build(tree, a, (v << 1) + 2, mid + 1, r, type);

        if (type == XOR)
            tree[v] = tree[(v << 1) + 1] ^ tree[(v << 1) + 2];
        else 
            tree[v] = max(tree[(v << 1) + 1], tree[(v << 1) + 2]);
    }
}

int get(int* tree, size_t v, size_t tl, size_t tr, size_t l, size_t r, int type)
{
    if (l > tr || r < tl)
        return 0;

    if (l <= tl && tr <= r)
        return tree[v];

    size_t mid = (tl + tr) >> 1;

    int left_answer = get(tree, (v << 1) + 1, tl, mid, l, r, type);
    int right_answer = get(tree, (v << 1) + 2, mid + 1, tr, l, r, type);

    if(type == XOR)
        return (left_answer ^ right_answer);
    else 
        return max(left_answer, right_answer);
}

void update(int* tree_xor, int* tree_max, size_t v, size_t l, size_t r, size_t pos, int val)
{
    if (l == r)
    {
        tree_xor[v] = tree_max[v] = val;
    }
    else
    {
        size_t mid = (l + r) >> 1;

        if (pos <= mid)
            update(tree_xor, tree_max, (v << 1) + 1, l, mid, pos, val);
        else
            update(tree_xor, tree_max, (v << 1) + 2, mid + 1, r, pos, val);

        tree_xor[v] = tree_xor[(v << 1) + 1] ^ tree_xor[(v << 1) + 2];
        tree_max[v] = max(tree_max[(v << 1) + 1], tree_max[(v << 1) + 2]);

    }
}


int main()
{
    size_t n, m;
    scanf("%ld %ld", &n, &m);

    int* a = (int*) calloc(n, sizeof(int));

    int* tree_xor = (int*) calloc(n << 2, sizeof(int));

    int* tree_max = (int*) calloc(n << 2, sizeof(int));

    if (a == NULL || tree_xor == NULL || tree_max == NULL)
        return 1;

    for(size_t i = 0; i < n; ++i)
        scanf("%d", &a[i]);

    build(tree_xor, a, 0, 0, n - 1, XOR);
    build(tree_max, a, 0, 0, n - 1, MAX);

    while(m--)
    {
        size_t operation_type;
        scanf("%ld", &operation_type);

        if (operation_type == 1)
        {
            size_t l, r;
            scanf("%ld %ld", &l, &r);
            printf("%d\n", get(tree_xor, 0, 0, n - 1, l, r, XOR));
        }
        if (operation_type == 2)
        {
            size_t l, r;
            scanf("%ld %ld", &l, &r);
            printf("%d\n", get(tree_max, 0, 0, n - 1, l, r, MAX));
        }
        if (operation_type == 3)
        {
            size_t pos;
            int val;
            scanf("%ld %d", &pos, &val);
            update(tree_xor, tree_max, 0, 0, n - 1, pos, val);
        }
        
    }

    free(a);
    free(tree_xor);
    free(tree_max);

    return 0;
}