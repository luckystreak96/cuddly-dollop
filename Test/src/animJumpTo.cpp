#include "animJumpTo.h"

AnimJumpTo::AnimJumpTo(Vector3f position, Actor_ptr target)
{
	// + 0.5f so that they appear above things when they jump
	_destination = position - Vector3f(0, 0, 0.5f);
	_destination.y -= 0.075f;
	_target = target;
	// + 0.5f so that they appear above things when they jump
	_initialPos = target->_Graphics->GetPosRef() - Vector3f(0, 0, 0.5f);
	_speed = (_destination - _initialPos) / 30.0f;
	_speed.z = 0;
	// 0.01 so that calculations dont give 0 somewhere
	_progress = 0.01;// -0.40;

	// Square root and all to make shorter jumps not be too long or too short
	// Divide by 8 so long jumps can be short-ish
	_duration = sqrtf(_destination.Distance2D(_initialPos) / 8.0f);

	_target->_Graphics->_specialAnimation = true;
	_target->_Graphics->_forceAnimation = true;
	auto& data = Animation::GetMetaData(_target->_Graphics->GetTexture()).data;
	_target->_Graphics->_row = data.at(AE_Jump)._position;
	_length = abs(data.at(AE_Jump)._end - data.at(AE_Jump)._start) + 1;
	_start = data.at(AE_Jump)._start;
}

void AnimJumpTo::Update()
{
	// Dont update if done
	if (_done)
		return;

	// Set position
	Vector3f result = MathUtils::FindPositionInParabola(_progress / _duration, _initialPos, _destination);
	_target->_Graphics->SetPhysics(result, Vector3f());

	// Set sprite
	int sprite = 0;
	if (_speed.x > 0)
		sprite = _length / 4 + (int)(((_progress) / _duration) * (double)_length / 2);
	else
		sprite = _length / 2 - (int)(((_progress) / _duration) * (double)_length / 2);

	_target->_Graphics->_sprite = sprite;

	// Update percent
	_progress += 0.025;
	if (_progress >= _duration)
	{
		_done = true;
		_target->_Graphics->SetPhysics(_destination, Vector3f());
		_target->_Graphics->_specialAnimation = false;
		_target->_Graphics->_forceAnimation = false;
		//_target->_Graphics->_sprite = 0;
	}
}