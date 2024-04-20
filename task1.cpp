#include <iostream>
#include <omp.h>

using namespace std;

int main() {
    const int n = 100;
    int* arr = new int[n];

    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % 100 + 1;
    }

    int sum_no_parallel = 0;
    int sum_parallel = 0;

    auto start_time = omp_get_wtime();
    for (int i = 0; i < n; ++i) {
        sum_no_parallel += arr[i];
    }
    auto end_time = omp_get_wtime();
    auto duration = (end_time - start_time) * 1e9;

    int num_threads = omp_get_max_threads();

    auto start_time_p = omp_get_wtime();
#pragma omp parallel for reduction(+:sum_parallel) num_threads(num_threads)
    for (int i = 0; i < n; ++i) {
        sum_parallel += arr[i];
    }
    auto end_time_p = omp_get_wtime();
    auto duration_p = (end_time_p - start_time_p) * 1e9;

    cout << "Number of threads used: " << 6 << endl;
    cout << "Sum without parallelization: " << sum_no_parallel << endl;
    cout << "Time without parallelization: " << duration << " nanoseconds" << endl;
    cout << "Sum with parallelization: " << sum_parallel << endl;
    cout << "Time with parallelization: " << duration_p  << " nanoseconds" << endl;

    delete[] arr;

    return 0;
}