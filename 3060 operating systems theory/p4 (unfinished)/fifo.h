#pragma once
#include <string>
#include <vector>
#include "scheduler.h"

class FIFO : public SchedulingAlgorithm { // first come first served scheduling algorithm
public:
	std::string getTimeAverages(const std::vector<int>& arrival_times, const std::vector<int>& burst_times) override;
};
