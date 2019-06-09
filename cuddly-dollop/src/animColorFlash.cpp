#include "animColorFlash.h"

AnimColorFlash::AnimColorFlash(Vector3f color, Actor_ptr target, double duration)
{
	_duration = duration;
	_target = target;
	_done = false;
	_progress = 0;
	_color = color;
//	_prevColor = _target->GetVertices()->front().color;
	_async = true;
}

void AnimColorFlash::Update()
{
	// Dont update if done
	if (_done)
		return;

	_target->SetColorAll(_color);

	// Update percent
	_progress += 0.025;
	if (_progress >= _duration)
	{
		_done = true;
		_target->_ColorState = CS_Update;
		//_target->UpdateColor();
	}
}
