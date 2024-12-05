//
// Created by MaxV on 03.12.2024.
//

#include "hyperparameters.h"


// Изначальные параметры
PhysicalParams default_physical_params = {
        .k = 1.0,                // Константа
        .e = 1.0,                // Заряд частиц
        .R = 1.0,                // Радиус (если необходимо)
        .L = 2.0                 // Размер области
};

OptimizationParams default_optimization_params = {
        .learning_rate = 0.00125,  // Коэффициент обучения
        .beta1 = 0.87,            // Параметр для первого момента (скользящее среднее)
        .beta2 = 0.838,             // Параметр для второго момента (квадраты градиентов)
        .epsilon = 1e-8,           // Малое число для предотвращения деления на ноль
        .max_iterations = 1000,    // Максимальное количество итераций
        .convergence_tol = 1e-8    // Погрешность сходимости
};

