#include "animBonusEffect.h"

AnimBonusEffect::AnimBonusEffect(Actor_ptr owner, Actor_ptr target, double delay)
{
	_duration = delay;
	_owner = owner;
	_target = target;
	_done = false;
	_progress = 0;
	_async = true;
}

void AnimBonusEffect::Update()
{
	// Dont update if done
	if (_done)
		return;

	// Update percent
	_progress += 0.025;
	if (_progress >= _duration)
	{
		_owner->_Fighter->ApplyBonusDamage(_target);
		_done = true;
	}
}
