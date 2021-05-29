#include <omp.h>
#include <stdio.h>

#define MAXSIZE 100
#define MAXWORKERS 12

int size = MAXSIZE;
int vector[MAXSIZE];
double start_time, end_time;
int numWorkers;

void Qsort(int first, int last) {
    int pivot, i_pivot, temp, left, right;
    if (first >= last)
        return;
    i_pivot = (first + last) / 2;
    pivot = vector[i_pivot];
    left = first;
    right = last;
    while (left <= right) {
        if (vector[left] > pivot) {
            temp = vector[left];
            vector[left] = vector[right];
            vector[right] = temp;
            if (right == i_pivot) {
                i_pivot = left;
            }
            right--;
        } else {
            left++;
        }
    }
    temp = vector[right];
    vector[right] = pivot;
    vector[i_pivot] = temp;
#pragma omp task
    Qsort(first, (right - 1));
#pragma omp task
    Qsort((right + 1), last);
}

int main(int argc, char *argv[]) {
    int i;

    size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
    if (size <= 0 || size > MAXSIZE)
        size = MAXSIZE;

    numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;
    if (numWorkers > MAXWORKERS)
        numWorkers = MAXWORKERS;

    omp_set_num_threads(numWorkers);

    for (i = 0; i < size; i++)
        vector[i] = (int)random() % MAXSIZE;
#ifdef DEBUG
    printf("initial vector: \n");
    for (i = 0; i < size; i++)
        printf(" %3d", vector[i]);
    printf("\n");
#endif

    start_time = omp_get_wtime();

#pragma omp parallel
    {
#pragma omp single
        Qsort(0, (size - 1));
    }

    end_time = omp_get_wtime();
    for (i = 0; i < size - 1; i++)
        if (vector[i] > vector[i + 1]) {
            printf("The resulting vector is not sorted!\n");
        }
#ifdef DEBUG
    printf("sorted vector: \n");
    for (i = 0; i < size; i++)
        printf(" %3d", vector[i]);
    printf("\n");
#endif

    printf("It took %g seconds\n", (end_time - start_time));
}
