#ifndef ANIM_WAIT_H__
#define ANIM_WAIT_H__

#include "battleAnimation.h"

class AnimWait : public BattleAnimation
{
public:
	AnimWait(double seconds);
	virtual void Update();

public:
};

#endif // !BATTLE_ANIMATION_H__
