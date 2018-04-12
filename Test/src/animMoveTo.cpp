#include "animMoveTo.h"

AnimMoveTo::AnimMoveTo(Vector3f position, Actor_ptr target, double duration) : AnimMoveTo(position, target)
{
	_duration = duration;
}

AnimMoveTo::AnimMoveTo(Vector3f position, Actor_ptr target)
{
	_destination = position;
	_target = target;
	_initialPos = target->_Graphics->GetPosRef();
	_speed = (_destination - _initialPos) / 30.0f;
	_speed.z = 0;
	_done = false;
	_progress = 0;
	_duration = 0.4;
	_prevDir = _target->_Graphics->_animation;
	_target->_Graphics->SetAnimation(_target->_Graphics->GetMoveDirection(_prevDir), _target->Sprite);
	_target->_Graphics->_forceAnimation = true;
}

void AnimMoveTo::Update()
{
	// Dont update if done
	if (_done)
		return;

	// Set position
	Vector3f result = (_destination - _initialPos) * _progress / _duration;
	_target->_Graphics->SetPhysics(result + _initialPos, Vector3f());

	// Update percent
	_progress += 0.025;
	if (_progress >= _duration || _target->_Graphics->GetPosRef() == _destination)
	{
		_done = true;
		_target->_Graphics->SetPhysics(_destination, Vector3f());
		_target->_Graphics->SetAnimation(_prevDir, _target->Sprite);
		_target->_Graphics->_forceAnimation = false;
	}
}