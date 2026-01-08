#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "fhandler.h"
#include "dhandler.h"

using namespace std;

int main(int argc, char* argv[]) {
    FileHandler fh;
    DataHandler dh;
    vector<string> datFiles;
    unordered_map<string, vector<int>> pulseLocations;  // <.dat file name, vector of pulse locations (ordered sequentially)>
    unordered_map<string, vector<int>> pulseAreas;      // <.dat file name, vector of pulse areas (ordered sequentially)>
    vector<int> dataPoints;
    float iniArgs[5];

    // check for .ini file argument
    if (argc == 1 || argc > 2) { // just one arg
        cerr << "Please provide one .ini file" << endl;
        return 1;
    }
    string cla = argv[1];
    if (cla.length() < 4 || cla.substr(cla.length() - 4) != ".ini") { // check file extension
        cerr << "Please provide a valid .ini file" << endl;
        return 1;
    }
    
    // handle .ini file
    ifstream iniFile(cla);
    if (!iniFile) {
        cerr << "Error opening .ini file: " << cla << endl;
        return 1;
    }
    try {
        fh.parseIni(iniFile, iniArgs);
    }
    catch (runtime_error& r) {
        cerr << "Error parisng .ini file: " << r.what() << endl;
        return 1;
    }
    iniFile.close();

    // handle .dat files
    datFiles = fh.getDatFiles();

    for (string datFile : datFiles) {
        // parse each .dat file
        ifstream df(datFile);
        if (!df) {
        cerr << "Error opening .dat file: " << datFile << endl;
        return 1;
        }
        dataPoints = fh.getDataPoints(df);

        cout << datFile << ":" << endl;
        
        // analyze the data
        pulseLocations[datFile] = dh.getPulseLocations(dataPoints, iniArgs);
        pulseAreas[datFile] = dh.getPulseAreas(dataPoints, pulseLocations[datFile], iniArgs);

        // output results
        for (size_t i = 0; i < pulseLocations[datFile].size(); i++) {
            cout << pulseLocations[datFile][i] << " (" << pulseAreas[datFile][i] << ")" << endl;
        }
        cout << endl;

        df.close();
    }

    return 0;
}