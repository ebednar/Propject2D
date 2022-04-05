#include "timer.h"
#include <iostream>

void Timer::set_start_point()
{
	start_frame = std::chrono::steady_clock::now();
}

void Timer::get_time_point()
{
	std::chrono::duration<double> elapsed_seconds = std::chrono::steady_clock::now() - start_frame;
	std::cout << "----- Time elapsed - " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds).count() << "ms -----" << std::endl;
}

void Timer::get_delta_time()
{
	std::chrono::duration<double> elapsed_seconds = std::chrono::steady_clock::now() - start_frame;
	std::cout << "----- Delta time - " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds).count() << " ms -----" << std::endl;
}