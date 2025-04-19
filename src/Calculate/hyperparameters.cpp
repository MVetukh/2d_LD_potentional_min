//
// Created by MaxV on 03.12.2024.
//

#include "hyperparameters.h"


// Инициализация параметров: диапазон радиусов задаём здесь
PhysicalParams default_physical_params = {
        .R_min   = 0.05,  // минимальный радиус
        .R_max   = 0.1,   // максимальный радиус

        .L       = 2.0,   // размер системы
        .epsilon = 1.0    // глубина потенциала
};

OptimizationParams default_optimization_params = {
        .learning_rate    = 0.01,
        .beta1            = 0.5,
        .beta2            = 0.98,
        .epsilon          = 1e-8,
        .max_iterations   = 1000,
        .convergence_tol  = 1e-8,
        .min_learning_rate= 1e-8,
        .num_neighbors    = 6
};

// Реализация функции генерации случайного радиуса
double randomRadius(double min, double max) {
    static std::mt19937_64 gen{std::random_device{}()};
    std::uniform_real_distribution<double> dist(min, max);
    return dist(gen);
}
