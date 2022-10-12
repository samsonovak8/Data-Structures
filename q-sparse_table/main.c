#include <stdio.h>
#include <stdlib.h>

int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

void precalc(int* log_table, size_t n)
{
    log_table[1] = 0;
    for(size_t i = 2; i <= n; ++i)
    {
        if (((i - 1) ^ i) == (i - 1) + i)
            log_table[i] = log_table[i - 1] + 1;
        else 
            log_table[i] = log_table[i - 1];
    }
    return;
}

void build(int** sparse_table, int n, int m)
{
    for(size_t i = 1; i < m; ++i)
    {
        for (size_t j = 0; j + (1 << (i - 1)) < n; ++j)
            sparse_table[i][j] = max(sparse_table[i - 1][j], sparse_table[i - 1][j + (1 << (i - 1))]);
    }
    return;
}

int query(int** sparse_table, int level, int l, int r)
{
    return max(sparse_table[level][l], sparse_table[level][r - (1 << level) + 1]);
}


int main()
{
    size_t n, m;
    scanf("%ld %ld", &n, &m);

    int* log_table = (int*) calloc(n + 1, sizeof(int));

    if (log_table == NULL)
        return 1;

    precalc(log_table, n);

    size_t height = log_table[n] + 2;

    int** sparse_table = (int**) calloc(height, sizeof(int*));

    if (sparse_table == NULL)
        return 1;
    
    for(size_t i = 0; i < height; ++i)
    {
        sparse_table[i] = (int*) calloc(n, sizeof(int));
        if (sparse_table[i] == NULL)
            return 1;
    }

    for(size_t i = 0; i < n; ++i)
    {
        scanf("%d", &sparse_table[0][i]);
    }
    
    build(sparse_table, n, height);

    printf("\n");
    while(m--)
    {
        size_t l, r;
        scanf("%ld %ld", &l, &r);

        printf("%d\n", query(sparse_table, log_table[r - l + 1], l, r));
    }

    for(size_t i = 0; i < height; ++i)
    {
        free(sparse_table[i]);
    }

    free(sparse_table);
    free(log_table);

    return 0;
}