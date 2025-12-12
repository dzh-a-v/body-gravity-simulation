#pragma once
#include "helpers.h"
#include "simulation.h"

class Physics {
public:
    static constexpr LD G = 6.67430e-11;

    static void computeAccelerations(Simulation& sim);
    static LD calculateDistance(const Body& a, const Body& b);
};