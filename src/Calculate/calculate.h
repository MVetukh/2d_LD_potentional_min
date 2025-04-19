#ifndef INC_2D_LD_POTENTIONAL_MIN_CALCULATE_H
#define INC_2D_LD_POTENTIONAL_MIN_CALCULATE_H

#include <array>
#include <vector>
#include <random>
#include "hyperparameters.h"

#include <vector>
#include <array>
#include "hyperparameters.h"  // Defines PhysicalParams and OptimizationParams

// Represents a point/charge with a 2D position
struct Charge {
    std::array<double, 2> position;
    double radius;
    // Constructor initializes position randomly within [-L/2, L/2]
    Charge(const PhysicalParams& params);
};

// Simulation class: holds charges and runs minimization
class Simulation {
public:
    // Constructor: creates n charges with given physical and optimization parameters
    Simulation(int n, const PhysicalParams& phys_params, OptimizationParams& opt_params);

    // Runs coordinate descent minimization
    void update_positions();

    // Computes total Lennard-Jones energy of the system
    double calculate_energy() const;

    // List of charges in the system
    std::vector<Charge> charges;

private:
    PhysicalParams phys_params;
    OptimizationParams opt_params;
};

// Applies periodic boundary conditions to all charges (wraps positions into box [-1,1])
void applyPBC(std::vector<Charge>& charges);

// Coordinate descent minimization on charges:
// - num_neighbors: how many nearest neighbors to consider
// - lr: learning rate
// - tol: convergence tolerance on energy change
// - max_iters: maximum iterations
void coordinateDescent(std::vector<Charge>& charges,
                       int num_neighbors,
                       double lr,
                       double tol,
                       int max_iters,
                       const PhysicalParams& phys_params);

#endif //INC_2D_LD_POTENTIONAL_MIN_CALCULATE_H
