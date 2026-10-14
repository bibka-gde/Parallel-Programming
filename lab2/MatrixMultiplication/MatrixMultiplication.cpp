#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <windows.h>
#include <omp.h>

int main() {
    SetConsoleOutputCP(65001);

    std::ifstream fin("Input.txt");
    if (!fin) {
        std::cerr << "Не удалось открыть Input.txt\n";
        return 1;
    }

    int n;
    fin >> n;

    std::vector<double> A(n * n), B(n * n), C(n * n, 0.0);
    std::vector<double> v(n * n);

    for (auto& val : v) fin >> val;
    A = v;

    for (auto& val : v) fin >> val;
    B = v;

    fin.close();

    // Получаем количество потоков из переменной окружения или используем 4 по умолчанию
    int num_threads = 4;
    char* threads_env = std::getenv("OMP_NUM_THREADS");
    if (threads_env) {
        num_threads = std::atoi(threads_env);
    }

    omp_set_num_threads(num_threads);

    std::cout << "Запуск умножения матриц " << n << "x" << n
        << " с использованием " << num_threads << " потоков...\n";

    auto start = std::chrono::high_resolution_clock::now();

    // Параллельное умножение матриц с использованием OpenMP
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();

    // Вывод результатов
    std::ofstream fout("Output.txt");
    fout << n << "\n";
    fout << std::fixed << std::setprecision(6);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fout << C[i * n + j] << " ";
        }
        fout << "\n";
    }

    fout << "Time: " << elapsed << " sec\n";
    fout << "Size: " << n << "x" << n << "\n";
    fout << "Threads: " << num_threads << "\n";
    fout << "Memory: " << (3.0 * n * n * sizeof(double) / (1024 * 1024)) << " MB\n";
    fout.close();

    std::cout << "Готово. Время = " << elapsed << " сек\n";
    std::cout << "Результаты сохранены в Output.txt\n";

    return 0;
}