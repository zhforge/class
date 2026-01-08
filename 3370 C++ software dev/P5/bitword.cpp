#include "bitword.h"
#include <iostream>
#include <cstddef>

void BitWord::set(size_t pos) {
    // Set bit to 1
    word |= (1u << pos);
}

void BitWord::reset(size_t pos) {
    // Set bit to 0
    word &= ~(1u << pos);
}

void BitWord::flip(size_t pos) {
    // Toggle bit
    word ^= (1u << pos);
}

bool BitWord::test(size_t pos) const {
    // Test if bit is set
    return word & (1u << pos);
}

unsigned int BitWord::extract(size_t m, size_t n) const {
    // Extract m bits at pos n
    unsigned int mask = (1u << m) - 1;
    return (word >> n) & mask;
}

BitWord::operator unsigned int() const {
    // Convert to unsigned int
    return this->word;
}

std::ostream& operator<<(std::ostream& os, const BitWord& b) {
    std::string bit = "";
    // Convert int to binary representation string
    for (int i=8; i>=0; i--) {
        // If i'th bit is set 
        if (b.test(i)) bit += '1';
        else bit += '0';
    }
    
    os << "Integer form: " << int(b) << std::endl;
    os << "Binary form (8 bits): " << bit;
    return os;
}