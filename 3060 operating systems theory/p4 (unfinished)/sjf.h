#pragma once
#include <string>
#include <vector>
#include "scheduler.h"

class SJF : public SchedulingAlgorithm { // shortest job first scheduling algorithm
public:
	std::string getTimeAverages(const std::vector<int>& arrival_times, const std::vector<int>& burst_times) override;
};
