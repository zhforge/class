#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <span>
#include <numeric>
#include <functional>
#include <iterator>

class DataHandler {
private:
    std::vector<int> _getMovingWindowAverage(const std::span<int> dataset) {
        // get the moving window average for a dataset

        std::vector<int> mwa;
        mwa.resize(dataset.size());
        std::copy(dataset.begin(), dataset.begin()+3, mwa.begin());
        
        auto m = mwa.begin() + 3;
        for (auto i = dataset.begin()+3; i < dataset.end()-3; ++i) {
            *m = (i[-3] + 2*i[-2] + 3*i[-1] + 3*i[0] + 3*i[1] + 2*i[2] + i[3]) / 15;
            ++m;
        }

        std::copy(dataset.end()-3, dataset.end(), mwa.end()-3);
        return mwa;
    }

public:
    std::vector<int> getPulseLocations(const std::span<int>& dataset, float* settings) {
        // get the starting positions of each pulse in the dataset

        float vt = settings[0]; // voltage threshold
        float pulse_delta = settings[2];
        float drop_ratio = settings[3];
        float below_drop_ratio = settings[4];

        std::vector<int> mwa = this->_getMovingWindowAverage(dataset);
        std::vector<int> pulseLocations;
        
        // find possible pulse locations
        bool pulsing = false;
        for (auto i = mwa.begin(); i < mwa.end()-2; ++i) {
            if (pulsing) {
                if (i[1] < i[0])
                    pulsing = false;
            }
            else {
                if (i[2] - i[0] > vt) {
                    pulsing = true;
                    pulseLocations.emplace_back(std::distance(mwa.begin(), i));
                }
            }
        }
        
        // find and account for piggyback pulses
        std::vector<int> pulsesToRemove;
        for (auto i = pulseLocations.begin(); i < pulseLocations.end()-1; ++i) {
            if (i[0]+pulse_delta >= i[1]) {
                auto pulsePeakLocation = std::max_element(mwa.begin()+i[0], mwa.begin()+i[1]);
                int pulsePeakHeight = *pulsePeakLocation;
                int belowDropCount = std::count_if(pulsePeakLocation+1, mwa.begin()+i[1], 
                    [&drop_ratio, &pulsePeakHeight](const int& x){
                        return x < drop_ratio*pulsePeakHeight;
                    });
                if (belowDropCount > below_drop_ratio) {
                    pulsesToRemove.emplace_back(i[0]);
                    std::cout << "Found piggyback at " << i[0] << std::endl;
                }
            }
        }
        for (int i : pulsesToRemove)
            pulseLocations.erase(std::remove(pulseLocations.begin(), pulseLocations.end(), i), pulseLocations.end());

        return pulseLocations;
    }

    std::vector<int> getPulseAreas(const std::span<int>& dataset, const std::span<int>& pulseLocations, float* settings) {
        // get the areas of each pulse

        float width = settings[1];
        std::vector<int> pulseAreas;

        for (auto i = pulseLocations.begin(); i < pulseLocations.end(); ++i) {
            if (i != pulseLocations.end()-1 && i[0] + width > i[1]) { // check for close pulses
                pulseAreas.emplace_back(std::reduce(dataset.begin()+i[0], dataset.begin()+i[1]));
            }
            else if (*i + width >= dataset.size()) { // check for pulses near the end
                pulseAreas.emplace_back(std::reduce(dataset.begin()+i[0], dataset.begin()+i[0]+width));
            }
            else {
                pulseAreas.emplace_back(std::reduce(dataset.begin()+i[0], dataset.begin()+i[0]+width));
            }
        }

        return pulseAreas;
    }
};