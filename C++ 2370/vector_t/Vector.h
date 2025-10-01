#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <stdexcept>
using std::size_t;

template<typename T>
class Vector {
    enum {CHUNK = 10};
    size_t capacity;    // Size of the current array allocation (total number of items, in use or not)
    size_t n_elems;     // Number of item spaces currently in use, starting from position 0
    T* data_ptr;      // Pointer to the heap array

    void grow() {
        // called whenever an object is added to a full vector
        // create new array with 1.6 times capacity, copy everything over

        capacity *= 1.6;
        T* new_vec = new T[capacity];

        for (int i = 0; i < n_elems; i++) {
            *(new_vec + i) = *(data_ptr + i);
        }

        data_ptr = new_vec;
    }
public:
    // Object Mgt.
    Vector() {                              // Constructor
        // create a vector with capacity CHUNK
        data_ptr = new T[CHUNK];
        capacity = CHUNK;
        n_elems = 0;
    }
    Vector(const Vector& v) {               // Copy constructor
        capacity = v.capacity;
        n_elems = v.n_elems;
        data_ptr = new T[capacity];

        for (int i = 0; i < n_elems; i++) {
            *(data_ptr + i) = *(v.data_ptr + i);
        }
    }
    Vector<T>& operator=(const Vector& v) {    // Copy assignment operator
        if (this != &v) {
            delete [] data_ptr;
            capacity = v.capacity;
            n_elems = v.n_elems;
            data_ptr = new T[capacity];

            for (int i = 0; i < n_elems; i++) {
                *(data_ptr+ i ) = *(v.data_ptr + i);
            }
        }

        return *this;
    }
    ~Vector() {                             // Destructor
        delete [] data_ptr;
    }

    // Accessors
    T front() const {         // Return the item in position 0, if any
        if (n_elems == 0) {
            throw std::range_error("Array is empty (front)");
        }

        return *data_ptr;
    }
    T back() const {          // Return last element (position n_elems-1)
        if (n_elems == 0) {
            throw std::range_error("Array is empty (back)");
        }

        return *(data_ptr + n_elems - 1);
    }
    T at(size_t pos) const {  // Return element in position "pos" (0-based)
        if (!(0 <= pos && pos < n_elems)) {
            throw std::range_error("Cannot access value out of array range (at)");
        }

        return *(data_ptr + pos);
    }
    size_t size() const {       // Return n_elems
        return n_elems;
    }
    bool empty() const {        // Return n_elems == 0
        return n_elems == 0;
    }

    // Mutators
    T& operator[](size_t pos) {        // Same as at but no bounds checking
        return *(data_ptr + pos);
    }
    void push_back(T item) {           // Append a new element at the end of the array
        if (n_elems == capacity) {
            this->grow();
        }

        *(data_ptr + n_elems) = item;
        n_elems += 1;
    }
    void pop_back() {                    // --n_elems (nothing else to do; returns nothing)
        if (n_elems == 0) {
            throw std::range_error("Array is empty (pop_back)");
        }

        n_elems--;
    }
    void erase(size_t pos) {             // Remove item in position pos and shuffles following items left
        if (!(0 <= pos && pos < n_elems)) {
            throw std::range_error("Cannot access value out of array range (erase)");
        }

        for (int i = pos; i < n_elems; i++) {
            *(data_ptr + i) = *(data_ptr + i + 1);
        }
        n_elems--;
    }
    void insert(size_t pos, T item) {  // Shuffle items right to make room for a new element
        if (!(0 <= pos && pos < n_elems + 1)) {
            throw std::range_error("Cannot access value out of array range (insert)");
        }

        if (n_elems == capacity) {
            this->grow();
        }

        for (int i = n_elems; i > pos; i--) {
            *(data_ptr + i) = *(data_ptr + i - 1);
        }
        *(data_ptr + pos) = item;
        n_elems++;
    }
    void clear() {                     // n_elems = 0 (nothing else to do; keep the current capacity)
        n_elems = 0;
    }


    // Iterators
    T* begin() {  // Return a pointer to 1st element, or nullptr if n_elems == 0
        if (n_elems == 0) {
            return nullptr;
        }

        return data_ptr;
    }
    T* end() {    // Return a pointer to 1 past last element, or nullptr if n_elems == 0
        if (n_elems == 0) {
            return nullptr;
        }

        return data_ptr + n_elems;
    }

    // Comparators
    bool operator==(const Vector& v) const {
        if (this->size() != v.size()) {
            return false;
        }

        for (int i = 0; i < v.size(); i++) {
            if (this->at(i) != v.at(i)) {
                return false;
            }
        }

        return true;
    }
    bool operator!=(const Vector& v) const {
        return !(this->operator==(v));
    }
};

#endif