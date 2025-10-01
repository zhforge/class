/*
During this project, I had a learning experience about a very simple but very
important problem, which is that you never want to start a project the day it's due.
But to be honest, it taught me a lot about the practical use of pointers and dynamic
memory. It's very, very important to keep the data structures you're using 100% clear
and understandable, because it's a pain to debug something you can't actually see.
When it comes down to it, though, it's also conceptually quite simple, and there's
often no need to overcomplicate a solution to a problem. It also makes commenting
easy, because the idea of what a function is doing is usually conceptually easy to
understand. So, to sum it up, start early and don't overcomplicate things.
*/


#include <iostream>
#include <string>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include "CircBuf.h"

using namespace std;


CircBuf::CircBuf(size_t reserve) {
    // allocate enough space in chunk sizes to fit reserved data
    buffer_length = ((reserve + CHUNK - 1) / CHUNK) * CHUNK;  // integer division to round up to the nearest chunk
    buffer = new char[buffer_length];
}

CircBuf::~CircBuf() {
    delete[] buffer;
}

void CircBuf::_resize(size_t new_size) {
    // either grow the buffer to be able to hold a new size
    // or shrink it to the minimal capacity in chunks for the existing size
    if (new_size == 0 || new_size < size()) { // disallow shrinking capacity below size (overflow)
        new_size = size();
    }
    size_t old_capacity = capacity();
    
    buffer_length = ((new_size + CHUNK - 1) / CHUNK) * CHUNK; // integer division to round up to the nearest chunk
    char* newBuffer = new char[buffer_length];

    if (old_capacity != 0) {    // check for division by 0
        // iterate across buffer from tail to head, move data into newBuffer
        for (size_t i = 0; i < (buffer_head - buffer_tail) % old_capacity; i++) {  
            newBuffer[i] = buffer[(buffer_tail + i) % old_capacity];
        }
    }
    buffer_head = new_size;
    buffer_tail = 0;
    delete[] buffer;
    buffer = newBuffer;
}

size_t CircBuf::size() {
    return buffer_size;
}

size_t CircBuf::capacity() {
    return buffer_length;
}


void CircBuf::insert(char input_char) {
    // prevent overflow
    if (size() == capacity()) {
        _resize(size() + 1);    // buffer is resized to the next *ideal* size, updating buffer_head
        buffer_head -= 1;       // but the size hasn't *actually* increased yet, so this is a temporary adjustment
   }                        

    // insert a character at the buffer head
    buffer[buffer_head] = input_char;
    buffer_size += 1;
    // move the buffer head forward
    buffer_head += 1;
    if (buffer_head >= capacity()) {   // or wrap around from the end
        buffer_head = 0;
    }
}

void CircBuf::insert(const char* input_string, size_t sz) {
    // prevent overflow
    if (size() + sz > capacity()) {
        _resize(size() + sz);   // buffer is resized to the next *ideal* size, including updating buffer_head
        buffer_head -= sz;      // but the size hasn't *actually* increased yet, so this is a temporary adjustment
    }                       
    
    // get each character from the input string
    for (size_t i = 0; i < sz; i++) {
        // insert a character at the buffer head
        buffer[buffer_head] = input_string[i];
        buffer_size += 1;
        // move the buffer head forward
        buffer_head += 1;
        if (buffer_head >= capacity()) {   // or wrap around from the end
            buffer_head = 0;
        }
    }
}

void CircBuf::insert(const string& input_string) {
    // prevent overflow
    if (size() + input_string.length() > capacity()) {
        _resize(size() + input_string.length());   // buffer is resized to the next *ideal* size, including updating buffer_head
        buffer_head -= input_string.length();      // but the size hasn't *actually* increased yet, so this is a temporary adjustment
    }                       
    
    // get each character from the input string
    for (size_t i = 0; i < input_string.length(); i++) {
        // insert a character at the buffer head
        buffer[buffer_head] = input_string[i];
        buffer_size += 1;
        // move the buffer head forward
        buffer_head += 1;
        if (buffer_head >= capacity()) {   // or wrap around from the end
            buffer_head = 0;
        }
    }
}

char CircBuf::get() {
    char output_char = buffer[buffer_tail];
    
    // prevent underflow
    if (size() == 0) {
        return '\0';
    }

    // move the tail forward
    buffer_tail += 1;
    buffer_size -= 1;
    if (buffer_tail >= capacity()) {   // or wrap around from the end
        buffer_tail = 0;
    }

    return output_char;
}

string CircBuf::get(size_t sz) {
    string output_string = "";

    // prevent underflow
    if (size() == 0) {
        return "";
    }

    // if more characters are requested than exist just output them all
    if (sz > size()) {
        sz = size();
    }

    // get sz amount of characters as a string
    for (size_t i = 0; i < sz; i++) {
        // get character from the tail
        output_string += buffer[buffer_tail];
        // move the buffer tail forward
        buffer_tail += 1;
        buffer_size -= 1;
        if (buffer_tail >= capacity()) {   // or wrap around from the end
            buffer_tail = 0;
        }
    }

    return output_string;
}

string CircBuf::flush() {
    string output_string;
    size_t sz = size();

    // get all characters from the buffer
    for (size_t i = 0; i < sz; i++) {
        // get character from the tail
        output_string += buffer[buffer_tail];
        // move the buffer tail forward
        buffer_tail += 1;
        buffer_size -= 1;
        if (buffer_tail >= capacity()) {   // or wrap around from the end
            buffer_tail = 0;
        }
    }

    shrink();
    return output_string;
}

string CircBuf::examine() {
    string output_string = "[";

    if (buffer_tail < buffer_head) {    // not wrapped around
        for (size_t i = 0; i < buffer_tail; i++) {              // empty
            output_string += "-";
        }
        for (size_t i = buffer_tail; i < buffer_head; i++) {    // buffer data
            output_string += buffer[i];
        }
        for (size_t i = buffer_head; i < capacity(); i++) {     // empty
            output_string += "-";
        }
    }
    else {                              // wrapped around
        for (size_t i = 0; i < buffer_head; i++) {              // buffer data
            output_string += buffer[i];
        }
        for (size_t i = buffer_head; i < buffer_tail; i++) {    // empty
            output_string += "-";
        }
        for (size_t i = buffer_tail; i < capacity(); i++) {     // buffer data
            output_string += buffer[i];
        }
    }

    output_string += "]";
    return output_string;
}

void CircBuf::shrink() {
    _resize();
}