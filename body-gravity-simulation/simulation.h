#pragma once
#include "helpers.h"
#include "body.h"

class Simulation {
public:
    std::vector<Body> bodies;
    LL double time = 0.0;
    LL double dt = 1.0; // seconds

    void addBody(const Body& body) {
        bodies.push_back(body);
    }

    bool hasCollision() const;
    bool isFinished = false;

    void step(bool twoBodyApprox = true);
};