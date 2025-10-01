// for pool template testing purposes

#pragma once

#include "pool.h"
#include <string>

class Particle {
private:

    std::string _effect;

    int _x;
    int _y;

public:

    Particle(std::string effect, int x, int y) : _effect(effect), _x(x), _y(y) {}

    friend std::ostream& operator<<(std::ostream& os, const Particle& particle);

    static void* operator new(std::size_t size);

    static void operator delete(void* ptr) noexcept;

    static void profile();

};