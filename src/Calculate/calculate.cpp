#include "calculate.h"
#include <cmath>
#include <iostream>
#include <random>

// Реализация конструктора класса Charge
Charge::Charge(const PhysicalParams& params) {
    position = {random_double(-params.L / 2, params.L / 2), random_double(-params.L / 2, params.L / 2)};
}

std::array<double, 2> Charge::calculate_force(const Charge& other, const PhysicalParams& params) const {
    std::array<double, 2> delta = {other.position[0] - position[0], other.position[1] - position[1]};

    for (int i = 0; i < 2; ++i) {
        if (delta[i] > params.L / 2) delta[i] -= params.L;
        else if (delta[i] < -params.L / 2) delta[i] += params.L;
    }

    double r = std::sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
    if (r == 0.0) return {0.0, 0.0};

    double F = (params.k * params.e * params.e) / (r * r);
    return {F * delta[0] / r, F * delta[1] / r};
}

double Charge::random_double(double min, double max) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

Simulation::Simulation(int n, const PhysicalParams& phys_params, const OptimizationParams& opt_params)
        : phys_params(phys_params), opt_params(opt_params) {
    // Инициализируем каждый заряд с уникальными значениями
    charges.reserve(n);
    for (int i = 0; i < n; ++i) {
        charges.push_back(Charge(phys_params));  // Инициализация каждого заряда
    }
    m.resize(n, {0.0, 0.0});
    v.resize(n, {0.0, 0.0});
}


double Simulation::calculate_energy() const {
    double W = 0.0;
    for (size_t i = 0; i < charges.size() - 1; ++i) {
        for (size_t j = i + 1; j < charges.size(); ++j) {
            std::array<double, 2> delta = {charges[j].position[0] - charges[i].position[0],
                                           charges[j].position[1] - charges[i].position[1]};

            for (int k = 0; k < 2; ++k) {
                if (delta[k] > phys_params.L / 2) delta[k] -= phys_params.L;
                else if (delta[k] < -phys_params.L / 2) delta[k] += phys_params.L;
            }

            double r = std::sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
            if (r != 0.0) W += phys_params.k * phys_params.e * phys_params.e / r;
        }
    }
    return W;
}

double random_double_fluct(double a, double b) {
    return a + (b - a) * static_cast<double>(rand())/RAND_MAX ;
}


void Simulation::update_positions() {
    double W = calculate_energy(), W0 = 0.0;
    int iteration = 0;

    while (std::abs(W - W0) > opt_params.convergence_tol && iteration < opt_params.max_iterations) {
        W0 += W;

        for (size_t i = 0; i < charges.size(); ++i) {
            std::array<double, 2> resultant_force = {0.0, 0.0};

            // Вычисляем силы между зарядами
            for (size_t j = 0; j < charges.size(); ++j) {
                if (i != j) {
                    auto force = charges[i].calculate_force(charges[j], phys_params);
                    resultant_force[0] += force[0];
                    resultant_force[1] += force[1];
                }
            }

            // Обновляем позиции зарядов с добавлением случайных колебаний
            for (int k = 0; k < 2; ++k) {
                m[i][k] = opt_params.beta1 * m[i][k] + (1.0 - opt_params.beta1) * resultant_force[k];
                v[i][k] = opt_params.beta2 * v[i][k] + (1.0 - opt_params.beta2) * (resultant_force[k] * resultant_force[k]);

                double m_hat = m[i][k] / (1.0 - std::pow(opt_params.beta1, iteration + 1));
                double v_hat = v[i][k] / (1.0 - std::pow(opt_params.beta2, iteration + 1));

                // Обновляем позицию с учетом случайных колебаний
                charges[i].position[k] -= opt_params.learning_rate * m_hat / (std::sqrt(v_hat) + opt_params.epsilon);

                // Добавляем небольшое случайное смещение (колебания)
               charges[i].position[k] += random_double_fluct(-0.00001, 0.0001);  // Значение может быть настроено

                // Периодические граничные условия
                if (charges[i].position[k] > phys_params.L / 2) charges[i].position[k] -= phys_params.L;
                else if (charges[i].position[k] < -phys_params.L / 2) charges[i].position[k] += phys_params.L;
            }
        }

        W = calculate_energy();
        ++iteration;

        if (iteration % 10 == 0)
            std::cout << "Iteration: " << iteration << ", Energy: " << W << "\n";
    }
}


void Simulation::print_positions() const {
    std::cout << "Positions:\n";
    for (const auto& charge : charges)
        std::cout << "(" << charge.position[0] << ", " << charge.position[1] << ") ";
    std::cout << "\n";
}
