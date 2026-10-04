#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    std::ifstream fin("Input.txt");
    if (!fin) { std::cerr << "Не удалось открыть Input.txt\n"; return 1; }

    int n;
    fin >> n;

    std::vector<double> A(n * n), B(n * n), C(n * n, 0.0);
    for (auto& v : A) fin >> v;
    for (auto& v : B) fin >> v;
    fin.close();

    auto start = std::chrono::high_resolution_clock::now();

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
        for (int j = 0; j < n; j++)
            fout << C[i * n + j] << " ";
        fout << "\n";
    }
    fout << "Time: " << elapsed << " sec\n";
    fout << "Size: " << n << "x" << n
        << " (" << 3.0 * n * n * sizeof(double) / (1024 * 1024) << " MB)\n";
    fout.close();

    std::cout << "Готово. Время = " << elapsed << " сек\n";
    return 0;
}