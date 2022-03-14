#include "animator.h"

void	Animator::init(int x, int y)
{
	width = x;
	height = y;
	animations[1].init(3, 3);
	set_animation(0);
}

void	Animator::set_animation(int animation_number)
{
	current_animation = &animations[animation_number];
	current_numb = animation_number;
}

void	Animator::play_animation()
{
	current_animation->play();
}

bool	Animator::frame_changed()
{
	return current_animation->frame_changed;
}

int		Animator::get_frame()
{
	return current_animation->current_frame;
}