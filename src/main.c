#include <stdio.h>
#include "8_task.h"

int main()
{
    int array_1[] = {1, 2, 3};
    int array_2[] = {4, 5, 6};
    int array_3[] = {7, 8, 9};
    int res[] = {0, 0, 0};
    int *arrays[] = {array_1, array_2, array_3};
    concotinate_arrays(arrays, 3, 3, res);
    for (int i = 0; i < 3; ++i)
    {
        printf("%d ", res[i]);
    }
    printf("\n");

    return 0;
}