#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <sstream>
#include "srtf.h"

//FIXME debug
#include <iostream>

std::string SRTF::getTimeAverages(const std::vector<int>& arrival_times, const std::vector<int>& burst_times) {
	_current_process = 0;
	bool processes_unfinished = true;
	bool process_running = true;
	bool no_next_process = false;
	int next_process_arriving = 1;
	_clock = arrival_times[0];
	std::multimap<int, int> ready_map;
	std::vector<int> current_runtimes(arrival_times.size()); // tracks amt of time processes have ran for

	_start_times.resize(arrival_times.size());
	_finish_times.resize(arrival_times.size());
	_response_times.resize(arrival_times.size());
	_wait_times.resize(arrival_times.size());
	_turnaround_times.resize(arrival_times.size());
	
	while (processes_unfinished) {
		// start (or restart) the next process
		if (current_runtimes[_current_process] == 0)
			_start_times[_current_process] = _clock;
		if (current_runtimes[_current_process] == 0)
			_response_times[_current_process] = _clock - arrival_times[_current_process];
		_finish_times[_current_process] = _clock + (burst_times[_current_process] - current_runtimes[_current_process]);
		process_running = true;

		while (process_running) {
			// skip to next significant event
			// if that's the current process finishing:
			// (or if there are no more arriving processes)
			if (no_next_process || _finish_times[_current_process] <= arrival_times[next_process_arriving]) {
				// add the time elapsed to the wait times of all ready processes
				for (const auto& pair : ready_map)
					_wait_times[pair.second] += _finish_times[_current_process] - _clock;
				// add the time elapsed to the runtime of the current process
				current_runtimes[_current_process] += _finish_times[_current_process] - _clock;

				_clock = _finish_times[_current_process];
				process_running = false;
				_turnaround_times[_current_process] = _clock - arrival_times[_current_process];

				if (!ready_map.empty()) {
					// start the shortest ready process
					auto p = ready_map.begin();
					_current_process = p->second;
					ready_map.erase(p);
				}
				else {
					// check if that was the last process
					if (no_next_process) {
						processes_unfinished = false;
						break;
					}

					// skip to the next arriving process
					_clock = arrival_times[next_process_arriving];
					_current_process = next_process_arriving;
					next_process_arriving++;
				}
			}
			// or if that's the next process arriving:
			else {
				// add the time elapsed to the wait times of all ready processes
				for (const auto& pair : ready_map)
					_wait_times[pair.second] += arrival_times[next_process_arriving] - _clock;
				// add the time elapsed to the runtime of the current process
				current_runtimes[_current_process] += arrival_times[next_process_arriving] - _clock;

				_clock = arrival_times[next_process_arriving];

				// check if arriving process would finish before than the current one would
				if (burst_times[next_process_arriving] < (burst_times[_current_process] - current_runtimes[_current_process])) {
					// put the current process and its remaining runtime into the ready map
					ready_map.insert({(burst_times[_current_process] - current_runtimes[_current_process]), _current_process});
					_current_process = next_process_arriving;
				}
				else {
					ready_map.insert({burst_times[next_process_arriving], next_process_arriving});
				}

				next_process_arriving++;

				// check if there are no more arriving processes
				if (next_process_arriving >= int(arrival_times.size()))
					no_next_process = true;
			}
		}
	}


	//FIXME debug
	std::cout << "CURRENT RUNTIMES - START TIMES - FINISH TIMES - RESPONSE TIMES - WAIT TIMES - TURNAROUND TIMES" << std::endl;
	for (size_t i = 0; i < arrival_times.size(); i++) {
		std::cout << current_runtimes[i] << " " << _start_times[i] << " " << _finish_times[i] << " " << _response_times[i] << " " << _wait_times[i] << " " << _turnaround_times[i] << std::endl;
	}


	_avg_response_time = std::accumulate(_response_times.begin(), _response_times.end(), 0.0) / _response_times.size();
	_avg_wait_time = std::accumulate(_wait_times.begin(), _wait_times.end(), 0.0) / _wait_times.size();
	_avg_turnaround_time = std::accumulate(_turnaround_times.begin(), _turnaround_times.end(), 0.0) / _turnaround_times.size();
	
	std::stringstream ss;
	ss << "Shortest Job First\nAvg. response: " << _avg_response_time << "ms, Avg. wait: " << _avg_wait_time << "ms, Avg. turnaround: " << _avg_turnaround_time << "ms\n";
	return ss.str();
}
