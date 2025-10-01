#include "npc.h"
#include <string>
#include <format>

inline static Pool<NPC> pool(100, true); // 100 NPCs, tracing enabled
/* It didn't really make sense to me how the pool could be initialized inline
when it requires the size of the object in its constructor, 
and the NPC class is an incomplete object at that time */

NPC::NPC(const std::string& name, const std::string& type, int x, int y, int health) 
    : _name(name), _type(type), _x(x), _y(y), _health(health) {
}

NPC* NPC::spawn(const std::string& name, const std::string& type, int x, int y, int health) {
    // ensure all NPC objects are allocated on the heap
    return new NPC(name, type, x, y, health);
}

std::ostream& operator<<(std::ostream& os, const NPC& npc) {
    return os << std::format("NPC[name='{}', type='{}', pos=({}, {}), hp={}]", npc._name, npc._type, npc._x, npc._y, npc._health);
}

void* NPC::operator new(std::size_t size) {
    return pool.allocate();
}

void NPC::operator delete(void* ptr) noexcept {
    pool.deallocate(ptr);
}

void NPC::profile() {
    pool.profile();
}