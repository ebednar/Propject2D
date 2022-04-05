#pragma once

#include <chrono>

class Timer
{
public:
	void	set_start_point();
	void	get_time_point();
	void	get_delta_time();
private:
	std::chrono::steady_clock::time_point start_frame;
	std::chrono::steady_clock::time_point end_frame;
};