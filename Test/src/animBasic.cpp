#include "animBasic.h"

AnimBasic::AnimBasic(Anim_Enum anim, Actor_ptr target, double seconds)
{
	_target = target;
	_anim = anim;
	_duration = seconds;
	_progress = 0;
	_prevState = _target->_animation;
	_target->_specialAnimation = true;
	_target->_animation = Animation::GetMetaData(_target->GetTexture()).data.at(AE_Attack).position;
	_length = Animation::GetMetaData(_target->GetTexture()).data.at(AE_Attack).numFrames;
	_done = false;
}

void AnimBasic::Update()
{
	// Dont update if done
	if (_done)
		return;

	// Set position
	_target->_sprite = (int)((_progress / _duration) * (double)_length);

	// Update percent
	_progress += 0.034f;
	if (_progress >= _duration)
	{
		_done = true;
		_target->_specialAnimation = false;
		_target->_sprite = 0;
		_target->_animation = _prevState;
	}
}
