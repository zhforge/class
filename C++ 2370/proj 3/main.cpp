#include <iostream>
using namespace std;

#include "bits.h"

int main() {
    Bits x(63);

    cout << x.ones() << endl;
    
    cout << x;

    return 0;
}