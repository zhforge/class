#ifndef NPC_H
#define NPC_H

#include "pool.h"
#include <string>

class NPC {
private: 

    NPC(const std::string& name, const std::string& type, int x, int y, int health);

    std::string _name;
    std::string _type;

    int _x, _y;     // position coordinates
    int _health;    // hp

public:

    // factory method
    static NPC* spawn(const std::string& name, const std::string& type, int x, int y, int health = 100);

    //this is for ease of displaying the contents of an NPC object.
    friend std::ostream& operator<<(std::ostream& os, const NPC& npc);
    // Should output: "NPC[name='Goblin_247', type='enemy', pos=(5,10), hp=100]"

    static void* operator new(std::size_t size);
    static void operator delete(void* ptr) noexcept;

    static void profile();
};

#endif