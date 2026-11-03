#include <stdio.h>

void concotinate_arrays(int **arrays, int k, int n, int *array)
{
    if (!arrays)
        return;
    for (int i = 0; i < k; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            array[j] += arrays[i][j];
        }
    }
}