#include <iostream>
#include <vector>
#include "npc.h"
#include "particle.h"

int main() {
    // Create several NPCs
    std::vector<NPC*> npcs;

    // Spawn a wave of enemies
    for (int i = 0; i < 10; ++i) {
        npcs.push_back(NPC::spawn(
            "Goblin_" + std::to_string(i),
            "enemy",
            i * 10, i * 10, // Position
            50 + i * 5 // Health
        ));
    }

    // Show one NPC
    std::cout << *npcs[5] << std::endl;

    // Delete some NPCs (simulating death)
    delete npcs[3];
    delete npcs[5];
    npcs[3] = nullptr;
    npcs[5] = nullptr;

    // Profile the pool
    NPC::profile();

    // Spawn replacements
    npcs[3] = NPC::spawn("Dragon_Boss", "boss", 50, 50, 500);
    npcs[5] = NPC::spawn("Shopkeeper", "merchant", 25, 25, 100);

    // Clean up all NPCs
    for (auto* npc : npcs) {
        delete npc;
    }

    // Final profile
    NPC::profile();


    // pool template testing with different class
    std::vector<Particle*> particles;
    particles.push_back(new Particle("fire", 3, 4));
    particles.push_back(new Particle("splash", 5, 1));
    particles.push_back(new Particle("dust", 1, 2));
    std::cout << *particles[1];
    Particle::profile();
    for (auto* particle : particles) delete particle;

    return 0;
}