#include <iostream>
#include <string>
#include <vector>
#include "fifo.h"
#include "sjf.h"
#include "srtf.h"
#include "rr.h"

using namespace std;

int main(int argc, char *argv[]) {
	if (argc > 1) 
		int quantum = stoi(argv[1]);
	vector<int> arrival_times;
	vector<int> burst_times;
	int t;
	
	cin >> t;
	while (cin) {
		arrival_times.emplace_back(t);
		cin >> t;
		burst_times.emplace_back(t);
		cin >> t;
	}

	/*
	cout << "arrival times" << endl;
	for (auto i : arrival_times)
		cout << i << endl;
	cout << "burst times" << endl;
	for (auto i : burst_times)
		cout << i << endl;
	*/

	FIFO f;
	SJF sj;
	SRTF srt;
	RR r;

	cout << f.getTimeAverages(arrival_times, burst_times) << endl;
	cout << sj.getTimeAverages(arrival_times, burst_times) << endl;
	cout << srt.getTimeAverages(arrival_times, burst_times) << endl;

	
	return 0;
}
