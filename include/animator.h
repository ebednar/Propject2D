#pragma once

#include "animation.h"

class Animator
{
public:
	Animation	animations[3];
	Animation*	current_animation = nullptr;
	int		current_numb = 0;
	int		total_numb = 2;
	int		width;
	int		height;
public:
	void	init(int x, int y);
	void	set_animation(int animation_number);
	void	play_animation();
	bool	frame_changed();
	int		get_frame();
private:

};