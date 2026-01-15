#pragma once
#include <vector>
#include <QString>
#include "helpers.h"

struct CelestialPreset {
    QString name;
    LD mass;        // kg
    LD radius;      // m
    LD x, y;        // initial position (m)
    LD vx, vy;      // initial velocity (m/s)
};

const std::vector<CelestialPreset>& getSolarSystemPresets();