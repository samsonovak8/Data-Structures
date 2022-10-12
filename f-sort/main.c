#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned get_max(unsigned *begin, unsigned *end, int *n)
{
    unsigned m = *begin;
    int cnt = 0;
    while(begin != end)
    {
        if (m < *begin)
            m = *begin;
        ++begin;
        ++cnt;
    }
    *n = cnt;
    return m;
}

void insertsort(unsigned *begin, unsigned *end, unsigned exp, int n, unsigned *new_array, int *digit)
{
    int count[256];
    for(size_t i = 0; i < 256; ++i)
        count[i] = 0;

    for(size_t i = 0; i < n; ++i)
        count[*begin++]++;

    //undigned int number;
    // void* ptr = &number;
    //unsigned char *ptr = (unsigned char*) ptr;

    for(size_t i = 1; i < 256; ++i)
        count[i] += count[i - 1];

    begin--;
    for(int i = n - 1; i >= 0; --i)
    {
        new_array[count[*begin] - 1] = *begin--;
        --count[digit[i]];

    }

    begin++;
    for(size_t i = 0; i < n; ++i)
        *begin++ = new_array[i];

    return;
}

void fast_sort(unsigned *begin, unsigned *end) {
    if(begin == NULL || end == NULL)
        return;

    int n;
    unsigned m = get_max(begin, end, &n);
    unsigned* new_array = (unsigned*) malloc(n * sizeof(unsigned));
    int* digit = (int*) malloc(n * sizeof(int));

    if (new_array == NULL || digit == NULL)
        return;

    for(size_t i = 1; m / i > 0; i *= 10)
    {
        insertsort(begin, end, i, n, new_array, digit);
    }

    free(new_array);
    free(digit);

    return;
}

int main()
{
    unsigned array[] = {10000000, 1534356433, 1398772987, 1398729331};
    fast_sort(array, array+4);

    for (int i = 0; i < 4; i++)
        printf("%d ", array[i]) ;
    return 0;
}