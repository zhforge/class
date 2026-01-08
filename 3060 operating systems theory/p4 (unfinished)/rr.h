#pragma once
#include <string>
#include <vector>
#include "scheduler.h"

class RR : public SchedulingAlgorithm { // round robin scheduling algorithm
private:
	int _time_quantum = 100; // in ms
public:
	std::string getTimeAverages(const std::vector<int>& arrival_times, const std::vector<int>& burst_times) override;

	void setQuantum(const int q) { this->_time_quantum = q; };
	int getQuantum() { return this->_time_quantum; };
};
