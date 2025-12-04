#pragma once
#include "helpers.h"
#include "simulation.h"

class Physics {
public:
    static constexpr LL double G = 6.67430e-11;

    static void computeAccelerations(Simulation& sim);
};