#include "entity/player.h"

Player::Player()
{
	
}

void	Player::awake()
{
	scale((float)sub_width / sub_height, 1.0f, 1.0f);
}

void	Player::update()
{

}

void	Player::play_animation()
{
	animator.play_animation();
	if (animator.frame_changed())
	{ 
		set_subtexture(animator.get_frame(), 0, sub_width, sub_height);
		
	}
}
