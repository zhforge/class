#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>

using namespace std;

int main() {
    ostringstream OSS;
    OSS << "hi" << " hello" << endl << "bi" << fixed << setprecision(2) << 2.2345;
    string h = OSS.str();
    cout << h;

    return 0;
}