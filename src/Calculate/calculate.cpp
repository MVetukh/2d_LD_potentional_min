#include "calculate.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

// Charge constructor: initialize random position in [-L/2, L/2]
// В конструкторе Charge:
Charge::Charge(const PhysicalParams& params) {
    static std::mt19937_64 gen{std::random_device{}()};
    std::uniform_real_distribution<double> dist_pos(-params.L/2.0, params.L/2.0);
    position[0] = dist_pos(gen);
    position[1] = dist_pos(gen);
    radius  = randomRadius(params.R_min, params.R_max);
}

// Simulation constructor: store params and create charges
Simulation::Simulation(int n,
                       const PhysicalParams& phys_params,
                       OptimizationParams& opt_params)
        : phys_params(phys_params), opt_params(opt_params)
{
    charges.reserve(n);
    for (int i = 0; i < n; ++i) {
        charges.emplace_back(phys_params);
    }
}

// Applies periodic boundary condition for coordinate in [-1,1]
inline double applyPBCCoord(double x) {
    double res = std::fmod(x + 1.0, 2.0);
    if (res < 0) res += 2.0;
    return res - 1.0;
}

void applyPBC(std::vector<Charge>& charges) {
    for (auto& c : charges) {
        c.position[0] = applyPBCCoord(c.position[0]);
        c.position[1] = applyPBCCoord(c.position[1]);
    }
}
static double localEnergy(int i,
                          const std::vector<Charge>& charges,
                          int num_neighbors,
                          const PhysicalParams& phys_params)
{
    int N = charges.size();
    std::vector<std::pair<double,int>> dist_idx;
    dist_idx.reserve(N-1);

    for (int j = 0; j < N; ++j) {
        if (j == i) continue;
        double dx = charges[j].position[0] - charges[i].position[0];
        double dy = charges[j].position[1] - charges[i].position[1];
        dx -= std::round(dx/phys_params.L)*phys_params.L;
        dy -= std::round(dy/phys_params.L)*phys_params.L;
        double r2 = dx*dx + dy*dy;
        dist_idx.emplace_back(r2, j);
    }

    int k = std::min(num_neighbors, N-1);
    std::nth_element(dist_idx.begin(), dist_idx.begin()+k, dist_idx.end());

    double energy = 0;
    for (int t = 0; t < k; ++t) {
        int j = dist_idx[t].second;
        double r = std::sqrt(std::max(dist_idx[t].first, 1e-12));
        // sigma = sum of radii
        double sigma = charges[i].radius + charges[j].radius;
        double sr = sigma/r;
        double sr6 = std::pow(sr,6);
        energy += 4*phys_params.epsilon*(sr6*sr6 - sr6);
    }
    return energy;
}

// Coordinate descent optimization
void coordinateDescent(std::vector<Charge>& charges,
                       int num_neighbors,
                       double lr,
                       double tol,
                       int max_iters,
                       const PhysicalParams& phys_params) {
    int N = static_cast<int>(charges.size());
    double step_scale = 1.0;
    double prev_energy = std::numeric_limits<double>::infinity();

    for (int iter = 0; iter < max_iters; ++iter) {
        double total_energy = 0.0;
        bool moved = false;

        for (int i = 0; i < N; ++i) {
            for (int dim = 0; dim < 2; ++dim) {
                double& coord = charges[i].position[dim];
                double old_val = coord;
                double e_curr = localEnergy(i, charges, num_neighbors, phys_params);

                // positive step
                coord = old_val + step_scale * lr;
                applyPBC(charges);
                double e_plus = localEnergy(i, charges, num_neighbors, phys_params);

                // negative step
                coord = old_val - step_scale * lr;
                applyPBC(charges);
                double e_minus = localEnergy(i, charges, num_neighbors, phys_params);

                // reset back
                coord = old_val;
                applyPBC(charges);

                if (e_plus < e_curr && e_plus < e_minus) {
                    coord = old_val + step_scale * lr;
                    total_energy += e_plus;
                    moved = true;
                } else if (e_minus < e_curr) {
                    coord = old_val - step_scale * lr;
                    total_energy += e_minus;
                    moved = true;
                } else {
                    total_energy += e_curr;
                }
            }
        }
        if (std::abs(prev_energy - total_energy) < tol) {
            std::cout << "Converged at iteration " << iter
                      << ", energy = " << total_energy << std::endl;
            break;
        }
        prev_energy = total_energy;
        if (!moved) {
            step_scale *= 0.5;
            if (step_scale < 1e-4) {
                std::cout << "Step scale too small, stopping at iter " << iter << std::endl;
                break;
            }
        }
    }
}

// Replace update_positions with coordinate descent in Simulation
void Simulation::update_positions() {
    std::cout<<"Start CD descent\n";
    applyPBC(charges);
    coordinateDescent(charges,
                      opt_params.num_neighbors,
                      opt_params.learning_rate,
                      opt_params.convergence_tol,
                      opt_params.max_iterations,
                      phys_params);
    std::cout<<"Final energy="<<calculate_energy()<<std::endl;
}

// Total energy computation (if needed elsewhere)
double Simulation::calculate_energy() const {
    double energy = 0.0;
    int N = static_cast<int>(charges.size());
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            double dx = charges[j].position[0] - charges[i].position[0];
            double dy = charges[j].position[1] - charges[i].position[1];
            dx -= std::floor(dx / phys_params.L + 0.5) * phys_params.L;
            dy -= std::floor(dy / phys_params.L + 0.5) * phys_params.L;
            double r = std::hypot(dx, dy);
            r = std::max(r, 1e-12);
            double sigma = phys_params.R * 2.0;
            double sr = sigma / r;
            double sr6 = std::pow(sr, 6);
            energy += 4.0 * phys_params.epsilon * (sr6 * sr6 - sr6);
        }
    }
    return energy;
}
