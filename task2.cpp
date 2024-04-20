#include <iostream>
#include <omp.h>

using namespace std;

void merge(int array[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* left_array = new int[n1];
    int* right_array = new int[n2];

    for (int i = 0; i < n1; ++i) {
        left_array[i] = array[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        right_array[j] = array[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (left_array[i] <= right_array[j]) {
            array[k] = left_array[i];
            ++i;
        }
        else {
            array[k] = right_array[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        array[k] = left_array[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        array[k] = right_array[j];
        ++j;
        ++k;
    }

    delete[] left_array;
    delete[] right_array;
}

void parallel_merge_sort(int array[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

#pragma omp parallel sections
        {
#pragma omp section
            parallel_merge_sort(array, left, mid);
#pragma omp section
            parallel_merge_sort(array, mid + 1, right);
        }

        merge(array, left, mid, right);
    }
}

void non_parallel_merge_sort(int array[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        non_parallel_merge_sort(array, left, mid);
        non_parallel_merge_sort(array, mid + 1, right);

        merge(array, left, mid, right);
    }
}

int main() {
    const int size = 1000;
    int elements[size];
    srand(time(NULL));
    for (int i = 0; i < size; ++i) {
        elements[i] = rand() % 1000;
    }

    int elements_non_parallel[size];
    int elements_parallel[size];
    copy(elements, elements + size, elements_non_parallel);
    copy(elements, elements + size, elements_parallel);

    double start_time_non_parallel = omp_get_wtime();
    non_parallel_merge_sort(elements_non_parallel, 0, size - 1);
    double end_time_non_parallel = omp_get_wtime();

    double start_time_parallel = omp_get_wtime();
    parallel_merge_sort(elements_parallel, 0, size - 1);
    double end_time_parallel = omp_get_wtime();

    double duration_non_parallel = (end_time_non_parallel - start_time_non_parallel) * 1e9;
    double duration_parallel = (end_time_parallel - start_time_parallel) * 1e9;

    cout << "without parallelization: " << duration_non_parallel << " nanosec" << endl;
    cout << "with parallelization: " << duration_parallel << " nanosec" << endl;

    return 0;
}