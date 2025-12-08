#include "simulation.h"
#include "physics.h"

void Simulation::step() {
    Physics::computeAccelerations(*this);

    for (auto& body : bodies) {
        body.velocity = body.velocity + body.acceleration * dt;
        body.position = body.position + body.velocity * dt;
    }

    time += dt;
}