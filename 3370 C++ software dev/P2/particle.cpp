#include "particle.h"
#include <string>
#include <format>

inline static Pool<Particle> pool(20, true);

std::ostream& operator<<(std::ostream& os, const Particle& particle) {
    return os << std::format("Particle[effect='{}', pos=({}, {})]", particle._effect, particle._x, particle._y) << std::endl;
}

void* Particle::operator new(std::size_t size) {
    return pool.allocate();
}

void Particle::operator delete(void* ptr) noexcept {
    pool.deallocate(ptr);
}

void Particle::profile() {
    pool.profile();
}