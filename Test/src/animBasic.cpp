#include "animBasic.h"

AnimBasic::AnimBasic(Anim_Enum anim, Actor_ptr target, double seconds)
{
	_target = target;
	_anim = anim;
	_duration = seconds;
	_progress = 0;

	_done = false;
}

void AnimBasic::first_time()
{
	_prevState = _target->_row;
	_target->_specialAnimation = true;
	_target->_forceAnimation = true;

	auto& data = Animation::GetMetaData(_target->GetTexture()).data;
	_target->_row = data.at(AE_Attack)._position;
	_length = abs(data.at(AE_Attack)._end - data.at(AE_Attack)._start) + 1;
	_start = data.at(AE_Attack)._start;

	_firstTime = false;
}


void AnimBasic::Update()
{
	// Dont update if done
	if (_done)
		return;

	if (_firstTime)
		first_time();

	// Set position
	_target->_sprite = _start + (int)((_progress / _duration) * (double)_length);

	// Update percent
	_progress += 0.016f;
	if (_progress >= _duration)
	{
		_done = true;
		_target->_specialAnimation = false;
		_target->_forceAnimation = false;
		_target->_sprite = 0;
		_target->_row = _prevState;
	}
}
