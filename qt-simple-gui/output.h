#pragma once
#include "helpers.h"

class Output {
public:
    virtual ~Output() = default;
    virtual void render(const Simulation& sim) = 0;
};

class ConsoleOutput : public Output {
public:
    void render(const Simulation& sim) {
        std::cout << "t = " << sim.time << " s\n";
        for (size_t i = 0; i < sim.bodies.size(); ++i) {
            const auto& b = sim.bodies[i];
            double speed = sqrt(pow(b.velocity.x, 2) + pow(b.velocity.y, 2));
            double acc = sqrt(pow(b.acceleration.x, 2) + pow(b.acceleration.y, 2));
            std::cout << "  [" << i << "] "
                << "pos=(" << b.position.x << ", " << b.position.y << ") "
                << "vel=(" << b.velocity.x << ", " << b.velocity.y << ") "
                << "acc=(" << b.acceleration.x << ", " << b.acceleration.y << ") "
                << "|v|=" << speed << ", "
                << "|a|=" << acc << std::endl;

        }
        std::cout << "---\n";
    }
};