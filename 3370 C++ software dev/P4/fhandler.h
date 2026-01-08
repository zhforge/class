#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;

class FileHandler {
public:
    void parseIni(std::ifstream& iniFile, float* iniArgs) {
        // take the arguments from specified .ini file
        // and enforce the specific parameter inclusions
        /* e.g.
        vt=100
        width=100
        pulse_delta=15
        drop_ratio=0.75
        below_drop_ratio=4
        */

        std::vector<std::string> lines;
        std::string line;
        std::unordered_map<std::string, float> iniSettings;

        // get the file's contents
        while (std::getline(iniFile, line))
            lines.emplace_back(line);
        
        if (lines.size() != 5) {
            throw std::runtime_error("Must contain exactly 5 parameters");
        }

        // format lines into a map
        try {
            std::transform(lines.begin(), lines.end(), std::inserter(iniSettings, iniSettings.end()), 
                [](std::string l) {
                    size_t s = l.find('=');
                    return std::make_pair(l.substr(0, s), stof(l.substr(s+1))); 
                });
        }
        catch (std::invalid_argument& e) {
            throw std::runtime_error("Invalid argument type - make sure .ini parameter values are numbers");
        }

        // check for correct parameters
        if (!(iniSettings.contains("vt") && iniSettings.contains("width") && iniSettings.contains("pulse_delta") && iniSettings.contains("drop_ratio"), iniSettings.contains("below_drop_ratio")))
            throw std::runtime_error("Incorrect parameters - include only and exactly the correct parameters");

        // return the settings
        iniArgs[0] = iniSettings["vt"];
        iniArgs[1] = iniSettings["width"];
        iniArgs[2] = iniSettings["pulse_delta"];
        iniArgs[3] = iniSettings["drop_ratio"];
        iniArgs[4] = iniSettings["below_drop_ratio"];
    }

    std::vector<std::string> getDatFiles() {
        // get all the .dat files in the local directory

        std::string fileName;
        std::vector<fs::directory_entry> datFileEntries;
        std::vector<std::string> datFiles;

        // get the dat file entries
        std::copy_if(fs::directory_iterator("."), fs::directory_iterator(), std::back_inserter(datFileEntries),
            [](const fs::directory_entry& entry){
                return entry.is_regular_file() && entry.path().extension() == ".dat";
            });

        // format to strings
        std::transform(datFileEntries.begin(), datFileEntries.end(), std::back_inserter(datFiles),
            [](const fs::directory_entry& entry){
                return entry.path().filename().string();
            });

        return datFiles;
    }

    std::vector<int> getDataPoints(std::ifstream& datFile) {
        // put all the data points from a .dat file into a vector
        // also negate them

        std::vector<int> dataPoints;
        std::string line;
        while (std::getline(datFile, line))
            dataPoints.emplace_back(-std::stoi(line));
        
        return dataPoints;
    }
};