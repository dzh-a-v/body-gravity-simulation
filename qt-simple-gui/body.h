#pragma once
#include "helpers.h"

struct Vec2 {
    LD x = 0.0, y = 0.0;
    Vec2 operator+(Vec2 o) const { return { x + o.x, y + o.y }; }
    Vec2 operator-(Vec2 o) const { return { x - o.x, y - o.y }; }
    Vec2 operator*(LD s) const { return { x * s, y * s }; }
    LD norm() const { return std::sqrt(x * x + y * y); }
};

class Body {
public:
    LD mass;
    LD radius;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration = { 0, 0 };

    LD density() const {
        return mass / ((4.0 / 3.0) * M_PI * radius * radius * radius);
    }

    Body(LD m = 10, LD r = 10, Vec2 pos = { 0,0 }, Vec2 vel = { 0,0 })
        : mass(m), radius(r), position(pos), velocity(vel) {
    }
};