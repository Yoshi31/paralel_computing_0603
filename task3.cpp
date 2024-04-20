#include <iostream>
#include <omp.h>

using namespace std;

int main() {
    const int total_points = 10000000;
    int points_inside_circle = 0;

    int num_threads = omp_get_max_threads();

    double start_time = omp_get_wtime();

#pragma omp parallel for reduction(+:points_inside_circle) num_threads(num_threads)
    for (int i = 0; i < total_points; ++i) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            points_inside_circle++;
        }
    }

    double pi = 4.0 * points_inside_circle / total_points;

    double end_time = omp_get_wtime();
    double elapsed_time = (end_time - start_time) * 1000; // в миллисекундах

    cout << "approximate value of the number Pi: " << pi << endl;
    cout << "Computation time: " << elapsed_time << " milisec" << endl;

    return 0;
}