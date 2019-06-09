#include "animMoveTo.h"

AnimMoveTo::AnimMoveTo(Vector3f position, Actor_ptr target, double duration) : AnimMoveTo(position, target)
{
	_duration = duration;
}

AnimMoveTo::AnimMoveTo(Vector3f position, Actor_ptr target)
{
	_destination = position;
	_target = target;
	_initialPos = target->get_position_ref();
	_speed = (_destination - _initialPos) / 30.0f;
	_speed.z = 0;
	_done = false;
	_progress = 0;
	_duration = 0.4;
	//_prevDir = _target->_animation;
	_target->SetAnimation(_target->GetMoveDirection(_target->_default_animation), _target->get_texture());
	_target->_forceAnimation = true;
}

void AnimMoveTo::Update()
{
	// Dont update if done
	if (_done)
		return;

	// Set position
	Vector3f result = (_destination - _initialPos) * _progress / _duration;
	_target->set_position(result + _initialPos);

	// Update percent
	_progress += 0.025;
	if (_progress >= _duration || _target->get_position_ref() == _destination)
	{
		_done = true;
		_target->set_position(_destination);
		//_target->SetAnimation(_prevDir, _target->get_texture());
		_target->_forceAnimation = false;
		_target->Update();
	}
}