#include <iostream>
#include <omp.h>
#include <chrono>
#include <vector>
#include <iomanip>

using namespace std;

#define THREAD_COUNT 2

double IntegralPi(int n) {
    double h = 1.0 / n;
    double pi = 0;
    double x = h;
    for (int i = 0; i < n; i++) {
        pi += 4 / (1 + x * x);
        x += h;
    }
    return pi * h;
}

double IntegralPiOMP(int n) {
    double h = 1.0 / n;
    double pi = 0;
    
    #pragma omp parallel for reduction (+:pi)
    for (int i = 0; i < n; i++) {
        pi += 4 / (1 + h * (i + 1) * h * (i + 1));
    }
    return pi * h;
}

double measureCalculationTime(int n, int quantity = 3) {
    chrono::high_resolution_clock::time_point start, finish;
    double time_spent = __DBL_MAX__;
    for (size_t i = 0; i < quantity; ++i) {
        start = chrono::high_resolution_clock::now();
        cout << "Pi value:" << fixed << setprecision(15) << IntegralPi(n) << "\n";
        finish = chrono::high_resolution_clock::now();
        time_spent = min(time_spent,
                         chrono::duration_cast<chrono::duration<double>>(finish - start).count());
    }
    return time_spent;
}

double measureParallelCalculationTime(int n, int quantity = 3) {
    chrono::high_resolution_clock::time_point start, finish;
    double time_spent = __DBL_MAX__;
    for (size_t i = 0; i < quantity; ++i) {
        start = chrono::high_resolution_clock::now();
        cout << "PI value:" << fixed << setprecision(10) << IntegralPiOMP(n) << "\n";
        finish = chrono::high_resolution_clock::now();
        time_spent = min(time_spent,
                         chrono::duration_cast<chrono::duration<double>>(finish - start).count());
    }
    return time_spent;
}

int main(int argc, char **argv) {
    omp_set_num_threads(THREAD_COUNT);
    vector<int> n_values = {10, 1000, 100000, 1000000, 10000000, 50000000, 100000000, 500000000};
    
    vector<double> one_thread_time, multi_thread_time;
    
    cout << "1Thread\n";
    for (int i = 0; i < n_values.size(); ++i) {
        one_thread_time.push_back(measureCalculationTime(n_values[i]));
        cout << "n: " << n_values[i] << ", time spent: " << one_thread_time[i] << "\n";
    }
    
    cout << "\n--------------------------\n";
    cout << THREAD_COUNT << "Threads\n";
    for (int i = 0; i < n_values.size(); ++i) {
        multi_thread_time.push_back(measureParallelCalculationTime(n_values[i]));
        cout << "n: " << n_values[i] << ", time spent: " << multi_thread_time[i] << "\n";
    }
    
    cout << "\n--------------------------\n";
    cout << "Speed increasing\n";
    for (int i = 0; i < n_values.size(); ++i) {
        cout << "For n: " << n_values[i] << " " << one_thread_time[i] / multi_thread_time[i] << "x\n";
    }
    return 0;
}

