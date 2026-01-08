#pragma once
#include <string>
#include <queue>
#include <vector>

class SchedulingAlgorithm {
protected:
	float _avg_response_time;
	float _avg_wait_time;
	float _avg_turnaround_time;
	int _clock = 0;
	int _current_process = 0;
	std::queue<int> _ready_queue;
	std::vector<int> _start_times;
	std::vector<int> _finish_times;
	std::vector<int> _response_times;
	std::vector<int> _wait_times;
	std::vector<int> _turnaround_times;
public:
	virtual std::string getTimeAverages(const std::vector<int>& arrival_times, const std::vector<int>& burst_times) = 0;
};
