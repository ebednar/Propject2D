#pragma once
#include <vector>
#include <chrono>

class Model;

struct Frame
{
	int x = 0;
	int y = 0;
	int width = 512;
	int height = 512;
};

class Animation
{
public:
	std::vector<Frame>	frames;
	unsigned int		texture_id = 0;
	int		frame_count = 0;
	int		xpad = 0;
	int		ypad = 0;
	int		current_frame = 0;
	int		width = 120;
	int		height = 80;
	float	duration = 3.0f;
	std::chrono::steady_clock::time_point	start_time;
	bool	frame_changed = false;
	void	init(unsigned int texture_id, int frame_count, float duration);
	void	clear();
	void	play();
private:
	float	current_time = 0.0f;
};