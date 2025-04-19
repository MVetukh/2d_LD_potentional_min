//
// Created by MaxV on 03.12.2024.
//

#ifndef INC_2D_LD_POTENTIONAL_MIN_HYPERPARAMETERS_H
#define INC_2D_LD_POTENTIONAL_MIN_HYPERPARAMETERS_H

#include <random>

struct PhysicalParams {
    double R_min;    // минимальный радиус частицы
    double R_max;    // максимальный радиус частицы
    double R;
    double L;        // размер системы
    double epsilon;  // глубина потенциала
};

// Функция возвращает случайный double в диапазоне [min, max]
double randomRadius(double min, double max);

// OptimizationParams уже содержит num_neighbors, learning_rate и т.д.
struct OptimizationParams {
    double learning_rate;
    double beta1;
    double beta2;
    double epsilon;
    int    max_iterations;
    double convergence_tol;
    double min_learning_rate;
    int    num_neighbors;
};

extern PhysicalParams   default_physical_params;
extern OptimizationParams default_optimization_params;


#endif //INC_2D_LD_POTENTIONAL_MIN_HYPERPARAMETERS_H
