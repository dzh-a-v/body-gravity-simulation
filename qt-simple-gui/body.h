#pragma once
#include "helpers.h"

struct Vec2 {
    LL double x = 0.0, y = 0.0;
    Vec2 operator+(Vec2 o) const { return { x + o.x, y + o.y }; }
    Vec2 operator-(Vec2 o) const { return { x - o.x, y - o.y }; }
    Vec2 operator*(double s) const { return { x * s, y * s }; }
    LL double norm() const { return std::sqrt(x * x + y * y); }
};

class Body {
public:
    LL double mass;
    LL double radius;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration = { 0, 0 };

    LL double density() const {
        return mass / ((4.0 / 3.0) * M_PI * radius * radius * radius);
    }

    Body(LL double m = 10, LL double r = 10, Vec2 pos = { 0,0 }, Vec2 vel = { 0,0 })
        : mass(m), radius(r), position(pos), velocity(vel) {
    }
};