#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "8_task.h"

int main()
{
    srand(time(NULL));
    int k = (rand() % 100) + 1;
    int n = (rand() % 100) + 1;
    printf("create %d arrays sizes %d", k, n);
    printf("\n");
    int **arrays = (int **)malloc(k * sizeof(int *));
    int *sizes = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < k; ++i)
    {
        sizes[i] = n;
        arrays[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j)
        {
            arrays[i][j] = rand();
        }
    }
    int *res = (int *)malloc(k * sizeof(int));
    concotinate_arrays(arrays, k, n, res);
    printf("%d", res[0]);
    printf("\n");

    printf("%d %d", k, n);
    printf("\n");
    for (int i = 0; i < k; ++i)
    {
        free(arrays[i]);
    }
    free(arrays);
    free(sizes);

    return 0;
}
