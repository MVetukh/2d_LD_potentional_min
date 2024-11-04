#include <iostream>
#include <chrono>
#include "src/Calculate/calculate.h"
#include "src/Renderer/Renderer.h"  // Include the Renderer class

int main() {
    int n;
    std::cout << "Enter number of points: ";
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    // Initialize simulation
    Simulation sim(n);
    sim.update_positions();  // Calculate final positions of charges

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Runtime: " << duration.count() << " seconds\n";

    // Initialize renderer
    Renderer renderer;
    if (!renderer.init(800, 800, "2D Charge Distribution")) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return -1;
    }

    // Convert charge positions from double to float and pass to renderer
    std::vector<std::array<float, 2>> final_positions;
    for (const auto& charge : sim.charges) {
        final_positions.push_back({static_cast<float>(charge.position[0]), static_cast<float>(charge.position[1])});
    }

    renderer.render(final_positions);  // Display final positions
    renderer.cleanup();  // Cleanup rendering resources

    return 0;
}
