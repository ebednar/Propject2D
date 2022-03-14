#pragma once
#include <vector>

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
	std::vector<Frame> frames;
	int		frame_count = 0;
	int		xpad = 0;
	int		ypad = 0;
	int		current_frame = 0;
	int		width = 120;
	int		height = 80;
	float	duration = 3.0f;
	bool	frame_changed = false;
	void	init(int frame_count, int duration);
	void	clear();
	void	play();
private:
	float	current_time = 0.0f;
};