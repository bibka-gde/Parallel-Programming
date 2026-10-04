#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <omp.h>

int main(int argc, char* argv[]) {
    int num_threads = (argc > 1) ? std::atoi(argv[1]) : 4;
    omp_set_num_threads(num_threads);

    std::ifstream fin("Input.txt");
    if (!fin) { std::cerr << "Не удалось открыть Input.txt\n"; return 1; }

    int n; fin >> n;
    std::vector<double> A(n * n), B(n * n), C(n * n, 0.0);
    for (auto& v : A) fin >> v;
    for (auto& v : B) fin >> v;
    fin.close();

    auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            double a_ik = A[i * n + k];
            for (int j = 0; j < n; j++)
                C[i * n + j] += a_ik * B[k * n + j];
        }

    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();

    std::ofstream fout("Output.txt");
    fout << n << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) fout << C[i * n + j] << " ";
        fout << "\n";
    }
    fout << "Time: " << elapsed << " sec\n";
    fout << "Threads: " << num_threads << "\n";
    fout << "Size: " << n << "x" << n
        << " (" << 3.0 * n * n * sizeof(double) / (1024 * 1024) << " MB)\n";
    fout.close();

    std::cout << "N=" << n << " T=" << num_threads
        << " Time=" << elapsed << " sec\n";
    return 0;
}