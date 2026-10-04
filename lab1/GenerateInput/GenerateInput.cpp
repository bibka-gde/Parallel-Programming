#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <windows.h>

//создание матриц с рандомными числами
int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);

    if (argc < 2) {
        std::cerr << "Использование: GenerateInput.exe <N>\n";
        return 1;
    }

    int n = std::atoi(argv[1]);
    std::srand(42);

    std::ofstream fout("Input.txt");
    fout << n << "\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fout << std::fixed << std::setprecision(6)
                << static_cast<double>(std::rand()) / RAND_MAX << " ";
        }
        fout << "\n";
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fout << std::fixed << std::setprecision(6)
                << static_cast<double>(std::rand()) / RAND_MAX << " ";
        }
        fout << "\n";
    }

    fout.close();
    std::cout << "Создан Input.txt для N=" << n << "\n";
    return 0;
}