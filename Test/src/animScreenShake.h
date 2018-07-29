#ifndef ANIM_SCREEN_SHAKE_H__
#define ANIM_SCREEN_SHAKE_H__

#include "battleAnimation.h"
#include "vector3f.h"
#include "actor.h"

class AnimScreenShake : public BattleAnimation
{
public:
	AnimScreenShake(double intensity = 0.2, double duration = 0.2);
	virtual void Update();

public:
	double _intensity;

private:
	int m_progress;
};

#endif // !BATTLE_ANIMATION_H__
