#include "animJumpTo.h"

AnimJumpTo::AnimJumpTo(bool jumpToActivePosition, Actor_ptr owner)
{
	// + 0.5f so that they appear above things when they jump
	if (jumpToActivePosition)
		_destination = owner->ActivePosition + Vector3f(0, 0, 0.5f);
	else
		_destination = owner->WaitPosition + Vector3f(0, 0, 0.5f);

	_target = owner;

	// 0.01 so that calculations dont give 0 somewhere
	_progress = 0.01;// -0.40;
}

AnimJumpTo::AnimJumpTo(Vector3f position, Actor_ptr target)
{
	// + 0.5f so that they appear above things when they jump
	_destination = position - Vector3f(0, 0, 0.5f);
	_destination.y -= 0.075f;
	_target = target;

	// 0.01 so that calculations dont give 0 somewhere
	_progress = 0.01;// -0.40;
}

void AnimJumpTo::first_time()
{
	_firstTime = false;

	_initialPos = _target->get_position_ref() - Vector3f(0, 0, 0.5f);
	_speed = (_destination - _initialPos) / 30.0f;
	_speed.z = 0;

	// Square root and all to make shorter jumps not be too long or too short
	// Divide by 8 so long jumps can be short-ish
	_duration = sqrtf(_destination.Distance2D(_initialPos) / (8.0f * 64.0f));

	_target->_specialAnimation = true;
	_target->_forceAnimation = true;
	_target->_animation = AE_Jump;

	auto data = Animation::GetMetaData(_target->get_texture()).data;
	_target->_row = data.at(AE_Jump)._position;
	_length = abs(data.at(AE_Jump)._end - data.at(AE_Jump)._start) + 1;
	_start = data.at(AE_Jump)._start;
}

void AnimJumpTo::Update()
{
	// Dont update if done
	if (_done)
		return;

	if (_firstTime)
		first_time();

	// Set position
	Vector3f result = MathUtils::FindPositionInParabola((float)(_progress / _duration), _initialPos, _destination);
	_target->set_position(result);

	// Set sprite
	int sprite = 0;
	if (_speed.x > 0)
		sprite = _length / 4 + (int)(((_progress) / _duration) * (double)_length / 2);
	else
		sprite = _length / 2 - (int)(((_progress) / _duration) * (double)_length / 2);

	_target->_sprite = sprite;

	// Update percent
	_progress += 0.025;
	if (_progress >= _duration)
	{
		_done = true;
		_target->set_position(_destination);

		//auto& data = Animation::GetMetaData(_target->get_texture()).data;
		//_target->_row = data.at(AE_Jump)._position;
		//_target->SetAnimation(AE_)

		_target->_specialAnimation = false;
		_target->_forceAnimation = false;
		_target->_sprite = 0;
		_target->Update();
	}
}
