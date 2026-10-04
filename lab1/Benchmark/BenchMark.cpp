#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <windows.h>

void RunExperiment(int n, int threads, double& time) {
    omp_set_num_threads(threads);
    std::vector<double> A(n * n, 1.0), B(n * n, 1.0), C(n * n, 0.0);

    auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            double a_ik = A[i * n + k];
            for (int j = 0; j < n; j++)
                C[i * n + j] += a_ik * B[k * n + j];
        }

    auto end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration<double>(end - start).count();
}

int main() {
    SetConsoleOutputCP(65001);

    std::ofstream csv("Results.csv");
    csv << "N,Threads,Time_sec\n";

    int sizes[] = { 128, 256, 512, 1024, 2048 };
    int threads[] = { 1, 2, 4, 8 };

    std::cout << "Запуск экспериментов...\n";
    for (int n : sizes) {
        for (int t : threads) {
            double time;
            RunExperiment(n, t, time);
            csv << n << "," << t << "," << time << "\n";
            std::cout << "N=" << n << " T=" << t << " Time=" << time << " sec\n";
        }
    }
    csv.close();
    std::cout << "Данные сохранены в Results.csv\n";
    return 0;
}