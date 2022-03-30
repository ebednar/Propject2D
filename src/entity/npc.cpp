#include "entity/npc.h"

void	Npc::awake()
{
	scale((float)sub_width / sub_height, 1.0f, 1.0f);
}

void	Npc::update()
{
	set_state(state::Idle);
}

void	Npc::play_animation()
{
	/*animator.play_animation();
	if (animator.frame_changed())
	{
		set_subtexture(animator.get_frame(), 0, sub_width, sub_height);
	}*/
}

void	Npc::set_state(state new_state)
{
	if (new_state == current_state)
		return;
	current_state = new_state;
	animator.set_animation((int)new_state);
	material.set_texture(animator.current_animation->texture_id);
}

void	Npc::set_state(int new_state)
{
	if ((state)new_state == current_state)
		return;
	current_state = (state)new_state;
	animator.set_animation(new_state);
	material.set_texture(animator.current_animation->texture_id);
}
