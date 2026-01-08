#include <iostream>
#include "bitword.h"

using namespace std;

int main(int argc, char* argv[]) {
    unsigned int x = 37u;
    try {
        if (argc > 1) x = stoi(argv[1]);
    }
    catch (...) {
        x = 37u;
    }

    BitWord b(x);
    cout << "Initial value = " << int(b) << endl;
    cout << b << endl << endl;

    cout << "Extracting first 8 bits: " << b.extract(8, 0) << endl;
    cout << "Extracting 4 bits from position 2: " << b.extract(4, 2) << endl;
    cout << "Extracting 5 bits from position 4: " << b.extract(5, 4) << endl << endl;

    cout << "Setting bit in position 1" << endl;
    b.set(1);
    cout << "Testing bit in position 1: " << b.test(1) << endl;
    cout << b << endl << endl;

    cout << "Resetting bit in position 2" << endl;
    b.reset(2);
    cout << "Testing bit in position 2: " << b.test(2) << endl;
    cout << b << endl << endl;

    cout << "Flipping bit in position 4" << endl;
    b.flip(4);
    cout << "Testing bit in position 4: " << b.test(4) << endl;
    cout << b << endl << endl;


    return 0;
}