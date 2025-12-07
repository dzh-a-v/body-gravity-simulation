#include "simulation.h"
#include "physics.h"

void Simulation::step() {
    // Обнуление ускорений теперь внутри Physics::computeAccelerations
    Physics::computeAccelerations(*this);

    // Интеграция методом Эйлера
    for (auto& body : bodies) {
        body.velocity = body.velocity + body.acceleration * dt;
        body.position = body.position + body.velocity * dt;
    }

    time += dt;
}

bool Simulation::hasCollision() const {
    if (bodies.size() < 2) return false;
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            Vec2 diff = bodies[i].position - bodies[j].position;
            if (diff.norm() <= bodies[i].radius + bodies[j].radius) {
                return true;
            }
        }
    }
    return false;
}