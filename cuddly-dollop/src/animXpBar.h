#ifndef ANIM_XP_BAR_H__
#define ANIM_XP_BAR_H__

#include "battleAnimation.h"
#include "vector3f.h"
#include "actor.h"
#include "hudHealthBar.h"

class AnimXpBar : public BattleAnimation
{
public:
	AnimXpBar(HudHealthBar* bar, int start, int end);
	virtual void Update();

public:
	// Animation length
	float _progress;
	int _end;
	Lerper _lerp;
	HudHealthBar* _bar;
};

#endif // !BATTLE_ANIMATION_H__
