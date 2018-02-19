#include "animJumpTo.h"

AnimJumpTo::AnimJumpTo(Vector3f position, Actor_ptr target)
{
	_destination = position;
	_destination.y -= 0.1f;
	_target = target;
	_initialPos = target->_Graphics->GetPosRef();
	_speed = (_destination - _initialPos) / 30.0f;
	_speed.z = 0;
	_progress = 0;
	_duration = 1;
}

void AnimJumpTo::Update()
{
	// Dont update if done
	if (_done)
		return;

	// Set position
	Vector3f result = MathUtils::FindPositionInParabola(_progress / _duration, _initialPos, _destination);
	_target->_Graphics->SetPhysics(result, Vector3f());

	// Update percent
	_progress += 0.025;
	if (_progress >= _duration)
	{
		_done = true;
		_target->_Graphics->SetPhysics(_destination, Vector3f());
	}
}