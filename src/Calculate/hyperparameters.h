//
// Created by MaxV on 03.12.2024.
//

#ifndef INC_2D_LD_POTENTIONAL_MIN_HYPERPARAMETERS_H
#define INC_2D_LD_POTENTIONAL_MIN_HYPERPARAMETERS_H

struct PhysicalParams {
    double k;  // электростатическая постоянная
    double e;  // заряд
    double R;  // радиус сферы
    double L;  // длина области
    double epsilon;
    double delta;
};

struct OptimizationParams {
    double learning_rate;
    double beta1;
    double beta2;
    double epsilon;
    int max_iterations;
    double convergence_tol;
};

// Объявление глобальных переменных
extern PhysicalParams default_physical_params;
extern OptimizationParams default_optimization_params;


#endif //INC_2D_LD_POTENTIONAL_MIN_HYPERPARAMETERS_H
