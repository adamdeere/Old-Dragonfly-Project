#pragma once
#include <chrono>
class Timer
{
public:
	Timer();
	~Timer();
	float GetDeltaTime();
private:
	std::chrono::steady_clock::time_point last;
};

