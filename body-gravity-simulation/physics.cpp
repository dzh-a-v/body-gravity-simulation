#include "physics.h"
#include "simulation.h"
#include "helpers.h"

void Physics::computeAccelerations(Simulation& sim, bool twoBodyApprox) {
    if (sim.bodies.empty()) return;
    
    if (twoBodyApprox && sim.bodies.size() >= 2) {
        // Только первое тело создаёт гравитацию
        const Body& central = sim.bodies[0];
        for (size_t i = 1; i < sim.bodies.size(); ++i) {
            Body& body = sim.bodies[i];
            Vec2 r_vec = central.position - body.position;
            double r = r_vec.norm();
            if (r > 1e-6) {
                // a = G * M / r^2 * (r_vec / r) = G * M * r_vec / r^3
                body.acceleration = r_vec * (G * central.mass / (r * r * r));
            }
        }
    }
    else { // Полный N-тел расчёт (для будущего)
        for (size_t i = 0; i < sim.bodies.size(); ++i) {
            for (size_t j = 0; j < sim.bodies.size(); ++j) {
                if (i == j) continue;
                const Body& source = sim.bodies[j];
                Body& target = sim.bodies[i];
                Vec2 r_vec = source.position - target.position;
                double r = r_vec.norm();
                if (r > 1e-6) {
                    target.acceleration = target.acceleration + r_vec * (G * source.mass / (r * r * r));
                }
            }
        }
    }
}