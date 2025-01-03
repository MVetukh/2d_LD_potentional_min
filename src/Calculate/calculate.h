#ifndef INC_2D_LD_POTENTIONAL_MIN_CALCULATE_H
#define INC_2D_LD_POTENTIONAL_MIN_CALCULATE_H

#include <array>
#include <vector>
#include "hyperparameters.h"

// Класс для представления заряда
class Charge {
public:
    std::array<double, 2> position;

    explicit Charge(const PhysicalParams& params);

    std::array<double, 2> calculate_force(const Charge& other, const PhysicalParams& params) const;

    double random_double(double min, double max) const;

private:

};

// Класс для симуляции системы зарядов
class Simulation {
public:
    Simulation(int n, const PhysicalParams& phys_params, const OptimizationParams& opt_params);

    double calculate_energy() const;

    std::vector<Charge> charges;

    void update_positions();

    void print_positions() const;

private:
    const PhysicalParams& phys_params;
    const OptimizationParams& opt_params;

    std::vector<std::array<double, 2>> m;
    std::vector<std::array<double, 2>> v;
};

#endif //INC_2D_LD_POTENTIONAL_MIN_CALCULATE_H
