#include <iostream>
#include <chrono>
#include "src/Calculate/calculate.h"
#include "src/Renderer/renderer.h"  // Include the Renderer class
#include "src/Calculate/hyperparameters.h"



int main() {
    int n;
    std::cout << "Enter number of points: ";
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    // Инициализация симуляции
    Simulation sim(n, default_physical_params, default_optimization_params);
    sim.update_positions();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Runtime: " << duration.count() << " seconds\n";

    // Преобразование позиций и радиусов для отрисовки
    std::vector<std::array<float, 2>> final_positions;
    std::vector<float> final_radii;
    for (const auto& charge : sim.charges) {
        final_positions.push_back({
                                          static_cast<float>(charge.position[0]),
                                          static_cast<float>(charge.position[1])
                                  });
        final_radii.push_back(static_cast<float>(charge.radius));
    }

    Renderer renderer;
    if (!renderer.init(800, 800, "2D Charge Distribution")) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return -1;
    }

    // Отрисовка с индивидуальными радиусами
    renderer.render(
            final_positions,
            default_physical_params.L,  // Размер системы
            final_radii                 // Вектор радиусов частиц
    );

    renderer.cleanup();
    return 0;
}
