#ifndef TIMER_H
#define TIMER_H

#include <chrono>

/*
* Class used for scoped based timer to measure performace of functions
*/
class Timer {
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> StartTimepoint;
public:
	Timer() {
		this->StartTimepoint = std::chrono::high_resolution_clock::now();
	}
	~Timer() {
		Stop();
	}

	void Stop() {
		auto endTimepoint = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(StartTimepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
	
		auto duration = end - start;

		double ms = duration * 0.001;

		std::cout << "Duration: " << ms << " ms\n";
	}
};

#endif
