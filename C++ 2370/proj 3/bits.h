#ifndef BITS_H
#define BITS_H

#include <iostream>
#include <bitset>
#include <cassert>

class Bits {
    using IType = unsigned long long; // We only have to change the integer type here, if desired
    enum {NBITS = sizeof(IType)*8};
    IType bits = 0;
public:
    Bits() = default;
    Bits(IType n) {
        bits = n;
    }
    static int size() {
        return NBITS;
    }
    bool at(int pos) const {  // Returns (tests) the bit at bit-position pos
        assert(0 <= pos && pos < NBITS);
        return bits & (IType(1) << pos);
    }
    void set(int pos) {        // Sets the bit at position pos
        assert(0 <= pos && pos < NBITS);
        bits |= (1 << pos);
    }
    void set() {               // Sets all bits
        bits |= ~(0);
    }
    void reset(int pos) {      // Resets (makes zero) the bit at position pos
        assert(0 <= pos && pos < NBITS);
        bits &= ~(1 << pos);
    }
    void reset() {             // Resets all bits
        bits &= (0);
    }
    void assign(int pos, bool val) { // Sets or resets the bit at position pos depending on val
        assert(0 <= pos && pos < NBITS);
        IType mask = (1 << pos);
        if (val) {
            bits |= (mask);
        } else {
            bits &= ~(mask);
        }
    }
    void assign(IType n) {     // Replaces the underlying integer with n
        bits = n;
    }
    void toggle(int pos) {     // Flips the bit at position pos
        assert(0 <= pos && pos < NBITS);
        bits ^= (1 << pos);
    }
    void toggle() {            // Flips all bits
        bits ^= ~(0);
    }
    void shift(int n) {        // If n > 0, shifts bits right n places; if n < 0, shifts left
        if (n > 0) {
            bits >>= n;
        } else if (n < 0) {
            bits <<= -n;
        }
    }
    void rotate(int n) {       // If n > 0, rotates right n places; if n < 0, rotates left
        n = n % 64;
        IType save;
        // save the bits to be rotated out, do the shift, then replace the saved bits on the other end
        if (n > 0) {
            save = IType(bits & ((1 << n) - 1));
            save = IType(save << (size() - n));
            bits >>= n;
            bits |= save;
        } else if (n < 0) {
            save = IType(bits & (((1 << n) - 1) << (size() + n)));
            save = IType(save >> (size() + n));
            bits <<= -n;
            bits |= save;
        }
    }
    int ones() const {         // Returns how many bits are set in the underlying integer
        int total = 0;
        for (int i = 0; i < size(); i++) {
            if (this->at(i)) {
                total++;
            }
        }
        return total;
    }
    int zeroes() const {      // Returns how many bits are reset in the underlying integer
        return NBITS - ones();
    }
    IType to_int() const {
        return bits;
    }
    friend bool operator==(const Bits& b1, const Bits& b2) {
        return b1.bits == b2.bits;
    }
    friend bool operator!=(const Bits& b1, const Bits& b2) {
        return b1.bits != b2.bits;
    }
    friend std::ostream& operator<<(std::ostream& os, const Bits& b) {
        return os << std::bitset<NBITS>(b.bits);    // Be sure to #include <bitset>
    }
};

#endif