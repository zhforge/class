/*
#include "pool.h"
#include <cstddef>
#include <vector>
#include <memory>
#include <span>
#include <iostream>
#include <format>
#include <stdexcept>

template <typename T>
Pool<T>::Pool(std::size_t count, bool trace) {
    _block_size = sizeof(T);
    trace_enabled = trace;
    alignas(alignof(T)) _memory_block = new std::byte[count * _block_size];
    for (std::size_t i = 0; i < count; i++) free_indices.push_back(i);

    // tracing
    if (trace_enabled) std::cout << std::format("Pool initialized with {} NPC slots ({} bytes per)", count, _block_size) << std::endl;
}

template <typename T>
Pool<T>::~Pool() {
    // tracing
    if (trace_enabled) std::cout << "Pool destructor called" << std::endl;
    delete[] _memory_block;
}

template <typename T>
[[nodiscard]] void* Pool<T>::allocate() {
    if (free_indices.size() == 0) {
        // check if there's no free memory left in pool
        throw std::bad_alloc();
    }

    // allocate a space in the pool for an NPC
    std::size_t free_index = free_indices.back();
    free_indices.pop_back();
    live_npcs++;

    // tracing
    if (trace_enabled) std::cout << std::format("Allocated slot index {} (at {})", free_index, get_slot(free_index)) << std::endl;

    // return the address to the allocated slot
    return get_slot(free_index);
}

template <typename T>
void Pool<T>::deallocate(void* ptr) {
    // cast ptr to a byte pointer to match _memory_block
    auto byte_ptr = static_cast<std::byte*>(ptr);

    // calculate index of allocated slot from its address
    std::size_t index = (byte_ptr - _memory_block) / _block_size;

    // push index back on free_indices to indicate deallocation
    free_indices.push_back(index);
    live_npcs--;

    // tracing
    if (trace_enabled) std::cout << std::format("Deallocated slot index {} (at {})", index, get_slot(index)) << std::endl;
}

template <typename T>
void Pool<T>::profile() const {
    std::cout << std::format("Live NPCs: {}, Free slots: {}", live_npcs, free_indices.size()) << std::endl
              << "Remaining free slots: ";
    for (auto i : free_indices) std::cout << std::format("{} ", i);
    std::cout << std::endl;
}

*/