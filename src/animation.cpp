#include "animation.h"
#include "model.h"

void Animation::init(unsigned int texture_id, int frame_count, float duration)
{
	this->frame_count = frame_count;
	this->duration = duration;
	this->texture_id = texture_id;
	start_time = std::chrono::steady_clock::now();
}

void Animation::clear()
{
	current_frame = 0;
	current_time = 0.0f;
	std::chrono::steady_clock::now();
}

void Animation::play()
{
	frame_changed = false;
	std::chrono::duration<double> elapsed_seconds = std::chrono::steady_clock::now() - start_time;
	if (elapsed_seconds.count() >= (double)duration / (double)frame_count)
	{
		frame_changed = true;
		current_frame++;
		if (current_frame == frame_count)
			current_frame = 0;
		start_time = std::chrono::steady_clock::now();
	}

	xpad = current_frame;
}
