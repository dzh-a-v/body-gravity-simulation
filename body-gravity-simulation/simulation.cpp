#include "simulation.h"
#include "physics.h"

void Simulation::step(bool twoBodyApprox) {
    // 1. Обнуляем ускорения
    for (auto& body : bodies) {
        body.acceleration = { 0, 0 };
    }

    // 2. Вычисляем ускорения через Physics
    Physics::computeAccelerations(*this, twoBodyApprox);

    // 3. Интегрируем уравнения движения (метод Эйлера — для MVP)
    for (auto& body : bodies) {
        body.velocity = body.velocity + body.acceleration * dt;
        body.position = body.position + body.velocity * dt;
    }

    time += dt;
}

bool Simulation::hasCollision() const {
    if (bodies.size() < 2) return false;

    const Body& b1 = bodies[0];
    const Body& b2 = bodies[1];

    Vec2 diff = b1.position - b2.position;
    double distance = diff.norm();
    return distance <= (b1.radius + b2.radius);
}