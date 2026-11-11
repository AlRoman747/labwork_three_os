#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// Структура для передачи данных в каждый поток
typedef struct
{
    int **arrays;
    int n;
    int *res;
    int start_index;
    int end_index;
} ThreadData;

// Однопоточная функция
void concotinate_arrays_single(int **arrays, int k, int n, int *res)
{
    for (int i = 0; i < k; ++i)
    {
        long long sum = 0;
        for (int j = 0; j < n; ++j)
        {
            sum += arrays[i][j];
        }
        res[i] = (int)(sum % 100000);
    }
}

// Потоковый воркер
void *worker_function(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->start_index; i < data->end_index; ++i)
    {
        long long sum = 0;
        for (int j = 0; j < data->n; ++j)
        {
            sum += data->arrays[i][j];
        }
        data->res[i] = (int)(sum % 100000);
    }
    return NULL;
}

// Многопоточная функция
void concotinate_arrays_multi(int **arrays, int k, int n, int *res, int max_threads)
{
    int active_threads = (max_threads > k) ? k : max_threads;

    pthread_t *threads = (pthread_t *)malloc(active_threads * sizeof(pthread_t));
    ThreadData *thread_data = (ThreadData *)malloc(active_threads * sizeof(ThreadData));

    int items_per_thread = k / active_threads;
    int remainder = k % active_threads;
    int current_start = 0;

    for (int i = 0; i < active_threads; ++i)
    {
        thread_data[i].arrays = arrays;
        thread_data[i].n = n;
        thread_data[i].res = res;
        thread_data[i].start_index = current_start;

        int current_items = items_per_thread + (i < remainder ? 1 : 0);
        thread_data[i].end_index = current_start + current_items;
        current_start = thread_data[i].end_index;

        pthread_create(&threads[i], NULL, worker_function, &thread_data[i]);
    }

    for (int i = 0; i < active_threads; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(thread_data);
}

int main(int argc, char *argv[])
{
    int max_threads = 4;
    if (argc > 1)
    {
        max_threads = atoi(argv[1]);
        if (max_threads < 1)
            max_threads = 1;
    }

    srand(time(NULL));
    int k = 10000;
    int n = 50000;

    int **arrays = (int **)malloc(k * sizeof(int *));
    for (int i = 0; i < k; ++i)
    {
        arrays[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j)
            arrays[i][j] = rand() % 100;
    }

    int *res_single = (int *)malloc(k * sizeof(int));
    int *res_multi = (int *)malloc(k * sizeof(int));

    struct timespec start, end;

    printf("=== НАЧАЛО ТЕСТИРОВАНИЯ ===\n");
    printf("Размер данных: %d массивов по %d элементов\n\n", k, n);

    // --- Тест 1: Однопоточный режим ---
    printf("Запуск однопоточного режима... ");
    fflush(stdout);

    clock_gettime(CLOCK_MONOTONIC, &start);
    concotinate_arrays_single(arrays, k, n, res_single);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_single = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Готово!\nВремя (1 поток):  %.5f сек\n\n", time_single);

    // --- Тест 2: Многопоточный режим ---
    printf("Запуск многопоточного режима (%d потоков)... ", max_threads);
    fflush(stdout);

    clock_gettime(CLOCK_MONOTONIC, &start);
    concotinate_arrays_multi(arrays, k, n, res_multi, max_threads);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_multi = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Готово!\nВремя (%d потоков): %.5f сек\n\n", max_threads, time_multi);

    // --- Итоговое сравнение ---
    printf("=== ИТОГОВОЕ УСКОРЕНИЕ ===\n");
    printf("Ускорение:        %.2fx\n", time_single / time_multi);

    // Освобождение ресурсов
    for (int i = 0; i < k; ++i)
        free(arrays[i]);
    free(arrays);
    free(res_single);
    free(res_multi);

    return 0;
}
