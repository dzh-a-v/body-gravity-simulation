#include "presets.h"

const std::vector<CelestialPreset>& getSolarSystemPresets() {
    static const std::vector<CelestialPreset> presets = {
        {"Sun",          1.989e30,   6.9634e8,     0.0, 0.0, 0.0, 0.0},
        {"Mercury",      3.301e23,   2.4397e6,     0.0, 0.0, 0.0, 0.0},
        {"Venus",        4.867e24,   6.0518e6,     0.0, 0.0, 0.0, 0.0},
        {"Earth",        5.972e24,   6.371e6,      0.0, 0.0, 0.0, 0.0},
        {"Moon",         7.342e22,   1.737e6,      0.0, 0.0, 0.0, 0.0},
        {"Satellite",    1000.0,     1.0,          0.0, 0.0, 0.0, 0.0},
        {"Mars",         6.417e23,   3.3895e6,     0.0, 0.0, 0.0, 0.0},
        {"Jupiter",      1.898e27,   6.9911e7,     0.0, 0.0, 0.0, 0.0},
        {"Saturn",       5.683e26,   5.8232e7,     0.0, 0.0, 0.0, 0.0},
        {"Uranus",       8.681e25,   2.5362e7,     0.0, 0.0, 0.0, 0.0},
        {"Neptune",      1.024e26,   2.4622e7,     0.0, 0.0, 0.0, 0.0}
    };
    return presets;
}