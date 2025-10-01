#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

static ostringstream OSS;

int readFile(string fileName) {
    ifstream inFS;
    string out;
    int total = 0;

    fileName = fileName.substr(1);

    inFS.open(fileName);
    if (!inFS.is_open()) {
        return 0;
    }

    inFS >> out;
    while (!inFS.fail()) {
        if (out[0] == '@') {
            total += readFile(out);
        }
        else {
            OSS << out << endl;
            total++;
        }
        inFS >> out;
    }

    inFS.close();
    return total;
}

int main(int argc, char* argv[]) {
    string word;
    string out;
    int total = 0;

    for (int i = 1; i < argc; i++) {
        word = argv[i];
        if (word[0] == '@') {
            total += readFile(word);
        }
        else {
            OSS << word << endl;
            total++;
        }
    }

    out = OSS.str();
    cout << total << " items:" << endl << endl << out;
    

    return 0;
}