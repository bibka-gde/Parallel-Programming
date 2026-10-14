#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <windows.h>
#include <omp.h>
#include <sstream>
#include <string>

// Функция генерации случайных матриц
void generateMatrices(int n, std::vector<double>& A, std::vector<double>& B) {
    std::srand(42);
    A.resize(n * n);
    B.resize(n * n);

    for (int i = 0; i < n * n; i++) {
        A[i] = static_cast<double>(std::rand()) / RAND_MAX;
        B[i] = static_cast<double>(std::rand()) / RAND_MAX;
    }
}

// Последовательное умножение матриц
double sequentialMultiply(const std::vector<double>& A, const std::vector<double>& B,
    std::vector<double>& C, int n) {
    C.assign(n * n, 0.0);

    auto start = std::chrono::high_resolution_clock::now();

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
    return std::chrono::duration<double>(end - start).count();
}

// Параллельное умножение матриц с OpenMP
double parallelMultiply(const std::vector<double>& A, const std::vector<double>& B,
    std::vector<double>& C, int n, int num_threads) {
    C.assign(n * n, 0.0);
    omp_set_num_threads(num_threads);

    auto start = std::chrono::high_resolution_clock::now();

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
    return std::chrono::duration<double>(end - start).count();
}

// Проверка корректности результатов
bool verifyResults(const std::vector<double>& C1, const std::vector<double>& C2, int n, double epsilon = 1e-9) {
    for (int i = 0; i < n * n; i++) {
        if (std::abs(C1[i] - C2[i]) > epsilon) {
            return false;
        }
    }
    return true;
}

int main() {
    SetConsoleOutputCP(65001);

    std::cout << "=== BENCHMARK: Matrix Multiplication with OpenMP ===\n\n";

    // Размеры матриц для тестирования
    std::vector<int> matrix_sizes = { 200, 400, 800, 1200, 1600, 2000 };
    std::vector<int> thread_counts = { 1, 2, 4, 8 };

    // Файл для сохранения результатов
    std::ofstream results("benchmark_results.csv");
    results << "Matrix_Size,Threads,Time_Seconds,Speedup,Memory_MB\n";

    // Таблица для вывода в консоль
    std::cout << std::setw(12) << "Size"
        << std::setw(12) << "Threads"
        << std::setw(15) << "Time (sec)"
        << std::setw(12) << "Speedup"
        << std::setw(12) << "Memory (MB)" << "\n";
    std::cout << std::string(63, '-') << "\n";

    for (int n : matrix_sizes) {
        std::cout << "\n--- Matrix size: " << n << "x" << n << " ---\n";

        std::vector<double> A, B, C_seq, C_par;
        generateMatrices(n, A, B);

        // Последовательное выполнение (эталон)
        double time_seq = sequentialMultiply(A, B, C_seq, n);
        double memory_mb = (3.0 * n * n * sizeof(double)) / (1024 * 1024);

        std::cout << std::setw(12) << n
            << std::setw(12) << "1 (seq)"
            << std::setw(15) << std::fixed << std::setprecision(4) << time_seq
            << std::setw(12) << "1.00"
            << std::setw(12) << std::setprecision(2) << memory_mb << "\n";

        results << n << ",1," << time_seq << ",1.00," << memory_mb << "\n";

        // Параллельное выполнение с разным количеством потоков
        for (int threads : thread_counts) {
            if (threads == 1) continue; // Уже выполнили

            double time_par = parallelMultiply(A, B, C_par, n, threads);
            double speedup = time_seq / time_par;

            // Проверка корректности
            if (!verifyResults(C_seq, C_par, n)) {
                std::cerr << "WARNING: Results mismatch for n=" << n << ", threads=" << threads << "\n";
            }

            std::cout << std::setw(12) << n
                << std::setw(12) << threads
                << std::setw(15) << std::fixed << std::setprecision(4) << time_par
                << std::setw(12) << std::setprecision(2) << speedup
                << std::setw(12) << memory_mb << "\n";

            results << n << "," << threads << "," << time_par << "," << speedup << "," << memory_mb << "\n";
        }
    }

    results.close();

    std::cout << "\n=== Benchmark completed ===\n";
    std::cout << "Results saved to: benchmark_results.csv\n";
    std::cout << "For detailed analysis, see benchmark_results.csv\n";

    return 0;
}