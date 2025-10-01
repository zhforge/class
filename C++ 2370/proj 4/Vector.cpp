#include "Vector.h"
#include <cstddef>
#include <stdexcept>
#include <iostream>

using namespace std;

// Private
void Vector::grow() {
    // called whenever an object is added to a full vector
    // create new array with 1.6 times capacity, copy everything over
    // cout "grow" every time this is called

    cout << "grow" << endl;

    capacity *= 1.6;
    int* new_vec = new int[capacity];

    for (int i = 0; i < n_elems; i++) {
        *(new_vec + i) = *(data_ptr + i);
    }

    data_ptr = new_vec;
    delete new_vec;
}

// Object Mgt.
Vector::Vector() {                              // Constructor
    // create a vector with capacity CHUNK
    data_ptr = new int[CHUNK];
    capacity = CHUNK;
    n_elems = 0;
}

Vector::Vector(const Vector& v) {               // Copy constructor
    capacity = v.capacity;
    n_elems = v.n_elems;
    data_ptr = new int[capacity];

    for (int i = 0; i < n_elems; i++) {
        *(data_ptr + i) = *(v.data_ptr + i);
    }
}

Vector& Vector::operator=(const Vector& v) {    // Copy assignment operator
    if (this != &v) {
        delete data_ptr;
        capacity = v.capacity;
        n_elems = v.n_elems;
        data_ptr = new int[capacity];

        for (int i = 0; i < n_elems; i++) {
            *(data_ptr+ i ) = *(v.data_ptr + i);
        }
    }

    return *this;
}

Vector::~Vector() {                             // Destructor
    delete data_ptr;
}

// Accessors
int Vector::front() const {         // Return the int in position 0, if any
    if (n_elems == 0) {
        throw range_error("Array is empty (front)");
    }

    return *data_ptr;
}

int Vector::back() const {          // Return last element (position n_elems-1)
    if (n_elems == 0) {
        throw range_error("Array is empty (back)");
    }

    return *(data_ptr + n_elems - 1);
}

int Vector::at(size_t pos) const {  // Return element in position "pos" (0-based)
    if (!(0 <= pos && pos < n_elems)) {
        throw range_error("Cannot access value out of array range (at)");
    }

    return *(data_ptr + pos);
}

size_t Vector::size() const {       // Return n_elems
    return n_elems;
}

bool Vector::empty() const {        // Return n_elems == 0
    return n_elems == 0;
}

// Mutators
int& Vector::operator[](size_t pos) {        // Same as at but no bounds checking
    return *(data_ptr + pos);
}

void Vector::push_back(int item) {           // Append a new element at the end of the array
    if (n_elems == capacity) {
        this->grow();
    }

    *(data_ptr + n_elems) = item;
    n_elems += 1;
}

void Vector::pop_back() {                    // --n_elems (nothing else to do; returns nothing)
    if (n_elems == 0) {
        throw range_error("Array is empty (pop_back)");
    }

    n_elems--;
}

void Vector::erase(size_t pos) {             // Remove item in position pos and shuffles following items left
    if (!(0 <= pos && pos < n_elems)) {
        throw range_error("Cannot access value out of array range (erase)");
    }

    for (int i = pos; i < n_elems; i++) {
        *(data_ptr + i) = *(data_ptr + i + 1);
    }
    n_elems--;
}

void Vector::insert(size_t pos, int item) {  // Shuffle items right to make room for a new element
    if (!(0 <= pos && pos < n_elems + 1)) {
        throw range_error("Cannot access value out of array range (insert)");
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

void Vector::clear() {                       // n_elems = 0 (nothing else to do; keep the current capacity)
    n_elems = 0;
}

// Iterators
int* Vector::begin() {  // Return a pointer to 1st element, or nullptr if n_elems == 0
    if (n_elems == 0) {
        return nullptr;
    }

    return data_ptr;
}

int* Vector::end() {    // Return a pointer to 1 past last element, or nullptr if n_elems == 0
    if (n_elems == 0) {
        return nullptr;
    }

    return data_ptr + n_elems;
}

// Comparators
bool Vector::operator==(const Vector& v) const {
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

bool Vector::operator!=(const Vector& v) const {
    return !(this->operator==(v));
}