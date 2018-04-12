#include "animWait.h"

AnimWait::AnimWait(double duration)
{
	_progress = 0;
	_duration = duration;
}

void AnimWait::Update()
{
	// Dont update if done
	if (_done)
		return;

	// Update percent
	_progress += 0.016;
	if (_progress >= _duration)
		_done = true;
}
