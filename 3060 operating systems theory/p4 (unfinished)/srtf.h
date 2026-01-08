#pragma once
#include <string>
#include <vector>
#include "scheduler.h"

class SRTF : public SchedulingAlgorithm { // shortest runtime first scheduling algorithm
public:
	std::string getTimeAverages(const std::vector<int>& arrival_times, const std::vector<int>& burst_times) override;
};
