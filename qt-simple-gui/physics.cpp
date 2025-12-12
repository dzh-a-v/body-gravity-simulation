#include "physics.h"
#include "simulation.h"
#include "helpers.h"
#include <cmath>

void Physics::computeAccelerations(Simulation& sim) {
    size_t n = sim.bodies.size();

    // Resetting acceleration
    // Without resetting the simulation goes wrong
    for (auto& body : sim.bodies) {
        body.acceleration = { 0, 0 };
    }

    // Each body is attracted to all the other bodies
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (i == j) continue;

            const Body& source = sim.bodies[j];
            Body& target = sim.bodies[i];

            Vec2 r_vec = source.position - target.position;
            LD r = r_vec.norm();

            // Zero division prevention
            if (r < MIN_NUMBER) continue;

            // Acceleration: a = G * M_source / r^2 * r_vec 
            // We sum it up with the acceleration this body has
            // from all other bodies (two 'for')
            target.acceleration = target.acceleration + r_vec * (G * source.mass / (r * r * r));
        }
    }
}

LD Physics::calculateDistance(const Body& a, const Body& b) {
    Vec2 diff = a.position - b.position;
    return diff.norm();
}