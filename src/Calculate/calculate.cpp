#include "calculate.h"
#include <cmath>
#include <random>
#include <iostream>

const double L = 2.0;  // Side length of the square domain

// Реализация конструктора класса Charge
Charge::Charge() {
    position = { random_double(-L / 2, L / 2), random_double(-L / 2, L / 2) };
}

// Реализация метода calculate_force класса Charge
std::array<double, 2> Charge::calculate_force(const Charge& other) const {
    std::array<double, 2> delta = { other.position[0] - position[0], other.position[1] - position[1] };

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
    if (r == 0.0) return { 0.0, 0.0 };

    double F = (k * e * e) / (r * r);

    // Calculate force vector with respect to periodic conditions
    return { F * delta[0] / r, F * delta[1] / r };
}

// Реализация метода random_double класса Charge
double Charge::random_double(double min, double max) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

// Реализация конструктора класса Simulation
Simulation::Simulation(int n) {
    charges.resize(n);
}

// Реализация метода calculate_energy класса Simulation
double Simulation::calculate_energy() const {
    double W = 0.0;
    for (size_t i = 0; i < charges.size() - 1; ++i) {
        for (size_t j = i + 1; j < charges.size(); ++j) {
            std::array<double, 2> delta = { charges[j].position[0] - charges[i].position[0],
                                            charges[j].position[1] - charges[i].position[1] };

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

// Реализация метода update_positions класса Simulation
void Simulation::update_positions() {
    double W = 1, W0 = 0;
    double step_multiplier = 0.09;  // начальный множитель шага
    const double step_decrement = 0.5;
    bool positions_updated;

    while (std::abs(W - W0) > 1e-6) {
        W0 = calculate_energy();
        positions_updated = false;

        for (size_t i = 0; i < charges.size(); ++i) {
            std::array<double, 2> resultant_force = { 0.0, 0.0 };
            for (size_t j = 0; j < charges.size(); ++j) {
                if (i != j) {
                    auto force = charges[i].calculate_force(charges[j]);
                    resultant_force[0] += force[0];
                    resultant_force[1] += force[1];
                }
            }

            auto previous_position = charges[i].position;

            for (int k = 0; k < 2; ++k) {
                charges[i].position[k] += step_multiplier * resultant_force[k];

                // Apply periodic wrapping for each coordinate
                if (charges[i].position[k] > L / 2) {
                    charges[i].position[k] -= L;
                } else if (charges[i].position[k] < -L / 2) {
                    charges[i].position[k] += L;
                }
            }

            double new_energy = calculate_energy();
            if (new_energy < W0) {
                positions_updated = true;
            } else {
                charges[i].position = previous_position;
            }
        }

        if (!positions_updated) {
            step_multiplier *= step_decrement;
        }

        W = calculate_energy();
    }
}

// Реализация метода print_positions класса Simulation
void Simulation::print_positions() const {
    for (const auto& charge : charges) {
        std::cout << "Position: (" << charge.position[0] << ", " << charge.position[1] << ")\n";
    }
}
