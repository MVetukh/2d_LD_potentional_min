#include <iostream>
#include "src/Calculate/calculate.h"
#include <chrono>

int main() {
    int n;
    std::cout << "Enter number of points: ";
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    Simulation sim(n);
    sim.update_positions();

    // Получение позиций зарядов для отрисовки
    sim.print_positions();

    auto end = std::chrono::high_resolution_clock::now();

    // Вычисление времени выполнения
    std::chrono::duration<double> duration = end - start;
    std::cout << "Runtime: " << duration.count() << " seconds\n";

    return 0;
}
