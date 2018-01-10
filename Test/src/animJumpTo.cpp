#include "animJumpTo.h"

AnimJumpTo::AnimJumpTo(Vector3f position, Actor_ptr target)
{
	_destination = position;
	_target = target;
	_initialPos = target->GetPosRef();
	_speed = (_destination - _initialPos) / 30.0f;
	_speed.z = 0;
	_done = false;
	_percent = 0;
}

void AnimJumpTo::Update()
{
	// Dont update if done
	if (_done)
		return;

	// Set position
	Vector3f result = MathUtils::FindPositionInParabola(_percent, _initialPos, _destination);
	_target->SetPhysics(result, Vector3f());

	// Update percent
	_percent += 0.034f;
	if (_percent >= 1)
	{
		_done = true;
		_target->SetPhysics(_destination, Vector3f());
	}
}