//
// Created by MaxV on 03.12.2024.
//

#include "hyperparameters.h"


// Изначальные параметры
PhysicalParams default_physical_params = {
        .k = 1.38*10-23,                // Константа Больцмана
        .e = 1.0,                // Заряд частиц
//        .R = 1.0,                // Радиус (если необходимо)
        .L = 2.0,               // Размер области
       .epsilon = 10e-3,
        .delta = 0.2*10e-9,
};

OptimizationParams default_optimization_params = {
        .learning_rate = 0.001,  // Коэффициент обучения
        .beta1 = 0.99,            // Параметр для первого момента (скользящее среднее)
        .beta2 = 0.88,             // Параметр для второго момента (квадраты градиентов)
        .epsilon = 1e-8,           // Малое число для предотвращения деления на ноль
        .max_iterations = 1000,    // Максимальное количество итераций
        .convergence_tol = 1e-8    // Погрешность сходимости
};

