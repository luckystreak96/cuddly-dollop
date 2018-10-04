#include "animXpBar.h"

AnimXpBar::AnimXpBar(HudHealthBar* bar, int start, int end) : _progress(start), _end(end), _bar(bar)
{
	_async = true;
	_lerp.Acceleration = 0.005f;
	_lerp.Amount = 0.025f;
	_lerp.MaxVelocity = 1.000f;
	_lerp.MinVelocity = 0.025f;
}

void AnimXpBar::Update()
{
	// Dont update if done
	if (_done)
		return;

	Vector3f exp = _lerp.Lerp(Vector3f(_progress), Vector3f(_end));
	_progress = exp.x;

	if (_bar->UpdateExpAnimation(exp.x))
		_done = true;
}
