#include "animBasic.h"

AnimBasic::AnimBasic(Anim_Enum anim, Actor_ptr target, double seconds)
{
	_target = target;
	_anim = anim;
	_duration = seconds;
	_progress = 0;
	_prevState = _target->_Graphics->_row;
	_target->_Graphics->_specialAnimation = true;
	_target->_Graphics->_forceAnimation = true;

	auto& data = Animation::GetMetaData(_target->_Graphics->GetTexture()).data;
	_target->_Graphics->_row = data.at(AE_Attack)._position;
	_length = abs(data.at(AE_Attack)._end - data.at(AE_Attack)._start) + 1;

	_done = false;
}

void AnimBasic::Update()
{
	// Dont update if done
	if (_done)
		return;

	// Set position
	_target->_Graphics->_sprite = (int)((_progress / _duration) * (double)_length);

	// Update percent
	_progress += 0.034f;
	if (_progress >= _duration)
	{
		_done = true;
		_target->_Graphics->_specialAnimation = false;
		_target->_Graphics->_forceAnimation = false;
		_target->_Graphics->_sprite = 0;
		_target->_Graphics->_row = _prevState;
	}
}
