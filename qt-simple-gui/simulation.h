#pragma once
#include "helpers.h"
#include "body.h"

class Simulation {
public:
    std::vector<Body> bodies;
    LD time = 0.0; // total time (s)
    LD dt = 1.0; // step (s)

    void addBody(const Body& body) {
        bodies.push_back(body);
    }

    void step();
};