#pragma once
#include "helpers.h"

class Output {
public:
    virtual ~Output() = default;
    virtual void render(const Simulation& sim) = 0;
};

class ConsoleOutput : public Output {
public:
    void render(const Simulation& sim) override {
        std::cout << "t = " << sim.time << " s\n";
        for (size_t i = 0; i < sim.bodies.size(); ++i) {
            const auto& b = sim.bodies[i];
            std::cout << "  [" << i << "] pos=(" << b.position.x << ", " << b.position.y << ")\n";
        }
        std::cout << "---\n";
    }
};