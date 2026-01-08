#pragma once
#include <iostream>
#include <cstddef>

class BitWord {
private:
    unsigned int word;

public:
    explicit BitWord(unsigned int n = 0) : word(n) {}

    void set(size_t pos);                           // Set bit to 1
    void reset(size_t pos);                         // Set bit to 0
    void flip(size_t pos);                          // Toggle bit
    bool test(size_t pos) const;                    // Test if bit is set
    unsigned int extract(size_t m, size_t n) const; // Extract m bits at pos n

    operator unsigned int() const; // Convert to unsigned int
    friend std::ostream& operator<<(std::ostream& os, const BitWord& b);
};