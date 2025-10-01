#ifndef POOL_H
#define POOL_H

#include <cstddef>
#include <vector>
#include <memory>
#include <span>
#include <iostream>
#include <format>
#include <format>
#include <stdexcept>

template <typename T> class Pool {
private:
    // Memory storage
    alignas(alignof(T)) std::byte* _memory_block; //pointer to array of bytes ('new' size of array)
    std::size_t _block_size; //how we divide up the pool

    // Free slot tracking using vector
    std::vector<std::size_t> free_indices; // Indices of free slots

    // Statistics
    std::size_t live_objects = 0; //how many objects exist in the pool
    bool trace_enabled; //if true, dump lots of extra info to stdout

    // get memory address for an index
    // (nodiscard means throw if the output of this member is being ignored)
    [[nodiscard]] void* get_slot(std::size_t index) {
        return _memory_block + (_block_size * index);
    }
    
    // Get a safe view of the memory
    // this is optional, but recommended
    [[nodiscard]] std::span<std::byte> get_memory_span() {
        std::span<std::byte> _memory_span(_memory_block, _block_size * free_indices.size());
        return _memory_span;
    }

public:

    Pool(std::size_t count, bool trace = true) 
    : _block_size(sizeof(T)), trace_enabled(trace) {
        _memory_block = new std::byte[count * _block_size];
        for (std::size_t i = 0; i < count; i++) free_indices.push_back(i);

        // tracing
        if (trace_enabled) std::cout << std::format("Pool initialized with {} object slots ({} bytes per)", count, _block_size) << std::endl;
    }

    ~Pool() {
        // tracing
        if (trace_enabled) std::cout << "Pool destructor called" << std::endl;
        delete[] _memory_block;
    }

    [[nodiscard]] void* allocate() {
        if (free_indices.size() == 0) {
            // check if there's no free memory left in pool
            throw std::bad_alloc();
        }

        // allocate a space in the pool for an object
        std::size_t free_index = free_indices.back();
        free_indices.pop_back();
        live_objects++;

        // tracing
        if (trace_enabled) std::cout << std::format("Allocated slot index {} (at {})", free_index, get_slot(free_index)) << std::endl;

        // return the address to the allocated slot
        return get_slot(free_index);
    }

    void deallocate(void* ptr) {
        // cast ptr to a byte pointer to match _memory_block
        auto byte_ptr = static_cast<std::byte*>(ptr);

        // calculate index of allocated slot from its address
        std::size_t index = (byte_ptr - _memory_block) / _block_size;

        // push index back on free_indices to indicate deallocation
        free_indices.push_back(index);
        live_objects--;

        // tracing
        if (trace_enabled) std::cout << std::format("Deallocated slot index {} (at {})", index, get_slot(index)) << std::endl;
    }
    
    void profile() const {
        std::cout << std::format("Live objects: {}, Free slots: {}", live_objects, free_indices.size()) << std::endl
                  << "Remaining free slots: ";
        for (auto i : free_indices) std::cout << std::format("{} ", i);
        std::cout << std::endl;
    }
};

#endif