#include "calculate.h"
#include <cmath>
#include <random>
#include <iostream>
#include <vector>
#include <array>

// Реализация конструктора класса Charge
Charge::Charge() {
    position = {random_double(-L / 2, L / 2), random_double(-L / 2, L / 2)};
}

// Реализация метода calculate_force класса Charge
std::array<double, 2> Charge::calculate_force(const Charge& other) const {
    std::array<double, 2> delta = {other.position[0] - position[0], other.position[1] - position[1]};

    // Apply periodic boundary conditions
    for (int i = 0; i < 2; ++i) {
        if (delta[i] > L / 2) {
            delta[i] -= L;
        } else if (delta[i] < -L / 2) {
            delta[i] += L;
        }
    }

    double r = std::sqrt(delta[0] * delta[0] + delta[1] * delta[1]);

    // Avoid division by zero
    if (r == 0.0) return {0.0, 0.0};

    double F = (k * e * e) / (r * r);

    // Calculate force vector with respect to periodic conditions
    return {F * delta[0] / r, F * delta[1] / r};
}

// Реализация метода random_double класса Charge
double Charge::random_double(double min, double max) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

// Реализация конструктора класса Simulation
Simulation::Simulation(int n) : beta1(0.9), beta2(0.999), epsilon(1e-8), learning_rate(0.01) {
    charges.resize(n);
    m.resize(n, {0.0, 0.0});
    v.resize(n, {0.0, 0.0});
}

// Реализация метода calculate_energy класса Simulation
double Simulation::calculate_energy() const {
    double W = 0.0;
    for (size_t i = 0; i < charges.size() - 1; ++i) {
        for (size_t j = i + 1; j < charges.size(); ++j) {
            std::array<double, 2> delta = {charges[j].position[0] - charges[i].position[0],
                                           charges[j].position[1] - charges[i].position[1]};

            // Apply periodic boundary conditions
            for (int k = 0; k < 2; ++k) {
                if (delta[k] > L / 2) {
                    delta[k] -= L;
                } else if (delta[k] < -L / 2) {
                    delta[k] += L;
                }
            }

            double r = std::sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
            if (r != 0.0) {
                W += k * e * e / r;
            }
        }
    }
    return W;
}

// Реализация метода update_positions класса Simulation с использованием Adam
void Simulation::update_positions() {
    double W = calculate_energy();
    double W0 = 0.0;

    int iteration = 0;
    int max_iterations = 1000;

    while (std::abs(W - W0) > 1e-6 && iteration < max_iterations) {
        W0 = W;

        for (size_t i = 0; i < charges.size(); ++i) {
            std::array<double, 2> resultant_force = {0.0, 0.0};

            // Вычисляем результирующую силу
            for (size_t j = 0; j < charges.size(); ++j) {
                if (i != j) {
                    auto force = charges[i].calculate_force(charges[j]);
                    resultant_force[0] += force[0];
                    resultant_force[1] += force[1];
                }
            }

            // Обновляем моменты m и v для Adam
            for (int k = 0; k < 2; ++k) {
                m[i][k] = beta1 * m[i][k] + (1.0 - beta1) * resultant_force[k];
                v[i][k] = beta2 * v[i][k] + (1.0 - beta2) * (resultant_force[k] * resultant_force[k]);

                // Bias-corrected moments
                double m_hat = m[i][k] / (1.0 - std::pow(beta1, iteration + 1));
                double v_hat = v[i][k] / (1.0 - std::pow(beta2, iteration + 1));

                // Обновляем позицию
                charges[i].position[k] -= learning_rate * m_hat / (std::sqrt(v_hat) + epsilon);

                // Apply periodic boundary conditions
                if (charges[i].position[k] > L / 2) {
                    charges[i].position[k] -= L;
                } else if (charges[i].position[k] < -L / 2) {
                    charges[i].position[k] += L;
                }
            }
        }

        W = calculate_energy();
        ++iteration;

        // Диагностика
        std::cout << "Iteration: " << iteration << ", Energy: " << W << "\n";

        // Визуализация
        print_positions();
    }
}

// Реализация метода print_positions класса Simulation
void Simulation::print_positions() const {
    std::cout << "Positions:\n";
    for (const auto& charge : charges) {
        std::cout << "(" << charge.position[0] << ", " << charge.position[1] << ") ";
    }
    std::cout << "\n";
}
