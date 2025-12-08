#include "physics.h"
#include "simulation.h"
#include "helpers.h"

void Physics::computeAccelerations(Simulation& sim) {
    size_t n = sim.bodies.size();

    // Обнуляем ускорения
    for (auto& body : sim.bodies) {
        body.acceleration = { 0, 0 };
    }

    // Каждое тело притягивается ко всем остальным
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (i == j) continue;

            const Body& source = sim.bodies[j];
            Body& target = sim.bodies[i];

            Vec2 r_vec = source.position - target.position;
            LD r = r_vec.norm();

            // Защита от деления на ноль
            if (r < 1e-6) continue;

            // Ускорение: a = G * M_source / r^2 * направление
            target.acceleration = target.acceleration + r_vec * (G * source.mass / (r * r * r));
        }
    }
}